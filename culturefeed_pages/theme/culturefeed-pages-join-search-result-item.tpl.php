<strong><?php print $title; ?></strong><br/>
<?php if (!empty($location)): ?>
<?php print $location['street']?> <?php print $location['city'] ?>
<?php endif; ?>

<?php if (!empty($become_member_link)): ?>
<?php print $become_member_link; ?>
<?php else: ?>
<?php print t('You are already a member'); ?>
<?php endif; ?>

<?php if (!empty($follow_link)): ?>
<?php print $follow_link; ?>
<?php else: ?>
<?php print t('You follow this page'); ?>
<?php endif; ?>

<?php print $view_page_link; ?>

