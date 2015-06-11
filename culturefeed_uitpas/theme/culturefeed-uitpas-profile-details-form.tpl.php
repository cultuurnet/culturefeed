<?php

/**
 * @file
 * Default theme implementation to display culturefeed uitpas profile details form.
 */

?>

<div class="details-form">

  <div class="clearfix form-row">
    <?php print $first_name; ?>
    <?php print $last_name; ?>
  </div>
  <div class="clearfix form-row">
    <?php print $dob; ?>
    <?php print $pob; ?>
  </div>
  <div class="clearfix form-row">
    <?php print $gender; ?>
    <?php print $nationality; ?>
  </div>
  <div class="clearfix form-row">
    <?php print $street; ?>
    <?php print $nr; ?>
  </div>
  <div class="clearfix form-row">
    <?php print $zip; ?>
    <?php print $city; ?>
  </div>
  <div class="clearfix form-row">
    <?php print $tel; ?>
    <div class="form-grouped">
      <?php print $mobile; ?>
      <?php print $mobile_notifications; ?>
    </div>
  </div>
  <div class="clearfix form-row">
    <div class="form-grouped">
      <?php print $email; ?>
      <?php print $email_notifications; ?>
    </div>
    <?php print $email_description; ?>
  </div>
  <div class="clearfix form-row">
    <?php print $main_form; ?>
    <?php print $actions; ?>
  </div>

</div>
