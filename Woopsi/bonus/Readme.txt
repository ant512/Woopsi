Bonus Code
----------

  This folder contains extra C++ classes that are not part of the main Woopsi
  library code.  To use them, drop them into your source folder or include the
  directory in your makefile.  Then, simply include them in your project.

Classes
-------

  Extra classes that have external dependencies and as such aren't included in
  the base library.

Collections
-----------

  Woopsi intentionally makes no use of the STL collection classes (vector, etc),
  instead making use of its own DynamicArray class.  This is done to reduce the
  size of Woopsi ROMs.  The collections in this folder are a linked list and a
  hashmap.  Both have associated iterator classes.
  
Gadgets
-------

  Extra gadgets that aren't officially part of Woopsi, or have external
  dependencies that will bulk up the size of the ROM if they were included by
  default.