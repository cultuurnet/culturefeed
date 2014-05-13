
(function ($) {
  Drupal.behaviors.culturefeedSearchUiCityFacet = {
    attach: function (context, settings) {
      $('.city-facet').autocomplete({
        source: function(term, callback) {
          $.getJSON(Drupal.settings.basePath + 'autocomplete/culturefeed_ui/city-region-suggestion/' + term.term, callback);
        },
        select: function(event, ui) {
          $(this).val(ui.item.value);
          this.form.submit();
        },
        open: function(event, ui) {
          var autocomplete = $(this).data("autocomplete");
          autocomplete.menu.next();
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
