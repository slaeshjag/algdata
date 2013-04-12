#include <darnit/darnit.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


void mergesort(unsigned int *data, int pivot, int size, unsigned int *scratchpad) {
	int i, j, k;

	for (i = j = k = 0; i < size; i++) {
		if (((k + pivot) < size && data[j] < data[k + pivot]) || j >= pivot) {
			scratchpad[i] = data[k + pivot];
			k++;
		} else {
			scratchpad[i] = data[j];
			j++;
		}
	}

	memcpy(data, scratchpad, size * sizeof(unsigned int));
	
	return;
}

void image_sort_do(unsigned int *data, unsigned int size, unsigned int *scratchpad) {
	if (size <= 1)
		return;

	image_sort_do(data, size >> 1, scratchpad);
	image_sort_do(data + (size >> 1), (size >> 1) + (size & 1), scratchpad);

	mergesort(data, size >> 1, size, scratchpad);

	return;
}


void image_sort(unsigned int *data, int size) {
	unsigned int *scratchpad;

	/* Allokeringar är *svindyra*. Därför är det bäst att bara allokera en gång. 	*
	 * To hell med andra program som vill ha minne under tiden 			*/
	if (!(scratchpad = malloc(sizeof(unsigned int) * size)))
		return;

	image_sort_do(data, size, scratchpad);

	/* Nu behöver vi inte den längre. */
	free(scratchpad);

	return;
}


int main(int argc, char **argv) {
	DARNIT_TILESHEET *ts;
	DARNIT_IMAGE_DATA img;

	if (argc < 2) {
		fprintf(stderr, "Usage: %s <image file to process>\n", argv[1]);
		return -1;
	}
	
	d_init("algdata", "algdata", NULL);
	d_render_clearcolor_set(200, 200, 255);

	if (!(img = d_img_load_raw(argv[1])).data)
		return -1;
	
	image_sort(img.data, img.w * img.h);
	
	ts = d_render_tilesheet_new(1, 1, img.w, img.h, DARNIT_PFORMAT_RGB5A1);
	d_render_tilesheet_update(ts, 0, 0, img.w, img.h, img.data);

	for (;;) {
		d_render_begin();
		d_render_tile_blit(ts, 0, 0, 0);
		d_render_end();

		d_loop();
	}

	d_quit();

	return 0;
}
