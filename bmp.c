
#include "include/bmp.h"

bool read_header(FILE* fp, struct bitmap_header* read_to) {
	if (!fp) return false;
	return fread(read_to, sizeof(struct bitmap_header), 1, fp);
}
