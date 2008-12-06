/*
 *   pinyin.c: 
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
#include	"ishanzi.h"
#include	"globaldef.h"

typedef struct _Pinyin{
	char	py[8];
}Pinyin;

#define	MAXPINYIN	8
int pinyin_to_data(const char* pinyin, const char* data)
{
	int fdt;
	FILE* fpy = fopen(pinyin, "r");
	if(fpy == NULL)
		ERR_SYS("fopen file %s error %x", pinyin, fpy);
	if( (fdt = open(data, O_RDWR | O_CREAT | O_EXCL,
					S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH )) < 0 )
		ERR_SYS("Open file %s error", data);
	if(lseek(fdt, MAXUNICODE * MAXPINYIN * sizeof(char) -1 , SEEK_SET) 
			== (off_t) -1)
		ERR_SYS("lseek %s error", data);
	write(fdt, "\0", 1);
	caddr_t mdt;
	if( (mdt = mmap(0, MAXUNICODE * MAXPINYIN * sizeof(char) - 1,
					PROT_READ | PROT_WRITE, MAP_SHARED, 
					fdt, 0)) == (caddr_t)-1)
		ERR_SYS("mmap %s error", data);
	memset(mdt, 0, sizeof(char) * MAXUNICODE * MAXPINYIN);
	Pinyin* pa = (Pinyin*)mdt;

	char buff[BUFFLEN] = {0};
	uint32_t multi = 0x10000;
	while(fgets(buff, BUFFLEN, fpy)){
		char* t = strstr(buff, " ");
		if(!t){
			ERR_MSG("format error: %s", buff);
			continue;
		}
		char py[8] = {0};
		strncpy(py, buff, t - buff);
		t++;
		while(t-buff<strlen(buff)){
			int l = utf8_length(buff);
			char hz[8] = {0};
			strncpy(hz, t, l);
			if(ishanzi(hz)){
				uint32_t unicode = stringprep_utf8_to_unichar(t);
				if(unicode >= MAXUNICODE)
					ERR_MSG("MAXUNICODE is too small");
				char* p = pa[unicode].py;
				if(p[0] == '\0'){
					p[0] = '1';
					p[1] = ' ';
					strncpy(p+2, py, 6);
				}
				else if(p[0] == '1'){
					uint32_t off = multi;
					multi += 16;
					strncpy(pa[off].py + 1, p+1, MAXPINYIN - 1);
					pa[off].py[0] = 's';
					memset(p, 0, MAXPINYIN * sizeof(char));
					char addr[16] = {0};
					snprintf(addr, 16, "%i", off);
					if(strlen(addr) > MAXPINYIN - 1){
						ERR_MSG("addr is big");
						continue;
					}
					strncpy(p+1, addr, MAXPINYIN - 1);
					p[0] = '2';
					off++;
					if(off >= MAXUNICODE)
						ERR_MSG("MAXUNICODE is too small");
					strncpy(pa[off].py + 2, py, MAXPINYIN - 2);
					ERR_MSG("pinyin is %s", py);
					pa[off].py[0] = 's';
					pa[off].py[1] = ' ';
				}
				else{
					int n = p[0] - '0';
					char coff[8] = {0};
					strncpy(coff, p+1, MAXPINYIN - 1 );
					uint32_t off = atoi(coff);
					p[0] ++;
					if(off + n >= MAXUNICODE)
						ERR_MSG("MAXUNICODE is too small");
					p = pa[off+ n].py;
					strncpy(p+2, py, MAXPINYIN - 2);
					p[0] = 's';
					p[1] = ' ';
				}
			}
			t += l;
		}
	}
	munmap(mdt, MAXUNICODE * MAXPINYIN * sizeof(char));
	close(fdt);
	fclose(fpy);
	return 0;
}
static Pinyin* pinyina = NULL;
int hanzi_pinyin_init(const char* pinyin)
{
	if(pinyina)
		return 0;
	int fdt;
	if( (fdt = open(pinyin, O_RDONLY,
					S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH )) < 0 )
		ERR_SYS("Open file %s error", pinyin);
	caddr_t mdt;
	if( (mdt = mmap(0, MAXUNICODE * MAXPINYIN * sizeof(char) - 1,
					PROT_READ, MAP_SHARED, 
					fdt, 0)) == (caddr_t)-1)
		ERR_SYS("mmap %s error", pinyin);
	pinyina = (Pinyin*)mdt;
	return 0;
}

int hanzi_to_pinyin(const char* u, char* pinyin, int len)
{
	memset(pinyin, 0, sizeof(char) * len);
	uint32_t unicode = stringprep_utf8_to_unichar(u);
	char* py = pinyina[unicode].py;
	if(py[0] == '1'){
		if(len < MAXPINYIN){
			ERR_MSG("len is too small");
			return -1;
		}
		strncpy(pinyin, py + 2, 6);
		return 0;
	}
	char t[8] = {0};
	strncpy(t, py + 1, 7);
	int off = atoi(t);
	int n = py[0] - '0';
	if(len < MAXPINYIN * n){
		ERR_MSG("len is too small");
		return -1;
	}
	int i;
	char* p = pinyin;
	for(i = 0; i < n; i++){
		strncpy(p, pinyina[off+i].py + 2, 6);
		p[strlen(p)] = '\t';
		p += strlen(p);
	}
	return 0;
}

int pinyin_to_list(const char* pinyin, const char* list)
{
	return 0;
}
