This module contains a drupal form element to enter or modify culturefeed dates.

The dates can be of different formats:
* timestamps (date and optional time)
* period (start date and end date including opening hours)
* opening times (including culturefeed weekscheme).

To provide sufficient form elements, this module provides form elements for
each individual type and these can be used separately.  The actual date control
form element uses the three above elements and a hybrid element.  The hybrid
element is an element that switches to one of the other elements depending on
initial input.
