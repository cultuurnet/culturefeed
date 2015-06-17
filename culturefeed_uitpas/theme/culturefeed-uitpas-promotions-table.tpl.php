<section class="promotions-overview">
  <?php foreach($items as $item): ?>
  <article class="<?php print implode(' ', $item['classes']); ?>">
    <main>
      <figure class="thumbnail">
        <?php print $item['image']; ?>
      </figure>
      <div class="content">
        <span class="provider <?php print $item['cardsystem']['class']; ?>"><?php print $item['cardsystem']['name']; ?></span>
        <h2 class="title"><?php print $item['title']; ?></h2>
        <ul class="locations">
          <?php foreach($item['counters'] as $counter): ?>
          <li class="<?php print $counter['class']; ?>"><?php print $counter['name']; ?></li>
          <?php endforeach; ?>
        </ul>
        <span class="availability"><?php print $item['availability']; ?></span>
      </div>
    </main>
    <aside class="points <?php print $item['points']['classes']; ?>">
      <span class="points-value"><?php print $item['points']['value']; ?></span>
      <?php if ($item['points']['remark']): ?>
      <span class="points-remark"><?php print $item['points']['remark']; ?></span>
      <?php endif; ?>
    </aside>
  </article>
  <?php endforeach; ?>
</section>
