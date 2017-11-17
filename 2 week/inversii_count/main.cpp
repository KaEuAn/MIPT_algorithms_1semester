#include <iostream>


int main() {
    int n;
    std::cin >> n;
    int a[n];
    for (int i = 0; i < n; ++i) {
        std::cin >> a[i];
    }
    Permutation perm(n, a);
    return 0;
}