(function ($) {
  Drupal.behaviors.geolocation = {
    attach: function(context) {
      if (navigator.geolocation) {
        //if the browser supports geolocations get along and execute
        navigator.geolocation.getCurrentPosition( function (position) {
          //build up the querie for the google api
          var geocoder = new google.maps.Geocoder();
          var lat = parseFloat(position.coords.latitude);
          var lng = parseFloat(position.coords.longitude);
          var latlng = new google.maps.LatLng(lat, lng);

          // execute the request and send the respons to addpostalcode function
          geocoder.geocode({'latLng': latlng}, addPostalCode);
        });
      }

      function addPostalCode(response) {
        if (response){
          
          var postalcode = '';
          var city = '';
          place = response[0];
          
          for (i = 0; i < place.address_components.length; i++) {
            
            if (place.address_components[i].types[0] == 'postal_code') {
              postalcode = place.address_components[i].long_name;    
            }
            else if(place.address_components[i].types[0] == 'locality') {
              city = place.address_components[i].long_name; 
            }
            
          }
          
          //inject the html in the label
          $('.geo-location').html(postalcode + ' ' +city );
          //inject the postalcode in the hidden field
          $('input[name$="geo_zipcode"]').val(postalcode);
          //switch the selected radio button to the geocode field
          $('#edit-search-geo').attr('checked', 'checked');
          //make everything visible
          $('#culturefeed-ui-recommendations-filter-form').addClass('geo-available');
        }
      }
    }
  }
})(jQuery);
