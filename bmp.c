
#include "include/bmp.h"

bool read_header(FILE* fin, struct bitmap_header* to) {
	if (!fin) return false;
	return fread(to, sizeof(struct bitmap_header), 1, fin);
}
bool write_header(FILE* fout, struct bitmap_header* from) {
	if (!fout) return false;
	return fwrite(from, sizeof(struct bitmap_header), 1, fout);
}
