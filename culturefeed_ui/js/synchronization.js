(function ($) {

  Drupal.behaviors.culturefeed_ui_synchronization = {

    attach: function (context, settings) {

      if ($.cookie('profile-synchronization') === null) {

        var container = $("<div></div>");
        var url = Drupal.settings.culturefeed_ui_synchronization.url;
        var title = Drupal.settings.culturefeed_ui_synchronization.title;

        $.ajax({
          url: url,
          type: 'GET',
          success: function(data) {

            var modal = {
              modal: true,
              title: title,
              open: function() {
                $.cookie('profile-synchronization', 1, { expires: 365, path: '/' });
              }
            };

            container.html(data).dialog(modal).dialog('open');

          }

        });

      }

    }

  };

})(jQuery);
