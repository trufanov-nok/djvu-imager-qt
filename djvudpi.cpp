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
#include "stdafx.h"

// modified by monday2000@yandex.ru to change DPI

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

char *curfile = 0;

int new_dpi;
int func_choice;
int page_count = 0;

int width;
int height;

void 
die(const char *s)
{
  AfxMessageBox(s);

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

void
write8(FILE *f, int offset, int val)
{
  unsigned char r[1];
  r[0] = val;
  if (fseek(f, offset, SEEK_SET) < 0)
    die("Cannot seek");
  if (fwrite(r,1,1,f) != 1)
    die("Cannot write");
}

long
read32(FILE *f, int offset)
{
  //long x;
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
	      if (func_choice == 2)
			   page_count++;

		  processN(f,offset+4, end);
	  }

      if  (formid == 0x444a564d  )  /* DJVM */        
		  processN(f,offset+4, end);
    }
  else if (chkid == 0x494e464f)  /* INFO */
    {
	  /*
      int vlo = read8(f, offset+4);
      int vhi = read8(f, offset+5);
      int version = (vhi<<8)|vlo;

      if (version > maxversion)
      {
         printf("%s: converting version %d to %d\n", 
                 curfile, version, maxversion);
		  printf("%s: DPI: %d\n", 
          write8(f, offset+4, maxversion & 0xff);
          write8(f, offset+5, (maxversion>>8) & 0xff);
      }  
	  	   
      write8(f, offset+4, maxversion & 0xff);
      write8(f, offset+5, (maxversion>>8) & 0xff);
      */
    if (func_choice == 0)
	{
      int dlo = read8(f, offset+6);
      int dhi = read8(f, offset+7);
      int dpi_old = (dhi<<8)|dlo;

      printf("%s: converting DPI %d to %d\n", 
                 curfile, dpi_old, new_dpi);

      write8(f, offset+6, new_dpi & 0xff);
      write8(f, offset+7, (new_dpi>>8) & 0xff);  
	}
	else if (func_choice == 1)
	{
      int whi = read8(f, offset+0);
      int wlo = read8(f, offset+1);
      width = (whi<<8)|wlo;

      int hhi = read8(f, offset+2);
      int hlo = read8(f, offset+3);
      height = (hhi<<8)|hlo;
	}
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



int ChangeDPI(CString InPath, CString StDPI)
{	
	func_choice = 0;

	FILE *f;
	
    curfile = InPath.GetBuffer(InPath.GetLength());
	new_dpi = atoi(StDPI.GetBuffer(StDPI.GetLength()));
    if (! (f = fopen(curfile,"rb+")))
		die("Cannot open djvu file.");
    if (fseek(f,0,SEEK_END) < 0)
		die("Cannot seek through file.");
	processN(f, 0, ftell(f));
    fclose(f);	  
		  
	return 0;
};

int GetDjvuSizes(CString InPath, int& djvu_image_width, int& djvu_image_height)
{	
	func_choice = 1;

	FILE *f;
	
    curfile = InPath.GetBuffer(InPath.GetLength());
    if (! (f = fopen(curfile,"rb+")))
		die("Cannot open djvu file.");
    if (fseek(f,0,SEEK_END) < 0)
		die("Cannot seek through file.");
	processN(f, 0, ftell(f));
    fclose(f);	

	char buf[20]={0};

	djvu_image_width = width;
	djvu_image_height = height;
		  
	return 0;
};

int GetDjvuPagesNum(CString DjvuPath)
{
	func_choice = 2;

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