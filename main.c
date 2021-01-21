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
size_t calculate_padding(size_t);

int main(int argc, char** argv) {
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

	
	uint8_t* shit2 = malloc(header.bfOffbits);
	fseek(bmp_file, 0, SEEK_SET);
	fread(shit2, header.bfOffbits - 1, 1, bmp_file);






	
	//long shitpos = ftell(bmp_file);
	//fseek(bmp_file, header.bfOffbits, SEEK_SET);
	//printf("shitpos: %"PRId64"\tcurrent: %"PRId64"\n", shitpos, ftell(bmp_file));
	//printf("shit bytes: %"PRId64, ftell(bmp_file) - shitpos);
	
	//fseek(bmp_file, header.bfOffbits, SEEK_SET);

	
	struct img_pixel* pixels = malloc(sizeof(struct img_pixel) * header.biWidth * header.biHeight);
	struct img_pixel pix;

	FORi0(header.biHeight) {
		//printf("Line: %zu\n", i);
		for (size_t j = 0; j < header.biWidth; j += 1) {
			fread(&pix, sizeof(struct img_pixel), 1, bmp_file);
			// printf(
			// 	"%"PRId8"\t"
			// 	"%"PRId8"\t"
			// 	"%"PRId8"\t",
			// 	pix.r,
			// 	pix.g,
			// 	pix.b
			// );
			// printf("\n");

			pixels[i*header.biWidth + j] = pix;
		}
		
		uint8_t padding;
		fread(&padding, 1, calculate_padding(header.biWidth), bmp_file);
	}

	uint8_t byte;
	while(fread(&byte, sizeof(byte), 1, bmp_file)) printf("0x%"PRIx8"\n", byte);
	
	fclose(bmp_file);

	FILE* h = fopen("header.bin", "wb");
	fwrite(&header, sizeof(header), 1, h);
	fclose(h);

	FILE* rotated = fopen("rot.bmp", "wb");
	struct img_pixel* rpx = malloc(sizeof(struct img_pixel) * header.biWidth * header.biHeight);

	rotate_pixels(pixels, rpx, header.biWidth, header.biHeight);
	//fwrite(&header, sizeof(header), 1, rotated);
	fwrite(shit2, sizeof(header.bfOffbits), 1, rotated);
	FORi0(header.biHeight) {
		fwrite((rpx + i*header.biWidth), sizeof(struct img_pixel), header.biWidth, rotated);
		uint8_t padding = 0;
		fwrite(&padding, 1, calculate_padding(header.biHeight), rotated);
	}


	fclose(rotated);
	free(rpx);
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
size_t calculate_padding(size_t w) {
	return w % 4 == 0 ? 0 : 4 - w % 4;
}
