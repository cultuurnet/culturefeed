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

<?php if ($is_page_admin && $logged_in_as_page_admin): ?>
  <?php print $admin_menu; ?>
  <?php elseif($is_page_admin): ?>
Je bent <strong>Beheerder</strong> van deze pagina, maar <strong>niet aangemeld</strong> als deze pagina. Wil je <strong>verder werken als deze pagina</strong>?
<?php print $switch_link; ?>
<?php endif; ?>
