
(function ($) {
  Drupal.behaviors.culturefeedSearchUiCityFacet = {
    attach: function (context, settings) {
      var lastResult = [];
      $('.city-facet').autocomplete({
        source: function(term, callback) {
          var filters = $.param({parents: Drupal.settings.culturefeed_search_ui.city_filters});
          //$.getJSON(Drupal.settings.basePath + 'autocomplete/culturefeed_ui/city-region-suggestion/' + term.term + '?' + filters, callback);
          var widget = $(this.element);
          $.ajax({
            url: Drupal.settings.basePath + 'autocomplete/culturefeed_ui/city-region-suggestion/' + term.term + '?' + filters,
            success: function (data) {
              lastResult = data;
              if (data.length === 0) {
                widget.removeClass('throbbing');
              }
              else {
                $.each(data, function(index, element) {
                  if (!(element.label.match(/^\d+/))) {
                    if (!(element.label.match(/^Provinc|Regio+/))) {
                      element.label += ' (+ ' + Drupal.t('boroughs') + ')';
                    }
                  }
                });
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
          $('.city-facet-submit').focus();
          //this.form.submit();
        },
        search: function(){
          $(this).addClass('throbbing');
        },
        open: function(event, ui) {
          $(this).removeClass('throbbing');
          // Workaround for autoFocus missing before version 1.8.11
          // (http://jqueryui.com/changelog/1.8.11/)
          // We only check from 1.8.7, the version shipped with Drupal.
          var version = $.ui.version;
          var old = ['1.8.7', '1.8.8.', '1.8.9', '1.8.10'];
          if ($.inArray(version, old) >= 0) {
            $(this).data("autocomplete").menu.next(event);
          }
        },
        close: function (event, ui) {
          if ($(this).val().length !== 0) {
            var pickFirst = true;
            var input = $(this).val();
            $.each(lastResult, function( key, obj ) {
              if (input === obj.value) {
                pickFirst = false;
              }
            });

            if (pickFirst === true) {
              $(this).val(lastResult[0].value);
            }
          }
        },
        change: function (event, ui) {
          if (lastResult.length === 0) {
            $(this).val('');
          }
        },
        autoFocus: true,
        autoSelect: true,
        selectFirst: true
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
            /*if (validated) {
              form.submit();
            }*/
          });
        }
      });
    }
  };
})(jQuery);
