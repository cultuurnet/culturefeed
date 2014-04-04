<h3><?php print t('News'); ?></h3>
<p><?php print $view_page_link; ?></p>

<?php if (!empty($items)): ?>

    <table class="table table-striped">
    <thead>
    <tr>
      <th><?php print t('Title') ?></th>
      <th><?php print t('Date') ?></th>
      <th></th>
    </tr>
    </thead>
      <tbody>
        <?php foreach ($items as $item): ?>
        <tr>
          <td><?php print $item['title']; ?></td>
          <td><?php print $item['date']; ?></td>
          <td><?php print l(t('Delete'), $item['delete_url']); ?><td>
        </tr>
        <?php endforeach; ?>
      </tbody>
  </table>


  <?php print theme('pager') ?>
<?php else: ?>
    <p><?php print t('There are currently no news items') ?></p>
<?php endif; ?>

<?php print l(t('Add a news item'), 'pages/' . $page->getId() . '/news/add'); ?>