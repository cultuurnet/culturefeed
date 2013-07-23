(function ($) {
  
  Drupal.CultureFeed = Drupal.CultureFeed || {};
  Drupal.CultureFeed.Agenda = {};
  
  $(document).ready(function() {
    
    var $nearby_checkbox = $('#culturefeed-agenda-search-block-form').find('input[name="nearby"]');
    $nearby_checkbox.bind('change', function() {
      
      if (this.checked) {
        $('#culturefeed-agenda-search-block-form').find('#current-location').before(document.createTextNode(": "));
        Drupal.CultureFeed.geolocate('#current-location', 'input[id="geolocate-coordinates"]', 'input[id="geolocate-city"]');        
      }
      
    });
    
    if ($nearby_checkbox.is(':checked')) {
      $nearby_checkbox.trigger('change');
    }
    
  });
  
})(jQuery);
