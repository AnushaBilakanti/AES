#include <stdio.h>
int main(int argc, char const *argv[])
{
    unsigned int val = 0;
    char c;
    char *str="0f3c";
    while(c = *str++)
    {
        val <<= 4;

        if (c >= '0' && c <= '9')
        {
            val += c & 0x0F;
            continue;
        }

        c &= 0xDF;
        if (c >= 'A' && c <= 'F')
        {
            val += (c & 0x07) + 9;
            continue;
        }
        return 0;
    }
    printf("%d\n",val);
    return 0;
}
