
(function ($) {

    /**
     * Hides the autocomplete suggestions.
     */
    Drupal.jsAC.prototype.hidePopup = function (keycode) {
        // Select item if the right key or mousebutton was pressed.
        if (this.selected && ((keycode && keycode != 46 && keycode != 8 && keycode != 27) || !keycode)) {
            this.input.value = $(this.selected).data('autocompleteValue');
        }
        // Hide popup.
        var popup = this.popup;
        if (popup) {
            this.popup = null;
            $(popup).fadeOut('fast', function () { $(popup).remove(); });
        }
        this.selected = false;
        $(this.ariaLive).empty();

        // Workaround for bootstrap losing tabindex on autocomplete popup.
        $(this.input).parents('.form-item').next().find(':tabbable').focus();

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
        var ul = $('<ul class="dropdown-menu"></ul>');
        var ac = this;
        ul.css({
            display: 'block',
            right: 0
        });
        for (key in matches) {

            var value;
            if (typeof(matches[key]) == "string") {
                value = matches[key];
                $('<li></li>')
                    .html($('<a href="#"></a>').html(value).click(function (e) { e.preventDefault(); }))
                    .mousedown(function () { ac.select(this); })
                    .mouseover(function () { ac.highlight(this); })
                    .mouseout(function () { ac.unhighlight(this); })
                    .data('autocompleteValue', value)
                    .appendTo(ul);
            }
            else if (typeof(matches[key]) == "object") {
                value = matches[key].title;
                $('<li></li>')
                    .html($('<a href="#"></a>').html(value).click(function (e) { e.preventDefault(); }))
                    .mousedown(function () { ac.select(this); })
                    .mouseover(function () { ac.highlight(this); })
                    .mouseout(function () { ac.unhighlight(this); })
                    .data('autocompleteTitle', value)
                    .data('autocompleteValue', matches[key].key)
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
                if (this.input.name == 'location[actor][location_actor_label]') {
                    $('#edit-location-new-add-new-location').css({ display: 'none' });
                }
            }
            else {
                $(this.popup).css({ visibility: 'hidden' });
                this.hidePopup();
                if (this.input.name == 'organiser[actor][organiser_actor_label]') {
                    $('#edit-organiser-add-new-actor').css({ display: 'block' });
                    $('#organiser_actor_id').val('');
                }
                if (this.input.name == 'location[actor][location_actor_label]') {
                    $('#edit-location-new-add-new-location').css({ display: 'block' });
                    $('#location_actor_id').val('');
                }
            }
        }
    };

})(jQuery);
