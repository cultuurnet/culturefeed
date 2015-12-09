<?php
/**
 * @file
 * Template for the detail of an actor.
 */
?>

<?php
/* print '<h1>' . $title . '</h1>'; */
?>

<?php if (!empty($recommend_link)) : ?>
  <?php print $recommend_link; ?>
<?php endif; ?>

<?php if (!empty($shortdescription)) : ?>
  <p>
    <?php print $shortdescription; ?>
  </p>
<?php endif; ?>

<dl class="clearfix">

  <?php if ($location): ?>
  <dt><?php print t('Where'); ?></dt>
  <dd>
    <?php if (!empty($location['link'])): ?>
    <?php print $location['link']; ?><br/>
    <?php else: ?>
    <?php print $location['title'];?><br/>
    <?php endif; ?>
    <?php print $location['street'] ?>, <?php print $location['zip']; ?> <?php print $location['city']; ?>
    <?php if (!empty($coordinates)): ?>
    <?php print $coordinates['lat'] ?> - <?php print $coordinates['lng'] ?>
    <?php endif; ?>
  </dd>
  <?php endif; ?>

  <?php if (!empty($contact)) : ?>
    <dt><?php print t('Contact'); ?></dt>
    <dd>
    <?php if (!empty($contact['mail'])) : ?>
      <?php print t('Mail'); ?>: <?php print $contact['mail'] ?><br />
    <?php endif; ?>
    <?php if (!empty($contact['phone'])) : ?>
      <?php print t('Phone'); ?>: <?php print $contact['phone'] ?><br />
    <?php endif; ?>
    <?php if (!empty($contact['fax'])) : ?>
      <?php print t('Fax'); ?>: <?php print $contact['fax'] ?>
    <?php endif; ?>
    </dd>
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

  <?php if (!empty($weekscheme)): ?>
  <dt><?php print t('Opening hours'); ?></dt>
  <dd><?php print $when_lg; ?></dd>
  <?php endif; ?>

</dl>

<img src="<?php print $main_picture; ?>" />

<?php foreach ($pictures as $picture): ?>
  <img src="<?php print $picture; ?>?width=160&height=120&crop=auto" />
<?php endforeach; ?>

<?php foreach ($videos as $video): ?>
  <?php print $video; ?>
<?php endforeach; ?>

<?php if (!empty($recommend_link)) : ?>
  <?php print $recommend_link; ?>
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
