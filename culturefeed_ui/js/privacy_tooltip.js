(function ($) {

  Drupal.behaviors.culturefeed_ui_privacy_tooltip = {

    attach: function (context, settings) {

      $(document).tooltip({
        content: Drupal.settings.culturefeed_ui_privacy_settings_anonymous_tooltip,
        items: "label[for='edit-setting-anonymous']"
      });

    }

  };

})(jQuery);
