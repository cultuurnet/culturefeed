(function ($) {

  Drupal.Culturefeed = Drupal.Culturefeed || {};

  /**
   * Get the nearby actors for current user.
   */
  Drupal.Culturefeed.getNearbyActors = function() {

    $.ajax({
      url : Drupal.settings.culturefeed.actorsSuggestUrl,
      success : function(data, status) {
        if (data.success) {
          $('#actor-search-suggest').html(data.data);
        }
      }
    })

  }

  /**
   * Attach behaviors for the actor autocomplete.
   */
  $(document).ready(function() {

    var cookie = $.cookie('Drupal.visitor.uitid.userLocation');
    var cookieParsed = {};

    if (cookie) {
      cookieParsed = JSON.parse(cookie);
    }

    if (cookieParsed.postal && cookieParsed.city) {
      Drupal.Culturefeed.getNearbyActors();
    }
    else {
      Drupal.CultureFeed.geolocate(Drupal.Culturefeed.getNearbyActors);
    }
  });

})(jQuery);
