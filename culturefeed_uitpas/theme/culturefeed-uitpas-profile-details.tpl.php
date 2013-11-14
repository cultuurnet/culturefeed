<?php

/**
 * @file
 * Default theme implementation to display culturefeed uitpas profile details.
 *
 * Available variables:
 * - $uitpas_number.
 * - $intro.
 * - $intro.
 * - $national_identification_number.
 * - $first_name.
 * - $last_name.
 * - $dob.
 * - $pob.
 * - $gender.
 * - $nationality.
 * - $street.
 * - $zip.
 * - $city.
 * - $tel.
 * - $mobile.
 * - $email.
 * - $kansenStatuut.
 * - $kansenStatuutValidEndDate.
 * - $status_title.
 * - $status_valid_till.
 * - $memberships.
 * - $outro.
 */
?>
<div class="profile_details">
  <div class="uitpas">
    <div><?php print $uitpas_number; ?></div>
  </div>
  <div class="data">
    <?php if ($intro): ?>
    <div class="intro"><?php print $intro; ?></div>
    <?php endif; ?>
    <div><?php print $national_identification_number; ?></div>
    <div><?php print $first_name; ?></div>
    <div><?php print $last_name; ?></div>
    <div><?php print $dob; ?></div>
    <div><?php print $pob; ?></div>
    <div><?php print $gender; ?></div>
    <div><?php print $nationality; ?></div>
    <div><?php print $street; ?></div>
    <div><?php print $zip; ?></div>
    <div><?php print $city; ?></div>
    <div><?php print $tel; ?></div>
    <div><?php print $mobile; ?></div>
    <div><?php print $email; ?></div>
    <?php if ($kansen_statuut && $kansen_statuut_valid_end_date): ?>
    <div class="status">
      <h3><?php print $status_title; ?></h3>
      <div><?php print $status_valid_till; ?></div>
      <?php if ($memberships): ?>
      <div><?php print $memberships; ?></div>
      <?php endif; ?>
    </div>
    <?php endif; ?>
    <?php if ($outro): ?>
    <div class="outro"><?php print $outro; ?></div>
    <?php endif; ?>
  </div>
</div>
