File Requester Demo
-------------------

  This demo shows how to initialise and make use of the FileRequester class.
  
  Once the basics are in place, such as the Woopsi singleton and a screen, using
  the FileRequester involves three steps.
  
  1.  Initialise the FAT system.
  
  Initialising the FAT system just involves calling "initFatDefault()".  You
  must be linking with the lfat9 lib in your makefile for this to work.  Note
  that adding libfat to your project will increase the size of the resultant ROM
  by about 80K.
  
  FAT initialisation can take a while, and the time taken varies between flash
  carts (some can take around 10 seconds), so it is best to add an
  "initialising FAT system" message to your GUI before you call the init
  function.  You should enable drawing on the Woopsi singleton and draw the GUI
  to ensure that this message is visible.
  
  2.  Create the FileRequester.
  
  The FileRequester is a gadget like any other.  The only difference is that it
  is a "compound" gadget, or a gadget that includes multiple sub-gadgets.  It
  derives from the AmigaWindow class, so the requester is basically just a
  window with a listbox and a couple of buttons inside it.  The initial path
  for the FileRequester must be sent as a constructor parameter.  libfat uses
  UNIX-style paths, so the root path is "/".
  
  3.  Receive events from the FileRequester.
  
  Just like any other gadget, the FileRequester raises events when something of
  interest happens to it.  The crucial event in this case is the
  "EVENT_VALUE_CHANGE" event, which is raised when the user double-clicks an
  option or clicks the "OK" button.  The event gets raised to the requester's
  event handler.
  
  In the example, the FileReqDemo class inherits from the EventHandler class.
  This means that it can listen for events from any gadget that has the
  FileReqDemo as an event handler.  In the "handleEvent()" function we use some
  boilerplate code to determine which gadget fired the event and which event has
  been fired, and once we're sure that we're dealing with the FileRequester and
  the "EVENT_VALUE_CHANGE" event, we can process it.  We extract the name
  of the selected file and print it to the screen via the same textbox we used

Building the Example
--------------------

  To build this example, open a shell, navigate to this directory, and type
  "make".  Note that this will also compile the Woopsi source; this will be
  necessary until Woopsi ships as a library.