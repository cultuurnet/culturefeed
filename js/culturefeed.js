(function ($) {

// Close popup.
Drupal.behaviors.culturefeedPopupConnect = {
  attach: function (context, settings) {
    $('a.culturefeedconnect').click(function() {
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

      window.open(href, 'UiTiD', 'width=720,height=500');

      return false;
    });
  }
};

})(jQuery);