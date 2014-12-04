This module contains a drupal form element to enter or modify culturefeed dates.

The dates can be of different formats:
* timestamps (date and optional time)
* period (start date and end date including opening hours)
* permanent (including opening hours).

To provide sufficient form elements, this module provides form elements for
each individual type and these can be used separately.  The actual date control
form element uses the three above elements and a hybrid element.  The hybrid
element is an element that switches to one of the other elements depending on
initial input.

Developer info:
* Data handling happens in element validate functions.  The use the date_popup
  element.  Date fields taken from element aren't flattened.  To ensure
  flattened data for the form validation, the trigger element is checked and
  form state instead of element values are set.
* To ensure default values don't creep back in the elements after user
  operations, value callbacks reset the default value.  Default values are set
  in the process functions and only if the form is initialised.