(function($) {

  Drupal.CultureFeed = Drupal.CultureFeed || {};
  Drupal.CultureFeed.Calendar = Drupal.CultureFeed.Calendar || {};

  $(document).ready(function() {
    Drupal.CultureFeed.Calendar.copyToClipboard();
  });

  Drupal.CultureFeed.Calendar.copyToClipboard = function() {

    var clipboard = new Clipboard('.js-copy-to-clipboard');
    clipboard.on('success', function(e) {
        $(".status").show().delay(2000).fadeOut('slow');
    });

  }


})(jQuery);
