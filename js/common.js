(function ($) {

  Drupal.CultureFeed = Drupal.CultureFeed || {};

  /**
   * Geolocate current position.
   */
  Drupal.CultureFeed.geolocate = function(response_callback) {

    if (navigator.geolocation) {
      //if the browser supports geolocations get along and execute
      navigator.geolocation.getCurrentPosition( function (position) {
        //build up the querie for the google api
        var geocoder = new google.maps.Geocoder();
        var lat = parseFloat(position.coords.latitude);
        var lng = parseFloat(position.coords.longitude);
        var latlng = new google.maps.LatLng(lat, lng);

        // execute the request and send the respons to addpostalcode function
        geocoder.geocode({'latLng': latlng}, response_callback);
      });

    }

  }

})(jQuery);
