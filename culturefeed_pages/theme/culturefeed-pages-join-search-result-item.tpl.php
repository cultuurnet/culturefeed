<strong><?php print $title; ?></strong><br/>
<?php if (!empty($location)): ?>
<?php print $location['street']?> <?php print $location['city'] ?>
<?php endif; ?>

<?php if (!empty($become_member_link)): ?>
<?php print $become_member_link; ?>
<?php else: ?>
U bent reeds lid
<?php endif; ?>
<?php print $view_page_link; ?>

