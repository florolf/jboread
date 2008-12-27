#include "global.h"

#include "db/gismu_db.h"
#include "db/cmavo_db.h"
#include "db/lujvo_db.h"

struct gismu_entry *get_gismu(char *v) {
	int i;

	for(i=0; gismu_db[i].gismu; i++)
		if(!strcmp(v, gismu_db[i].gismu))
			return &gismu_db[i];

	return NULL; //durka?
}

struct cmavo_entry *get_cmavo(char *v) {
	int i;

	for(i=0; cmavo_db[i].cmavo; i++)
		if(!strcmp(v, cmavo_db[i].cmavo))
			return &cmavo_db[i];

	return NULL;
}

struct lujvo_entry *get_lujvo(char *v) {
	int i;
	
	//FIXME: This only works if v is in canonical form

	for(i=0; lujvo_db[i].lujvo; i++)
		if(!strcmp(v, lujvo_db[i].lujvo))
			return &lujvo_db[i];

	return NULL;
}

struct {
	char *k, *v;
} cmatch[] = {
	{"lu", "li'u"},
	{"to", "toi"},
	{"lo'u","le'u"}, //this is probably broken
	{NULL, NULL}
};

char *cmavo_get_matching(char *v, int *forward) {
	int i;

	*forward=1;
	for(i=0; cmatch[i].k; i++)
		if(!strcmp(v, cmatch[i].k))
			return strdup(cmatch[i].v);
	*forward=0;
	for(i=0; cmatch[i].k; i++)
		if(!strcmp(v, cmatch[i].v))
			return strdup(cmatch[i].k);
	return NULL;
}
