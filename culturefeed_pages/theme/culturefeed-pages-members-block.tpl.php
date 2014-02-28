<div class="members-block">
  <h3><?php print $title; ?></h3>

  <?php if (!empty($members)): ?>

    <div class="membered-times"><?php print $num_followers ?> x</div>

    <ul>
    <?php foreach ($members as $member): ?>
      <li>
        <a href="<?php print $member['url'] ?>"><?php print $member['name']; ?></a>
        <?php if ($member['picture']): ?>
        <?php print theme('image', array('path' => $member['picture'] . '?maxwidth=100')) ?>
        <?php endif; ?>
      </li>
    <?php endforeach; ?>
    </ul>

  <?php else: ?>
    <p><?php print t('This page has no employees yet.'); ?></p>
  <?php endif; ?>

  <?php
  /**
   * .follow-PAGEID is used to refresh that part of the html. You can use it
   * freely as you want. E.g. wrap the text above in it or not.
   */
   ?>
  <div class="follow-<?php print $page->getId() ?>">
    <?php print $member_link ?>
  </div>

</div>