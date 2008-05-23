DimmedScreen
------------

  This gadget works like a standard Screen, in that it fills one of the DS'
  displays and can contain other gadgets.  However, unlike the standard screen
  it does not draw anything to the display.  Instead, it reduces the brightness
  of any gadgets behind it by half.  Thus, any child gadgets or gadgets in front
  of a DimmedScreen appear normal, whilst any gadgets behind it are shown at
  half-brightness.
  
  The DimmedScreen is intended to duplicate the faded effect employed in Ubuntu
  when it asks for a password before allowing a privileged operation.
  
  The dimming effect is achieved using two rather hacky tricks.  Firstly, the
  gadget's draw(Rect) *erases* the clipping rect before it does anything else.
  This makes the gadget transparent, since trying to draw it allows all of the
  gadgets behind it to show through.  After erasing the rect, the draw(Rect)
  method scans through the framebuffer, reads out the value if each pixel
  within the clipping region, halves the value of each of its RGB components,
  and writes the new value back.
  
  The DimmedScreen has a big limitation, which is why it is not part of the main
  library.  If the screen sits above an animated gadget, that gadget will appear
  to freeze.  This is because the screen only redraws itself when necessary,
  not every frame, so the animation does not get updated.  It would be possible
  to update the screen every frame, but this is highly inadvisable because of
  the time it takes to scan through the framebuffer and redraw the screen.