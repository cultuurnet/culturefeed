<?php

/**
 * @file
 * Template for the block that contains page suggestions.
 */
?>

<div class="container">

    <p class="block-title">In jouw buurt <span id="nearby-activities-title-location" class="text-muted" /></p>

    <p>
      <?php if (!empty($change_location_link)): ?>
        <?php print $change_location_link; ?>
      <?php endif; ?>

      <?php if (!empty($all_activities_for_location_link)): ?>
        <?php print $all_activities_for_location_link; ?>
      <?php endif; ?>
    </p>

    <div class="hidden" id="nearby-activities-filter-form-wrapper">
      <?php print drupal_render($filter_form); ?>
    </div>

    <div id="nearby-activities">
      <p class="text-muted"><i class="fa fa-refresh fa-spin"></i>Loading</p>
    </div>

</div>