<?php if ($tooltip_text): ?>
  <div><?php print $tooltip_text; ?></div>
<?php endif; ?>

<ul class="list-unstyled">
<?php foreach ($items as $item): ?>
  <li><?php print $item; ?></li>
  <hr class="small" />
<?php endforeach; ?>
</ul>

<?php if ($show_more): ?>
  <div class="text-right"><a href="<?php print $more_url ?>"><?php print $more_text; ?></a></div>
<?php endif; ?>