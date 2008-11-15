#include	<stdio.h>
#include	<string.h>
#include	"sim2tri.h"
#include	"error.h"

int main(int argc, char *argv[])
{
	if(argc != 5){
		err_msg("usage: %s <convertinfo> <soure> <target> <st/ts>", argv[0]);
		return 1;
	}
	FILE* info = fopen(argv[1], "r");
	if(!info)
		err_sys("Open file %s failed", argv[1]);
	FILE* s = fopen(argv[2], "r");
	if(!s)
		err_sys("Open file %s failed", argv[2]);
	FILE* t = fopen(argv[3], "w+");
	if(!t)
		err_sys("Open file %s failed", argv[3]);
	
	if(stinit(argv[1]) < 0)
		err_sys("stinit failed");
	if(strcmp(argv[4], "st") == 0){
		if(file_s2t(argv[2], argv[3]) < 0)
			err_sys("file_s2t error");
	}
	else if(strcmp(argv[4], "ts") == 0){
		if(file_t2s(argv[2], argv[3]) < 0)
			err_sys("file_t2s error");
	}
	else{
		err_msg("%s error only st or ts is valid", argv[4]);
		return 1;
	}
	fclose(t);
	return 0;
}
