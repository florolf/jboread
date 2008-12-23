#include "global.h"

FILE *transcribe=NULL;

void check_transcribe(struct valsi *v) {
	if(!transcribe)
		return;
	
	fprintf(transcribe, "%s\n", v->v);
}
