<?php
/**
 * @file
 * Template for the detail of an event.
 */
?>

<?php
/* print '<h1>' . $title . '</h1>'; */
?>

<?php print t('Reactions'); ?>
<?php if ($comment_count > 0): ?>
  <span class="comments"><?php print $comment_count; ?></span>
  <a href="#lees"><?php print t('Read reactions'); ?></a>
  <a href="#schrijf"><?php print t('Write reaction'); ?></a>
<?php else: ?>
  <span class="no-comments"><?php print $comment_count; ?></span>
  <a href="#schrijf"><?php print t('Be the first to write a review'); ?></a>
<?php endif; ?>

<?php if (!empty($themes_links)): ?>
<?php print t('Theme'); ?>: <?php print $themes_links[0] ?><br/>
<?php endif; ?>

<?php print t('Short description'); ?>: <?php print $shortdescription; ?><br/>

<dl class="clearfix">

  <?php if ($location): ?>
  <dt><?php print t('Where'); ?></dt>
  <dd>
    <?php if (!empty($location['link'])): ?>
    <?php print $location['link']; ?><br/>
    <?php else: ?>
    <?php print $location['title'];?><br/>
    <?php endif; ?>
    <?php if (!empty($location['street'])): ?>
      <?php print $location['street'] ?>,
    <?php endif; ?>
    <?php if (!empty($location['zip'])): ?>
      <?php print $location['zip']; ?>
    <?php endif; ?>
    <?php if (!empty($location['city'])): ?>
      <?php print $location['city']; ?>
    <?php endif; ?>
    <?php if (!empty($coordinates)): ?>
    <?php print $coordinates['lat'] ?> - <?php print $coordinates['lng'] ?>
    <?php endif; ?>
  </dd>
  <?php endif; ?>

  <dt><?php print t('When'); ?></dt>
  <?php if ($calendar['type'] == 'timestamps'): ?>
    <?php if (count($calendar['timestamps']) > 0): ?>

      <?php foreach ($calendar['timestamps'] as $timestamp): ?>
        <?php if (isset($timestamp['begintime']) && !is_array($timestamp['begintime'])): ?>
          <dd><?php print $timestamp['date'] . t(' at ') . $timestamp['begintime']; ?></dd>
        <?php else: ?>
          <?php $i = 0; ?>
          <dd><?php print $timestamp['date'] ?>
            <?php if (isset($timestamp['begintime'])): ?>
            <?php print t(' at '); ?>
            <?php foreach ($timestamp['begintime'] as $begintime): ?>
              <?php print $begintime; ?>
              <?php if (++$i !== count($timestamp['begintime'])): ?>
                <?php print ' | '; ?>
              <?php endif; ?>
            <?php endforeach; ?>
            <?php endif; ?>
          </dd>
        <?php endif; ?>
      <?php endforeach; ?>
    <?php else: ?>
      <dd><?php print t('This event is finished.'); ?></dd>
    <?php endif; ?>
  <?php else: ?>
    <dd><?php print $when_lg; ?></dd>
  <?php endif; ?>

   <?php if (!empty($personal_calendar_buttons)): ?>
   <?php foreach ($personal_calendar_buttons['content'] as $button) : ?>
     <?php print $button; ?>
    <?php endforeach; ?>
   <?php endif; ?>

  <?php if ($organiser): ?>
  <dt><?php print t('Organization'); ?></dt>
  <dd>
    <?php if (empty($organiser['link'])):?>
    <?php print $organiser['title']; ?>
    <?php else: ?>
    <?php print $organiser['link'] ?>
    <?php endif; ?>
  </dd>
  <?php endif; ?>

  <?php if (!empty($price)): ?>
  <dt><?php print t('Price'); ?></dt>
  <dd><?php print $price . ' '; ?><?php print $price_description; ?></dd>
  <?php endif; ?>

  <?php if (!empty($reservation) || !empty($tickets)) : ?>
    <dt><?php print t('Reservation'); ?></dt>
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

  <?php if (!empty($contact['mail']) || !empty($contact['phone']) || !empty($contact['fax'])) : ?>
    <dt><?php print t('Contact'); ?></dt>
    <dd>
    <?php if (!empty($contact['mail'])) : ?>
      <?php print $contact['mail'] ?><br />
    <?php endif; ?>
    <?php if (!empty($contact['phone'])) : ?>
      <?php print t('Phone'); ?>: <?php print $contact['phone'] ?><br />
    <?php endif; ?>
    <?php if (!empty($contact['fax'])) : ?>
      <?php print t('Fax'); ?>: <?php print $contact['fax'] ?>
    <?php endif; ?>
    </dd>
  <?php endif; ?>

  <?php if (!empty($age)): ?>
  <dt><?php print t('Age'); ?></dt>
  <dd><?php print $age; ?></dd>
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

  <?php if (!empty($links)): ?>
  <dt><?php print t('Links'); ?></dt>
  <dd>
    <ul>
    <?php foreach ($links as $link): ?>
      <li><?php print $link; ?></li>
    <?php endforeach; ?>
    </ul>
  </dd>
  <?php endif; ?>

  <?php if (!empty($uitpas_promotions)): ?>
  <dt><?php print t('Promotions'); ?></dt>
  <dd class="promotions"><?php print $uitpas_promotions; ?></dd>
  <?php endif; ?>

</dl>

<?php if (!empty($main_picture)): ?>
<img src="<?php print $main_picture; ?>" />

<?php foreach ($pictures as $picture): ?>
  <img src="<?php print $picture; ?>?width=160&height=120&crop=auto" />
<?php endforeach; ?>

<?php endif; ?>

<?php if (!empty($videos)): ?>
<?php foreach ($videos as $video): ?>
  <?php print $video; ?>
<?php endforeach; ?>
<?php endif; ?>

<?php if (!empty($recommend_link)) : ?>
  <?php print $recommend_link; ?>
<?php endif; ?>

<?php if (!empty($personal_calendar_buttons)): ?>
<?php foreach ($personal_calendar_buttons['aside'] as $button) : ?>
  <?php print $button; ?>
<?php endforeach; ?>
<?php endif; ?>

<div>
  <?php print $facebook_link; ?>
  <br>
  <?php print $googleplus_link; ?>
  <br>
  <?php print $twitter_link; ?>
  <br>
  <?php print $whatsapp_link; ?>
  <br>
  <?php print $mail_link; ?>
</div>
