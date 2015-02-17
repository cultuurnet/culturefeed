/**
 * @file
 * Contains culturefeed date control opening times javascript.
 */

(function ($) {

  Drupal.behaviors.culturefeed_date_control_opening_times_complete_time = {
    attach: function (context, setings) {
      $(".culturefeed-date-control-opening-times-complete input").blur(function () {
        var string = $(this).val();
        if (string) {
          var time = string.split(':');
          var has_separator = time.length - 1;
          if (has_separator === 0) {
            $(this).val(string + ':00');
          }
          if (time[1].length === 0) {
            $(this).val(string + '00');
          }
        }
      });
    }
  }

})(jQuery);

