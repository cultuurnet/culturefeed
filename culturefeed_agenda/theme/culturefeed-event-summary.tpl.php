<?php
/**
 * @file
 * Template for the summary of an event.
 */
?>

<div class="event-teaser">

  <h2><a href="<?php print $url ?>"><?php print $title; ?></a></h2>

  <div class="activity-wrapper">
    <div class="comment-wrapper">
      <?php if ($comment_count > 0): ?>
        <span class="comments"><?php print $comment_count; ?></span>
        <a href="<?php print $url ?>#lees"><?php print t('Read reactions'); ?></a>
        <a href="<?php print $url ?>#schrijf"><?php print t('Write reaction'); ?></a>
      <?php else: ?>
        <span class="no-comments"><?php print $comment_count; ?></span>
        <a href="<?php print $url ?>#schrijf"><?php print t('Be the first to write a review'); ?></a>
      <?php endif; ?>
    </div>
    <?php if ($recommend_count > 0): ?>
      <div class="count-aangeraden"><span><?php print $recommend_count ?></span> <?php print t('time recommended'); ?></div>
    <?php endif; ?>
    <?php print $recommend_link; ?>
    <?php print $attend_link; ?>
  </div>

  <div class="image">
    <?php if (!empty($thumbnail)): ?>
    <img src="<?php print $thumbnail; ?>?width=160&height=120&crop=auto" />
    <?php endif; ?>
  </div>

  <dl class="clearfix">

    <?php if ($location): ?>
    <dt><?php print t('Where'); ?></dt>
    <dd><?php print $location['title'] ?> <?php print $location['city']; ?></dd>
    <?php endif; ?>

    <?php if (!empty($when)): ?>
    <dt><?php print t('When'); ?></dt>
    <dd><?php print $when; ?></dd>
    <?php endif; ?>

    <?php if ($organiser): ?>
    <dt><?php print t('Organization'); ?></dt>
    <dd><?php print $organiser['title']; ?></dd>
    <?php endif; ?>

    <?php
      if (!empty($themes)):
    ?>
    <dt><?php print t('Theme'); ?></dt>
      <dd>
      <ul>
      <?php foreach ($themes as $theme): ?>
        <li><?php print $theme; ?></li>
      <?php endforeach; ?>
      </ul>
      </dd>
    <?php endif; ?>
  </dl>

  <?php print culturefeed_search_detail_l('event', $cdbid, $title, t('More info'), array('attributes' => array('class' => 'button'))); ?>

</div>