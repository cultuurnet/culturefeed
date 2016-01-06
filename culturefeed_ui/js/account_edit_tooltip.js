(function ($) {

  Drupal.behaviors.culturefeed_ui_privacy_tooltip = {

    attach: function (context, settings) {

      $(document).tooltip({
        content: Drupal.settings.culturefeed_ui_account_edit_email_description_tooltip,
        items: "#edit-mbox"
      });

    }

  };

})(jQuery);
