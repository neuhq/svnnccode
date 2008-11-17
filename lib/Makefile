
#big52gbk_convert.o :
#	g++ -c big52gbk_convert.cpp

#$.o : %.c
#$(CC) -c -Wall -g $(CFLAGS) $(CPPFLAGS) $<-o $@

CFLAGS := -Wall -g `pkg-config libidn --cflags --libs`

wordsearch = wordsearch_main.o wordsearch.o error.o sim2tri.o
wordsearch_main : $(wordsearch)
	gcc ${CFLAGS} ${wordsearch} -o wordsearch_main

clean:
	rm *.o
