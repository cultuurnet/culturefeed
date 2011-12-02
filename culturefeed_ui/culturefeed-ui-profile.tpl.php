<?php if ($profile_edit_link) : ?>
  <?php print $profile_edit_link ?>
<?php endif; ?>

<?php if ($picture || $name || $gender || $dob || $bio || $city) : ?> 
  <?php if ($picture) : ?>
    <div class="profile-picture">
      <?php print $picture ?>
    </div>
  <?php endif; ?>
  
  <div class="profile-fields">   
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
      
      <?php if ($city) : ?>
        <div class="profile-field city"><?php print $city ?></div>
      <?php endif; ?>
  </div>
<?php else : ?>
<div class="no-profile"></div> 
<?php endif; ?>
<div class="clearfix"></div>