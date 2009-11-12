Gadget Drawing
--------------

  This example demonstrates the drawing capabilities available through the
  GraphicsPort object.  Every gadget can create and work with GraphicsPort
  objects in order to draw within themselves.  The GraphicsPort automatically
  clips all drawing so that it does not exceed the boundaries of the gadget.
  
  Drawing done via the GraphicsPort is not persistent.  If the region that has
  been drawn over is refreshed, either by moving the gadget, moving another
  gadget over the area, etc, then the drawing is lost.

Building the Example
--------------------

  To build this example, open a shell, navigate to this directory, and type
  "make".  Note that this will also compile the Woopsi source; this will be
  necessary until Woopsi ships as a library.