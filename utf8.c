/*
 * Copyright 2011-2015 Thierry FOURNIER
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version
 * 2 of the License.
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int utf8_pos(char *s, int len) {
	int i = 0;
	int j = 0;

	while (s[i] != '\0') {
		if ((s[i] & 0xc0) != 0x80)
			j++;
		if (j == len)
			return i;
		i++;
	}

	if (s[i] == '\0')
		return -1;

	return i;
}

int next_utf8(const char *s, unsigned int *c) {
	const char *p;
	int dec = 0;
	int chars = 0;

	for (p = s; *p != 0; p++) {

		/* decode extra chars */
		if (dec > 0) {

			/* need 0b10 for the 2 first bits */
			if ( ( *p & 0xc0 ) != 0x80 )
				return -1;

			/* add data at char */
			*c = ( *c << 6 ) | ( *p & 0x3f );

			dec--;

			if (dec > 0)
				continue;

			/* invalid range */
			switch (*c) {
			case 0xd800 ... 0xdfff:
			case 0xfffe ... 0xffff:
				return -1;
			}

			/* overlong
			 * chars == 1 : 5 + 6         : 11 : 0x80    ... 0x7ff
			 * chars == 2 : 4 + 6 + 6     : 16 : 0x800   ... 0xffff
			 * chars == 3 : 3 + 6 + 6 + 6 : 21 : 0x10000 ... 0x1fffff
			 */
			switch (*c) {

			case 0x00080 ... 0x0007ff:
				if (chars != 2)
					return -1;
				break;

			case 0x00800 ... 0x00ffff:
				if (chars != 3)
					return -1;
				break;

			case 0x10000 ... 0x1fffff:
				if (chars != 4)
					return -1;
				break;
			}
		}

		else {

			/* 0xf8
			 * 0.... > 0x00 <= 0x7f
			 * 10... > 0x7f <= 0xbf interdit
			 * 110.. > 0xbf <= 0xdf
			 * 1110. > 0xdf <= 0xef
			 * 11110 > 0xef <= 0xf7
			 */
			switch (*p & 0xf8) {
	
			/* 0.... > 0x00 <= 0x7f */
			case 0x00 ... 0x7f:
				*c = *p;
				chars = 1;
				break;
	
			/* 10... > 0x7f <= 0xbf interdit */
			case 0x80 ... 0xbf:
				return -1;
	
			/* 110.. > 0xbf <= 0xdf */
			case 0xc0 ... 0xdf:
				dec = 1;
				chars = 2;
				*c = *p & 0x1f;
				continue;
	
			/* 1110. > 0xdf <= 0xef */
			case 0xe0 ... 0xef:
				dec = 2;
				chars = 3;
				*c = *p & 0x0f;
				continue;
	
			/* 11110 > 0xef <= 0xf7 */
			case 0xf0 ... 0xf7:
				dec = 3;
				chars = 4;
				*c = *p & 0x07;
				continue;
	
			default:
				return -1;
			}
		}

		return chars;
	}

	/* si on a fini, mais qu'il reste du decodage, c'est invalide */
	if (dec > 0)
		return -1;

	return chars;
}

int utf8_is_valid(char *s) {
	char *p;
	unsigned int c;
	int ret;

	p = s;
	c = 0;

	while (1) {

		ret = next_utf8(p, &c);

		if (ret == -1)
			return 0;

		p += ret;
		if (*p == '\0')
			break;
	}

	return 1;
}

