#include	<search.h>
#include	<stdlib.h>
#include	<string.h>
#include	<stdio.h>
#include	<ctype.h>
#include	"error.h"

#define	MAXNUM	1024*1024*8

char* key;
void** data;
static	int	index_key;
static	int index_data;

void insertdata(char* str)
{
	int j = 0;
	for(j = 0; j<strlen(str); j++)
		if(isascii(str[j]))
			return;
	static	int init = 0;
	if(!init){
		init = 1;
		index_key = 0;
		index_data = 0;
		if(!hcreate(MAXNUM))
			err_sys("hcreate error in %s line %i", __FILE__, __LINE__);
		key = malloc(MAXNUM*8*sizeof(char));
		if(!key)
			err_sys("malloc error in %s line %i", __FILE__, __LINE__);
		memset(key, 0, MAXNUM*8*sizeof(char));
		data = malloc(MAXNUM * sizeof(void*));
		if(!data)
			err_sys("malloc error in %s line %i", __FILE__, __LINE__);
		memset(data, 0, MAXNUM * sizeof(void));
	}
	ENTRY e, *ep;
	strncpy(key+index_key, str, strlen(str) );
	e.key = key + index_key;

	data[index_data] = (void*)(e.key);
	e.data = data + index_data;
	ep = hsearch(e, ENTER);
	if(!ep)
		err_sys("hsearch error in %s line %i", __FILE__, __LINE__);
	if(ep->key == e.key){
		index_key += strlen(str) + 1;
		index_data ++;
		printf("%s\n", str);
		return;
	}
	else{
		memset(key + index_key, 0, strlen(str));
	}

}
void output()
{
	int i;
	for(i = 0; i<index_data; i++)
		printf("%s\n", (char*)(data[i]));
}
