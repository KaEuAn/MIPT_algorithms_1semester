#include <cstdio>

int **create_array(int n, int m)
{
    int** arr = new int*[n];
    for (int i = 0; i < n; ++i)
        arr[i] = new int[m];
    return arr;
}

void delete_array(int** arr, int m)
{
    for(int i = 0; i < m; ++i)
        delete[] arr[i];
    delete[] arr;
}


void MergeSort(int **start, int **finish, int** buffer)
{
    if (start + 1 == finish || start == finish)
        return;
    int** middle = start;
    for (int** new_f = finish; middle < new_f; --new_f, ++middle){}
    MergeSort(start, middle, buffer);
    MergeSort(middle, finish, buffer);
    int** it_left = start;
    int** it_right = middle;
    int** new_buffer = buffer;
    while (it_left < middle && it_right < finish)
    {
        if (*it_left < *it_right)
        {
            *new_buffer[0] = *it_left[0];
            *new_buffer[1] = *it_left[1];
            ++it_left;
        }
        else
        {
            (*new_buffer)[0] = (*it_right)[0];
            *new_buffer[1] = *it_right[1];
            ++it_right;
        }
        ++new_buffer;
    }
    while (it_left < middle)
    {
        *new_buffer[0] = *it_left[0];
        *new_buffer[1] = *it_left[1];
        ++new_buffer;
        ++it_left;
    }
    while (it_right < finish)
    {
        *new_buffer[0] = *it_right[0];
        *new_buffer[1] = *it_right[1];
        ++new_buffer;
        ++it_right;
    }
    while (buffer < new_buffer)
    {
        *start[0] = *buffer[0];
        *start[1] = *buffer[1];
        ++buffer;
        ++start;
    }


}

int main()
{
    int n;
    int a[100001][2];
    scanf("%d", &n);
    for (int i = 0; i < n; ++i)
    {
        scanf("%d", &(a[i][0]));
        a[i][1] = i+1;
    }
    int** buffer = create_array(100000, 2);
    int* bet = a[0];
    int* met = a[n];
    MergeSort( &bet, &met, buffer);
    delete_array(buffer, 1000000);
    int max_ans = a[0][0];
    int ans = a[0][0];
    int* sp_ans[2] = {(int*)&a[0], (int*)&a[0]};
    for (int left = 0, right = 0; left <= n-1; ans -= a[left][0], ++left)
    {
        while (a[right][0] - a[left][0] >= a[left+1][0])
        {
            ++right;
            ans += a[right][0];
        }
        ans -= a[right][0];
        --right;
        if (max_ans < ans)
        {
            max_ans = ans;
            sp_ans[0] = (int*)&a[left];
            sp_ans[1] = (int*)&a[right];
        }
    }
    for (int* iter = sp_ans[0]; iter <= sp_ans[1]; ++iter)
        printf("%ll ", iter[1]);
}
