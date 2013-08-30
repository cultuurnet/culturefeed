<div class="uitpas_ui_advantage_details">
  <div class="uitpas_ui_advantage_description1 large"><strong><?php print $description1; ?></strong></div>
  <?php if ($period): ?>
  <div class="uitpas_ui_advantage_period"><?php print $period; ?></div>
  <?php endif; ?>
  <div class="uitpas_ui_advantage_location"><?php print $location; ?></div>
  <div class="uitpas_ui_advantage_available"><?php print $available; ?></div>
</div>
<div class="uitpas_ui_advantage_image">
<?php if (count($pictures) > 0) print $pictures[0]; ?>
</div>
<?php if ($description2): ?>
<div class="advantages_info">
  <fieldset>
    <legend>Meer info</legend>
    <?php if (count($pictures) > 1): ?>
    <div class="image"><?php print $pictures[1]; ?></div>
  <?php endif; ?>
    <div class="uitpas_ui_advantage_description2 text"><?php print $description2; ?></div>
  </fieldset>
</div>
<?php endif; ?>
