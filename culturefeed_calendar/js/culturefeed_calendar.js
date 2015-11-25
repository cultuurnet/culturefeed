
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

        // Show the correct buttons.
        $('.calendar-button').each(function() {

          var $this = $(this);
          var eventid = $this.data('eventid');
          // When item is added, only show btn-view-calendar.
          if ($.inArray(eventid, ids) !== -1) {
            if ($this.hasClass('btn-view-calendar')) {
              $this.show();
            }
          }
          else {
            if (!$this.hasClass('btn-view-calendar')) {
              $this.show();
            }
          }

      })

    }
    else {
      // No cookie => show all add to calendars.
      $(".btn-add-calendar").show();
      $(".btn-like-calendar").show();
    }
  }

  /**
   * Show how many activites are currently stored in the cookie.
   */
  Drupal.CultureFeed.Calendar.showTotalAdded = function() {

    var $calendarItem = $('.calendar-popover-link');

    // Get cookie calendar information.
    if (Drupal.CultureFeed.Calendar.cookieJson !== null) {

      // Count the cookie event objects.
      var total = 0;
      $.each(Drupal.CultureFeed.Calendar.cookieJson, function(index, value) {
        total++;
      });

      // set the label and value if needed.
      // show popover by triggering click
      // not trigger on calendar-page!
      if (total > 0) {
        if (document.location.href.search("/culturefeed/calendar")==-1){
          $calendarItem.show();
          $('.calendar-popover-link').trigger('click');
        }

      }
    }
  }

})(jQuery);
