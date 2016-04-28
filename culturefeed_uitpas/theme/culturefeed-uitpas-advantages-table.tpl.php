<section class="overview overview-advantages">
  <?php foreach($items as $item): ?>
  <article class="article <?php print implode(' ', $item['classes']); ?>">

    <?php print $item['overlay_link']; ?>
    <div class="article--main">
      <figure>
        <?php print $item['image']; ?>
      </figure>
      <span class="provider <?php print $item['cardsystem']['class']; ?>"><?php print $item['cardsystem']['name']; ?></span>
      <h2><?php print $item['title']; ?></h2>
      <ul>
        <?php foreach($item['counters'] as $counter): ?>
        <li class="<?php print $counter['class']; ?>"><?php print $counter['name']; ?></li>
        <?php endforeach; ?>
      </ul>
      <span class="availability"><?php print $item['availability']; ?></span>
    </div> <!--/ end .main -->

    <div class="points <?php print $item['points']['classes']; ?>">
      <span class="points-value"><?php print $item['points']['value']; ?></span>
      <?php if ($item['points']['remark']): ?>
        <span class="points-remark"><?php print $item['points']['remark']; ?></span>
      <?php endif; ?>
    </div> <!--/ end aside -->

  </article> <!--/ end article -->
  <?php endforeach; ?>
</section>

<div class="pager clearfix">
<?php print $pager; ?>
</div>
