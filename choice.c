#include "global.h"

extern int width, height;

int choice_box(struct box_choice *choice) {
	int i, maxlen, n, c;
	WINDOW *box;

	maxlen=n=0;

	for(i=0; choice[i].key; i++) {
		int len;

		len = strlen(choice[i].descr) + 4;
		if(len > maxlen) maxlen=len;

		n++;
	}

	maxlen = MAX(maxlen, strlen("Press any other key to abort"));

	box=newwin(n+4, maxlen+4, height/2 - (n/2) - 2 , width/2 - (maxlen/2) - 2);
	wborder(box, 0, 0, 0, 0, 0, 0, 0, 0);

	for(i=0; choice[i].key; i++)
		mvwprintw(box, i+1, 2, "%c: %s", choice[i].key, choice[i].descr);

	mvwprintw(box, i+2, 2, "Press any other key to abort", choice[i].key, choice[i].descr);
        refresh();
        wrefresh(box);

	c=getch();

	delwin(box);
	return c;
}
