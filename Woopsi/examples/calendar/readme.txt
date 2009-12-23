Calendar Test
-------------

  This example shows how the Calendar class should be used.
  
  The Calendar class allows the developer to quickly create a date picker.  It
  has forward/backward buttons for switching between months, a textual display
  of the currently-visible month and year, and a grid of buttons from which the
  user can choose a day.  The initially selected date is chosen by the
  programmer and specified in the constructor.
  
  When the user clicks a date, the calendar raises an EVENT_ACTION event.
  Checking for date changes is simply a matter of listening for events from the
  calendar, and when one is received, checking that it originates from the
  calendar and that it is an EVENT_ACTION event.
  
  
Building the Example
--------------------

  To build this example, open a shell, navigate to this directory, and type
  "make".