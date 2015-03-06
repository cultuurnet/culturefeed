<div id="view-profile">
  <a href="<?php print $user_url; ?>"><?php print t('View profile'); ?></a>
</div>

<h2><?php print t('Pages where I am a member of'); ?></h2>
<?php if (!empty($my_pages)): ?>
  <?php print $my_pages; ?>
<?php else: ?>
  <div class="alert alert-info"><?php print t('<strong>You are not a member of any page.</strong> <br/> Search (or create) the page of your employer and become a member. If you create a new page, you automatically become a member and administrator of the page.'); ?>
  </div>
<?php endif; ?>

<h2><?php print t('Pages I follow'); ?></h2>
<?php if (!empty($i_follow)): ?>
  <?php print $i_follow; ?>
<?php else: ?>
  <div class="alert alert-info"><?php print t('<strong> You follow no pages. </strong> <br/> Find and follow pages that interest you and receive a notification when there are relevant updates.'); ?></div>
<?php endif; ?>

<h2><?php print t('Search new pages:'); ?></h2>
<?php print $search_pages; ?>

<div id="page_confirm" class="modal fade" tabindex="-1" role="dialog" aria-hidden="true">
  <div class="modal-dialog">
    <div class="modal-content"></div>
  </div>
</div>
