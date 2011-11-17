<?php if ($picture) : ?>
  <div class="profile-picture">
    <?php print $picture ?>
    <?php if ($picture_change_link) : ?>
      <?php print $picture_change_link ?>
    <?php endif; ?>
  </div>
<?php endif; ?>

<dl>

  <?php if ($name) : ?>
    <dt>Naam</dt>
    <dd><?php print $name ?></dd>
  <?php endif; ?>

  <?php if ($gender) : ?>
    <dt>Geslacht</dt>
    <dd><?php print $gender ?></dd>
  <?php endif; ?>

  <?php if ($dob) : ?>
    <dt>Geboortedatum</dt>
    <dd><?php print $dob?></dd>
  <?php endif; ?>

  <?php if ($bio) : ?>
    <dt>Bio</dt>
    <dd><?php print $bio ?></dd>
  <?php endif; ?>

</dl>