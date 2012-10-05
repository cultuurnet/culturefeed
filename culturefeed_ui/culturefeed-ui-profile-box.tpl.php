<?php if ($is_logged_in) : ?>

  <div class="image">
    <?php print $picture ?>
  </div>

  <div class="options">
    <ul>
      <li><strong><?php print $nick ?></strong></li>
      <li><?php print $link_profile ?></li>
      <li><?php print $link_logout ?></li>
    </ul>
  </div>

<?php else : ?>

  <div class="options">
    <ul>
      <li><?php print $link_login ?></li>
      <li><?php print $link_register ?></li>
      <li><?php print $link_login_facebook ?></li>
    </ul>
  </div>

<?php endif; ?>
