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
     if ($('#specific-dates-range').length > 0) {
       Drupal.CulturefeedSearch.bindDatePicker();
     }

     $('input.auto-submit').click(Drupal.CulturefeedSearch.autoSubmit);

   }
  };

  /**
   * Bind the datepicker functionality.
   */
  Drupal.CulturefeedSearch.bindDatePicker = function() {
    var format = 'DD/MM/YYYY';
    var from = moment($('#edit-date-from').val(), format, true).isValid()
      ? $('#edit-date-from').val() : null;
    var to = moment($('#edit-date-to').val(), format, true).isValid()
      ? $('#edit-date-to').val() : null;

    $('#edit-date-from').hide();
    $('#edit-date-to').hide();
    $('#submit-specific-dates').hide();

    if (from && to) {
      $('#specific-dates-range span').html(from + ' - ' + to);
    }

    $('#specific-dates-range').daterangepicker({
      format: format,
      startDate: from ? from : moment(),
      endDate: to ? to : moment(),
      opens: 'right',
      showWeekNumbers: false,
      applyClass: "btn-primary",
      cancelClass: "btn-link",
      locale: {
        cancelLabel: Drupal.t('Cancel'),
        applyLabel: Drupal.t('Apply'),
        fromLabel: Drupal.t('Between'),
        toLabel: Drupal.t('And'),
        monthNames: [
          Drupal.t('January'),
          Drupal.t('February'),
          Drupal.t('March'),
          Drupal.t('April'),
          Drupal.t('May'),
          Drupal.t('June'),
          Drupal.t('July'),
          Drupal.t('August'),
          Drupal.t('September'),
          Drupal.t('October'),
          Drupal.t('November'),
          Drupal.t('December')
        ],
        daysOfWeek: [
          Drupal.t('Sun'),
          Drupal.t('Mon'),
          Drupal.t('Tue'),
          Drupal.t('Wed'),
          Drupal.t('Thu'),
          Drupal.t('Fri'),
          Drupal.t('Sat')
        ],
        firstDay: 1
      }
    }, function(start, end) {
      var from = start.format(format);
      var to = end.format(format);
      $('#specific-dates-range span').html(from + ' - ' + to);
      $('#edit-date-from').val(from);
      $('#edit-date-to').val(to);
      $('#specific-dates-range').closest('form').submit();
    }).click(function (e) {
      e.preventDefault();
    });
  };

  /**
   * Click listener on autosubmit fields.
   */
  Drupal.CulturefeedSearch.autoSubmit = function() {
    $(this).parents('form').submit();
  }

})(jQuery);
