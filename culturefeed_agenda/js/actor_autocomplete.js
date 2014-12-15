(function ($) {

  Drupal.Culturefeed = Drupal.Culturefeed || {};

  /**
   * Get the nearby actors for current user.
   */
  Drupal.Culturefeed.getNearbyActors = function() {

    $.ajax({
      url : Drupal.settings.culturefeed.actorsSuggestUrl,
      success : function(data, status) {
        console.log(data);
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
    if ($.cookie('Drupal.visitor.uitid.userLocation') !== null) {
      Drupal.Culturefeed.getNearbyActors();
    }
    else {
      Drupal.CultureFeed.geolocate(Drupal.Culturefeed.getNearbyActors);
    }
  });

})(jQuery);
