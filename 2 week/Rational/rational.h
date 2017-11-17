#include <iostream>
#include <vector>
#include <cstring>

class BigInteger {
    static short base;
    std::vector <short> number;
    size_t _size;
    mutable bool positive;

    inline bool isZero() const {
        return (_size == 0) || (_size == 1 && number[0] == 0);
    }

    void makeTransitionThroughDozens() {
        int pre = 0;
        for (unsigned int i = 0; i < _size; ++i) {
            number[i] += pre;
            pre = number[i] / base;
            number[i] %= base;
        }
    }

public:
    friend class Rational;

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



    BigInteger& operator += (const BigInteger& a)   {
        // plus
        if (positive == a.positive) {
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
        if (! static_cast<bool> (a) || ! static_cast<bool> (*this)) {
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
            int quotient = (base * number[j + n] + number[j + n - 1]) / a.number[n - 1];
            int modulo = (base * number[j + n] + number[j + n - 1]) % a.number[n - 1];
            if ((n - 2 <= n) && (quotient == base || quotient * a.number[n - 2] > base * modulo + number[j + n - 2])) {
                modulo += a.number[n - 1];
                --quotient;
            }
            BigInteger q_um = a;
            q_um *= quotient;
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
                --quotient;
                int more = 0;
                for (size_t i = j; i < q_um._size - 1; ++i) {
                    number[i] += q.number[i] + more;
                    more = number[i] / 10;
                    number[i] -= more * 10;
                    number[i + 1] += more;
                    more = 0;
                }
            }
            q.number[j] = quotient;
        }
        popZeroes();
        q.makeTransitionThroughDozens();
        q.popZeroes();
        *this = q;
        positive = ! (real_sign1 != real_sign2);
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
        return (a < *this);
    }

    bool operator >= (const BigInteger& a) const {
        return !(*this < a);
    }

    bool operator <= (const BigInteger& a) const {
        return ! (a < *this);
    }



    const BigInteger operator - () const {
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



    std::string toString() const {
        int le = (int) (!positive && *this != 0);
        std::string str(_size+le, '0');
        if (le)
            str[0] = '-';
        for (size_t i = _size - 1; i <= _size; --i)
            str[_size + le - 1 - i] = (char) (number[i] + '0');
        return str;
    }

    int getNumber(const int& i) const {
        return number[i];
    }
    size_t size() const {
        return _size;
    }
    bool isPositive() const {
        return positive;
    }



    explicit operator bool() const {
        return !( *this == 0);
    }

private:
    void popZeroes() {
        while (number.size() > 1 && number.back() == 0) {
            number.pop_back();
        }
        _size = number.size();
    }

    void pushOne() {
        number.push_back(0);
        ++_size;
    }

    void changeSign() const {
        positive = ! positive;
    }


};

short BigInteger::base = 10;

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


class Rational {
    BigInteger numerator;
    BigInteger denominator;

    bool isPositive() {
        return numerator.isPositive();
    }
    void reduce() {
        if (! denominator.isPositive()) {
            denominator.changeSign();
            numerator.changeSign();
        }
        BigInteger a(numerator), b(denominator);
        if(! numerator.positive)
            a.changeSign();
        while(b != 0) {
            BigInteger c(b);
            a %= b;
            b = a;
            a = c;
        }
        if (a == 1)
            return;
        numerator /= a;
        denominator /= a;
    }

public:

    Rational (const BigInteger& a): numerator(a), denominator(1) {}
    Rational (const int& a): numerator(a), denominator(1) {}
    Rational (): numerator(0), denominator(1) {}

    Rational& operator += (const Rational& a) {
        BigInteger x = a.numerator * denominator;
        numerator *= a.denominator;
        denominator *= a.denominator;
        numerator -= x;
        reduce();
        return *this;
    }

    Rational& operator -= (const Rational& a) {
        a.numerator.changeSign();
        *this += a;
        a.numerator.changeSign();
        return *this;
    }

    Rational& operator *= (const Rational& a) {
        numerator *= a.numerator;
        denominator *= a.denominator;
        reduce();
        return *this;
    }

    Rational& operator /= (const Rational& a) {
        numerator *= a.denominator;
        denominator *= a.numerator;
        reduce();
        return *this;
    }



    bool operator == (const Rational& a) const {
        return numerator == a.numerator && denominator == a.denominator;
    }

    bool operator < (const Rational& a) const {
        return (numerator * a.denominator) < (denominator * a.numerator);
    }

    bool operator != (const Rational& a) const{
        return !(*this == a);
    }

    bool operator > (const Rational& a) const {
        return (a < *this);
    }

    bool operator >= (const Rational& a) const {
        return !(*this < a);
    }

    bool operator <= (const Rational& a) const {
        return ! (a < *this);
    }

    std::string toString () {
        std::string ans;
        std::string str = numerator.toString();
        for (int i = 0; i < str.size(); ++i) {
            ans.push_back(str[i]);
        }
        str = denominator.toString();
        if (str == "1")
            return ans;
        ans.push_back('/');
        for (int i = 0; i < str.size(); ++i) {
            ans.push_back(str[i]);
        }
        return ans;
    }

    std::string asDecimal(size_t precision=0) const {
        BigInteger quotient(numerator);
        bool q_is_posit = quotient.positive;
        std::string ans;
        if (! q_is_posit) {
            quotient.changeSign();
            ans.push_back('-');
        }
        BigInteger remainder(quotient);
        quotient /= denominator;
        remainder -= quotient * denominator;
        ans.append(quotient.toString());
        if (precision == 0)
            return ans;
        ans.push_back('.');
        for (int i = 0; i < precision; ++i) {
            remainder *= 10;
            BigInteger p = remainder / denominator;
            ans.push_back(p.toString()[0]);
            remainder -= p * denominator;
        }

        return ans;
    }

    explicit operator double() {
        std::string string_ans(asDecimal(50));
        double double_ans;
        double p = 1;
        for (int i = string_ans.find('.') - 1; i > -1; ++i, p *= 10) {
            double_ans += p * (string_ans[i] - '0');
        }
        p = 0.1;
        for (int i = string_ans.find('.') + 1; i < string_ans.size(); ++i, p /= 10) {
            double_ans += p * (string_ans[i] - '0');
        }
        return double_ans;
    }

};

const Rational operator + (const Rational& a, const Rational& b) {
    Rational c(a);
    return c += b;
}
const Rational operator - (const Rational& a, const Rational& b) {
    Rational c(a);
    return c -= b;
}
const Rational operator * (const Rational& a, const Rational& b) {
    Rational c(a);
    return c *= b;
}
const Rational operator / (const Rational& a, const Rational& b) {
    Rational c(a);
    return c /= b;
}