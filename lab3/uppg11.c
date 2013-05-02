#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include "utf8.h"

typedef struct {
	unsigned int		codepoint;
	int			num;
} ENTRY;


typedef struct {
	ENTRY			**l;
	int			s;
} ENTRY_VECTOR;


ENTRY *char_isolate(const char *str, int *len) {
	int l, t, i, j;
	ENTRY *e = NULL;

	for (i = l = 0; str[i] != 0; i += utf8GetValidatedCharLength(&str[i])) {
		if (str[i] == '\n')
			continue;
		if (str[i] == ' ')
			continue;
		t = utf8GetChar(&str[i]);
		for (j = 0; j < l; j++)
			if (e[j].codepoint == t)
				break;
		if (j != l) {
			e[j].num++;
			continue;
		}

		l++;
		e = realloc(e, sizeof(ENTRY) * l);
		e[l-1].codepoint = t;
		e[l-1].num = 1;
	}

	*len = l;
	return e;
}


ENTRY_VECTOR *build_vector(ENTRY *e, int num, int *len) {
	int i, max;
	ENTRY_VECTOR *vec;

	for (i = max = 0; i < num; i++)
		if (e[i].num > max)
			max = e[i].num;
	
	max++;
	vec = malloc(sizeof(ENTRY_VECTOR) * (max));
	for (i = 0; i < max; i++) {
		vec[i].l = NULL;
		vec[i].s = 0;
	}

	for (i = 0; i < num; i++)
		vec[e[i].num].s++;
	for (i = 0; i < max; i++)
		vec[i].l = malloc(sizeof(ENTRY *) * vec[i].s);
	
	for (i = 0; i < max; i++)
		vec[i].s = 0;
	for (i = 0; i < num; i++) {
		vec[e[i].num].l[vec[e[i].num].s] = &e[i];
		vec[e[i].num].s++;
	}

	*len = max;
	return vec;
}


int main(int argc, char **argv) {
	char buff[4096];
	int num, i, j;
	ENTRY *e;
	ENTRY_VECTOR *vec;

	fprintf(stderr, "Enter a string to process: ");
	fgets(buff, 4096, stdin);
	
	e = char_isolate(buff, &num);
	vec = build_vector(e, num, &num);

	for (i = 1; i < num; i++) {
		fprintf(stdout, "Following characters appeared %i times: ", i);
		for (j = 0; j < vec[i].s; j++) {
			buff[utf8Encode(vec[i].l[j]->codepoint, buff, 4096)] = 0;
			fprintf(stdout, " %s", buff);
		}
		fprintf(stdout, "\n");
	}


	return 0;
}
