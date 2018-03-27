(function ($) {

  Drupal.behaviors.togglePostalCodeField = {
    attach: function (context, settings) {
      $('.toggle_postal_code').once('toggle-init').click(function(e){
        e.preventDefault();
        $('#' + $(this).data('id')).toggle();
      });
    }
  }

}(jQuery));