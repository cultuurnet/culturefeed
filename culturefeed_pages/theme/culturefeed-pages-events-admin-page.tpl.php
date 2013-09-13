<div id="view-page"><?php print $view_page_link; ?></div>

<?php if (!empty($items)): ?>

  <?php print $items; ?>

  <?php print theme('pager') ?>

<?php else: ?>
  <div class="no-results"></div>
<?php endif; ?>

<div class="info">
  <?php print t('This list shows only the activities that are currently online. You can find past or unpublished activities on <a href="http://www.uitdatabank.be"> UiT database </ a>.'); ?>
</div>

<div class="new-event">
  <h2><?php print t('Add a new activity'); ?></h2>
  <p>
    <?php print t('Add new activities with the UiTdatabank. You can sign in with the same account and add activities immediately. <a href="http://www.uitdatabank.be/">Add new activity</a>.'); ?>
  </p>
</div>