/**
 * @file
 * Contains culturefeed date control opening times javascript.
 */

(function ($) {

  Drupal.behaviors.culturefeed_date_control_time_select_complete_time = {

    attach: function (context, setings) {

      var time_complete = $(".date-hour > .time-select-complete-minutes");

      time_complete.each(function () {
        var id = $(this).attr('id');
        if (typeof Drupal.ajax[id] !== 'undefined') {
          var parentBeforeSerialize = Drupal.ajax[id].options.beforeSerialize;
          Drupal.ajax[id].options.beforeSerialize = function (element, options) {
            parentBeforeSerialize(element, options);
            culturefeed_date_control_time_select_complete_time($('#' + id));
          }
        }
      });

      time_complete.change(function () {
        if ($(this).val()) {
          culturefeed_date_control_time_select_complete_time($(this));
        }
      });

    }

  }

  function culturefeed_date_control_time_select_complete_time(element) {
    var minute = element.parents(".form-type-date-select").find(".date-minute > .time-select-complete-minutes");
    if (!minute.val()) {
      minute.val('00');
    }

  }

})(jQuery);
