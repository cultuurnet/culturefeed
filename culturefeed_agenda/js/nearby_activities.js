(function($) {


  Drupal.CultureFeed = Drupal.CultureFeed || {};
  Drupal.CultureFeed.Agenda = Drupal.CultureFeed.Agenda || {};
  Drupal.CultureFeed.Agenda.setLatLon = Drupal.CultureFeed.Agenda.getUserLocation || {};

  // At page load, read the postal cookie and submit the form.
  Drupal.behaviors.nearbyActivites = {
    attach: function(context, settings) {

      var $submit = $('#culturefeed-agenda-nearby-activities-filter-form').find('#edit-submit');
      if ($submit.hasClass('ajax-processed')) {
        $submit.once('suggestions-loaded', function() {

          var cookie = $.cookie('culturefeed_agenda_nearby_activities');
          if (cookie) { console.log(cookie);
            $('#culturefeed-agenda-nearby-activities-filter-form').find('#edit-location').val(decodeURIComponent(cookie.replace(/\+/g, ' ')));
          }
          else {
            Drupal.CultureFeed.Agenda.getUserLocation();
          }

          $(this).trigger('mousedown');
        });
      }
    }
  }

  // On page load.
  $(document).ready(function() {

    // Check if the user's cookie is set with his location.

    // Show/hide the location form.
    $('#change-location-link').click(function(e) {
      e.preventDefault();
      $('#nearby-activities-filter-form-wrapper').toggleClass('hidden');
    });

    // Add autosubmit to the location autocomplete form.
    Drupal.jsAC.prototype.select = function (node) {
      this.input.value = $(node).data('autocompleteValue');

      if(jQuery(this.input).hasClass('auto-submit')) {

        // Hide the location form if it's visible.
        if (!$('#nearby-activities-filter-form-wrapper').hasClass('hidden')) {
          $('#nearby-activities-filter-form-wrapper').addClass('hidden');
        }

        var $submit = $('#culturefeed-agenda-nearby-activities-filter-form').find('#edit-submit');
        $submit.trigger('mousedown');
      }
    };

  });

  // Helper function to set the user's location in the cookie.
  Drupal.CultureFeed.Agenda.getUserLocation = function() {

    // Does browser support geolocation?
    if (!!navigator.geolocation) {

      navigator.geolocation.getCurrentPosition(function(position) {

        var latlng = new google.maps.LatLng(position.coords.latitude, position.coords.longitude);
        var geocoder = new google.maps.Geocoder();
        geocoder.geocode({'latLng': latlng}, function(results, status) {
console.log(results);
          if (status == google.maps.GeocoderStatus.OK) {
// todo : use results[0], whioch contains the city in the correct language
            if (results[1]) {
              var city_index = 0;
              var postal_index = 0;
              for (var j = 0; j < results.length; j++) {
                if (results[j].types[0] == 'locality') {
                  city_index = j;
                }
                if (results[j].types[0] == 'postal_code') {
                  postal_index = j;
                }
                if (city_index > 0 && postal_index > 0) {
                  break;
                }
              }

              largest_index = postal_index > city_index ? postal_index : city_index;

              for (var i = 0; i < results[largest_index].address_components.length; i++) {
                if (results[city_index].address_components[i].types[0] == "locality") {
                  city = results[city_index].address_components[i];
                  console.log(city.long_name);
                }
                if (results[postal_index].address_components[i].types[0] == "postal_code") {
                  postal = results[postal_index].address_components[i];
                  console.log(postal.long_name);
                }
              }
            } else {
              alert("No results found");
            }

          } else {
            alert("Geocoder failed due to: " + status);
          }
        })
      });

    }
  }

})(jQuery);