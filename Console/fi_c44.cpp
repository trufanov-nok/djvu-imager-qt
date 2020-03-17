//C-  -*- C++ -*-
//C- -------------------------------------------------------------------
//C- DjVuLibre-3.5
//C- Copyright (c) 2002  Leon Bottou and Yann Le Cun.
//C- Copyright (c) 2001  AT&T
//C-
//C- This software is subject to, and may be distributed under, the
//C- GNU General Public License, Version 2. The license should have
//C- accompanied the software or you may obtain a copy of the license
//C- from the Free Software Foundation at http://www.fsf.org .
//C-
//C- This program is distributed in the hope that it will be useful,
//C- but WITHOUT ANY WARRANTY; without even the implied warranty of
//C- MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//C- GNU General Public License for more details.
//C- 
//C- DjVuLibre-3.5 is derived from the DjVu(r) Reference Library
//C- distributed by Lizardtech Software.  On July 19th 2002, Lizardtech 
//C- Software authorized us to replace the original DjVu(r) Reference 
//C- Library notice by the following text (see doc/lizard2002.djvu):
//C-
//C-  ------------------------------------------------------------------
//C- | DjVu (r) Reference Library (v. 3.5)
//C- | Copyright (c) 1999-2001 LizardTech, Inc. All Rights Reserved.
//C- | The DjVu Reference Library is protected by U.S. Pat. No.
//C- | 6,058,214 and patents pending.
//C- |
//C- | This software is subject to, and may be distributed under, the
//C- | GNU General Public License, Version 2. The license should have
//C- | accompanied the software or you may obtain a copy of the license
//C- | from the Free Software Foundation at http://www.fsf.org .
//C- |
//C- | The computer code originally released by LizardTech under this
//C- | license and unmodified by other parties is deemed "the LIZARDTECH
//C- | ORIGINAL CODE."  Subject to any third party intellectual property
//C- | claims, LizardTech grants recipient a worldwide, royalty-free, 
//C- | non-exclusive license to make, use, sell, or otherwise dispose of 
//C- | the LIZARDTECH ORIGINAL CODE or of programs derived from the 
//C- | LIZARDTECH ORIGINAL CODE in compliance with the terms of the GNU 
//C- | General Public License.   This grant only confers the right to 
//C- | infringe patent claims underlying the LIZARDTECH ORIGINAL CODE to 
//C- | the extent such infringement is reasonably necessary to enable 
//C- | recipient to make, have made, practice, sell, or otherwise dispose 
//C- | of the LIZARDTECH ORIGINAL CODE (or portions thereof) and not to 
//C- | any greater extent that may be necessary to utilize further 
//C- | modifications or combinations.
//C- |
//C- | The LIZARDTECH ORIGINAL CODE is provided "AS IS" WITHOUT WARRANTY
//C- | OF ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED
//C- | TO ANY WARRANTY OF NON-INFRINGEMENT, OR ANY IMPLIED WARRANTY OF
//C- | MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.
//C- +------------------------------------------------------------------
// 
// $Id: c44.cpp,v 1.12 2003/11/07 22:08:22 leonb Exp $
// $Name:  $

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif
#if NEED_GNUG_PRAGMAS
# pragma implementation
#endif

/** @name c44

 {\bf Synopsis}\\
 \begin{verbatim}
 c44 [options] pnmfile [djvufile]
 c44 [options] jpegfile [djvufile]
 \end{verbatim}
 
  {\bf Description} ---
  File #"c44.cpp"# illustrates the use of classes \Ref{IWBitmap} and
  \Ref{IWPixmap} for compressing and encoding a color image or a gray level
  image using the DjVu IW44 wavelets.  This is the preferred mode for
  creating a DjVu image which does not require separate layers for encoding
  the text and the background images.  The files created by #c44# are
  legal Photo DjVu files recognized by the DjVu decoder (see program \Ref{ddjvu}).
  
   {\bf Arguments} ---
   Argument #pnmfile# or #jpegfile# is the name of the input file.  PGM files
   are recognized for gray level images; PPM files are recognized for color
   images.  Popular file formats can be converted to PGM or PPM using the
   NetPBM package (\URL{http://www.arc.umn.edu/GVL/Software/netpbm.html})
   or the ImageMagick package (\URL{http://www.wizards.dupont.com/cristy/}).
   
    Optional argument #djvufile# is the name of the output file.  It is
    customary to use either suffix #".djvu"#, #".djv"#, #".iw44"# or #".iw4"#.
    Suffix #".djvu"# emphasizes the fact that IW44 files is seamlessly
    recognized by the current DjVu decoder.  Suffix #".iw4"# however was
    required by older versions of the DjVu plugin.  If this argument is
    omitted, a filename is generated by replacing the suffix of #pnmfile# 
    or #jpegfile# with suffix #".djvu"#.
	
	 {\bf Quality Specification}---
	 Files produced by the DjVu IW44 Wavelet Encoder are IFF files composed of
	 an arbitrary number of chunks (see \Ref{showiff} and \Ref{IWImage.h})
	 containing successive refinements of the encoded image.  Each chunk is
	 composed of several slices.  A typical file contains a total of 100 slices
	 split between three or four chunks.  Various options provide quality
	 targets (#-decibel#), slicing targets (#-slice#) or file size targets
	 (#-bpp# or #-size#) for each of these refinements.  Chunks are generated
	 until meeting either the decibel target, the file size target, or the
	 slicing target for each chunk.
	 \begin{description}
	 \item[-bpp n,..,n] 
	 Selects a increasing sequence of bitrates for building 
	 progressive IW44 file (in bits per pixel).
	 \item[-size n,..,n] 
	 Selects a increasing sequence of minimal sizes for building 
	 progressive IW44 file (in bytes).
	 \item[-decibel n,..,n]
	 Selects an increasing sequence of luminance error expressed as decibels
	 ranging from 16 (very low quality) to 48 (very high quality).  This
	 criterion should not be used when recoding an image which was already
	 compressed with a lossy compression scheme (such as Wavelets or JPEG)
	 because successive losses of quality accumulate.    
	 \item[-slice n+...+n]
	 Selects an increasing sequence of data slices expressed as integers 
	 ranging from 1 to 140. 
	 \end{description}
	 These options take a target specification list expressed either as a comma
	 separated list of increasing numbers or as a list of numbers separated by
	 character #'+'#.  Both commands below for instance are equivalent:
	 \begin{verbatim}
	 c44 -bpp 0.1,0.2,0.5 inputfile.ppm  outputfile.djvu
	 c44 -bpp 0.1+0.1+0.3 inputfile.ppm  outputfile.djvu
	 \end{verbatim}
	 Both these commands generate a file whose first chunk encodes the image
	 with 0.1 bits per pixel, whose second chunk refines the image to 0.2 bits
	 per pixel and whose third chunk refines the image to 0.5 bits per pixel.
	 In other words, the second chunk provides an extra 0.1 bits per pixel and
	 the third chunk provides an extra 0.3 bits per pixels.
	 
	  When no quality specification is provided, program #c44# usually generates
	  a file composed of three progressive refinement chunks whose quality
	  should be acceptable.  The best results however are achieved by precisely
	  tuning the image quality.  As a rule of thumb, #c44# generates an
	  acceptable quality when you specify a size equal to 50% to 75% of the size
	  of a comparable JPEG image.
	  
	   {\bf Color Processing Specification} ---
	   Five options control the encoding of the chrominance information of color
	   images.  These options are of course meaningless for processing a gray
	   level image.
	   \begin{description}
	   \item[-crcbnormal]
	   Selects normal chrominance encoding (default).  Chrominance information is
	   encoded at the same resolution as the luminance. 
	   \item[-crcbhalf]
	   Selects half resolution chrominance encoding.  Chrominance information is
	   encoded at half the luminance resolution. 
	   \item[-crcbdelay n]
	   This option can be used with #-crcbnormal# and #-crcbhalf# for specifying
	   an encoding delay which reduces the bitrate associated with the chrominance. The
	   default chrominance encoding delay is 10 slices.
	   \item[-crcbfull]
	   Selects the highest possible quality for encoding the chrominance information. This
	   is equivalent to specifying #-crcbnormal# and #-crcbdelay 0#.
	   \item[-crcbnone]
	   Disables encoding of the chrominance.  Only the luminance information will
	   be encoded. The resulting image will show in shades of gray.
	   \end{description}
	   
		{\bf Advanced Options} ---
		Program #c44# also recognizes the following options:
		\begin{description}
		\item[-dbfrac f]
		This option alters the meaning of the -decibel option.  The decibel target then
		addresses only the average error of the specified fraction of the most
		misrepresented 32x32 pixel blocks.
		\item[-mask pbmfile] 
		This option can be used when we know that certain pixels of a background
		image are going to be covered by foreground objects like text or drawings.
		File #pbmfile# must be a PBM file whose size matches the size of the input
		file.  Each black pixel in #pbmfile# means that the value of the corresponding
		pixel in the input file is irrelevant.  The DjVu IW44 Encoder will replace
		the masked pixels by a color value whose coding cost is minimal (see
		\URL{http://www.research.att.com/~leonb/DJVU/mask}).
		\end{description}
		
		 {\bf Photo DjVu options} ---
		 Photo DjVu images have the additional capability to store the resolution
		 and gamma correction information.
		 \begin{description}
		 \item[-dpi n]  Sets the resolution information for a Photo DjVu image.
		 \item[-gamma n] Sets the gamma correction information for a Photo DjVu image.
		 \end{description}
		 
		  {\bf Performance} ---
		  The main design objective for the DjVu wavelets consisted of allowing
		  progressive rendering and smooth scrolling of large images with limited
		  memory requirements.  Decoding functions process the compressed data and
		  update a memory efficient representation of the wavelet coefficients.
		  Imaging function then can quickly render an arbitrary segment of the image
		  using the available data.  Both process can be carried out in two threads
		  of execution.  This design plays an important role in the DjVu system.
		  
		   We have investigated various state-of-the-art wavelet compression schemes:
		   although these schemes may achieve slightly smaller file sizes, the
		   decoding functions did not even approach our requirements.  The IW44
		   wavelets reach these requirements today and may in the future implement
		   more modern refinements (such as trellis quantization, bitrate
		   allocation, etc.) if (and only if) these refinements can be implemented
		   within our constraints.
		   
			@memo
			DjVu IW44 wavelet encoder.
			@author
			L\'eon Bottou <leonb@research.att.com>
			@version
#$Id: c44.cpp,v 1.12 2003/11/07 22:08:22 leonb Exp $# */
//@{
//@}

#include "GString.h"
#include "GException.h"
#include "IW44Image.h"
#include "DjVuInfo.h"
#include "IFFByteStream.h"
#include "GOS.h"
#include "GBitmap.h"
#include "GPixmap.h"
#include "GURL.h"
#include "DjVuMessage.h"
#include "JPEGDecoder.h"

#include <locale.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "FreeImage.h"

// command line data

int flag_mask = 0;
int flag_bpp = 0;
int flag_size = 0;
int flag_percent = 0;
int flag_slice = 0;
int flag_decibel = 0;
int flag_crcbdelay = -1;
int flag_crcbmode = -1;  
double flag_dbfrac = -1;
int flag_dpi = -1;
double flag_gamma = -1;
int argc_bpp = 0;
int argc_size = 0;
int argc_slice = 0;
int argc_decibel = 0;
IW44Image::CRCBMode arg_crcbmode = IW44Image::CRCBnormal;

int flag_bsf = 1;

#define MAXCHUNKS 64
float argv_bpp[MAXCHUNKS];
int   argv_size[MAXCHUNKS];
int   argv_slice[MAXCHUNKS];
float argv_decibel[MAXCHUNKS];

struct C44Global 
{
	// Globals that need static initialization
	// are grouped here to work around broken compilers.
	GURL pnmurl;
	GURL iw4url;
	GURL mskurl;
	IWEncoderParms parms[MAXCHUNKS];
}; 

static C44Global& g(void)
{ 
	static C44Global g; 
	return g;
}


// parse arguments

void 
usage()
{
	DjVuPrintErrorUTF8(
#ifdef DJVULIBRE_VERSION
		"C44 --- DjVuLibre-" DJVULIBRE_VERSION "\n"
#endif
		"Image compression utility using IW44 wavelets\n\n"
		"Usage: fi_c44 [options] graphical-file [djvufile]\n"
		"Options:\n"
		"    -slice n+...+n   -- select an increasing sequence of data slices\n"
		"                        expressed as integers ranging from 1 to 140.\n"
		"    -bpp n,..,n      -- select a increasing sequence of bitrates\n"
		"                        for building progressive file (in bits per pixel).\n"
		"    -size n,..,n     -- select an increasing sequence of minimal sizes\n"
		"                        for building progressive files (expressed in bytes).\n"
		"    -percent n,..,n  -- selects the percentage of original file size\n"
		"                        for building progressive file.\n"
		"    -decibel n,..,n  -- select an increasing sequence of luminance error\n"
		"                        expressed as decibels (ranging from 16 to 50).\n"
		"    -dbfrac frac     -- restrict decibel estimation to a fraction of\n"
		"                        the most misrepresented 32x32 blocks\n"
		"    -mask pbmfile    -- select bitmask specifying image zone to encode\n"
		"                        with minimal bitrate. (default none)\n"
		"    -dpi n           -- sets the image resolution\n"
		"    -gamma n         -- sets the image gamma correction\n"
		"    -crcbfull        -- encode chrominance with highest quality\n"
		"    -crcbnormal      -- encode chrominance with normal resolution (default)\n"
		"    -crcbhalf        -- encode chrominance with half resolution\n"
		"    -crcbnone        -- do not encode chrominance at all\n"
		"    -crcbdelay n     -- select chrominance coding delay (default 10)\n"
		"                        for -crcbnormal and -crcbhalf modes\n"
		"    -bsf n           -- select an int background subsampling factor (2..12)\n"
		"\n");
	exit(1);
}



void 
parse_bpp(const char *q)
{
	flag_bpp = 1;
	argc_bpp = 0;
	double lastx = 0;
	while (*q)
    {
		char *ptr; 
		double x = strtod(q, &ptr);
		if (ptr == q)
			G_THROW( ERR_MSG("c44.bitrate_not_number") );
		if (lastx>0 && q[-1]=='+')
			x += lastx;
		if (x<=0 || x>24 || x<lastx)
			G_THROW( ERR_MSG("c44.bitrate_out_of_range") );
		lastx = x;
		if (*ptr && *ptr!='+' && *ptr!=',')
			G_THROW( ERR_MSG("c44.bitrate_comma_expected") );
		q = (*ptr ? ptr+1 : ptr);
		argv_bpp[argc_bpp++] = (float)x;
		if (argc_bpp>MAXCHUNKS)
			G_THROW( ERR_MSG("c44.bitrate_too_many") );
    }
	if (argc_bpp < 1)
		G_THROW( ERR_MSG("c44.bitrate_no_chunks") );
}


void 
parse_size(const char *q)
{
	flag_size = 1;
	argc_size = 0;
	int lastx = 0;
	while (*q)
    {
		char *ptr; 
		int x = strtol(q, &ptr, 10);
		if (ptr == q)
			G_THROW( ERR_MSG("c44.size_not_number") );
		if (lastx>0 && q[-1]=='+')
			x += lastx;
		if (x<lastx)
			G_THROW( ERR_MSG("c44.size_out_of_range") );
		lastx = x;
		if (*ptr && *ptr!='+' && *ptr!=',')
			G_THROW( ERR_MSG("c44.size_comma_expected") );
		q = (*ptr ? ptr+1 : ptr);
		argv_size[argc_size++] = x;
		if (argc_size>=MAXCHUNKS)
			G_THROW( ERR_MSG("c44.size_too_many") );
    }
	if (argc_size < 1)
		G_THROW( ERR_MSG("c44.size_no_chunks") );
}

void 
parse_slice(const char *q)
{
	flag_slice = 1;
	argc_slice = 0;
	int lastx = 0;
	while (*q)
    {
		char *ptr; 
		int x = strtol(q, &ptr, 10);
		if (ptr == q)
			G_THROW( ERR_MSG("c44.slice_not_number") );
		if (lastx>0 && q[-1]=='+')
			x += lastx;
		if (x<1 || x>1000 || x<lastx)
			G_THROW( ERR_MSG("c44.slice_out_of_range") );
		lastx = x;
		if (*ptr && *ptr!='+' && *ptr!=',')
			G_THROW( ERR_MSG("c44.slice_comma_expected") );
		q = (*ptr ? ptr+1 : ptr);
		argv_slice[argc_slice++] = x;
		if (argc_slice>=MAXCHUNKS)
			G_THROW( ERR_MSG("c44.slice_too_many") );
    }
	if (argc_slice < 1)
		G_THROW( ERR_MSG("c44.slice_no_chunks") );
}


void 
parse_decibel(const char *q)
{
	flag_decibel = 1;
	argc_decibel = 0;
	double lastx = 0;
	while (*q)
    {
		char *ptr; 
		double x = strtod(q, &ptr);
		if (ptr == q)
			G_THROW( ERR_MSG("c44.decibel_not_number") );
		if (lastx>0 && q[-1]=='+')
			x += lastx;
		if (x<16 || x>50 || x<lastx)
			G_THROW( ERR_MSG("c44.decibel_out_of_range") );
		lastx = x;
		if (*ptr && *ptr!='+' && *ptr!=',')
			G_THROW( ERR_MSG("c44.decibel_comma_expected") );
		q = (*ptr ? ptr+1 : ptr);
		argv_decibel[argc_decibel++] = (float)x;
		if (argc_decibel>=MAXCHUNKS)
			G_THROW( ERR_MSG("c44.decibel_too_many") );
    }
	if (argc_decibel < 1)
		G_THROW( ERR_MSG("c44.decibel_no_chunks") );
}


int 
resolve_quality(int npix)
{
	// Convert ratio specification into size specification
	if (flag_bpp)
    {
		if (flag_size)
			G_THROW( ERR_MSG("c44.exclusive") );
		flag_size = flag_bpp;
		argc_size = argc_bpp;
		for (int i=0; i<argc_bpp; i++)
			argv_size[i] = (int)(npix*argv_bpp[i]/8.0+0.5);
    }
	// Compute number of chunks
	int nchunk = 0;
	if (flag_slice && nchunk<argc_slice)
		nchunk = argc_slice;
	if (flag_size && nchunk<argc_size)
		nchunk = argc_size;
	if (flag_decibel && nchunk<argc_decibel)
		nchunk = argc_decibel;
	// Force default values
	if (nchunk == 0)
    {
#ifdef DECIBELS_25_30_34
		nchunk = 3;
		flag_decibel = 1;
		argc_decibel = 3;
		argv_decibel[0]=25;
		argv_decibel[1]=30;
		argv_decibel[2]=34;
#else
		nchunk = 3;
		flag_slice = 1;
		argc_slice = 3;
		argv_slice[0]=74;
		argv_slice[1]=89;
		argv_slice[2]=99;
#endif
    }
	// Complete short specifications
	while (argc_size < nchunk)
		argv_size[argc_size++] = 0;
	while (argc_slice < nchunk)
		argv_slice[argc_slice++] = 0;
	while (argc_decibel < nchunk)
		argv_decibel[argc_decibel++] = 0.0;
	// Fill parm structure
	for(int i=0; i<nchunk; i++)
    {
		g().parms[i].bytes = argv_size[i];
		g().parms[i].slices = argv_slice[i];
		g().parms[i].decibels = argv_decibel[i];
    }
	// Return number of chunks
	return nchunk;
}


void
parse(GArray<GUTF8String> &argv)
{
	const int argc=argv.hbound()+1;
	for (int i=1; i<argc; i++)
    {
		if (argv[i][0] == '-')
        {
			if (argv[i] == "-percent")
            {
				if (++i >= argc)
					G_THROW( ERR_MSG("c44.no_bpp_arg") );
				if (flag_bpp || flag_size)
					G_THROW( ERR_MSG("c44.multiple_bitrate") );
				parse_size(argv[i]);
				flag_percent = 1;
            }
			else if (argv[i] == "-bpp")
            {
				if (++i >= argc)
					G_THROW( ERR_MSG("c44.no_bpp_arg") );
				if (flag_bpp || flag_size)
					G_THROW( ERR_MSG("c44.multiple_bitrate") );
				parse_bpp(argv[i]);
            }
			else if (argv[i] == "-size")
            {
				if (++i >= argc)
					G_THROW( ERR_MSG("c44.no_size_arg") );
				if (flag_bpp || flag_size)
					G_THROW( ERR_MSG("c44.multiple_size") );
				parse_size(argv[i]);
            }
			else if (argv[i] == "-decibel")
            {
				if (++i >= argc)
					G_THROW( ERR_MSG("c44.no_decibel_arg") );
				if (flag_decibel)
					G_THROW( ERR_MSG("c44.multiple_decibel") );
				parse_decibel(argv[i]);
            }
			else if (argv[i] == "-slice")
            {
				if (++i >= argc)
					G_THROW( ERR_MSG("c44.no_slice_arg") );
				if (flag_slice)
					G_THROW( ERR_MSG("c44.multiple_slice") );
				parse_slice(argv[i]);
            }
			else if (argv[i] == "-mask")
            {
				if (++i >= argc)
					G_THROW( ERR_MSG("c44.no_mask_arg") );
				if (! g().mskurl.is_empty())
					G_THROW( ERR_MSG("c44.multiple_mask") );
				g().mskurl = GURL::Filename::UTF8(argv[i]);
            }
			else if (argv[i] == "-dbfrac")
            {
				if (++i >= argc)
					G_THROW( ERR_MSG("c44.no_dbfrac_arg") );
				if (flag_dbfrac>0)
					G_THROW( ERR_MSG("c44.multiple_dbfrac") );
				char *ptr;
				flag_dbfrac = strtod(argv[i], &ptr);
				if (flag_dbfrac<=0 || flag_dbfrac>1 || *ptr)
					G_THROW( ERR_MSG("c44.illegal_dbfrac") );
            }
			else if (argv[i] == "-crcbnone")
            {
				if (flag_crcbmode>=0 || flag_crcbdelay>=0)
					G_THROW( ERR_MSG("c44.incompatable_chrominance") );
				flag_crcbdelay = flag_crcbmode = 0;
				arg_crcbmode = IW44Image::CRCBnone;
            }
			else if (argv[i] == "-crcbhalf")
            {
				if (flag_crcbmode>=0)
					G_THROW( ERR_MSG("c44.incompatable_chrominance") );
				flag_crcbmode = 0;
				arg_crcbmode = IW44Image::CRCBhalf;
            }
			else if (argv[i] == "-crcbnormal")
            {
				if (flag_crcbmode>=0)
					G_THROW( ERR_MSG("c44.incompatable_chrominance") );
				flag_crcbmode = 0;
				arg_crcbmode = IW44Image::CRCBnormal;
            }
			else if (argv[i] == "-crcbfull")
            {
				if (flag_crcbmode>=0 || flag_crcbdelay>=0)
					G_THROW( ERR_MSG("c44.incompatable_chrominance") );
				flag_crcbdelay = flag_crcbmode = 0;
				arg_crcbmode = IW44Image::CRCBfull;
            }
			else if (argv[i] == "-crcbdelay")
            {
				if (++i >= argc)
					G_THROW( ERR_MSG("c44.no_crcbdelay_arg") );
				if (flag_crcbdelay>=0)
					G_THROW( ERR_MSG("c44.incompatable_chrominance") );
				char *ptr; 
				flag_crcbdelay = strtol(argv[i], &ptr, 10);
				if (*ptr || flag_crcbdelay<0 || flag_crcbdelay>=100)
					G_THROW( ERR_MSG("c44.illegal_crcbdelay") );
            }
			else if (argv[i] == "-dpi")
            {
				if (++i >= argc)
					G_THROW( ERR_MSG("c44.no_dpi_arg") );
				if (flag_dpi>0)
					G_THROW( ERR_MSG("c44.duplicate_dpi") );
				char *ptr; 
				flag_dpi = strtol(argv[i], &ptr, 10);
				if (*ptr || flag_dpi<25 || flag_dpi>4800)
					G_THROW( ERR_MSG("c44.illegal_dpi") );
            }
			else if (argv[i] == "-gamma")
            {
				if (++i >= argc)
					G_THROW( ERR_MSG("c44.no_gamma_arg") );
				if (flag_gamma > 0)
					G_THROW( ERR_MSG("c44.duplicate_gamma") );
				char *ptr; 
				flag_gamma = strtod(argv[i], &ptr);
				if (*ptr || flag_gamma<=0.25 || flag_gamma>=5)
					G_THROW( ERR_MSG("c44.illegal_gamma") );
            }
			else if (argv[i] == "-bsf")
            {
				if (++i >= argc)
					G_THROW( ERR_MSG("fi_c44.no_bsf_arg") );
				if (flag_bsf > 1)
					G_THROW( ERR_MSG("fi_c44.duplicate_bsf") );
				char *ptr; 
				flag_bsf = strtol(argv[i], &ptr, 10);
				if (*ptr || flag_bsf<2 || flag_bsf>12)
					G_THROW( ERR_MSG("fi_c44.illegal_bsf") );
            }
			else
				usage();
        }
		else if (g().pnmurl.is_empty())
			g().pnmurl = GURL::Filename::UTF8(argv[i]);
		else if (g().iw4url.is_empty())
			g().iw4url = GURL::Filename::UTF8(argv[i]);
		else
			usage();
    }
	if (g().pnmurl.is_empty())
		usage();
	if (g().iw4url.is_empty())
    {
		GURL codebase=g().pnmurl.base();
		GUTF8String base = g().pnmurl.fname();
		int dot = base.rsearch('.');
		if (dot >= 1)
			base = base.substr(0,dot);
		const char *ext=".djvu";
		g().iw4url = GURL::UTF8(base+ext,codebase);
    }
}



GP<GBitmap>
getmask(int w, int h)
{
	GP<GBitmap> msk8;
	if (! g().mskurl.is_empty())
    {
		GP<ByteStream> mbs=ByteStream::create(g().mskurl,"rb");
		msk8 = GBitmap::create(*mbs);
		if (msk8->columns() != (unsigned int)w || 
			msk8->rows()    != (unsigned int)h  )
			G_THROW( ERR_MSG("c44.different_size") );
    }
	return msk8;
}


static void 
create_photo_djvu_file(IW44Image &iw, int w, int h,
                       IFFByteStream &iff, int nchunks, IWEncoderParms xparms[])
{
	// Prepare info chunk
	GP<DjVuInfo> ginfo=DjVuInfo::create();
	DjVuInfo &info=*ginfo;
	info.width = w;
	info.height = h;
	info.dpi = (flag_dpi>0 ? flag_dpi : 100);
	info.gamma = (flag_gamma>0 ? flag_gamma : 2.2);
	// Write djvu header and info chunk
	iff.put_chunk("FORM:DJVU", 1);
	iff.put_chunk("INFO");
	info.encode(*iff.get_bytestream());
	iff.close_chunk();
	// Write all chunks
	int flag = 1;
	for (int i=0; flag && i<nchunks; i++)
    {
		iff.put_chunk("BG44");
		flag = iw.encode_chunk(iff.get_bytestream(), xparms[i]);
		iff.close_chunk();
    }
	// Close djvu chunk
	iff.close_chunk();
}

////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
//
//  WinDjView functions section
//
//	Copyright (C) 2004-2009 Andrew Zhezherun
//

struct VersionInfo
{
	VersionInfo();
	
	bool bNT, b2kPlus, bXPPlus, bVistaPlus;
};
static VersionInfo theVersionInfo;

VersionInfo::VersionInfo()
: bNT(false), b2kPlus(false), bXPPlus(false), bVistaPlus(false)
{
	OSVERSIONINFO vi;
	vi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	if (::GetVersionEx(&vi))
	{
		bNT = (vi.dwPlatformId == VER_PLATFORM_WIN32_NT);
		if (bNT)
		{
			b2kPlus = (vi.dwMajorVersion >= 5);
			bXPPlus = (vi.dwMajorVersion > 5 || vi.dwMajorVersion == 5 && vi.dwMinorVersion >= 1);
			bVistaPlus = (vi.dwMajorVersion >= 6);
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////

bool IsWinXPOrLater()
{
	return theVersionInfo.bXPPlus;
}

int ReadUTF8Character(const char* str, int& nBytes)
{
	const unsigned char* s = reinterpret_cast<const unsigned char*>(str);
	unsigned char c = s[0];
	
	if (c < 0x80)
	{
		nBytes = 1;
		return c;
	}
	else if (c < 0xC2)
	{
		return -1;
	}
	else if (c < 0xE0)
	{
		if (s[1] == 0 || (s[1] ^ 0x80) >= 0x40)
			return -1;
		nBytes = 2;
		return ((s[0] & 0x1F) << 6) + (s[1] ^ 0x80);
	}
	else if (c < 0xF0)
	{
		if (s[1] == 0 || s[2] == 0 || (s[1] ^ 0x80) >= 0x40
			|| (s[2] ^ 0x80) >= 0x40 || (c == 0xE0 && s[1] < 0xA0))
			return -1;
		nBytes = 3;
		return ((s[0] & 0xF) << 12) + ((s[1] ^ 0x80) << 6) + (s[2] ^ 0x80);
	}
	else if (c < 0xF5)
	{
		if (s[1] == 0 || s[2] == 0 || s[3] == 0 || (s[1] ^ 0x80) >= 0x40
			|| (s[2] ^ 0x80) >= 0x40 || (s[3] ^ 0x80) >= 0x40
			|| (c == 0xF0 && s[1] < 0x90) || (c == 0xF4 && s[1] > 0x8F))
			return -1;
		nBytes = 4;
		return ((s[0] & 0x07) << 18) + ((s[1] ^ 0x80) << 12) + ((s[2] ^ 0x80) << 6)
			+ (s[3] ^ 0x80);
	}
	else
		return -1;
}

////////////////////////////////////////////////////////////////////////////////////////////

bool IsValidUTF8(const char* pszText)
{
	const char* s = pszText;
	while (*s != 0)
	{
		int nBytes = 0;
		if (ReadUTF8Character(s, nBytes) < 0)
			return false;
		s += nBytes;
	}
	
	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////
//
// Convert GUTF8String to ASCII string:
//
// returns the lenght of the converted string
//
int MakeString(const GUTF8String& text, char* strResult)
{
	if (text.length() == 0)
		return 0;
	
	// Prepare Unicode text
	LPWSTR pszUnicodeText = NULL;
	int nResult = 0;
	
	// Bookmarks or annotations may not be encoded in UTF-8
	// (when file was created by old software)
	// Treat input string as non-UTF8 if it is not well-formed
	DWORD dwFlags = 0;
	
	// Only Windows XP supports checking for invalid characters in UTF8 encoding
	// inside MultiByteToWideChar function
	if (IsWinXPOrLater())
	{
		dwFlags |= MB_ERR_INVALID_CHARS;
	}
	
	// Make our own check anyway
	if (!IsValidUTF8(text))
	{
		strcpy(strResult,text);
		
		printf("%s\n","Not valid UTF8");
		
		return 0;
	}
	
	int nSize = ::MultiByteToWideChar(CP_UTF8, dwFlags, (LPCSTR)text, -1, NULL, 0);	
	
	if (nSize != 0)
	{
		pszUnicodeText = new WCHAR[nSize];
		nResult = ::MultiByteToWideChar(CP_UTF8, dwFlags, (LPCSTR)text, -1, pszUnicodeText, nSize);
	}
	
	if (nResult != 0)
	{
#ifdef _UNICODE
		// something may be implemented here (if needed)
		
		//strResult = pszUnicodeText;
#else
		// Prepare ANSI text
		nSize = ::WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK | WC_DISCARDNS,
			pszUnicodeText, -1, NULL, 0, NULL, NULL);
		if (nSize == 0)
		{
			delete[] pszUnicodeText;
			
			return 0;
		}
		
		if (nSize > MAX_PATH)
		{
			delete[] pszUnicodeText;			
			
			printf("Filepath is longer than %d symbols\n", MAX_PATH);
			
			return 0;
		}
		
		::WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK | WC_DISCARDNS,
			pszUnicodeText, -1, strResult, nSize+1, NULL, NULL);
		
#endif
	}
	else
	{		
		strcpy(strResult,text);
	}
	
	delete[] pszUnicodeText;
	
	return strlen(strResult);
}

//  End of the WinDjView functions section
//
/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////

// FreeImage routines:

////////////////////////////////////////////////////////////////////////////////
/**
FreeImage error handler
@param fif Format / Plugin responsible for the error 
@param message Error message
*/
void FreeImageErrorHandler(FREE_IMAGE_FORMAT fif, const char *message) {
	printf("\n*** "); 
	printf("%s Format\n", FreeImage_GetFormatFromFIF(fif));
	printf(message);
	printf(" ***\n");
}

////////////////////////////////////////////////////////////////////////////////

/** Generic image loader

  @param lpszPathName Pointer to the full file name
  @param flag Optional load flag constant
  @return Returns the loaded dib if successful, returns NULL otherwise
*/

FIBITMAP* GenericLoader(const char* lpszPathName, int flag)
{	
	FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
	// check the file signature and deduce its format
	// (the second argument is currently not used by FreeImage)
	
	fif = FreeImage_GetFileType(lpszPathName, 0);
	
	FIBITMAP* dib;
	
	if(fif == FIF_UNKNOWN)
	{
		// no signature ?
		// try to guess the file format from the file extension
		fif = FreeImage_GetFIFFromFilename(lpszPathName);
	}
	
	// check that the plugin has reading capabilities ...
	if((fif != FIF_UNKNOWN) && FreeImage_FIFSupportsReading(fif))
	{
		// ok, let's load the file
		dib = FreeImage_Load(fif, lpszPathName, flag);
		
		// unless a bad file format, we are done !
		if (!dib)
		{
			printf("%s%s%s\n","File \"", lpszPathName, "\" not found.");
			return NULL;
		}
	}	
	
	return dib;
}

/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
//
// Main function:
//

int
main(int argc, char **argv)
{
	setlocale(LC_ALL,"");
	djvu_programname(argv[0]);
	GArray<GUTF8String> dargv(0,argc-1);
	for(int i=0;i<argc;++i)
		dargv[i]=GNativeString(argv[i]);
	G_TRY
    {
		// Parse arguments
		parse(dargv);
		
		FreeImage_SetOutputMessage(FreeImageErrorHandler);
		
		char strResult[MAX_PATH] = {0};
		
		MakeString(g().pnmurl.UTF8Filename(), strResult);
		
		// load a file into a universal FIBITMAP memory structure:
		
		FIBITMAP *dib;
		
		if (flag_bsf>1)
		{		
			FIBITMAP *dib_tmp = GenericLoader(strResult, 0);
			
			unsigned width  = FreeImage_GetWidth(dib_tmp);
			
			unsigned height = FreeImage_GetHeight(dib_tmp);
			
			dib = FreeImage_Rescale(dib_tmp, (int)((width+(flag_bsf-1))/flag_bsf), 
				(int)((height+(flag_bsf-1))/flag_bsf), FILTER_BICUBIC);
			
			FreeImage_Unload(dib_tmp);
		}	
		
		else
			
			dib = GenericLoader(LPCSTR(strResult), 0);
		
		if (FreeImage_GetImageType(dib) != FIT_BITMAP)
		{
			printf("Non-FIT_BITMAP images are not supported.\n");
			
			FreeImage_Unload(dib);
			
			return -2;
		}
		
		if (!(FreeImage_GetBPP(dib) == 24) &&
			
			!((FreeImage_GetBPP(dib) == 8) && (FreeImage_GetColorType(dib) != FIC_PALETTE)))
		{
			printf("Only 24-bit color or 8-bit greyscale images are supported.\n");
			
			FreeImage_Unload(dib);
			
			return -3;
		}
		
		// Load images		
		unsigned w = FreeImage_GetWidth(dib); // FIBITMAP width in pixels		
		unsigned h = FreeImage_GetHeight(dib); // FIBITMAP height in pixels		
		
		// Returns the width of the bitmap in bytes, rounded to the next 32-bit boundary,
		// also known as "pitch" or "stride" or "scan width".
		unsigned pitch = FreeImage_GetPitch(dib);
		
		BYTE* bits = (BYTE*)FreeImage_GetBits(dib); // the pointer to the 1 pixel
		// in the 1 pixel row (in FIBITMAP)		
		BYTE* lines;
		
		unsigned bpp = FreeImage_GetBPP(dib); // bitdepth, 24 - for color, 8 - for greyscale
		
		unsigned btpp = bpp/8;
		
		GP<IW44Image> iw;
		
		//Just create a GPixmap of size w*h 		
		GP<GPixmap> gipm = GPixmap::create(h,w);
		
		//and fill it with the RGB data:
		unsigned i,j;
		
		for (j=0;j<h;j++)
		{ 
			GPixel *row = (*gipm)[j]; 
			
			lines = bits + j * pitch;
			
			for (i=0; i<w; i++)
			{ 
				GPixel p; 
				
				if (btpp == 3) // color 24 bit
				{
					p.r = lines[FI_RGBA_RED]; // red byte
					p.g = lines[FI_RGBA_GREEN]; // green byte
					p.b = lines[FI_RGBA_BLUE]; // blue byte
					
					row[i] = p;
					
					lines += btpp;
					
				}
				else if (btpp == 1) // greyscale 8 bit
				{
					p.r = lines[i]; // red byte
					p.g = lines[i]; // green byte
					p.b = lines[i]; // blue byte
					
					row[i] = p;
				}
			}
		}
		
		GPixmap &ipm=*gipm;
		
		int bm_size = w*h*btpp;
		
		// Change percent specification into size specification
		if (flag_size && flag_percent)
			for (int i=0; i<argc_size; i++)
				argv_size[i] = (argv_size[i]*bm_size)/ 100;
			
			flag_percent = 0;
			
			iw = IW44Image::create_encode(ipm, getmask(w,h), arg_crcbmode);
			
			// Perform compression PM44 or BM44 as required
			if (iw)
			{
				g().iw4url.deletefile();
				GP<IFFByteStream> iff =
					IFFByteStream::create(ByteStream::create(g().iw4url,"wb"));
				if (flag_crcbdelay >= 0)
					iw->parm_crcbdelay(flag_crcbdelay);
				if (flag_dbfrac > 0)
					iw->parm_dbfrac((float)flag_dbfrac);
				int nchunk = resolve_quality(w*h);
				// Create djvu file
				create_photo_djvu_file(*iw, w, h, *iff, nchunk, g().parms);
			}
			
			FreeImage_Unload(dib);
    }
	G_CATCH(ex)
    {
		ex.perror();
		exit(1);
    }
	G_ENDCATCH;
	return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////