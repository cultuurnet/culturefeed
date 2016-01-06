<?php
/**
 * @file
 * Template for the summary of an event.
 * Please don't remove the cf- prefixed id's. This is used by GTM for user behavior tracking.
 * Some day your client will benefit from our aggregated insights & benchmarks too.
 * See https://github.com/cultuurnet/culturefeed/wiki/Culturefeed-tracking
 * Thanks!
 */
?>

<div class="event-teaser">

  <h2><?php print culturefeed_search_detail_l('event', $cdbid, $title, $title, array('attributes' => array('id' => 'cf-title_' . $cdbid), 'html' => TRUE)); ?></h2>

  <div class="activity-wrapper">
    <div class="comment-wrapper">
      <?php if ($comment_count > 0): ?>
        <span class="comments"><?php print $comment_count; ?></span>
        <a href="<?php print $url ?>#read" id="cf-review-read_<?php print $cdbid ?>"><?php print t('Read reactions'); ?></a>
        <a href="<?php print $url ?>#write" id="cf-review-write_<?php print $cdbid ?>"><?php print t('Write reaction'); ?></a>
      <?php else: ?>
        <span class="no-comments"><?php print $comment_count; ?></span>
        <a href="<?php print $url ?>#write" id="cf-review-write_<?php print $cdbid ?>"><?php print t('Be the first to write a review'); ?></a>
      <?php endif; ?>
    </div>
    <?php if ($recommend_count > 0): ?>
      <div class="count-aangeraden"><span><?php print $recommend_count ?></span> <?php print t('time recommended'); ?></div>
    <?php endif; ?>
    <?php if (!empty($recommend_link)) : ?>
      <?php print $recommend_link; ?>
    <?php endif; ?>
    <?php if ($attend_count > 0): ?>
      <div class="count-attend"><span><?php print $attend_count ?></span> <?php print format_plural($attend_count, 'attendee', 'attendees') ?></div>
    <?php endif; ?>
    <?php if (!empty($attend_link)) : ?>
      <?php print $attend_link; ?>
    <?php endif; ?>
  </div>

  <div class="image">
    <?php if (!empty($thumbnail)): ?>
      <?php print culturefeed_search_detail_l('event', $cdbid, $title, '<img src="' . $thumbnail . '?width=160&height=120&crop=auto" />', array('attributes' => array('id' => 'cf-image_' . $cdbid), 'html' => TRUE)); ?>
    <?php endif; ?>
  </div>

  <dl class="clearfix">

    <?php if ($location): ?>
    <dt><?php print t('Where'); ?></dt>
    <dd><?php if (!empty($location['title'])): print $location['title']; endif; ?> <?php if (!empty($location['city'])): print $location['city']; endif; ?></dd>
    <?php endif; ?>

    <?php if (!empty($when_md)): ?>
    <dt><?php print t('When'); ?></dt>
    <dd><?php print $when_md; ?></dd>
    <?php endif; ?>

    <?php if (!empty($organiser)): ?>
    <dt><?php print t('Organization'); ?></dt>
    <dd><?php print $organiser['title']; ?></dd>
    <?php endif; ?>

    <?php if (!empty($themes)): ?>
    <dt><?php print t('Theme'); ?></dt>
      <dd>
      <ul>
      <?php foreach ($themes as $theme): ?>
        <li><?php print $theme; ?></li>
      <?php endforeach; ?>
      </ul>
      </dd>
    <?php endif; ?>

    <?php if (!empty($keywords)): ?>
    <dt><?php print t('Keywords'); ?></dt>
    <dd><?php print $keywords; ?></dd>
    <?php endif; ?>

    <?php if (!empty($perfomers)): ?>
    <dt><?php print t('With'); ?></dt>
    <dd><?php print $perfomers; ?></dd>
    <?php endif; ?>

  </dl>

  <?php if (!empty($tickets)): ?>
    <?php print culturefeed_search_detail_l('event', $cdbid, $title, t('Info & tickets'), array('attributes' => array('class' => 'button', 'id' => 'cf-readmore_' . $cdbid))); ?>
  <?php else: ?>
    <?php print culturefeed_search_detail_l('event', $cdbid, $title, t('More info'), array('attributes' => array('class' => 'button', 'id' => 'cf-readmore_' . $cdbid))); ?>
  <?php endif; ?>

</div>
