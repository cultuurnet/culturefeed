<?php
/**
 * @file
 * Template for the calendar add or view buttons.
 * Leave the wrapper div class + data-eventid, as it is used for javascript.
 */
?>

<div class="calendar-button" data-eventid="<?php print $event_id; ?>">
  <?php if (isset($button['description'])) : ?>
    <?php print $button['description']; ?>
  <?php endif; ?>
  <?php print l($button['text'], $button['path'], $button['options']); ?>
</div>
