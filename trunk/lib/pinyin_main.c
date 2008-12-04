#include	<stdlib.h>
#include	<unistd.h>
#include	<sys/types.h>
#include	<sys/stat.h>
#include	<fcntl.h>
#include	<stdio.h>
#include	<string.h>
#include	"wordsearch.h"
#include	"pinyin.h"

int main(int argc, char *argv[])
{
	if(argc == 3){
		//pinyin_to_data(argv[1], argv[2]);
		//return 0;
		hanzi_pinyin_init(argv[1]);
		char buff2[1024];
		hanzi_to_pinyin(argv[2], buff2, 1024);
		printf("%s\n", buff2);
		return 0;
	}
	else
		return -1;
	return 0;
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

	char t[1024*4] = {0};
	char splited[1024*4] = {0};
	word_split_r("中国美国加拿大墨西哥", splited, 1024*4);
	printf("%s\n", splited);

	while(fgets(buff, 1024, stdin)){
		char* p = buff;
		while(p - buff <strlen(buff) && strlen(p)){
			char t[1024*4] = {0};
			char splited[1024*4] = {0};
			char splited_r[1024*4] = {0};
			sentence_clear(p, &p, t, 1024*4);
			word_split_r(t, splited_r, 1024*4);
			word_split(t, splited, 1024*4);
			if(strcmp(splited, splited_r)){
				printf("%s\n", splited);
				printf("%s\n", splited_r);
			}
//			printf("%s\n", splited);
		}
		memset(buff, 0, sizeof(char) * 1024 * 4);
	}
	return 0;
}
