#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h> // malloc
#include <string.h> // strlen
#include <unistd.h> // getopt; optind

void usage(char *name)
{
	printf("Usage:\t%s [-ds] [message...]\n\n"
	       "\t-d: decrypt\n"
	       "\t-s: I-J and U-V will use the same values respectivly\n",
	       name);
}

void baconian(int argc, char **argv, bool decrypt, bool same);

int main(int argc, char **argv)
{
	bool same    = false;
	bool decrypt = false;

	for (char flag = getopt(argc, argv, "ds"); flag != -1;
	     flag      = getopt(argc, argv, "ds")) {
		switch (flag) {
		case 'd':
			decrypt = true;
			break;
		case 's':
			same = true;
			break;
		default:
			usage(argv[0]);
			return 0;
			break;
		}
	}

	if (optind + 1 > argc) {
		puts("Error: no message\n");
		usage(argv[0]);
	} else {
		baconian(argc, argv, same, decrypt);
	}

	return 0;
}

bool isCoded(char c) { return c == 'A' || c == 'a' || c == 'B' || c == 'b'; }

char *decryptBac(char *str, bool same)
{
	// max size should be 1/5th size of input
	char *ret_str = malloc(sizeof(char) * (strlen(str) / 5));

	size_t i     = 0;
	size_t ret_i = 0;

	int char_count = 0;
	int char_num   = 0;

	while (str[i]) {
		if (isCoded(str[i])) {
			char_count++;
			if (tolower(str[i]) == 'b')
				char_num |= 1 << (5 - char_count);
		}

		if (char_count == 5) {
			char c = 'a' + char_num;

			// shift for -s flag
			if (same && c >= 'v' - 'a')
				c += 2;
			else if (same && (c >= 'j' - 'a'))
				++c;

			ret_str[ret_i] = c;
			char_count     = 0;
			char_num       = 0;
			++ret_i;
		} else if (char_count == 0 && str[i] == ' ') {
			ret_str[ret_i] = ' ';
			++ret_i;
		}

		++i;
	}

	ret_str[ret_i] = '\0';
	return ret_str;
}

char *encryptBac(char *str, bool same)
{
	// max size will be 5 * length of input
	const size_t length = strlen(str);
	char *ret_str       = malloc(sizeof(char) * length * 5);

	size_t i     = 0;
	size_t ret_i = 0;

	for (; i < length; i++) {
		if (isalpha(str[i])) {
			// convert to lowercase for simplicity
			char c = tolower(str[i]) - 'a';

			// shift for -s flag
			if (same && c >= 'v' - 'a')
				c -= 2;
			else if (same && (c >= 'j' - 'a'))
				--c;

			// encode letters; 0 bits are 'a' and 1 bits are 'b'
			for (int j = 4; j >= 0; j--) {
				ret_str[ret_i] = 'a' + ((c & (1 << j)) > 0);
				++ret_i;
			}
		} else if (isspace(str[i])) {
			ret_str[ret_i] = ' ';
			++ret_i;
		}
	}
	ret_str[ret_i] = '\0';

	return ret_str;
}

// flatten args into a single string
char *flatten(int argc, char **argv)
{
	size_t buffer_size = 0;

	for (size_t i = optind; i < argc; i++)
		buffer_size += strlen(argv[i]);

	// spaces between args +1 for null terminator
	int spaces = argc - optind;
	char *str  = malloc(sizeof(char) * buffer_size + spaces);

	size_t str_i = 0;

	for (size_t i = optind; i < argc; i++) {
		for (size_t j = 0; argv[i][j]; j++)
			str[str_i++] = argv[i][j];

		if (i < argc - 1)
			str[str_i++] = ' ';
	}

	str[str_i] = '\0';

	return str;
}

void baconian(int argc, char **argv, bool same, bool decrypt)
{
	// get appropriate transform function
	char *(*transform)(char *, bool) = (decrypt) ? decryptBac : encryptBac;
	// make args into a single string
	char *flatArgs = flatten(argc, argv);
	char *str      = (*transform)(flatArgs, same);

	if (decrypt && !strcmp(str, ""))
		puts("Error: message does not look encrypted");
	else
		printf("%s\n", str);

	free(flatArgs);
	free(str);
}
