#ifndef _IMG_H
#define _IMG_H

#include <stdint.h>

struct img_pixel {
	uint8_t b, g, r;
};


struct img_image {
	uint64_t width, height;
	struct img_pixel* pixel_data;
};

#endif // _IMG_H
