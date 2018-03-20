#include <stdio.h>
#include <string.h>
#include <ctype.h>

void usage()
{
        puts("Usage :\n./atbash [message]");
}

void atbash(char*);

int main(int argc, char **argv)
{
        if (argc < 2) {
                usage();
                return 0;
        }

        for (int i = 1; i < argc; i++) {
                atbash(*(argv + i));
                printf("%s%c", *(argv + i), (i == argc -1 ) ? '\0' : ' ');
        }
        putchar('\n');

        return 0;
}

void atbash(char* str)
{
        while (*str) {
                if (isalpha(*str)) {
                        *str = (isupper(*str)) ?
                                'Z' - (*str - 'A') :
                                'z' - (*str - 'a');
                }
                ++str;
        }
}
