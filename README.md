# DjVu Imager-Qt
This is a port of DjVu Imager 2.9 by Alexey Kryukov (monday2000) from MFC to Qt 5.x.
The main idea was to make it more cross-platform. Specifically - make a native version for Linux.

**DjVu Imager** - is a program which creates the grey or color illustrations in a DjVu-book. It is based on the combination of the background glueing and mask coloring methods.

The program accepts the background subscans from the specially prepared subscans pairs, converts each background subscan to a ready DjVu-pic and inserts them into a target DjVu-book (e.g. a multipage black-and-white DjVu file).

The subscans pairs are suggested to be created in the ScanKromsator or ScanTailor programs - although it can be done in any graphical editor.

**DjVu Imager** is licensed under "GPL 2 and later" and has the open sourcecodes.

DjVu Imager utilizes a free programmatic graphical library [FreeImage](http://freeimage.sourceforge.net/) (under GPL) and several console tools from [DjVuLibre](http://djvu.sourceforge.net/) project.

## The workflow outline:

ScanKromsator 5.92 or ScanTailor -> subscans pairs -> background subscan -> DjVu Imager

## Usage:

1. Prepare the subscans pairs in ScanKromsator 5.92 or ScanTailor.

2. Launch **DjVu Imager**.

3. Open the background subscans either by choosing **Open file(s)** or **Open folder**.

4. In the column # the program automatically forms the numbers of the loaded files (based on their names). Each such number signifies the DjVu-page number where this loaded graphical file is going to be glued on (as an illuatration). If necessary you can manually edit any such number (by double-clicking on it).

5. If you wish choose the encoding parameters: **BSF** or **Background quality**.

6. Using the switches **All files** / **Current file**, you can set the individual encoding parameters for any file in the list.

7. Press the **Convert** button to launch encoding of a group of the DjVu-pics.

8. Choose the target DjVu-book by pressing **Source**. This will automatically fill the output DjVu path (can be changed by **Dest**)

9. Press the **Insert** in DjVu button. The ready DjVu-pics will be automaticaly incorporated in the corresponding places of a copy of the target DjVu-book (Not that the target DjVu-book itself remains unchainged).

10. In the folder where a target DjVu-book pathname points will be created a copy of the target DjVu-book with the inserted DjVu-pics. It is a result of this program work.

## Details:

The DjVu Imager - is just a GUI controlling the console executables taken from DjVuLibre package:

Utilities in use:

* fi_c44 - the modified (with FreeImage support) console utility **с44** (for the DjVu-pics creation). In this port its crossplatform adaptation is used. The project and sources are [here](https://github.com/trufanov-nok/fi_c44-multiplatform)

- djvmcvt - the console utility for packing-unpacking the multypage DjVu-files to the separate pages.

- djvused - for getting a DjVu-book structure, pages number information and for changing dpi in djvu files for some cases.

- djvuextract - the console utility for disassembling single-page DjVu-files into the constituents (chunks).

- djvumake - the console utility for assembling single-page DjVu-files from the constituents (chunks). 

- djvm - the console utility which collects the ready DjVu-pics into a temporary multypage DjVu-file (to show it later when View button is pressed).


The Qt port uses up-to-date non-modified version of DjVuLibre tools, except for fi_c44.

For more details please check help files.

**Authors:** Alexey Kryukov <anagnost at yandex dot ru>
monday2000.

**GUI creation:** monday2000.

**Port to Qt:** Alexander Trufanov
