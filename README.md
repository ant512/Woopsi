Woopsi
======

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
   - Colour picker.
 - Draggable gadgets, including draggable windows and screens;
 - Borderless gadgets, including borderless windows;
 - Built-in debug console;
 - All gadgets can become modal;
 - Intelligent, optimised gadget drawing and erasing;
 - Event system;
 - Automatic gadget garbage collection;
 - Complete API for constructing new gadgets:
   - Extensible font system that supports:
     - Compressed proportional and fixed-width fonts (monochrome and 16-bit);
     - FreeType library integration for TTF/OTF/etc fonts with antialiasing
       available in a separate distribution.
   - Text manipulation, including wrapping functions and width calculation;
   - Animation class with support for variable framerates and standard/pingpong
     looping;
   - Bitmap class for 16-bit bitmap image manipulation;
   - Graphics class providing clipped, DMA-accelerated drawing functions:
     - Pixels;
     - Lines;
     - Filled and unfilled rectangles;
     - Filled and unfilled ellipses;
     - XOR horizontal/vertical lines, pixels, rectangles and filled rectangles;
     - Bitmap blitting;
     - Bitmap blitting with transparent colour key;
     - Bitmap blitting in greyscale;
     - Flood fill;
     - Region scrolling;
     - Region copying;
     - Region dimming;
     - Region conversion to greyscale;
     - Text rendering.
   - Dynamic array container and iterator classes.
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
cart.  To compile the project you need [devKitPro][1].

  [1]: http://devkitpro.org


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
     
Note that you should not use the Woopsi makefiles, as these are only relevant
to DS builds of Woopsi.


Credits and Acknowledgements
---------------------------

 - Coding and design                               - Antony Dzeryn
 - Testing, technical advice, other contributions  - Jeff Laing
 - PALib independence                              - Steven Harrison
 - Testing and suggestions                         - John Buxton
                                                   - Chase
                                                   - Leonelhs
                                                   - Carpfish
 - Unicode work and libfreetype integration        - Olivier Binda (Lakedaemon)
 - Testing and gadget contributions                - Mark Adamson
 - Simian Zombie logo                              - John Clay
 - Thanks to                                       - The devKitPro team


Links
-----

 - <http://woopsi.org>                             - Main site
 - <http://simianzombie.com>                       - Development blog
 - <http://github.com/ant512/woopsi>               - GitHub page
 - <http://bitbucket.org/Lakedaemon/woopsi-xl>     - Woopsi with extra libs


Email
-----

Contact me at <ant@woopsi.org>.


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
