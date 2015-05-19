(function($) {

  window.fbAsyncInit = function() {
    FB.init({
      appId: Drupal.settings.culturefeed.fbAppId,
      xfbml: true,
      version: 'v2.1'
    });
  };

  Drupal.behaviors.culturefeed_calendar_facebook_share = {
    attach: function(context, settings) {
      $('a.facebook-share', context).bind('click', function(e) {
        e.preventDefault();
        FB.ui(
         {
          method: 'share',
          href: this.href,
        }, function(response){});
      });
    }
  };

})(jQuery);

(function(d, s, id) {
  var js, fjs = d.getElementsByTagName(s)[0];
  if (d.getElementById(id)) {
    return;
  }
  js = d.createElement(s);
  js.id = id;
  js.src = "//connect.facebook.net/nl_BE/sdk.js";
  fjs.parentNode.insertBefore(js, fjs);
}(document, 'script', 'facebook-jssdk'));
