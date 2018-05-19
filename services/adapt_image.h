#ifndef SERVER_CONTENTADAPTATION_ADAPT_IMAGE_H
#define SERVER_CONTENTADAPTATION_ADAPT_IMAGE_H

#include <wand/magick_wand.h>
#include "basics.h"

#define DIM_PATH 100

void find_image(char *name, char *path);
void compress_image(char *name, double q, char *destination, char *format);

#endif //SERVER_CONTENTADAPTATION_ADAPT_IMAGE_H
