Dimmed Screen Test
------------------

  This example shows how the DimmedScreen class should be used.
  
  The DimmedScreen class allows the developer to simulate the fading effect used
  in Windows Explorer, GNOME and other windowing systems when a modal window,
  such as a logout, password entry or shutdown requester appears.
  
  Once the supporting gadgets (Woopsi, other screens, etc) are in place,
  creating a DimmedScreen with a modal child window is a simple process.
  
  1.  Adding the DimmedScreen.
  
  Adding the screen itself entails just two lines of code:
  
      DimmedScreen* myScreen = new DimmedScreen();
      woopsiApplication->addGadget(myScreen);
  
  2.  Adding a modal child window.
  
  Now that we have the screen, adding the child window is just as easy:
  
      Alert* myAlert = new Alert(10, 10, 100, 100, "Title", "Message");
      myScreen->addGadget(myAlert);
  
  This code adds an alert to the screen.  Other gadgets can be added in the
  same way.
  
  3.  Making the child modal.
  
  Once the child gadgets have been created and added to the window, ensure that
  Woopsi is drawn to the display and call "myAlert->goModal();" to start the
  child gadget's modal loop:
  
      enableDrawing();
      draw();
      myAlert->goModal();
  
  4.  Closing the screen.
  
  It is important that the DimmedScreen be closed when its child gadgets have
  all closed.  In the case of the single alert window, closing that window
  should also close the screen it belongs to or that screen will be in the way
  of the other gadgets.
  
  To do this, wire up an event handler that will listen for the alert's "close"
  event and close the DimmedScreen when this event is raised.
  
Building the Example
--------------------

  To build this example, open a shell, navigate to this directory, and type
  "make".