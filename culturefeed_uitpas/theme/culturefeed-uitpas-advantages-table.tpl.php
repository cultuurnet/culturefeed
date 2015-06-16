<section class="advantages-overview">
  <?php foreach($items as $item): ?>
  <article>
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
    <aside>
      <span class="points"><?php print $item['points']['value']; ?></span>
      <span class="points"><?php print $item['points']['remark']; ?></span>
    </aside>
  </article>
  <?php endforeach; ?>
</section>
<?php print $pager; ?>
