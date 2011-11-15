<h<?php print $heading_level ?><?php print $title_attributes; ?>><?php print $title ?></h<?php print $heading_level ?>>

<div class="when"><?php print $date ?></div>

<div class="where"><?php print $location ?></div>

<div class="more-link"><?php print $more ?></div>

<?php if ($like_link) : ?>
  <p><?php print $like_link ?></p>
<?php endif; ?>

<?php if ($remove_link) : ?>
  <p><?php print $remove_link ?></p>
<?php endif; ?>