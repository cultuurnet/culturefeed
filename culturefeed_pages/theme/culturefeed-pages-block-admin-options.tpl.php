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

  <div>
  <?php if (!$has_activities): ?>
    <div class="alert">
      Jou pagina heeft nog geen activiteiten gepubliceerd.
      <a href="http://www.uitdatabank.be">Voeg nieuwe activiteiten toe.</a>
    </div>
  <?php endif; ?>

  <?php if ($notifications): ?>
  <h2>Laatste meldingen</h2>
  <?php print $notifications; ?>
  <?php endif; ?>
  </div>
  <div class="admin-menu">
    <?php print $admin_menu; ?>
  </div>

<?php elseif($is_page_admin): ?>
  Je bent <strong>Beheerder</strong> van deze pagina, maar <strong>niet aangemeld</strong> als deze pagina. Wil je <strong>verder werken als deze pagina</strong>?
  <?php print $switch_link; ?>
<?php endif; ?>
