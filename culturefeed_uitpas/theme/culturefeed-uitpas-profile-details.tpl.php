<?php

/**
 * @file
 * Default theme implementation to display culturefeed uitpas profile details.
 *
 * Available variables:
 * - $uitpas_number.
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
 * - $outro.
 */
?>
  
<p class="uitpas_number"><?php print $uitpas_number; ?></p>
<?php if ($intro): ?>

<p class="intro"><?php if ($intro): ?></p>
<?php endif; ?>

<ul class="data">
  <li><?php print $first_name; ?></li>
  <li><?php print $last_name; ?></li>
  <li><?php print $dob; ?></li>
  <li><?php print $pob; ?></li>
  <li><?php print $gender; ?></li>
  <li><?php print $nationality; ?></li>
  <li><?php print $street; ?></li>
  <li><?php print $zip; ?></li>
  <li><?php print $city; ?></li>
  <li><?php print $tel; ?></li>
  <li><?php print $mobile; ?></li>
  <li><?php print $email; ?></li>
</ul>

<?php if ($kansen_statuut && $kansen_statuut_valid_end_date): ?>
<div class="status">
  <h3><?php print $status_title; ?></h3>
  <p><?php print $status_valid_till; ?></p>
  <?php if ($memberships): ?>
  <p><?php print $memberships; ?></p>
  <?php endif; ?>
</div>
<?php endif; ?>

<?php if ($outro): ?>
<p><?php if ($outro): ?></p>
<?php endif; ?>
