#include	<stdlib.h>
#include	<unistd.h>
#include	<sys/types.h>
#include	<sys/stat.h>
#include	<fcntl.h>
#include	<stdio.h>
#include	"wordsearch.h"

int main(int argc, char *argv[])
{
	if(argc == 2){
		words_arrange(argv[1], "1");	
		return 0;
	}
	word_search_init("1.idx", "1.txt");
//	word_search("中", NULL, 0);
	if(word_search2("我们") == 1)
		printf("我们 is a word\n");
	else
		printf("我们 is not a word\n");

	char buff[1024*4] = {0};
	memset(buff, 0, sizeof(char) * 1024 * 4);
	int off;
	FILE* file = fopen("split.txt", "r");
	int fd = open("out.txt", O_APPEND | O_WRONLY);
	while(fgets(buff, 1024, stdin)){
		char* p = buff;
		while(p - buff <strlen(buff) && strlen(p)){
			char t[1024*4] = {0};
			sentence_clear(p, &p, t, 1024*4);
				printf("%s\n", t);
		}
		memset(buff, 0, sizeof(char) * 1024 * 4);
	}
	return 0;
}
