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
int base = 256;
std::vector<std::vector<long long> > tenInBinary;

void makeTransitionThroughDozens (std::vector <long long>& number) {
    long long _size = number.size();
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

bool isZero (std::vector<long long>& number) {
    return (number.size() == 0) || (number.size() == 1 && number[0] == 0);
}

void popZeroes (std::vector< long long>& number) {
    while (number.size() > 1 && number.back() == 0) {
        number.pop_back();
    }
}

void pushOne (std::vector< long long>& number) {
    number.push_back(0);
}


std::vector<long long>& operator += (std::vector< long long>& number, std::vector< long long>& a) {
    long long int more = 0;long long _size = number.size(); long long a_size = a.size();

    _size = (_size > a_size) ? _size + 1: a_size + 1;
    number.resize((signed int)_size, 0);
    for (size_t i = a_size - 1; i < a_size; --i) {
        number[i] += a[i] + more;
        more = number[i] / base;
        number[i] -= more * base;
        for (int j = 1; more; ++j) {
            number[i + j] += more;
            more  = number[i + j] / base;
            number[i + j] -= more * base;
        }
    }
    popZeroes(number);
}

std::vector<long long>& operator *= (std::vector< long long>& number, std::vector< long long>& a) {
    std::vector<long long> c;
    c.push_back(0);
    //if one of them is zero
    if (isZero(number) || isZero(a)) {
        return number = c;
    }

    c.resize(number.size() + a.size() + 1, 0);
    for (unsigned long long i = number.size()-1; i <= number.size(); --i) {
        for (unsigned long long j = a.size()-1; j <= a.size(); --j) {
            c[i + j] += (number[i] * a[j]);      //multiolication
            long long per = c[i + j] / base;
            c[i + j] %= base;
            unsigned long long per_index = i + j + 1;
            while (per > 0) {                       // push over the ten
                c[per_index] += per % base;
                per = c[per_index] / base;
                c[per_index] %= base;
                ++per_index;
            }
        }
    }
    popZeroes(c);
    number = c;
    return number;
}


std::vector<long long> FurieMultiplication (std::vector< long long>& number, std::vector< long long>& a){
        if (number.size() < 10 || a.size() < 10) {
            std::vector<long long> c(number);
            return c *= a;
        }
        std::vector <std::complex <double> > this_copy(number.begin(), number.end()), a_copy(a.begin(), a.end());
        int two_size = 1;
        while (two_size < number.size() || two_size < a.size())
            two_size <<= 1;
        two_size <<= 1;
        this_copy.resize(two_size), a_copy.resize(two_size);
        bpf(this_copy, false);
        bpf(a_copy, false);
        for (size_t i = 0; i < two_size; ++i) {
            this_copy[i] *= a_copy[i];
        }
        bpf(this_copy, true);
        std::vector<long long> answer;
        answer.resize(two_size);
        for (long long i = 0; i < two_size; ++i) {
            answer[i] = int(round(this_copy[i].real()));
        }
        pushOne(answer);
        makeTransitionThroughDozens(answer);
        popZeroes(answer);
        return answer;
    }


std::vector<long long> convertToBinary(std::vector<long long>& vec) {
    std::vector <long long> answer;
    while (vec.size() != 0 && vec.back() == 0)
        vec.pop_back();
    while (vec.size() > 0) {
        if (vec.size() == 1 && vec[0] == 0)
            break;
        long long tenstep = 0;
        for (long long i = vec.size() - 1; i >= 0; --i) {
            long long new_s = tenstep * 10 + vec[i];
            vec[i] = new_s / base;
            tenstep = new_s % base;
        }
        answer.push_back(tenstep);
        while (vec.size() != 0 && vec.back() == 0)
            vec.pop_back();
    }
    return answer;
}

std::vector<long long> radix(std::vector<long long>& vec) {
    if (vec.size() < 180)
        return convertToBinary(vec);
    std::vector<long long> right(vec.begin(), vec.begin() + vec.size() / 2);
    std::vector<long long> left(vec.begin() + vec.size() / 2, vec.end());
    right = radix(right);
    left = radix(left);
    popZeroes(left);
    popZeroes(right);
    left = FurieMultiplication(left, (tenInBinary[vec.size() / 2]));
    left += right;
    popZeroes(left);
    return left;
}

void print(std::vector <long long>& a) {
    bool flag = false;
    for (long long i = a.size() - 1; i >= 0; --i) {
        std::string s;
        int num = 1;
        for (long long j = 0; j < 8; ++j, num *= 2) {
            s.push_back((a[i] & num) / num + '0');
        }
        for (long long k = 7; k > -1; --k) {
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
    if (input == "0") {
        std::cout << "0" << std::endl;
        return 0;
    }
    std::vector<long long> vec_input;
    for (long long i = input.size() - 1; i >= 0; --i) {
        vec_input.push_back(input[i] - '0');
    }
    long long i = 1;
    while (i < vec_input.size())
        i <<= 1;
    vec_input.insert(vec_input.end(), i - vec_input.size(), 0);

    tenInBinary.reserve(vec_input.size() + 1);
    std::vector <long long> ten;
    ten.push_back(1);
    tenInBinary.push_back(ten);
    ten[0] = 10;
    tenInBinary.push_back(ten);
    for (long long i = 0; i < input.size() + 1; ++i)
        tenInBinary.push_back(ten);
    for (long long j = 2; j < vec_input.size() + 1; j *= 2) {
        tenInBinary[j] = FurieMultiplication(tenInBinary[j / 2], tenInBinary[j / 2]);
    }
    std::vector<long long> answer = radix(vec_input);
    print(answer);
    std::cout << std::endl;
    return 0;
}