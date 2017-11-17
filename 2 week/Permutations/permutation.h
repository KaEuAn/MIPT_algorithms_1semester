#include <iostream>

class Permutation {
    size_t _size;
    int* inversion;

    bool compare (int a, int b, bool T) {
        return T ? a <= b : a >= b;
    }

    Permutation& makeNext (bool const reverse) {
        size_t j;
        size_t i;

        for ( j = _size - 2; (j <= _size) && compare(inversion[j], inversion[j + 1], reverse); --j);
        if (j > _size)
            return *this;
        for (i = _size - 1; compare(inversion[j], inversion[i], reverse); --i);
        int per = inversion[i];
        inversion[i] = inversion[j];
        inversion[j] = per;
        for (size_t k1 = j + 1, k2 = _size - 1; k1 < k2; ++k1, --k2) {
            per = inversion[k2];
            inversion[k2] = inversion[k1];
            inversion[k1] = per;
        }
        return *this;
    }

public:
    size_t size () const {
        return _size;
    }

    int operator [] (int i) const {
        return inversion[i];
    }

    void operator() (int* p) const {
        int* f = new int[_size];
        for (size_t i = 0; i < _size; ++i) {
            f[inversion[i]] = p[i];
        }
        for (size_t i = 0; i < _size; ++i) {
            p[i] = f[i];
        }
        delete[] f;
    }

    Permutation& operator = (const Permutation& a) {
        if (this == &a)
            return *this;
        delete [] inversion;
        _size = a._size;
        inversion = new int[_size];
        for (size_t i = 0; i < _size; ++i) {
            inversion[i] = a.inversion[i];
        }
        return *this;
    }
    Permutation& operator *= (const Permutation& a) {
        return *this = (*this) * a;
    }

    Permutation (const Permutation& a) {
        _size = a._size;
        inversion = new int[_size];
        for (size_t i = 0; i < _size; ++i) {
            inversion[i] = a.inversion[i];
        }
    }

    Permutation& operator ++ () {
        return makeNext(false);




    }

    const Permutation operator ++ (int) {
        Permutation c = *this;
        ++(*this);
        return c;
    }

    Permutation& operator -- () {
        return makeNext(true);
    };

    Permutation operator -- (int) {
        Permutation c = *this;
        --(*this);
        return c;
    }

    Permutation next() const {
        Permutation c = *this;
        return ++c;
    }

    Permutation previous () const {
        Permutation c = *this;
        return --c;
    }

    Permutation inverse () const {
        Permutation c(_size);
        for (size_t i = 0; i < _size; ++i)
            c.inversion[inversion[i]] = i;
        return c;
    }

    explicit Permutation (unsigned int a): _size(a) {
        inversion = new int [_size];
        for (size_t i = 0; i < _size; ++i) {
            inversion[i] = i;
        }
    }

    bool isOdd () {
        bool* color = new bool[_size];
        int answer = 0;
        for (size_t i = 0; i < _size; ++i) {
            color [i] = false;
        }
        for (size_t i = 0; i < _size; ++i) {
            if (color [i])
                continue;
            int ans = 0;
            size_t per = i;
            while (! color[per]) {
                ++ans;
                color[per] = true;
                per = inversion[per];
            }
            answer += 1 * (ans % 2 == 0);
        }

        delete [] color;
        return (answer % 2 == 1);
    }

    bool isEven () {
        return ! (this->isOdd());
    }

    Permutation pow(int degree) {
        Permutation answer(_size);
        int* mas = new int[_size];
        bool* color = new bool[_size];

        for (size_t i = 0; i < _size; ++i) {
            color [i] = false;
        }
        for (size_t i = 0; i < _size; ++i) {
            if (color[i])
                continue;
            unsigned int ans = 0;
            size_t per = i;
            while (! color[per]) {
                mas[ans] = inversion[per];
                ++ans;
                color[per] = true;
                per = inversion[per];
            }
            if (ans == 0)
                continue;
            int deg = degree % ans;
            if (deg < 0)
                deg += ans;
            for (size_t k = 0, j = deg; k < ans; ++k, ++j) {
                if (j >= ans)
                    j -= ans;
                answer.inversion[mas[k]] = mas[j];
            }
        }
        delete [] mas;
        delete [] color;
        return answer;
    }

    Permutation (unsigned int a, const int* n): _size(a) {
        inversion = new int [_size];
        for (size_t i = 0; i < _size; ++i)
            inversion[i] = n[i];
    }

    ~Permutation() {
        delete [] inversion;
    }

    const Permutation operator * (const Permutation& a) const {
        Permutation c(_size);
        for (size_t i = 0; i < _size; ++i) {
            c.inversion[i] = inversion[a.inversion[i]];
        }
        return c;
    }

    bool operator == (const Permutation& a) const {
        if (_size == a._size) {
            bool f = true;
            for (size_t i = _size-1; i <= _size; --i) {
                if (a.inversion[i] != inversion[i]) {
                    f = false;
                    break;
                }
            }
            return f;
        }
        return false;
    }

    bool operator != (const Permutation& a) const {
        return ! (*this == a);
    }

    bool operator < (const Permutation& a) const {
        bool answer = true;
        if (_size < a._size)
            answer = true;
        else if (_size > a._size)
            answer = false;
        else if (a == *this)
            answer = false;
        else {
            for (size_t i = 0; i <= _size; ++i) {
                if (inversion[i] > a.inversion[i]) {
                    answer = false;
                    break;
                }
                else if (inversion[i] < a.inversion[i]) {
                    answer = true;
                    break;
                }
            }
        }
        return answer;
    }

    bool operator > (const Permutation& a) const {
        return !(a >= *this);
    }

    bool operator >= (const Permutation& a) const {
        return (a <= *this);
    }

    bool operator <= (const Permutation& a) const {
        return !(a < *this);
    }

};