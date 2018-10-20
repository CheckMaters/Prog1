all:simpleCSVsorter

simpleCSVsorter: mergesort.h movieListData.h movieValueList.h columnsorter.h tokenizer.h simpleCSVsorter.h generalFunctions.h mergesort.c movieListData.c movieValueList.c columnsorter.c tokenizer.c simpleCSVsorter.c movieListData.h
	gcc -Wall -Werror -fsanitize=address -lm mergesort.c movieListData.c movieValueList.c columnsorter.c tokenizer.c simpleCSVsorter.c movieListData.h -o simpleCSVsorter

clean:
	rm -rf simpleCSVsorter
