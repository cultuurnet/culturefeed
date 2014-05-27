<h3><?php print $title; ?></h3>
<?php if ($picture || $name || $gender || $age || $bio || $city) : ?>
  <p>
  <?php if ($picture) : ?>
    <?php print $picture ?>
  <?php endif; ?>
  </p>
  
  <ul>
    <?php if ($name) : ?>
      <li><?php print $name ?></li>
    <?php endif; ?>
  
    <?php if ($gender) : ?>
      <li><?php print $gender ?></li>
    <?php endif; ?>
  
    <?php if ($age) : ?>
     <li><?php print $age?> jaar</li>
    <?php endif; ?>
  
    <?php if ($bio) : ?>
     <li><?php print $bio ?></li>
    <?php endif; ?>
  
    <?php if ($city) : ?>
     <li><?php print $city ?></li>
    <?php endif; ?>
  </ul>
  
<?php else : ?>
  <p><?php print t('No profile data available'); ?>. <a href="/culturefeed/profile/edit"><?php print t('Complete your profile'); ?></a></p>
<?php endif; ?>

<hr />
