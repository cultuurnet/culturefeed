(function ($) {

// Close popup.
Drupal.behaviors.culturefeedPopupConnect = {
  attach: function (context, settings) {
    // This will open in a popup in case there's class called 'culturefeedpopuplogin'.
    // By default, the class on those links is culturefeedconnect, so change this
    // in your theme in case you want to have a popup functionality for your site.
    $('a.culturefeedpopuplogin', context).click(function() {
      var href = $(this).attr('href');

      var hasPopupQuery = href.search(/closepopup\=true/i) > -1;
      var hasQuestionMark = href.search(/\?/) > -1;

      if (!hasPopupQuery) {
        if (hasQuestionMark) {
          href += '&closepopup=true';
        }
        else {
          href += '?closepopup=true';
        }
      }

      window.open(href, 'UiTiD', 'location=1,status=1,scrollbars=1,resizable=1,width=810,height=635');

      return false;
    });
  }
};

/**
 * Add click events on all share links to register activities in uitid.
 */
Drupal.behaviors.culturefeedPushActivityToUitId = {
  attach: function(context, settings) {
    $('a.share-link', context).bind('click', function(e) {

      e.preventDefault();
      var rel = $(this).prop('rel');
      var href = $(this).attr('href');
      $.ajax({
        url: rel,
        complete: function() {
          if ($(this).hasClass("print-link")) {
            window.print();
          }
          else {
            location.href = href;
          }
        }
      });

    });
  }
};

if (Drupal.ajax) {

  /**
   * Command to provide a bridge between culturefeed_bootstrap and framework.
   */
  Drupal.ajax.prototype.commands.culturefeedModal = function (ajax, response, status) {

    if (Drupal.ajax.prototype.commands.bootstrapModal != undefined) {
      Drupal.ajax.prototype.commands.bootstrapModal(ajax, response, status);
    }
    else {
      Drupal.ajax.prototype.commands.insert(ajax, response, status);
    }

  };

  /**
   * Command to reload current page.
   */
  Drupal.ajax.prototype.commands.culturefeedGoto = function (ajax, response, status) {

    if (ajax.progress.element) {
      $(ajax.element).addClass('progress-disabled').attr('disabled', 'disabled');
      $(ajax.element).after(ajax.progress.element);
    }

    window.location.href = response.url;
  }

}

})(jQuery);