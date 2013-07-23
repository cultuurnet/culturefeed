(function ($) {
  
  Drupal.CultureFeed = Drupal.CultureFeed || {};
  Drupal.CultureFeed.Agenda = {};
  
  $(document).ready(function() {
    
    var $nearby_checkbox = $('#culturefeed-agenda-search-block-form').find('input[name="nearby"]');
    if (navigator.geolocation) {
      
     $nearby_checkbox.bind('change', function() {
       
       if (this.checked) {
         Drupal.CultureFeed.geolocate('#current-location', 'input[id="geolocate-coordinates"]', 'input[id="geolocate-city"]');        
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
  
})(jQuery);
