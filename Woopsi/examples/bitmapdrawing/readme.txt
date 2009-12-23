Bitmap Drawing
--------------

  This example demonstrates the drawing capabilities available via the Graphics
  object.  Every bitmap can create a Graphics object in order for them to be
  drawn on.  Drawing is automatically clipped to the size of the bitmap.
  
  Unlike the "Gadget Drawing" example, drawing to bitmaps is persistent.
  Anything drawn to a bitmap will remain there until it is drawn over using
  further calls to the drawing functions of the Graphics object.

Building the Example
--------------------

  To build this example, open a shell, navigate to this directory, and type
  "make".