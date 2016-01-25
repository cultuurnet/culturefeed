(function($) {

  Drupal.behaviors.culturefeed_calendar_facebook_share = {
    attach: function(context, settings) {
      $('a.facebook-share', context).bind('click', function(e) {
        e.preventDefault();

        // Get default (legacy) sharer url
        var default_share_url = this.href;

        // Remove the "https://www.facebook.com/sharer/sharer.php?u="
        var dialog_share_url = default_share_url.replace('https://www.facebook.com/sharer/sharer.php?u=','');
        var dialog_share_url = decodeURIComponent(dialog_share_url);             

        FB.ui(
         {
          method: 'share',
          href: dialog_share_url,
        }, function(response){});
      });
    }
  };

})(jQuery);

  window.fbAsyncInit = function() {
    //console.log('timeout finished');
    FB.init({
      appId: Drupal.settings.culturefeed.fbAppId,
      xfbml: true,
      version: 'v2.1'
    });
  };

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
