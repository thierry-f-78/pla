#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "pla.h"
#include "load.h"
#include "render.h"

void usage(void) {
	fprintf(stderr,
		"\n"
		"pla -i <filename> -o <filename> [-f (eps|png|svg|pdf)]\n"
		"    [-s yyyymmdd] [-e yyyymmdd] [-id task_id] [-oid task_id]\n"
		"\n"
	);
}

time_t convert_yyymmdd(const char *date)
{
	struct tm tm;

	memset(&tm, 0, sizeof(struct tm));

	tm.tm_year = conv(date, 4) - 1900;
	if (tm.tm_year < 0)
		return -1;

	tm.tm_mon = conv(date + 4, 2) - 1;
	if (tm.tm_mon < 0)
		return -1;

	tm.tm_mday = conv(date + 6, 2);
	if (tm.tm_mday < 0)
		return -1;

	return mktime(&tm);
}

static inline
void oid_add(int **oid, int *noid, int id)
{
	int i;

	/* check if exists */
	for (i=0; i<(*noid); i++)
		if ((*oid)[i] == id)
			return;

	/* add */
	(*oid) = realloc((*oid), ((*noid)+1)*sizeof(int));
	(*oid)[(*noid)] = id;
	(*noid)++;
}

int main(int argc, char *argv[])
{
	const char *out = NULL;
	const char *in = NULL;
	char *p;
	int mode = 0;
	struct disp d;
	struct list_head base = LIST_HEAD_INIT(base);
	struct list_head res = LIST_HEAD_INIT(res);
	struct task *t;
	struct task *tt;
	struct res *r;
	struct res *rr;
	time_t max;
	int i;
	time_t start = -1;
	time_t end = -1;
	int nid = 0;
	int *id = NULL;
	int noid = 0;
	int *oid = NULL;
	int tmp;
	int ok;

	d.display_res = 0;

	/* argument parser */
	for (i=1; i<argc; i++) {

		/* input file */
		/**/ if (strcmp(argv[i], "-i") == 0) {
			i++;
			if (i == argc) {
				fprintf(stderr, "\nargument -i expect filename\n");
				usage();
				exit(1);
			}
			in = argv[i];
		}

		/* output file */
		else if (strcmp(argv[i], "-o") == 0) {
			i++;
			if (i == argc) {
				fprintf(stderr, "\nargument -o expect filename or -\n");
				usage();
				exit(1);
			}
			out = argv[i];
		}

		/* start date for render */
		else if (strcmp(argv[i], "-s") == 0) {
			i++;
			if (i == argc) {
				fprintf(stderr, "\nargument -s expect start date in format yyymmdd\n");
				usage();
				exit(1);
			}
			start = convert_yyymmdd(argv[i]);
			if (start == -1) {
				fprintf(stderr, "\nargument -s: invalid date\n");
				usage();
				exit(1);
			}
		}

		/* end date for render */
		else if (strcmp(argv[i], "-e") == 0) {
			i++;
			if (i == argc) {
				fprintf(stderr, "\nargument -e expect end date in format yyymmdd\n");
				usage();
				exit(1);
			}
			end = convert_yyymmdd(argv[i]);
			if (end == -1) {
				fprintf(stderr, "\nargument -e: invalid date\n");
				usage();
				exit(1);
			}
			end += 86400;
		}

		/* format */
		else if (strcmp(argv[i], "-f") == 0) {
			i++;
			if (i == argc) {
				fprintf(stderr, "\nargument -f expect format (eps, png, pdf or svg)\n");
				usage();
				exit(1);
			}
			/**/ if (strcasecmp(argv[i], "png") == 0)
				mode = 1;
			else if (strcasecmp(argv[i], "eps") == 0)
				mode = 2;
			else if (strcasecmp(argv[i], "svg") == 0)
				mode = 3;
			else if (strcasecmp(argv[i], "pdf") == 0)
				mode = 4;
		}

		/* task id */
		else if (strcmp(argv[i], "-id") == 0) {
			i++;
			if (i == argc) {
				fprintf(stderr, "\nargument -i expect id\n");
				usage();
				exit(1);
			}
			tmp = conv(argv[i], strlen(argv[i]));
			if (tmp == -1) {
				fprintf(stderr, "\nargument -i: invalid id\n");
				usage();
				exit(1);
			}

			/* add id */
			id = realloc(id, (nid+1)*sizeof(int));
			id[nid] = tmp;
			nid++;
		}

		/* task id */
		else if (strcmp(argv[i], "-oid") == 0) {
			i++;
			if (i == argc) {
				fprintf(stderr, "\nargument -oid expect id\n");
				usage();
				exit(1);
			}
			tmp = conv(argv[i], strlen(argv[i]));
			if (tmp == -1) {
				fprintf(stderr, "\nargument -oid: invalid id\n");
				usage();
				exit(1);
			}

			/* add oid */
			oid = realloc(oid, (noid+1)*sizeof(int));
			oid[noid] = tmp;
			noid++;
		}

		/* display resource */
		else if (strcmp(argv[i], "-res") == 0) {
			d.display_res = 1;
		}

		/* help */
		else if (strcmp(argv[i], "-h") == 0) {
			usage();
			exit(0);
		}

		/* help */
		else {
			fprintf(stderr, "\nunknown argument\n");
			usage();
			exit(0);
		}

	}

	/* checks */
	if (in == NULL) {
		fprintf(stderr, "\nsource file expected\n");
		usage();
		exit(1);
	}

	if (out == NULL) {
		fprintf(stderr, "output file expected\n");
		usage();
		exit(1);
	}

	/* loda planning */
	pla_load(&base, &res, in);

	/* oid */
	if (noid > 0) {

		/* check childs */
		list_for_each_entry(t, &base, c)
			for (i=0; i<noid; i++)
				if (t->id == oid[i])
					list_for_each_entry(tt, &t->childs, _child)
						oid_add(&oid, &noid, tt->id);

		/* delete task */
		list_for_each_entry_safe(t, tt, &base, c) {
			ok = 0;
			for (i=0; i<noid; i++)
				if (t->id == oid[i])
					ok = 1;
			if (ok == 1)
				continue;
			list_del(&t->c);
		}

		/* remove res */
		list_for_each_entry_safe(r, rr, &res, c) {
			ok = 0;
			list_for_each_entry(t, &base, c)
				for (i=0; i<t->nres; i++)
					if (t->res[i] == r)
						ok = 1;
			if (ok == 0)
				list_del(&r->c);
		}
	}

	/* recherche la date la plus petite */
	d.start = -1;
	list_for_each_entry(t, &base, c) {

		if (t->start == 0)
			continue;

		if (d.start == -1) {
			d.start = t->start;
			continue;
		}

		if (t->start < d.start) {
			d.start = t->start;
			continue;
		}
	}

	/* recherche la date la plus grande */
	max = 0;
	list_for_each_entry(t, &base, c) {

		if (t->start == 0)
			continue;

		if (t->start + t->duration > max)
			max = t->start + t->duration;
	}

	d.duration = max - d.start;
	d.base = &base;
	d.res = &res;

	/* if id s known */
	if (nid > 0) {
		start = -1;
		end = -1;

		for (i=0; i<nid; i++) {

			t = pla_task_get_by_id(&base, id[i]);
			if (t == NULL) {
				fprintf(stderr, "\nunknown id\n");
				usage();
				exit(1);
			}
			pla_task_update_date(t);

			if (start == -1)
				start = t->start;

			else if (start > t->start)
				start = t->start;

			if (end == -1)
				end = t->start + t->duration;

			else if (end < t->start + t->duration)
				end = t->start + t->duration;
		}
	}

	/* if start is set */
	if (start != -1)
		d.start = start;
	
	/* if end is set */
	if (end != -1)
		d.duration = end - d.start;

	/* auto select mode if needed */
	if (mode == 0) {
		p = strchr(out, '.');
		if (p == NULL)
			mode = 1; 
		else if (strcasecmp(p, ".png") == 0)
			mode = 1;
		else if (strcasecmp(p, ".eps") == 0)
			mode = 2;
		else if (strcasecmp(p, ".svg") == 0)
			mode = 3;
		else if (strcasecmp(p, ".pdf") == 0)
			mode = 4;
		else {
			fprintf(stderr, "Unknown extension file, output format expected. see -f\n");
			usage();
			exit(1);
		}
	}

	pla_draw(mode, out, &d);
//	pla_store(&base, "out.pla");
	return 0;
}
