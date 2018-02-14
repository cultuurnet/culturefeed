/**
 * @file
 * Js functionality for the social links.
 */

(function ($) {

  Drupal.behaviors.culturefeedSocialLinks = {
    attach: function (context, settings) {

      $(context).find('a.do-link').once('no-double-click').bind('click', function (e) {
        console.log('click');
        var $link = $(e.target);
        if (!$link.data('no-click')) {

          $link.data('no-click', 1);

          setTimeout(function(){
            $link.data('no-click', 0);
          }, 5000);
        }
        else {
          console.log('nope');
          e.preventDefault();
        }
      });
    }
  };

})(jQuery);
