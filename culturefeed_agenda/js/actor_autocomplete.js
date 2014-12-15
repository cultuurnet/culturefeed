(function ($) {

  Drupal.Culturefeed = Drupal.Culturefeed || {};

  /**
   * Get the nearby actors for current user.
   */
  Drupal.Culturefeed.getNearbyActors = function() {

    $.ajax({
      url : Drupal.settings.culturefeed.actorsSuggestUrl,
      success : function(data, status) {
        console.log(data);
        if (data.success) {
          $('#actor-search-suggest').html(data.data);
        }
      }
    })

  }

  /**
   * If an input has autosubmit as class. Redirect to the selected value.
   */
  Drupal.jsAC.prototype.select = function (node) {
    if ($(this.input).hasClass('auto-submit-field')) {
      window.location.href = $(node).data('autocompleteValue');
    }
    else {
      this.input.value = $(node).data('autocompleteValue');
    }
  };

  /**
   * Hides the autocomplete suggestions.
   */
  Drupal.jsAC.prototype.hidePopup = function (keycode) {
    // Select item if the right key or mousebutton was pressed.
    if (this.selected && ((keycode && keycode != 46 && keycode != 8 && keycode != 27) || !keycode)) {

      if ($(this.input).hasClass('auto-submit-field')) {
        window.location.href = $(node).data('autocompleteValue');
      }
      else {
        this.input.value = $(this.selected).data('autocompleteValue');
      }

    }
    // Hide popup.
    var popup = this.popup;
    if (popup) {
      this.popup = null;
      $(popup).fadeOut('fast', function () { $(popup).remove(); });
    }
    this.selected = false;
    $(this.ariaLive).empty();
  };

  /**
   * Attach behaviors for the actor autocomplete.
   */
  $(document).ready(function() {
    if ($.cookie('Drupal.visitor.uitid.userLocation') !== null) {
      Drupal.Culturefeed.getNearbyActors();
    }
    else {
      Drupal.CultureFeed.geolocate(Drupal.Culturefeed.getNearbyActors);
    }
  });

})(jQuery);
