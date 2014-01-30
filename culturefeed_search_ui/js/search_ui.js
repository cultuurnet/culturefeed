/**
 * @file
 * Js functionality for the search ui.
 */

Drupal.CulturefeedSearch = Drupal.CulturefeedSearch || {};

(function ($) {
  
  Drupal.behaviors.culturefeedSearchUi = {
   attach: function (context, settings) {
     
     Drupal.CulturefeedSearch.bindSortDropdown();
     if ($('#edit-date-range').length > 0) {
       Drupal.CulturefeedSearch.bindDatePicker();       
     }
     
     $('input.auto-submit').click(Drupal.CulturefeedSearch.autoSubmit);
     
   }
  };
  
  /**
   * Bind sort dropdown functionality.
   */
  Drupal.CulturefeedSearch.bindSortDropdown = function() {
    
    var $form = $('.sortorder-form');
    if ($form.length == 0) {
      return;
    }
    
    // Submit the form when sort selection changes.
    $form.find('#edit-sort').change(function(e) {
      $form.submit();
    });
    
    // Hide the submit button.
    $form.find('button').hide();
    
  }
  
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
  }
  
  /**
   * Click listener on autosubmit fields.
   */
  Drupal.CulturefeedSearch.autoSubmit = function() {
    $(this).parents('form').submit();
  }

})(jQuery);
