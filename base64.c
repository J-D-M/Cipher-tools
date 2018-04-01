#include <stdio.h>
#include <string.h>
#include <stdint.h>

// table for char values
const char b64_char[] = {
    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M',
    'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
    'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm',
    'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
    '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '+', '/',
};

void usage(char *name)
{
	printf("Usage:\t%s [-d] [message...]\n\n"
	       "\t-d: decrypt\n",
	       name);
}

char *flatten(int argc, char **argv);
char *decode(char *input);
char *encode(char *input);

int main(int argc, char **argv)
{
	if (argc < 2)
		usage(argv[0]);

	char *input = flatten(argc, argv);

	char *output =
	    !strcmp("-d", argv[0]) ? decode(input) : encode(input);

	puts(output);

	free(input);
	free(output);

	return 0;
}

// flatten args into single string
char *flatten(int argc, char **argv)
{
	size_t buffer_size = 0;

	for (size_t i = 1; i < argc; i++)
		buffer_size += strlen(argv[i]);

	// spaces between args +1 for null terminator
	int32_t spaces = argc - 1;
	char *str  = malloc(sizeof(char) * buffer_size + spaces);

	size_t str_i = 0;

	for (size_t i = 1; i < argc; i++) {
		for (size_t j = 0; argv[i][j]; j++)
			str[str_i++] = argv[i][j];

		if (i < argc - 1)
			str[str_i++] = ' ';
	}

	str[str_i] = '\0';

	return str;
}

size_t getSize(char *str)
{
	size_t ret = strlen(str);

	while ((ret * 8) % 24 != 0)
		++ret;

	return ret;
}

char *decode(char *input)
{
	char *ret_str = malloc(sizeof(char * (strlen(input) + 1));
}

char *encode(char *input)
{
	char *ret_str    = malloc(sizeof(char) * (getSize(input) + 1));
	size_t itter     = 0;
	size_t ret_itter = 0;

	while (input[itter]) {

	}
}
