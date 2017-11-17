#include <cstdio>


void MergeSort(int *start, int *finish, int* buffer)
{
    if (start + 1 >= finish)
        return;
    int* middle = start + (finish - start)/2;
    MergeSort(start, middle, buffer);
    MergeSort(middle, finish, buffer);
    int* it_left = start;
    int* it_right = middle;
    int* new_buffer = buffer;
    while (it_left < middle && it_right < finish)
        *new_buffer++ = *it_left < *it_right ? *it_left++ : *it_right++;
    while (it_left < middle)
        *new_buffer++ = *it_left++;
    while (it_right < finish)
        *new_buffer++ = *it_right++;
    while (buffer < new_buffer)
        *start++ = *buffer++;


}
int a[1000000];
int main()
{
    int n,m;
    scanf("%d %d", &n, &m);
    for (int i = 0; i < n*m; ++i)
    {
        scanf("%d", &a[i]);
    }
    int* buffer = new int[1000000];
    MergeSort(&a[0], &(a[n * m - 1]) + 1, buffer);
    for (int i = 0; i < n * m; ++i)
    {
        printf("%d ", a[i]);
    }
    delete[] buffer;
    return 0;
}
