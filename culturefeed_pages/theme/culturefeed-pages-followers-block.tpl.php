<div class="followers-block">
  <h3><?php print $title; ?></h3>

  <?php if (!empty($followers)): ?>

    <div class="followed-times"><?php print $num_followers ?> x</div>

    <ul>
    <?php foreach ($followers as $follower): ?>
      <li>
        <a href="<?php print $follower['url'] ?>"><?php print $follower['name']; ?></a>
        <?php if ($follower['picture']): ?>
        <?php print theme('image', array('path' => $follower['picture'] . '?maxwidth=100')) ?>
        <?php endif; ?>
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
  <div class="follow-<?php print $page->getId() ?>">
    <?php print $follow_link ?>
  </div>

</div>