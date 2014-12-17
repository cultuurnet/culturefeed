/**
 * @file
 * Contains culturefeed date control opening times javascript.
 */

(function ($) {

  Drupal.behaviors.culturefeed_date_control_time_select_complete_time = {
    attach: function (context, setings) {
      $(".date-hour > .time-select-complete-minutes").change(function () {
        if ($(this).val()) {
          var minute = $(this).parents(".form-type-date-select").find(".date-minute > .time-select-complete-minutes");
          if (!minute.val()) {
            minute.val('00');
          }
        }
      });
    }
  }

})(jQuery);

