/* http://keith-wood.name/maxlength.html
   Dutch initialisation for the jQuery Max Length extension */
(function($) { // hide the namespace

$.maxlength.regionalOptions['nl'] = {
	feedbackText: '{r} karakters resterend ({m} maximum)',
	overflowText: '{o} karakters teveel ({m} maximum)'
};
$.maxlength.setDefaults($.maxlength.regionalOptions['nl']);

})(jQuery);
