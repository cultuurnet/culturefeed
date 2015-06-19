<section class="overview overview-advantages">
  <?php foreach($items as $item): ?>
  <article class="<?php print implode(' ', $item['classes']); ?> clearfix">
    <div class="row">
      <div class="main col-md-8 col-lg-9" role="main">
        <figure class="thumbnail">
          <?php print $item['image']; ?>
        </figure>
        <div class="content">
          <span class="provider <?php print $item['cardsystem']['class']; ?>"><?php print $item['cardsystem']['name']; ?></span>
          <h2 class="title"><?php print $item['title']; ?></h2>
          <ul class="locations list-unstyled">
            <?php foreach($item['counters'] as $counter): ?>
            <li class="<?php print $counter['class']; ?>"><?php print $counter['name']; ?></li>
            <?php endforeach; ?>
          </ul>
          <span class="availability"><i class="fa fa-exclamation-circle"></i></a><?php print $item['availability']; ?></span>
        </div>
      </div> <!--/ end .main -->
      <aside class="points <?php print $item['points']['classes']; ?> col-md-4 col-lg-3">
        <span class="points-value"><?php print $item['points']['value']; ?></span>
        <?php if ($item['points']['remark']): ?>
          <span class="points-remark"><?php print $item['points']['remark']; ?></span>
        <?php endif; ?>
      </aside> <!--/ end aside -->
    </div>
  </article> <!--/ end article -->
  <?php endforeach; ?>
</section>
<div class="pager clearfix">
<?php print $pager; ?>
</div>
