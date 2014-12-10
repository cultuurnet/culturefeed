
(function($) {

  Drupal.behaviors.culturefeedCalendar = {
    attach: function (context, settings) {
      Drupal.setAddViewCalendarButtons();
      Drupal.setTotalCookieActivitiesLabel();
    }
  };

  Drupal.setAddViewCalendarButtons = function () {

    // Set default buttons.
    $(".add-to-calendar").show();
    $(".view-calendar").hide();

    if ($.cookie('Drupal.visitor.calendar') !== null) {

      // Get cookie calendar information.
      cookie = jQuery.parseJSON($.cookie('Drupal.visitor.calendar'));

      // Loop through the cookie event objects and store the nodeId's in an array.
      var ids = [];
      $.each(cookie, function(index, value) {
        ids.push(value["nodeId"]);
      });

      // Get the id of the current event (from the url)
      var pathArray = window.location.pathname.split( '/' );
      var id = pathArray[4];

      // Change buttons if needed.
      if ($.inArray(id, ids) !== -1) {
        $(".add-to-calendar").hide();
        $(".view-calendar").show();
      }
    }
  }

  Drupal.setTotalCookieActivitiesLabel = function() {
    // Set default state.
    $("small.activity-count").hide();

    // Get cookie calendar information.
    if ($.cookie('Drupal.visitor.calendar') !== null) {
      cookie = jQuery.parseJSON($.cookie('Drupal.visitor.calendar'));

      // Count the cookie event objects.
      var total = 0;
      $.each(cookie, function(index, value) {
        total++;
      });

      // set the label and value if needed.
      if (total > 0) {
        $("span.unread-activities").text(total);
        $("small.activity-count").show();
      }
    }
  }

})(jQuery);