
(function($) {
  Drupal.behaviors.culturefeedSearchUiCityActorFacet = {
    attach: function(context, settings) {


      $.widget("custom.catcomplete", $.ui.autocomplete, {
        _create: function() {
          this._super();
          this.widget().menu("option", "items", "> :not(.ui-autocomplete-category)");
        },
        _renderMenu: function(ul, items) {
          var that = this,
                  currentCategory = "";
          $.each(items, function(index, item) {
            var li;
            if (item.category != currentCategory) {
              ul.append("<li class='ui-autocomplete-category'>" + item.category + "</li>");
              currentCategory = item.category;
            }
            li = that._renderItemData(ul, item);
            if (item.category) {
              li.attr("aria-label", item.category + " : " + item.label);
            }
          });
        }
      });

      $(function() {
        var data = function(term, callback) {
          var widget = $(this.element);
          $.ajax({
            url: Drupal.settings.basePath + 'autocomplete/culturefeed_ui/city-actor-suggestion/' + term.term,
            success: function(data) {
              if (data.length === 0) {
                widget.removeClass('throbbing');
              }
              callback(data);
            },
            error: function() {
              callback([]);
            }
          });
        }

        $("#edit-city-actor").catcomplete({
          source: data,
          select: function(event, ui) {
            $(this).val(ui.item.suggestion + '|' + ui.item.type);
            $('.city-actor-facet-submit').focus();
            this.form.submit();
          },
          search: function() {
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
        });
      });





    }
  };
})(jQuery);
