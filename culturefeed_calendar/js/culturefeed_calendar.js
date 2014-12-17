
(function($) {

  Drupal.CultureFeed = Drupal.CultureFeed || {};
  Drupal.CultureFeed.Calendar = {};
  Drupal.CultureFeed.Calendar.cookieJson = null;

  $(document).ready(function() {

    if ($.cookie('Drupal.visitor.calendar') !== null) {
      Drupal.CultureFeed.Calendar.cookieJson = jQuery.parseJSON($.cookie('Drupal.visitor.calendar'));
    }

    Drupal.CultureFeed.Calendar.initButtons();
    Drupal.CultureFeed.Calendar.showTotalAdded();
  });

  /**
   * Init the calendar buttons.
   */
  Drupal.CultureFeed.Calendar.initButtons = function () {

    if (Drupal.CultureFeed.Calendar.cookieJson !== null) {

      // Loop through the cookie event objects and store the nodeId's in an array.
      var ids = [];
      $.each(Drupal.CultureFeed.Calendar.cookieJson, function(index, value) {
        ids.push(value["nodeId"]);
      });

      // Change buttons if needed.
      if ($.inArray(Drupal.settings.culturefeed.currentEventId, ids) !== -1) {
        $(".add-to-calendar").hide();
        $(".view-calendar").show();
      }
      else {
        $(".add-to-calendar").show();
        $(".view-calendar").hide();
      }
    }
    else {
      $(".add-to-calendar").show();
    }
  }

  /**
   * Show how many activites are currently stored in the cookie.
   */
  Drupal.CultureFeed.Calendar.showTotalAdded = function() {

    var $calendarItem = $('#block-culturefeed-ui-profile-box').find('li.activities');
    $calendarItem.hide();

    // Get cookie calendar information.
    if (Drupal.CultureFeed.Calendar.cookieJson !== null) {

      // Count the cookie event objects.
      var total = 0;
      $.each(Drupal.CultureFeed.Calendar.cookieJson, function(index, value) {
        total++;
      });

      // set the label and value if needed.
      if (total > 0) {
        $("span.unread-activities").text(total);
        $calendarItem.show();
      }
    }
  }

})(jQuery);