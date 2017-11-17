#include <iostream>
#include <vector>

class stackOfInt {
    int one[1071];
    int two[1071];
    int size;
public:

    stackOfInt (): size(0) {
        one[0] = -1;
        two[0] = -1;
    }

    stackOfInt& push(int i) {
        ++size;
        one[size] = i;
        two[size] = i;
        if (size != 1 && two[size - 1] < i)
            two[size] = two[size - 1];
        return *this;
    }

    stackOfInt& pop () {
        size -= 1;
        return *this;
    }

    int top() const {
        return one[size];
    }

    int min () const {
        return two[size];
    }

    int getSize() const {
        return size;
    }
};

class queue {
    stackOfInt front;
    stackOfInt end;

public:

    queue () {}

    queue (int* start, int* end) {
        while (start != end) {
            front.push(*start);
            ++start;
        }
    }

    queue& push (int i) {
        front.push(i);
        return *this;
    }

    queue& pop () {
        if (end.getSize() != 0) {
            end.pop();
        } else {
            while (front.getSize() != 0) {
                int i = front.top();
                front.pop();
                end.push(i);
            }
            end.pop();
        }
    }

    int min() {
        int mi = front.min();
        int ma = end.min();
        int ans = 0;
        if (front.getSize() == 0)
            ans = ma;
        else if (end.getSize() == 0)
            ans = mi;
        else if (mi < ma)
            ans = mi;
        else ans = ma;
        return ans;
    }
};


int main() {
    int n, L;
    std::cin >> n >> L;
    int a[n][n], ans_str[n][n - L + 1], ans[n - L + 1][n - L + 1];
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            std::cin >> a[i][j];
        }
    }

    for (int i = 0; i < n; ++i){
        queue qu(static_cast<int*> (&(a[i][0])), static_cast<int*> (&(a[i][L])));
        ans_str[i][0] = qu.min();
        for (int j = L; j < n; ++j){
            qu.pop();
            qu.push(a[i][j]);
            ans_str[i][j - L + 1] = qu.min();
        }
    }
    for (int i = 0; i < n - L + 1; ++i){
        queue qu;
        for (int j = 0; j < L; ++j)
            qu.push(ans_str[j][i]);
        ans[0][i] = qu.min();
        for (int j = L; j < n; ++j){
            qu.pop();
            qu.push(ans_str[j][i]);
            ans[j - L + 1][i] = qu.min();
        }
    }
    for (int i = 0; i < n - L + 1; ++i) {
        for (int j = 0; j < n - L + 1; ++j) {
            std::cout<< ans[i][j] << ' ';
        }
        std::cout << '\n';
    }
    return 0;
}