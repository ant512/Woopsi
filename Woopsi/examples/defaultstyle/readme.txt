Default Style
-------------

  This demo illustrates the use of the "defaultGadgetStyle" global object to
  change the appearance of Woopsi.
  
  When creating a new object, developers have the option of passing in a pointer
  to a "GadgetStyle" object.  This contains definitions of all the colours used
  by a gadget, and the font used.  If no style object is specified, the gadget
  falls back to using values from the global defaultGadgetStyle object instead.
  
  Changing the properties of the defaultGadgetStyle object will affect all
  objects created subsequently.  Note that each gadget makes its own copy of the
  GadgetStyle object, so any objects made previous to the changed to the
  defaultGadgetStyle object will not affect any objects that have already been
  created.
  
  By altering the properties of the defaultGadgetStyle object in the startup()
  method, this demo changes the appearance of every object created.

Building the Example
--------------------

  To build this example, open a shell, navigate to this directory, and type
  "make".