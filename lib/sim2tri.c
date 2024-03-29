/*
 *   sim2tri.c: 
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
#include	<stdint.h>
#include	<stdio.h>
#include	<stringprep.h>
#include	<string.h>
#include	"error.h"

#define	BLOCKNUM	(1<<20)
#define MAXBYTE		1024
static uint16_t s[BLOCKNUM];
static uint16_t t[BLOCKNUM];

/* check utf8 length 
 * 110xxxxx 10xxxxxx	
 * 1110xxxx 10xxxxxx 10xxxxxx
 * 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
 * 111110xx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
 * 1111110x 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
 * */
inline int utf8_length(const char* s)
{
	unsigned char c = (unsigned char)(s[0]);
	if(!(c & 0x80))
		return 1;
/*	else if((c & 0x80) == 0x80)
		return 1;*/
	else if((c & 0xFC) == 0xFC)
		return 6;
	else if((c & 0xF8) == 0xF8)
		return 5;
	else if((c & 0xF0) == 0xF0)
		return 4;
	else if((c & 0xE0) == 0xE0)
		return 3;
	else if((c & 0xC0) == 0xC0)
		return 2;
	else
		return 1;
}

/* init s(Simplified) t(Traditional) */
int stinit(const char* path)
{
	int lineno = 0;
	FILE* file = fopen(path, "r");
	if(!file){
		err_msg("%s open %s error", path);
		return -1;
	}
	memset(s, 0, BLOCKNUM * sizeof(uint16_t));
	memset(t, 0, BLOCKNUM * sizeof(uint16_t));
	
	char buff[MAXBYTE];
	memset(buff, 0, MAXBYTE * sizeof(char));
	while(fgets(buff,MAXBYTE, file)){
		lineno++;
		char* temp = buff;
		int l;
		while(*temp && (l = utf8_length(temp)) <= 1){
			temp ++;
		}
		if(!temp){
			err_msg("in %s file %s lineno %i %s data error(original\\ttarget)",
					__func__, path, lineno, buff);
			memset(buff, 0, MAXBYTE * sizeof(char));
			continue;
			
		}
		uint32_t u1 = stringprep_utf8_to_unichar(temp);
		temp += l;

		while(*temp &&  (l = utf8_length(temp)) <= 1 ) {
			temp ++;
		}
		if(!temp){
			err_msg("in %s file %s lineno %i %s data error(original\\ttarget)",
					__func__, path, lineno, buff);
			memset(buff, 0, MAXBYTE * sizeof(char));
			continue;
		}
		uint32_t u2 = stringprep_utf8_to_unichar(temp);

		if(u1 < BLOCKNUM && u2 < BLOCKNUM){
			s[u1] = u2;
			t[u2] = u1;
		}else{
			err_msg("func %s file %s line %i %s, unichar is greater than 2^20",
					__func__, path, lineno, buff);
		}
		memset(buff, 0, MAXBYTE * sizeof(char));
	}
	return 0;
}

int s2t(const char* ss, char* tt)
{
	uint32_t si = stringprep_utf8_to_unichar(ss);
	if(si >= BLOCKNUM){
		return -1;
	}
	if(s[si] == 0)
		return 0;
	return stringprep_unichar_to_utf8(s[si], tt);
}

int t2s(const char* ss, char* tt)
{
	uint32_t si = stringprep_utf8_to_unichar(ss);
	if(si >= BLOCKNUM){
		return -1;
	}
	if(t[si] == 0)
		return 0;
	return stringprep_unichar_to_utf8(t[si], tt);
}

int file_s2t(const char* spath, const char* tpath)
{
	FILE* ss = fopen(spath, "r");
	if(!ss){
		err_msg("Open file %s error", spath);
		return -1;
	}
	FILE* tt = fopen(tpath, "w");
	if(!tt){
		err_msg("Open file %s error", tpath);
		return -1;
	}

	char buff[MAXBYTE];
	char outbuff[MAXBYTE];
	memset(buff, 0, sizeof(char) * MAXBYTE);
	memset(outbuff, 0, sizeof(char) * MAXBYTE);
	while(fgets(buff, MAXBYTE, ss)){
		char* tb = buff;
		char* to = outbuff;
		while(tb - buff < strlen(buff)){
			if(utf8_length(tb) <= 1){
				*to++ = *tb++;
				continue;
			}
			int i = s2t(tb, to);
			if(i <= 0){
				int j = utf8_length(tb);
				strncpy(to, tb, j);
				to += j;
				tb += j;
			}
			else{
				to += i;
				tb += utf8_length(tb);
			}
		}
		if(fputs(outbuff, tt) == EOF)
			err_sys("in %s %s %i fputs error", __FILE__, __func__, __LINE__);
		memset(buff, 0, sizeof(char) * MAXBYTE);
		memset(outbuff, 0, sizeof(char) * MAXBYTE);
	}
	return 0;
}

int file_t2s(const char* spath, const char* tpath)
{
	FILE* ss = fopen(spath, "r");
	if(!ss){
		err_msg("Open file %s error", spath);
		return -1;
	}
	FILE* tt = fopen(tpath, "w");
	if(!tt){
		err_msg("Open file %s error", tpath);
		return -1;
	}

	char buff[MAXBYTE];
	char outbuff[MAXBYTE];
	memset(buff, 0, sizeof(char) * MAXBYTE);
	memset(outbuff, 0, sizeof(char) * MAXBYTE);
	while(fgets(buff, MAXBYTE, ss)){
		char* tb = buff;
		char* to = outbuff;
		while(tb - buff < strlen(buff)){
			if(utf8_length(tb) <= 1){
				*to++ = *tb++;
				continue;
			}
			int i = t2s(tb, to);
			if(i <= 0){
				int j = utf8_length(tb);
				strncpy(to, tb, j);
				to += j;
				tb += j;
			}
			else{
				to += i;
				tb += utf8_length(tb);
			}
		}
		if(fputs(outbuff, tt) == EOF)
			err_sys("in %s %s %i fputs error", __FILE__, __func__, __LINE__);
		memset(buff, 0, sizeof(char) * MAXBYTE);
		memset(outbuff, 0, sizeof(char) * MAXBYTE);
	}
	return 0;
}
