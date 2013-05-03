<?php
/**
 * @file
 * Template for the summary of a page.
 */
?>

<div class="page-teaser">

  <h2><a href="<?print $url ?>"><?print $title; ?></a></h2>

  <div class="activity-wrapper">
    <?php if ($member_count > 0): ?>
    <div class="members-wrapper">
        <?php print format_plural($member_count, '<span class="members">@count</span> lid', '<span class="members">@count</span> leden'); ?>
        <a href="<?php print $url ?>#members">Bekijk leden</a>
    </div>
    <?php endif; ?>
    <?php if ($follower_count > 0): ?>
      <div class="count-followers"><?php print format_plural($follower_count, '<span>@count</span> volger', '<span>@count</span> volgers'); ?></div>
    <?php endif; ?>
  </div>

  <div class="image">
    <?php if (!empty($thumbnail)): ?>
    <img src="<?php print $thumbnail; ?>?width=160&height=120&crop=auto" />
    <?php endif; ?>
  </div>

  <dl class="clearfix">
    <?php if (!empty($address)): ?>
    <dt>Adres</dt>
    <dd><?php print $address['street'] . $address['city'] . ' ' . $address['zip']; ?></dd>
    <?php endif; ?>
  </dl>

  <?php print $description ?>

  <?php print culturefeed_search_detail_l('page', $id, $title, $more_text, array('attributes' => array('class' => 'button'))); ?>

</div>