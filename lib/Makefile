
#$.o : %.c
#$(CC) -c -Wall -g $(CFLAGS) $(CPPFLAGS) $<-o $@

CFLAGS := -Wall -g `pkg-config libidn --cflags --libs`

sim2tri = error.o sim2tri_main.o sim2tri.o
sim2tri_main : ${sim2tri}
	gcc ${CFLAGS} ${sim2tri} -o sim2tri_main

wordsearch = wordsearch_main.o wordsearch.o error.o sim2tri.o ishanzi.o
wordsearch_main : $(wordsearch)
	gcc ${CFLAGS} ${wordsearch} -o wordsearch_main

ishanzi = ishanzi.o ishanzi_main.o
ishanzi_main : ${ishanzi}
	gcc ${CFLAGS} ${ishanzi} -o ishanzi_main

pinyin = pinyin_main.o wordsearch.o error.o sim2tri.o ishanzi.o pinyin.o
pinyin_main : $(pinyin)
	gcc ${CFLAGS} ${pinyin} -o pinyin_main

clean:
	rm *.o
