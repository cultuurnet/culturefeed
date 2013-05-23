(function ($) {
  
  Drupal.CultureFeed = Drupal.CultureFeed || {};
  Drupal.CultureFeed.Agenda = {};
  
  $(document).ready(function() {
    Drupal.CultureFeed.geolocate('#current-location', 'input[name="coordinates"]');
  });
  
})(jQuery);
