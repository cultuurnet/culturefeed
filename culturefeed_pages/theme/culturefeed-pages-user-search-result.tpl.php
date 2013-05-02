<?php if ($total == 0) : ?>
<div class="no-results">Geen gebruikers gevonden</div>
<?php else: ?>

  <ul>
  <?php foreach ($results as $result): ?>
    <li>
      <?php print $result['nick']; ?>
      <?php print $result['profile_link']; ?>
      <?php print $result['add_link']; ?>
    </li>
  <?php endforeach; ?>
  </ul>

<?php endif; ?>

<div id="page_confirm" class="modal hide fade" tabindex="-1" role="dialog" aria-hidden="true">
  <div class="modal-body"></div>
</div>