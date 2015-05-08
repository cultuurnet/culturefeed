<ul>

	<?php foreach ($items as $item): ?>
		<li class="<?php print $item['class'][0]; ?>"><?php print $item['data']; ?></li>
	<?php endforeach; ?>
</ul>