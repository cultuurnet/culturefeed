<?php
/**
 * @file
 * Template for the calendar add or view buttons.
 * Leave the wrapper div class + print classes + data-eventid, as it is used for javascript.
 */
?>

<div class="<?php print $classes; ?>" data-eventid="<?php print $event_id; ?>">
  <?php if ($finished) : ?>
    <?php print $finished_message; ?>
  <?php else : ?>
    <?php if (isset($button['description'])) : ?>
      <?php print $button['description']; ?>
    <?php endif; ?>
    <?php print l($button['text'], $button['path'], $button['options']); ?>
  <?php endif; ?>
</div>
