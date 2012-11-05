<h3><?php print $title ?></h3>

<div class="image"><?php print $thumbnail ?></div>

<div class="when"><?php print $date ?></div>

<div class="where">
  <span class="where-location"><?php print $location ?></span>
  <span class="where-city"><?php print $city ?></span>
</div>

<div class="more"><?php print $more ?></div>

<div class="actions">
  <?php if ($rate_link) : ?><div class="like"><?php print $rate_link ?></div><?php endif; ?>
</div>

<div class="clearfix"></div>