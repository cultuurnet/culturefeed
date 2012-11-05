(function ($) {
  
  Drupal.CultureFeedUI = Drupal.CultureFeedUI || {}
  Drupal.CultureFeedUI.Geo = {};
  
  Drupal.behaviors.geolocation = {
    attach: function(context) {
      Drupal.CultureFeedUI.Geo.getLocation();
      $('a.geo-refresh').bind('click', Drupal.CultureFeedUI.Geo.getLocation);
    }
  }
  
  /**
   * Get the location from current user.
   */
  Drupal.CultureFeedUI.Geo.getLocation = function(e) {
    
    if (e) {
      e.preventDefault();
    }
    
    if (navigator.geolocation) {
      //if the browser supports geolocations get along and execute
      navigator.geolocation.getCurrentPosition( function (position) {
        //build up the querie for the google api
        var geocoder = new google.maps.Geocoder();
        var lat = parseFloat(position.coords.latitude);
        var lng = parseFloat(position.coords.longitude);
        var latlng = new google.maps.LatLng(lat, lng);

        // execute the request and send the respons to addpostalcode function
        geocoder.geocode({'latLng': latlng}, Drupal.CultureFeedUI.Geo.setLocation);
      });
      
    }    
  }
  
  /**
   * Set the location properties on all fields.
   */
  Drupal.CultureFeedUI.Geo.setLocation = function(response) {
   
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

      // Switch the selected radio button to the geocode field, when this is the first search.
      if ($('input[name="previous_search_type"]').val() == '') {
        $('#edit-search-geo').attr('checked', 'checked');        
      }

      //make everything visible
      $('#culturefeed-ui-recommendations-filter-form').addClass('geo-available');
      
    } 
    
    return false;
    
  }
  
})(jQuery);
