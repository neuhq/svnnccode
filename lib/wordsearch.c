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
#include	"error.h"

#define	WORDSNUM	8*1024*1024
#define	BUFFLEN		1024
static	char*	wordptr[WORDSNUM];

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
	FILE*	tidx = fopen(buff, "w+");
	if(!tidx){
		err_msg("%s %s %i open file %s error",
				__FILE__, __func__, __LINE__, buff);
		return -1;
	}
	memset(buff, 0, sizeof(char) * BUFFLEN);
	snprintf(buff, BUFFLEN, "%s.txt", tpath);
	FILE*	ttxt = fopen(buff, "w+");
	if(!ttxt){
		err_msg("%s %s %i open file %s error",
				__FILE__, __func__, __LINE__, buff);
		return -1;
	}
	
}
