//
// Created by ale96 on 07/04/18.
//

#ifndef SERVER_CONTENTADAPTATION_ADAPT_IMAGE_H
#define SERVER_CONTENTADAPTATION_ADAPT_IMAGE_H



#include <wand/magick_wand.h>
#include "basics.h"

void find_image(char *name, char *path);
void compress_image(char *name, double q, char *destination, char *format);

#endif //SERVER_CONTENTADAPTATION_ADAPT_IMAGE_H
