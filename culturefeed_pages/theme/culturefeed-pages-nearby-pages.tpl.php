<?php if ($tooltip_text): ?>
  <div><?php print $tooltip_text; ?></div>
<?php endif; ?>

<?php foreach ($items as $item): ?>
  <div class="nearby-pages-row">
    <div>
      <img src="<?php print $item['image'] ?>" width="50" height="50" />
    </div>
    <div>
      <?php print $item['link']; ?><br />
      <small><?php print $item['location']; ?></small>
    </div>
  </div>
  <hr />
<?php endforeach; ?>

<?php if ($show_more): ?>
  <div><a href="<?php print $more_url ?>"><?php print $more_text; ?></a></div>
<?php endif; ?>