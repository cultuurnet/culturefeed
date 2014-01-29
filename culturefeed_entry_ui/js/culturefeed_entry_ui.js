/**
 * @file
 * Js functionality for the review add form.
 */

Drupal.Culturefeed_entry_ui = Drupal.Culturefeed_entry_ui || {};

(function ($) {

    /**
     * Hides the autocomplete suggestions.
     */
    Drupal.jsAC.prototype.hidePopup = function (keycode) {

        // Select item if the right key or mousebutton was pressed.
        if (this.selected && ((keycode && keycode != 46 && keycode != 8 && keycode != 27) || !keycode)) {
            if ($(this.selected).data('autocompleteTitle') != undefined) {

                this.input.value = $(this.selected).data('autocompleteTitle');
                // TODO: For Debugging this true/false must be replaced
                /*if (true) {
                    $('#location_actor_id').val($(this.selected).data('autocompleteValue'));
                } else {
                    $('#organiser_actor_id').val($(this.selected).data('autocompleteValue'));
                }*/

                // Trigger eventsearch event to trigger ajax post.
                var $input = $('#location_actor_id');
                var $input = $('#organiser_actor_id');
                $input.trigger('location_eventsearch');
                $input.trigger('organiser_eventsearch');
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
                    .data('autocompleteLocationTitle', 'TEST')
                    .data('autocompleteValue', row.key)
                    .appendTo(ul);
            }
        }

        // Show popup with matches, if any.
        if (this.popup) {
            if (ul.children().length) {
                $(this.popup).empty().append(ul).show();
                $(this.ariaLive).html(Drupal.t('Autocomplete popup'));
                //$('#edit-organiser-add-new-actor').css({ display: 'none' });
            }
            else {
                $(this.popup).css({ visibility: 'hidden' });
                this.hidePopup();
                //$('#edit-organiser-add-new-actor').css({ display: 'block' });
            }
        }
    };

    /**
     * Puts the currently highlighted suggestion into the autocomplete field.
     */
    Drupal.jsAC.prototype.select = function (node) {
        if ($(node).data('autocompleteTitle') != undefined) {
            this.input.value = $(node).data('autocompleteTitle');
        }
        else {
            this.input.value = $(node).data('autocompleteValue');
        }
    };


})(jQuery);