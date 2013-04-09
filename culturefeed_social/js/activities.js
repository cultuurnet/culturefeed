/**
 * @file
 * Js functionality for the activity overviews.
 */

Drupal.CulturefeedSearch = Drupal.CulturefeedSearch || {};
Drupal.CulturefeedSearch.Activities = Drupal.CulturefeedSearch.Activities || {};
Drupal.CulturefeedSearch.Activities.currentPage = 0;

(function ($) {
  
  Drupal.behaviors.culturefeedActivitiesOverview = {
   attach: function (context, settings) {
     
     alert('ok');
     $('.activity-list').find('.pager-link').bind('click', Drupal.CulturefeedSearch.Activities.pagerClickListener)
     
   }
  };

  /**
   * Listener on the pager link.
   */
  Drupal.CulturefeedSearch.Activities.pagerClickListener = function() {
    
    $.ajax({
      url : $(this).attr('href'),
      data : 'page=' . Drupal.CulturefeedSearch.Activities.currentPage + 1,,
      success : Drupal.CulturefeedSearch.Activities.addPagerResult,
      'dataType' : 'json'
    })
    
  }
  
  /**
   * Add the pager results to the current list.
   */
  Drupal.CulturefeedSearch.Activities.addPagerResult(data) {
    console.log(data);
  }
  
})(jQuery);
