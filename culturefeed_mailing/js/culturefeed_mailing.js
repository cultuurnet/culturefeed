/**
 * @file
 * Js behavior for the mailing forms.
 */

(function($) {

  /**
   * Attach the behavior.
   */
  Drupal.behaviors.culturefeed_mailing = {
    attach: function (context, settings) {

      // If a userlocation is known, set the default zip value.
      var userLocationJson = $.cookie('Drupal.visitor.uitid.userLocation');
      if (typeof userLocationJson !== 'undefined') {
        var userLocation = jQuery.parseJSON(userLocationJson);
        if (userLocation) {
          if ($('.zip-field').length && userLocation.hasOwnProperty('postal')) {
            $('.zip-field').val(userLocation.postal);
          }
        }
      }

    }
  };

})(jQuery);
