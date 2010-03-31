#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <cairo.h>
#include <cairo-ps.h>
#include <cairo-pdf.h>
#include <cairo-svg.h>

#include "pla.h"

static inline
int hex_to_int(char c)
{
	if (c >= '0' && c <='9')
		return c - '0';
	else if (c >= 'A' && c <= 'F')
		return c + 10 - 'A';
	else if (c >= 'a' && c <= 'f')
		return c + 10 - 'a';
	return 0;
}

void convert_rgba_hex(const char *hex, unsigned char alpha, struct color *out)
{
	if (hex == NULL)
		return;

	if (hex[0] == '#')
		hex++;
	
	if (strlen(hex) != 6) {
		out->r = 0;
		out->g = 0;
		out->b = 0;
		out->a = 0;
		return;
	}
	
	out->r = ( hex_to_int(hex[0]) * 16.0f ) + hex_to_int(hex[1]);
	out->g = ( hex_to_int(hex[2]) * 16.0f ) + hex_to_int(hex[3]);
	out->b = ( hex_to_int(hex[4]) * 16.0f ) + hex_to_int(hex[5]);
	out->a = alpha;

	out->r /= 255.0f;
	out->g /= 255.0f;
	out->b /= 255.0f;
	out->a /= 255.0f;
}

void cairo_set_source_col(cairo_t *c, const struct color *col)
{
	cairo_set_source_rgba(c, col->r, col->g, col->b, col->a);
}

void col_dark(struct color *in, struct color *out, double dark) {
	out->r = in->r * dark;
	out->g = in->g * dark;
	out->b = in->b * dark;
	out->a = in->a;
}

double max(double *in, int nb)
{
	double ret = 0;
	int i;

	for (i=0; i<nb; i++)
		if (in[i] > ret)
			ret = in[i];
	return ret;
}

cairo_status_t cairo_wr(void *closure, const unsigned char *data, unsigned int length)
{
	FILE *out = closure;

	fwrite(data, 1, length, out);

	return CAIRO_STATUS_SUCCESS;
}
