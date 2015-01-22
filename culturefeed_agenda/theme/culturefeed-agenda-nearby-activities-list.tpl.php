<?php

/**
 * @file
 * Theme the list of suggestions.
 */
?>


  <?php if (!empty($activities)): ?>
    <ul>
      <?php foreach ($activities as $activity): ?>
        <li>
          <?php print theme('culturefeed_agenda_nearby_activities_list_item', array('item' => $activity)); ?>
        </li>
      <?php endforeach; ?>
    </ul>
  <?php else: ?>
    <p><?php print t('No nearby activities found'); ?></p>
  <?php endif; ?>
