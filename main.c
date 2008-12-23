#include <global.h>

void finish() {
	clear();
	refresh();
	endwin();
	exit(0);
}

int main(int argc, char **argv) {
	FILE *in;
	struct valsi *text;

	if(argc < 2) {
		fprintf(stderr, "Usage: %s file\n", argv[0]);
		return EXIT_FAILURE;
	}

	in=fopen(argv[1], "r");
	if(!in) {
		perror("Couldn't open input file for reading");
		return EXIT_FAILURE;
	}

	if(parse_text(in, &text)) {
		fprintf(stderr, "Failed to parse text.\n");
		return EXIT_FAILURE;
	}

	atexit(finish);

	if(init_ui()) {
		fprintf(stderr, "Failed to initialize UI.\n");
		return EXIT_FAILURE;
	}

	ui_loop(text);
	return EXIT_SUCCESS;
}
