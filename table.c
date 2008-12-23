#include "global.h"

extern int width, height;

void display_tabular(char **tabs) {
	WINDOW *win;
	char **lines=NULL;
	int maxkey, maxval, maxlen;
	int i, nlines;

	maxkey=maxval=0;

	for(i=0; tabs[i]; i += 2) {
		if(strlen(tabs[i]) > maxkey)
			maxkey = strlen(tabs[i]);
	}

	for(i=1; tabs[i]; i += 2) {
		if(strlen(tabs[i]) > maxval)
			maxval = strlen(tabs[i]);
	}

	maxlen = maxkey + maxval + 3;

	if(maxlen > width/2)
		maxlen=width/2;

	nlines=0;
	for(i=0; tabs[i]; i += 2) {
		nlines++;
		lines=realloc(lines,sizeof(char*)*nlines);

		if(maxkey + strlen(tabs[i+1]) + 3 <= maxlen) {
			lines[nlines-1] = malloc(maxkey + strlen(tabs[i+1]) + 4);
			sprintf(lines[nlines-1], " %-*s: %s", maxkey, tabs[i], tabs[i+1]);
		}
		else {
			char *tmp;
			lines[nlines-1] = malloc(maxlen+1);
			sprintf(lines[nlines-1], " %-*s:", maxkey, tabs[i]);

			tmp=tabs[i+1];
			while(1) {
				char *tmp2;

				nlines++;
				lines=realloc(lines,sizeof(char*)*nlines);
				lines[nlines-1] = malloc(maxlen+1);

				if(strlen(tmp)+3 < maxlen) {
					sprintf(lines[nlines-1], "  %s ", tmp);
					break;
				}
					
				tmp2 = tmp + maxlen-3;
				for(; *tmp2 != ' '; tmp2--);
				
				lines[nlines-1][0]=' ';
				lines[nlines-1][1]=' ';
				strncpy(&lines[nlines-1][2], tmp, tmp2-tmp);
				lines[nlines-1][tmp2-tmp+2]=0;
				tmp=tmp2+1;
			}
		}
	}

	win=newwin(nlines+2, maxlen+3, height/2-nlines/2-1, width/2-maxlen/2-1);
	wborder(win,0,0,0,0,0,0,0,0);

	for(i=0; i<nlines; i++)
		mvwprintw(win, i+1, 1, "%s", lines[i]);

        refresh();
        wrefresh(win);

	getch();

	for(i=0; i<nlines; i++)
		free(lines[i]);
	free(lines);
	delwin(win);
}
