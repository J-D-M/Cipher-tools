#include <stdbool.h>
#include <stdio.h>
#include <string.h> // strlen
#include <unistd.h> // getopt; optind

void usage(char *name)
{
	printf("Usage:\t%s [-ds] [message...]\n\n"
	       "\t-s: I-J and U-V have the same code\n"
	       "\t-d: decrypt\n",
	       name);
}

char *decrypt(char *str, bool)
{
	if (strlen(str) % 5 != 0) {
		puts("Error: string does not look encoded");
		return 0;
	}
}

void baconian(int argc, char **argv, bool decrypt, bool unique)
{
	// get appropriat transform function
	char *function(char *, bool) = (decrypt) ? decryptBac : encryptBac;

	for (size_t i = optind; i < argc; i++) {
		printf("%s%c", (*function)(argv[i], unique),
		       (i < argc - 1) ? ' '
				      : '\n'); // put newline after last word
	}
}

int main(int argc, char **argv)
{
	bool unique  = true;
	bool decrypt = false;

	for (char flag = getopt(argc, argv, "ds"); flag != -1;
	     flag      = getopt(argc, argv, "ds")) {
		switch (flag) {
		case 'd':
			decrypt = true;
			break;
		case 's':
			unique = false;
			break;
		default:
			usage(argv[0]);
			return (-2);
			break;
		}
	}

	if (optind + 1 > argc) {
		puts("Error: no message\n");
		usage(argv[0]);
	} else {
		baconian(argc, argv, unique, decrypt);
	}

	return 0;
}