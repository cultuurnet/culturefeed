/**
 * @file
 * Js functionality for the search ui.
 */

Drupal.CulturefeedSearch = Drupal.CulturefeedSearch || {};

(function ($) {

  Drupal.behaviors.culturefeedSearchUi = {
   attach: function (context, settings) {

     if ($('#edit-sort').length > 0) {
       Drupal.CulturefeedSearch.bindSortDropdown();
     }
     if ($('#edit-date-range').length > 0) {
       Drupal.CulturefeedSearch.bindDatePicker();
     }

     $('input.auto-submit').click(Drupal.CulturefeedSearch.autoSubmit);

   }
  };

  /**
   * Bind the datepicker functionality.
   */
  Drupal.CulturefeedSearch.bindDatePicker = function() {
    $('#edit-date-range').daterangepicker({
      presetRanges: [],
      presets : {
        specificDate: Drupal.t('Specific date'),
        dateRange: Drupal.t('Period from/to')
      },
      rangeStartTitle: Drupal.t('Start date'),
      rangeEndTitle: Drupal.t('End date'),
      doneButtonText: Drupal.t('OK'),
      dateFormat: 'd/m/yy',
      earliestDate : Date.parse(),
      constrainDates : true,
      appendTo: '.form-item-date-range'
    });
  };

  /**
   * Click listener on autosubmit fields.
   */
  Drupal.CulturefeedSearch.autoSubmit = function() {
    $(this).parents('form').submit();
  };

})(jQuery);
