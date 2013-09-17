<p><?php print t('points'); ?>: <?php print $total_points; ?> <?php print $exchange_link; ?></p>

<table>
  <tbody>
    <?php foreach ($items as $item): ?>
      <tr>
        <td><?php print $item['date']; ?></td>
        <td><?php print $item['points']; ?></td>
        <td><?php print $item['description']; ?></td>
      </tr>
    <?php endforeach; ?>  
  </tbody>
</table>
