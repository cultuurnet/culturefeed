(function($) {

  // Adding support for ajaxload event to core's ajax.inc framework.
  $(document).ready(function() {
    if (Drupal.settings.ajax) {
      for (var base in Drupal.settings.ajax) {
        if (Drupal.settings.ajax[base].event == 'ajaxload') {
          $(Drupal.settings.ajax[base].selector).trigger('ajaxload');
        }
      }
    }
  });

})(jQuery);
