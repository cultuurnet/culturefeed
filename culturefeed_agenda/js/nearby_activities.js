(function($) {


  Drupal.CultureFeed = Drupal.CultureFeed || {};
  Drupal.CultureFeed.Agenda = Drupal.CultureFeed.Agenda || {};
  Drupal.CultureFeed.Agenda.updateLocationForm = Drupal.CultureFeed.Agenda.updateLocationForm || {};

  Drupal.behaviors.nearbyActivities = {
    attach: function(context, settings) {

      var $submit = $('#culturefeed-agenda-nearby-activities-filter-form').find('#change-location-submit');
      if ($submit.hasClass('ajax-processed')) {
        $submit.once('nearby-activities', function() {

          var cookie = $.cookie('Drupal.visitor.uitid.userLocation');

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
    $('#nearby-activities-filter-form-wrapper').hide();
    // Show/hide the location form.
    $('#change-location-link').click(function(e) {
      e.preventDefault();
      $('#nearby-activities-filter-form-wrapper').toggle();
    });

  });


  /*
   * Update the location form and the bock-title with the new location.
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

    $location_string = postal + ' ' + city;

    // Set the input value.
    var $form = $('#culturefeed-agenda-nearby-activities-filter-form');
    $form.find('input[name="location"]').val($location_string);

    // Submit the form.
    $form.find('#change-location-submit').trigger('mousedown');


    // Update the block title with the location.
    if ($('#nearby-activities-title-location').length) {
      $('#nearby-activities-title-location').html('// ' + $location_string + ' ');
    }

    // Update the link to all events for ths location.
    var $everything_link = $("#all-activities-link");
    $everything_link.find('.location-string').text($location_string);
    $everything_link.attr('href', 'agenda/search/' + $location_string);
    $everything_link.removeClass("hidden");

    //hide the form;
    $('#nearby-activities-filter-form-wrapper').hide();
  }

})(jQuery);
