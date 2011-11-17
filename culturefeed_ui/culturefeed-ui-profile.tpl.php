<?php if ($profile_edit_link) : ?>
  <?php print $profile_edit_link ?>
<?php endif; ?>

<?php if ($name || $gender || $dob || $bio) : ?> 
  <?php if ($picture) : ?>
    <div class="profile-picture">
      <?php print $picture ?>
      <?php if ($picture_change_link) : ?>
        <?php print $picture_change_link ?>
      <?php endif; ?>
    </div>
  <?php endif; ?>
  
  <div class="profile-fields">
    <?php if ($name || $gender || $dob || $bio) : ?>    
      <?php if ($name) : ?>
        <div class="profile-field name"><?php print $name ?></div>
      <?php endif; ?>
      
      <?php if ($gender) : ?>
        <div class="profile-field gender"><?php print $gender ?></div>
      <?php endif; ?>
      
      <?php if ($dob) : ?>
        <div class="profile-field dob"><?php print $dob?></div>
      <?php endif; ?>
      
      <?php if ($bio) : ?>
        <div class="profile-field bio"><?php print $bio ?></div>
      <?php endif; ?>
    <?php else : ?>
      <div class="no-public-profile-fields">
        Het profiel van <span class="nick"><?php print $nick ?></span> is niet publiek beschikbaar.
      </div>
    <?php endif; ?>
  </div>
<?php else : ?>
<div class="no-profile"></div> 
<?php endif; ?>
<div class="clearfix"></div>