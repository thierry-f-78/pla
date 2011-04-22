#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"
#include "pla.h"

#define DATE_SIZE 64

void render_text(const char *file_out, struct disp *d)
{
	FILE *out;
	int nb;
	int nb2;
	struct task *t;
	struct res *r;
	char datestart[DATE_SIZE];
	char datestop[DATE_SIZE];
	float duree;
	struct tm tm;
	time_t stop;
	int i;

	/* compte le nombre d'éléments a afficher */
	nb = 0;
	list_for_each_entry(t, d->base, c)
		nb++;

	/* compte le nombre de resources */
	nb2 = 0;
	list_for_each_entry(r, d->res, c)
		nb2++;

	/* open output file */
	if (strcmp(file_out, "-") == 0)
		out = stdout;

	else {
		out = fopen(file_out, "w");
		if (out == NULL) {
			fprintf(stderr, "can't open output file\n");
			exit(1);
		}
	}

	/* headers */
	fprintf(out, "Nom%sDémarré%sTerminé%sDurée%sAssigné à\n", "\t", "\t", "\t", "\t");

	/* compte le nombre d'éléments a afficher */
	list_for_each_entry(t, d->base, c) {

		/* on calcule la date start */
		localtime_r(&t->start, &tm);
		strftime(datestart, DATE_SIZE, "%d %B", &tm);

		/* on calcule la date stop */
		stop = t->start + t->duration;
		localtime_r(&stop, &tm);
		strftime(datestop, DATE_SIZE, "%d %B", &tm);

		/* calcule la durée en jours */
		duree = (float)t->duration / 86400.0;

		/* si on est une tache, on affiche la ligne qui va bien */
		if (t->childs.next == &t->childs) {
			fprintf(out, "%s%s%s%s%s%s%.1f%s",
			        t->name, "\t", datestart, "\t", datestop, "\t", duree, "\t");
			for (i=0; i<t->nres; i++) {
				if (i+1 < t->nres)
					fprintf(out, "%s, ", t->res[i]->name);
				else
					fprintf(out, "%s", t->res[i]->name);
			}
			fprintf(out, "\n");
		}

		/* on affiche un titre */
		else {
			fprintf(out, "**%s**%s%s%s%s%s%.1f\n",
			        t->name, "\t", datestart, "\t", datestop, "\t", duree);
		}

	}

	/* end */
	fflush(out);
	fclose(out);
}
