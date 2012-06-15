<div class="uitpas_ui_actor_details">
  <?php if ($icons): ?>
    <div class="uitpas_ui_actor_details_icons"><?php print $icons; ?></div>
  <?php endif; ?>
  <div class="uitpas_ui_actor_details_general">
    <div class="uitpas_ui_actor_details_general_shortdescription"><?php print $shortdescription; ?></div>
    <div class="uitpas_ui_actor_details_general_address"><?php print $address; ?></div>
    <div class="uitpas_ui_actor_details_general_contact"><?php print $contact; ?></div>
    <div class="uitpas_ui_actor_details_general_links"><?php print $links; ?></div>
  </div>
  <?php if ($image): ?>
    <div class="uitpas_ui_actor_details_image"><?php print $image; ?></div>
  <?php endif; ?>
  <?php if ($legend): ?>
    <div class="uitpas_ui_actor_details_legend"><?php print $legend; ?></div>
  <?php endif; ?>
</div>