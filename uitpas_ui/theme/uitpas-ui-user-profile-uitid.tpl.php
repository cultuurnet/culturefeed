<div class="uitpas_ui_user_profile_uitid">
  <?php if ($image): ?>
    <div class="uitpas_ui_user_profile_uitid_image"><?php print $image; ?></div>
  <?php endif; ?>
  <div class="uitpas_ui_user_profile_uitid_details">
    <div class="uitpas_ui_user_profile_uitid_details_first_name"><?php print $first_name; ?></div>
    <div class="uitpas_ui_user_profile_uitid_details_gender"><?php print $gender; ?></div>
    <div class="uitpas_ui_user_profile_uitid_details_dob"><?php print $dob; ?></div>
    <div class="uitpas_ui_user_profile_uitid_details_pob"><?php print $pob; ?></div>
    <div class="uitpas_ui_user_profile_uitid_details_more_info"><?php print $more_info; ?></div>
  </div>
  <div class="uitpas_ui_user_profile_uitid_details">
    <?php print $actions; ?>
  </div>
  <div class="uitpas_ui_user_profile_uitid_form">
    <?php print $form; ?>
  </div>
</div>