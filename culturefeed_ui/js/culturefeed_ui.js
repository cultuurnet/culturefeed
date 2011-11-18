(function ($) {
  
  $(document).ready(function() {
    for (var base in Drupal.settings.ajax) {
      if (Drupal.settings.ajax[base].event == 'ajaxload') {
        $(Drupal.settings.ajax[base].selector).trigger('ajaxload');
      }
    }
  });

})(jQuery);