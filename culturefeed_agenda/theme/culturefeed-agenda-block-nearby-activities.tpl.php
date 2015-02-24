<?php

/**
 * @file
 * Template for the block that contains page suggestions.
 */
?>

<div>

  <p><?php print (t('Events nearby')); ?><span id="nearby-activities-title-location"></span></p>

    <p>
      <?php if (!empty($change_location_link)): ?>
        <?php print $change_location_link; ?>
      <?php endif; ?>

      <?php if (!empty($all_activities_for_location_link)): ?>
        <?php print $all_activities_for_location_link; ?>
      <?php endif; ?>
    </p>

    <div id="nearby-activities-filter-form-wrapper">
      <?php print drupal_render($filter_form); ?>
    </div>

    <div id="nearby-activities">
      <p>Loading</p>
    </div>

</div>