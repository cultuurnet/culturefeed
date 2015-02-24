<?php
/**
 * @file
 * Template for the calendar summary of an activity.
 */
?>

<hr class="small"/>
<div class="calendar-activity-wrapper">
  <div class="activity-header">
    <?php if (!empty($date)): ?>
      <?php print $date; ?><br />
    <?php endif; ?>
    <a href="<?php print $url; ?>"><?php print $title; ?></a><br />
    <?php if ($my_calendar): ?>
      <a href="<?php print $delete_link['url'] ?>"><?php print $delete_link['text']; ?></a>
      <?php if ($edit_link['show']): ?>
        <br />
        <a href="<?php print $edit_link['url'] ?>"><?php print $edit_link['text']; ?></a>
      <?php endif; ?>
    <?php endif; ?>
  </div>
  <dl class="clearfix">
    <?php if ($location): ?>
      <dt><?php print t('Where'); ?></dt>
      <dd>
        <?php if (!empty($location['link'])): ?>
          <?php print $location['link']; ?><br />
        <?php else: ?>
          <?php print $location['title'];?><br />
        <?php endif; ?>
        <?php if (!empty($location['street'])): ?>
          <?php print $location['street'] ?><br />
        <?php endif; ?>
        <?php if (!empty($location['zip'])): ?>
          <?php print $location['zip']; ?>
        <?php endif; ?>
        <?php if (!empty($location['city'])): ?>
          <?php print $location['city']; ?>
        <?php endif; ?>
      </dd>
    <?php endif; ?>

    <?php if (!empty($reservation) || !empty($tickets)) : ?>
      <dt><?php print t('Price'); ?></dt>
      <dd>
        <?php if (!empty($tickets)) : ?>
          <?php print implode(', ', $tickets) ?><br />
        <?php endif; ?>
        <?php if (!empty($reservation['mail'])) : ?>
          <?php print $reservation['mail'] ?><br />
        <?php endif; ?>
        <?php if (!empty($reservation['url'])) : ?>
          <?php print $reservation['url'] ?><br />
        <?php endif; ?>
        <?php if (!empty($reservation['phone'])) : ?>
          <?php print t('Phone'); ?>: <?php print $reservation['phone'] ?><br />
        <?php endif; ?>
      </dd>
    <?php endif; ?>

    <dd>
      <a href="<?php print $url; ?>"><?php print t('More info'); ?></a>
    </dd>
  </dl>
</div>
