<?php if (!empty($image)): ?>
    <a href="<?php print $url; ?>"><img src="<?php print $image; ?>?width=75&height=75&crop=auto" /></a>
<?php endif; ?>

<a href="<?php print $url; ?>"><?php print $title; ?></a>
<?php if (!empty($address['city'])): ?>
  <?php print $address['city']; ?>
<?php endif; ?>
<br/>

<?php if (!empty($address)): ?>
  <?php print $address['street'] ?><?php if (!empty($address['city'])): ?><?php print $address['city'] ?><?php endif; ?>
  <br/>
<?php endif; ?>

<?php print $description ?>

<?php if (!empty($become_member_url)): ?>
  <a href="<?php print $become_member_url; ?>"><?php print t('Become a member') ?></a>
<?php else: ?>
<?php print t('You are already a member'); ?>
<?php endif; ?>

<?php if (!empty($follow_url)): ?>
    <a href="<?php print $follow_url; ?>"><?php print $follow_text; ?></a>
<?php else: ?>
  <?php print t('You follow this page'); ?>
<?php endif; ?>