Hello World
-----------

  This is a "Hello World" example program using the object-orientated approach
  to Woopsi development.  In this example, the "main.cpp" file is kept to an
  absolute minimum.  It is used simply to instantiate the "HelloWorld" class and
  call that class' own "main()" function.
  
  The "HelloWorld" class is the real application.  It inherits from the Woopsi
  class and overrides two essential methods - "startup()" and "shutdown()".  The
  "startup()" method contains all initialisation code, such as the UI creation,
  extra hardware initialisation, data loading, etc.  It is essential that this
  method call the "enableDrawing()" and "draw()" functions at some point,
  preferably towards the end of the function once the UI has been created.  It
  should also call the base "Woopsi::startup()" method as the very first
  instruction.
  
  The "shutdown()" method should be used to tidy up once the main application
  exits.  Any non-gadget classes that have been instantiated or other allocated
  memory should be deleted, and any other post-main loop code that you want to
  run.  The only requirement for overrides of this function is that it should
  call the base "Woopsi::shutdown()" method as the last instruction, although
  this requirement can be ignored if you are working with an SDL build of Woopsi
  and want to manually shut down the SDL subsystems.