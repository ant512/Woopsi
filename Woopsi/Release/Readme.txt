* Copyright (c) 2007, Antony Dzeryn
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
  
  The system itself is loosely based on the Commodore Amiga's "Intuition"
  windowing system.  The look-and-feel of the system intentionally resembles the
  old Amiga system.  There are several reasons for this:
  
  - It gives me a template to work from, reducing the amount of work I need to
    do;
  - Intuition was designed for low-power, low-resolution hardware;
  - It seemed like a good idea at the time.
  
  The windowing system is *not* intended to be a complete desktop environment.
  It is *not* going to be a DS version of GNOME, KDE or Windows Explorer.  It
  will simply provide a system for rapidly building window and gadget-based
  interfaces.
  

Features
--------

  - Multiple screen support;
  - A full set of user interface components:
    - Text buttons with text alignment functionality;
    - Image buttons;
    - Animated buttons;
    - Radio buttons and radio button groups;
    - Check boxes;
    - Single-line textboxes;
    - Multi-line text boxes;
    - Smooth-scrolling text viewers;
    - Screens;
    - Windows;
    - Gradient backgrounds;
    - SuperBitmap (for persistent drawing and displaying large bitmaps in small
      gadgets);
  - Skinnable screens and windows;
  - Draggable gadgets, including draggable windows and screens;
  - Borderless gadgets, including borderless windows;
  - Intelligent, optimised gadget drawing and erasing;
  - Event system;
  - Depth sorting;
  - Complete API for constructing new gadgets:
    - Extensible font system that supports monochrome and 16-bit font bitmaps
      by default;
    - Text manipulation, including wrapping functions and width calculation;
    - Animation class with support for variable framerates and standard/pingpong
      looping;
    - GraphicsPort class providing depth-sorted, hardware accelerated drawing
      functions:
      - Optimised horizontal/vertical lines;
      - Filled and unfilled rectangles;
      - Bitmap blitting;
      - XOR functions;
      - Pixel plotting;
  - Object-orientated design for easy integration into other C++ software;
  - Simple API for rapid GUI creation;
  - Simplistic "multitasking" - each window runs simultaneously;
  - Uses 16-bit display mode;
  - Uses the old AmigaOS 3.0 "Topaz" font;
  - Optional PALib support.


Requirements
------------

  To run the project you will need either a DS emulator or a real DS and a flash
  cart.
  
  To compile the project you need devKitPro and an IDE of some sort.  This
  archive contains a VC++ 2005 Express project.
  
  To use Woopsi with PALib:
  
   - Uncomment the line in "woopsi/woopsifuncs.h" that reads:

     //#define USING_PALIB
  
   - Replace the "Makefile" with "Makefile.palib".
  
  
Credits and Acknowlegements
---------------------------

 - Coding and design                              - Antony Dzeryn
 - Testing, technical advice, other contributions - Jeff Laing
 - PALib removal                                  - Steve
 - Simian Zombie logo                             - John Clay
 - Thanks to                                      - The devKitPro team
                                                  - The PALib team
                                                  - Nintendo
                                                  - R. J. Michal
   
Links
-----

 - http://ant.simianzombie.com                    - My main site
 - http://ant.simianzombie.com/blog               - DS development blog
 - http://www.sourceforge.net/projects/woopsi     - Woopsi SourceForge page
 - http://woopsi.sourceforge.net                  - Woopsi documentation


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