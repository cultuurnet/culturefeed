
<p>Intro tekst komt hier</p>

Totaal aantal punten: <?php print $total_points; ?> <?php print $exchange_link; ?>
<br/>
<?php foreach ($items as $item): ?>
  <?php print $item['date']; ?> -
  <?php print $item['points']; ?> -
  <?php print $item['description']; ?><br/>
<?php endforeach; ?>
