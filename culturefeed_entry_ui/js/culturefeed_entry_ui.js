/**
 * @file
 * Js functionality for the review add form.
 */

Drupal.Culturefeed_entry_ui = Drupal.Culturefeed_entry_ui || {};

(function ($) {

  Drupal.behaviors.price = {

    attach: function (context, settings) {

      $(window).bind('load', function() {
        if($("#edit-price-free").is(":checked")) {
          $('#edit-price-amount').val('0');
          $('#edit-price-amount').attr('disabled','disabled');
          $('#edit-price-amount').css('color','#ccc');
          $('#edit-price-extra').css('display','none');
          $('#edit-price-extra-extra-info').val('');
        }
        else {
          //$('#edit-price-amount').val('');
          $('#edit-price-amount').removeAttr('disabled');
          $('#edit-price-amount').css('color','#000');
          $('#edit-price-extra').css('display','block');
        }
      });

      $('#edit-price-free').change(function () {
        if($("#edit-price-free").is(":checked")) {
          $('#edit-price-amount').val('0');
          $('#edit-price-amount').attr('disabled','disabled');
          $('#edit-price-amount').css('color','#ccc');
          $('#edit-price-extra').css('display','none');
          $('#edit-price-extra-extra-info').val('');
        }
        else {
          $('#edit-price-amount').val('');
          $('#edit-price-amount').removeAttr('disabled');
          $('#edit-price-amount').css('color','#000');
          $('#edit-price-extra').css('display','block');
        }
      });

      $('#edit-location-actor-location-actor-label').change(function() {
        if($("[name='location[location_control][asset][label]']").val() == '') {
          $('#location_actor_id').val('');
        }
      });

      $('#edit-organiser-actor-organiser-actor-label').change(function() {
        if($('#edit-organiser-actor-organiser-actor-label').val() == '') {
          $('#organiser_actor_id').val('');
        }
      });

    }

  };

  /**
   * Maxlength
   */
  Drupal.behaviors.maxlength = {

    attach: function (context, settings) {
      $('#edit-description-sd-short-description').maxlength({
        max: 400,
        feedbackTarget: '#edit-description-sd .help-block, #edit-description-sd .description'
      });
    }

  }

  /**
   * Fire the autocomplete on paste.
   */
  Drupal.behaviors.autocomplete_paste = {

    attach: function () {
      $('input.form-autocomplete, .form-autocomplete input.form-text').bind("input propertychange", function (event) {
        $(this).trigger('keyup');
      });
    }

  }

  /**
   * Ensure submit button only gets clicked once.
   */
  Drupal.behaviors.submit_once = {

    attach: function () {

      var form = $('#culturefeed-entry-ui-event-form');

      form.submit(function(event) {
        $(this).data('submitted', true);
      });

      $('.main-submit, .btn-primary').click(function () {
        if (form.data('submitted') === true) {
          $(this).attr('disabled', 'disabled');
          return false;
        }
      });

    }

  }

  /**
   * Hides the autocomplete suggestions.
   */
  Drupal.jsAC.prototype.hidePopup = function (keycode, op) {

    // Select item if the right key or mousebutton was pressed.
    if (this.selected && ((keycode && keycode != 46 && keycode != 8 && keycode != 27) || !keycode)) {

      if ($(this.selected).data('autocompleteTitle') != undefined) {

        this.input.value = $(this.selected).data('autocompleteTitle');

        if (this.input.name == 'location[location_control][asset][label]') {
          $('#location_actor_id').val($(this.selected).data('autocompleteValue'));
          $('#location_asset_remove').show();
          $(this.input).attr('readonly', 'readonly');
        }

        if (this.input.name == 'organiser[actor][organiser_actor_label]') {
          $('#organiser_actor_id').val($(this.selected).data('autocompleteValue'));
        }

      }

      else {
        this.input.value = $(this.selected).data('autocompleteValue');
      }

    }

    // Hide popup.
    var popup = this.popup;
    if (popup) {

      this.popup = null;
      $(popup).fadeOut('fast', function () {
        $(popup).remove();
      });

    }
    this.selected = false;
    $(this.ariaLive).empty();

    // Workaround for bootstrap losing tabindex on autocomplete popup.
    if ((!op || op != 'empty') && this.input.value) {

      var inputs = $(this.input).closest('form').find(':input:visible');
      var index = inputs.index($(this.input));
      inputs.eq(index + 1).focus();

    }

  };

  /**
   * Fills the suggestion popup with any matches received.
   */
  Drupal.jsAC.prototype.found = function (matches) {

    // If no value in the textfield, do not show the popup.
    if (!this.input.value.length) {
      return false;
    }

    // Prepare matches.
    var ul = $('<ul></ul>');
    var ac = this;
    for (key in matches) {

      var row = matches[key];

      if (typeof(row) == "string") {

        $('<li></li>')
          .html($('<div></div>').html(matches[key]))
          .mousedown(function () {
            ac.select(this);
          })
          .mouseover(function () {
            ac.highlight(this);
          })
          .mouseout(function () {
            ac.unhighlight(this);
          })
          .data('autocompleteValue', key)
          .appendTo(ul);

      }

      else if (typeof(row) == "object") {

        $('<li></li>')
          .html($('<div></div>').html(row.locationTitle))
          .mousedown(function () {
            ac.select(this);
          })
          .mouseover(function () {
            ac.highlight(this);
          })
          .mouseout(function () {
            ac.unhighlight(this);
          })
          .data('autocompleteTitle', row.title)
          .data('autocompleteValue', row.key)
          .appendTo(ul);

      }

    }

    // Show popup with matches, if any.
    if (this.popup) {

      if (ul.children().length) {

        $(this.popup).empty().append(ul).show();
        $(this.ariaLive).html(Drupal.t('Autocomplete popup'));

        if (this.input.name == 'organiser[actor][organiser_actor_label]') {
          $('#edit-organiser-add-new-actor').css({ display: 'none' });
        }
        if (this.input.name == 'location[location_control][asset][label]') {
          $('#location_custom_add').hide();
        }

      }

      else {

        $(this.popup).css({ visibility: 'hidden' });
        this.hidePopup();

        if (this.input.name == 'organiser[actor][organiser_actor_label]') {
          $('#edit-organiser-add-new-actor').css({ display: 'block' });
          $('#organiser_actor_id').val('');
        }
        if (this.input.name == 'location[location_control][asset][label]') {
          $('#location_custom_add').show();
          $('#location_actor_id').val('');
        }

      }

    }

  };

  /**
   * Puts the currently highlighted suggestion into the autocomplete field.
   */
  Drupal.jsAC.prototype.select = function (node) {

    if ($(node).data('autocompleteTitle') != undefined) {

      this.input.value = $(node).data('autocompleteTitle');

      if (this.input.name == 'location[location_control][asset][label]') {
        $('#location_actor_id').val($(node).data('autocompleteValue'));
        $('#location_asset_remove').show();
        $(this.input).attr('readonly', 'readonly');
      }

      if (this.input.name == 'organiser[actor][organiser_actor_label]') {
        $('#organiser_actor_id').val($(node).data('autocompleteValue'));
      }

    }

    else {
      this.input.value = $(node).data('autocompleteValue');
    }

  };

  /**
   * Performs a cached and delayed search.
   */
  Drupal.ACDB.prototype.search = function (searchString) {
    var db = this;
    searchString = searchString.replace(/^\s+|\s+$/, '');
    this.searchString = searchString;

    // See if this string needs to be searched for anyway.
    if (searchString.length <= 0 ||
      searchString.charAt(searchString.length - 1) == ',') {
      return;
    }

    // See if this key has been searched for before.
    if (this.cache[searchString]) {
      return this.owner.found(this.cache[searchString]);
    }

    // Initiate delayed search.
    if (this.timer) {
      clearTimeout(this.timer);
    }
    this.timer = setTimeout(function () {
      db.owner.setStatus('begin');

      // Ajax GET request for autocompletion. We use Drupal.encodePath instead of
      // encodeURIComponent to allow autocomplete search terms to contain slashes.
      $.ajax({
        type: 'GET',
        url: db.uri + '/' + Drupal.encodePath(searchString),
        dataType: 'json',
        success: function (matches) {
          if (typeof matches.status == 'undefined' || matches.status != 0) {
            db.cache[searchString] = matches;
            // Verify if these are still the matches the user wants to see.
            //if (db.searchString == searchString) {
            db.owner.found(matches);
            //}
            db.owner.setStatus('found');
          }
        },
        error: function (xmlhttp) {
          alert(Drupal.ajaxError(xmlhttp, db.uri));
        }
      });
    }, this.delay);
  };

})(jQuery);
