/**
 * @file
 * Js functionality for the review add form.
 */

Drupal.Culturefeed_entry_ui = Drupal.Culturefeed_entry_ui || {};

(function ($) {
  
  Drupal.behaviors.price = {
    attach: function (context, settings) {
    
      $(window).bind('load', function() {
        if($("#edit-price-free").attr("checked")==true) {
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
	    if($("#edit-price-free").attr("checked")==true) {
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
        if($('#edit-location-actor-location-actor-label').val() == '') {
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
     * Hides the autocomplete suggestions.
     */
    Drupal.jsAC.prototype.hidePopup = function (keycode, op) {

        // Select item if the right key or mousebutton was pressed.
        if (this.selected && ((keycode && keycode != 46 && keycode != 8 && keycode != 27) || !keycode)) {
            if ($(this.selected).data('autocompleteTitle') != undefined) {

                this.input.value = $(this.selected).data('autocompleteTitle');
                
                if (this.input.name == 'actor[location_actor_label]') {
				  $('#location_actor_id').val($(this.selected).data('autocompleteValue'));
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
            $(this.input).parents('.form-item').next().find(':tabbable').focus();
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
                    //.data('autocompleteLocationTitle', 'TEST')
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

    /**
     * Puts the currently highlighted suggestion into the autocomplete field.
     */
    Drupal.jsAC.prototype.select = function (node) {
        if ($(node).data('autocompleteTitle') != undefined) {
          this.input.value = $(node).data('autocompleteTitle');
          
          if (this.input.name == 'location[actor][location_actor_label]') {
		    $('#location_actor_id').val($(node).data('autocompleteValue'));
		  }
		  
		  if (this.input.name == 'organiser[actor][organiser_actor_label]') {
		    $('#organiser_actor_id').val($(node).data('autocompleteValue'));
		  }
		  
        }
        else {
            this.input.value = $(node).data('autocompleteValue');
        }
    };

})(jQuery);
