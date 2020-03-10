/*
  Copyright 2003, Leon Bottou

  Permission to use, copy, modify, distribute, and sell this software and its
  documentation for any purpose is hereby granted without fee, provided that
  the above copyright notice appear in all copies and that both that
  copyright notice and this permission notice appear in supporting
  documentation.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
  THE OPEN GROUP BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
  IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
  CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

// This is a DjVuVersion utility code modified to calculate the number of pages
// in a DjVu file
//
// Author: monday2000@yandex.ru
#include "stdafx.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

char *curfile = 0;
int page_count = 0;

void 
die(const char *s)
{
	fprintf(stderr,"djvucount: %s");
	if (curfile)
		fprintf(stderr,": %s", curfile);
	if (errno > 0)
		perror(": ");
	fprintf(stderr,"\n");  
	exit(10);
}

int
read8(FILE *f, int offset)
{
	unsigned char r[1];
	if (fseek(f, offset, SEEK_SET) < 0)
		die("Cannot seek");
	if (fread(r,1,1,f) != 1)
		die("Cannot read");
	return r[0];
}


long
read32(FILE *f, int offset)
{
	//  long x;
	unsigned char r[4];
	fseek(f, offset, SEEK_SET);
	if (fread(r,1,4,f) != 4)
		die("Premature end of file");
	return ((((((r[0]<<8)|r[1])<<8)|r[2])<<8)|r[3]);
}

void processN(FILE *f, long offset, long end);

void
process1(FILE *f, long chkid, int offset, int end)
{
	if (chkid == 0x464f524d) /* FORM */
    {
		long formid = read32(f, offset);
		if  (formid == 0x444a5655)   /* DJVU */
		{
			page_count++;
			processN(f,offset+4, end);
		}
		
		if  (formid == 0x444a564d)  /* DJVM */	  
			processN(f,offset+4, end); 
    }
}

void 
processN(FILE *f, long offset, long end)
{
	while (offset < end)
    {
		long chkid = read32(f,offset);
		if (chkid == 0x41542654) /* AT&T */
        {
			offset += 4;
        }
		else
        {
			long chklen = read32(f,offset+4);
			long chkend = offset+8+chklen;
			process1(f, chkid, offset+8, chkend);
			offset = chkend;
        }
		if (offset & 1)
			offset += 1;
    }
}

/*
int 
main(int argc, char **argv)
{
	FILE* f;
	
	if (argc < 2)
	{
		fprintf(stderr, "Usage: djvucount djvu_file_path\n");
		exit(10);
    }
	
	curfile = argv[1];
	if (! (f = fopen(curfile,"rb+")))
		die("Cannot open djvu file.");
	if (fseek(f,0,SEEK_END) < 0)
		die("Cannot seek through file.");
	processN(f, 0, ftell(f));
	fclose(f);
	  
	printf("page count = %d",page_count);
		  
	return 0;
}
*/

int GetDjvuPagesNum(CString DjvuPath)
{
	int page_num=0;
	
	page_count=0;

	FILE* f;

	curfile = DjvuPath.GetBuffer(DjvuPath.GetLength());
	if (! (f = fopen(curfile,"rb+")))
		die("Cannot open djvu file.");
	if (fseek(f,0,SEEK_END) < 0)
		die("Cannot seek through file.");
	processN(f, 0, ftell(f));
	fclose(f);	

	page_num = page_count;
		  
	return page_num;
}