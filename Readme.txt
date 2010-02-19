* Copyright (c) 2007-2010, Antony Dzeryn
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*     * Redistributions of source code must retain the above copyright
*       notice, this list of conditions and the following disclaimer.
*     * Redistributions in binary form must reproduce the above copyright
*       notice, this list of conditions and the following disclaimer in the
*       documentation and/or other materials provided with the distribution.
*     * Neither the names "Woopsi", "Simian Zombie" nor the
*       names of its contributors may be used to endorse or promote products
*       derived from this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY Antony Dzeryn ``AS IS'' AND ANY
* EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL Antony Dzeryn BE LIABLE FOR ANY
* DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
* ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

  
Woopsi
------

  Woopsi is a windowing system for the Nintendo DS, written in C++.  The aim of
  the project is to create a windowing system that other programmers can use for
  their applications, decreasing the tedious amount of GUI code that they have
  to write.
  

Features
--------

  - Multiple screen support;
  - A full set of user interface components:
    - Text buttons with text alignment functionality;
    - Image buttons;
    - Animated buttons;
    - Radio buttons and radio button groups;
    - Check boxes;
    - Labels;
    - Single-line textboxes with cursor support;
    - Multi-line textboxes with cursor support.
    - Screens;
    - Windows;
    - Gradient backgrounds;
    - SuperBitmap, for persistent drawing and displaying large bitmaps in small
      gadgets;
    - Scrolling panels, for containing graphics or gadgets within regions that
      are larger than the parent container;
    - Scroll bars;
    - Slider bars;
    - Context-sensitive menu;
    - List boxes;
    - Progress bars;
    - Requesters;
    - File requesters;
    - Cycle buttons;
    - Keyboard;
    - Calendar;
    - Timer, for VBL-driven effects;
  - Draggable gadgets, including draggable windows and screens;
  - Borderless gadgets, including borderless windows;
  - Built-in debug console;
  - All gadgets can become modal;
  - Intelligent, optimised gadget drawing and erasing;
  - Event system;
  - Automatic gadget garbage collection;
  - Complete API for constructing new gadgets:
    - Extensible font system that supports:
      - Fixed-width, bit-packed monochrome fonts;
      - Fixed-width, 16-bit colour bitmap fonts;
      - Compressed proportional fonts (monochrome and 16-bit);
      - FreeType library integration for TTF/OTF/etc fonts with antialiasing.
    - Text manipulation, including wrapping functions and width calculation;
    - Animation class with support for variable framerates and standard/pingpong
      looping;
    - Bitmap class for 16-bit bitmap image manipulation;
    - Graphics class providing clipped, DMA-accelerated drawing functions:
      - Pixel plotting;
      - Optimised horizontal/vertical lines;
      - Bresenham omni-directional lines;
      - Filled and unfilled rectangles;
      - Filled and unfilled circles;
      - Bitmap blitting;
      - Bitmap blitting with transparent colour key;
      - Bitmap blitting in greyscale;
      - XOR horizontal/vertical lines, pixels, rectangles and filled rectangles;
      - Flood fill;
      - Region scrolling;
      - Region copying;
      - Region dimming;
      - Region conversion to greyscale;
      - Text rendering.
    - Dynamic array container class and associated iterator class.
    - Date class.
  - Object-orientated design for easy integration into other C++ software;
  - Simple API for rapid GUI creation;
  - Non pre-emptive, FIFO multitasking - each gadget runs simultaneously;
  - Unicode strings encoded with UTF-8;
  - Two build options:
    - Standard libwoopsi.a library for use with DS projects;
    - SDL layer to facilitate native compilation on non-DS platforms and
      aid porting/debugging.
  - Suite of supporting Windows tools:
    - bmp2font, to convert BMPs to Woopsi fonts;
    - font2bmp, to convert Windows fonts to BMP images;
    - bmp2bmp, to convert BMPs to Woopsi bitmaps;
    - font2font, to convert Windows fonts to Woopsi fonts.


Requirements
------------

  To run the project you will need either a DS emulator or a real DS and a flash
  cart.
  
  To compile the project you need devKitPro.  This archive contains a VC++ 2008
  Express project, but the Woopsi library and all examples and tests are built
  from the command line.
   

SDL Compatibility
-----------------

  Woopsi has been designed to be portable.  Thanks to this, Woopsi comes
  equipped with an optional SDL layer that replaces libnds and allows the code
  to be compiled for any platform with an SDL port.  To create an SDL build of
  Woopsi:
  
  - Create a new SDL project in your dev environment of choice;
  - Copy the "libwoopsi" source folder into your SDL project;
  - Copy the contents of the "sdl" folder from this archive into your copy of
     the "libwoopsi/src" folder.

  To use the FreeType integration of Woopsi with SDL, you will need to download,
  compile and install FreeType for your chosen platform.  See the "Credits and
  Acknowledgements" section below for the URL.
  
  If you do not want to download FreeType, or cannot compile it for your
  platform (I confess that I have not tried this), delete any .h and .cpp files
  from the libwoopsi/src and libwoopsi/include folders that begin with
  the word "freetype".  This will prevent you from using TTF/OTF/etc fonts, but
  will allow you to develop the core of your GUI using SDL.
     
  Note that you should not use the Woopsi makefiles, as these are only relevant
  to DS builds of Woopsi.
  
  
Credits and Acknowledgements
---------------------------

  - Coding and design                                - Antony Dzeryn
  - Testing, technical advice, other contributions   - Jeff Laing
  - PALib independence                               - Steven Harrison
  - More testing and suggestions                     - John Buxton
  - Unicode work                                     - Olivier Binda
  - Simian Zombie logo                               - John Clay
  - Thanks to                                        - The devKitPro team
                                                     - The PALib team
                                                     - Nintendo
                                                     - R. J. Michal

  Woopsi uses the FreeType library for advanced font support.  See
  http://www.freetype.org for more information.


Links
-----

  - http://ant.simianzombie.com                      - Woopsi development blog
  - http://ant.simianzombie.com/forum                - Woopsi forum
  - http://www.sourceforge.net/projects/woopsi       - Woopsi SourceForge page


Email
-----

  Contact me at spam_mail250@yahoo.com.


Licencing and Contributions
---------------------------

  Any contributions of bugfixes or new features will be gratefully received.
  Any such contributions must follow the style guidelines laid out in Woopsi's
  documentation.  They must also be of sufficient general usefulness and quality
  to qualify for inclusion in the main code base.
  
  Please don't be offended if I reject or tidy up submissions that don't meet
  these criteria.
  
  All code will continue to be distributed under the BSD licence, so do not
  contribute code if you are unwilling to release it under that licence.