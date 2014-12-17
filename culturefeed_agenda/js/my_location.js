(function ($) {

  Drupal.CultureFeed = Drupal.CultureFeed || {};
  Drupal.CultureFeed.Agenda = Drupal.CultureFeed.Agenda || {};
  Drupal.CultureFeed.Agenda.setLatLon = Drupal.CultureFeed.Agenda.setLatLon || {};

  /**
   * Update the user's location in the cookie.
   */
  Drupal.CultureFeed.Agenda.setLatLon = function(where) {

    var geocoder = new google.maps.Geocoder();
    geocoder.geocode({'address': where}, function(response) {

      // Set the info also in a cookie.
      var place = response[0];
      var location = {
          latitude : place.geometry.location.lat(),
          longitude : place.geometry.location.lng(),
          city : 'city',
          postal : 'postal'
      }

      $.cookie('Drupal.visitor.uitid.userLocation', JSON.stringify(location), { path : '/' });

    });

  }

  Drupal.behaviors.userLatLon = {

   attach: function (context, settings) {
    $("form#culturefeed-agenda-set-user-location-form").submit(function() {

      var where = $("#edit-where").val();
      if (where.length) {
        Drupal.CultureFeed.Agenda.setLatLon(where);
      }
    });

   }

  };

  /**
   * Set the location cookie.
   */
  $(document).ready(function() {

    if ($.cookie('Drupal.visitor.uitid.userLocation') !== null) {

      cookie = JSON.parse($.cookie('Drupal.visitor.uitid.userLocation'));
      if (!(cookie.latitude || cookie.longitude) && cookie.city && cookie.postal) {
        Drupal.CultureFeed.Agenda.setLatLon(cookie.postal, cookie.city);
      }

    }
    else {
      Drupal.CultureFeed.geolocate();
    }

  });

})(jQuery);
