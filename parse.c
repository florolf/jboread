#include "global.h"

int allow_cultural_rafsi=1;

int vlatai(struct valsi **v) {
	char *start[256], **pstart;
	struct morf_xtra xtra;
	MorfType type;

	pstart=start;
	type = morf_scan((*v)->v, &pstart, &xtra);
	if(type == MT_BOGUS || type == MT_BAD_UPPERCASE) (*v)->type = V_BROKEN;
	if(type == MT_GISMU) (*v)->type = V_GISMU;
	if(type == MT_LUJVO) (*v)->type = V_LUJVO;
	if(type == MT_CMENE) (*v)->type = V_CMENE;
	if(type >= MT_FUIVLA3 && type <= MT_FUIVLA4) (*v)->type = V_FUHIVLA; //i hope this works

	if(type == MT_CMAVOS) {
		(*v)->type = V_CMAVO;

		if(start[0] != *pstart) { //compound cmavo
			struct valsi *cur=*v;
			char *valsi = (*v)->v;
			int i;

		        for(i=0; start[i] != *pstart; i++) {
                		struct valsi *tmp;
				char buf[512];
				
		                memset(buf,0,512);

		                strncpy(buf, start[i], start[i+1]-start[i]);

				cur->v = strdup(buf);
				tmp=malloc(sizeof(struct valsi));
				tmp->type=V_CMAVO;
				tmp->prev=cur;
				cur->next=tmp;
				cur=tmp;
		        }

			cur->v=strdup(*pstart);
			free(valsi);
			*v=cur;
		}
	}

	return 0;
}

int parse_text(FILE *in, struct valsi **text) {
	char buf[512];
	int c, i;
	struct valsi *cur;

	i=0;
	memset(buf, 0, 512);

	cur=NULL;

	while(c=getc(in)) {
/*		c=tolower(c);
		if(!strchr(" '.abcdefghijklmnoprstuvxyz", c))
			continue;*/
		if(isspace(c) || c=='.' || c==EOF || !cur) { //space or beginning? start new word
			struct valsi *tmp;

			if(cur) {
				buf[i]=0;
				cur->v=strdup(buf);

				if(vlatai(&cur))
					return 1;
				i=0;
			}

			while(isspace(c) || c == '.')
				c=getc(in);
			ungetc(c, in); //skip spaces

			if(c==EOF)
				return 0;

			tmp=malloc(sizeof(struct valsi));
			tmp->prev=cur;
			if(cur)
				cur->next=tmp;
			else
				*text=tmp;

			cur=tmp;
			continue;
		}

		buf[i]=c;
		i++;

		if(i > 511) {
			fprintf(stderr, "Word too long\n");
			return 1;
		}
	}

	return 0;
}
