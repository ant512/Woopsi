Hello World
-----------

  This is a "Hello World" example program.  It illustrates the basics of getting
  a Woopsi application up and running.
    
  The "HelloWorld" class is the real meat of the application.  It inherits from
  the Woopsi class and overrides two essential methods - "startup()" and
  "shutdown()".  The "startup()" method contains all initialisation code, such
  as the UI creation, extra hardware initialisation, data loading, etc.
  
  The "shutdown()" method should be used to tidy up once the main application
  exits.  Any non-gadget classes that have been instantiated or other allocated
  memory should be deleted, and any other post-main loop code that you want to
  run.  The only requirement for overrides of this function is that it should
  call the base "Woopsi::shutdown()" method as the last instruction, although
  this requirement can be ignored if you are working with an SDL build of Woopsi
  and want to manually shut down the SDL subsystems.

Building the Example
--------------------

  To build this example, open a shell, navigate to this directory, and type
  "make".