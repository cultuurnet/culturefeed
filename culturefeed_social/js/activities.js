/**
 * @file
 * Js functionality for the activity overviews.
 */

(function ($) {

  Drupal.CulturefeedSearch = Drupal.CulturefeedSearch || {};
  Drupal.CulturefeedSearch.Activities = Drupal.CulturefeedSearch.Activities || {};
  Drupal.CulturefeedSearch.Activities.currentPage = 0;
  Drupal.CulturefeedSearch.Activities.activityList;
  Drupal.CulturefeedSearch.Activities.filterForm;
  Drupal.CulturefeedSearch.Activities.filterSelect;
  Drupal.CulturefeedSearch.Activities.throbber = $('<div class="ajax-progress ajax-progress-throbber"><div class="throbber">&nbsp;</div></div>');  
  
  Drupal.behaviors.culturefeedActivitiesOverview = {
   attach: function (context, settings) {
     Drupal.CulturefeedSearch.Activities.activityList = $('.activity-list').find('ul');
     $('.activity-list').find('.pager-link').bind('click', Drupal.CulturefeedSearch.Activities.pagerClickListener);
     Drupal.CulturefeedSearch.Activities.filterForm = $('#culturefeed-social-user-activity-filter-form');
     if (Drupal.CulturefeedSearch.Activities.filterForm.length) {
       Drupal.CulturefeedSearch.Activities.filterSelect = Drupal.CulturefeedSearch.Activities.filterForm.find('#edit-filter');
       Drupal.CulturefeedSearch.Activities.filterSelect.bind('change', Drupal.CulturefeedSearch.Activities.filterListener);  
     }
     
   }
  };

  /**
   * Listener on the pager link.
   */
  Drupal.CulturefeedSearch.Activities.pagerClickListener = function() {
    
    $(this).after(Drupal.CulturefeedSearch.Activities.throbber);
    
    var post_data = 'page=' + (Drupal.CulturefeedSearch.Activities.currentPage + 1);
    if (Drupal.CulturefeedSearch.Activities.filterSelect.val() != 'all') {
      post_data += ('&type=' + Drupal.CulturefeedSearch.Activities.filterSelect.val());
    }
    
    $.ajax({
      url : $(this).attr('href'),
      data : post_data,
      success : Drupal.CulturefeedSearch.Activities.applyPagerResult,
      'dataType' : 'json'
    });
    
    return false;
    
  }
  
  /**
   * Add the pager results to the current list.
   */
  Drupal.CulturefeedSearch.Activities.applyPagerResult = function(result) {
    
    Drupal.CulturefeedSearch.Activities.currentPage++;
    for (var i = 0; i < result.results.length; i++) {
      Drupal.CulturefeedSearch.Activities.activityList.append('<li>' + result.results[i] + '</li>');
    }
    
    Drupal.CulturefeedSearch.Activities.throbber.remove();
    
    if (!result.show_pager) {
      $('.activity-list').find('.pager-link').hide();
    }
    else {
      $('.activity-list').find('.pager-link').show();
    }
    
  }
  
  /**
   * Listener on the filter dropdown.
   */
  Drupal.CulturefeedSearch.Activities.filterListener = function() {
   
    Drupal.CulturefeedSearch.Activities.filterForm.after(Drupal.CulturefeedSearch.Activities.throbber);
    var post_data = 'new_filter=true';
    if ($(this).val() != 'all') {
      post_data += ('&type=' + $(this).val());
    }
    
    $.ajax({
      url : Drupal.CulturefeedSearch.Activities.filterForm.find('input[name=filter_url]').val(),
      data : post_data,
      success : Drupal.CulturefeedSearch.Activities.filterActivities,
      'dataType' : 'json'
    });
    
    return false;    
    
  }
  
  /**
   * Filter the activity results.
   */
  Drupal.CulturefeedSearch.Activities.filterActivities = function(result) {
    
    Drupal.CulturefeedSearch.Activities.activityList.empty();
    Drupal.CulturefeedSearch.Activities.applyPagerResult(result);
    Drupal.CulturefeedSearch.Activities.currentPage = 0;
    
  }
  
})(jQuery);
