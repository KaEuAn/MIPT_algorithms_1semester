#include <cstdio>

long long **create_array(int n, int m)
{
    long long** arr = new long long*[m];
    for (int i = 0; i < n; ++i)
        arr[i] = new long long[m];
    return arr;
}

void delete_array(long long** arr, int m)
{
    for(int i = 0; i < m; ++i)
        delete[] arr[i];
    delete[] arr;
}


void MergeSort(long long **start, long long **finish, long long** buffer)
{
    if (start + 1 == finish)
        return;
    long long** middle = start + (finish - start)/2;
    MergeSort(start, middle, buffer);
    MergeSort(middle, finish, buffer);
    long long** it_left = start;
    long long** it_right = middle;
    long long** new_buffer = buffer;
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
            *new_buffer[0] = *it_right[0];
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
    long long a[100000][2];
    scanf("%d", &n);
    for (int i = 0; i < n; ++i)
    {
        scanf("%d", &a[i][0]);
        a[i][1] = i+1;
    }
    long long** buffer = create_array(1000000, 2);
    MergeSort( (long long**)&((long long*)&a[0])[0], (long long**)&((long long*)&a[n] + 1)[0], buffer);
    delete_array(buffer, 1000000);
    long long max_ans = a[0][0];
    long long ans = a[0][0];
    long long* sp_ans[2] = {(long long*)&a[0], (long long*)&a[0]};
    for (long long left = 0, right = 0; left <= n-1; ans -= a[left][0], ++left)
    {
        while (a[right][0] >= a[left][0] + a[left+1][0])
        {
            ++right;
            ans += a[right][0];
        }
        ans -= a[right][0];
        --right;
        if (max_ans < ans)
        {
            max_ans = ans;
            sp_ans[0] = (long long*)&a[left];
            sp_ans[1] = (long long*)&a[right];
        }
    }
    for (long long* iter = sp_ans[0]; iter <= sp_ans[1]; ++iter)
        printf("%ll ", iter[1]);
}
