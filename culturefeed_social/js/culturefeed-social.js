/**
 * @file
 * Js functionality for the activity overviews.
 */

Drupal.CulturefeedSocial = Drupal.CulturefeedSocial || {};

(function ($) {

  Drupal.behaviors.culturefeedSocial = {
    attach: function (context, settings) {
      Drupal.CulturefeedSocial.bindToggles('.comment-list-item');
    }
  };

  Drupal.CulturefeedSocial.bindToggles = function(item_list_selector) {
    
    // Look up toggle links.
    $items = $(item_list_selector);
    var $comment_forms = $items.find('.comment-subform');
    var $abuse_forms = $items.find('.comment-abuse-form');
    $comment_forms.hide();
    $abuse_forms.hide();
    
    $items.find('.comment-subform-toggle').bind('click', function() {
      return Drupal.CulturefeedSocial.toggle($(this), '.comment-subform', item_list_selector);
    });
    
    $items.find('.comment-abuse-toggle').bind('click', function() {
      return Drupal.CulturefeedSocial.toggle($(this), '.comment-abuse-form', item_list_selector);
    });     
    
  }
  
  Drupal.CulturefeedSocial.toggle = function(element, toggle_class, item_list_selector) {
    
    $wrapper = element.parents(item_list_selector).eq(0).find(toggle_class).eq(0);
    console.log(element.parents(item_list_selector).eq(0).find(toggle_class));
    $wrapper.toggle('slow');
	    
    return false;
  }
	
  
})(jQuery);