<div id="view-page"><?php print $view_page_link; ?></div>

<?php if (!empty($items)): ?>

  <?php print $items; ?>

  <?php print theme('pager') ?>

<?php else: ?>
  <div class="no-results">Je pagina heeft nog geen activiteiten</div>
<?php endif; ?>

<div class="info">
  Deze lijst toont enkel de activiteiten die momenteel online staan. Afgelopen of niet gepubliceerde activeiten kan je raadplegen via de <a href="http://www.uitdatabank.be">Uitdatabank</a>.
</div>

<div class="new-event">
  <h2>Een nieuwe activiteit toevoegen</h2>
  <p>
    Dat kan via de Uitdatabank. Je kan er aanmelden met dezelfde account als deze om onmiddelijk aan de slag te gaan.
    <a href="http://www.uitdatabank.be/">Voeg nieuwe activiteit toe.</a>
  </p>
</div>