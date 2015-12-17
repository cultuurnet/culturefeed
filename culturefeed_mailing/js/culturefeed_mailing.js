(function($) {
  Drupal.behaviors.culturefeed_mailing = {
    attach: function (context, settings) {
      console.log($.cookie('Drupal_visitor_uitid_userLocation'));
    }
  };

})(jQuery);