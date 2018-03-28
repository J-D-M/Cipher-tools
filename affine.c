#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void affine(int, char **);
void printUsage();

int main(int argc, char **argv)
{
	if (argc < 4) {
		printUsage();
	} else {
		affine(argc, argv);
	}
	return 0;
}

void printUsage()
{
	puts(
			"Usage\n"
			"\tEncrypt: ./affine [a] [b] [message]\n"
			"\tDecrypt: ./affine -d [a] [b] [message]\n"
			"\tWhere [a] and [b] are digits and [a]is coprime\n"
			"\tto 26 (the amount of letters in the alphabet)"
	    );
}

// size of alphabet
const int m = 26;

// return 1 if coprime with 26
bool coprime(int a)
{
	int b = m;
	int tmp;

	while (b) {
		tmp = b;
		b   = a % b;
		a   = tmp;
	}

	if (a == 1) {
		return 1;
	} else {
		puts("[a] not coprime\n");
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

// d(x) = ((x - b) / a) mod m
void decrypt(int a, int b, char *c)
{
	char shift = (isupper(*c)) ? 'A' : 'a';
	*c -= shift;
	*c = ((*c - b) / a) % m;
	*c += shift;
}

void affine(int argc, char **argv)
{
	bool flag = !strcmp("-d", argv[1]);
	char *strA = argv[1 + flag];
	char *strB = argv[2 + flag];

	if (badArgs(strA, strB)) {
		printUsage();
		return;
	}

	void (*f)(int, int, char *) =
		(flag) ? decrypt : encrypt;

	int a = atoi(strA);
	int b = atoi(strB);

	for (size_t i = 3 + flag; i < argc; i++) {
		char *str = argv[i];

		for (; *str; str++) {
			if isalpha(*str)
				(*f)(a, b, str);
		}

		printf("%s%c", argv[i], (i == argc - 1) ? '\n' : ' ');
	}
}
