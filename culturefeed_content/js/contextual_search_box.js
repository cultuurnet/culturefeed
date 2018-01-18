(function ($) {

  Drupal.behaviors.togglePostalCodeField = {
    attach: function (context, settings) {
      $('#toggle_postal_code').click(function(e){
        e.preventDefault();
        $('div.postal-code-container').show();
      });

      $('#edit-postal-code').keypress(function(e){
        if (e.which == 13) {
          e.preventDefault();
          $('div.postal-code-container').hide();
        }
      });
    }
  }

}(jQuery));