
(function($) {

  Drupal.CulturefeedSearch = Drupal.CulturefeedSearch || {};
  Drupal.CulturefeedSearch.lastLocationData = [];

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
        change: function (event, ui) {
          Drupal.CulturefeedSearch.validateSelectedSuggestion(this);
        },
        autoFocus: true,
        autoSelect: true,
        selectFirst: true
      });

      $("#edit-where").once('location-search-init').categorisedAutocomplete({
        source: Drupal.CulturefeedSearch.getLocationData,
        select: function(event, ui) {
          Drupal.CulturefeedSearch.handleSuggestionSelect(this, ui.item);
        },
        change: function (event, ui) {
          Drupal.CulturefeedSearch.validateSelectedSuggestion(this);
        },
        autoFocus: true,
        autoSelect: true,
        selectFirst: true
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
        Drupal.CulturefeedSearch.lastLocationData = data;
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
            // Always make sure the element has a value.
            if (!element.value) {
              element.value = element.label;
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

  /**
   * Handle the suggestion select.
   */
  Drupal.CulturefeedSearch.handleSuggestionSelect = function(element, selected_suggestion) {

    // Actors are direct links.
    if (selected_suggestion.type == 'actor') {
      // Go to the actor page aftoer selecting an actor.
      window.location.href = selected_suggestion.suggestion;
      // Disable form submission after selection of an actor item.
      $(this).parents('form').find('button').attr('disabled', true);
    }
    // Trigger location search.
    else {
      if (selected_suggestion.value.indexOf(' (+ ') != -1) {
        // Reset value and label to remove the boroughs text.
        selected_suggestion.value = selected_suggestion.suggestion;
        selected_suggestion.label = selected_suggestion.suggestion;
      }
      $(element).val(selected_suggestion.suggestion);
    }

  }

  /**
   * Validate if the selected suggestion is correct.
   * If not, take the first correct suggestion.
   */
  Drupal.CulturefeedSearch.validateSelectedSuggestion = function(element) {

    var $element = $(element);
    var current_val = $element.val();
    var valid_suggestion = false;
    // Loop over all found location data and check if it matches current value.
    for (var i = 0; i < Drupal.CulturefeedSearch.lastLocationData.length; i++) {
      if (Drupal.CulturefeedSearch.lastLocationData[i].label == current_val) {
        valid_suggestion = true;
        // Trigger the select handler incase a special case was selected.
        Drupal.CulturefeedSearch.handleSuggestionSelect(element, Drupal.CulturefeedSearch.lastLocationData[i]);
        break;
      }
    }

    // None of the items in the autocomplete are selected.
    if (!valid_suggestion) {
      if (Drupal.CulturefeedSearch.lastLocationData.length === 0) {
        $element.val('');
      }
      else {
        $element.val(Drupal.CulturefeedSearch.lastLocationData[0].suggestion);
      }
    }

  }


})(jQuery);
