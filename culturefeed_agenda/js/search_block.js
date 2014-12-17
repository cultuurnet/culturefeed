(function ($) {

  Drupal.CultureFeed = Drupal.CultureFeed || {};
  Drupal.CultureFeed.Agenda = Drupal.CultureFeed.Agenda || {};

  $(document).ready(function() {

    var $nearby_checkbox = $('#culturefeed-agenda-search-block-form').find('input[name="nearby"]');
    if (navigator.geolocation) {

      $nearby_checkbox.bind('change', function() {

        if (this.checked) {
          Drupal.CultureFeed.geolocate(Drupal.CultureFeed.Agenda.setLocationAutocomplete);
       }

     });

     if ($nearby_checkbox.is(':checked')) {
       $nearby_checkbox.trigger('change');
     }

    }
    else {
      $nearby_checkbox.parent().hide();
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
  }

})(jQuery);
