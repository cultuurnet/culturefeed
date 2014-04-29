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
  <p><?php print t('Posted by'); ?> <?php print $author ?> <span><?php print t('on'); ?> <?php print $date ?></span>.</p>

  <div>
    <?php if ($level == 0): ?>
      <?php print $comment_link ?>
    <div id="comment-wrapper-<?php print $activity->id ?>" class="modal hide fade" tabindex="-1" role="dialog" aria-hidden="true">
      <div class="modal-body outer"></div>
    </div>
    <?php endif; ?>
    <?php if ($abuse_link): ?>
      <?php print $abuse_link ?>
    <div id="abuse-wrapper-<?php print $activity->id ?>" class="modal hide fade" tabindex="-1" role="dialog" aria-hidden="true">
      <div class="modal-body"></div>
        </div>
    <?php endif; ?>

    <?php if ($delete_link): ?>
    <?php print $delete_link ?>
    <div id="delete-wrapper-<?php print $activity->id ?>" class="modal hide fade" tabindex="-1" role="dialog" aria-hidden="true">
      <div class="modal-body outer"></div>
        </div>
    <?php endif; ?>
  </div>

  <?php if (!empty($list)): ?>
    <div style="margin-left: 30px;">
    <?php foreach ($list as $list_item): ?>
    <?php print $list_item ?>
    <?php endforeach;?>
    </div>
  <?php endif; ?>
</div>
