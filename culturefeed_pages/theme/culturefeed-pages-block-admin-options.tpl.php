<?php
/**
 * @file
 * Template file for the administrative options for a page.
 *
 * @vars
 * - $account The DrupalCultureFeed loggedin user
 * - $page The Page object
 * - $is_page_admin is Page Admin,
 * - $logged_in_as_page_admin Logged In As Page Admin
 *
 */
?>

<?php if ($logged_in_as_page_admin): ?>

  <div>
  <?php if (!$has_activities): ?>
    <div class="alert">
      <?php print t('Your page has published no activities yet.'); ?>
      <a href="http://www.uitdatabank.be"><?php print t('Add new activities.'); ?></a>
    </div>
  <?php endif; ?>

  <?php if (!empty($notifications)): ?>
  <h2><?php print t('Latest notifications'); ?></h2>
  <?php print $notifications; ?>
  <?php endif; ?>
  </div>
  <div class="admin-menu">
    <?php print $admin_menu; ?>
  </div>

<?php else: ?>
  <div class="alert">
    <p><?php print t('You are <strong> manager</strong> on this page, but <strong>not logged in</strong> as this page. Do you want to continue working as <strong>this page</strong>?'); ?></p>
    <?php print $switch_link; ?>
  </div>
<?php endif; ?>
