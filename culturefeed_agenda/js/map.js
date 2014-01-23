
(function($) {

  Drupal.CultureFeed = Drupal.CultureFeed || {};
  Drupal.CultureFeed.Agenda = {};

  Drupal.CultureFeed.Agenda.getDirections = function() {
    // set the start and end locations
    var saddr = document.getElementById("saddr").value;
    window.open('http://maps.google.be/maps?saddr=' + saddr + '&daddr=' + escape(Drupal.settings.culturefeed_map.info.location.street + ', ' + Drupal.settings.culturefeed_map.info.location.zip + ' ' + Drupal.settings.culturefeed_map.info.location.city) + '&hl=en&z=15', '_blank');
  }

  Drupal.CultureFeed.Agenda.initializeMap = function() {

    var d = Drupal.settings.culturefeed_map.info;
    var myOptions = {
      zoom: 15,
      mapTypeId: google.maps.MapTypeId.ROADMAP,
      center: new google.maps.LatLng(51.020577, 3.959729),
    };

    if (d.coordinates != null) {
      myOptions.center = new google.maps.LatLng((parseFloat(d.coordinates.lat) + 0.003), d.coordinates.lng);
    }

    // Generate map
    var map = new google.maps.Map(document.getElementById('map_canvas'), myOptions);

    if (d.location != null) {

      // Create infowindow content
      var contentString = '<div>';
      if (d.title != null) {
        contentString += d.title + '<br />';
      }

      contentString += d.location.street + ' ';
      contentString += d.location.zip + ' ' + d.location.city + '<br />';

      contentString += '<br />' + Drupal.t('Directions') + ':' +
              '<form action="javascript:Drupal.CultureFeed.Agenda.getDirections()"><input type="text" size="20" maxlength="40" name="saddr" id="saddr" value="';
      if (d.to_address) {
        contentString += d.to_address;
      }
      contentString += '" /> <input value="' + Drupal.t('Search') + '" type="submit"></form>' +
              '</div>';

      // Create infowindow element
      var infowindow = new google.maps.InfoWindow({
        content: contentString,
        maxWidth: 250
      });

      // Create marker
      var myLatLng = new google.maps.LatLng(d.coordinates.lat, d.coordinates.lng);
      var Marker = new google.maps.Marker({
        position: myLatLng,
        map: map
      });

      // Map infowindow on Marker at click
      google.maps.event.addListener(Marker, 'click', function() {
        infowindow.open(map, Marker);
      });

      // Trigger click for default open infowindow
      google.maps.event.trigger(Marker, 'click');

    }

  }

  // Place map
  $(document).ready(function() {
    google.maps.event.addDomListener(window, 'load', Drupal.CultureFeed.Agenda.initializeMap);
  })

})(jQuery);