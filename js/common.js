(function ($) {

  Drupal.CultureFeed = Drupal.CultureFeed || {};
  Drupal.CultureFeed.uiAutocomplete = Drupal.CultureFeed.uiAutocomplete || {};

  /**
   * Geolocate current position.
   */
  Drupal.CultureFeed.geolocate = function(response_callback) {

    if (navigator.geolocation) {
      //if the browser supports geolocations get along and execute
      navigator.geolocation.getCurrentPosition( function (position) {
        //build up the querie for the google api
        var geocoder = new google.maps.Geocoder();
        var lat = parseFloat(position.coords.latitude);
        var lng = parseFloat(position.coords.longitude);
        var latlng = new google.maps.LatLng(lat, lng);

        // Execute the request and send the respons to addpostalcode function
        geocoder.geocode({'latLng': latlng}, function(response, status) {

          // Check if response status is OK
          if (status == google.maps.GeocoderStatus.OK) {
  
            // Set the info also in a cookie.
            var place = response[0];
            for (i = 0; i < place.address_components.length; i++) {
  
              if (place.address_components[i].types[0] == 'postal_code') {
                current_postal = place.address_components[i].long_name;
              }
              else if(place.address_components[i].types[0] == 'locality') {
                current_city = place.address_components[i].long_name;
              }
  
            }
  
            var location = {
              latitude : place.geometry.location.lat(),
              longitude : place.geometry.location.lng(),
              city : current_city,
              postal : current_postal
            }
            $.cookie('Drupal.visitor.uitid.userLocation', JSON.stringify(location), { path : '/' });
  
            // Call the response callback function.
            if (response_callback) {
              response_callback(response);
            }
          } 

          // Show alert if response is not available
          else {
            alert("Location not found for the following reason: " + status);
          }

        });

      });

    }

  }

  /**
   * Get the latitude and longitude for a given place (postal + city).
   */
  Drupal.CultureFeed.getLatLonFromAddress = function(address, response_callback) {

    var geocoder = new google.maps.Geocoder();

    current_postal = address.substring(0, 4);
    current_city = address.substring(5);

    geocoder.geocode( { 'address': address}, function(response, status) {
      var place = response[0];
      if (status == google.maps.GeocoderStatus.OK) {

        var location = {
          latitude : response[0].geometry.location.lat(),
          longitude : response[0].geometry.location.lng(),
          city : current_city,
          postal : current_postal
        }

        $.cookie('Drupal.visitor.uitid.userLocation', JSON.stringify(location), { path : '/' });

        // Call the response callback function.
        if (response_callback) {
          response_callback(response);
        }
      }
    });
  }

  if (Drupal.jsAC) {

    /**
     * If an input has autosubmit as class. Redirect to the selected value.
     */
    Drupal.jsAC.prototype.select = function (node) {

      if (jQuery(this.input).hasClass('location-form-autocomplete-submit')) {
        // Set user input in cookie and location form.
        Drupal.CultureFeed.getLatLonFromAddress($(node).data('autocompleteValue'), Drupal.CultureFeed.Agenda.updateLocationForm);
      }

      if ($(this.input).hasClass('auto-submit-field')) {
        window.location.href = $(node).data('autocompleteValue');
      }
      else {
        this.input.value = $(node).data('autocompleteValue');
      }
    };

    /**
     * Hides the autocomplete suggestions.
     */
    Drupal.jsAC.prototype.hidePopup = function (keycode) {
      // Select item if the right key or mousebutton was pressed.
      if (this.selected && ((keycode && keycode != 46 && keycode != 8 && keycode != 27) || !keycode)) {

        if (jQuery(this.input).hasClass('location-form-autocomplete-submit')) {
          // Set user input in cookie and location form.
          Drupal.CultureFeed.getLatLonFromAddress($(this.selected).data('autocompleteValue'), Drupal.CultureFeed.Agenda.updateLocationForm);
        }

        if ($(this.input).hasClass('auto-submit-field')) {
          window.location.href = $(this.selected).data('autocompleteValue');
        }
        else {
          this.input.value = $(this.selected).data('autocompleteValue');
        }

      }
      // Hide popup.
      var popup = this.popup;
      if (popup) {
        this.popup = null;
        $(popup).fadeOut('fast', function () { $(popup).remove(); });
      }
      this.selected = false;
      $(this.ariaLive).empty();
    };

  }

  // Create a custom autocomplete widget that supports categorisation of data.
  if ($.ui && $.ui.autocomplete) {
    $.widget("custom.categorisedAutocomplete", $.ui.autocomplete, {
      _create: function() {
        this._super();
        this.widget().menu("option", "items", "> :not(.ui-autocomplete-category)");
      },
      _renderMenu: function(ul, items) {

        $(this.element).removeClass('throbbing');

        var that = this,
        currentCategory = "";
        $.each(items, function(index, item) {
          var li;
          if (!item.label) {
            if (item.category != currentCategory) {
              ul.append("<li class='ui-autocomplete-category " + item.type+ "'>" + item.category + "</li>");
              currentCategory = item.category;
            }
          } else {
            if (item.category != currentCategory) {
              ul.append("<li class='ui-autocomplete-category " + item.type+ "'>" + item.category + "</li>");
              currentCategory = item.category;
            }
            li = that._renderItemData(ul, item);
            if (item.category) {
              li.attr("aria-label", item.category + " : " + item.label);
            }
          }
        });
      },
      search: function() {
        $(this.element).addClass('throbbing');
        this._super();
      },
    });
  }

})(jQuery);
