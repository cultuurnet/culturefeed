<?php

/**
 * @file
 * Contains culturefeed-uitpas-profile-section-register.tpl.php.
 */

?>

<?php if(culturefeed_uitpas_not_yet_registered()): ?>

  <h5><?php echo t('You did not register your UiTPAS yet.'); ?></h5>

  <p><?php echo t('Register and...'); ?></p>

  <ul>
  <li><?php echo t('consult your benefits') ?></li>
  <li><?php echo t('check points balance') ?></li>
  <li><?php echo t('access your information') ?></li>
  </ul>

  <p>
    <a href="/register_uitpas" class="btn btn-primary btn-block">
    <?php echo t('Register your UiTPAS') ?>
    </a>
  </p>

<?php else: ?>

  <p>
    <?php echo t('Holders of an UiTPAS can earn points by participating in leisure activities and exchange them for ') . ' ' . l(t('nice benefits'), 'promotions') . '.'; ?>
  </p>

  <p>
    <a href="/register_where" class="btn btn-default btn-block">
      <?php echo t('Get an UiTPAS'); ?>
    </a>
  </p>

<?php endif; ?>
