/*
 *   wordsearch.c: 
 *   Copyright (C) <2008>  <svncode：svncode@gmail.com>
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
#include	<sys/types.h>
#include	<sys/stat.h>
#include	<fcntl.h>
#include	<stdint.h>
#include	<stdlib.h>
#include	<stdio.h>
#include	<stringprep.h>
#include	<string.h>
#include	<sys/types.h>
#include	<sys/mman.h>
#include	<netinet/in.h>
#include	"error.h"
#include	"libdef.h"
#include	"sim2tri.h"

#define	WORDSNUM	8*1024*1024
#define	BUFFLEN		1024

static int utf8_cmp(const void* p1, const void* p2)
{
	int l1, l2;
	const char* s1 = *(char* const*)p1, *s2 = *(char* const*)p2;
	while( (l1 = utf8_length(s1)) > 1 && (l2 = utf8_length(s2)) > 1){
		uint32_t i1 = stringprep_utf8_to_unichar(s1);
		uint32_t i2 = stringprep_utf8_to_unichar(s2);
		if(i1 != i2)
			return (i1 - i2);
		s1 += l1;
		s2 += l2;
		continue;
	}
	if(l1 <= 1)
		return 1;
	return -1;
}

static	char*		wordptr[WORDSNUM];

/*	words_arrange
 *	const char* spath: file containing words
 *	const char* tpath: file to store arranged words
 *					   tpath.idx	words index
 *					   tpath.txt	words
 */
int	words_arrange(const char* spath, const char* tpath)
{
	char	buff[BUFFLEN];
	FILE*	f = fopen(spath, "r");
	if(!f){
		err_msg("%s %s %i open file %s error",
				__FILE__, __func__, __LINE__, spath);
		return -1;
	}
	memset(buff, 0, sizeof(char) * BUFFLEN);
	snprintf(buff, BUFFLEN, "%s.idx", tpath);
	int	fidx = open(buff, O_RDWR | O_CREAT | O_EXCL, 
					S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH);
	if(fidx < 0){
		err_msg("%s %s %i open file %s error",
				__FILE__, __func__, __LINE__, buff);
		return -1;
	}
	if(lseek(fidx, UNICODELEN * sizeof(int32_t) - 1, SEEK_SET) == -1){
		err_msg("%s %s %i lseek file %s.idx error",
				__FILE__, __func__, __LINE__, tpath);
		return -1;
	}
	write(fidx, "\0", 1);
	memset(buff, 0, sizeof(char) * BUFFLEN);
	snprintf(buff, BUFFLEN, "%s.txt", tpath);
	int	ftxt = open(buff, O_RDWR | O_CREAT | O_EXCL,
					S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH);
	if(ftxt < 0){
		err_msg("%s %s %i open file %s error",
				__FILE__, __func__, __LINE__, buff);
		return -1;
	}
	caddr_t	idx;
	if((idx = mmap(0, UNICODELEN * sizeof(uint32_t), PROT_READ | PROT_WRITE,
					 MAP_SHARED, fidx, 0) ) == (caddr_t)-1){
		err_msg("%s %s %i mmap failed", __FILE__, __func__, __LINE__);
		return -1;
	}
	memset(idx, 0, UNICODELEN * sizeof(uint32_t));	
/*	munmap(idx, UNICODELEN * sizeof(uint32_t));
	return 0;*/

	memset(wordptr, 0, sizeof(char*) * WORDSNUM);
	memset(buff, 0, sizeof(char) * BUFFLEN);
	int index = 0;
	while(fgets(buff, BUFFLEN, f) != NULL){
		char*	t = buff;
		do{
			while(*t && utf8_length(t) <= 1)
				t++;
			if(!*t){
				memset(buff, 0, sizeof(char) * BUFFLEN);
				continue;
			}
			char* t1 = t;
			int l ;
			while(*t && (l = utf8_length(t)) > 1){
				t += l;
			}
			if(index < WORDSNUM){
				wordptr[index] = malloc((t - t1 + 1) * sizeof(char*) );
				if(!wordptr[index])
					err_sys("%s %s %i malloc error",
							__FILE__, __func__, __LINE__);
				strncpy(wordptr[index], t1, t - t1);
				wordptr[index][t-t1] = '\0';
				index ++;
				continue;
			}
			else{
				err_msg("%s WORDSNUM is too small", __FILE__);
				return -1;
			}
		}while(*t);
	}
	qsort(wordptr, index, sizeof(char*), utf8_cmp);
	
	int i;
	uint32_t pre = 0;
	uint32_t* idxp = (uint32_t*)idx;
	for(i = 0; i < index; i++){
		uint32_t now = stringprep_utf8_to_unichar(wordptr[i]);
		if(now != pre){
			pre = now;
			uint32_t addr = lseek(ftxt, 0, SEEK_CUR);
			if(addr == (off_t)-1){
				err_msg("%s %s %i lseek file %s.txt error",
						__FILE__, __func__, __LINE__, tpath);
				return -1;
			}
			if(addr >= UNICODELEN){
				err_msg("UNICODELEN(%i) is too small", UNICODELEN);
				continue;
			}
			idxp[now] = htonl(addr);
		}
		memset(buff, 0, BUFFLEN * sizeof(char));
		strncpy(buff, wordptr[i], BUFFLEN);
		buff[strlen(buff)] = '\n';
		write(ftxt, buff, strlen(buff));
		free(wordptr[i]);
	}
	close(ftxt);
	munmap(idx, UNICODELEN * sizeof(uint32_t));
	close(fidx);
	return 0;
}

static int	idxf;
static int	wordf;
uint32_t*	idxm;
caddr_t	wordm;
static int	sign = 0;
int word_search_init(const char* idxpath, const char* wordpath)
{
	struct stat statbuff;
	if(stat(idxpath, &statbuff) < 0){
		err_msg("%s %s %i stat %s error",
				__FILE__, __func__, __LINE__, idxpath);
		return -1;
	}
	idxf = open(idxpath, O_RDONLY);
	if(idxf < 0){
		err_msg("%s %s %i open %s error",
				__FILE__, __func__, __LINE__, idxpath);
		return -1;
	}
	caddr_t addrt = mmap(0, statbuff.st_size, PROT_READ, MAP_SHARED, idxf, 0);
	if(addrt == (caddr_t)-1){
		err_msg("%s %s %i mmap %s error",
				__FILE__, __func__, __LINE__, idxpath);
		return -1;
	}
	idxm = (uint32_t*)addrt;

	if(stat(wordpath, &statbuff) < 0){
		err_msg("%s %s %i stat %s error",
				__FILE__, __func__, __LINE__, wordpath);
		return -1;
	}
	wordf = open(wordpath, O_RDONLY);
	if(wordf < 0){
		err_msg("%s %s %i open %s error",
				__FILE__, __func__, __LINE__, wordpath);
		return -1;
	}
	wordm = mmap(0, statbuff.st_size, PROT_READ, MAP_SHARED, wordf, 0);
	if(wordm == (caddr_t)-1){
		err_msg("%s %s %i mmap %s error",
				__FILE__, __func__, __LINE__, wordpath);
		return -1;
	}
	sign = 1;
	return 0;
}

int word_search(const char* word, char* buff, int len)
{
	if(!sign){
		err_msg("please call word_search_init first");
		return -1;
	}
	if(utf8_length(word) <= 1)
		return -1;
	uint32_t index_unicode = stringprep_utf8_to_unichar(word);
	if(index_unicode >= UNICODELEN){
		err_msg("%s %s %i UNICODELEN is too small %s",
				__FILE__, __func__, __LINE__, word);
		return -1;
	}
	uint32_t index_word = ntohl(idxm[index_unicode]);
	uint32_t i;
	uint32_t index_end;
	for(i=index_unicode + 1; i < UNICODELEN; i++ ){
		if(ntohl(idxm[i]) != 0)
			break;
	}
	index_end = ntohl(idxm[i]);
	write(STDOUT_FILENO, (wordm + index_word), index_end - index_word);
	return 0;
}
