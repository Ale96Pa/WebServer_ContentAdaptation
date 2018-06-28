#ifndef SERVER_CONTENTADAPTATION_ADAPT_IMAGE_H
#define SERVER_CONTENTADAPTATION_ADAPT_IMAGE_H

#include <wand/magick_wand.h>
#include "basics.h"

void find_image(char *name, char *path);
int compress_image(char *name, double q, char *destination, char *format);
FILE *open_file(void);
char *check_device(char *device, char *line);
int convert(char **array, int v);
int *find_value(char *dev, int *value);
void read_line(char *device, FILE *f, int *value);
void get_sizes(int dimensions[2]);



#endif //SERVER_CONTENTADAPTATION_ADAPT_IMAGE_H
