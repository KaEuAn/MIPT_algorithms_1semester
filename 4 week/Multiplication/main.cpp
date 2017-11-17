#include <iostream>
#include <vector>
#include <string>

class BigInteger {
    static short base;
    std::vector <short> number;
    size_t _size;
    mutable bool positive;

    inline bool isZero () const {
        return (_size == 0) || (_size == 1 && number[0] == 0);
    }

public:
    void prot () {
        int pre = 0;
        for (unsigned int i = 0; i < _size; ++i) {
            number[i] += pre;
            pre = number[i] / base;
            number[i] %= base;
        }
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

    BigInteger (const std::string a): _size(0), positive(1) {
        if (a.length() > 0) {
            positive = (a[0] != '-');
            for (size_t i = a.length() - 1; i <= a.length(); --i, ++_size) {
                if ( ! positive && i == 0)
                    break;
                number.push_back((short) a[i] - '0');
            }
            PopZeroes();
        } else {
            _size = 1;
            number.push_back(0);
        }
    }



    BigInteger& operator += (const BigInteger& a)   {
        if (positive == a.positive) {                // plus
            int more = 0;
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
            PopZeroes();
        } else {                           // minus
            const BigInteger *p2 = &a;
            const BigInteger *p1 = &a;
            bool real_sign1 = positive;
            bool real_sign2 = a.positive;    // change sign to compare only mod of numbers
            a.positive = true;
            positive = true;
            if (a > *this) {
                p2 = this;
            } else
                p1 = this;              //making pointer on max and min element to choose a sign
            positive = real_sign1;
            a.positive = real_sign2;
            while (_size < a._size)
                PushOne();
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
            PopZeroes();
        };
        return *this;
    }

    BigInteger& operator -= (const BigInteger& a) {
        if (this == &a) {
            *this = 0;
            return *this;
        }
        a.ChangeSign();
        *this += a;
        a.ChangeSign();
        return *this;
    }

    BigInteger operator *= (const BigInteger& a) {
        BigInteger c(0);
        c.positive = ! (positive ^ a.positive);
        if (! bool (a) || ! bool (*this)) {          //if one is zero
            return *this = c;
        }

        c.number.resize(_size + a._size + 1, 0);
        for (unsigned long long i = _size-1; i <= _size; --i) {
            for (unsigned long long j = a._size-1; j <= a._size; --j) {
                c.number[i + j] += (number[i] * a.number[j]);      //multiolication
                int per = c.number[i + j] / base;
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
        c.PopZeroes();
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
        PushOne();
        for (size_t j = _size - n - 1; j < _size - n; --j) {
            int chast = (base * number[j + n] + number[j + n - 1]) / a.number[n - 1];
            int r = (base * number[j + n] + number[j + n - 1]) % a.number[n - 1];
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
                int more = 0;
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
        PopZeroes();
        q.prot();
        q.PopZeroes();
        q.PopZeroes();
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



    BigInteger operator - () const {
        BigInteger c = *this;
        return c *= -1;
    }

    const BigInteger operator ++ (int) {
        BigInteger c = *this;
        *this += 1;
        return c;
    }

    BigInteger& operator ++ () {
        return *this += 1;
    }

    const BigInteger operator -- (int) {
        BigInteger c = *this;
        *this -= 1;
        return c;
    }

    BigInteger& operator -- () {
        return *this -= 1;
    }



    std::string toString () const {
        int le = (int) (!positive && *this != 0);
        std::string str(_size+le, '0');
        if (le)
            str[0] = '-';
        for (size_t i = _size - 1; i <= _size; --i)
            str[_size + le - 1 - i] = (char) (number[i] + '0');
        return str;
    }

    int getNumber (const int& i) const {
        return number[i];
    }
    size_t size () const {
        return _size;
    }
    bool isPositive () const {
        return positive;
    }



    explicit operator bool () const {
        return !( *this == 0);
    }

private:
    void PopZeroes () {
        while (number.size() > 1 && number.back() == 0) {
            number.pop_back();
        }
        _size = number.size();
    }

    void PushOne () {
        number.push_back(0);
        ++_size;
    }

    void ChangeSign () const {
        positive = ! positive;
    }


};

short BigInteger::base = 10;

std::ostream& operator << (std::ostream &stream, const BigInteger &a) {
    std::string str;
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



int main() {
    BigInteger a, b;
    std::cin >> a >> b;
    std::cout << a*b;
    return 0;
}