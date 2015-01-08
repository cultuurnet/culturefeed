(function($) {

// At page load, read the postal cookie and submit the form.
  Drupal.behaviors.nearbyActivites = {
    attach: function(context, settings) {
      var $submit = $('#culturefeed-agenda-nearby-activities-filter-form').find('#edit-submit');
      if ($submit.hasClass('ajax-processed')) {
        $submit.once('nearby-activites-loaded', function() {
          var cookie = $.cookie('Drupal.visitor.uitid.userLocation');
          if (cookie) {
            cookie = JSON.parse($.cookie('Drupal.visitor.uitid.userLocation'));
            $('#culturefeed-agenda-nearby-activities-filter-form').find('#where').val(cookie.city + ' ' + cookie.postal);
          }

          $(this).trigger('mousedown');
        });
      }
    }
  }

  $(document).ready(function(){
    Drupal.jsAC.prototype.select = function (node) {
      this.input.value = $(node).data('autocompleteValue');
      if(jQuery(this.input).hasClass('auto_submit')){
        var $submit = $('#culturefeed-agenda-nearby-activites-filter-form').find('#edit-submit');
        $submit.trigger('mousedown');
      }
    };
  });

})(jQuery);
