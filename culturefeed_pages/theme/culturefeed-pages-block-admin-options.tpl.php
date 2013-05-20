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
      Jouw pagina heeft nog geen activiteiten gepubliceerd.
      <a href="http://www.uitdatabank.be">Voeg nieuwe activiteiten toe.</a>
    </div>
  <?php endif; ?>

  <?php if (!empty($notifications)): ?>
  <h2>Laatste meldingen</h2>
  <?php print $notifications; ?>
  <?php endif; ?>
  </div>
  <div class="admin-menu">
    <?php print $admin_menu; ?>
  </div>

<?php else: ?>
  <div class="alert">
    <p>Je bent <strong>beheerder</strong> van deze pagina, maar <strong>niet aangemeld</strong> als deze pagina. Wil je <strong>verder werken als deze pagina</strong>?</p>
    <?php print $switch_link; ?>
  </div>
<?php endif; ?>
