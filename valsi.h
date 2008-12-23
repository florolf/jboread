struct valsi {
	enum {
		V_GISMU=1,
		V_CMENE,
		V_CMAVO,
		V_LUJVO,
		V_FUHIVLA,
		V_BROKEN
	} type;

	char *v;

	struct valsi *next, *prev;
	int x,y;
};
