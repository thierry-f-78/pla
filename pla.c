/*
 * Copyright 2011-2015 Thierry FOURNIER
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version
 * 2 of the License.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "list.h"
#include "utils.h"
#include "pla.h"

int conv(const char *in, int len)
{
	char buf[20];
	char *error;
	int ret;

	if (len > 19)
		return -1;

	memcpy(buf, in, len);
	buf[len] = 0;

	ret = strtol(buf, &error, 10);
	if (*error != 0)
		return -1;

	return ret;
}

struct res *pla_res_new(struct list_head *base, const char *name)
{
	struct res *r;

	/* init */
	r = calloc(sizeof(struct res), 1);
	if (name != NULL)
		r->name = strdup(name);

	/* chain */
	list_add_tail(&r->c, base);

	return r;
}

void pla_res_set_name(struct res *res, const char *name)
{
	if (name != NULL)
		res->name = strdup(name);
}

struct res *pla_res_get_by_name(struct list_head *base, const char *name)
{
	struct res *r;

	list_for_each_entry(r, base, c) {
		if (strcmp(r->name, name) == 0)
			return r;
	}

	return NULL;
}

struct task *pla_task_new(struct list_head *base, const char *name, const char *color,
                          time_t start, unsigned int duration)
{
	struct task *t;

	/* init */
	t = calloc(sizeof(struct task), 1);
	INIT_LIST_HEAD(&t->_child);
	INIT_LIST_HEAD(&t->childs);

	/* chain */
	list_add_tail(&t->c, base);

	/* fill name */
	if (name != NULL)
		t->name = strdup(name);

	/* fill color */
	if (color != NULL)
		convert_rgba_hex(color, 0xff, &t->color);
	else
		convert_rgba_hex("#8CB6CE", 0xff, &t->color);

	/* set dates */
	t->start = start;
	t->duration = duration;

	return t;
}

void pla_task_set_start(struct task *task, time_t start)
{
	task->start = start;
}

int pla_task_set_start_ymd(struct task *task, const char *start)
{
	struct tm tm;

	if (strlen(start) != 8)
		return -1;

	memset(&tm, 0, sizeof(struct tm));

	tm.tm_year = conv(start, 4) - 1900;
	if (tm.tm_year < 0)
		return -1;

	tm.tm_mon = conv(start + 4, 2) - 1;
	if (tm.tm_mon < 0)
		return -1;

	tm.tm_mday = conv(start + 6, 2);
	if (tm.tm_mday < 0)
		return -1;

	task->start = mktime(&tm);

	return 0;
}

int pla_task_set_start_ymdd(struct task *task, const char *start)
{
	struct tm tm;

	if (strlen(start) != 10)
		return -1;

	if (start[4] != '-' || start[7] != '-')
		return -1;

	memset(&tm, 0, sizeof(struct tm));

	tm.tm_year = conv(start, 4) - 1900;
	if (tm.tm_year < 0)
		return -1;

	tm.tm_mon = conv(start + 5, 2) - 1;
	if (tm.tm_mon < 0)
		return -1;

	tm.tm_mday = conv(start + 8, 2);
	if (tm.tm_mday < 0)
		return -1;

	task->start = mktime(&tm);

	return 0;
}

int pla_task_set_start_ymdh(struct task *task, const char *start)
{
	struct tm tm;

	if (strlen(start) != 10)
		return -1;

	memset(&tm, 0, sizeof(struct tm));

	tm.tm_year = conv(start, 4) - 1900;
	if (tm.tm_year < 0)
		return -1;

	tm.tm_mon = conv(start + 4, 2) - 1;
	if (tm.tm_mon < 0)
		return -1;

	tm.tm_mday = conv(start + 6, 2);
	if (tm.tm_mday < 0)
		return -1;

	tm.tm_hour = conv(start + 8, 2);
	if (tm.tm_mday < 0)
		return -1;

	task->start = mktime(&tm);

	return 0;
}

int pla_task_set_start_ymdhh(struct task *task, const char *start)
{
	struct tm tm;

	if (strlen(start) != 13)
		return -1;

	memset(&tm, 0, sizeof(struct tm));

	if (start[4] != '-' || start[7] != '-' || start[10] != ' ')
		return -1;

	tm.tm_year = conv(start, 4) - 1900;
	if (tm.tm_year < 0)
		return -1;

	tm.tm_mon = conv(start + 5, 2) - 1;
	if (tm.tm_mon < 0)
		return -1;

	tm.tm_mday = conv(start + 8, 2);
	if (tm.tm_mday < 0)
		return -1;

	tm.tm_hour = conv(start + 11, 2);
	if (tm.tm_mday < 0)
		return -1;

	task->start = mktime(&tm);

	return 0;
}

void pla_task_set_duration(struct task *task, unsigned int duration)
{
	task->duration = duration;
}

void pla_task_set_duration_h(struct task *task, unsigned int duration)
{
	task->duration = duration * 3600;
}

int pla_task_set_duration_sh(struct task *task, const char *duration)
{
	int d;
	d = conv(duration, strlen(duration));
	if (d < 0)
		return -1;
	pla_task_set_duration_h(task, d);
	return 0;
}

void pla_task_set_duration_d(struct task *task, unsigned int duration)
{
	task->duration = duration * 86400;
}

int pla_task_set_duration_sd(struct task *task, const char *duration)
{
	int d;
	d = conv(duration, strlen(duration));
	if (d < 0)
		return -1;
	pla_task_set_duration_d(task, d);
	return 0;
}

void pla_task_set_percent(struct task *task, unsigned int percent)
{
	if (percent > 100)
		percent = 100;
	task->percent = percent;
}

int pla_task_set_percent_s(struct task *task, const char *percent)
{
	int d;
	d = conv(percent, strlen(percent));
	if (d < 0)
		return -1;
	if (d > 100)
		d = 100;
	pla_task_set_percent(task, (unsigned int)d);
	return 0;
}

void pla_task_add_child(struct task *task, struct task* child)
{
	list_add_tail(&child->_child, &task->childs);
	child->parent = task;
}

void pla_task_add_res(struct task *task, struct res *res)
{
	task->res = realloc(task->res, ( task->nres + 1 ) * sizeof(struct res*));
	if (task->res == NULL) {
		fprintf(stderr, "realloc error");
		exit(1);
	}
	task->res[task->nres] = res;
	task->nres++;
}

void pla_task_add_dep(struct task *task, struct task* dep)
{
	task->deps = realloc(task->deps, ( task->ndep + 1 ) * sizeof(struct task*));
	if (task->deps == NULL) {
		fprintf(stderr, "realloc error");
		exit(1);
	}
	task->deps[task->ndep] = dep;
	task->ndep++;
}

void pla_task_del_child(struct task *child)
{
}

void pla_task_del_dep(struct task *dep)
{
}
int pla_task_get_order(struct list_head *base, struct task *t)
{
	struct task *tt;
	int order = 0;

	list_for_each_entry(tt, base, c) {
		if (tt == t)
			return order;
		order++;
	}
	return 0;
}
void pla_task_update_date(struct task *task)
{
	struct task *tt;
	time_t start = 2147483647;
	time_t stop = 0;

	if (task->childs.prev == task->childs.next)
		return;

	list_for_each_entry(tt, &task->childs, _child) {
		if (tt->start < start)
			start = tt->start;
		if (tt->start + tt->duration > stop)
			stop = tt->start + tt->duration;
	}
	task->start = start;
	task->duration = stop - start;
}
int pla_task_get_level(struct task *t)
{
	int lvl = 0;

	while (1) {

		/* if empty, is the end */
		if (t->parent == NULL)
			return lvl;

		/* one next level */
		lvl++;

		/* search parent */
		t = t->parent;
	}
}
void pla_task_set_color(struct task *task, const char *color)
{
	convert_rgba_hex(color, 0xff, &task->color);

}
void pla_task_set_bg(struct task *task, const char *color)
{
	convert_rgba_hex(color, 0xff, &task->bg);
	task->bg_isset = 1;
}
struct task *pla_task_get_by_id(struct list_head *base, const char *id)
{
	struct task *t;

	list_for_each_entry(t, base, c) {
		if (strcmp(t->id, id) == 0)
			return t;
	}

	return NULL;
}

struct task *pla_task_get_by_id_n(struct list_head *base, const char *id, int len)
{
	struct task *t;

	list_for_each_entry(t, base, c) {
		if (strlen(t->id) == len && memcmp(t->id, id, len) == 0)
			return t;
	}

	return NULL;
}

static
int pla_res_sort_func(const void *a, const void *b)
{
	const struct res *aa;
	const struct res *bb;

	aa = *(const struct res **)a;
	bb = *(const struct res **)b;

	return strcmp(aa->name, bb->name);;
}

void pla_res_sort(struct list_head *base)
{
	struct res *t;
	struct res **st;
	int n;
	int i;

	/* count entries */
	n=0;
	list_for_each_entry(t, base, c)
		n++;

	/* memoire pour le qsort */
	st = malloc(sizeof(struct res *) * n);
	i = 0;
	list_for_each_entry(t, base, c) {
		st[i] = t;
		i++;
	}

	/* tri */
	qsort(st, n, sizeof(struct res *), pla_res_sort_func);

	/* clean list */
	INIT_LIST_HEAD(base);

	/* fill list */
	for (i=0; i<n; i++)
		list_add_tail(&st[i]->c, base);
}

/* top is the next available slot in the visited stack
 * return 0 if no loop, otherwise return 1
 */
int pla_has_cycle_r(struct task **visited, int top, struct task *task, struct task **cycle_task) {
	int i;
	struct task *t;

	/* search task in visited stack */
	for (i = 0; i < top; i++) {
		if (visited[i] == task) {
			return 1;
		}
	}

	/* push node in the visited stack */
	visited[top] = task;

	/* browse childrens */
	list_for_each_entry(t, &task->childs, _child) {
		if (pla_has_cycle_r(visited, top + 1, t, cycle_task) == 1) {
			if (*cycle_task == NULL) {
				*cycle_task = task;
			}
			return 1;
		}
	}

	return 0;
}

struct task *pla_has_cycle(struct list_head *base) {
	struct task *t;
	int nodes;
	struct task **visited;
	int processed;
	struct task *cycle_task;

	/* count total of nodes and total of orphan nodes */
	nodes = 0;
	list_for_each_entry(t, base, c) {
		nodes++;
	}

	/* allocate and init memory */
	visited = calloc(sizeof(struct task *), nodes);
	if (visited == NULL) {
		fprintf(stderr, "out of memory errror.\n");
		exit(1);
	}

	/* check cycle for each orphan nodes */
	processed = 0;
	cycle_task = NULL;
	list_for_each_entry(t, base, c) {
		if (t->parent == NULL) {
			if (pla_has_cycle_r(visited, 0, t, &cycle_task) == 1) {
				return cycle_task;
			}
			processed++;
		}
	}

	/* if there are no nodes processed, it indicates a loop
	 * because there are no start node. Return the first task
	 * of the list.
	 */
	if (processed == 0) {
		list_for_each_entry(t, base, c) {
			return t;
		}
	}

	return NULL;
}
