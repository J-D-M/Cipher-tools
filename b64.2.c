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
	       "\t-d: decrypt\n",
	       name);
}

int main(int argc, char **argv) { return 0; }

// b64 char values
const char b64_char[] = {
    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M',
    'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
    'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm',
    'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
    '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '+', '/',
};

// values of 64 encoded characters
char b64ToVal(char c)
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

char *base64(char *str, bool decode)
{
	if (decode) {
		getBuffer = decodeBuffer;
		shift_size = 18;
		bit_size = 6;
	} else {
		getBuffer = encodeBuffer;
		shift_size = 16;
		bit_size = 8;
	}

	for (size_t i = 0; i < str_len; i += step) {
		uint64_t buffer = 0;
		int shift       = shift_size;

		while (*str_ptr && shift >= 0) {
			buffer |= (*getBuffer)(*str_ptr) << shift;

			++str_ptr;
			shift -= bit_size;
		}

		for (int j = 0; j < take && j <= str_size - i; j++) {
			uint64_t temp = buffer >> bit_size * (take - j);
			tmp &= bit_mask;
			ret[ret_itter] = tmp;
			++ret_itter;
		}
	}

	if (!decrypt) {
		for (; ret_itter < ret_size - 1; ret_itter++)
			ret_str[ret_itter] = '=';
	}

	ret_str[ret_itter] = '\0';

	return ret_str;
}
