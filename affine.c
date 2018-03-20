#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void usage();
int check(char *, char*);
void encrypt(int argc, char **argv);
void decrypt(int argc, char **argv);

int main(int argc, char **argv)
{
        if (argc < 4 || !check(argv[1], argv[2])) {
                usage();
                return 0;
        } else if (!strcmp("-d", argv[3])) {
                decrypt(argc, argv);
        } else {
                encrypt(argc, argv);
        }

        return 0;
}

void usage()
{
        puts(
                        "Usage\n"
                        "\tEncrypt: ./affine [a] [b] [message]\n"
                        "\tDecrypt: ./affine -d [a] [b] [message]\n"
                        "\tWhere [a] and [b] are digits and [a]is coprime\n"
                        "\tto the amount of letters in the alphabet (26)"
            );
}

// check that a and b are numbers and that a is coprime to 26 (greatest common
// denominator is 1)
int gcd(int a, int b)
{
        int tmp;

        while (b) {
                tmp = b;
                b   = a % b;
                a   = tmp;
        }

        return a == 1;
}

int isnum(char *s)
{
        while (*s) {
                if (!isdigit(*s))
                        return 0;
                s++;
        }

        return 1;
}

int check(char *a, char *b)
{
        return isnum(a) && isnum(b) && gcd(atoi(a), 26);
}

// e(x) = (ax + b) mod  m
void encrypt(int argc, char **argv)
{
        int a = atoi(argv[1]);
        int b = atoi(argv[2]);
}

// d(x) = (a^(-1)) * (x - b)
void decrypt(int argc, char **argv)
{
        int a = atoi(argv[1]);
        int b = atoi(argv[2]);
}
