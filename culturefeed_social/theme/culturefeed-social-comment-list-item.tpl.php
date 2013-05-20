<?php
/**
 * @vars
 *   $activity
 *   $content
 *   $picture
 *   $date
 *   $author
 *   $activity_id
 *   $level (0 of 1)
 */
?>

<div id="activity-<?php print $activity_id ?>" class="comment-list-item">
  <?php print $picture ?>
  <?php print $content ?>
  <p>Geplaatst door <?php print $author ?> <span>op <?php print $date ?></span>.</p>

  <div>
    <?php if ($level == 0): ?>
    <?php print $react_link ?>
    <?php endif; ?>
    <?php print $abuse_link ?>
    <?php print $delete_link ?>
  </div>

  <?php if ($level == 0) : ?>
  <?php print $react_form ?>
  <?php endif; ?>

  <?php print $abuse_form; ?>

  <?php if (!empty($list)): ?>
    <div style="margin-left: 30px;">
    <?php foreach ($list as $list_item): ?>
    <?php print $list_item ?>
    <?php endforeach;?>
    </div>
  <?php endif; ?>
</div>
