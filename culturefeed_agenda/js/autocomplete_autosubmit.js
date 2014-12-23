(function ($) {

  Drupal.jsAC.prototype.select = function (node) {

    if($(this.input).hasClass('auto-submit-field')) {
      window.location.href = $(node).data('autocompleteValue');
    }
  };

  $(document).ready(function() {
    if (navigator.geolocation) {

      //if the browser supports geolocations get along and execute
      navigator.geolocation.getCurrentPosition( function (position) {
        //build up the querie for the google api
        var geocoder = new google.maps.Geocoder();
        var lat = parseFloat(position.coords.latitude);
        var lng = parseFloat(position.coords.longitude);
        var latlng = new google.maps.LatLng(lat, lng);

        // execute the request and send the respons to addpostalcode function
        geocoder.geocode({'latLng': latlng}, function (response) {

          var location = {
            latitude : response[0].geometry.location.lat(),
            longitude : response[0].geometry.location.lng(),
          }

          $.cookie('uitid_location', JSON.stringify(location));
        });
      });

    }
  })

})(jQuery);
