<?php
/**
 * @file
 * Template for the summary of a page.
 */
?>

<div class="row">
  <div class="col-md-4 col-sm-4">
    <?php if (!empty($thumbnail)): ?>
      <img src="<?php print $thumbnail; ?>?width=80&height=80&crop=auto" />
    <?php endif; ?>
  </div>

  <div class="col-md-8 col-sm-8">
    <h4 class="media-heading"><a href="<?php print $url ?>"><?php print $title; ?></a></h4>

    <?php if (isset($location['city'])): ?>
    <div class="row"><span class=text-muted"><?php print $location['city']; ?></span></div>
    <?php endif;?>

    <?php if (isset($when)): ?>
      <div class="row"><?php print $when; ?></div>
    <?php endif;?>
  </div>
</div>
