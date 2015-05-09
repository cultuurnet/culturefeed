(function ($) {

  Drupal.CultureFeed = Drupal.CultureFeed || {};
  Drupal.CultureFeed.Agenda = Drupal.CultureFeed.Agenda || {};

  $(document).ready(function() {

    var $nearby_link = $('#culturefeed-agenda-search-block-form').find('.search-find-location');
    if (navigator.geolocation) {

      $nearby_link.bind('click', function() {
        $(this).unbind('click');
        $nearby_link.append(' <span id="current-location" class="loading-location throbber">Loading...</span>');
        Drupal.CultureFeed.geolocate(Drupal.CultureFeed.Agenda.setLocationAutocomplete);
     });

    }
    else {
      $nearby_link.hide();
    }

  });

  /**
   * Set the location autocomplete value.
   */
  Drupal.CultureFeed.Agenda.setLocationAutocomplete = function(response) {

    var place = response[0];
    var city = '';
    var postal = '';

    for (i = 0; i < place.address_components.length; i++) {

      if (place.address_components[i].types[0] == 'postal_code') {
        postal = place.address_components[i].long_name;
      }
      else if(place.address_components[i].types[0] == 'locality') {
        city = place.address_components[i].long_name;
      }

    }

    $('#culturefeed-agenda-search-block-form').find('input[name="where"]').val(postal + ' ' + city);
    
    // Add default radius when searching on current location
    if ($('#culturefeed-agenda-search-block-form').find('input[name="radius"]').val() == '') {
      $('#culturefeed-agenda-search-block-form').find('input[name="radius"]').val('10');
    }
    $('#current-location').remove();
  }

})(jQuery);
