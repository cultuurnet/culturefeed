<?php if ($items): ?>
<ul>
<?php foreach ($items as $item) :?>
  <li><?php print $item; ?>
<?php endforeach; ?>
</ul>

<?php elseif ($is_admin) :?>

<div>
  <h3>Jou pagina heeft nog geen activiteiten gepubliceerd.</h3>
  <p>Voeg nieuwe activiteiten toe via <a href="http://www.uitdatabank.be">www.uitdatabank.be</a>.
</div>

<?php endif; ?>
