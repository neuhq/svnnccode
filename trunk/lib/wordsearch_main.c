#include	<stdlib.h>
#include	<stdio.h>
#include	"wordsearch.h"

int main(int argc, char *argv[])
{
/*	if(argc != 2){
		printf("usage: %s <wordsfile>", argv[0]);
		return 1;
	}
	words_arrange(argv[1], "1");	*/
	word_search_init("1.idx", "1.txt");
	word_search("一", NULL, 0);
	return 0;
}
