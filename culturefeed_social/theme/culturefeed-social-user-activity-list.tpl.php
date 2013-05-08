<div class="activity-list">

  <ul class="media-list">
  <?php foreach ($items as $item): ?>
    <li class="media"><?php print $item;?></li>
  <?php endforeach; ?>
  </ul>

  <br />

  <?php if ($read_more_url): ?>
  <p>
    <a class="btn btn-warning pager-link" href="<?php print $read_more_url; ?>" rel="no-follow"><?php print $read_more_text; ?></a>
  </p>
  <?php endif; ?>

</div>