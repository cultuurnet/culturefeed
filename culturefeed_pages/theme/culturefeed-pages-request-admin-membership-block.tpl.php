<?php
/**
 * Block template to request admin membership.
 * @vars
 *   - $url: use this if you want to customize the link (no ajax)
 *   - $link : ajax link
 */
?>
<h3>Foutje gezien?</h3>
<p>Deze pagina heeft nog geen beheerder. Als beheerder kan je de info op deze pagina wijzigen en de leden beheren.</p>
<a href="<?php print $url ?>">Stuur aanvraag om beheerder te worden</a>

<?php print $link ?>