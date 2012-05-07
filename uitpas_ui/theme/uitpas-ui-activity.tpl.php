<div class="uitpas_ui_activity_details">
  <?php if ($icons): ?>
    <div class="uitpas_ui_activity_details_icons"><?php print $icons; ?></div>
  <?php endif; ?>
  <div class="uitpas_ui_activity_details_general">
    <div class="uitpas_ui_activity_details_general_headings"><?php print $headings; ?></div>
    <div class="uitpas_ui_activity_details_general_shortdescription"><?php print $shortdescription; ?></div>
    <div class="uitpas_ui_activity_details_general_when"><?php print $when; ?></div>
    <div class="uitpas_ui_activity_details_general_where"><?php print $where; ?></div>
    <div class="uitpas_ui_activity_details_general_price"><?php print $price; ?></div>
    <div class="uitpas_ui_activity_details_general_links"><?php print $links; ?></div>
  </div>
  <?php if ($image): ?>
    <div class="uitpas_ui_activity_details_image"><?php print $image; ?></div>
  <?php endif; ?>
  <?php if ($icons): ?>
    <div class="uitpas_ui_activity_details_legend"><?php print $icons; ?></div>
  <?php endif; ?>
  </div>
</div>