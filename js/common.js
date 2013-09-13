(function ($) {
  
  Drupal.CultureFeed = Drupal.CultureFeed || {};
  
  /**
   * Geolocate current position.
   */
  Drupal.CultureFeed.geolocate = function(label_selector, coordinates_input_selector, city_input_selector) {
    
    if (navigator.geolocation) {
      //if the browser supports geolocations get along and execute
      navigator.geolocation.getCurrentPosition( function (position) {
        //build up the querie for the google api
        var geocoder = new google.maps.Geocoder();
        var lat = parseFloat(position.coords.latitude);
        var lng = parseFloat(position.coords.longitude);
        var latlng = new google.maps.LatLng(lat, lng);

        // execute the request and send the respons to addpostalcode function
        geocoder.geocode({'latLng': latlng}, function (response) { Drupal.CultureFeed.setLocation(response, label_selector, coordinates_input_selector, city_input_selector) });
      });
      
    }       
    
  }
  
  /**
   * Set the location properties based on a response on the given fields.
   * @param label_selector Selector for the visual label
   * @param coordinates_input_selector Selector for the hidden field with coordinates. 
   */
  Drupal.CultureFeed.setLocation = function(response, label_selector, coordinates_input_selector, city_input_selector) {
   
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
      
      // Inject the html in the label.
      if (label_selector != '') {
        $(label_selector).html(postalcode + ' '  + city );        
      }
      
      // Inject the postalcode in the hidden field.
      if (coordinates_input_selector != '') {
        $(coordinates_input_selector).val(place.geometry.location.lat() + ',' + place.geometry.location.lng());        
      }

      if (city_input_selector != '') {
        $(city_input_selector).val(postalcode + ' ' + city);        
      }      
      
    } 
    
    return false;
    
  }
  
})(jQuery);
