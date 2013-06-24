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

})(jQuery);