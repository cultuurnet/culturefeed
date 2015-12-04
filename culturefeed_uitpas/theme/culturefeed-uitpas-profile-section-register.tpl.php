<?php

/**
 * @file
 * Contains culturefeed-uitpas-profile-section-register.tpl.php.
 */

?>

<p>
  <?php echo t("Holders of an UiTPAS can earn points by participating in leisure activities and exchange them for benefits."); ?>
</p>

<ul class="links">
  <li><a href="/register_where"><?php echo t("Find out where to get an UiTPAS"); ?></a></li>
  <li><a href="/promotions"><?php echo t("Promotions"); ?></a></li>
  <li>
    <a href="/authenticated?destination=register_uitpas">
      <strong>
      <?php echo t("Already an UiTPAS?"); ?>
      </strong><br/>
      <?php echo t("Register your UiTPAS online"); ?>
    </a>
  </li>
</ul>
