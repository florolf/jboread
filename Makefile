OBJS=info.o\
     main.o\
     parse.o\
     table.o\
     transcription.o\
     ui.o

all: $(OBJS)
	gcc -o jboread $(OBJS) morf/libmorf.a -lncurses

clean:
	rm *.o
	rm jboread
