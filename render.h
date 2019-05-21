/*
 * Copyright 2011-2015 Thierry FOURNIER
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version
 * 2 of the License.
 *
 */
#include "pla.h"

#ifndef __RENDER_H__
#define __RENDER_H__

void pla_draw(int mode, const char *file_out, struct disp *d, enum language lng);

#endif /* __RENDER_H__ */
