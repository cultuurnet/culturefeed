<?php

/**
 * @file
 * Template for the timeline block.
 *
 * #timeline must exist to allow the ajax of the filter-form to work.
 */
?>

<?php print render($filter_form); ?>
<div id="timeline">
  <?php print $activities; ?>
</div>
