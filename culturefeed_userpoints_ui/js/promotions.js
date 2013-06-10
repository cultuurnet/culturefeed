(function ($) {
  
  Drupal.CultureFeed = Drupal.CultureFeed || {};
  Drupal.CultureFeed.Userpoints = {};

  Drupal.behaviors.culturefeedActivitiesOverview = {
    attach: function (context, settings) {

      if ($('.culturefeed-userpoints-ui-promotions-list-item.item-selected').length > 0) {
		$('#culturefeed-userpoints-exchange-form-wrapper').show();
	  }
	  else {
		$('#culturefeed-userpoints-exchange-form-wrapper').hide();
	  }
    	
    }
  };
  
})(jQuery);
