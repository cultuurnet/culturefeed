(function($) {
  Drupal.behaviors.culturefeed_mailing = {
    attach: function (context, settings) {
    	var userLocationJson = $.cookie('Drupal.visitor.uitid.userLocation');
      if(typeof userLocationJson !== 'undefined') {
      	var userLocation = jQuery.parseJSON(userLocationJson);
      	if ($('.zip-field').length && userLocation.hasOwnProperty('postal')) {
      		$('.zip-field').val(userLocation.postal);
      	}
      }
    }
  };

})(jQuery);