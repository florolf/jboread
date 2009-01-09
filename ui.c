#include "global.h"

int with_colors=1;
int width, height;
int page=0, maxp;
struct valsi **breaks, *curw;

void color_state() {
	if(!has_colors())
		return;

	start_color();
	if(with_colors) {

		//i tried to fit my own synaesthetic perception of the word classes into ncurse's default colors, feel free to change this
		init_pair(1, COLOR_WHITE, COLOR_BLACK); //gismu
		init_pair(2, COLOR_RED, COLOR_BLACK); //cmene
		init_pair(3, COLOR_GREEN, COLOR_BLACK); //cmavo
		init_pair(4, COLOR_CYAN, COLOR_BLACK); //lujvo
		init_pair(5, COLOR_MAGENTA, COLOR_BLACK); //fu'ivla
		init_pair(6, COLOR_BLACK, COLOR_RED); //broken
	}
	else {
		init_pair(1, COLOR_WHITE, COLOR_BLACK);
		init_pair(2, COLOR_WHITE, COLOR_BLACK);
		init_pair(3, COLOR_WHITE, COLOR_BLACK);
		init_pair(4, COLOR_WHITE, COLOR_BLACK);
		init_pair(5, COLOR_WHITE, COLOR_BLACK);
		init_pair(6, COLOR_BLACK, COLOR_WHITE); //we still highlight broken stuff
	}
}

int init_ui() {
	initscr();
	cbreak();
	noecho();
	nonl();
	keypad(stdscr, TRUE);

	color_state();

	return 0;
}

void fix_pauses(char *word, char *fixed) {
	int i, j;

	i=j=0;
	if(strchr("aeiouy", *word)) {
		fixed[0]='.';
		i++;
	}
	
	for(; word[j]; i++, j++)
		fixed[i]=word[j];

	j--;
	if(strchr("bcdfgh'jklmnprstvxyz", word[j])) {
		fixed[i]='.';
		i++;
	}
	fixed[i]=0;
}

int check_paragraph(char *v) {
	return strcmp("i", v) && strcmp("ni'o", v) && strcmp("no'i", v);
}

void scan_pagebreaks(struct valsi *text) {
	struct valsi *cur;
	int xuse, yuse, i;
	int bsize;

	xuse=yuse=0;
	bsize=5;//lojban doesn't have long texts
	breaks=malloc(sizeof(struct valsi*) * bsize);

	breaks[0]=text;
	i=1;
	for(cur=text; cur; cur=cur->next) {
		int len;
		char fixed[512];

		fix_pauses(cur->v, fixed);

		len=strlen(fixed);
		if(!strcmp(cur->v, "ni'o")) yuse++;

		if(xuse+len > width || (cur->type == V_CMAVO && !check_paragraph(cur->v))) {
			yuse++;
			if(yuse >= height) {
				breaks[i]=cur;
				i++;
				if(i == bsize) {
					bsize += 5;
					breaks=realloc(breaks, sizeof(struct valsi*) * bsize);
				}
				yuse=0;
			}
			xuse=0;
		}
		cur->x=xuse;
		cur->y=yuse;

		xuse += len+1;
	}
	maxp = i-1;
}

void render_text(int page) {
	struct valsi *cur;

	clear();

	for(cur=breaks[page]; cur && cur != breaks[page+1]; cur=cur->next) {
		char fixed[512];

		fix_pauses(cur->v, fixed);

		attrset(COLOR_PAIR(cur->type));
		mvprintw(cur->y, cur->x, "%s", fixed);
	}

	refresh();
}

void jumpto(struct valsi *v) {
	struct valsi *tmp;

	for(tmp=v; tmp; tmp=tmp->prev) {
		int i;
		for(i=0; i <= maxp; i++)
			if(breaks[i] == tmp) {
				if(i != page) {
					page=i;
					render_text(page);
				}

				curw=v;
				move(curw->y, curw->x);
				return;
			}
	}
}

void show_message(char *msg) {
	int len;
	WINDOW *errw;

	len=strlen(msg);
	errw=newwin(5, MAX(len, sizeof("Press any key to continue"))+2, height/2, (width-len)/2);

	wborder(errw, 0,0,0,0,0,0,0,0);
	mvwprintw(errw, 1, 1, "%s", msg);
	mvwprintw(errw, 3, 1, "Press any key to continue");
	refresh();
	wrefresh(errw);

	getch();
	delwin(errw);
	render_text(page);
	move(curw->y, curw->x);
}

void display_gismu() {
	struct gismu_entry *g;
	char **tab, *rafsi, *p;
	int i;

	g=get_gismu(curw->v);
	if(!g) {
		show_message("This gismu does not exist");
		return;
	}

	tab=malloc(sizeof(char*)*12);
	rafsi=malloc(sizeof(char)*13);//3*3 letter + possible ' + 2 spaces + \0

	tab[0]="gismu"; tab[1]=g->gismu;
	tab[2]="gloss"; tab[3]=g->gloss;

	p=rafsi;
	for(i=0; i<3; i++) {
		if(g->rafsi[i]) {
			strcpy(p, g->rafsi[i]);
			p+=strlen(g->rafsi[i]);
			*p=' '; p++;
		}
	}
	*p=0;

	tab[4]="rafsi"; tab[5]=rafsi;
	tab[6]="def"; tab[7]=g->def;
	tab[8]="misc"; tab[9]=g->misc;
	tab[10]=tab[11]=NULL;

	display_tabular(tab);
	
	render_text(page);
	move(curw->y, curw->x);

	free(tab);
	free(rafsi);
}

void display_cmavo() {
	struct cmavo_entry *c;
	char **tab;

	c=get_cmavo(curw->v);
	if(!c) {
		show_message("This cmavo is not part of the DB");
		return;
	}

	tab=malloc(sizeof(char*)*10);

	tab[0]="cmavo"; tab[1]=c->cmavo;
	tab[2]="gloss"; tab[3]=c->gloss;
	tab[4]="selma'o"; tab[5]=c->selmaho;
	tab[6]="def"; tab[7]=c->descr;
	tab[8]=tab[9]=NULL;

	display_tabular(tab);
	
	render_text(page);
	move(curw->y, curw->x);

	free(tab);
}

void display_lujvo_decompose() {
	MorfType type;
	char *exp, *old, *p, **tab;
	int cnt, i;

	exp=strdup(canon_lujvo(curw->v));

	cnt=1;
	for(p=exp; *p; p++)
		if(*p == '+') cnt++;
		else if(*p == '?') {
			show_message("This lujvo is broken");
			return;
		}


	tab=malloc(sizeof(char*) * (cnt+1) * 2);

	i=0;
	for(p=old=exp; *p; p++)
		if(*p == '+') {
			*p = 0;
			tab[i] = strdup(old);

			type = vlatai(tab[i]);
			if(type == MT_CMAVOS) {
				struct cmavo_entry *c;
				
				c=get_cmavo(tab[i]);
				tab[i+1]=c->gloss;
			}
			else if(type == MT_GISMU) {
				struct gismu_entry *g;
				
				g=get_gismu(tab[i]);
				tab[i+1]=g->gloss;
			}


			old=p+1;
			p++;
			i += 2;
		}


	tab[i] = strdup(old);

	type = vlatai(tab[i]);
	if(type == MT_CMAVOS) {
		struct cmavo_entry *c;
		
		c=get_cmavo(tab[i]);
		tab[i+1]=c->descr;
	}
	else if(type == MT_GISMU) {
		struct gismu_entry *g;
		
		g=get_gismu(tab[i]);
		tab[i+1]=g->def;
	}


	tab[i+2]=tab[i+3]=NULL;
	display_tabular(tab);

	for(i=0; i < 2*cnt; i += 2)
		free(tab[i]);
	free(tab);
}

void display_lujvo_jbovlaste() {
	struct lujvo_entry *l;
	char **tab;

	l=get_lujvo(curw->v);
	if(!l) {
		show_message("jbovlaste doesn't know of this lujvo");
		return;
	}

	tab=malloc(sizeof(char*) * 4);
	tab[0]=l->lujvo;
	tab[1]=l->def;
	tab[2]=tab[3]=NULL;

	display_tabular(tab);
	free(tab);
}

void display_lujvo() {
	struct box_choice box[] = {{'j', "jbovlaste"},{'d', "Decompose lujvo"},{0,0}};
	int c;

	c=choice_box(box);
	
	switch(c) {
		case 'd':
			display_lujvo_decompose();
			break;
		case 'j':
			display_lujvo_jbovlaste();
			break;
	}

	render_text(page);
	move(curw->y, curw->x);
}

void lookup_valsi() {
	switch(curw->type) {
		case V_GISMU:
			check_transcribe(curw);
			display_gismu();
			break;
		case V_CMAVO:
			check_transcribe(curw);
			display_cmavo();
			break;
		case V_LUJVO:
			check_transcribe(curw);
			display_lujvo();
			break;
		case V_CMENE: //looking up cmene doesn't really make sense
		case V_BROKEN:
			break;
		default:
			show_message("This word type is not implemented yet");
			break;
	}
}

void match_struct() {
	struct valsi *v;
	char *match;
	int nest, forward;

	if(curw->type != V_CMAVO)
		return;

	match=cmavo_get_matching(curw->v, &forward);
	if(!match)
		return;

	nest=0;
	for(v=curw; v; v = forward?v->next:v->prev) {
		if(!strcmp(v->v, curw->v)) {
			nest++;
			continue;
		}
		if(!strcmp(v->v, match)) {
			nest--;
			if(nest == 0) {
				curw=v;
				goto end;
			}
		}
	}
end:
	free(match);
	return;
}

void ui_loop(struct valsi *text) {
	getmaxyx(stdscr, height, width);
	scan_pagebreaks(text);

	curw=text;
	render_text(page);
	move(curw->y, curw->x);
	while(1) {
		int c;

		c=getch();
		switch(c) {
			case KEY_LEFT:
				if(curw->prev == NULL)
					break;
				curw=curw->prev;
				if(page > 0 && curw->next==breaks[page]) {
					page--;
					render_text(page);
				}
				move(curw->y, curw->x);
				break;
			case KEY_RIGHT:
				if(curw->next == NULL)
					break;
				curw=curw->next;
				if(page < maxp && curw==breaks[page+1]) {
					page++;
					render_text(page);
				}
				move(curw->y, curw->x);
				break;
			case KEY_UP:
				if(curw->prev)
					curw=curw->prev;
				for(; curw->prev && check_paragraph(curw->v); curw=curw->prev);
				jumpto(curw);
				break;
			case KEY_DOWN:
				if(curw->next)
					curw=curw->next;
				for(; curw->next && check_paragraph(curw->v); curw=curw->next);
				jumpto(curw);
				break;
			case KEY_PPAGE:
				if(page > 0) {
					page--;
					render_text(page);
					curw=breaks[page];
					move(curw->y, curw->x);
				}
				break;
			case KEY_NPAGE:
				if(page < maxp) {
					page++;
					render_text(page);
					curw=breaks[page];
					move(curw->y, curw->x);
				}
				break;
			case KEY_HOME:
				for(; curw->prev && check_paragraph(curw->v); curw = curw->prev);
				jumpto(curw);
				break;
			case KEY_END:
				for(; curw->next && check_paragraph(curw->next->v); curw = curw->next);
				jumpto(curw);
				break;
			case 'c':
				with_colors=1-with_colors; //toggle colors
				color_state();
				refresh();
				break;
			case ' ':
				lookup_valsi();
				break;
			case 'q':
				return;
				break;
			case '[':
			case ']':
				match_struct();
				jumpto(curw);
				break;
		}
	}
}
