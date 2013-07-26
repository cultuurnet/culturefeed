<?php
/**
 * Block template to request admin membership.
 * @vars
 *   - $url: use this if you want to customize the link (no ajax)
 *   - $link : ajax link
 */
?>
<h3><?php print t('Mistake seen?'); ?></h3>
<p><?php print t('This page has no administrator. As an administrator, you can change the information on this page and manage the members.'); ?></p>
<a href="<?php print $url ?>"><?php print t('Send application to become an administrator'); ?></a>

<?php print $link ?>