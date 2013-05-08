/* uppg7.c - Steven Arnow <s@rdw.se>,  2013 */

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

/* All hail FailAPI */
#define	ZeroMemory(ptr, size)		(memset((ptr), 0, size))
#define	HASH_TABLE_SIZE			32768

typedef struct {
	int				*index;
	unsigned int			entries;
} HASH_TABLE;


typedef struct {
	const char			*str;
	int				references;
} WORD_COUNT;


unsigned int str_hash(const char *str, int range) {
	unsigned int hash, i;
	const unsigned char *stra = (void *) str;

	hash = 0;
	for (i = 0; str[i]; hash += stra[i] * (i + stra[i] * 4 + 1), i++);
	hash ^= 0xAAAAAAAA;
	hash %= range;
	
	return hash;
}


HASH_TABLE *hash_table_new(int size) {
	HASH_TABLE *hash;
	int i;

	hash = malloc(sizeof(HASH_TABLE) * size);
	ZeroMemory(hash, sizeof(HASH_TABLE) * size);

	return hash;
}


void hash_table_add(HASH_TABLE *hash, int hash_val, int index) {
	hash[hash_val].index = realloc(hash[hash_val].index, sizeof(int) * (hash[hash_val].entries + 1));
	hash[hash_val].index[hash[hash_val].entries++] = index;
	return;
}


int hash_lookup(WORD_COUNT *word, HASH_TABLE *hash, int size, char *compare) {
	int i;
	unsigned int hash_d;

	hash_d = str_hash(compare, HASH_TABLE_SIZE);

	for (i = 0; i < hash[hash_d].entries; i++)
		if (!strcmp(compare, word[hash[hash_d].index[i]].str)) {
			word[hash[hash_d].index[i]].references++;
			return 1;
		}
//	if (i == hash[hash_d].entries && i > 16)
//		fprintf(stderr, "Collision at 0x%X, %i\n", hash_d, hash[hash_d].entries);
	return 0;
}


void word_sort(WORD_COUNT *word, int words) {
	int i, j;
	WORD_COUNT tmp;

	for (i = 0; i < words; i++) {
		for (j = i; j > 0 && word[j].references > word[j-1].references; j--) {
			tmp = word[j];
			word[j] = word[j - 1];
			word[j - 1] = tmp;
		}
	}

	return;
}


char *strip_case(char *str) {
	int i;

	if (*str == '"')
		str++;
	for (i = 0; str[i]; i++) {
		if (!isalpha(str[i]))
			str[i] = 0;
		if (str[i] < 'a' && str[i])
			str[i] += 'a' - 'A';
	}

	return str;
}


int main(int argc, char **argv) {
	HASH_TABLE *hash;
	WORD_COUNT *word;
	int words, i;
	char buff[512], *tmp;
	FILE *fp;

	if (argc <2) {
		fprintf(stderr, "A book must be provided\n");
		return -1;
	}

	hash = hash_table_new(HASH_TABLE_SIZE);
	if (!(fp = fopen(argv[1], "r"))) {
		fprintf(stderr, "Unable to open %s\n", argv[1]);
		return -1;
	}
	
	words = 0;
	word = NULL;
	while (!feof(fp)) {
		fscanf(fp, "%s", buff);
		if (*buff == ' ' || *buff == '\n' || *buff == '.')
			continue;
		tmp = strip_case(buff);
		if (strlen(buff) == 0)
			continue;
		if (!hash_lookup(word, hash, HASH_TABLE_SIZE, buff)) {
			word = realloc(word, sizeof(WORD_COUNT) * ++words);
			word[words - 1].str = strdup(tmp);
			word[words - 1].references = 1;
			hash_table_add(hash, str_hash(tmp, HASH_TABLE_SIZE), words - 1);
		}
	}

	word_sort(word, words);
	for (i = 0; i < words && i < 100; i++)
		fprintf(stdout, "%s: %i\n", word[i].str, word[i].references);


	return 0;
}
