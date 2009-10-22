Bitmap Loader
-------------

  This program demonstrates the use of the BitmapIO bonus class to load a BMP
  file and display it via a SuperBitmap gadget.  The simplest way to test this
  program is to copy the supplied "logo.bmp" to the root directory of your flash
  card and run the ROM via the DS itself.
  
  Using the BitmapIO class is very simple, and requires just two steps.
  
  1.  Initialise the FAT system.
  
  As with the FileRequester demo, the FAT library must be initialised before any
  I/O operations are attempted. This is achieved via a call to
  "fatInitDefault()".  You must be linking with the lfat9 lib in your makefile
  for this to work.  Note that adding libfat to your project will increase the
  size of the resultant ROM by about 80K.
  
  2.  Load the bitmap.
  
  Call BitmapIO::loadBMP() to load a bitmap from the flash card's file system.
  This will return a Bitmap object containing the width and height of the bitmap
  and its pixel data.  The bitmap can be displayed via a SuperBitmap, drawn
  directly to the screen, manipulated, etc.


Building the Example
--------------------

  To build this example, open a shell, navigate to this directory, and type
  "make".  Note that this will also compile the Woopsi source; this will be
  necessary until Woopsi ships as a library.