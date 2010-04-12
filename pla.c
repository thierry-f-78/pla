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

int pla_task_set_start_ymdh(struct task *task, const char *start)
{
	struct tm tm;

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
struct task *pla_task_get_by_id(struct list_head *base, int id)
{
	struct task *t;

	list_for_each_entry(t, base, c) {
		if (t->id == id)
			return t;
	}

	return NULL;
}
