<h5><?php print $title; ?></h5>

<?php if (!empty($followers)): ?>
  <p><?php print $num_followers ?> <?php print t('followers'); ?> - <?php print $follow_link ?></p>

  <ul>
  <?php foreach ($followers as $follower): ?>
    <li>
      <a href="<?php print $follower['url'] ?>"><?php print $follower['name']; ?></a>
    </li>
  <?php endforeach; ?>
  </ul>

<?php else: ?>
  <p><?php print t('This page has no followers yet.'); ?></p>
<?php endif; ?>


<?php
/**
 * .follow-PAGEID is used to refresh that part of the html. You can use it
 * freely as you want. E.g. wrap the text above in it or not.
 */
 ?>
