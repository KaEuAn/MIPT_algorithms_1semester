#include <iostream>
#include <string>

template <class T>
class Comp{
public:
    bool operator () (const T& a, const T& b) const {
        return a == b;
    }
};

template <class T, class Comparator = Comp<T> >
class Polynomial {
private:
    T* coefficients;
    size_t size;

    void makeGreater(size_t new_size) {
        T* new_coef = new T[new_size];
        for(size_t i = 0; i < size; ++i) {
            new_coef[i] = coefficients[i];
        }
        for(size_t i = size; i < new_size; ++i) {
            new_coef[i] = 0;
        }
        size = new_size;
        coefficients = new_coef;
    }

    void popZeroes() {
        int new_degree = size - 1;
        for(; new_degree >= 0; --new_degree) {
            if(coefficients[new_degree] != 0)
                break;
        }
        if (new_degree != size) {
            size = new_degree + 1;
            if (new_degree < 0) {
                size = 1;
                new_degree = 0;
            }
            T* new_coef = new T[new_degree + 1];
            for(size_t i = 0; i < new_degree + 1; i++)
                new_coef[i] = coefficients[i];
            delete [] coefficients;
            coefficients = new_coef;
        }
    }

public:
    Polynomial(T* coef, size_t new_size) : size(new_size){
        coefficients = new T[new_size];
        for(size_t i = 0; i < new_size; ++i) {
            coefficients[i] = coef[i];
        }
    }
    Polynomial() {}
    Polynomial(T a) : size(1) {
        coefficients = new T[1];
        coefficients[0] = a;
    }

    ~Polynomial() {
        delete[] coefficients;
    }
    Polynomial(const Polynomial& a) {
        size = a.size;
        coefficients = new T[size];
        for(size_t i = 0; i < size; ++i) {
            coefficients[i] = a.coefficients[i];
        }
    }
    Polynomial& operator = (const Polynomial& a) {
        if (this == &a)
            return *this;
        delete [] coefficients;
        size = a.size;
        coefficients = new T[size];
        for(size_t i = 0; i < size; ++i) {
            coefficients[i] = a.coefficients[i];
        }
        return *this;
    }

    const Polynomial operator - () const {
        Polynomial c = *this;
        return c *= -1;
    }

    Polynomial& operator += (const Polynomial& a) {
        if (a.size > size) {
            makeGreater(a.size);
        }
        for(int i = 0; i < a.size; ++i) {
            coefficients[i] += a.coefficients[i];
        }
        popZeroes();
        return *this;
    }
    Polynomial& operator -= (const Polynomial& a) {
        if (a.size > size) {
            makeGreater(a.size);
        }
        for(int i = 0; i < a.size; ++i) {
            coefficients[i] -= a.coefficients[i];
        }
        popZeroes();
        return *this;
    }
    Polynomial& operator *= (const Polynomial& a) {
        Polynomial c(*this);
        for(int i = 0; i < c.size; ++i) {
            c.coefficients[i] = 0;
        }
        c.makeGreater(size + a.size - 1);
        for(size_t i = 0; i < size; ++i) {
            for(size_t j = 0; j < a.size; ++j) {
                c.coefficients[i + j] += a.coefficients[j] * coefficients[i];
            }
        }
        c.popZeroes();
        *this = c;
        return *this;
    }
    Polynomial& operator /= (const Polynomial& a) {
        if (size < a.size) {
            Polynomial<T, Comparator> answer(0);
            *this = answer;
            return *this;
        }
        Polynomial<T, Comparator> q(*this);
        size_t n = a.size;
        T answer[size - n + 1];
        for(size_t j = size - n, per = 0; j < size - n + 1; --j, ++per) {
            T quotient = q.coefficients[size - 1 - per] / a.coefficients[n - 1];
            Polynomial<T, Comparator> q_um = a;
            q_um *= quotient;
            for(int i = 0; i < q_um.size; ++i) {
                q.coefficients[size - 1 - per - i] -= q_um.coefficients[q_um.size - 1 - i];
            }
            answer[j] = quotient;
        }
        Polynomial<T, Comparator> new_answer(answer, size - n + 1);
        new_answer.popZeroes();
        *this = new_answer;
        return *this;
    }
    Polynomial& operator %= (const Polynomial& a) {
        Polynomial c(*this);
        c /= a;
        c *= a;
        return *this -= c;
    }

    void set(size_t degree, const T& value) {
        if (size <= degree) {
            makeGreater(degree);
            size = degree;
        }
        coefficients[degree] = value;
    }
    int degree() const {
        if (size > 1)
            return size - 1;
        if (coefficients[0] == 0)
            return -1;
        return 0;
    }
    const T operator () (const T& x) const {
        T answer = coefficients[0];
        T new_x = x;
        for (size_t i = 1; i < size; ++i, new_x *= x) {
            answer += new_x * coefficients[i];
        }
        return answer;
    }

    void swap(Polynomial& a) {
        size_t inter_size = size;
        size = a.size;
        a.size = inter_size;
        T* inter_coef = coefficients;
        coefficients = a.coefficients;
        a.coefficients = inter_coef;

    }

    std::string toString () const {
        std::string answer = std::to_string(coefficients[0]);
        for(size_t i = 1; i < size; ++i) {
            T per = coefficients[i];
            if (per >= 0) {
                answer.push_back('+');
            }
            answer = answer + std::to_string(per)+ "*x^" + std::to_string(i);
        }
        return answer;

    }

    bool operator == (const Polynomial& a) const {
        if (a.size != size)
            return false;
        bool answer = true;
        for(size_t i = 0; i < size; ++i) {
            Comparator comp;
            if (! comp(coefficients[i], a.coefficients[i])) {
                answer = false;
                break;
            }
        }
        return answer;
    }
    bool operator != (const Polynomial& a) const {
        return !(*this == a);
    }

    const T get(int degree) const {
        if (degree >= size)
            return 0;
        return coefficients[degree];
    }
    void print() const {
        for(int i = 0; i < size; ++i) {
            std::cout << coefficients[i] << ' ';
        }
        std::cout << std::endl;
    }

    const Polynomial operator + (T a) const {
        Polynomial<T, Comparator> b(a);
        return *this + b;
    }
    const Polynomial operator - (T a) const {
        Polynomial<T, Comparator> b(a);
        return *this - b;
    }
    const Polynomial operator * (T a) const {
        Polynomial<T, Comparator> b(a);
        return *this * b;
    }
    const Polynomial operator / (T a) const {
        Polynomial<T, Comparator> b(a);
        return *this / b;
    }
    const Polynomial operator % (T a) const {
        Polynomial<T, Comparator> b(a);
        return *this % b;
    }
};
template <class T, class Comp>
const Polynomial<T, Comp> operator + (const Polynomial<T, Comp>& a, const Polynomial<T, Comp>& b) {
    Polynomial<T, Comp> c = a;
    return c += b;
}
template <class T, class Comp>
const Polynomial<T, Comp> operator - (const Polynomial<T, Comp>& a, const Polynomial<T, Comp>& b) {
    Polynomial<T, Comp> c = a;
    return c -= b;
}
template <class T, class Comp>
const Polynomial<T, Comp> operator * (const Polynomial<T, Comp>& a, const Polynomial<T, Comp>& b) {
    Polynomial<T, Comp> c = a;
    return c *= b;
}
template <class T, class Comp>
const Polynomial<T, Comp> operator / (const Polynomial<T, Comp>& a, const Polynomial<T, Comp>& b) {
    Polynomial<T, Comp> c = a;
    return c /= b;
}
template <class T, class Comp>
const Polynomial<T, Comp> operator % (const Polynomial<T, Comp>& a, const Polynomial<T, Comp>& b) {
    Polynomial<T, Comp> c = a;
    return c %= b;
}

template <class T, class Comp>
const Polynomial<T, Comp> operator == (const Polynomial<T, Comp>& a, const Polynomial<T, Comp>& b) {
    return a == b;
}


template <class T, class Comp>
std::ostream& operator << (std::ostream &stream, const Polynomial<T, Comp> &a) {
    stream << a.get(0);
    for(size_t i = 1; i < a.degree() + 1; ++i) {
        T per = a.get(i);
        if (per >= 0) {
            stream << '+';
        }
        stream << per;
        stream << "*x^";
        stream << i;
    }
    stream << std::endl;
    return stream;
}