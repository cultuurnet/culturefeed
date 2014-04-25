<?php if ($tooltip_text): ?>
  <div><?php print $tooltip_text; ?></div>
<?php endif; ?>

<?php foreach ($items as $item): ?>
  <div class="nearby-pages-row">
    <?php if ($item['image']): ?>
      <div>
        <img src="<?php print $item['image'] ?>?width=50&height=50&crop=auto" width="50" height="50" />
      </div>
    <?php endif; ?>
    <div>
      <?php print $item['link']; ?><br />
      <small><?php print $item['location']; ?></small>
    </div>
  </div>
  <br />
<?php endforeach; ?>

<?php if ($show_more): ?>
  <div><a href="<?php print $more_url ?>"><?php print $more_text; ?></a></div>
<?php endif; ?>