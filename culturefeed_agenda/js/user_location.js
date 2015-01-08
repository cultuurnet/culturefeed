(function($) {


  Drupal.CultureFeed = Drupal.CultureFeed || {};
  Drupal.CultureFeed.Agenda = Drupal.CultureFeed.Agenda || {};
  Drupal.CultureFeed.Agenda.setLatLon = Drupal.CultureFeed.Agenda.setLatLon || {};


  /*
  Drupal.behaviors.userGeoLocation = {

    attach: function(context, settings) {

      // Set default value on input.
      var $submit = $('#culturefeed-agenda-nearby-activities-filter-form').find('#edit-submit');
      if ($submit.hasClass('ajax-processed')) {
        $submit.once('suggestions-loaded', function() {
          var cookie = $.cookie('Drupal.visitor.uitid.userLocation');
          if (cookie) {
            cookie = JSON.parse($.cookie('Drupal.visitor.uitid.userLocation'));
            $('#culturefeed-pages-page-suggestions-filter-form').find('#edit-city').val(cookie.city + ' ' + cookie.postal);
          }
          var where = $('#culturefeed-agenda-nearby-activities-filter-form #edit-where').val().trim();
            if (where.length) {
            postal = where.substring(0, 4);
            city = where.substring(4, where.length);
            Drupal.CultureFeed.Agenda.setLatLon(postal.trim(), city.trim());
          }

          $(this).trigger('mousedown');
        });
      }

    }

  }*/

  /**
   * Update the user's location in the cookie with lat and lon.
   */
  Drupal.CultureFeed.Agenda.setLatLon = function(postal, city) {
    var geocoder = new google.maps.Geocoder();
    address = postal + ' ' + city;
    geocoder.geocode({'address': address}, function(response) {
      // Set the info also in a cookie.
      var place = response[0];
      var location = {
          latitude : place.geometry.location.lat(),
          longitude : place.geometry.location.lng(),
          postal: postal,
          city: city
      }
      $.cookie('Drupal.visitor.uitid.userLocation', JSON.stringify(location), { path : '/' });
    });

  }

  /**
   * On page load.
   */
  $(document).ready(function() {

//    Drupal.CultureFeed.geolocate();
/*
    if ($.cookie('Drupal.visitor.uitid.userLocation') !== null) {

      // User submitted location only contains city and postal; add lat-lon.
      cookie = JSON.parse($.cookie('Drupal.visitor.uitid.userLocation'));
      if (!(cookie.latitude || cookie.longitude) && cookie.city && cookie.postal) {
        Drupal.CultureFeed.Agenda.setLatLon(cookie.postal, cookie.city);
      }
      console.log('ha');
    }
    else {
      // No user info stored yet; geolocate user's location and store in cookie.
      Drupal.CultureFeed.geolocate();
    }*/

    Drupal.jsAC.prototype.select = function (node) {
      this.input.value = $(node).data('autocompleteValue');
      if(jQuery(this.input).hasClass('auto_submit')){
        var $submit = $('#culturefeed-agenda-nearby-activities-filter-form').find('#edit-submit');
        $submit.trigger('mousedown');
      }

    };

  });

})(jQuery);
