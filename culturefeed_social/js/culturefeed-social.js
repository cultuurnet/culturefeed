/**
 * @file
 * Js functionality for the activity overviews.
 */

Drupal.CulturefeedSocial = Drupal.CulturefeedSocial || {};

(function ($) {

  Drupal.behaviors.culturefeedSocial = {
    attach: function (context, settings) {
	   
	  // Look up toggle links.
      $('.recommendation-list-item .recommendation-subform').hide();
      $list = $('.recommendation-list-item .recommend-subform-toggle');
      $list.bind('click', Drupal.CulturefeedSocial.commentReactionClickListener);
    }
  };
  
  Drupal.CulturefeedSocial.commentReactionClickListener = function() {
    
	$self = $(this);
	$wrapper = $self.parents('.recommendation-list-item').find('.recommendation-subform');
	$wrapper.toggle('slow');
	    
	return false;
  }
	
  
})(jQuery);
