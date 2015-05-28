/*
 * Copyright 2011-2015 Thierry FOURNIER
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version
 * 2 of the License.
 *
 */

#ifndef __UTF8_H__
#define __UTF8_H__

int utf8_pos(char *s, int len);
int utf8_is_valid(char *s);
int next_utf8(const char *s, unsigned int *c);

#endif
