<?php
/* @var \CultureFeed_Cdb_Item_Event $event */
$event = $item->getEntity();

foreach ($event->getDetails() as $detail) {
  if ('nl' === $detail->getLanguage()) {
      break;
  }
}

$recommend_count = $item->getActivityCount('recommend');
$comment_count = $item->getActivityCount('comment');

if ($event->getLocation()) {
  if ($event->getLocation()->getLabel()) {
    $waar = $event->getLocation()->getLabel();
  }
  else if ($event->getLocation()->getActor() && $event->getLocation()->getActor()->getDetailByLanguage('nl')) {
    $waar = $event->getLocation()->getActor()->getDetailByLanguage('nl')->getTitle();
  }
}

if ($event->getOrganiser()) {
  if ($event->getOrganiser()->getLabel()) {
    $organisatie = $event->getOrganiser()->getLabel();
  }
  else if ($event->getOrganiser()->getActor() && $event->getOrganiser()->getActor()->getDetailByLanguage('nl')) {
    $organisatie = $event->getOrganiser()->getActor()->getDetailByLanguage('nl')->getTitle();
  }
}

if ($detail && $detail->getCalendarSummary()) {
  $wanneer = $detail->getCalendarSummary();
}

?>
<div class="event-teaser">
  <h2><?php print l($detail->getTitle(), 'agenda/e/detail/' . $event->getCdbId()); ?></h2>

  <div class="activity-wrapper">
    <div class="comment-wrapper">
      <?php if ($comment_count > 0): ?>
        <span class="comments"><?php print $comment_count; ?></span> 
        <?php print l('Lees beoordelingen', 'agenda/e/detail/' . $event->getCdbId(), array('fragment' => 'lees')); ?>
        <?php print l('Schrijf beoordeling', 'agenda/e/detail/' . $event->getCdbId(), array('fragment' => 'schrijf')); ?>
      <?php else: ?>
        <span class="no-comments"><?php print $comment_count; ?></span>
        <?php print l('Schrijf als eerste een beoordeling', 'agenda/e/detail/' . $event->getCdbId(), array('fragment' => 'schrijf')); ?>
      <?php endif; ?>
    </div>
    <?php if ($recommend_count > 0): ?>
      <div class="count-aangeraden"><?php print format_plural($recommend_count, '<span>@count</span> keer aangeraden', '<span>@count</span> keer aangeraden'); ?></div>
    <?php endif; ?>
    <?php print culturefeed_agenda_activity_recommend_link($item); ?>
  </div>
 
  <div class="image">
  <?php
    /* @var CultureFeed_Cdb_Data_File $file */
    foreach ($detail->getMedia()->byMediaType(CultureFeed_Cdb_Data_File::MEDIA_TYPE_PHOTO) as $file) {
      print '<img src="' . $file->getHLink() . '?width=160&height=120&crop=auto" /><br />';
    }
  ?>
  </div>

  <dl class="clearfix">

    <?php if (isset($waar)): ?>
    <dt>Waar</dt>
    <dd><?php print check_plain($waar); ?></dd>
    <?php endif; ?>

    <?php if (isset($wanneer)): ?>
    <dt>Wanneer</dt>
    <dd><?php print check_plain($wanneer); ?></dd>
    <?php endif; ?>

    <?php if (isset($organisatie)): ?>
    <dt>Organisatie</dt>
    <dd><?php print $organisatie; ?></dd>
    <?php endif; ?>

    <?php
      $themes = culturefeed_agenda_get_categories('theme', $event);
      if (count($themes) > 0):
    ?>
    <dt>Thema</dt>
      <dd>
      <?php
      array_walk($themes, function(&$item) {
        $item = check_plain($item->getName());
      });

      print implode(',', $themes);
      ?>
      </dd>
    <? endif; ?>
  </dl>
  <?php
    // example
    // print $event->getCdbId();
  ?>
  <?php print l('Meer info en boeking', 'agenda/e/info-en-boeking/' . $event->getCdbId(), array('attributes' => array('class' => 'button'))); ?>

</div>