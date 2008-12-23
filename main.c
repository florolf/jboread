#include "global.h"

extern FILE *transcribe;

void finish() {
	clear();
	refresh();
	endwin();

	if(transcribe)
		fclose(transcribe);

	exit(0);
}

int main(int argc, char **argv) {
	FILE *in;
	struct valsi *text;

	while(1) {
		int c;

		c = getopt(argc, argv, "x:t");
		
		if(c == -1)
			break;

		switch(c) {
			case 'x': //oh my god. this hurts so much. isn't there a non-broken way to do this?
				if(transcribe)
					break;
				transcribe = fopen(optarg, "a+");
				if(!transcribe) {
					perror("Opening transcript");
					return EXIT_FAILURE;
				}
				break;
			case 't':
				if(transcribe)
					break;
				transcribe = fopen("transcript", "a+");
				if(!transcribe) {
					perror("Opening transcript");
					return EXIT_FAILURE;
				}
				break;
		}
	}

	if(argc-optind == 0) {
		fprintf(stderr, "Usage: %s [-t] [-x tfile] file\n\
-t\tTranscribe to ./transcript\n\
-x tfile\tTranscribe to tfile\n", argv[0]);
		return EXIT_FAILURE;
	}

	in=fopen(argv[optind], "r");
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
