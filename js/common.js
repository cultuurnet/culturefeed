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
        geocoder.geocode({'latLng': latlng}, function(response) {

          // Set the info also in a cookie.
          var place = response[0];
          for (i = 0; i < place.address_components.length; i++) {

            if (place.address_components[i].types[0] == 'postal_code') {
              current_postal = place.address_components[i].long_name;
            }
            else if(place.address_components[i].types[0] == 'locality') {
              current_city = place.address_components[i].long_name;
            }

          }

          var location = {
            latitude : place.geometry.location.lat(),
            longitude : place.geometry.location.lng(),
            city : current_city,
            postal : current_postal
          }
          $.cookie('Drupal.visitor.uitid.userLocation', JSON.stringify(location), { path : '/' });

          // Call the response callback function.
          if (response_callback) {
            response_callback(response);
          }

        });


      });

    }

  }

})(jQuery);
