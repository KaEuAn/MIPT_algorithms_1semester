#include <iostream>
#include <deque>
#include <vector>
#include <string>
#include <cmath>
#include <complex>
#define M_PI 3.14159265358979323846

void bpf (std::vector< std::complex< double> > & a, bool reverse) {
    if (a.size() == 1)
        return;
    std::vector< std::complex< double> > even(a.size()/2), odd(a.size()/2);
    for (int i = 0; i < a.size()/2; ++i) {
        even[i] = a[2*i];
        odd[i] = a[2*i + 1];
    }
    bpf(even, reverse);
    bpf(odd, reverse);
    double x = 2 * M_PI / a.size();
    if (reverse)
        x *= -1;
    std::complex< double> w(cos(x), sin(x)),  wi(1);
    for (int i = 0; i < a.size() / 2; ++i) {
        a[i] = even[i] + wi * odd[i];
        a[i + a.size()/2] = even[i] - wi * odd[i];
        if (reverse) {
            std::complex <double> f(2);
            a[i] = a[i] / f;
            a[i + a.size()/2] /= f;
        }
        wi *= w;
    }
}

class BigInteger {
public:
    static int base;
    std::vector <long long> number;
    size_t _size;
    mutable bool positive;

    inline bool isZero () const {
        return (_size == 0) || (_size == 1 && number[0] == 0);
    }

    void makeTransitionThroughDozens () {
        long long pre = 0;
        for (unsigned int i = 0; i < _size; ++i) {
            number[i] += pre;
            pre = number[i] / base;
            number[i] %= base;
        }
        while(pre != 0) {
            ++_size;
            number.push_back(pre);
            pre = number.back() / base;
            number.back() %= base;
        }
    }

    const BigInteger makeLeftSide (int i) const {
        BigInteger c(0);
        c._size = _size - i;
        c.positive = positive;
        if (c._size <= 0) {
            return c;
        }
        c.number[0] = number[i];
        for (long long j = i + 1; j < _size; ++j) {
            c.number.push_back(number[j]);
        }
        return c;
    }

    const BigInteger makeRightSide (int i) const {
        BigInteger c(0);
        c._size = i;
        c.positive = positive;
        c.number[0] = number[0];
        for (int j = 1; j < i; ++j) {
            c.number.push_back(number[j]);
        }
        c.popZeroes();
        return c;
    }

    BigInteger& operator << (int i) {
        _size += i;
        std::vector<long long> vec(_size, 0);
        for (int j = i; j < _size; ++j) {
            vec[j] = number[j-i];
        }
        number = vec;
        return *this;
    }




    BigInteger (int a): _size(0),  positive(a >= 0) {
        a = a >= 0 ? a : -1*a;
        while (a > 0) {
            number.push_back(a % base);
            a /= base;
            ++_size;
        }
        if (_size == 0) {
            number.push_back(0);
            ++_size;
        }
    }

    BigInteger (): _size(1), positive(true) {
        number.push_back(0);
    }

    BigInteger (const std::string& a): _size(0), positive(1) {
        if (a.length() > 0) {
            positive = (a[0] != '-');
            for (size_t i = a.length() - 1; i <= a.length(); --i, ++_size) {
                if ( ! positive && i == 0)
                    break;
                number.push_back((short) a[i] - '0');
            }
            popZeroes();
        } else {
            _size = 1;
            number.push_back(0);
        }
    }
    BigInteger (std::vector<long long>& a) {
        _size = a.size();
        number = a;
        positive = true;
    }



    BigInteger& operator += (const BigInteger& a)   {
        // plus
        if (positive == a.positive) {
            long long int more = 0;
            _size = (_size > a._size) ? _size + 1: a._size + 1;
            number.resize((signed int)_size, 0);
            for (size_t i = a._size - 1; i < a._size; --i) {
                number[i] += a.number[i] + more;
                more = number[i] / base;
                number[i] -= more * base;
                for (int j = 1; more; ++j) {
                    number[i + j] += more;
                    more  = number[i + j] / base;
                    number[i + j] -= more * base;
                }
            }
            popZeroes();
        }
            // minus
        else {
            const BigInteger *p2 = &a;
            const BigInteger *p1 = &a;
            bool real_sign1 = positive;
            bool real_sign2 = a.positive;

            // change sign to compare only mod of numbers
            a.positive = true;
            positive = true;
            if (a > *this) {
                p2 = this;
            } else
                p1 = this;

            //making pointer on max and min element to choose a sign
            positive = real_sign1;
            a.positive = real_sign2;
            while (_size < a._size)
                pushOne();
            int per = 0;
            for (size_t i = 0; i < p2->_size || per == 1; ++i) {
                number[i] = (i < p2->_size)? p1->number[i] - (p2->number[i] + per) : p1->number[i] -(per);
                per = 0;
                if (number[i] < 0) {
                    number[i] += base;
                    per = 1;
                }
            }
            positive = p1->positive;
            popZeroes();
        };
        return *this;
    }

    BigInteger& operator -= (const BigInteger& a) {
        if (this == &a) {
            *this = 0;
            return *this;
        }
        a.changeSign();
        *this += a;
        a.changeSign();
        return *this;
    }

    BigInteger operator *= (const BigInteger& a) {
        BigInteger c(0);
        c.positive = ! (positive ^ a.positive);

        //if one of them is zero
        if (a.isZero() || isZero()) {
            return *this = c;
        }

        c.number.resize(_size + a._size + 1, 0);
        for (unsigned long long i = _size-1; i <= _size; --i) {
            for (unsigned long long j = a._size-1; j <= a._size; --j) {
                c.number[i + j] += (number[i] * a.number[j]);      //multiolication
                long long per = c.number[i + j] / base;
                c.number[i + j] %= base;
                unsigned long long per_index = i + j + 1;
                while (per > 0) {                       // push over the ten
                    c.number[per_index] += per % base;
                    per = c.number[per_index] / base;
                    c.number[per_index] %= base;
                    ++per_index;
                }
            }
        }
        c.popZeroes();
        *this = c;
        return *this;
    }

    BigInteger& operator /= (const BigInteger& a) {
        BigInteger q(0);
        q.number.resize(_size, 0);
        bool real_sign1 = positive;
        bool real_sign2 = a.positive;
        positive = true;
        a.positive = true;
        if (*this < a) {
            positive = real_sign1;
            a.positive = real_sign2;
            return *this = 0;
        }
        positive = real_sign1;
        a.positive = real_sign2;
        size_t n = a._size;
        pushOne();
        for (size_t j = _size - n - 1; j < _size - n; --j) {
            long long chast = (base * number[j + n] + number[j + n - 1]) / a.number[n - 1];
            long long r = (base * number[j + n] + number[j + n - 1]) % a.number[n - 1];
            if ((n - 2 <= n) && (chast == base || chast * a.number[n - 2] > base * r + number[j + n - 2])) {
                r += a.number[n - 1];
                --chast;
            }
            BigInteger q_um = a;
            q_um *= chast;
            int per = 0;
            for (size_t i = j; i < j + q_um._size; ++i) {
                //
                number[i] -= q_um.number[i-j] + per;
                per = 0;
                if (number[i] < 0) {
                    number[i] += 10;
                    per = 1;
                }
            }
            if (number[j + q_um._size] < 0) {
                --chast;
                long long more = 0;
                for (size_t i = j; i < q_um._size - 1; ++i) {
                    number[i] += q.number[i] + more;
                    more = number[i] / 10;
                    number[i] -= more * 10;
                    number[i + 1] += more;
                    more = 0;
                }
            }
            q.number[j] = chast;
        }
        popZeroes();
        q.makeTransitionThroughDozens();
        q.popZeroes();
        *this = q;
        positive = ! (positive ^ a.positive);
        return *this;
    }

    BigInteger& operator %= (const BigInteger& a) {
        BigInteger c = *this;
        c /= a;
        c *= a;
        return *this -= c;
    }


    bool operator == (const BigInteger& a) const {
        if (isZero() && a.isZero()) return true;
        if (_size != a._size) return false;
        if (positive != a.positive) return false;
        for (size_t i = _size - 1; i <= _size; --i) {
            if (a.number[i] != number[i])
                return false;
        }
        return true;
    }

    bool operator != (const BigInteger& a) const {
        return ! (*this == a);
    }

    bool operator < (const BigInteger& a) const {
        if (*this == 0) {
            return a != 0 && a.positive;
        }else if (positive != a.positive) {
            return positive < a.positive;
        }
        bool answer = true;
        if (_size < a._size)
            answer = true;
        else if (_size > a._size)
            answer = false;
        else if (*this != a) {
            for (size_t i = _size-1; i <= _size; --i) {
                if (number[i] > a.number[i]) {
                    answer = false;
                    break;
                }
                else if (number[i] < a.number[i]) {
                    answer = true;
                    break;
                }
            }
        }
        answer = positive == answer;
        if (*this == a)
            answer = false;
        return answer;
    }

    bool operator > (const BigInteger& a) const {
        return ! (*this <= a);
    }

    bool operator >= (const BigInteger& a) const {
        return !(*this < a);
    }

    bool operator <= (const BigInteger& a) const {
        return (a == *this || *this < a);
    }


    long long getNumber (const int& i) const {
        return number[i];
    }
    size_t size () const {
        return _size;
    }

    const BigInteger KaratsubaMultiplication(BigInteger &a) {
        if (_size < a._size) {
            for (int i = _size; i < a._size; ++i) {
                number.push_back(0);
            }
            _size = a._size;
        }
        if (_size > a._size) {
            for (int i = a._size; i < _size; ++i) {
                a.number.push_back(0);
            }
            a._size = _size;
        }
        int m = std::max(_size, a._size);
        if (m < 36) {
            BigInteger X(*this);
            X *= a;
            return X;
        }
        m /= 2;
        BigInteger X_l, X_r, Y_l, Y_r, P1, P2, P3;
        X_l = makeLeftSide(m);
        X_r = makeRightSide(m);
        Y_l = a.makeLeftSide(m);
        Y_r = a.makeRightSide(m);
        P1 = X_l.KaratsubaMultiplication(Y_l);
        P2 = X_r.KaratsubaMultiplication(Y_r);
        X_l += X_r;
        Y_l += Y_r;
        P3 = X_l.KaratsubaMultiplication(Y_l);
        P3 -= P1;
        P3 -= P2;
        P3 << m;
        P1 << (m*2);
        P1 += P2;
        P1 += P3;
        return P1;
    }

    const BigInteger FurieMultiplication (BigInteger& a) const {
        if (size() < 10 || a.size() < 10) {
            BigInteger c(*this);
            return c *= a;
        }
        std::vector <std::complex <double> > this_copy(number.begin(), number.end()), a_copy(a.number.begin(), a.number.end());
        int two_size = 1;
        while (two_size < _size && two_size < a._size)
            two_size <<= 1;
        two_size <<= 1;
        this_copy.resize(two_size), a_copy.resize(two_size);
        bpf(this_copy, false);
        bpf(a_copy, false);
        for (size_t i = 0; i < two_size; ++i) {
            this_copy[i] *= a_copy[i];
        }
        bpf(this_copy, true);
        BigInteger answer;
        answer.positive = positive == a.positive;
        answer.number.resize(two_size);
        for (int i = 0; i < two_size; ++i) {
            answer.number[i] = int(round(this_copy[i].real()));
        }
        answer._size = two_size;
        answer.pushOne();
        answer.makeTransitionThroughDozens();
        answer.popZeroes();
        return answer;
    }

    explicit operator bool () const {
        return !( *this == 0);
    }

    void popZeroes () {
        while (number.size() > 1 && number.back() == 0) {
            number.pop_back();
        }
        _size = number.size();
    }

    void pushOne () {
        number.push_back(0);
        ++_size;
    }

    void changeSign() const {
        positive = ! positive;
    }


};

int BigInteger::base = 128;
std::vector<BigInteger> tenInBinary;


std::ostream& operator << (std::ostream &stream, const BigInteger &a) {
    if (a < 0)
        stream << '-';
    for (size_t i = a.size() - 1; i < a.size(); --i) {
        stream << a.getNumber(i);
    }
    return stream;
}

std::istream& operator >> (std::istream &stream, BigInteger &a) {
    std::string str;
    stream >> str;
    BigInteger b(str);
    a = b;
    return stream;
}

const BigInteger operator + (const BigInteger& a, const BigInteger& b) {
    BigInteger c = a;
    return c += b;
}
const BigInteger operator - (const BigInteger& a, const BigInteger& b) {
    BigInteger c = a;
    return c -= b;
}
const BigInteger operator * (const BigInteger& a, const BigInteger& b) {
    BigInteger c = a;
    return c *= b;
}
const BigInteger operator / (const BigInteger& a, const BigInteger& b) {
    BigInteger c = a;
    return c /= b;
}
const BigInteger operator % (const BigInteger& a, const BigInteger& b) {
    BigInteger c = a;
    return c %= b;
}

std::vector<long long> convertToBinary(std::vector<long long>& vec) {
    std::vector <long long> answer;
    while (vec.size() != 0 && vec.back() == 0)
        vec.pop_back();
    while (vec.size() > 0) {
        if (vec.size() == 1 && vec[0] == 0)
            break;
        long long tenstep = 0;
        for (int i = vec.size() - 1; i >= 0; --i) {
            long long new_s = tenstep * 10 + vec[i];
            vec[i] = new_s / BigInteger::base;
            tenstep = new_s % BigInteger::base;
        }
        answer.push_back(tenstep);
        while (vec.size() != 0 && vec.back() == 0)
            vec.pop_back();
    }
    return answer;
}

std::vector<long long> radix(std::vector<long long>& vec) {
    if (vec.size() < 10)
        return convertToBinary(vec);
    std::vector<long long> right(vec.begin(), vec.begin() + vec.size() / 2);
    std::vector<long long> left(vec.begin() + vec.size() / 2, vec.end());
    right = radix(right);
    left = radix(left);
    BigInteger a(right), b(left);
    b.popZeroes();
    a.popZeroes();
    b = b.FurieMultiplication(tenInBinary[vec.size() / 2]);
    b += a;
    b.popZeroes();
    return b.number;
}

void print(std::vector <long long>& a) {
    bool flag = false;
    for (int i = a.size() - 1; i >= 0; --i) {
        std::string s;
        int num = 1;
        for (int j = 0; j < 7; ++j, num *= 2) {
            s.push_back((a[i] & num) / num + '0');
        }
        for (int k = 6; k > -1; --k) {
            if(! flag && (s[k] == '0')) {
                continue;
            } else {
                flag = true;
                std::cout << s[k];
            }
        }
    }
    
}

int main() {
    std::string input;
    std::cin >> input;
    std::vector<long long> vec_input;
    for (int i = input.size() - 1; i >= 0; --i) {
        vec_input.push_back(input[i] - '0');
    }
    int i = 1;
    while (i < vec_input.size())
        i <<= 1;
    vec_input.insert(vec_input.end(), i - vec_input.size(), 0);

    tenInBinary.reserve(vec_input.size() + 1);
    tenInBinary.push_back(1);
    tenInBinary.push_back(10);
    for(int i = 0; i < input.size(); ++i)
        tenInBinary.push_back(0);
    for (int j = 2; j < vec_input.size() + 1; j *= 2) {
        tenInBinary[j] = tenInBinary[j / 2].FurieMultiplication(tenInBinary[j / 2]);
        std::cout << tenInBinary[j] << ' ';
    }
    std::vector<long long> answer = radix(vec_input);
    print(answer);
    return 0;
}