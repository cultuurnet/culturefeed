<row>
  <section  class="col-md-12">
    <?php if (!empty($nav_months)) : ?>
      <?php print $nav_months ?>
    <?php endif; ?>
  </section>
</row>

<row>
  <aside class="col-md-3" role="complementary">
    <div class="region region-sidebar-first">
      <div class="panel panel-default">
        <div class="panel-body">
          SIDEBAR
          <?php print $sidebar; ?>
        </div>
      </div>
    </div>
  </aside>
  <section class="col-md-9">
    <?php if (!empty($content)): ?>
      <?php print $content ?><br />
    <?php else: ?>
      <h3><?php print t('No activities added to your calendar yet.') ?></h3>
    <?php endif; ?>

    <?php if (empty($content)): ?>
      <?php print $content; ?>
    <?php endif; ?>
  </section>
</row>
