(function ($) {

Drupal.behaviors.culturefeedPopupConnect = {
  attach: function (context, settings) {
    $('a.culturefeedconnect').click(function() {
      var href = $(this).attr('href');
      
      var hasPopupQuery = href.search(/popup\=true/i) > -1;
      var hasQuestionMark = href.search(/\?/) > -1;
      
      if (!hasPopupQuery) {
        if (hasQuestionMark) {
          href += '&popup=true';
        }
        else {
          href += '?popup=true';
        }
      }
      
      window.open(href, 'UiTid', 'width=720,height=500');
      
      return false;
    });
  }
};

})(jQuery);