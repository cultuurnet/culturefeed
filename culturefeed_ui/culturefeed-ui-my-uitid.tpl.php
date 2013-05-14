<?php if ($picture || $name || $gender || $dob || $bio || $city): ?>

<div class="uitid-contextual-links-wrapper">
  <a href="" class="uitid-contextual-links-trigger"></a>
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
      <?php elseif ($nick) : ?>
        <div class="profile-field nick"><?php print $nick; ?></div>
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
  <div class="tooltip-text">Hoe meer je laat weten of je een activiteit leuk vindt of naartoe gaat, hoe beter je persoonlijke tips zullen worden.</div>
</div>
<?php if ($like): ?>
  <div class="total-likes">
    <span class="number"><?php print $like; ?> x</span> <span>vind ik leuk</span>
  </div>
<?php endif; ?>

<?php if ($goto): ?>
  <div class="total-goto">
    <span class="number"><?php print $goto; ?> x</span> <span>ga ik naar toe</span>
  </div>
<?php endif; ?>

<?php if (!empty($memberships)): ?>
<div class="page-memberships">
  Pagina's waar ik lid van ben:
  <ul>
  <?php foreach ($memberships as $membership) :?>
    <li><?php print $membership; ?></li>
  <?php endforeach; ?>
  </ul>
</div>
<?php endif; ?>

<?php if (!empty($following)): ?>
<div class="following">
  Pagina's die ik volg:
  <ul>
  <?php foreach ($following as $following_page) :?>
    <li><?php print $following_page; ?></li>
  <?php endforeach; ?>
  </ul>
</div>
<?php endif; ?>

<div class="clearfix"></div>

<?php if ($facebook_privacy_toggle): ?>
  <?php print $facebook_privacy_toggle; ?>
<?php endif; ?>
