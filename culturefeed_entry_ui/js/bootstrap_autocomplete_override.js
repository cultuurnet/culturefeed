jQuery(function($) {

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

        value = matches[key].locationTitle;
        $('<li></li>')
          .html($('<a href="#"></a>').html(value).click(function (e) { e.preventDefault(); }))
          .mousedown(function () { ac.select(this); })
          .mouseover(function () { ac.highlight(this); })
          .mouseout(function () { ac.unhighlight(this); })
          .data('autocompleteTitle', matches[key].title)
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
        if (this.input.name == 'location[location_control][asset][label]') {
          $('#location_custom_add').hide();
        }

      }

      else {

        $(this.popup).css({ visibility: 'hidden' });
        this.hidePopup(null, 'empty');

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

  if (Drupal.ACDB) {

    /**
     * Performs a cached and delayed search.
     * Custom override: Don't show an error when people are navigation away of the site.
     *
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
              if (db.searchString == searchString) {
                db.owner.found(matches);
              }
              db.owner.setStatus('found');
            }
          },

          error: function (xmlhttp) {
            if (xmlhttp.status) {
              alert(Drupal.ajaxError(xmlhttp, db.uri));
            }
          }

        });

      }, this.delay);

    };

  }

});
