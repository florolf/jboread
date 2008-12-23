#libmorf: morf.o morf_dfa.o bccheck.o canonluj.o
#        gcc -shared -fPIC -o libmorf.so morf.o morf_dfa.o bccheck.o canonluj.o
libmorf: morf.o morf_dfa.o bccheck.o canonluj.o
	ar rc libmorf.a morf.o morf_dfa.o bccheck.o canonluj.o
	ranlib libmorf.a
