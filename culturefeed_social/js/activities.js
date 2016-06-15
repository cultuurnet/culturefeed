/**
 * @file
 * Js functionality for the activity overviews.
 */

(function ($) {

  Drupal.CulturefeedSearch = Drupal.CulturefeedSearch || {};
  Drupal.CulturefeedSearch.Activities = Drupal.CulturefeedSearch.Activities || {};
  Drupal.CulturefeedSearch.Activities.filterForm;
  Drupal.CulturefeedSearch.Activities.filterSelect;
  Drupal.CulturefeedSearch.Activities.throbber = $('<div class="ajax-progress ajax-progress-throbber"><div class="throbber">&nbsp;</div></div>');

  Drupal.behaviors.culturefeedActivitiesFilter = {
    attach: function (context, settings) {
      $('#edit-filter').change(function (e) {
        $('#activities-table').hide();
        $(this).after(Drupal.CulturefeedSearch.Activities.throbber);
        $(this).parents('form').submit();
      });
      $('#culturefeed-social-user-activity-filter-form #edit-submit').hide();
    }
  };

  Drupal.behaviors.culturefeedActivitiesOverview = {
   attach: function (context, settings) {
     $('.activity-list-wrapper').find('.pager-link').bind('click', Drupal.CulturefeedSearch.Activities.pagerClickListener);
     Drupal.CulturefeedSearch.Activities.filterForm = $('#culturefeed-social-user-activity-filter-form');
     if (Drupal.CulturefeedSearch.Activities.filterForm.length) {
       Drupal.CulturefeedSearch.Activities.filterSelect = Drupal.CulturefeedSearch.Activities.filterForm.find('#edit-filter');
       Drupal.CulturefeedSearch.Activities.filterSelect.bind('change', Drupal.CulturefeedSearch.Activities.filterListener);
     }

     $('#activities-table tr').hover(function () {
       $(this).toggleClass("hover");
     });

   }
  };

  /**
   * Listener on the pager link.
   */
  Drupal.CulturefeedSearch.Activities.pagerClickListener = function() {

    $(this).after(Drupal.CulturefeedSearch.Activities.throbber);

    var post_data = '';
    if (Drupal.CulturefeedSearch.Activities.filterSelect && Drupal.CulturefeedSearch.Activities.filterSelect.val() != 'all') {
      post_data = ('type=' + Drupal.CulturefeedSearch.Activities.filterSelect.val());
    }

    var $pager = $(this);

    $.ajax({
      url : $pager.attr('href'),
      data : post_data,
      success : function (result) {
        Drupal.CulturefeedSearch.Activities.applyPagerResult($pager, result);
      },
      'dataType' : 'json'
    });

    return false;

  }

  /**
   * Add the pager results to the current list.
   */
  Drupal.CulturefeedSearch.Activities.applyPagerResult = function($pager, result) {

    var $list_wrapper = $pager.parents('.activity-list-wrapper').eq(0);
    var $list = $list_wrapper.find('ul');

    for (var i = 0; i < result.results.length; i++) {
      $list.append('<li class="media">' + result.results[i] + '</li>');
    }

    Drupal.CulturefeedSearch.Activities.throbber.remove();

    if (result.new_pager_url == '') {
      $pager.hide();
    }
    else {
      $pager.attr('href', result.new_pager_url);
      $pager.show();
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

    var $list_wrapper = $('.activity-list').eq(0);
    var $list = $list_wrapper.find('ul');
    var $pager = $list_wrapper.find('.pager-link');

    $list.empty();
    Drupal.CulturefeedSearch.Activities.applyPagerResult($pager, result);

  }

})(jQuery);
