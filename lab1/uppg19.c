#include "common.h"

void image_process(DARNIT_IMAGE_DATA img) {
	int i, j;

	for (i = 0; i < img.w; i++)
		for (j = 0; j < img.h; j++)
			if (img.data[i + j * img.w] == PIXEL_SOLID)
				fill_image(img, i, j, PIXEL_SOLID, (rand() & 0x7F7F7F) | 1);
	return;
}


int main(int argc, char **argv) {
	DARNIT_IMAGE_DATA img;
	DARNIT_TILESHEET *ts;

	if (argc < 2) {
		fprintf(stderr, "Usage: %s <iamge to process>\n");
		return -1;
	}
	
	d_init("algdata", "algdata", NULL);
	
	if (!(img = d_img_load_raw(argv[1])).data)
		return -1;

	ts = d_render_tilesheet_new(1, 1, img.w, img.h, DARNIT_PFORMAT_RGB5A1);
	image_process(img);
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
