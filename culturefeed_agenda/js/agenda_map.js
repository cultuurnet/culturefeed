(function($) {

    Drupal.CultureFeed = Drupal.CultureFeed || {};
    Drupal.CultureFeed.Agenda = {};

  /**
   * Initialize the omd map.
   */
  Drupal.CultureFeed.Agenda.initializeAgendaMap = function(myOptions) {

    var markers = myOptions.markers;
    if (markers.length == 0) {
      return;
    }

    var infoWindow = new google.maps.InfoWindow({});

    // Start map.
    Drupal.CultureFeed.Agenda.agendaMap = new google.maps.Map(document.getElementById("agenda-search-map"), myOptions);
    var bounds = new google.maps.LatLngBounds();

    // Add markers and click events.
    var map_markers = [];

    for (var i = 0; i < markers.length; i++) {
      var point = markers[i];
      var latLong = new google.maps.LatLng(point.latitude,point.longitude);
      bounds.extend(latLong);
      var marker = new google.maps.Marker({
        position: latLong,
        map: Drupal.OmdSearch.omdMap,
        title: point.title,
        html: point.html
      });

      if (point.html) {
        google.maps.event.addListener(marker, 'click', function() {
          infoWindow.setContent(this.html);
          infoWindow.open(Drupal.CultureFeed.Agenda.agendaMap, this);
        });
      }

      map_markers.push(marker);
    }

    // Zoom to current location or to fit current markers.
    if (Drupal.settings.omd.onDetail) {
      Drupal.CultureFeed.Agenda.agendaMap.setCenter(latLong);
      Drupal.CultureFeed.Agenda.agendaMap.setZoom(12);
    }
    else if (Drupal.settings.omd.doGeolocate) {
      Drupal.CultureFeed.Agenda.geolocate();
    }
    else {
      Drupal.CultureFeed.Agenda.agendaMap.setCenter(bounds.getCenter());
      Drupal.CultureFeed.Agenda.agendaMap.fitBounds(bounds);
      //remove one zoom level to ensure no marker is on the edge.
      Drupal.CultureFeed.Agenda.agendaMap.setZoom(Drupal.CultureFeed.Agenda.agendaMap.getZoom()-1);
    }

    // Cluster the search page.
    if (Drupal.settings.culturefeed_agenda_map.onSearch) {
      // Cluster options.
      var cluster_styles = [{
        url: Drupal.settings.culturefeed_agenda_map.cluster,
        height: 48,
        width: 48,
        anchor: [13, 0],
        textColor: '#fff',
        textSize: 12
      }];

      var markerCluster = new MarkerClusterer(Drupal.CultureFeed.Agenda.agendaMap, map_markers, {
        styles: cluster_styles
      });
    }

    // Check for auto-print.
    if (Drupal.settings.culturefeed_agenda_map.printonLoad) {
      google.maps.event.addListener(Drupal.CultureFeed.Agenda.agendaMap, 'tilesloaded', function() {

        // Start print after 5 seconds, this to make sure all markers are shown.
        setTimeout(function() {
          window.print();
        }, 1000);
      });
    }

  }

  /**
   * Geolocate current position.
   */
  Drupal.CultureFeed.Agenda.geolocate = function(response_callback) {

    if (navigator.geolocation) {
      //if the browser supports geolocations get along and execute
      navigator.geolocation.getCurrentPosition(function (position) {

        //build up the query for the google api
        var geocoder = new google.maps.Geocoder();
        var lat = parseFloat(position.coords.latitude);
        var lng = parseFloat(position.coords.longitude);
        var latlng = new google.maps.LatLng(lat, lng);

        // Execute the request and send the respons to addpostalcode function
        geocoder.geocode({'latLng': latlng}, function (response) {

          // Zoom in to current position.
          var center = new google.maps.LatLng(response[0].geometry.location.lat(), response[0].geometry.location.lng());
          Drupal.CultureFeed.Agenda.agendaMap.setCenter(center);
          Drupal.CultureFeed.Agenda.agendaMap.setZoom(10);

        });


      });

    }

  }

})(jQuery);