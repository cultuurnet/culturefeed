
(function ($) {
  Drupal.behaviors.culturefeedSearchUiCityFacet = {
    attach: function (context, settings) {
      $('.city-facet').autocomplete({
        source: function(term, callback) {
          var filters = $.param({parents: Drupal.settings.culturefeed_search_ui.city_filters});
          //$.getJSON(Drupal.settings.basePath + 'autocomplete/culturefeed_ui/city-region-suggestion/' + term.term + '?' + filters, callback);
          var widget = $(this.element);
          $.ajax({
            url: Drupal.settings.basePath + 'autocomplete/culturefeed_ui/city-region-suggestion/' + term.term + '?' + filters,
            success: function (data) {
              if (data.length === 0) {
                widget.removeClass('throbbing');
              }
              callback(data);
            },
            error: function () {
              callback([]);
            }
          });
        },
        select: function(event, ui) {
          $(this).val(ui.item.value);
          this.form.submit();
        },
        search: function(){
          $(this).addClass('throbbing');
        },
        open: function(event, ui) {
          $(this).removeClass('throbbing');
          $(this).data("autocomplete").menu.next(event);
        },
        autoFocus: true
      }).keydown(function(event) {
        if (event.which == 13) {
          event.preventDefault();
          var value = $(this).val();
          var form = this.form;
          $.getJSON(Drupal.settings.basePath + 'autocomplete/culturefeed_ui/city-region-suggestion/' + value, function(data) {
            var validated = false;
            $.each(data, function(key, option) {
              if (option.value == value) {
                validated = true;
              }
            });
            if (validated) {
              form.submit();
            }
          });
        }
      });
    }
  };
})(jQuery);
