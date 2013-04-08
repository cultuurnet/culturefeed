/**
 * @file
 * Js functionality for the search ui.
 */

Drupal.CulturefeedSearch = Drupal.CulturefeedSearch || {};

(function ($) {
  
  //
  Drupal.behaviors.culturefeedSearchUi = {
   attach: function (context, settings) {
     Drupal.CulturefeedSearch.bindSortDropdown();
     Drupal.CulturefeedSearch.bindDatePicker();
   }
  };
  
  /**
   * Bind sort dropdown functionality.
   */
  Drupal.CulturefeedSearch.bindSortDropdown = function() {
    
    var $form = $('#culturefeed-search-ui-search-sortorder-form');
    if ($form.length == 0) {
      return;
    }
    
    // Submit the form when sort selection changes.
    $form.find('#edit-sort').change(function(e) {
      $form.submit();
    });
    
    // Hide the submit button.
    $form.find('input:submit').hide();
    
  }
  
  /**
   * Bind the datepicker functionality.
   */
  Drupal.CulturefeedSearch.bindDatePicker = function() {
    $('#edit-date-range').daterangepicker();
  }

})(jQuery);
