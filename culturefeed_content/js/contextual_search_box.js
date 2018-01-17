(function ($) {

  Drupal.behaviors.togglePostalCodeField = {
    attach: function (context, settings) {
      $('#toggle_postal_code').click(function(e){
        e.preventDefault();
        $('div.form-item-postal-code').toggle();
      });
    }
  }

}(jQuery));