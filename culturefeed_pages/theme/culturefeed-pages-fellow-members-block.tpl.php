<div class="colleague-block">
  <h3><?php print $title; ?></h3>

  <?php if (!empty($colleagues)): ?>
  <ul>
  <?php foreach ($colleagues as $colleague): ?>
    <li>
      <a href="<?php print $colleague['url'] ?>"><?php print $colleague['name']; ?></a>
      <?php if ($colleague['picture']): ?>
      <?php print theme('image', array('path' => $colleague['picture'] . '?maxwidth=100')) ?>
      <?php endif; ?>
    </li>
  <?php endforeach; ?>
  </ul>

  <?php else: ?>
    <p><?php print $nick; ?> <?php print t('is at the moment the only member of the page') . ' ' . $title ?>.</p>
  <?php endif; ?>

  <?php if ($is_member): ?>
  <p><?php print t('You are a member of') . ' ' . $title ?></p>
  <?php else: ?>
  <p><?php print t('Are you a colleague of') . ' ' . $nick . ' ' . t('of') . ' ' . $title ?>?</p>
  <a href="<?php print $become_member_url; ?>"><?php print t('Become a member'); ?></a>
  <?php endif; ?>

</div>