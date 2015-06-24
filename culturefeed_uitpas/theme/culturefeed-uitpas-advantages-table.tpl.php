<section class="overview overview-advantages">
  <?php foreach($items as $item): ?>
  <article class="<?php print implode(' ', $item['classes']); ?> clearfix">
    <div class="row">
      <?php print $item['overlay_link']; ?>
      <div class="main col-md-8 col-lg-9" role="main">
        <div class="row">
          <figure class="col-xs-3 col-lg-2">
            <?php print $item['image']; ?>
          </figure>
          <div class="content col-xs-9 col-lg-10">
            <span class="provider <?php print $item['cardsystem']['class']; ?>"><?php print $item['cardsystem']['name']; ?></span>
            <h2 class="title"><?php print $item['title']; ?></h2>
            <ul class="locations list-unstyled">
              <?php foreach($item['counters'] as $counter): ?>
              <li class="<?php print $counter['class']; ?>"><?php print $counter['name']; ?></li>
              <?php endforeach; ?>
            </ul>
            <span class="availability"><i class="fa fa-exclamation-circle"></i></a><?php print $item['availability']; ?></span>
          </div>
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
<!-- The Bootstrap Image Gallery lightbox, should be a child element of the document body -->
<div id="blueimp-gallery" class="blueimp-gallery blueimp-gallery-controls" data-use-bootstrap-modal="false">
<!-- The container for the modal slides -->
<div class="slides"></div>
<h3 class="title"></h3>
<a class="prev">‹</a>
<a class="next">›</a>
<a class="close">×</a>
<a class="play-pause"></a>
<ol class="indicator"></ol>
<!-- The modal dialog, which will be used to wrap the lightbox content -->
<div class="modal fade">
  <div class="modal-dialog">
    <div class="modal-content">
      <div class="modal-body"></div>

      </div>
    </div>
  </div>
</div>
