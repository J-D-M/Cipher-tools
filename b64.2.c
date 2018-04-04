#include <assert.h>
#include <ctype.h> // isupper, islower, isdigit
#include <stdbool.h>
#include <stdint.h> // uint64_t
#include <stdio.h>
#include <stdlib.h> // malloc, free
#include <string.h> // strlen

void usage(char *name)
{
	printf("Usage:\t%s [-d] [message...]\n\n"
	       "\t-d: decode base64 message\n",
	       name);
}

char *base64(char *str, bool decode);
char *flatten(char **argv, size_t start, size_t end);

int main(int argc, char **argv)
{
	bool decode_flag = (argc > 2 && !strcmp(argv[1], "-d"));

	if ((decode_flag && argc > 2) || argc > 1) {
		char *in  = flatten(argv, 1 + decode_flag, argc);
		char *out = base64(in, decode_flag);

		puts(out);

		free(in);
		free(out);
	} else {
		usage(argv[0]);
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

// b64 char values
const char b64_char[] = {
    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M',
    'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
    'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm',
    'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
    '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '+', '/',
};

// values of 64 encoded characters
uint64_t b64ToVal(char c)
{
	if (isupper(c))
		return c - 'A';
	else if (islower(c))
		return c - 'a' + 26;
	else if (isdigit(c))
		return c - '0' + 52;
	else if (c == '+')
		return 62;
	else if (c == '/')
		return 63;
	else if (c == '=')
		return 0;
	else
		return -1; // error
}

size_t getEncryptSize(char *str)
{
	size_t size = strlen(str);
	size_t ret  = (size / 3) * 4;

	ret += size % 3;
	// pad end if necessary
	while (ret % 4 != 0)
		++ret;

	return ret;
}

uint64_t encodeBuffer(char c) { return c; }
uint64_t decodeBuffer(char c) { return b64ToVal(c); }

char *base64(char *str, bool decode)
{
	size_t str_size  = strlen(str);
	size_t ret_itter = 0;
	char *str_ptr    = str;

	size_t ret_size;
	int take;
	int step;
	int char_size;
	int bit_size;
	int bit_mask;

	if (decode) {
		ret_size  = str_size;
		take      = 3;
		step      = 4;
		bit_size  = 6;
		char_size = 8;
		bit_mask  = 0xff; // 8 bits
	} else {
		ret_size  = getEncryptSize(str);
		take      = 4;
		step      = 3;
		bit_size  = 8;
		char_size = 6;
		bit_mask  = 0x3f; // 6 bits
	}

	char *ret = malloc(sizeof(char) * (ret_size + 1));

	for (size_t i = 0; i < str_size; i += step) {
		uint64_t buffer = 0;

		for (int shift = step * bit_size; *str_ptr && shift >= 0;
		     shift -= bit_size) {
			buffer |= ((decode) ? b64ToVal(*str_ptr) : *str_ptr)
				  << shift;

			++str_ptr;
			shift -= bit_size;
		}

		for (int shift = take * char_size; shift; shift -= char_size) {
			// shift bits to extract char
			uint64_t tmp = buffer >> shift;

			ret[ret_itter] =
			    tmp & bit_mask; // isolate significant bits
			++ret_itter;
		}
	}

	if (!decode) {
		for (; ret_itter < ret_size - 1; ret_itter++)
			ret[ret_itter] = '=';
	}

	ret[ret_itter] = '\0';

	return ret;
}
