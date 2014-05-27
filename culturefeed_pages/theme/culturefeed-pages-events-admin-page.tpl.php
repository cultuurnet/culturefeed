<h3><?php print t('Activities'); ?></h3>
<p><?php print $view_page_link; ?></p>

<?php if (!empty($items)): ?>

  <p><?php print t('The <strong>list</strong> below shows all the <strong>activities</strong> that are <strong>currently published</strong> on your page.') ?>
  <br />
  <?php print t('Past or unpublished activities can be consulted via <a href="http://www.uitdatabank.be" target="_blank"> UiTdatabank </a>.'); ?></p>

  <?php print $items; ?>

  <?php print theme('pager') ?>

<?php else: ?>
    <p><?php print t('There are currently no published activities available for your page. Past or unpublished activities can be consulted via <a href="http://www.uitdatabank.be" target="_blank"> UiTdatabank </a>.') ?></p>
<?php endif; ?>

<p><a href="http://www.uitdatabank.be" target="_blank"><?php print t('Add a new activity via UiTdatabank'); ?></a> (<?php print t('You can sign in with your current username and password.'); ?>)</p>