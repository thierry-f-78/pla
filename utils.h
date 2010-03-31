#ifndef __UTILS_H__
#define __UTILS_H__

#include <cairo.h>

#include "pla.h"

void convert_rgba_hex(const char *hex, unsigned char alpha, struct color *out);
void cairo_set_source_col(cairo_t *c, const struct color *col);
void col_dark(struct color *in, struct color *out, double dark);
double max(double *in, int nb);
cairo_status_t cairo_wr(void *closure, const unsigned char *data, unsigned int length);

#endif /* __UTILS_H__ */
