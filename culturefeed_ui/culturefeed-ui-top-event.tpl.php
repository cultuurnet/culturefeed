<h3><?php print $title ?></h3>

<?php print $thumbnail ?>

<div class="where">
  <span class="where-location"><?php print $location ?></span>
  <span class="where-city"><?php print $address ?></span>
</div>

<div class="when"><?php print $date ?></div>

<div class="more"><?php print $more ?></div>

<div class="actions">
  <?php if ($rate_link) : ?><?php print $rate_link ?><?php endif; ?>
</div>

<div class="clearfix"></div>