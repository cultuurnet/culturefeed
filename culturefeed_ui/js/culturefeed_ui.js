(function ($) {

Drupal.behaviors.cultureFeedUIRelatedEvents = {
  attach: function (context, settings) {
    if (Drupal.settings.culturefeed_ui.related_events) {
      var url = Drupal.settings.culturefeed_ui.related_events;
      $.get(url, function(data) {
        $('#related-events').html(data);
      });
    }
    if (Drupal.settings.culturefeed_ui.recommendations) {
      var url = Drupal.settings.culturefeed_ui.recommendations;
      $.get(url, function(data) {
        $('#recommendations').html(data);
      });
    }
  }
};

})(jQuery);