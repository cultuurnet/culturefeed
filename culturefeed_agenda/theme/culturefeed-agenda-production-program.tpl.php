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

        <?php if (!empty($content['personal_calendar_buttons'])): ?>
        <?php foreach ($content['personal_calendar_buttons']['content'] as $button) : ?>
          <?php print $button; ?>
         <?php endforeach; ?>
        <?php endif; ?>

        <dt><?php print t('When'); ?></dt>
        <?php if ($content['calendar']['type'] == 'timestamps'): ?>
          <?php if (count($content['calendar']['timestamps']) > 0): ?>
            <?php foreach ($content['calendar']['timestamps'] as $timestamp): ?>
              <?php if (!is_array($timestamp['begintime'])): ?>
                <dd><?php print $timestamp['day'] . ' ' . $timestamp['date'] . t(' at ') . $timestamp['begintime']; ?></dd>
              <?php else: ?>
                <?php $i=0; ?>
                <dd><?php print $timestamp['day'] . ' ' . $timestamp['date'] . t(' at '); ?>
                  <?php foreach ($timestamp['begintime'] as $begintime): ?>
                    <?php print $begintime; ?>
                    <?php if (++$i !== count($timestamp['begintime'])): ?>
                      <?php print ' | '; ?>
                    <?php endif; ?>
                  <?php endforeach; ?>
                </dd>
              <?php endif; ?>
            <?php endforeach; ?>
          <?php else: ?>
            <dd><?php print t('This event is finished.'); ?></dd>
          <?php endif; ?>
        <?php else: ?>
          <dd><?php print $content['when']; ?></dd>
        <?php endif; ?>
        <?php if (isset($content['all_url'])): ?>
        <a href="<?print $content['all_url']; ?>"><?php print t('Show all'); ?></a>
        <?php endif; ?>

      </div>
      <?php endforeach; ?>
    </div>
  </div>
  <?php endforeach; ?>
</div>
