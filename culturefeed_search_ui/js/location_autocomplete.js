
(function($) {

  Drupal.CulturefeedSearch = Drupal.CulturefeedSearch || {};

  /**
   * Init the location autocomplete.
   */
  Drupal.behaviors.culturefeedSearchUiCityActorFacet = {
    attach: function(context, settings) {

      $("#edit-block-location-search").once('location-search-init').categorisedAutocomplete({
        source: Drupal.CulturefeedSearch.getLocationData,
        select: function(event, ui) {

          // Actors are direct links.
          if (ui.item.type == 'actor') {
            window.location.href = ui.item.suggestion;
          }
          // Trigger location search.
          else {
            $(this).val(ui.item.suggestion);
            $('.location-search-submit').focus();
            this.form.submit();
          }

        },
        autoFocus: true,
      });

      $("#edit-where").once('location-search-init').categorisedAutocomplete({
        source: Drupal.CulturefeedSearch.getLocationData,
        select: function(event, ui) {

          // Actors are direct links.
          if (ui.item.type == 'actor') {
            window.location.href = ui.item.suggestion;
          }
          // Trigger location search.
          else {
            if (ui.item.value.indexOf(' (+ ') != -1) {
              // Reset value and label to remove the boroughs text.
              ui.item.value = ui.item.suggestion;
              ui.item.label = ui.item.suggestion;
            }
            $(this).val(ui.item.suggestion);
          }

        },
        autoFocus: true
      });

    }
  };

  /**
   * Get the data for the location autocomplete
   */
  Drupal.CulturefeedSearch.getLocationData = function(term, callback) {
    var widget = $(this.element);
    $.ajax({
      url: Drupal.settings.basePath + Drupal.settings.pathPrefix + 'autocomplete/culturefeed_ui/city-actor-suggestion/' + term.term,
      success: function(data) {
        if (data.length === 0) {
          widget.removeClass('throbbing');
        }
        else {
          $.each(data, function(index, element) {
            if (element.type == 'city' && !(element.label.match(/^\d+/))) {
              if (!(element.label.match(/^Provinc|Regio+/))) {
                element.label += ' (+ ' + Drupal.t('boroughs') + ')';
              }
            }
          });
        }
        callback(data);
      },
      error: function() {
        callback([]);
      }
    });
  }



})(jQuery);
