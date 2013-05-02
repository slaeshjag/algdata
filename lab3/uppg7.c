#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct {
	int			uid;
	char			*country;
	int			country_sum;
	char			*capital;
	int			capital_sum;
	long long		surface;
	long long		population;
	int			*neighbour;
	int			neighbours;
} ENTRY;

typedef struct {
	ENTRY			*entry;
	int			entries;
	int			*translate;
	int			max_uid;
} DATABASE;


int sum_str(const char *str) {
	int i, sum;

	for (i = sum = 0; str[i] != 0; sum += str[i++]);
	return sum;
}

void db_rebuild_translate(DATABASE *db) {
	int i;
	free(db->translate);
	db->translate = malloc((db->max_uid + 1) * sizeof(DATABASE));

	for (i = 0; i < db->max_uid + 1; i++)
		db->translate[i] = -1;
	for (i = 0; i < db->entries; i++)
		db->translate[db->entry[i].uid] = i;
	return;
}

void db_add_entry(DATABASE *db, const char *country) {
	int e;

	e = db->entries++;
	db->entry = realloc(db->entry, sizeof(ENTRY) * db->entries);
	db->entry[e].uid = db->max_uid++;
	db->entry[e].country = strdup(country);
	db->entry[e].country_sum = sum_str(country);
	db->entry[e].capital = NULL;
	db->entry[e].neighbour = NULL;
	db->entry[e].capital_sum = db->entry[e].surface = db->entry[e].population = db->entry[e].neighbours = 0;

	db_rebuild_translate(db);

	return;
}

int db_locate_country(DATABASE *db, const char *country) {
	int i, sum = sum_str(country);

	for (i = 0; i < db->entries; i++)
		if (db->entry[i].country_sum == sum)
			if (!strcmp(db->entry[i].country, country))
				return i;
	return -1;
}

int db_delete_country(DATABASE *db, const char *country) {
	int index;

	if ((index = db_locate_country(db, country)) < 0)
		return 0;
	free(db->entry[index].country);
	free(db->entry[index].capital);
	db->translate[db->entry[index].uid] = -1;
	if (index + 1 > db->entries) {
		memmove(&db->entry[index], &db->entry[index + 1], index - db->entries - 1);
		db->entry = realloc(db->entry, sizeof(ENTRY) * (db->entries - 1));
	}
	db->entries--;
	return 1;
}

int db_compare_null(ENTRY *prim, ENTRY *sec) {
	return -1;
}

int db_compare_country(ENTRY *prim, ENTRY *sec) {
	return (strcmp(prim->country, sec->country));
}

int db_compare_population(ENTRY *prim, ENTRY *sec) {
	return (prim->population - sec->population);
}

int db_compare_surface(ENTRY *prim, ENTRY *sec) {
	return (prim->surface - sec->surface);
}

int db_compare_density(ENTRY *prim, ENTRY *sec) {
	return (prim->population / (!prim->population ? 1 : prim->population)) - (sec->population / (!sec->population ? 1 : sec->population));
}

void *db_sort_get(const char *mode) {
	if (!strcmp(mode, "country")) return db_compare_country;
	if (!strcmp(mode, "population")) return db_compare_population;
	if (!strcmp(mode, "surface")) return db_compare_surface;
	if (!strcmp(mode, "density")) return db_compare_density;
	return db_compare_null;
}

void db_sort(DATABASE *db, const char *mode) {
	ENTRY tmp;
	int i, j, (*sort)(ENTRY *prim, ENTRY *sec) = db_sort_get(mode);
	
	for (i = 0; i < db->entries; i++)
		for (j = i; j > 0 && sort(&db->entry[j-1], &db->entry[j]) > 0; j--) {
			tmp = db->entry[j];
			db->entry[j] = db->entry[j-1];
			db->entry[j-1] = tmp;
		}
	return;
}

int db_set_null(DATABASE *db, int country, const char *value) {
	fprintf(stderr, "Bad command\n");
	return -1;
}

int db_set_country(DATABASE *db, int country, const char *value) {
	free(db->entry[country].country);
	db->entry[country].country = strdup(value);
	db->entry[country].country_sum = sum_str(value);
	return 1;
}

int db_set_capital(DATABASE *db, int country, const char *value) {
	free(db->entry[country].capital);
	db->entry[country].capital = strdup(value);
	db->entry[country].capital_sum = sum_str(value);
	return 1;
}

int db_set_surface(DATABASE *db, int country, const char *value) {
	return db->entry[country].surface = atoll(value);
}

int db_set_population(DATABASE *db, int country, const char *value) {
	return db->entry[country].population = atoll(value);
}

int db_find_neighbour(DATABASE *db, int entry, const char *neighbour) {
	int i;
	for (i = 0; i < db->entry[entry].neighbours; i++)
		if (!strcmp(db->entry[db->translate[db->entry[entry].neighbour[i]]].country, neighbour))
			return i;
	return -1;
}

int db_del_neighbour(DATABASE *db, int country, const char *value) {
	int i;
	for (i = 0; i < db->entry[country].neighbours; i++) {
		if (db->translate[db->entry[country].neighbour[i]] < 0)
			continue;
		if (!strcmp(db->entry[db->translate[db->entry[country].neighbour[i]]].country, value))
			break;
	}
	if (i == db->entry[country].neighbours) return 0;
	if (i + 1 < db->entry[country].neighbours)
		memmove(&db->entry[country].neighbour[i], &db->entry[country].neighbour[i+1], sizeof(int) * (db->entry[country].neighbours - i - 1));
	db->entry[country].neighbours--;
	db_del_neighbour(db, db_locate_country(db, value), db->entry[country].country);
	return 1;
}

int db_add_neighbour(DATABASE *db, int country, const char *value) {
	if (db_locate_country(db, value) < 0) return 0;
	if (db_find_neighbour(db, country, value) >= 0) return 0;
	db->entry[country].neighbour = realloc(db->entry[country].neighbour, sizeof(int) * (db->entry[country].neighbours + 1));
	db->entry[country].neighbour[db->entry[country].neighbours++] = db->entry[db_locate_country(db, value)].uid;
	db_add_neighbour(db, db_locate_country(db, value), db->entry[country].country);
	return 1;
}

void *db_get_prop(const char *prop) {
	if (!strcmp(prop, "country")) return db_set_country;
	if (!strcmp(prop, "poulation")) return db_set_population;
	if (!strcmp(prop, "surface")) return db_set_surface;
	if (!strcmp(prop, "capital")) return db_set_capital;
	if (!strcmp(prop, "delneighbour")) return db_del_neighbour;
	if (!strcmp(prop, "addneighbour")) return db_add_neighbour;
	return db_set_null;
}

int db_set(DATABASE *db, const char *mode, int country, const char *value) {
	int (*set)(DATABASE *db, int country, const char *value) = db_get_prop(mode);
	return set(db, country, value);
}

void init_db(DATABASE *db) {
	db->entry = NULL;
	db->entries = 0;
	db->translate = 0;
	db->max_uid = 0;

	return;
}

int db_save(DATABASE *db, const char *file) {
	int i, j;
	FILE *fp = fopen(file, "w");
	for (i = 0; i < db->entries; i++) {
		fprintf(fp, "%s %s %lli %lli %i\n", db->entry[i].country, db->entry[i].capital, db->entry[i].surface, db->entry[i].population, db->entry[i].neighbours);
		for (j = 0; j < db->entry[i].neighbours; j++) 
			if (db->entry[i].neighbour[j]) fprintf(fp, " %i", db->translate[db->entry[i].neighbour[j]]);
		fprintf(fp, "\n");
	}
	fclose(fp);
	return 1;
}

int db_load(DATABASE *db, const char *file) {
	int c, i, j, neighbours;
	char country[128], capital[128], surface[128], population[128];
	FILE *fp = fopen(file, "r");
	if (!fp) return 0;
	c = db->max_uid;
	for (i = 0; !feof(fp); i++) {
		fscanf(fp, "%s %s %s %s %i\n", country, capital, surface, population, &neighbours);
		db_add_entry(db, country);
		db_set_capital(db, c + i, capital);
		db_set_population(db, c + i, population);
		db_set_surface(db, c + i, surface);
		db->entry[c + i].neighbour = malloc(sizeof(int) * neighbours);
		db->entry[c + i].neighbours = neighbours;
		for (j = 0; j < neighbours; j++) {
			fscanf(fp, " %i ", &db->entry[c + i].neighbour[j]);
			db->entry[c + i].neighbour[j] += c;
		}
	}
	fclose(fp);
	return 1; 
}

void db_print_entry(DATABASE *db, int i) {
	int j;
	if (i < 0) return;
	fprintf(stdout, "%s: Cap: %s, pop: %lli, area: %lli km²\nNeighbours: ", db->entry[i].country, db->entry[i].capital, db->entry[i].population, db->entry[i].surface);
	for (j = 0; j < db->entry[i].neighbours; j++)
		fprintf(stdout, "%s ", db->entry[db->translate[db->entry[i].neighbour[j]]].country);
	fprintf(stdout, "\n");
	return;
}

void db_list(DATABASE *db, const char *country) {
	int i;
	for (i = 0; i < db->entries && !(*country); i++)
		db_print_entry(db, i);
	if ((*country)) db_print_entry(db, db_locate_country(db, country));
	return;
}

void db_clear(DATABASE *db) {
	int i, num;
	for (i = 0, num = db->entries; i < num; i++)
		db_delete_country(db, db->entry[0].country);
	return;
}

int main(int argc, char **argv) {
	DATABASE db;
	char command[128], country[128], option[128], input[512];
	int c;
	
	init_db(&db);
	for (;;) {
		*country = 0;
		fprintf(stderr, "> ");
		fgets(input, 512, stdin);
		sscanf(input, "%s %s %s", command, country, option);
		if (!strcmp(command, "exit")) return 0;
		else if (!strcmp(command, "add")) db_add_entry(&db, country);
		else if (!strcmp(command, "del")) db_delete_country(&db, country);
		else if (!strcmp(command, "load")) db_load(&db, country);
		else if (!strcmp(command, "save")) db_save(&db, country);
		else if (!strcmp(command, "list")) db_list(&db, country);
		else if (!strcmp(command, "clear")) db_clear(&db);
		else {
			if ((c = db_locate_country(&db, country)) < 0)
				fprintf(stderr, "Unknown country %s\n", country);
			else db_set(&db, command, c, option);
		}
	}
	
	return 0;
}
