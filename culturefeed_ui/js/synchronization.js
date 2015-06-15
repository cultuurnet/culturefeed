(function ($) {

  Drupal.behaviors.culturefeed_ui_synchronization = {

    attach: function (context, settings) {

      $('#profile-edit-synchronization').find('a').click(function (event) {

        var link = $(this);
        var container = $("<div></div>");
        var url = link.attr('href');
        var title = link.attr('title');
        var throbber_html = "&nbsp;<span class=\"ajax-progress ajax-progress-throbber\">";
        throbber_html += "<span class=\"throbber\">&nbsp;</span></span>";
        var throbber = $(throbber_html);

        link.append(throbber);

        $.ajax({
          url: url,
          type: 'GET',
          success: function(data) {

            var modal = {
              modal: true,
              title: title,
              open: function() {
                throbber.remove();
              }
            };

            container.html(data).dialog(modal).dialog('open');

          }

        });

        event.preventDefault();

      });

    }

  };

})(jQuery);
