/**
 * @file
 * Js functionality for the activity overviews.
 */

Drupal.CulturefeedSocial = Drupal.CulturefeedSocial || {};

(function ($) {

  Drupal.behaviors.culturefeedSocial = {
    attach: function (context, settings) {
	   
	  // Look up toggle links.
      $items = $('.comment-list-item');
      var $comment_forms = $items.find('.comment-subform');
      var $abuse_forms = $items.find('.comment-abuse-form');
      $comment_forms.hide();
      $abuse_forms.hide();
      
      $items.find('.comment-subform-toggle').bind('click', function() {
        return Drupal.CulturefeedSocial.toggle($(this), '.comment-subform');
      });
      
      $items.find('.comment-abuse-toggle').bind('click', function() {
        return Drupal.CulturefeedSocial.toggle($(this), '.comment-abuse-form');
      });      
      
    }
  };
  
  Drupal.CulturefeedSocial.toggle = function(element, toggle_class) {
    
    $wrapper = element.parents('.comment-list-item').eq(0).find(toggle_class).eq(0);
    $wrapper.toggle('slow');
	    
    return false;
  }
	
  
})(jQuery);
