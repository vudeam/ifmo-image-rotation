#include "include/img.h"

size_t calculate_padding(size_t w) { return w % 4 == 0 ? 0 : 4 - w % 4; }

bool read_bitmap(FILE* fin, struct img_image* i) {
	if (!fin || !i) return false;
	
	read_header(fin, &i->header);

	i->pixels_data = malloc(sizeof(struct img_pixel) * i->header.biWidth * i->header.biHeight);

	fseek(fin, i->header.bfOffbits, SEEK_SET);
	for (size_t y = 0; y < i->header.biHeight; y += 1) {

		// fread 1 pixel to array
		//for (size_t x = 0; x < i->header.biWidth; x += 1)
			//fread(&i->pixels_data[y*i->header.biWidth + x], sizeof(struct img_pixel), 1, fin);
		fread(
			&i->pixels_data[y * i->header.biHeight], ///?????
			sizeof(struct img_pixel),
			i->header.biWidth,
			fin
		);

		// garbage byte(-s)
		uint8_t _padding;
		fread(&_padding, sizeof(_padding), calculate_padding(i->header.biWidth), fin);
	}

	return true;
}

void rotate_pixels(struct img_pixel* from, struct img_pixel* to, uint32_t w, uint32_t h) {
	for (uint32_t y = 0; y < h; y += 1) {
		for (uint32_t x = 0; x < w; x += 1) {
			to[(w*h - w) + y - (x*w)] = from[y*w + x];
			//to[y*w + x] = from[(w*h - w) + y - (x*w)];
		}
	}
}
void rotate_pixels_new(struct img_pixel from[], struct img_pixel to[], uint32_t w, uint32_t h) {
	//int m = 3, n = 3; // h = m, w = n
	int64_t x = 0;
	for (int64_t i = 0; i < w; i += 1) {
		for(int64_t j = h-1; j >= 0; j -= 1) {
			*( to + i*h + (x++)) = *(from + j*w + i);
		}
		x = 0;
	}
}

void rotate_image(struct img_image* orig, struct img_image* to) {
	to->pixels_data = malloc(sizeof(struct img_pixel) * orig->header.biWidth * orig->header.biHeight);
	rotate_pixels_new(
		orig->pixels_data,
		to->pixels_data,
		orig->header.biWidth,
		orig->header.biHeight
	);
}
bool write_bitmap(FILE* fout, struct img_image* i) {
	if (!fout || !i) return false;
	if (!write_header(fout, &i->header)) return false;

	// TODO: consider writing darbage bits (between header and pixels)

	for (size_t y = 0; y < i->header.biHeight; y += 1) {
		fwrite(
			(i->pixels_data + y*i->header.biWidth),
			sizeof(struct img_pixel),
			i->header.biWidth,
			fout
		);

		uint8_t _padding = 0x0;
		fwrite(&_padding, 1, calculate_padding(i->header.biWidth), fout);
	}

	// FORi0(header.biHeight) { // !
	// 	fwrite((rpx + i*header.biWidth), sizeof(struct img_pixel), header.biWidth, rotated);
	// 	uint8_t padding = 0;
	// 	fwrite(&padding, 1, calculate_padding(header.biHeight), rotated);
	// }
	// fwrite
	// 	i->pixels_data,
	// 	sizeof(struct img_pixel) * i->header.biWidth * i->header.biHeight,
	// 	1,
	// 	fout
	// );

	return true;
}
