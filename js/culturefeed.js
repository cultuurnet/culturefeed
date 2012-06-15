(function ($) {

// Close popup.
Drupal.behaviors.culturefeedPopupConnect = {
  attach: function (context, settings) {
    $('a.culturefeedconnect', context).click(function() {
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