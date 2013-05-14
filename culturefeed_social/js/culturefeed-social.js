/**
 * @file
 * Js functionality for the activity overviews.
 */

Drupal.CulturefeedSocial = Drupal.CulturefeedSocial || {};

(function ($) {

  Drupal.behaviors.culturefeedSocial = {
    attach: function (context, settings) {
	   
	  // Look up toggle links.
      $items = $('.recommendation-list-item');
      var $comment_forms = $items.find('.recommendation-subform');
      var $abuse_forms = $items.find('.recommendation-abuse-form');
      $comment_forms.hide();
      $abuse_forms.hide();
      
      $items.find('.recommend-subform-toggle').bind('click', function() {
        return Drupal.CulturefeedSocial.toggle($(this), '.recommendation-subform');
      });
      
      $items.find('.recommend-abuse-toggle').bind('click', function() {
        return Drupal.CulturefeedSocial.toggle($(this), '.recommendation-abuse-form');
      });      
      
    }
  };
  
  Drupal.CulturefeedSocial.toggle = function(element, toggle_class) {
    
    $wrapper = element.parents('.recommendation-list-item').find(toggle_class);
    $wrapper.toggle('slow');
	    
    return false;
  }
	
  
})(jQuery);
