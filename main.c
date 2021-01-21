#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <errno.h>
#include <malloc.h>

#include "include/bmp.h"
#include "include/err.h"
#include "include/img.h"

#define FORi(start, stop) for (size_t i = start; i < stop; i += 1)
#define FORi0(stop) FORi(0, stop)


void rotate_pixels(struct img_pixel*, struct img_pixel*, uint32_t, uint32_t);
void rotate_arr(uint64_t*, uint64_t*, uint32_t, uint32_t);

uint64_t arr[] = {
	1,  2,  3,  4,
	5,  6,  7,  8,
	9,  10, 11, 12,
	13, 14, 15, 16
};


int main(int argc, char** argv) {

	/*
	for (uint32_t y = 0; y < 4; y += 1) {
		for (uint32_t x = 0; x < 4; x += 1) {
			printf("%"PRIu64"\t", arr[y*4 + x]);
		}
		printf("\n");
	}
	uint64_t* rotated = malloc(16);
	rotate_arr(arr, rotated, 4, 4);
	for (uint32_t y = 0; y < 4; y += 1) {
		for (uint32_t x = 0; x < 4; x += 1) {
			printf("%"PRIu64"\t", rotated[y*4 + x]);
		}
		printf("\n");
	}
	*/


	if (argc == 1) {
		errno = NO_FILENAME;
		perror("Specify file name");
		exit(NO_FILENAME);
	}

	struct bitmap_header header;

	FILE* bmp_file = fopen(argv[1], "rb");
	if (!bmp_file) {
		perror("File failure");
		exit(EXIT_FAILURE);
	}

	if (!read_header(bmp_file, &header)) {
		perror("File failure");
		exit(EXIT_FAILURE);
	}

	printf("File magic: %"PRIx16"\n", header.bfType);
	printf("File size: %"PRId32"\n", header.bfSize);
	printf("Offbits: %"PRId32" (0x%"PRIx32")\n", header.bfOffbits, header.bfOffbits);
	printf("%"PRId32"x%"PRId32"\n", header.biWidth, header.biHeight);
	
	long shitpos = ftell(bmp_file);
	fseek(bmp_file, header.bfOffbits, SEEK_SET);
	printf("shitpos: %"PRId64"\tcurrent: %"PRId64"\n", shitpos, ftell(bmp_file));
	//printf("shit bytes: %"PRId64, ftell(bmp_file) - shitpos);
	
	//fseek(bmp_file, header.bfOffbits, SEEK_SET);
	
	struct img_pixel* pixels = malloc(sizeof(struct img_pixel) * header.biWidth * header.biHeight);
	struct img_pixel pix;

	FORi0(header.biHeight) {
		printf("Line: %zu\n", i);
		for (size_t j = 0; j < header.biWidth; j += 1) {
			fread(&pix, sizeof(struct img_pixel), 1, bmp_file);
			printf(
				"%"PRId8"\t"
				"%"PRId8"\t"
				"%"PRId8"\t",
				pix.r,
				pix.g,
				pix.b
			);
			printf("\n");

			pixels[i*header.biWidth + j] = pix;
		}
		
		uint16_t padding;
		fread(&padding, 2, 1, bmp_file);
	}

	uint8_t byte;
	while(fread(&byte, sizeof(byte), 1, bmp_file)) printf("0x%"PRIx8"\n", byte);

	FILE* h = fopen("header.bin", "wb");
	fwrite(&header, sizeof(header), 1, h);
	fclose(h);

	FILE* rotated = fopen("rot.bmp", "wb");
	struct img_pixel* rpx = malloc(sizeof(struct img_pixel) * header.biWidth * header.biHeight);

	rotate_pixels(pixels, rpx, header.biWidth, header.biHeight);
	fwrite(&header, sizeof(header), 1, rotated);
	//fwrite(rpx,header.biWidth * header.biHeight, 1, rotated);
	FORi0(header.biHeight) {
		//fwrite((rotated + i*), sizeof(struct img_pixel) * header.biWidth, header.biWidth, rotated );
		for (size_t j = 0; j < header.biWidth; j += 1) {
			fwrite(&rpx[header.biHeight * i + j], sizeof(struct img_pixel), 1, rotated);
		}
		uint16_t padding = 0;
		fwrite(&padding, 2, 1, rotated);
	}


	fclose(rotated);
	free(rpx);


	/*
	for (size_t i = 0; i < 22; i += 1) {
		printf("pix %zu:\n", i+1);
		struct img_pixel pix;
		fread(&pix, sizeof(struct img_pixel), 1, bmp_file);
		printf(
			"%"PRId8"\t"
			"%"PRId8"\t"
			"%"PRId8"\t",
			pix.r,
			pix.g,
			pix.b
		);
		printf("\n");
	}
	uint16_t padding;
	fread(&padding, 2, 1, bmp_file);
	for (size_t i = 0; i < 22; i += 1) {
		printf("pix %zu:\n", i+1);
		struct img_pixel pix;
		fread(&pix, sizeof(struct img_pixel), 1, bmp_file);
		printf(
			"%"PRId8"\t"
			"%"PRId8"\t"
			"%"PRId8"\t",
			pix.r,
			pix.g,
			pix.b
		);
		printf("\n");
	}
	*/

	fclose(bmp_file);
	free(pixels);

	puts("\n");
	return 0;
}

void rotate_pixels(struct img_pixel* from, struct img_pixel* to, uint32_t w, uint32_t h) {
	for (uint32_t y = 0; y < h; y += 1) {
		for (uint32_t x = 0; x < w; x += 1) {
			to[(w*h - w) + y - (x*w)] = from[y*w + x];
			//to[y*w + x] = from[(w*h - w) + y - (x*w)];
		}
	}
}
void rotate_arr(uint64_t* from, uint64_t* to, uint32_t w, uint32_t h) {
	for (uint32_t y = 0; y < h; y += 1) {
		for (uint32_t x = 0; x < w; x += 1) {
			to[y*w + x] = from[(w*h - w) + y - (x*w)];
		}
	}
}
