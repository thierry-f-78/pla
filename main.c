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
		"    [-s yyyymmdd] [-e yyyymmdd] [-id task_id]\n"
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

int main(int argc, char *argv[])
{
	const char *out = NULL;
	const char *in = NULL;
	char *p;
	int mode = 0;
	struct disp d;
	struct list_head base = LIST_HEAD_INIT(base);
	struct task *t;
	time_t max;
	int i;
	time_t start = -1;
	time_t end = -1;
	int id = -1;

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
			id = conv(argv[i], strlen(argv[i]));
			if (id == -1) {
				fprintf(stderr, "\nargument -i: invalid id\n");
				usage();
				exit(1);
			}
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
	pla_load(&base, in);

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

	/* if start is set */
	if (start != -1)
		d.start = start;
	
	/* if end is set */
	if (end != -1)
		d.duration = end - d.start;

	/* if id known */
	if (id != -1) {
		t = pla_task_get_by_id(&base, id);
		if (t == NULL) {
			fprintf(stderr, "\nunknown id\n");
			usage();
			exit(1);
		}
		d.start = t->start;
		d.duration = t->duration;
	}

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
