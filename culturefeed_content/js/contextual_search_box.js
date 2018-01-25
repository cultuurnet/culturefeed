(function ($) {

  Drupal.behaviors.togglePostalCodeField = {
    attach: function (context, settings) {
      $('.toggle_postal_code').click(function(e){
        e.preventDefault();
        var id = '#postal_code_container_' + $(this).data('id');
        $(id).show();
      });

      $('.postal-code-container-postal-code').keypress(function(e){
        if (e.which == 13) {
          e.preventDefault();
          var id = '#postal_code_container_' + $(this).data('id');
          $(id).hide();
        }
      });
    }
  }

}(jQuery));