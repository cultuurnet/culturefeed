<?php if ($picture || $name || $gender || $dob || $bio || $city): ?>

<div class="uitid-contextual-links-wrapper">
  <a href="#" class="uitid-contextual-links-trigger"></a>
  <?php print $contextual_links; ?>
</div>

  <?php if ($picture): ?>
    <div class="profile-picture">
      <?php print $picture; ?>
    </div>
  <?php endif; ?>

  <div class="profile-fields">
      <?php if ($name) : ?>
        <div class="profile-field name"><?php print $name; ?></div>
      <?php endif; ?>

      <?php if ($nick): ?>
        <div class="profile-field name"><?php print $nick; ?></div>
      <?php endif; ?>

      <?php if ($gender): ?>
        <div class="profile-field gender"><?php print $gender; ?></div>
      <?php endif; ?>

      <?php if ($age): ?>
        <div class="profile-field dob"><?php print $age; ?></div>
      <?php endif; ?>

      <?php if ($city): ?>
        <div class="profile-field city"><?php print $city; ?></div>
      <?php endif; ?>

      <?php if ($profile_edit_link): ?>
        <?php print $profile_edit_link; ?>
      <?php endif; ?>
  </div>
<?php else : ?>
<div class="no-profile"></div>
<?php endif; ?>
<div class="clearfix"></div>

<div class="aantal-acties">
  Aantal acties
  <div class="tooltip">?</div>
  <div class="tooltip-text">Lorem ipsum</div>
</div>
<?php if ($like): ?>
  <div class="total-likes">
    <span><?php print $like; ?> X</span> vind ik leuk
  </div>
<?php endif; ?>

<?php if ($goto): ?>
  <div class="total-goto">
    <span><?php print $goto; ?> X</span> ga ik naar toe
  </div>
<?php endif; ?>
<?php if ($recommendations_link): ?>
  <?php print $recommendations_link; ?>
<?php endif; ?>

<div class="clearfix"></div>

<?php if ($facebook_privacy_toggle): ?>
  <?php print $facebook_privacy_toggle; ?>
<?php endif; ?>
