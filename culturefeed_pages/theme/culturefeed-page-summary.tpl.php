<?php
/**
 * @file
 * Template for the summary of a page.
 */
?>

<div class="page-teaser">

  <h2><a href="<?php print $url ?>"><?php print $title; ?></a></h2>

  <div class="activity-wrapper">
    <?php if ($follower_count > 0): ?>
      <div class="count-followers"><?php print format_plural($follower_count, '<span>@count</span> follower', '<span>@count</span> followers'); ?></div>
    <?php endif; ?>
    <?php if ($member_count > 0): ?>
    <div class="members-wrapper">
        <?php print format_plural($member_count, '<span class="members">@count</span> member', '<span class="members">@count</span> members'); ?>
        <a href="<?php print $url ?>#members"><?php print t('View members'); ?></a>
    </div>
    <?php endif; ?>
  </div>

  <div class="image">
    <?php if (!empty($image)): ?>
    <img src="<?php print $image; ?>?width=160&height=120&crop=auto" />
    <?php endif; ?>
  </div>

  <dl class="clearfix">
    <?php if (!empty($address)): ?>
    <dt><?php print t('Address'); ?></dt>
    <dd><?php print $address['street'] . $address['city'] . ' ' . $address['zip']; ?></dd>
    <?php endif; ?>
  </dl>

  <?php print $description ?>

  <a href="<?php print $url; ?>" class="button"><?php print $more_text; ?></a>

</div>