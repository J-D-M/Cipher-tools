#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void affine(int, char **);
void printUsage();

int main(int argc, char **argv)
{
	if (argc < 4)
		printUsage(argv[0]);
	else
		affine(argc, argv);
	return 0;
}

void printUsage(char *name)
{
	printf("Usage:\n"
	       "%s [-d] [a] [b] [message]\n"
	       "\t-d: decrypt message\n\n"
	       "\t[a] and [b] must be digits\n"
	       "\tand [a] must be coprime with 26\n",
	       name);
}

#define ALPH_SIZE 26

// return true if coprime with 26
bool coprime(int a)
{
	int b = ALPH_SIZE;
	int tmp;

	while (b) {
		tmp = b;
		b   = a % b;
		a   = tmp;
	}

	if (a == 1) {
		return 1;
	} else {
		printf("%d not coprime with 26\n", a);
		return 0;
	}
}

bool isNum(char *str)
{
	for (; *str; str++)
		if (!isdigit(*str))
			return 0;
	return 1;
}

bool badArgs(char *a, char *b)
{
	return !(isNum(a) && isNum(b) && coprime(atoi(a)));
}

// e(x) = (ax + b) mod  m
void encrypt(int a, int b, char *c)
{
	char shift = (isupper(*c)) ? 'A' : 'a';
	*c -= shift;
	*c = ((*c) * a + b) % m;
	*c += shift;
}

// inverse of e(x)
void decrypt(int a, int b, char *c)
{
	char shift = (isupper(*c)) ? 'A' : 'a';
	*c -= shift;

	// preventing underflow
	b %= ALPH_SIZE;
	if (b > *c)
		*c = ALPH_SIZE - (b - *c % ALPH_SIZE);
	else
		*c -= b;

	*c = a * (*c) % m;
	*c += shift;
}

// find modular multiplicative inverse
void inverseA(int *a)
{
	int i;
	for (i = *a + 1; (*a * i) % ALPH_SIZE != 1; i++)
		;
	*a = i;
}

void affine(int argc, char **argv)
{
	bool flag  = !strcmp("-d", argv[1]);
	char *strA = argv[1 + flag];
	char *strB = argv[2 + flag];

	if (badArgs(strA, strB)) {
		printUsage(argv[0]);
		return;
	}

	void (*f)(int, int, char *) = (flag) ? decrypt : encrypt;

	int a = atoi(strA);
	int b = atoi(strB);

	// invert a for decryption
	if (flag)
		inverseA(&a);

	for (size_t i = 3 + flag; i < argc; i++) {
		char *str = argv[i];

		for (; *str; str++) {
			if (isalpha(*str))
				(*f)(a, b, str);
		}

		printf(
		    "%s%c", argv[i],
		    (i == argc - 1)
			? '\n'
			: ' '); // print a spaces between words and end with \n
	}
}
