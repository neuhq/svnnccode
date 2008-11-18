#include	<stdlib.h>
#include	<stdio.h>
#include	"wordsearch.h"

int main(int argc, char *argv[])
{
	if(argc == 2){
//		printf("usage: %s <wordsfile>\n", argv[0]);
//		return 1;
//	}
		words_arrange(argv[1], "1");	
		return 0;
	}
	word_search_init("1.idx", "1.txt");
//	word_search("中", NULL, 0);
	if(word_search2("张一元") == 1)
		printf("张一元 is a word\n");
	else
		printf("张一元 is not a word\n");
	return 0;
}
