<?php
/**
 * @file
 * Template for the summary of an actor.
 * Please don't remove the cf- prefixed id's. This is used by GTM for user behavior tracking. 
 * Some day your client will benefit from our aggregated insights & benchmarks too.
 * See https://github.com/cultuurnet/culturefeed/wiki/Culturefeed-tracking
 * Thanks!
 */
?>

<div class="event-teaser">

  <h2><?php print culturefeed_search_detail_l('actor', $cdbid, $title, $title, array('attributes' => array('id' => 'cf-title_' . $cdbid))); ?></h2>

  <div class="image">
    <?php if (!empty($thumbnail)): ?>
      <?php print culturefeed_search_detail_l('actor', $cdbid, $title, '<img src="' . $thumbnail . '?width=160&height=120&crop=auto" />', array('attributes' => array('id' => 'cf-image_' . $cdbid), 'html' => TRUE)); ?>
    <?php endif; ?>
  </div>

  <dl class="clearfix">

    <?php if ($location): ?>
    <dt><?php print t('Where'); ?></dt>
    <dd><?php if (!empty($location['title'])): print $location['title']; endif; ?> <?php if (!empty($location['city'])): print $location['city']; endif; ?></dd>
    <?php endif; ?>

    <?php if (!empty($keywords)): ?>
    <dt><?php print t('Keywords'); ?></dt>
    <dd><?php print $keywords; ?></dd>
    <?php endif; ?>

  </dl>

  <?php print culturefeed_search_detail_l('production', $cdbid, $title, t('More info'), array('attributes' => array('class' => 'button', 'id' => 'cf-readmore_' . $cdbid))); ?>

</div>

