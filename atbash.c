#include <ctype.h>
#include <stdio.h>
#include <string.h>

void usage(char *name) { printf("Usage:\n%s [message]", name); }

void atbash(char *str)
{
	while (*str) {
		if (isalpha(*str)) {
			*str = (isupper(*str)) ? 'Z' - (*str - 'A')
					       : 'z' - (*str - 'a');
		}
		++str;
	}
}

int main(int argc, char **argv)
{
	if (argc < 2) {
		usage(argv[0]);
		return 0;
	}

	for (int i = 1; i < argc; i++) {
		atbash(*(argv + i));
		printf("%s%c", *(argv + i), (i == argc - 1) ? '\0' : ' ');
	}
	putchar('\n');

	return 0;
}
