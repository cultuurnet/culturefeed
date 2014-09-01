<?php
/**
 * @file
 * Template file for comment teasers when shown in streams.
 */
?>

<div id="activity-<?php print $activity_id ?>" class="comment-list-item">

  <?php print $summary; ?>
  <?php print $body; ?>

  <?php if (!empty($comment_link)): ?>
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
