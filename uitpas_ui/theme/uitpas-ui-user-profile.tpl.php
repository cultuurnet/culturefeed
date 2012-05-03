<h2>Mijn UITPAS</h2>
<div class="uitpas_ui_profile_details">
<?php if ($image): ?>
  <div class="uitpas_ui_profile_details_image"><?php print $image; ?></div>
<?php endif; ?>
  <div class="uitpas_ui_profile_details_name"><?php print $name; ?></div>
  <div class="uitpas_ui_profile_details_points"><?php print $points; ?></div>
</div>
<div class="uitpas_ui_profile_advantages_promotions">
<h3>Jouw UITPAS voordelen</h3>
<?php print $advantages_promotions; ?>
</div>
<div class="uitpas_ui_profile_nearby_promotions">
<h3>Nog enkele punten sparen voor</h3>
<?php print $nearby_promotions; ?>
</div>