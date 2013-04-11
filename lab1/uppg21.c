#include "common.h"


typedef struct IMG_STRUCT {
	DARNIT_TILESHEET	*ts;
	int			piczels;
	struct IMG_STRUCT	*next;
} IMG_STRUCT;


DARNIT_TILESHEET *copy_image(DARNIT_IMAGE_DATA img, int x, int y, int w, int h, int *piczels) {
	unsigned int *pixbuf;
	DARNIT_TILESHEET *ts;
	int i, j;

	if (!(pixbuf = malloc(sizeof(unsigned int) * w * h)))
		return NULL;
	if (!(ts = d_render_tilesheet_new(1, 1, w, h, DARNIT_PFORMAT_RGB5A1))) {
		free(pixbuf);
		return NULL;
	}

	for (i = 0; i < w; i++)
		for (j = 0; j < h; j++)
				if (img.data[(i + x) + (j + y) * img.w] == PIXEL_HIGHLIGHT) {
					pixbuf[i + j * w] = PIXEL_SOLID;
					(*piczels)++;
				} else
					pixbuf[i + j * w] = PIXEL_NONE;
	d_render_tilesheet_update(ts, 0, 0, w, h, pixbuf);
	free(pixbuf);

	return ts;
}



DARNIT_TILESHEET *detect_image(DARNIT_IMAGE_DATA img, int x, int y, int *piczels) {
	int minx, w, h;
	DARNIT_TILESHEET *ts;

	fill_image(img, x, y, PIXEL_SOLID, PIXEL_HIGHLIGHT);
	minx = geometrics_image(img, &w, &h);
	ts = copy_image(img, minx, y, w, h, piczels);
	fill_image(img, x, y, PIXEL_HIGHLIGHT, PIXEL_NONE);

	return ts;
}


DARNIT_TILESHEET *next_image(DARNIT_IMAGE_DATA img, int *piczels) {
	int i, j;
	
	for (i = 0; i < img.w; i++)
		for (j = 0; j < img.h; j++)
			if (img.data[i + j * img.w] == PIXEL_SOLID)
				return detect_image(img, i, j, piczels);
	return NULL;
}


IMG_STRUCT *new_image(DARNIT_TILESHEET *ts, int piczels) {
	IMG_STRUCT *img;

	if (!(img = malloc(sizeof(IMG_STRUCT))))
		return NULL;
	img->ts = ts;
	img->piczels = piczels;

	return img;
}


void sort_image(IMG_STRUCT **img, int img_size) {
	IMG_STRUCT *tmp;
	int i, j;

	for (i = 0; i < img_size; i++)
		for (j = i; j > 0 && img[j]->piczels > img[j-1]->piczels; j--) {
			tmp = img[j];
			img[j] = img[j-1];
			img[j-1] = tmp;
		}
	return;
}


int main(int argc, char **argv) {
	DARNIT_IMAGE_DATA img;
	DARNIT_TILESHEET *ts;
	IMG_STRUCT **imgs, **tmp;
	DARNIT_KEYS keys;
	int images, image_disp, i;

	imgs = NULL;
	images = 0;
	image_disp = 0;

	if (argc < 2) {
		fprintf(stderr, "Usage: %s <image to analyze>\n", argv[0]);
		return -1;
	}

	d_init("algdata", "algdata", NULL);
	if (!((img = d_img_load_raw(argv[1])).data)) {
		fprintf(stderr, "Unable to load %s\n", argv[1]);
		d_quit();
	}

	d_render_clearcolor_set(255, 255, 255);
	do {
		i = 0;
		ts = next_image(img, &i);
		if (ts) {
			if (!(tmp = realloc(imgs, sizeof(IMG_STRUCT **) * (images + 1))))
				d_quit();
			imgs = tmp;
			imgs[images] = new_image(ts, i);
			images++;
		}
	} while (ts);

	if (!imgs)
		d_quit();

	sort_image(imgs, images);

	for (;;) {
		keys = d_keys_get();
		if (keys.start) {
			image_disp = (image_disp + 1 >= images) ? 0 : image_disp + 1;
			d_keys_set(keys);
		}
			
		d_render_begin();

		d_render_tile_blit(imgs[image_disp]->ts, 0, 0, 0);

		d_render_end();
		d_loop();
	}

	return 0;
}
