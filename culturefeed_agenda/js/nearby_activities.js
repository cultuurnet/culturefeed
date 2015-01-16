(function($) {


  Drupal.CultureFeed = Drupal.CultureFeed || {};
  Drupal.CultureFeed.Agenda = Drupal.CultureFeed.Agenda || {};
  Drupal.CultureFeed.Agenda.dummy = Drupal.CultureFeed.Agenda.updateLocationForm || {};

  Drupal.behaviors.nearbyActivites = {
    attach: function(context, settings) {

      var $submit = $('#culturefeed-agenda-nearby-activities-filter-form').find('#edit-submit');
      if ($submit.hasClass('ajax-processed')) {
        $submit.once('nearby-activities', function() {

          var cookie = $.cookie('Drupal.visitor.uitid.userLocation');
          cookie = null;
          if (cookie) {
            cookieParsed = JSON.parse(cookie);
            city = cookieParsed.city;
            postal = cookieParsed.postal;

            // Set default form input value.
            $('#culturefeed-agenda-nearby-activities-filter-form').find('#edit-location').val(postal + ' ' + city);
            // Set location in the block title.
            $('#nearby-activities-title-location').html('// ' + postal + ' ' + city);

            $(this).trigger('mousedown');

          }
          else {
            Drupal.CultureFeed.geolocate(Drupal.CultureFeed.Agenda.updateLocationForm);
          }

        });
      }
    }

  }

  // On page load.
  $(document).ready(function() {

    // Show/hide the location form.
    $('#change-location-link').click(function(e) {
      e.preventDefault();
      $('#nearby-activities-filter-form-wrapper').toggleClass('hidden');
    });

  });

  /*
   * Updte the location form and the bock-title with the new location.
   */
  Drupal.CultureFeed.Agenda.updateLocationForm = function(response) {
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

    // Set the autocomplete input value.
    var $form = $('#culturefeed-agenda-nearby-activities-filter-form');
    $form.find('input[name="location"]').val(postal + ' ' + city);

    // Update the block title with the location.
    if ($('#nearby-activities-title-location').length) {
      $('#nearby-activities-title-location').html('// ' + postal + ' ' + city + ' ');
    }

    // Submit the form.
    $form.find('#edit-submit').trigger('mousedown');

    // Update the link to all events for ths location.

  }

})(jQuery);
