<?php
/**
 * @file 
 * Template file for the list of wishlist items.
 */
?>

<?php if (!empty($items)): ?>
  <h2>Je koos voor</h2>

  <table class="table table-striped">
    <thead>
      <tr>
        <th>Titel</th>
        <th>Aantal</th>
        <th>Verwijder</th>
      </tr>
    </thead>
    <tbody>
      <?php foreach ($items as $item): ?>
        <?php print $item ?>
      <?php endforeach;?>
    </tbody>
  </table>
<?php else: ?>
  <div class="alert">
    <span>Je hebt nog geen items geselecteerd.</span>
  </div>
<?php endif; ?>