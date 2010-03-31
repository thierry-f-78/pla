#ifndef __PLA_H__
#define __PLA_H__

#include <time.h>

#include "list.h"

struct disp {
	time_t start;
	unsigned int duration;
	int w;
	int h;
	struct list_head *base;
};

struct color {
	double r;
	double g;
	double b;
	double a;
};

struct task {

	/* chain */
	struct list_head c;

	/* task */
	int id;
	char *name;
	struct color color;

	/* durée */
	time_t start;
	unsigned int duration;

	/* dependances */
	int ndep;
	struct task **deps;

	/* fils */
	struct task *parent;
	struct list_head _child;
	struct list_head childs;
};

struct task *pla_task_new(struct list_head *base, const char *name, const char *color,
                          time_t start, unsigned int duration);
void pla_task_set_start(struct task *task, time_t start);
int pla_task_set_start_ymd(struct task *task, const char *start);
int pla_task_set_start_ymdh(struct task *task, const char *start);
void pla_task_set_duration(struct task *task, unsigned int duration);
void pla_task_set_duration_h(struct task *task, unsigned int duration);
int pla_task_set_duration_sh(struct task *task, const char *duration);
void pla_task_set_duration_d(struct task *task, unsigned int duration);
int pla_task_set_duration_sd(struct task *task, const char *duration);
void pla_task_add_child(struct task *task, struct task *child);
void pla_task_add_dep(struct task *task, struct task *dep);
void pla_task_del_child(struct task *child);
void pla_task_del_dep(struct task *dep);
int pla_task_get_level(struct task *task);
int pla_task_get_order(struct list_head *base, struct task *t);
void pla_task_update_date(struct task *task);

#endif /* __PLA_H__ */
