#include <ctype.h> // isupper, islower, isdigit
#include <stdbool.h>
#include <stdint.h> // uint64_t
#include <stdio.h>
#include <stdlib.h> // malloc, free
#include <string.h> // strlen

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
	bool decode_flag = argc > 2 && !strcmp("-d", argv[1]);

	if (argc < 2 + decode_flag) {
		usage(argv[0]);
	} else {
		// make args into one long string
		// skip first arg if its a flag
		// then decode / encode
		char *input  = flatten(argv, (1 + decode_flag), argc);
		char *output = (decode_flag) ? decode(input) : encode(input);

		puts(output);
		// return memory
		free(input);
		free(output);
	}

	return 0;
}

// flatten args into single string
char *flatten(char **argv, size_t start, size_t end)
{
	size_t buffer_size = 0;

	for (size_t i = start; i < end; i++)
		buffer_size += strlen(argv[i]);

	// spaces between strings +1 for null terminator
	size_t spaces = (end - start) - 1;
	char *str     = malloc((sizeof(char) * buffer_size) + spaces);

	size_t str_i = 0;

	while (start < end) {
		for (char *j = argv[start]; *j; j++) {
			str[str_i] = *j;
			++str_i;
		}

		if (start < end - 1) {
			str[str_i] = ' ';
			++str_i;
		}
		++start;
	}

	str[str_i] = '\0';

	return str;
}

// encode value to b64
const char b64_char[] = {
    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M',
    'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
    'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm',
    'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
    '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '+', '/',
};
// return value of b64 character
uint64_t b64ToVal(char c)
{
	size_t val = 0;

	if (isupper(c))
		val = c - 'A';
	else if (islower(c))
		val = c - 'a' + 26;
	else if (isdigit(c))
		val = c - '0' + 52;
	else if (c == '+')
		val = 62;
	else if (c == '/')
		val = 63;
	else if (c == '=')
		val = 0;
	else
		val = -1; // error

	return val;
}

// size for encoding string
size_t getSize(char *str)
{
	size_t size = strlen(str);
	size_t ret  = (size / 3) * 4;
	ret += size % 3;

	// pad end if necessary
	while (ret % 4 != 0)
		++ret;

	return ret;
}

char *encode(char *str)
{
	size_t str_len = strlen(str);
	size_t ret_len = getSize(str);
	size_t ret_i   = 0;
	char *ret      = malloc(sizeof(char) * (ret_len + 1));

	for (size_t i = 0; i < str_len; i += 3) {
		uint64_t buffer = 0;

		// put 3 chars into buffer
		char *tmp_ptr = &str[i];
		int shift     = 16;
		while (*tmp_ptr && shift >= 0) {
			buffer |= *tmp_ptr << shift;

			++tmp_ptr;
			shift -= 8;
		}

		shift = 18;
		// extract base 64 characters from buffer
		for (int j = 0; j < 4 && j + i <= str_len; j++) {
			char val = buffer >> shift;
			val &= 0x3f; // 6 rightmost bits

			shift -= 6;
			ret[ret_i] = b64_char[val];
			++ret_i;
		}
	}

	// add '=' padding if necessary
	for (; ret_i < ret_len; ret_i++)
		ret[ret_i] = '=';

	ret[ret_i] = '\0';
	return ret;
}

char *decode(char *str)
{
	size_t str_len = strlen(str);
	size_t ret_i   = 0;
	char *ret      = mallco(sizeof(char) * str_len + 1);

	for (size_t i = 0; i < str_len; i += 4) {
		uint64_t buffer = 0;
		// put 4 chars into buffer
		char *tmp_ptr = &str[i];
		int shift     = 18;
		while (*tmp_ptr && shift >= 0) {
			buffer |= b64ToVal(*tmp_ptr) << shift;

			++tmp_ptr;
			shift -= 6;
		}
		// take 3 chars out
		shift = 16;
		for (int j = 0; j < 3 && j + i <= str_len; j++) {
			char c = buffer >> shift;

			shift -= 8;
			ret[ret_i] = c;
			++ret_i;
		}
	}

	ret[ret_i] = '\0';
	return ret;
}
