(function ($) {
  Drupal.behaviors.culturefeed_ui = {
    attach: function(context, settings) {

      // Prepend a close button to each message.
      $('.messages:not(.cookie_close-processed):contains("' + Drupal.settings.culturefeed_ui.path + '")').each( function() {
        $(this).addClass('cookie_close-processed');
        $(this).prepend('<a href="#" class="cookie_close" title="' + Drupal.t('close') + '">' + Drupal.t('OK, I understand') + '</a>');
      });

      // When a close button is clicked hide this message.
      $(".messages a.cookie_close").click( function(event) {
        event.preventDefault();
        $.cookie('culturefeed_ui_cookies', 'hidden', { path : '/', expires: 1095 });
        $(this).parent().fadeOut("slow", function() {
          var messages_left = $("#messages .section").children().size();
          if (messages_left == 1) {
            $("#messages").remove();
          }
          else {
            $(this).remove();
          }
        });
      });

    }
  }
}(jQuery));