#ifndef _BMP_H
#define _BMP_H

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

struct __attribute__((packed))
bitmap_header {
	uint16_t bfType;
	uint32_t bfSize;
	uint16_t bfReserved1, bfReserved2;
	uint32_t bfOffbits;

	uint32_t biSize;
	uint32_t biWidth;
	uint32_t biHeight;
	uint16_t biPlanes;
	uint16_t biBitCount;
	uint32_t biCompression;
	uint32_t biSizeImage;
	uint32_t biXPelsPerMeter;
	uint32_t biYPelsPerMeter;
	uint32_t biClrUsed;
	uint32_t biClrImportant;
};


bool read_header(FILE*, struct bitmap_header*);

#endif // _BMP_H
