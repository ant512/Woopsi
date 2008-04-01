Bonus Code
----------

  This folder contains extra C++ classes that are not part of the main Woopsi
  library code.  To use them, drop them into your source folder and include
  them in your project.
  
Helpers
-------

  Classes in this folder abstract some of the complexities of Woopsi away to
  make applications easier to write.
  
Collections
-----------

  Woopsi intentionally makes no use of the STL collection classes (vector, etc),
  instead using its own DynamicArray class.  This is done to reduce the size of
  the Woopsi ROM.  The collections in this folder are a linked list and a
  hashmap.  Both have associated iterator classes.