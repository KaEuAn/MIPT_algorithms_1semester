#include <iostream>
#include "rational.h"

int main() {
    Rational r(60);
    std::cout << r.toString() << '\n';
    r /= 2;
    std::cout << r.toString() << '\n';
    Rational m(r);
    m /= 13;
    std::cout << m.toString() << '\n';
    Rational f(r);
    f *= -1 / m;
    std::cout << f.toString() << '\n';
    f /= m * 9;
    std::cout << f.toString() << '\n';
    std::cout << f.asDecimal(50) << '\n';
    std::cout << double(f) << '\n';
}