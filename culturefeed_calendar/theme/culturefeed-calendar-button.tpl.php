<?php
/**
 * @file
 * Template for the calendar add or view buttons.
 * Leave the wrapper div class + print classes + data-eventid, as it is used for javascript.
 */
?>

<div class="<?php print $classes; ?>" data-eventid="<?php print $event_id; ?>">
  <?php if ($started) : ?>
    <?php print t('This event is already started'); ?>
  <?php elseif($finished) : ?>
    <?php print t('This event is already finished'); ?>
  <?php else : ?>
    <?php if (isset($button['description'])) : ?>
      <?php print $button['description']; ?>
    <?php endif; ?>
    <?php print l($button['text'], $button['path'], $button['options']); ?>
  <?php endif; ?>
</div>
