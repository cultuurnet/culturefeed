<?php

/**
 * @file
 * Default theme implementation to display culturefeed uitpas profile details.
 *
 * Available variables:
 * - $uitpas_numbers_title.
 * - $uitpas_numbers.
 * - $intro.
 * - $intro.
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
 */
?>

<div class="profile_uitpas">

  <?php if ($intro): ?>
    <p class="intro"><?php print $intro; ?></p>
  <?php endif; ?>

  <p class="uitpas_numbers"><?php print $uitpas_numbers; ?></p>

  <div class="details">
    <h3><?php print $form_title; ?></h3>
    <?php if ($form_intro): ?>
      <div class="form-intro"><?php print $form_intro; ?></div>
    <?php endif; ?>
    <?php print $form; ?>
  </div>

  <?php if ($memberships): ?>
    <div class="status">
      <h3><?php print $status_title; ?></h3>
      <?php if ($memberships): ?>
      <p><?php print $memberships; ?></p>
      <?php endif; ?>
    </div>
  <?php endif; ?>

</div>
