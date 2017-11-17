#include <iostream>

void MergeSort(long long *start, long long *finish, long long* buffer, long long& ans) {
    if (start + 1 >= finish)
        return;
    long long *middle = start + (finish - start) / 2;
    MergeSort(start, middle, buffer, ans);
    MergeSort(middle, finish, buffer, ans);
    long long *it_left = start;
    long long *it_right = middle;
    long long *new_buffer = buffer;
    while (it_left < middle && it_right < finish) {
        if (*it_left > *it_right) {
            ans += middle - it_left;
            *new_buffer++ = *it_right++;
        } else {
            *new_buffer++ = *it_left++;
        };
    }
    while (it_left < middle)
        *new_buffer++ = *it_left++;
    while (it_right < finish)
        *new_buffer++ = *it_right++;
    while (buffer < new_buffer)
        *start++ = *buffer++;
}

int main() {
    long long n;
    std::cin >> n;
    long long* a = new long long[n];
    for (int i = 0; i < n; ++i) {
        std::cin >> a[i];
    }
    long long* buffer = new long long[n];
    long long answer = 0;
    MergeSort(&a[0], &a[n], buffer, answer);
    std::cout << answer;
    delete[] a;
    delete[] buffer;
    return 0;
}