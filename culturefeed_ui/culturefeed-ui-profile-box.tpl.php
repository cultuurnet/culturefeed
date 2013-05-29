<div class="image">
  <?php print $picture ?>
</div>

<div class="options">
  <ul>
    <li><strong><?php print $nick ?></strong></li>

    <?php // Render dropdown if he contains items. ?>
    <?php if ($dropdown_items): ?>
    <li class="dropdown">
      <ul>
      <?php foreach ($dropdown_items as $dropdown_item): ?>
        <li<?php if (isset($dropdown_item['class'])):?> class="<?php print $dropdown_item['class']?>"<?php endif;?>>
          <?php print $dropdown_item['data']; ?>

          <?php if (isset($dropdown_item['children'])): ?>
          <ul>
          <?php foreach ($dropdown_item['children'] as $dropdown_sub_item): ?>
            <li<?php if (isset($dropdown_item['class'])):?> class="<?php print $dropdown_item['class']?>"<?php endif;?>>
            <?php print $dropdown_sub_item['data']; ?>
            </li>
          <?php endforeach; ?>
          </ul>
          <?php endif; ?>

        </li>
      <?php endforeach;?>
      </ul>
    </li>
    <?php endif; ?>

    <?php // Render main items ?>
    <?php foreach ($main_items as $item): ?>
    <li<?php if (isset($item['class'])): print ' class="' . $item['class'] . '"' ?> <?php endif;?>><?php print $item['data']; ?></li>
    <?php endforeach; ?>

  </ul>
</div>