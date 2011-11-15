<h<?php print $heading_level ?><?php print $title_attributes; ?>><?php print $title ?></h<?php print $heading_level ?>>

<p><?php print $date ?></p>

<p><?php print $location ?></p>

<p><?php print $more ?></p>

<?php if ($like_link) : ?>
  <p><?php print $like_link ?></p>
<?php endif; ?>

<?php if ($remove_link) : ?>
  <p><?php print $remove_link ?></p>
<?php endif; ?>