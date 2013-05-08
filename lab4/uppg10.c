#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int morse_code[256] = {
	['a'] = 0x42,
	['b'] = 0x81,
	['c'] = 0x85,
	['d'] = 0x61,
	['e'] = 0x20,
	['f'] = 0x84,
	['g'] = 0x63,
	['h'] = 0x80,
	['i'] = 0x40,
	['j'] = 0x8E,
	['k'] = 0x65,
	['l'] = 0x82,
	['m'] = 0x43,
	['n'] = 0x41,
	['o'] = 0x67,
	['p'] = 0x86,
	['q'] = 0x8B,
	['r'] = 0x62,
	['s'] = 0x60,
	['t'] = 0x21,
	['u'] = 0x64,
	['v'] = 0x88,
	['w'] = 0x66,
	['x'] = 0x89,
	['y'] = 0x8D,
	['z'] = 0x83,
	['0'] = 0xBF,
	['1'] = 0xBE,
	['2'] = 0xBC,
	['3'] = 0xB8,
	['4'] = 0xB0,
	['5'] = 0xA0,
	['6'] = 0xA1,
	['7'] = 0xA3,
	['8'] = 0xA7,
	['9'] = 0xAF
};

static int alpha_code[256] = {
	[0x42] = 'a',
	[0x81] = 'b',
	[0x85] = 'c',
	[0x61] = 'd',
	[0x20] = 'e',
	[0x84] = 'f',
	[0x63] = 'g',
	[0x80] = 'h',
	[0x40] = 'i',
	[0x8E] = 'j',
	[0x65] = 'k',
	[0x82] = 'l',
	[0x43] = 'm',
	[0x41] = 'n',
	[0x67] = 'o',
	[0x86] = 'p',
	[0x8B] = 'q',
	[0x62] = 'r',
	[0x60] = 's',
	[0x21] = 't',
	[0x64] = 'u',
	[0x88] = 'v',
	[0x66] = 'w',
	[0x89] = 'x',
	[0x8D] = 'y',
	[0x83] = 'z',
	[0xBF] = '0',
	[0xBE] = '1',
	[0xBC] = '2',
	[0xB8] = '3',
	[0xB0] = '4',
	[0xA0] = '5',
	[0xA1] = '6',
	[0xA3] = '7',
	[0xA7] = '8',
	[0xAF] = '9'
};


int ascii_to_morse(char c, char *buff) {
	unsigned char morse, loop;
	int i;

	if (c == ' ') {
		strcpy(buff, "   ");
		return 3;
	}

	if (c >= 'A' && c <= 'Z')
		c += ' ';
	morse = morse_code[c];
	loop = (morse & 0xE0) >> 5;
	morse &= 0x1F;

	for (i = 0; i < loop; i++)
		buff[i] = ((morse >> i) & 1) ? '-' : '.';
	buff[i] = ' ';

	return loop + 1;
}


void string_to_morse(char *str, char *buff) {
	int i, j;

	for (i = j = 0; str[i]; i++)
		j += ascii_to_morse(str[i], &buff[j]);
	buff[j] = 0;
	return;
}
		

int morse_to_ascii(char *str, char *buff) {
	int loop, i;
	unsigned char morse = 0;

	if (strstr(str, "   ") == str) {
		*buff = ' ';
		return 3;
	}
	
	if (!strstr(str, " "))
		loop = strlen(str);
	else
		loop = (int) (strstr(str, " ") - str + 1);
	for (i = 0; i < loop; i++) {
		if (str[i] == ' ')
			break;
		morse |= (str[i] == '-') << i;
	}

	if (str[i] == ' ')
		loop--;
	morse |= loop << 5;
	*buff = alpha_code[morse];
//	fprintf(stderr, "%X %i\n", morse, loop);
	return loop + 1;
}



void string_to_ascii(char *str, char *buff) {
	int i, j;

	for (i = j = 0; str[j]; i++)
		j += morse_to_ascii(&str[j], &buff[i]);
	buff[i] = 0;
	return;	
}


int main(int argc, char **argv) {
	char buff[512], buff2[512];

	string_to_morse("hello world", buff);
	fprintf(stderr, "hello world: %s\n", buff);
	string_to_ascii(buff, buff2);
	fprintf(stderr, "%s\n", buff2);

	return 0;
}
