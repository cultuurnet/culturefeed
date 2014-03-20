<h5><?php print $title; ?></h5>

<?php if (!empty($members)): ?>
  <p><?php print $num_members ?> <?php print t('members'); ?> - <?php print $member_link ?></p>

  <ul>
  <?php foreach ($members as $member): ?>
    <li>
      <a href="<?php print $member['url'] ?>"><?php print $member['name']; ?></a>
    </li>
  <?php endforeach; ?>
  </ul>

<?php else: ?>
  <p><?php print t('This page has no members yet.'); ?></p>
<?php endif; ?>


<?php
/**
 * .follow-PAGEID is used to refresh that part of the html. You can use it
 * freely as you want. E.g. wrap the text above in it or not.
 */
 ?>
 