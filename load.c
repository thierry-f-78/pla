#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "list.h"
#include "pla.h"

#define BUFSIZE 1024

void pla_load(struct list_head *base, struct list_head *res, const char *file)
{
	FILE *fp;
	char buf[BUFSIZE];
	char *p;
	char *attr;
	char *value;
	struct task *t = NULL;
	struct task *tt = NULL;
	struct res *r;
	char *error;
	int line = 0;
	int id;

	/* open file */
	fp = fopen(file, "r");
	if (fp == NULL) {
		fprintf(stderr, "cant open file \"%s\"\n", file);
		exit(1);
	}

	/* read file */
	while (1) {

		/**************************** LECTURE *************************/

		/* read line */
		memset(buf, 0, BUFSIZE);
		fgets(buf, BUFSIZE, fp);
		if (feof(fp))
			break;

		/* next line */
		line ++;

		/* remove white spaces */
		p = buf;
		while (*p == ' ' || *p == '\t' || *p == '\r' || *p == '\n') {
			if (*p == '\0')
				break;
			p++;
		}

		/* empty line */
		if (*p == 0)
			continue;

		/* extrait le premier parametre */
		attr = p;
		while (*p != ' ' && *p != '\t' && *p != '\r' && *p != '\n') {
			if (*p == '\0') {
				fprintf(stderr, "bad file format at line %d\n", line);
				exit(1);
			}
			p++;
		}

		/* saute les espaces */
		while (*p == ' ' || *p == '\t' || *p == '\r' || *p == '\n') {
			if (*p == '\0') {
				fprintf(stderr, "bad file format at line %d\n", line);
				exit(1);
			}
			*p = '\0';
			p++;
		}

		/* voila la valeur */
		value = p;

		/* degage les saut de lignes */
		while (*p != '\0') {
			if (*p == '\r' || *p == '\n')
				*p = '\0';
			p++;
		}

		/**************************** TRAITEMENT *************************/

		/* nouvelle tache */
		if (attr[0] == '[') {

			/* convert task id */
			id = strtol(attr+1, &error, 10);
			if (*error != ']') {
				fprintf(stderr, "bad file format at line %d: number expected\n", line);
				exit(1);
			}

			/* verif des doublons */
			if (pla_task_get_by_id(base, id) != NULL) {
				fprintf(stderr, "task %d, line %d: this id already exists\n", id, line);
				exit(1);
			}

			/* add task */
			t = pla_task_new(base, value, NULL, 0, 0);
			t->id = id;
		}
	}

	fclose(fp);

	/* open file */
	fp = fopen(file, "r");
	if (fp == NULL) {
		fprintf(stderr, "cant open file \"%s\"\n", file);
		exit(1);
	}

	line = 0;

	/* read file */
	while (1) {

		/**************************** LECTURE *************************/

		/* read line */
		memset(buf, 0, BUFSIZE);
		fgets(buf, BUFSIZE, fp);
		if (feof(fp))
			break;

		/* next line */
		line ++;

		/* remove white spaces */
		p = buf;
		while (*p == ' ' || *p == '\t' || *p == '\r' || *p == '\n') {
			if (*p == '\0')
				break;
			p++;
		}

		/* empty line */
		if (*p == 0)
			continue;

		/* extrait le premier parametre */
		attr = p;
		while (*p != ' ' && *p != '\t' && *p != '\r' && *p != '\n') {
			if (*p == '\0') {
				fprintf(stderr, "bad file format at line %d\n", line);
				exit(1);
			}
			p++;
		}

		/* saute les espaces */
		while (*p == ' ' || *p == '\t' || *p == '\r' || *p == '\n') {
			if (*p == '\0') {
				fprintf(stderr, "bad file format at line %d\n", line);
				exit(1);
			}
			*p = '\0';
			p++;
		}

		/* voila la valeur */
		value = p;

		/* degage les saut de lignes */
		while (*p != '\0') {
			if (*p == '\r' || *p == '\n')
				*p = '\0';
			p++;
		}

		
		/**************************** TRAITEMENT *************************/

		/* nouvelle tache */
		if (attr[0] == '[') {
			id = strtol(attr+1, &error, 10);
			t = pla_task_get_by_id(base, id);
			if (t == NULL) {
				fprintf(stderr, "strange error: id does not exists at line %d\n", line);
				exit(1);
			}
		}

		/* date de debut */
		else if (strcmp(attr, "start") == 0) {
			if (t == NULL) {
				fprintf(stderr, "bad file format at line %d: task expected\n", line);
				exit(1);
			}

			if (strlen(value) > 10 && value[10] == ' ') {
				if (pla_task_set_start_ymdhh(t, value) < 0) {
					fprintf(stderr, "bad date format at line %d\n", line);
					exit(1);
				}
			}

			else {
				if (pla_task_set_start_ymdd(t, value) < 0) {
					fprintf(stderr, "bad date format at line %d\n", line);
					exit(1);
				}
			}
		}

		/* durée */
		else if (strcmp(attr, "duration") == 0) {
			if (t == NULL) {
				fprintf(stderr, "bad file format at line %d: task expected\n", line);
				exit(1);
			}	
			if (pla_task_set_duration_sh(t, value) < 0) {
				fprintf(stderr, "bad duration format at line %d\n", line);
				exit(1);
			}
		}

		/* couleur */
		else if (strcmp(attr, "color") == 0) {
			if (t == NULL) {
				fprintf(stderr, "bad file format at line %d: task expected\n", line);
				exit(1);
			}
			pla_task_set_color(t, value);
		}

		/* percent */
		else if (strcmp(attr, "percent") == 0) {
			if (t == NULL) {
				fprintf(stderr, "bad file format at line %d: task expected\n", line);
				exit(1);
			}
			if (pla_task_set_percent_s(t, value) < 0) {
				fprintf(stderr, "bad file format at line %d: bad percent\n", line);
				exit(1);
			}
		}

		/* fils */
		else if (strcmp(attr, "child") == 0) {
			if (t == NULL) {
				fprintf(stderr, "bad file format at line %d: task expected\n", line);
				exit(1);
			}
			id = strtol(value, &error, 10);
			tt = pla_task_get_by_id(base, id);
			if (tt == NULL) {
				fprintf(stderr, "error: child id does not exists at line %d\n", line);
				exit(1);
			}
			pla_task_add_child(t, tt);
		}

		/* dependances */
		else if (strcmp(attr, "dep") == 0) {
			if (t == NULL) {
				fprintf(stderr, "bad file format at line %d: task expected\n", line);
				exit(1);
			}
			id = strtol(value, &error, 10);
			tt = pla_task_get_by_id(base, id);
			if (tt == NULL) {
				fprintf(stderr, "error: dep id does not exists at line %d\n", line);
				exit(1);
			}
			pla_task_add_dep(t, tt);
		}

		/* resource */
		else if (strcmp(attr, "res") == 0) {
			if (t == NULL) {
				fprintf(stderr, "bad file format at line %d: task expected\n", line);
				exit(1);
			}

			/* get resource */
			r = pla_res_get_by_name(res, value);

			/* not found: new resource */
			if (r == NULL)
				r = pla_res_new(res, value);

			/* add resource */
			pla_task_add_res(t, r);
		}

		/* unknown keyword */
		else {
			fprintf(stderr, "bad file format at line %d: unknown keyword\n", line);
			exit(1);
		}
	}

	fclose(fp);

	/* sort resources */
	pla_res_sort(res);
}

void pla_store(struct list_head *base, const char *file)
{
	FILE *fp;
	struct task *t;
	struct task *tt;
	struct tm tm;
	int i;

	/* open file */
	fp = fopen(file, "w");
	if (fp == NULL) {
		fprintf(stderr, "cant open file \"%s\"\n", file);
		exit(1);
	}

	/* write file */
	list_for_each_entry(t, base, c) {
		localtime_r(&t->start, &tm);

		/* new task */
		fprintf(fp, "[%d] %s\n", t->id, t->name);

		/* save start time */
		fprintf(fp, "\tstart %04d-%02d-%02d %02d\n",
		        tm.tm_year + 1900, tm.tm_mon+1, tm.tm_mday, tm.tm_hour);

		/* save duration */
		fprintf(fp, "\tduration %d\n", t->duration / 3600);

		/* save percent */
		fprintf(fp, "\tpercent %u\n", t->percent);

		/* save color */
		fprintf(fp, "\tcolor #%02x%02x%02x\n", 
		        (unsigned int)(t->color.r * 255),
		        (unsigned int)(t->color.g * 255),
		        (unsigned int)(t->color.b * 255));

		/* save deps */
		for (i=0; i<t->ndep; i++)
			fprintf(fp, "\tdep %d\n", t->deps[i]->id);

		/* save childrens */
		list_for_each_entry(tt, &t->childs, _child)
			fprintf(fp, "\tchild %d\n", tt->id);

		/* end */
		fprintf(fp, "\n");
	}
}
