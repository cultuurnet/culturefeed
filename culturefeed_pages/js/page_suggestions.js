(function($) {

  // At page load, read the postal cookie and submit the form.
  Drupal.behaviors.pageSuggestions = {
    attach: function(context, settings) {
      var $submit = $('#culturefeed-pages-page-suggestions-filter-form').find('#edit-submit');
      if ($submit.hasClass('ajax-processed')) {
        $submit.once('suggestions-loaded', function() {

          var cookie = $.cookie('culturefeed_pages_suggestions_city');
          if (cookie) {
            $('#culturefeed-pages-page-suggestions-filter-form').find('#edit-city').val(decodeURIComponent(cookie.replace(/\+/g, ' ')));
          }

          $(this).trigger('mousedown');
        });
      }
    }
  }

  $(document).ready(function(){
    Drupal.jsAC.prototype.select = function (node) {
      this.input.value = $(node).data('autocompleteValue');
      if(jQuery(this.input).hasClass('auto_submit')){
        var $submit = $('#culturefeed-pages-page-suggestions-filter-form').find('#edit-submit');
        $submit.trigger('mousedown');
      }
    };
  });

})(jQuery);