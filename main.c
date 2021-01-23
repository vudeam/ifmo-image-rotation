#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <errno.h>
#include <malloc.h>

//#include "include/bmp.h"
#include "include/err.h"
#include "include/img.h"

#define FORi(start, stop) for (size_t i = start; i < stop; i += 1)
#define FORi0(stop) FORi(0, stop)



int main(int argc, char** argv) {

	struct img_image our_image = { 0 };

	if (argc == 1) {
		errno = NO_FILENAME;
		perror("Specify file name");
		exit(NO_FILENAME);
	}

	//struct bitmap_header header;

	FILE* bmp_file = fopen(argv[1], "rb");
	if (!bmp_file) {
		perror("File failure");
		exit(EXIT_FAILURE);
	}

	if (!read_bitmap(bmp_file, &our_image)) {
		perror("File failure");
		exit(EXIT_FAILURE);
	}

	fclose(bmp_file);

	printf("BMP file magic: %"PRIx16"\n", our_image.header.bfType);
	printf("File size: %"PRId32"\n", our_image.header.bfSize);
	printf("Offbits: %"PRId32" (0x%"PRIx32")\n", our_image.header.bfOffbits, our_image.header.bfOffbits);
	printf("Resolution: %"PRId32"x%"PRId32"\n", our_image.header.biWidth, our_image.header.biHeight);
	printf("Padding (horizontal): %zu\n", calculate_padding(our_image.header.biWidth));


	struct img_image our_rotated = { 0 };
	our_rotated.header = our_image.header;
	our_rotated.header.biWidth  = our_image.header.biHeight;
	our_rotated.header.biHeight = our_rotated.header.biWidth;

	// hoping to rotate
	//rotate_image(&our_image, &our_rotated);
	our_rotated.pixels_data = malloc(sizeof(struct img_pixel) * our_rotated.header.biWidth * our_rotated.header.biHeight);
	FORi0(our_rotated.header.biWidth * our_rotated.header.biHeight)
		our_rotated.pixels_data[i] = our_image.pixels_data[i];

	FILE* bmp_rotated = fopen("rot.bmp", "wb");
	fwrite(&our_image.header, sizeof(struct bitmap_header), 1, bmp_rotated);
	FORi0(our_image.header.biHeight) {
		fwrite(
			&our_image.pixels_data[i * our_image.header.biWidth],
			sizeof(struct img_pixel),
			our_image.header.biWidth,
			bmp_rotated
		);

		uint8_t _p = 0x0;
		fwrite(&_p, 1, calculate_padding(our_image.header.biWidth), bmp_rotated);
	}
	fclose(bmp_rotated);





	/*
	//write_bitmap(bmp_rotated, &our_rotated);
	fwrite(&our_rotated.header, sizeof(struct bitmap_header), 1, bmp_rotated);
	FORi0(our_rotated.header.biHeight) {
		fwrite(
			(our_rotated.pixels_data + i*our_rotated.header.biWidth),
			sizeof(struct img_pixel) * our_rotated.header.biWidth,
			our_rotated.header.biWidth,
			bmp_rotated
		);

		uint8_t _p = 0x0;
		fwrite(&_p, 1, calculate_padding(our_rotated.header.biWidth), bmp_rotated);
	}
	*/

	free(our_image.pixels_data);
	free(our_rotated.pixels_data);

#if 0
	
	//uint8_t* shit2 = malloc(header.bfOffbits);
	//fseek(bmp_file, 0, SEEK_SET);
	//fread(shit2, header.bfOffbits - 1, 1, bmp_file);

	
	//long shitpos = ftell(bmp_file);
	//fseek(bmp_file, header.bfOffbits, SEEK_SET);
	//printf("shitpos: %"PRId64"\tcurrent: %"PRId64"\n", shitpos, ftell(bmp_file));
	//printf("shit bytes: %"PRId64, ftell(bmp_file) - shitpos);
	
	fseek(bmp_file, header.bfOffbits, SEEK_SET);

	/*
	struct img_pixel* pixels = malloc(sizeof(struct img_pixel) * header.biWidth * header.biHeight);
	//struct img_pixel pix;

	FORi0(header.biHeight) {
		//printf("Line: %zu\n", i);
		for (size_t j = 0; j < header.biWidth; j += 1) {
			fread(&pixels[i*header.biWidth + j], sizeof(struct img_pixel), 1, bmp_file);
			// printf(
			// 	"%"PRId8"\t"
			// 	"%"PRId8"\t"
			// 	"%"PRId8"\t",
			// 	pix.r,
			// 	pix.g,
			// 	pix.b
			// );
			// printf("\n");

			//pixels[i*header.biWidth + j] = pix;
		}
		
		uint8_t padding;
		fread(&padding, 1, calculate_padding(header.biWidth), bmp_file);
	}
	*/
	struct img_image our_img = { 0 };
	puts("READING................................");
	if (!read_bitmap(bmp_file, &our_img, header)) {
		fprintf(stderr, "Error reading bitmap\n");
		exit(EXIT_FAILURE);
	}

	uint8_t byte;
	while(fread(&byte, sizeof(byte), 1, bmp_file)) printf("EXTRA BYTE:\t0x%"PRIx8"\n", byte);
	
	printf(
		"Read image: %"PRIu32"x%"PRIu32", %zu pixels\n",
		our_img.width,
		our_img.height,
		sizeof(our_img.pixels_data)
	);

	fclose(bmp_file);


	FILE* h = fopen("header.bin", "wb");
	write_header(h, &header);
	fclose(h);

	FILE* rotated = fopen("rot.bmp", "wb");
	struct img_pixel* rpx = malloc(sizeof(struct img_pixel) * header.biWidth * header.biHeight);

	rotate_pixels(our_img.pixels_data, rpx, header.biWidth, header.biHeight);
	

	uint32_t buf_width = header.biWidth;
	header.biWidth = header.biHeight;
	header.biHeight = buf_width;
	fwrite(&header, sizeof(header), 1, rotated);
	//fwrite(shit2, sizeof(header.bfOffbits), 1, rotated);
	
	FORi0(header.biHeight) { // !
		fwrite((rpx + i*header.biWidth), sizeof(struct img_pixel), header.biWidth, rotated);
		uint8_t padding = 0;
		fwrite(&padding, 1, calculate_padding(header.biHeight), rotated);
	}


	fclose(rotated);
	free(rpx);
	free(our_img.pixels_data);
#endif

	puts("\n");
	return 0;
}
