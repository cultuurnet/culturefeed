<div class="activity-list">

  <ul class="media-list">
  <?php foreach ($items as $item): ?>
    <li class="media"><?php print $item;?></li>
  <?php endforeach; ?>
  </ul>

  <?php if ($pager_path): ?>
  <div class="more-pager">
    <a class="btn btn-warning pager-link" href="<?php print $pager_path; ?>" rel="no-follow">Lees oudere berichten</a>
  </div>
  <?php endif; ?>
  
</div>