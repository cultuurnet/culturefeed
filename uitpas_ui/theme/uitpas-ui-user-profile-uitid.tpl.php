<div class="uitpas_ui_user_profile_uitid">
  <?php if ($image): ?>
    <div class="uitpas_ui_user_profile_uitid_image"><?php print $image; ?></div>
  <?php endif; ?>
  <div class="uitpas_ui_user_profile_uitid_details">
    <div class="uitpas_ui_user_profile_uitid_details_first_name"><strong><?php print $first_name; ?></strong></div>
    <div class="uitpas_ui_user_profile_uitid_details_gender"><?php print $gender; ?></div>
    <div class="uitpas_ui_user_profile_uitid_details_city"><?php print $city; ?></div>
    <div class="uitpas_ui_user_profile_uitid_details_bio"><?php print $bio; ?></div>
    <div class="uitpas_edit_uitid">
      <?php print l('Beheer je UiTiD-profiel','http://www.uitinvlaanderen.be/authenticated?destination=culturefeed/profile/edit') ?>
      <span class="small">(via UiTinVlaanderen.be)</span>
    </div>
  </div>
  <div class="uitpas_ui_user_profile_uitid_actions">
    <?php print $actions; ?>
  </div>
  <div class="uitpas_ui_user_profile_uitid_form">
    <?php print $form; ?>
  </div>
</div>
