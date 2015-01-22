
(function ($) {

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
                this.hidePopup(null, 'empty');
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
