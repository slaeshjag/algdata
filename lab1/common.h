#ifndef __COMMON_H__
#define	__COMMON_H__

#include <darnit/darnit.h>
#include <stdio.h>
#include <stdlib.h>

#define	MIN(x, y)		((x < y) ? (x) : (y))
#define	MAX(x, y)		((x > y) ? (x) : (y))

#define	PIXEL_HIGHLIGHT		0xF0F0F0
#define	PIXEL_SOLID		0xFF000000
#define	PIXEL_NONE		0xFFFFFFFF


int geometrics_image(DARNIT_IMAGE_DATA img, int *w, int *h);
void fill_image(DARNIT_IMAGE_DATA img, int x, int y, unsigned int replace, unsigned int color);

#endif
