<p><strong><span class="lead"><?php print $total_points; ?> <?php print t('points'); ?></span></strong> <?php print $exchange_link; ?></p>

<p class="muted"><small><?php print t('As an UiTiD user, you can save points by performing actions. Points can be exchanged for one or more gifts.'); ?> <a href="#"><?php print t('More info'); ?></a></small></p>

<table class="table table-striped table-hover">
  <thead>
    <tr>
      <th></th>
      <th></th>
      <th></th>
    </tr>
  </thead>
  <tbody>
    <?php foreach ($items as $item): ?>
      <tr>
        <td class="muted"><small><?php print $item['date']; ?></small></td>
        <td><small><strong><?php print $item['points']; ?></strong></small></td>
        <td><?php print $item['description']; ?></td>
      </tr>
    <?php endforeach; ?>  
  </tbody>
</table>





