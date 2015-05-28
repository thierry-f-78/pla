/*
 * Copyright 2011-2015 Thierry FOURNIER
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version
 * 2 of the License.
 *
 */

#ifndef __LOAD_H__
#define __LOAD_H__

void pla_load(struct list_head *base, struct list_head *res, const char *file);
void pla_store(struct list_head *base, const char *file);

#endif /* __LOAD_H__ */
