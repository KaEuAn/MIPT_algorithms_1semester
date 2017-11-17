#include <cstdio>
char s[1000020], x, y;

unsigned int findRepetitionsCount(const char* s, char c1, char c2)
{
    unsigned int i = 0, sum = 0;
    while (s[i])
    {
        if (s[i] == c1 && s[i + 1] == c2) ++sum;
        ++i;
    }
    return sum;
}

int main()
{

    scanf("%s\n", &s);
    scanf("%c %c", &x, &y);
    printf("%u", findRepetitionsCount(s, x, y));
    return 0;
}
