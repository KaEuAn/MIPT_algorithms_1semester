#include <iostream>
#include "polynomial.h"
#include <string>

int main() {
    float ap[5] = {1,2,3,5,4};
    float cp[6] = {2,6,8, 9 ,9, 9};
    int cccccc[5] = {0,1,0,5,3};
    Polynomial<int> bad(cccccc, 5);
    Polynomial<int> b = bad - bad;
    std::cout << b.degree() << ' ';
    for(int i = b.degree(); i >= 0; --i) {
        std::cout << b.get(i) << ' ';
    }
    std::cout << std::endl;
    Polynomial<float> a(ap, 5);
    Polynomial<float> c(cp, 6);
    std::cout << a;
    a.print();
    c.print();
    (c + 1).print();
    (c/2).print();
    std::cout << a.degree() << ' ' << c.degree()<< std::endl;
    (a+c).print();
    (a-a).print();
    (c-c).print();
    (a-c).print();
    (c-a).print();
    (a*c).print();
    (a/c).print();
    (c/a).print();
    (c%a).print();
    std::cout << (bool)(a == c);
    return 0;
}