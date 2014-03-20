<?php if ($picture || $name || $gender || $dob || $bio || $city): ?>

<div class="uitid-contextual-links-wrapper">
  <a href="" class="uitid-contextual-links-trigger"></a>
  <?php print $contextual_links; ?>
</div>

<?php if ($picture): ?>
    <?php print $picture; ?>
<?php endif; ?>

<ul>
<?php if ($name) : ?>
  <li><?php print $name; ?></li>
<?php elseif ($nick) : ?>
  <li><?php print $nick; ?></li>
<?php endif; ?>

<?php if ($gender): ?>
  <li><?php print $gender; ?></li>
<?php endif; ?>

<?php if ($age): ?>
  <li><?php print $age; ?></li>
<?php endif; ?>

<?php if ($city): ?>
 <li><?php print $city; ?></li>
<?php endif; ?>
</ul>

<?php else : ?>

<?php endif; ?>

<hr />

<h3><?php print t('My actions') ?></h3>
<p><?php print t('The more items you like or events you attend, the more tips will have a personal touch') ?></p>

<ul>

<?php if ($like): ?>
<li><?php print $like; ?> x <?php print t('likes') ?></li>
<?php endif; ?>

<?php if ($goto): ?>
<li><?php print $goto; ?> x <?php print t('attends') ?></li>
<?php endif; ?>

<?php if (!empty($memberships)): ?>
<li>
  <?php print ($view_own_page ? t('Pages of which i\'m a member') : t('Pages of which @username is a member', array('@username' => $user->nick))); ?>  <ul>
  <?php foreach ($memberships as $membership) :?>
    <li><?php print $membership; ?></li>
  <?php endforeach; ?>
  </ul>
</li>
<?php endif; ?>

<?php if (!empty($following)): ?>

<li>
<?php print t('Pages I follow') ?>
  <ul>
  <?php foreach ($following as $following_page) :?>
    <li><?php print $following_page; ?></li>
  <?php endforeach; ?>
  </ul>
</li>
<?php endif; ?>
</ul>

<hr />

<?php if ($facebook_privacy_toggle): ?>
  <?php print $facebook_privacy_toggle; ?>
<?php endif; ?>
