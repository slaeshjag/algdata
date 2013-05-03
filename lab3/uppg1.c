#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
	int		root;
	int		uid;
	char		*word;
	int		word_sum;
	int		*link;
	int		links;
} WORD;


typedef struct {
	WORD		*word;
	int		words;
	int		*translate;
	int		uid_max;
} WORD_LIST;


int str_sum(const char *str) {
	int i, sum;

	for (i = sum = 0; str[i]; sum += str[i], i++);

	return sum;
}


WORD_LIST *init_wordlist() {
	WORD_LIST *l;

	l = malloc(sizeof(WORD_LIST));
	l->word = NULL;
	l->words = 0;
	l->translate = NULL;
	l->uid_max = 0;

	return l;
}


void wordlist_fix_translate(WORD_LIST *l) {
	int i;

	free(l->translate);
	l->translate = malloc(sizeof(int) * l->uid_max);
	for (i = 0; i < l->uid_max; i++)
		l->translate[i] = -1;
	for (i = 0; i < l->words; i++)
		l->translate[l->word[i].uid] = i;
	return;
}


int wordlist_new(WORD_LIST *l, const char *word, int root) {
	int id;

	id = l->words++;
	l->word = realloc(l->word, l->words * sizeof(WORD));
	l->word[id].uid = l->uid_max++;
	l->word[id].root = root;
	l->word[id].word = strdup(word);
	l->word[id].word_sum = str_sum(word);
	l->word[id].link = NULL;
	l->word[id].links = 0;

	wordlist_fix_translate(l);

	return 1;
}



int wordlist_locate_word(WORD_LIST *l, const char *word) {
	int i, sum;
	
	sum = str_sum(word);
	for (i = 0; i < l->words; i++)
		if (l->word[i].word_sum == sum)
			if (!strcmp(word, l->word[i].word))
				return i;
	return -1;
}


int wordlist_unlink(WORD_LIST *l, int word, int link) {
	int i;

	for (i = 0; i < l->word[word].links; i++)
		if (l->word[word].link[i] == l->word[link].uid) {
			l->word[word].link[i] = -1;
			wordlist_unlink(l, link, word);
			break;
		}
	if (i == l->word[word].links)
		return 1;
	if (i >= --l->word[word].links)
		return 1;
	memmove(&l->word[word].link[i], &l->word[word].link[i + 1], sizeof(int) * (l->word[word].links - i));
	l->word[word].link = realloc(l->word[word].link, sizeof(int) * l->word[word].links);

	return 0;
}


int wordlist_link(WORD_LIST *l, const char *word, const char *link) {
	int id, id_link;

	if ((id = wordlist_locate_word(l, word)) < 0) {
		fprintf(stderr, "Word %s does not exist\n", word);
		return 0;
	}

	if ((id_link = wordlist_locate_word(l, link)) < 0) {
		wordlist_new(l, link, 0);
		id_link = wordlist_locate_word(l, link);
	}

	l->word[id].link = realloc(l->word[id].link, sizeof(int) * (++l->word[id].links));
	l->word[id].link[l->word[id].links - 1] = l->word[id_link].uid;
	l->word[id_link].link = realloc(l->word[id_link].link, sizeof(int) * (++l->word[id_link].links));
	l->word[id_link].link[l->word[id_link].links - 1] = l->word[id].uid;

	return 1;
}


void wordlist_list(WORD_LIST *l, const char *word) {
	int i, j;

	for (i = 0; i < l->words; i++) {
		if (*word && strcmp(word, l->word[i].word))
			continue;
		if (!l->word[i].root && l->word[i].links <= 1 && !(*word))
			continue;
		fprintf(stdout, "%s: ", l->word[i].word);
		for (j = 0; j < l->word[i].links; j++)
			fprintf(stdout, "%s ", l->word[l->translate[l->word[i].link[j]]].word);
		fprintf(stdout, "\n\n");
	}

	return;
}


void wordlist_save(WORD_LIST *l, const char *filename) {
	int i, j, skip;
	FILE *fp;

	fp = fopen(filename, "w");
	for (i = skip = 0; i < l->words; i++) {
		if (!l->word[i].links) {
			skip++;
			continue;
		}
		fprintf(fp, "%s %i %i", l->word[i].word, l->word[i].root, l->word[i].links);
		for (j = 0; j < l->word[i].links; j++)
			fprintf(fp, " %i", l->translate[l->word[i].link[j]] - skip);
		fprintf(fp, "\n");
	}

	fclose(fp);
	return;
}


void wordlist_load(WORD_LIST *l, const char *filename) {
	int i, j, root, links;
	char word[256];
	FILE *fp;

	if (!(fp = fopen(filename, "r"))) {
		fprintf(stderr, "Unable to load %s\n", filename);
		return;
	}

	for (i = 0; !feof(fp); i++) {
		fscanf(fp, "%s %i %i", word, &root, &links);
		wordlist_new(l, word, root);
		l->word[i].links = links;
		l->word[i].link = malloc(sizeof(int) * links);
		for (j = 0; j < links; j++)
			fscanf(fp, "%i ", &l->word[i].link[j]);
	}

	fclose(fp);
}



int wordlist_delete(WORD_LIST *l, const char *word) {
	int id, i, links;

	if ((id = wordlist_locate_word(l, word)) < 0)
		return -1;
	links = l->word[id].links;
	for (i = 0; i < links; i++)
		wordlist_unlink(l, id, l->translate[l->word[id].link[0]]);
	free(l->word[id].word);

	l->translate[l->word[id].uid] = -1;
	if (id == --l->words)
		return 1;
	memmove(&l->word[id], &l->word[id + 1], sizeof(WORD) * (l->words - id));
	return 1;
}


int main(int argc, char **argv) {
	WORD_LIST *l = init_wordlist();
	char input[512], command[128], word[128], target[128];

	if (argc <2) {
		fprintf(stderr, "A synonym list file name must be specified\n");
		return -1;
	}

	wordlist_load(l, argv[1]);
	for (;;) {
		fprintf(stderr, "> ");
		*command = *word = *target = 0;
		fgets(input, 512, stdin);
		if (strstr("\n", input))
			*strstr("\n", input) = ' ';
		sscanf(input, "%s %s %s", command, word, target);
		if (!strcmp(command, "add")) wordlist_new(l, word, 1);
		else if (!strcmp(command, "del")) wordlist_delete(l, word);
		else if (!strcmp(command, "link")) wordlist_link(l, word, target);
		else if (!strcmp(command, "unlink")) wordlist_unlink(l, wordlist_locate_word(l, word), wordlist_locate_word(l, target));
		else if (!strcmp(command, "list")) wordlist_list(l, word);
		else if (!strcmp(command, "save")) wordlist_save(l, argv[1]);
		else if (!strcmp(command, "help")) fprintf(stderr, "Commands: add, del, link, unlink, save\n");
		else if (*command == -1) return 0;
		else if (!strcmp(command, "exit")) return 0;
		else fprintf(stderr, "? %s\n", command);
	}

	
	return 0;
}
