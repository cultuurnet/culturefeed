(function ($) {

  Drupal.behaviors.togglePostalCodeField = {
    attach: function (context, settings) {
      $('#toggle_postal_code').click(function(e){
        e.preventDefault();
        $('div.postal-code-container').toggle();
      });
    }
  }

}(jQuery));