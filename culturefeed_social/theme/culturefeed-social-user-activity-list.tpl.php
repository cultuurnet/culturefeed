<div class="activity-list">

  <ul class="media-list">
  <?php foreach ($items as $item): ?>
    <li class="media"><?php print $item;?></li>
  <?php endforeach; ?>
  </ul>

  <br />

  <?php if ($pager_path): ?>
  <p>
    <a class="btn btn-warning pager-link" href="<?php print $pager_path; ?>" rel="no-follow">Lees oudere berichten</a>
  </p>
  <?php endif; ?>
  
</div>