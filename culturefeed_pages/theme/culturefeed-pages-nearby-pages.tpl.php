<?php if ($tooltip_text): ?>
  <div><?php print $tooltip_text; ?></div>
<?php endif; ?>

<?php foreach ($items as $item): ?>
  <div class="row nearby-pages-row">
    <div class="col-xs-3">
      <img src="<?php print $item['image'] ?>" class="img-responsive" />
    </div>
    <div class="col-xs-9">
      <?php print $item['link']; ?><br />
      <small class="text-muted"><?php print $item['location']; ?></small>
    </div>
  </div>
  <hr class="small" />
<?php endforeach; ?>

<?php if ($show_more): ?>
  <div class="text-right"><a href="<?php print $more_url ?>"><?php print $more_text; ?></a></div>
<?php endif; ?>