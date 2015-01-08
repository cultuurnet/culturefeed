<?php
/**
 * @file
 * Template for the summary of a page.
 */
?>

<hr />
Fluttersturrkph
<h3><a href="<?php print $url ?>"><?php print $title; ?> <?php if (!empty($address)): ?>- <?php print $address['city']; ?><?php endif; ?></a></h3>

<p><?php print $description ?></p>

<p><a href="<?php print $url; ?>"><?php print $more_text; ?></a></p>





















