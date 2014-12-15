(function ($) {

  Drupal.Culturefeed = Drupal.Culturefeed || {};

  /**
   * Get the nearby actors for current user.
   */
  Drupal.Culturefeed.getNearbyActors = function() {

    $.ajax({
      url : Drupal.settings.culturefeed.actorsSuggestUrl,
      success : function(data, status) {
        $('#actor-search-suggest').html(data);
      }
    })

  }

  /**
   * If an input has autosubmit as class. Redirect to the selected value.
   */
  Drupal.jsAC.prototype.select = function (node) {
    if ($(this.input).hasClass('auto-submit-field')) {
      window.location.href = $(node).data('autocompleteValue');
    }
  };


  /**
   * Attach behaviors for the actor autocomplete.
   */
  Drupal.behaviors.culturefeedActorAutocomplete = {
    attach: function (context, setings) {
      if ($.cookie('Drupal.visitor.uitid.userLocation') !== null) {
        Drupal.Culturefeed.getNearbyActors();
      };
    }
  }

  // Temp code, untill homepage story is started.
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

          $.cookie('Drupal.visitor.uitid.userLocation', JSON.stringify(location));
        });
      });

    }
  })

})(jQuery);
