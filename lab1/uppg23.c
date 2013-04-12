#include <darnit/darnit.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define	MAZE_INDEX(x, y, w)	((x) + (y) * (w))
#define	MAZE_TRY(dx, dy)	if (!maze_generate_path(tm, x + (dx), y + (dy), allow_exit, min)) \
					continue; else goto found; break;
#define	MAZE_PATH_MIN		32
#define	MAZE_GOOD		2
#define	MAZE_BAD		3

int animate;


void render(DARNIT_TILEMAP *tm) {
	d_render_begin();
	d_tilemap_draw(tm);
	d_render_end();

	d_loop();
	
	return;
}


/* Returnerar 1 om den är klar, 0 vid misslyckande */
int maze_generate_path(DARNIT_TILEMAP *tm, int x, int y, int allow_exit, int min) {
	int i, count = 0;

	min++;
	/* Finns redan en gång här, vilket betyder att vi försökt backa. Inte pra... */
	if (tm->data[MAZE_INDEX(x, y, tm->w)] == 0)
		return 0;

	count += (tm->data[MAZE_INDEX(x + 1, y, tm->w)] == 0) ? 1 : 0;
	count += (tm->data[MAZE_INDEX(x, y + 1, tm->w)] == 0) ? 1 : 0;
	count += (tm->data[MAZE_INDEX(x - 1, y, tm->w)] == 0) ? 1 : 0;
	count += (tm->data[MAZE_INDEX(x, y - 1, tm->w)] == 0) ? 1 : 0;

	/* Vi håller på att brygga två gångar; inte tillåtet! */
	if (count > 1)
		return 0;

	if (x == 25)
		return 0;
	if ((x == 0 || x == tm->w - 1 || y == 0 || y == tm->h - 1) && !(x == 24 && y == 13)) {
		if (allow_exit) {
			if (min < MAZE_PATH_MIN)
				return 0;
			tm->data[MAZE_INDEX(x, y, tm->w)] = 0;
			return 1;
		} else {
			return 0;
		}
	}

	tm->data[MAZE_INDEX(x, y, tm->w)] = 0;
	/* Ge slumpen några försök */
	for (i = 0; i < 4 + (allow_exit * 16 * (min < 32)); i++) 
		switch (rand() & 0x3) {
			case 0:
				MAZE_TRY(-1, 0);
			case 1:
				if (x == 24)
					break;
				MAZE_TRY(0, -1);
			case 2:
				MAZE_TRY(1, 0);
			case 3:
				if (x == 24)
					break;
				MAZE_TRY(0, 1);
		}
	/* Vi missyckades, bara att backa tillbaka... */
	if (allow_exit)
		return !(tm->data[MAZE_INDEX(x, y, tm->w)] = 1);
	
	/* Om vi hittade ett giltigt nästa steg */
	found:
	
	tm->data[MAZE_INDEX(x, y, tm->w)] = 0;
	return 1;
}


void maze_generate(DARNIT_TILEMAP *tm) {
	int i, j, dx, dy;
	/* Först, generera vägen ut! */
	maze_generate_path(tm, 24, 13, 1, 0);

	for (i = 1; i < tm->w - 1; i++) 
		for (j = 1; j < tm->h - 1; j++)
			if (tm->data[MAZE_INDEX(i, j, tm->w)] == 0) {
				dx = (rand() & 1) * ((rand() & 1) * -1);
				if (!dx)
					dy = (rand() & 1) * ((rand() & 1) * -1);
				else
					dy = 0;
				maze_generate_path(tm, i + dx, j + dy, 0, 0);
			}

	return;
}

int maze_find(DARNIT_TILEMAP *tm, int x, int y) {
	if (x == 25 || y == 15 || x < 0 || y < 0)
		return 0;
	if (tm->data[MAZE_INDEX(x, y, tm->w)])
		return 0;

	tm->data[MAZE_INDEX(x, y, tm->w)] = MAZE_GOOD;

	if (x == 24 && y == 13);
	else if (x == 0 || y == 0 || x == 24 || y == 14)
		return 1;

	/* Om sant så ska försöket animeras */
	if (animate) {
		/* Tala om för libdarnit att labyrinten ändrats */
		d_tilemap_recalc(tm);
	
		render(tm);
	}

	if (maze_find(tm, x - 1, y))
		return 1;
	if (maze_find(tm, x, y + 1))
		return 1;
	if (maze_find(tm, x, y - 1))
		return 1;
	if (maze_find(tm, x + 1, y))
		return 1;
	
	tm->data[MAZE_INDEX(x, y, tm->w)] = MAZE_BAD;
	
	/* Om sant så ska försöket animeras */
	if (animate) {
		/* Tala om för libdarnit att labyrinten ändrats */
		d_tilemap_recalc(tm);
	
		render(tm);
	}
	
	return 0;
}
	

void maze_make(DARNIT_TILEMAP *tm) {
	/* Fyll labyrinten med hinder */
	memset(tm->data, 1, tm->w * tm->h * 4); 

	/* Generera gångar, bäde vägen ut och stumpar */
	maze_generate(tm);

	/* Tala om för libdarnit att labyrinten ändrats */
	d_tilemap_recalc(tm);
	
	/* Rendera en bildruta */
	render(tm);

	/* Leta efter vägen ut */
	maze_find(tm, 24, 13);
	
	/* Tala om för libdarnit att labyrinten ändrats */
	d_tilemap_recalc(tm);

	return;
}


int main(int argc, char **argv) {
	DARNIT_TILESHEET *ts;
	DARNIT_TILEMAP *tm;

	if (!d_init("algdata", "algdata", NULL))
		return -1;
	
	if (argc > 1)
		animate = 1;
	
	if (!(ts = d_render_tilesheet_load("maze_tex.png", 32, 32, DARNIT_PFORMAT_RGB5A1))) {
		fprintf(stderr, "maze_tex.png could not be loaded\n");
		d_quit();
	}

	if (!(tm = d_tilemap_new(0xFF, ts, 0xFF, 25, 15))) {
		fprintf(stderr, "Unable to create a tilemap\n");
		d_quit();
	}

	maze_make(tm);

	for (;;) {
		if (d_keys_get().start) {
			maze_make(tm);
			d_keys_set(d_keys_get());
		}
		
		render(tm);
	}

	d_quit();

	return 0;
}


