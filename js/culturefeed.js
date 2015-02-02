(function ($) {

// Close popup.
Drupal.behaviors.culturefeedPopupConnect = {
  attach: function (context, settings) {
    // This will open in a popup in case there's class called 'culturefeedpopuplogin'.
    // By default, the class on those links is culturefeedconnect, so change this
    // in your theme in case you want to have a popup functionality for your site.
    $('a.culturefeedpopuplogin', context).click(function() {
      var href = $(this).attr('href');

      var hasPopupQuery = href.search(/closepopup\=true/i) > -1;
      var hasQuestionMark = href.search(/\?/) > -1;

      if (!hasPopupQuery) {
        if (hasQuestionMark) {
          href += '&closepopup=true';
        }
        else {
          href += '?closepopup=true';
        }
      }

      window.open(href, 'UiTiD', 'location=1,status=1,scrollbars=1,resizable=1,width=810,height=635');

      return false;
    });
  }
};

if (Drupal.ajax) {

  /**
   * Command to provide a bridge between culturefeed_bootstrap and framework.
   */
  Drupal.ajax.prototype.commands.culturefeedModal = function (ajax, response, status) {

    if (Drupal.ajax.prototype.commands.bootstrapModal != undefined) {
      Drupal.ajax.prototype.commands.bootstrapModal(ajax, response, status);
    }
    else {
      Drupal.ajax.prototype.commands.insert(ajax, response, status);
    }

  };

  /**
   * Command to reload current page.
   */
  Drupal.ajax.prototype.commands.culturefeedGoto = function (ajax, response, status) {
    window.location.href = response.url;
  }

}

})(jQuery);