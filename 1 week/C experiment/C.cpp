#include <cstdio>
#include <ctime>
#include <iostream>


void func(const char* parametr, const char* file_name, const unsigned int double_count)
{
    double x;
    freopen(file_name, "r", stdin);
    std::cin.tie(NULL);
    std::ios_base::sync_with_stdio(false);
    clock_t start_clock = clock();
    for (int j = 0; j < double_count; ++j)
    {
        if (parametr[0] == 's')
            scanf("%lf", &x);
        else
            (std::cin >> x);
    }
    unsigned int just = clock() - start_clock;
    printf("%s: %d %f --- ", parametr, just, (float)just/CLOCKS_PER_SEC);
    fclose(stdin);
}

int main() {
    freopen("results.txt", "w", stdout);
    const char* a[] = {"test_4.txt", "test_5.txt", "test_6.txt"};
    const char* en[] = {"scanf", "cin"};
    unsigned int double_count = 10000;
    for (int i = 0; i < 3; ++i, double_count *= 10)
    {
        for (int k = 0; k < 2; ++k)
        {
            func(en[k], a[i], double_count);
        }
        printf("\n");
    }

    return 0;
}
