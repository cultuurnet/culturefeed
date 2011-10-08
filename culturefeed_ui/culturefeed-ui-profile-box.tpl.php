<?php if ($is_logged_in) : ?>

  <?php print $picture ?>

  <p>
    <strong><?php print $nick ?></strong><br />
    <?php print $link_profile ?><br />
    <?php print $link_logout ?>
  </p>

<?php else : ?>

  <?php print $link_login ?><br />
  <?php print $link_register ?>

<?php endif; ?>