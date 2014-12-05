This module contains a drupal form element to enter or modify culturefeed dates.

The dates can be of different formats:
* timestamps (date and optional time)
* period (start date and end date including opening hours)
* opening times (including culturefeed weekscheme).

To provide flexible form elements, this module provides form elements for
each individual type and these can be used separately.  The actual date control
form element uses the three above elements and a hybrid element.  The hybrid
element is an element that switches to one of the other elements depending on
initial input.

Developers:
* The form is very depending on user interaction.  Therefore all manipulation of
  the form is done on user input level.  This because the manipulation is on
  elements (adding and/or removing elements) and not data.  This means working
  with element value callback and form state input.  This also ensures that the
  final date in form state values stays clean.
* The actual manipulation is done in element validate functions, a common core
  practice.