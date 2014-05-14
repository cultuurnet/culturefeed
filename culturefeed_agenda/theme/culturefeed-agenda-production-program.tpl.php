<div>
  <?php foreach ($tabs as $tabId => $tab): ?>
  <div>
    <div>
      <h3><?php print $tab['name']; ?></h3>
    </div>
    <div>
      <?php foreach ($tab['children'] as $content): ?>
      <div class="<?print $tab['class']; ?>">

        <a href="<?php print $content['url'] ?>"><?php print $content['title']; ?></a>
        <?php print $content['city']; ?>
        <?php print $content['venue']; ?>
        <?php print $content['calendar']; ?>
        <?php if (isset($content['all_url'])): ?>
        <a href="<?print $content['all_url']; ?>"><?php print t('Show all'); ?></a>
        <?php endif; ?>

      </div>
      <?php endforeach; ?>
    </div>
  </div>
  <?php endforeach; ?>
</div>
