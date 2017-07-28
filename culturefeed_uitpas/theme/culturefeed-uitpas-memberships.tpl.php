<?php
  foreach ($memberships as $key => $membership_per_card_system):
?>
  <h3><?php print $key; ?></h3>
  <table>
    <tr>
      <th><?php print t('Name'); ?></th>
      <th><?php print t('Valid until'); ?></th>
    </tr>
    <?php foreach ($membership_per_card_system as $membership): ?>
      <tr>
        <td><?php print $membership->name; ?></td>
        <td><?php print date('d/m/Y', $membership->endDate); ?></td>
      </tr>
    <?php endforeach; ?>
  </table>
<?php
  endforeach;
?>