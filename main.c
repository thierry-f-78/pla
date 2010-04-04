#include <string.h>
#include <stdio.h>

#include "pla.h"
#include "load.h"
#include "render.h"

int main(int argc, char *argv[])
{
	const char *out;
	char *p;
	int mode;
	struct disp d;
	struct list_head base = LIST_HEAD_INIT(base);
	struct task *t;
	time_t max;

	pla_load(&base, "out.pla");

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

//	d.start = 1272664800;
//	d.duration = 2678400;
//	d.start = 1275602400;
//	d.duration = 2678400;

	if (argc < 2) {
		out = "pla.png";
		mode = 1;
	}
	
	else {
		out = argv[1];
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
			fprintf(stderr, "Unknown extension file\n");
			exit(1);
		}
	}

	pla_draw(mode, out, &d);
//	pla_store(&base, "out.pla");
	return 0;
}
