#ifndef _IMG_H
#define _IMG_H

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <malloc.h>

#include "bmp.h"


struct img_pixel {
	uint8_t b, g, r;
};

struct img_image {
	struct bitmap_header header;
	//uint64_t width, height;
	struct img_pixel* pixels_data;
	
};

bool read_bitmap (FILE*, struct img_image*);
bool write_bitmap(FILE*, struct img_image*);
size_t calculate_padding(size_t);
void rotate_image(struct img_image*, struct img_image*);

#endif // _IMG_H
