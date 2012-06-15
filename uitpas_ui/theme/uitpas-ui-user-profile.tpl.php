<div class="uitpas_ui_profile">
  <div class="uitpas_ui_profile_details">
    <?php if ($image): ?><div class="uitpas_ui_profile_details_image"><?php print $image; ?></div><?php endif; ?>
    <div class="uitpas_ui_profile_details_name"><?php print $name; ?></div>
    <div class="uitpas_ui_profile_details_points"><?php print $points; ?></div>
  </div>
  <div class="uitpas_ui_profile_advantages_promotions">
  <h3>Jouw UITPAS voordelen</h3>
    <?php print $advantages_promotions; ?>
  </div>
  <?php if ($coming_promotions): ?>
  <div class="uitpas_ui_profile_coming_promotions">
    <h3>Nog enkele punten sparen voor</h3>
    <?php print $coming_promotions; ?>
  </div>
  <?php endif; ?>
  <div class="uitpas_ui_profile_all_promotions">
    <?php print $all_promotions; ?>
  </div>
</div>