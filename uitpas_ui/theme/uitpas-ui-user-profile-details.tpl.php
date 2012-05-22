<div class="uitpas_ui_user_profile_details">
  <div class="uitpas_ui_user_profile_details_uitpas">
    <div><?php print $uitpas_number; ?></div>
  </div>
  <div class="uitpas_ui_user_profile_details_data">
    <div class="uitpas_ui_user_profile_details_intro"><?php print $intro; ?></div>
    <?php if ($image): ?>
      <div class="uitpas_ui_user_profile_details_image"><?php print $image; ?></div>
    <?php endif; ?>
    <div><?php print $national_identification_number; ?></div>
    <div><?php print $first_name; ?></div>
    <div><?php print $last_name; ?></div>
    <div><?php print $dob; ?></div>
    <div><?php print $pob; ?></div>
    <div><?php print $gender; ?></div>
    <div><?php print $nationality; ?></div>
    <div><?php print $street; ?></div>
    <div><?php print $nr; ?></div>
    <div><?php print $bus; ?></div>
    <div><?php print $zip; ?></div>
    <div><?php print $city; ?></div>
    <div><?php print $tel; ?></div>
    <div><?php print $mobile; ?></div>
    <div><?php print $email; ?></div>
    <?php if ($kansenStatuut && $kansenStatuutValidEndDate): ?>
      <div class="uitpas_ui_user_profile_details_status">
        <h3><?php print $status_title; ?></h3>
        <div><?php print $status_valid_till; ?></div>
        <?php if ($memberships): ?>
        <div><?php print $memberships; ?></div>
        <?php endif; ?>
      </div>
    <?php endif; ?>
  </div>
</div>