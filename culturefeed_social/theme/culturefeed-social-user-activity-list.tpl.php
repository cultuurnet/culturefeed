<div class="activity-list">

  <div class="items">
    <ul>
    <?php foreach ($items as $item): ?>
      <li><?php print $item;?></li>
    <?php endforeach; ?>
    </ul>
  </div>

  <?php if ($pager_path): ?>
  <div class="more-pager">
    <a class="pager-link" href="<?php print $pager_path; ?>" rel="no-follow">Lees oudere berichten</a>
  </div>
  <?php endif; ?>

</div>