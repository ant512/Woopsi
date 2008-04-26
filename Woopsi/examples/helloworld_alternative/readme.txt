Hello World
-----------

  This is a "Hello World" example using the procedural approach to Woopsi
  development.  In this approach, all of the Woopsi setup code needs to be run
  from the "main()" entry point function.  In order, this function must:
  
   - Instantiate the "woopsiApplication" singleton;
   - Call "woopsiApplication->startup()";
   - Set up the GUI;
   - Call "woopsiApplication->enableDrawing()";
   - Call "woopsiApplication->draw()";
   - Call "woopsiApplication->goModal()";
   - Call "woopsiApplication->shutdown()".
   
  Any other code that your application needs to run before Woopsi takes over
  should be called before "goModal()".  Any shutdown code should be called
  before "shutdown()".
  
  Note that, should you need more control over the main loop, the "goModal()"
  call can be excised.  You can replace it with this code instead:
  
  while (woopsiApplication->isModal()) {
      woopsiApplication->processOneVBL();
  }