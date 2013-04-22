<div class="colleague-block">
  <h3><?php print $title; ?></h3>

  <?php if (!empty($colleagues)): ?>
  <ul>
  <?php foreach ($colleagues as $colleague): ?>
    <li>
      <a href="<?print $colleague['url'] ?>"><?php print $colleague['name']; ?></a>
      <?php if ($colleague['picture']): ?>
      <?php print theme('image', array('path' => $colleague['picture'] . '?maxwidth=100')) ?>
      <?php endif; ?>
    </li>
  <?php endforeach; ?>
  </ul>

  <?php else: ?>
    <p><?php print $nick; ?> is momenteel het enige lid van pagina <?php print $title ?>.</p>
  <?php endif; ?>

  <?php if ($is_member): ?>
  <p>Je bent lid van <?php print $title ?></p>
  <?php else: ?>
  <p>Ben jij een collega van <?php print $nick; ?> bij <?php print $title ?>?</p>
  <a href="<?php print $become_member_url; ?>">Word lid</a>
  <?php endif; ?>


</div>