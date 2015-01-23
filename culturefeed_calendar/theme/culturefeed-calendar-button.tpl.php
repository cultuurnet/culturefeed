<?php
/**
 * @file
 * Template for the calendar add or view buttons.
 */
?>

<div class="calendar-button">
  <?php if (isset($button['description'])) : ?>
    <?php print $button['description']; ?>
  <?php endif; ?>
  <?php print l($button['text'], $button['path'], $button['options']); ?>
</div>
