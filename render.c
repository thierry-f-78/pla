#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <cairo.h>
#include <cairo-ps.h>
#include <cairo-pdf.h>
#include <cairo-svg.h>

#include "utils.h"
#include "pla.h"

#define DAY_W 16.0f
#define DAY_H 13.0f
#define HDR_W 150.0f
#define HDR_DH 17.0f
#define HDR_MH 15.0f
#define FONT_SIZE 10.0f
#define ARROW 3.0f
#define TASK_BORD 3.0f
#define NAME_TAB 20.0f
#define PERCENT_DECAL 0.3f
#define SPACE 10.0f

/* #EDEDED */
struct color lgray1 = {
	0.92941176470588235294f,
	0.92941176470588235294f,
	0.92941176470588235294f,
	1
};
struct color lgray2 = {
	0.80784313725490196078f,
	0.80784313725490196078f,
	0.80784313725490196078f,
	1
};
struct color lgray3 = {
	0.48235294117647058823f,
	0.48235294117647058823f,
	0.48235294117647058823f,
	1
};

/* 8CB6CE */
struct color defcol = {
	0.54901960784313725490f,
	0.71372549019607843137f,
	0.80784313725490196078f,
	1
};
struct color black_50p = {
	0.0f,
	0.0f,
	0.0f,
	0.5f
};
struct color black = {
	0.0f,
	0.0f,
	0.0f,
	1.0f
};
struct color white_80p = {
	1.0f,
	1.0f,
	1.0f,
	0.8f
};

struct color white = {
	1.0f,
	1.0f,
	1.0f,
	1.0f
};

static
void pla_cairo_day_ferie(cairo_t *c, int ps, struct disp *d)
{
	/* colonne grise top */
	cairo_new_path(c);
	cairo_move_to(c, ps, HDR_MH + HDR_DH);
	cairo_line_to(c, ps, d->h1);
	cairo_line_to(c, ps + DAY_W, d->h1);
	cairo_line_to(c, ps + DAY_W, HDR_MH + HDR_DH);
	cairo_set_source_col(c, &lgray1);
	cairo_fill(c);

	/* colonne grise bottom */
	cairo_new_path(c);
	cairo_move_to(c, ps, d->rs);
	cairo_line_to(c, ps, d->rs + d->h2);
	cairo_line_to(c, ps + DAY_W, d->rs + d->h2);
	cairo_line_to(c, ps + DAY_W, d->rs);
	cairo_set_source_col(c, &lgray1);
	cairo_fill(c);
}

static
void pla_cairo_day(cairo_t *c, int ps, int day, struct disp *d)
{
	cairo_text_extents_t exts;
	char buf[3];
	double x;
	double y;

	/* black border */
	cairo_new_path(c);
	cairo_move_to(c, ps + DAY_W, HDR_MH);
	cairo_line_to(c, ps + DAY_W, HDR_MH + HDR_DH);
	cairo_set_source_col(c, &black);
	cairo_stroke(c);

	/* build str date */
	snprintf(buf, 3, "%d", day);
	buf[2] = 0;

	/* select font and get */
	cairo_select_font_face (c, "Sans", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
	cairo_set_font_size (c, FONT_SIZE);

	/*
	 *
	 * ^        |           +-----+
	 * |        |           |     |
	 * |        | bearing   |     |
	 * |        |           |     |
	 * | height |           |     |
	 * |        v     (x,y) +-----+
	 * |                    |
	 * |                    |
	 * |                    |
	 *
	 */

	/* compute center of text */
	cairo_text_extents (c, buf, &exts);
	x = ( ps + ( DAY_W / 2 ) )       -  ( ( exts.width  / 2 ) + exts.x_bearing );
	y = ( HDR_MH + ( ( HDR_DH - ( HDR_MH * .15 ) ) / 2 ) )  -  ( ( exts.height / 2 ) + exts.y_bearing );

	/* clipping zone */
	cairo_rectangle(c, ps, HDR_MH, ps + DAY_W, HDR_MH + HDR_DH);
	cairo_clip(c);

	/* display text */
	cairo_new_path(c);
	cairo_move_to(c, x, y);
	cairo_show_text(c, buf);
	cairo_set_source_col(c, &black);
	cairo_fill(c);
	cairo_stroke(c);

	/* end of clip */
	cairo_reset_clip(c);

	/* vrule */
	cairo_new_path(c);
	cairo_move_to(c, ps + DAY_W, HDR_MH + HDR_DH);
	cairo_line_to(c, ps + DAY_W, d->h1);
	cairo_set_source_col(c, &lgray2);
	cairo_stroke(c);

	/* vrule */
	cairo_new_path(c);
	cairo_move_to(c, ps + DAY_W, d->rs);
	cairo_line_to(c, ps + DAY_W, d->rs + d->h2);
	cairo_set_source_col(c, &lgray2);
	cairo_stroke(c);

}

static char *str_mois[] = {
	"Janvier",
	"FÃ©vrier",
	"Mars",
	"Avril",
	"Mai",
	"Juin",
	"Juillet",
	"Aout",
	"Septembre",
	"Octore",
	"Novembre",
	"DÃ©cembre"
};

static
const char *get_mois(int mois) {
	if (mois >= 0 && mois <= 11)
		return str_mois[mois];
	else
		return "Unknown month";
}

static
void pla_cairo_month(cairo_t *c, int start, int stop, int mois, struct disp *d)
{
	cairo_text_extents_t exts;
	double x;
	double y;

	/* bordure noire */
	cairo_new_path(c);
	cairo_move_to(c, stop, 0);
	cairo_line_to(c, stop, HDR_MH);
	cairo_set_source_col(c, &black);
	cairo_stroke(c);

	/* select font and get */
	cairo_select_font_face (c, "Sans", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
	cairo_set_font_size (c, FONT_SIZE);

	/*
	 *
	 * ^        |           +-----+
	 * |        |           |     |
	 * |        | bearing   |     |
	 * |        |           |     |
	 * | height |           |     |
	 * |        v     (x,y) +-----+
	 * |                    |
	 * |                    |
	 * |                    |
	 *
	 */

	/* compute center of text */
	cairo_text_extents (c, get_mois(mois), &exts);
	x = ( start + ( (stop-start) / 2 ) )       -  ( ( exts.width  / 2 ) + exts.x_bearing );
	y = ( HDR_MH / 2 ) - ( ( exts.height / 2 ) + exts.y_bearing );

	/* clipping zone */
	cairo_rectangle(c, start, 0, stop, HDR_MH);
	cairo_clip(c);

	/* display text */
	cairo_new_path(c);
	cairo_move_to(c, x, y);
	cairo_show_text(c, get_mois(mois));
	cairo_set_source_col(c, &black);
	cairo_stroke(c);

	/* end of clip */
	cairo_reset_clip(c);
}

static
void pla_cairo_head(cairo_t *c, int ps, struct task *t, struct disp *d)
{
	cairo_text_extents_t exts;
	double x;
	double y;

	/* draw black separation */
	cairo_new_path(c);
	cairo_move_to(c, 0, ps + DAY_H);
	cairo_line_to(c, HDR_W, ps + DAY_H);
	cairo_set_source_col(c, &black);
	cairo_stroke(c);

	/* hrule */
	cairo_new_path(c);
	cairo_move_to(c, HDR_W, ps + DAY_H);
	cairo_line_to(c, d->w, ps + DAY_H);
	cairo_set_source_col(c, &lgray2);
	cairo_stroke(c);

	/* select font and get */
	cairo_select_font_face (c, "Sans", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
	cairo_set_font_size (c, FONT_SIZE);

	/*
	 *
	 * ^        |           +-----+
	 * |        |           |     |
	 * |        | bearing   |     |
	 * |        |           |     |
	 * | height |           |     |
	 * |        v     (x,y) +-----+
	 * |                    |
	 * |                    |
	 * |                    |
	 *
	 */

	/* compute center of text */
	cairo_text_extents (c, t->name, &exts);
	x = 2 + ( pla_task_get_level(t) * NAME_TAB );
	y = ( ps + ( DAY_H / 2 ) )  -  ( ( exts.height / 2 ) + exts.y_bearing );

	/* clipping zone */
	cairo_rectangle(c, 0, ps, HDR_W, ps + DAY_H);
	cairo_clip(c);

	/* display text */
	cairo_new_path(c);
	cairo_move_to(c, x, y);
	cairo_set_source_col(c, &black);
	cairo_show_text(c, t->name);
	cairo_stroke(c);

	/* end of clip */
	cairo_reset_clip(c);

}

static
void pla_cairo_task(cairo_t *c, int ps, struct task *t, struct disp *d)
{
	double x1;
	double x2;
	double y1;
	double y2;
	double px1;
	double px2;
	double py1;
	double py2;
	double m;
	double x;
	double y;
	char buf[10];
	double center_offset;
	cairo_text_extents_t exts;

	/* update dates */
	pla_task_update_date(t);

	/* affiche - t - on ? */
	if ( !( (t->start >= d->start && t->start <= d->start + d->duration) ||
	        (t->start + t->duration >= d->start && t->start + t->duration <= d->start + d->duration) ) )
		return;

	/* si on est une tache, on affcihe le carré bleu */
	if (t->childs.next == &t->childs) {

		/* calcule le départ */
		x1 = HDR_W + ( (t->start - d->start) / 86400 * DAY_W );
		x2 = HDR_W + ( (( t->start + t->duration ) - d->start) / 86400 * DAY_W );
		y1 = ps + TASK_BORD;
		y2 = ps + DAY_H - TASK_BORD;

		/* draw carré */
		cairo_new_path(c);
		cairo_move_to(c, x1, y1);
		cairo_line_to(c, x2, y1);
		cairo_line_to(c, x2, y2);
		cairo_line_to(c, x1, y2);
		cairo_line_to(c, x1, y1);
		cairo_set_source_col(c, &t->color);
		cairo_fill(c);

		if (t->percent > 0) {

			/* percent */
			px1 = x1;
			px2 = x1 + ( ( x2 - x1 ) * t->percent / 100.0f );
			m = y2 - y1;
			py1 = y1 + ( m * PERCENT_DECAL );
			py2 = y2 - ( m * PERCENT_DECAL );

			/* draw percent */
			cairo_new_path(c);
			cairo_move_to(c, px1, py1);
			cairo_line_to(c, px2, py1);
			cairo_line_to(c, px2, py2);
			cairo_line_to(c, px1, py2);
			cairo_line_to(c, px1, py1);
			cairo_set_source_col(c, &black);
			cairo_fill(c);

		}

		/* draw black border */
		cairo_new_path(c);
		cairo_move_to(c, x1, y1);
		cairo_line_to(c, x2, y1);
		cairo_line_to(c, x2, y2);
		cairo_line_to(c, x1, y2);
		cairo_line_to(c, x1, y1);
		cairo_set_source_col(c, &black);
		cairo_stroke(c);

		center_offset = 0;
	}



	/* si on est une surtache, on affiche la ligne englobante */
	else {

		/*  */
		pla_task_update_date(t);
	
		/* calcule le départ */
		x1 = HDR_W + ( ( t->start - d->start ) / 86400.0f * DAY_W );
		x2 = HDR_W + ( ( ( t->start + t->duration ) - d->start ) / 86400.0f * DAY_W );
		y1 = ps + 4.0f;
		y2 = ps + DAY_H - 4.0f;
		m = ( y2 - y1 ) / 2.0f;

		center_offset = (y1 + ( m / 2.0f ) ) - (ps + (DAY_H / 2.0f) );

		/* draw form */
		cairo_new_path(c);
		cairo_move_to(c, x1, y1);
		cairo_line_to(c, x2, y1);
		cairo_line_to(c, x2, y2);
		cairo_line_to(c, x2 - m, y2 - m);
		cairo_line_to(c, x1 + m, y2 - m);
		cairo_line_to(c, x1, y2);
		cairo_line_to(c, x1, y1);
		cairo_set_source_col(c, &black);
		cairo_fill_preserve(c);
	}

	if (d->display_id == 1) {

		/* y for displaying id */
		y1 = ps + TASK_BORD + center_offset;
		y2 = ps + DAY_H - TASK_BORD + center_offset;

		/* select font and get */
		cairo_select_font_face (c, "Sans", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
		cairo_set_font_size (c, ( y2 - y1 ) );

		/* compute center of text */
		snprintf(buf, 10, "%d", t->id);
		cairo_text_extents (c, buf, &exts);
		x = x1 + 3.0f;
		y = ( y1 + ((y2-y1)/2) )  -  ( ( exts.height / 2 ) + exts.y_bearing );

		/* display text */
		cairo_new_path(c);
		cairo_move_to(c, x, y);
		cairo_set_source_col(c, &white);
		cairo_text_path(c, buf);
		cairo_fill_preserve(c);
		cairo_set_source_col(c, &black);
		cairo_stroke(c);
	}

}

static
void pla_cairo_arrow(cairo_t *c, int ps, struct task *t, struct disp *d)
{
	double x1;
	double x2;
	double y1;
	double y2;
	//double m;
	struct task *tt;
	int i;

	/* update dates */
	pla_task_update_date(t);

	for (i=0; i<t->ndep; i++) {
		tt = t->deps[i];

		/* update dates */
		pla_task_update_date(tt);

		/* affiche - t - on ? */
		if ( !( (t->start >= d->start && t->start <= d->start + d->duration) ||
		        (tt->start >= d->start && tt->start <= d->start + d->duration) ) )
			return;

		/* calcule le départ */
		x1 = HDR_W + ( ( t->start - d->start ) / 86400 * DAY_W ) + ( (t->duration) / 86400 * DAY_W );
		y1 = ps + TASK_BORD + ( ( ( ps + DAY_H - TASK_BORD ) - ( ps + TASK_BORD ) ) / 2 );

		/* calcule l'arrivé */
		x2 = HDR_W + ( ( tt->start - d->start ) / 86400 * DAY_W ) + ARROW;
		y2 = HDR_DH + HDR_MH + ( pla_task_get_order(d->base, tt) * DAY_H ) + TASK_BORD;

		/* draw carré */
		cairo_new_path(c);
		cairo_move_to(c, x1, y1);
		cairo_line_to(c, x2, y1);
		cairo_line_to(c, x2, y2);
		cairo_set_source_col(c, &black);
		cairo_stroke(c);

		/* draw fleche */
		cairo_new_path(c);
		cairo_move_to(c, x2, y2);
		cairo_line_to(c, x2 - ARROW, y2 - ARROW);
		cairo_line_to(c, x2 + ARROW, y2 - ARROW);
		cairo_line_to(c, x2, y2);
		cairo_set_source_col(c, &black);
		cairo_fill_preserve(c);
		cairo_stroke(c);

	}
}

static
void pla_cairo_res_head(cairo_t *c, int ps, struct res *r, struct disp *d)
{
	cairo_text_extents_t exts;
	double x;
	double y;

	/* draw black separation */
	cairo_new_path(c);
	cairo_move_to(c, 0, ps + DAY_H);
	cairo_line_to(c, HDR_W, ps + DAY_H);
	cairo_set_source_col(c, &black);
	cairo_stroke(c);

	/* hrule */
	cairo_new_path(c);
	cairo_move_to(c, HDR_W, ps + DAY_H);
	cairo_line_to(c, d->w, ps + DAY_H);
	cairo_set_source_col(c, &lgray2);
	cairo_stroke(c);

	/* select font and get */
	cairo_select_font_face (c, "Sans", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
	cairo_set_font_size (c, FONT_SIZE);

	/*
	 *
	 * ^        |           +-----+
	 * |        |           |     |
	 * |        | bearing   |     |
	 * |        |           |     |
	 * | height |           |     |
	 * |        v     (x,y) +-----+
	 * |                    |
	 * |                    |
	 * |                    |
	 *
	 */

	/* compute center of text */
	cairo_text_extents (c, r->name, &exts);
	x = 2;
	y = ( ps + ( DAY_H / 2 ) )  -  ( ( exts.height / 2 ) + exts.y_bearing );

	/* clipping zone */
	cairo_rectangle(c, 0, ps, HDR_W, ps + DAY_H);
	cairo_clip(c);

	/* display text */
	cairo_new_path(c);
	cairo_move_to(c, x, y);
	cairo_set_source_col(c, &black);
	cairo_show_text(c, r->name);
	cairo_stroke(c);

	/* end of clip */
	cairo_reset_clip(c);

}

void pla_cairo_disp_res(cairo_t *c, int ps, struct res *r, struct disp *d)
{
	struct task *t;
	double x1;
	double x2;
	double y1;
	double y2;
	int ok = 0;
	int i;

	list_for_each_entry(t, d->base, c) {

		/* doit on afficher */
		ok = 0;
		for (i=0; i<t->nres; i++)
			if (t->res[i] == r) {
				ok = 1;
				break;
			}
		if (ok == 0)
			continue;

		/* update dates */
		pla_task_update_date(t);

		/* affiche - t - on ? */
		if ( !( (t->start >= d->start && t->start <= d->start + d->duration) ||
		        (t->start + t->duration >= d->start && t->start + t->duration <= d->start + d->duration) ) )
			continue;

		/* calcule le départ */
		x1 = HDR_W + ( (t->start - d->start) / 86400 * DAY_W );
		x2 = HDR_W + ( (( t->start + t->duration ) - d->start) / 86400 * DAY_W );
		y1 = ps + TASK_BORD;
		y2 = ps + DAY_H - TASK_BORD;

		/* draw carré */
		cairo_new_path(c);
		cairo_move_to(c, x1, y1);
		cairo_line_to(c, x2, y1);
		cairo_line_to(c, x2, y2);
		cairo_line_to(c, x1, y2);
		cairo_line_to(c, x1, y1);
		cairo_set_source_col(c, &t->color);
		cairo_fill(c);

		/* draw black border */
		cairo_new_path(c);
		cairo_move_to(c, x1, y1);
		cairo_line_to(c, x2, y1);
		cairo_line_to(c, x2, y2);
		cairo_line_to(c, x1, y2);
		cairo_line_to(c, x1, y1);
		cairo_set_source_col(c, &black);
		cairo_stroke(c);

	}
}

static
void pla_cairo_day_feries(cairo_t *c, struct disp *d) {
	time_t r;
	int ps;
	struct tm tm;

	ps = HDR_W;
	for(r = d->start; r < d->start + d->duration; r += 86400) {
		localtime_r(&r, &tm);
		if (tm.tm_wday == 0 || tm.tm_wday == 6)
			pla_cairo_day_ferie(c, ps, d);

		/* TODO: a degager: 13 mai 21010 !!! jeudi de l'ascencion */
		if (tm.tm_mon == 4 && tm.tm_mday == 13)
			pla_cairo_day_ferie(c, ps, d);

		ps += DAY_W;
	}
}

static
void pla_cairo_days(cairo_t *c, struct disp *d) {
	time_t r;
	int ps;
	struct tm tm;

	ps = HDR_W;
	for(r = d->start; r < d->start + d->duration; r += 86400) {
		localtime_r(&r, &tm);
		pla_cairo_day(c, ps, tm.tm_mday, d);
		ps += DAY_W;
	}
}

static
void pla_cairo_months(cairo_t *c, struct disp *d) {
	time_t r;
	int ps;
	struct tm tm;
	int m_mem = -1;
	int start;

	ps = HDR_W;
	for(r = d->start; r < d->start + d->duration; r += 86400) {
		localtime_r(&r, &tm);

		if (m_mem != tm.tm_mon) {

			/* on dessine l'ancien mois */
			if (m_mem != -1)
				pla_cairo_month(c, start, ps, m_mem, d);

			/* on note le debut du nouveau mois */
			start = ps;
			m_mem = tm.tm_mon;

		}

		ps += DAY_W;
	}

	/* le dernier mois pas complet */
	pla_cairo_month(c, start, ps, m_mem, d);
}

static
void pla_cairo_heads(cairo_t *c, struct disp *d) {
	struct task *t;
	struct res *r;
	int ps;

	/* compte le nombre d'éléments a afficher */
	ps = HDR_DH + HDR_MH;
	list_for_each_entry(t, d->base, c) {
		pla_cairo_head(c, ps, t, d);
		ps += DAY_H;
	}

	ps = d->rs;
	list_for_each_entry(r, d->res, c) {
		pla_cairo_res_head(c, ps, r, d);
		ps += DAY_H;
	}
}

static
void pla_cairo_tasks(cairo_t *c, struct disp *d) {
	struct task *t;
	int ps;

	/* compte le nombre d'éléments a afficher */
	ps = HDR_DH + HDR_MH;
	list_for_each_entry(t, d->base, c) {
		pla_cairo_task(c, ps, t, d);
		ps += DAY_H;
	}
}

static
void pla_cairo_arrows(cairo_t *c, struct disp *d) {
	struct task *t;
	int ps;

	/* compte le nombre d'éléments a afficher */
	ps = HDR_DH + HDR_MH;
	list_for_each_entry(t, d->base, c) {
		pla_cairo_arrow(c, ps, t, d);
		ps += DAY_H;
	}
}

void pla_cairo_res(cairo_t *c, struct disp *d) {
	struct res *r;
	int ps;

	/* compte le nombre d'éléments a afficher */
	ps = d->rs;
	list_for_each_entry(r, d->res, c) {
		pla_cairo_disp_res(c, ps, r, d);
		ps += DAY_H;
	}
}

void pla_draw(int mode, const char *file_out, struct disp *d)
{
	cairo_surface_t *s;
	cairo_t *c;
	FILE *out;
	int nb;
	int nb2;
	struct task *t;
	struct res *r;

	/* compte le nombre d'éléments a afficher */
	nb = 0;
	list_for_each_entry(t, d->base, c)
		nb++;

	/* compte le nombre de resources */
	nb2 = 0;
	list_for_each_entry(r, d->res, c)
		nb2++;

	/* calcule X et Y */
	d->w = ( ( d->duration / 86400 ) * DAY_W ) + HDR_W;
	if (d->display_res == 1)
		d->h = ( nb *  DAY_H ) + HDR_DH + HDR_MH + SPACE + ( nb2 * DAY_H);
	else
		d->h = ( nb *  DAY_H ) + HDR_DH + HDR_MH;
	d->h1 = ( nb *  DAY_H ) + HDR_DH + HDR_MH;
	d->rs = ( nb *  DAY_H ) + HDR_DH + HDR_MH + SPACE;
	d->h2 = ( nb2 * DAY_H);

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

	/* create image */

	switch (mode) {

	/* PNG */
	case 1:
		s = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, d->w, d->h);
		break;

	/* EPS */
	case 2:
		s = cairo_ps_surface_create_for_stream(cairo_wr, out, d->w, d->h);
		cairo_ps_surface_set_eps(s, 1);
		break;

	/* SVG */
	case 3:
		s = cairo_svg_surface_create_for_stream(cairo_wr, out, d->w, d->h);
		break;

	/* PDF */
	case 4:
		s = cairo_pdf_surface_create_for_stream(cairo_wr, out, d->w, d->h);
		break;
	}

	/* create cairo */
	c = cairo_create(s);

	cairo_set_line_width(c, 0.2);
	
	/* draw background */
	cairo_new_path(c);
	cairo_move_to(c, 0, 0);
	cairo_line_to(c, 0, d->h);
	cairo_line_to(c, d->w, d->h);
	cairo_line_to(c, d->w, 0);
	cairo_set_source_col(c, &white);
	cairo_fill(c);

	/* draw top header background */
	cairo_new_path(c);
	cairo_move_to(c, d->w,  0);
	cairo_line_to(c, HDR_W, 0);
	cairo_line_to(c, HDR_W, HDR_MH + HDR_DH);
	cairo_line_to(c, d->w,  HDR_MH + HDR_DH);
	cairo_set_source_col(c, &lgray1);
	cairo_fill(c);

	/* draw top header shadow */
	cairo_new_path(c);
	cairo_move_to(c, d->w,  HDR_MH + ( HDR_DH * .85 ) );
	cairo_line_to(c, HDR_W, HDR_MH + ( HDR_DH * .85 ) );
	cairo_line_to(c, HDR_W, HDR_MH + HDR_DH);
	cairo_line_to(c, d->w,  HDR_MH + HDR_DH);
	cairo_set_source_col(c, &lgray2);
	cairo_fill(c);

	/* draw left task header background */
	cairo_new_path(c);
	cairo_move_to(c, 0,     d->h1);
	cairo_line_to(c, 0,     HDR_MH + HDR_DH);
	cairo_line_to(c, HDR_W, HDR_MH + HDR_DH);
	cairo_line_to(c, HDR_W, d->h1);
	cairo_set_source_col(c, &lgray1);
	cairo_fill(c);

	/* draw left resources header background */
	cairo_new_path(c);
	cairo_move_to(c, 0,     d->rs + d->h2);
	cairo_line_to(c, 0,     d->rs);
	cairo_line_to(c, HDR_W, d->rs);
	cairo_line_to(c, HDR_W, d->rs + d->h2);
	cairo_set_source_col(c, &lgray1);
	cairo_fill(c);

	/* vertical gray week-end and days "fériés" */
	pla_cairo_day_feries(c, d);

	/* month names */
	pla_cairo_months(c, d);

	/* days and vertical lines */
	pla_cairo_days(c, d);

	/* left headers content and horizontal lines */
	pla_cairo_heads(c, d);

	/* clip drawing zone */
	cairo_rectangle(c, HDR_W, HDR_MH + HDR_DH, d->w, d->h1);
	cairo_clip(c);

	/*  draw task and dependecies */
	pla_cairo_tasks(c, d);
	pla_cairo_arrows(c, d);

	/* unclip */
	cairo_reset_clip(c);

	/* clip drawing zone */
	cairo_rectangle(c, HDR_W, HDR_MH + HDR_DH, d->w, d->rs + d->h2);
	cairo_clip(c);

	/* draw ersources */
	pla_cairo_res(c, d);

	/* unclip */
	cairo_reset_clip(c);

	/* draw top header black border */
	cairo_new_path(c);
	cairo_move_to(c, d->w,  0);
	cairo_line_to(c, HDR_W, 0);
	cairo_line_to(c, HDR_W, HDR_MH + HDR_DH);
	cairo_line_to(c, d->w,  HDR_MH + HDR_DH);
	cairo_set_source_col(c, &black);
	cairo_stroke(c);

	/* draw top header black separation */
	cairo_new_path(c);
	cairo_move_to(c, HDR_W, HDR_MH);
	cairo_line_to(c, d->w,  HDR_MH);
	cairo_set_source_col(c, &black);
	cairo_stroke(c);

	/* draw left header black border top */
	cairo_new_path(c);
	cairo_move_to(c, 0,     d->h1);
	cairo_line_to(c, 0,     HDR_MH + HDR_DH);
	cairo_line_to(c, HDR_W, HDR_MH + HDR_DH);
	cairo_line_to(c, HDR_W, d->h1);
	cairo_set_source_col(c, &black);
	cairo_stroke(c);

	/* draw left header black border bottom */
	cairo_new_path(c);
	cairo_move_to(c, 0,     d->rs + d->h2);
	cairo_line_to(c, 0,     d->rs);
	cairo_line_to(c, HDR_W, d->rs);
	cairo_line_to(c, HDR_W, d->rs + d->h2);
	cairo_set_source_col(c, &black);
	cairo_stroke(c);

	/* draw left top border black bottom section */
	cairo_new_path(c);
	cairo_move_to(c, HDR_W, d->rs);
	cairo_line_to(c, d->w, d->rs);
	cairo_set_source_col(c, &lgray2);
	cairo_stroke(c);

	/* end of drawing */
	cairo_close_path(c);
	cairo_show_page(c);

	/* write image */
	if (mode == 1)
		cairo_surface_write_to_png_stream(s, cairo_wr, out);

	/* general vectoriel */
	else {
		cairo_surface_flush(s);
		cairo_surface_finish(s);
		cairo_surface_destroy(s);
		cairo_destroy(c);
	}

	/* end */
	fflush(out);
	fclose(out);
}
