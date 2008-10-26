#include	<stdio.h>
#include	<string.h>

#define	MAXBUFFER	8192

/* read zhwikir-xxxxxxxx-pages-articles.xml for words
 *
 *
 */
int main(int argc, char *argv[])
{
	char	buff[MAXBUFFER];
	char	out[MAXBUFFER];

	memset(buff, 0, sizeof(char) * MAXBUFFER);
	while(fgets(buff, MAXBUFFER, stdin)){
		char* p1 = buff;
		char* p2;
		do{
			p1 = strstr(p1, "[[");
			if(!p1)
				break;
			p2 = strstr(p1, "]]");
			if(p1 && p2){
				memset(out, 0, sizeof(char) * MAXBUFFER);
				p1 += strlen("[[");
				strncpy(out, p1 , p2 - p1 );
				puts(out);
				p1 = p2;
			}
		}while(p1 && p2);
		memset(buff, 0, sizeof(char) * MAXBUFFER);
	}
	return 0;
}
