Progress Bar Test
-----------------

  This example shows how the ProgressBar class should be used.
  
  The ProgressBar class allows the developer to create a horizontal bar that
  can gradually fill up to indicate the progress of a particular process, such
  as loading a text file, etc.  The developer can specify the minimum and
  maximum values that the bar can represent, and the bar's present value.
  
  This example uses the WoopsiTimer gadget to increase the ProgressBar by a
  random amount (between 0 and 10) every 10 frames.
  
  
Building the Example
--------------------

  To build this example, open a shell, navigate to this directory, and type
  "make".  Note that this will also compile the Woopsi source; this will be
  necessary until Woopsi ships as a library.