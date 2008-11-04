/*
 *   Parsewiki: read zhwiki-xxxxxxxx-pages-articles.xml for words
 *   Copyright (C) <year>  <name of author：svncode@gmail.com>
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
#include	<stdio.h>
#include	<string.h>
#include	"../lib/hash.h"

#define	MAXBUFFER	8192

#define	LEFT_SEP	"[["
#define RIGH_SEP	"]]"

typedef struct _ADDRESS{
	int left;
	char* p;
}address;

/* read zhwiki-xxxxxxxx-pages-articles.xml for words
 *
 *
 */
int main(int argc, char *argv[])
{
	char	buff[MAXBUFFER];
	char	out[MAXBUFFER];
	address	stack[MAXBUFFER];

	memset(buff, 0, sizeof(char) * MAXBUFFER);
	memset(stack, 0, sizeof(address*) * MAXBUFFER);
	while(fgets(buff, MAXBUFFER, stdin)){
		int i = 0;
		char* p1 = buff;
		char* p2;
		do{
			char* t = p1;
			p1 = strstr(t, LEFT_SEP);
			p2 = strstr(t, RIGH_SEP);
			if(p1 && p2){
				if(p2 > p1){
					stack[i].p = p1 + strlen(LEFT_SEP);
					stack[i++].left = 1;
					p1 += strlen(LEFT_SEP);
				}
				else{
					stack[i].left = -1;
					stack[i++].p = p2;
					p1 = p2 + strlen(RIGH_SEP);
				}
			}
			else if(p1){
				stack[i].left = 1;
				p1 += strlen(LEFT_SEP);
				stack[i++].p = p1;
			}
			else if(p2){
				stack[i].left = -1;
				stack[i++].p = p2;
				p1 = p2 + strlen(RIGH_SEP);
			}
			else	break;
		}while(p1 && *p1);

		int j;
		int k = 0;

		for(j = 1; j < i; j ++){
			if(stack[j].left < 0 && stack[j-1].left > 0){
				memset(out, 0, sizeof(char) * MAXBUFFER);
				strncpy(out, stack[j-1].p, stack[j].p - stack[j-1].p);
				insertdata(out);
				if(k>1)
					k --;
			}
//			else
//				stack[++k] = stack[j];
		}

		
		p1 = buff;
/*		do{
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
*/
		memset(buff, 0, sizeof(char) * MAXBUFFER);
		memset(stack, 0, sizeof(address) * MAXBUFFER);
	}
	output();
	return 0;
}
