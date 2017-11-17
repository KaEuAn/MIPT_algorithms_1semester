#include <iostream>
#include <vector>
#include <algorithm>



int main() {
    int a[100500];
    int b[100500];
    std::pair<int, int> c[100500];
    int answer[100500];
    int n, m;
    std::cin >> n >> m;
    for (int i = 0; i < n; ++i) {
        int first, second;
        std::cin >> first >> second;
        if (first > second) {
            int k = first;
            first = second;
            second = k;
        }
        a[i] = (first);
        b[i] = (second);
    }
    std::sort(a, a + n);
    std::sort(b, b + n);
    for (int i = 0; i < m; ++i) {
        int point;
        std::cin >> point;
        c[i].first = point;
        c[i].second = i;
    }
    std::sort(c, c + m);
    int p1 = -1, p2 = -1;
    for (int i = 0; i < m; ++i) {
        while (p1 < n - 1 && c[i].first >= a[p1 + 1]) {
            ++p1;
        }
        while (p2 < n - 1 && c[i].first > b[p2 + 1]) {
            ++p2;
        }
        answer[c[i].second] = p1 - p2;
    }

    for (int i = 0; i < m; ++i) {
        std::cout << answer[i] << ' ';
    }
    return 0;
}