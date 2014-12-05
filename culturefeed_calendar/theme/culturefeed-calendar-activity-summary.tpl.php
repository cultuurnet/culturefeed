<?php
/**
 * @file
 * Template for the calendar summary of an activity.
 */
?>
<div class="calendar-activity-wrapper col-xs-6">
  <table class="table table-bordered">
    <tbody>
      <tr>
        <th colspan="2">
          <?php if (!empty($date)): ?>
            <?php print $date; ?><br />
          <?php endif; ?>
          <?php print $title ?>
        </th>
      </tr>
      <tr>
        <td class="col-lg-2 col-md-2 col-sm-1 col-xs-1"><strong class="hidden-xs hidden-sm"><?php print t('Where'); ?></strong><i class="fa fa-map-marker hidden-md hidden-lg"></i></td>
        <td>
          <?php if ($location): ?>
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
          <?php endif; ?>
        </td>
      </tr>
      <?php if (!empty($reservation) || !empty($tickets)) : ?>
        <tr>
          <td><strong class="hidden-xs hidden-sm"><?php print t('Price'); ?></strong><i class="fa fa-calendar hidden-md hidden-lg"></i></td>
          <td>
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
          </td>
        </tr>
      <?php endif; ?>
      <tr>
        <td><strong class="hidden-xs hidden-sm"><?php print t('More info'); ?></strong><i class="fa fa-info-circle hidden-md hidden-lg"></i></td>
        <td><a href="<?php print $url; ?>"><?php print t('More info'); ?></a></td>
      </tr>
    </tbody>
  </table>
</div>