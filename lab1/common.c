#include "common.h"

/* Returnerar minsta X */
int geometrics_image(DARNIT_IMAGE_DATA img, int *w, int *h) {
	int minx, miny, maxx, maxy, i, j;

	minx = miny = MAX(img.w, img.h);
	maxx = maxy = 0;

	for (i = 0; i < img.w; i++)
		for (j = 0; j < img.h; j++)
			if (img.data[img.w * j + i] == PIXEL_HIGHLIGHT) {
				minx = MIN(minx, i);
				miny = MIN(miny, j);
				maxx = MAX(maxx, i);
				maxy = MAX(maxy, j);
			}
	*w = maxx - minx + 1;
	*h = maxy - miny + 1;

	return minx;
}


void fill_image(DARNIT_IMAGE_DATA img, int x, int y, unsigned int replace, unsigned int color) {
	unsigned int replace_this;

	if (x < 0 || y < 0)
		return;
	if (x >= img.w || y >= img.h)
		return;
	
	if (img.data[x + y * img.w] != replace)
		return;
	img.data[x + y * img.w] = color;
	
	fill_image(img, x - 1, y, replace, color);
	fill_image(img, x, y - 1, replace, color);
	fill_image(img, x + 1, y, replace, color);
	fill_image(img, x, y + 1, replace, color);

	return;
}
