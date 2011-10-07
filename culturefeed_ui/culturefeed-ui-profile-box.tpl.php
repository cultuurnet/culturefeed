<?php if ($is_logged_in) : ?>

  <?php print $picture ?>

  <?php print $nick ?>

  <?php print $link_profile ?>

  <?php print $link_logout ?>

<?php else : ?>

  <?php print $link_login ?>
  <?php print $link_register ?>

<?php endif; ?>