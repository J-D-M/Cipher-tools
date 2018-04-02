#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void usage(char *name)
{
	printf("Usage:\t%s [-d] [message...]\n\n"
	       "\t-d: decrypt\n",
	       name);
}

char *flatten(char **argv, size_t, size_t);
char *decode(char *input);
char *encode(char *input);

int main(int argc, char **argv)
{

	if (argc < 2) {
		usage(argv[0]);
		return 0;
	}

	bool decode_flag = !strcmp("-d", argv[1]);

	if (decode_flag && argc < 3) {
		usage(argv[0]);
		return 0;
	}

	char *input =
	    flatten(argv, (1 + decode_flag), argc); // skip first arg if

	char *output = !strcmp("-d", argv[0]) ? decode(input) : encode(input);

	puts(output);

	free(input);
	free(output);

	return 0;
}

// flatten args into single string
char *flatten(char **argv, size_t start, size_t end)
{
	size_t buffer_size = 0;

	for (size_t i = 1; i < end; i++)
		buffer_size += strlen(argv[i]);

	// spaces between strings +1 for null terminator
	size_t spaces = end - 1;
	char *str     = malloc(sizeof(char) * buffer_size + spaces);

	size_t str_i = 0;

	for (size_t i = 1; i < end; i++) {
		for (size_t j = 0; argv[i][j]; j++) {
			str[str_i] = argv[i][j];
			++str_i;
		}

		if (i < end - 1) {
			str[str_i] = ' ';
			++str_i;
		}
	}

	str[str_i] = '\0';

	return str;
}

// table for char values
const char b64_char[] = {
    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M',
    'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
    'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm',
    'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
    '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '+', '/',
};

size_t getSize(char *str)
{
	size_t ret = strlen(str);

	while ((ret * 8) % 24 != 0)
		++ret;

	return ret;
}

char *decode(char *input)
{
	char *ret_str = malloc(sizeof(char) * (strlen(input) + 1));
}

char *encode(char *input)
{
	assert(input != NULL);

	char *ret_str    = malloc(sizeof(char) * (getSize(input) + 1));
	size_t itter     = 0;
	size_t ret_itter = 0;
	size_t max = strlen(input);

	while (itter < max) {
		uint64_t buffer = (input[itter] << 15);
		if (itter + 1 < max)
			(input[itter + 1] << 7);
		if (itter + 2 < max)
			(input[itter + 2]);

		for (int i = 0; i < 5; i++) {
			uint64_t tmp = buffer;
			tmp >>= 6 * (3 - i);
			tmp &= 0x3f;

			ret_str[ret_itter] = b64_char[tmp];
			++ret_itter;
		}

		input += 3;
	}

	ret_str[ret_itter] = '\0';
	return ret_str;
}
