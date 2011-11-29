(function ($) {
  
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
  
  Drupal.behaviors.cultureFeedEvaluationFader = {
    attach: function (context, settings) {
      $('.evaluated-negative, .evaluated-positive').parents('li').delay(1000).slideUp(600);
    }
  };

})(jQuery);

/**
 * Core override : overriding the form redirection error to not display the alert.
 */
Drupal.ajax.prototype.error = function (response, uri) {
  // alert(Drupal.ajaxError(response, uri)); // we don't want Drupal's default behavior to show alerts!
  // Remove the progress element.
  if (this.progress.element) {
    $(this.progress.element).remove();
  }
  if (this.progress.object) {
    this.progress.object.stopMonitoring();
  }
  // Undo hide.
  $(this.wrapper).show();
  // Re-enable the element.
  $(this.element).removeClass('progress-disabled').removeAttr('disabled');
  // Reattach behaviors, if they were detached in beforeSerialize().
  if (this.form) {
    var settings = response.settings || this.settings || Drupal.settings;
    Drupal.attachBehaviors(this.form, settings);
  }
};