<?php
/**
 * @file
 * Template for the summary of a page.
 */
?>

<div class="page-teaser">

  <h2><a href="<?print $link ?>"><?print $title; ?></a></h2>

  <?php
  /*
  <div class="activity-wrapper">
    <div class="members-wrapper">
      <?php if ($members > 0): ?>
        <span class="members"><?php print $members; ?></span> leden
        <a href="<?php print $link ?>#members">Bekijk leden</a>
    </div>
    <?php if ($followers > 0): ?>
      <div class="count-followers"><?php print format_plural($recommend_count, '<span>@count</span> volgers', '<span>@count</span> volger'); ?></div>
    <?php endif; ?>
  </div>
  */
  ?>

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