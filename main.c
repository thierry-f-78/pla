#include <string.h>
#include <stdio.h>

#include "pla.h"
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
	struct task *ref0_1;
	struct task *ref0_2;
	struct task *ref0_3;
	struct task *ref0;
	struct task *ref8_1;
	struct task *ref8_2;
	struct task *ref8_3;
	struct task *ref8_4;
	struct task *ref8_5;
	struct task *ref8;
	struct task *ref6_1;
	struct task *ref6_2;
	struct task *ref6_3;
	struct task *ref6_4;
	struct task *ref6_5;
	struct task *ref6;
	struct task *ref9_1;
	struct task *ref9_2;
	struct task *ref9_3;
	struct task *ref9_4;
	struct task *ref9_5;
	struct task *ref9;
	struct task *ref7_1;
	struct task *ref7_2;
	struct task *ref7_3;
	struct task *ref7_4;
	struct task *ref7_5;
	struct task *ref7_6;
	struct task *ref7;
	struct task *ref2_1;
	struct task *ref2_2;
	struct task *ref2_3;
	struct task *ref2_4;
	struct task *ref2_5;
	struct task *ref2_6;
	struct task *ref2;


	ref0 = pla_task_new(&base, "REF0 Install des serveurs", NULL, 0, 0);

	ref0_1 = pla_task_new(&base, "REF0 Install 1", NULL, 0, 0);
	pla_task_set_start_ymd(ref0_1, "20100408");
	pla_task_set_duration_sd(ref0_1, "1");

	ref0_2 = pla_task_new(&base, "REF0 Install 2", NULL, 0, 0);
	pla_task_set_start_ymd(ref0_2, "20100409");
	pla_task_set_duration_sd(ref0_2, "1");

	ref0_3 = pla_task_new(&base, "REF0 Install 3", NULL, 0, 0);
	pla_task_set_start_ymd(ref0_3, "20100412");
	pla_task_set_duration_sd(ref0_3, "1");

	pla_task_add_child(ref0, ref0_1);
	pla_task_add_child(ref0, ref0_2);
	pla_task_add_child(ref0, ref0_3);


	ref8 = pla_task_new(&base, "REF8 DNS entrant", NULL, 0, 0);

	ref8_1 = pla_task_new(&base, "REF8 Etude", NULL, 0, 0);
	pla_task_set_start_ymd(ref8_1, "20100401");
	pla_task_set_duration_sd(ref8_1, "1");

	ref8_2 = pla_task_new(&base, "REF8 Config 1", NULL, 0, 0);
	pla_task_set_start_ymd(ref8_2, "20100409");
	pla_task_set_duration_sd(ref8_2, "1");

	ref8_3 = pla_task_new(&base, "REF8 Config 2", NULL, 0, 0);
	pla_task_set_start_ymd(ref8_3, "20100412");
	pla_task_set_duration_sd(ref8_3, "1");

	ref8_4 = pla_task_new(&base, "REF8 Migration", NULL, 0, 0);
	pla_task_set_start_ymd(ref8_4, "20100415");
	pla_task_set_duration_sd(ref8_4, "1");

	ref8_5 = pla_task_new(&base, "REF8 Doc", NULL, 0, 0);
	pla_task_set_start_ymd(ref8_5, "20100416");
	pla_task_set_duration_sd(ref8_5, "1");

	pla_task_add_child(ref8, ref8_1);
	pla_task_add_child(ref8, ref8_2);
	pla_task_add_child(ref8, ref8_3);
	pla_task_add_child(ref8, ref8_4);
	pla_task_add_child(ref8, ref8_5);


	ref6 = pla_task_new(&base, "REF6 Mail entrant", NULL, 0, 0);

	ref6_1 = pla_task_new(&base, "REF6 Etude", NULL, 0, 0);
	pla_task_set_start_ymd(ref6_1, "20100422");
	pla_task_set_duration_sd(ref6_1, "1");

	ref6_2 = pla_task_new(&base, "REF6 Config 1", NULL, 0, 0);
	pla_task_set_start_ymd(ref6_2, "20100423");
	pla_task_set_duration_sd(ref6_2, "1");

	ref6_3 = pla_task_new(&base, "REF6 Config 2", NULL, 0, 0);
	pla_task_set_start_ymd(ref6_3, "20100426");
	pla_task_set_duration_sd(ref6_3, "1");

	ref6_4 = pla_task_new(&base, "REF6 Migration", NULL, 0, 0);
	pla_task_set_start_ymd(ref6_4, "20100427");
	pla_task_set_duration_sd(ref6_4, "1");

	ref6_5 = pla_task_new(&base, "REF6 Doc", NULL, 0, 0);
	pla_task_set_start_ymd(ref6_5, "20100428");
	pla_task_set_duration_sd(ref6_5, "1");

	pla_task_add_child(ref6, ref6_1);
	pla_task_add_child(ref6, ref6_2);
	pla_task_add_child(ref6, ref6_3);
	pla_task_add_child(ref6, ref6_4);
	pla_task_add_child(ref6, ref6_5);


	ref9 = pla_task_new(&base, "REF9 Mail sortant", NULL, 0, 0);

	ref9_1 = pla_task_new(&base, "REF9 Etude", NULL, 0, 0);
	pla_task_set_start_ymd(ref9_1, "20100506");
	pla_task_set_duration_sd(ref9_1, "1");

	ref9_2 = pla_task_new(&base, "REF9 Config 1", NULL, 0, 0);
	pla_task_set_start_ymd(ref9_2, "20100507");
	pla_task_set_duration_sd(ref9_2, "1");

	ref9_3 = pla_task_new(&base, "REF9 Config 2", NULL, 0, 0);
	pla_task_set_start_ymd(ref9_3, "20100514");
	pla_task_set_duration_sd(ref9_3, "1");

	ref9_4 = pla_task_new(&base, "REF9 Migration", NULL, 0, 0);
	pla_task_set_start_ymd(ref9_4, "20100520");
	pla_task_set_duration_sd(ref9_4, "1");

	ref9_5 = pla_task_new(&base, "REF9 Doc", NULL, 0, 0);
	pla_task_set_start_ymd(ref9_5, "20100521");
	pla_task_set_duration_sd(ref9_5, "1");

	pla_task_add_child(ref9, ref9_1);
	pla_task_add_child(ref9, ref9_2);
	pla_task_add_child(ref9, ref9_3);
	pla_task_add_child(ref9, ref9_4);
	pla_task_add_child(ref9, ref9_5);


	ref7 = pla_task_new(&base, "REF7 Browsing HTTP", NULL, 0, 0);

	ref7_1 = pla_task_new(&base, "REF7 Etude", NULL, 0, 0);
	pla_task_set_start_ymd(ref7_1, "20100527");
	pla_task_set_duration_sd(ref7_1, "1");

	ref7_2 = pla_task_new(&base, "REF7 Config 1", NULL, 0, 0);
	pla_task_set_start_ymd(ref7_2, "20100528");
	pla_task_set_duration_sd(ref7_2, "1");

	ref7_3 = pla_task_new(&base, "REF7 Config 2", NULL, 0, 0);
	pla_task_set_start_ymd(ref7_3, "20100602");
	pla_task_set_duration_sd(ref7_3, "1");

	ref7_4 = pla_task_new(&base, "REF7 Migration", NULL, 0, 0);
	pla_task_set_start_ymd(ref7_4, "20100603");
	pla_task_set_duration_sd(ref7_4, "2");

	ref7_5 = pla_task_new(&base, "REF7 Reprise", NULL, 0, 0);
	pla_task_set_start_ymd(ref7_5, "20100607");
	pla_task_set_duration_sd(ref7_5, "1");

	ref7_6 = pla_task_new(&base, "REF7 Doc", NULL, 0, 0);
	pla_task_set_start_ymd(ref7_6, "20100608");
	pla_task_set_duration_sd(ref7_6, "2");

	pla_task_add_child(ref7, ref7_1);
	pla_task_add_child(ref7, ref7_2);
	pla_task_add_child(ref7, ref7_3);
	pla_task_add_child(ref7, ref7_4);
	pla_task_add_child(ref7, ref7_5);
	pla_task_add_child(ref7, ref7_6);


	ref2 = pla_task_new(&base, "REF2 WIFI Interne", NULL, 0, 0);

	ref2_1 = pla_task_new(&base, "REF2 Etude", NULL, 0, 0);
	pla_task_set_start_ymd(ref2_1, "20100617");
	pla_task_set_duration_sd(ref2_1, "2");

	ref2_2 = pla_task_new(&base, "REF2 Config VPN", NULL, 0, 0);
	pla_task_set_start_ymd(ref2_2, "20100621");
	pla_task_set_duration_sd(ref2_2, "1");

	ref2_3 = pla_task_new(&base, "REF2 Config règles", NULL, 0, 0);
	pla_task_set_start_ymd(ref2_3, "20100622");
	pla_task_set_duration_sd(ref2_3, "2");

	ref2_4 = pla_task_new(&base, "REF2 Reorg infra", NULL, 0, 0);
	pla_task_set_start_ymd(ref2_4, "20100624");
	pla_task_set_duration_sd(ref2_4, "1");

	ref2_5 = pla_task_new(&base, "REF2 Migration", NULL, 0, 0);
	pla_task_set_start_ymd(ref2_5, "20100625");
	pla_task_set_duration_sd(ref2_5, "1");

	ref2_6 = pla_task_new(&base, "REF2 Doc", NULL, 0, 0);
	pla_task_set_start_ymd(ref2_6, "20100628");
	pla_task_set_duration_sd(ref2_6, "1");

	pla_task_add_child(ref2, ref2_1);
	pla_task_add_child(ref2, ref2_2);
	pla_task_add_child(ref2, ref2_3);
	pla_task_add_child(ref2, ref2_4);
	pla_task_add_child(ref2, ref2_5);
	pla_task_add_child(ref2, ref2_6);

	/* set dependencies */
	pla_task_add_dep(ref0_1, ref0_2);
	pla_task_add_dep(ref0_1, ref8_2);
	pla_task_add_dep(ref0_2, ref0_3);
	pla_task_add_dep(ref0_3, ref6_2);
	pla_task_add_dep(ref0_3, ref9_2);
	pla_task_add_dep(ref0_3, ref7_2);
	pla_task_add_dep(ref8_1, ref8_2);
	pla_task_add_dep(ref8_2, ref8_3);
	pla_task_add_dep(ref8_3, ref8_4);
	pla_task_add_dep(ref8_4, ref8_5);
	pla_task_add_dep(ref6_1, ref6_2);
	pla_task_add_dep(ref6_2, ref6_3);
	pla_task_add_dep(ref6_3, ref6_4);
	pla_task_add_dep(ref6_4, ref6_5);
	pla_task_add_dep(ref9_1, ref9_2);
	pla_task_add_dep(ref9_2, ref9_3);
	pla_task_add_dep(ref9_3, ref9_4);
	pla_task_add_dep(ref9_4, ref9_5);
	pla_task_add_dep(ref7_1, ref7_2);
	pla_task_add_dep(ref7_2, ref7_3);
	pla_task_add_dep(ref7_3, ref7_4);
	pla_task_add_dep(ref7_4, ref7_5);
	pla_task_add_dep(ref7_5, ref7_6);
	pla_task_add_dep(ref2_1, ref2_2);
	pla_task_add_dep(ref2_2, ref2_3);
	pla_task_add_dep(ref2_3, ref2_4);
	pla_task_add_dep(ref2_4, ref2_5);
	pla_task_add_dep(ref2_5, ref2_6);

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

	if (argc < 2) {
		out = "pla.png";
		mode = 1;
	}
	
	else {
		out = argv[1];
		p = strchr(out, '.');
		if (p == NULL)
			mode = 1; 
		else if (strcmp(p, ".png") == 0)
			mode = 1;
		else if (strcmp(p, ".eps") == 0)
			mode = 2;
		else if (strcmp(p, ".svg") == 0)
			mode = 3;
		else if (strcmp(p, ".pdf") == 0)
			mode = 4;
	}

	pla_draw(mode, out, &d);
	return 0;
}
