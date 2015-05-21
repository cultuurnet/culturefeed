<?php
/**
 * @file
 * Template for the detail of a production.
 */
?>

<?php
/* print '<h1>' . $title . '</h1>'; */
?>

<?php print t('Reactions'); ?>:
<?php if ($comment_count > 0): ?>
  <span class="comments"><?php print $comment_count; ?></span>
  <a href="#lees"><?php print t('Read reactions'); ?></a>
  <a href="#schrijf"><?php print t('Write reaction'); ?></a>
<?php else: ?>
  <span class="no-comments"><?php print $comment_count; ?></span>
  <a href="#schrijf"><?php print t('Be the first to write a review'); ?></a>
<?php endif; ?>

<?php if ($themes_links): ?>
<?php print t('Theme'); ?>: <?php print $themes_links[0] ?><br/>
<?php endif; ?>

<?php print t('Short description'); ?>: <?php print $shortdescription; ?><br/>

<dl class="clearfix">

  <?php if ($location): ?>
  <dt><?php print t('Where'); ?></dt>
  <dd>
    <?php print $location['title'];?><br/>
    <?php if (!empty($location['street'])): ?>
    <?php print $location['street'] ?>, <?php print $location['zip']; ?> <?php print $location['city']; ?>
    <?php endif; ?>
    <?php if (!empty($regions)): ?>
    <ul>
    <?php foreach ($regions as $region): ?>
      <li><?php print $region?></li>
    <?php endforeach; ?>
    </ul>
    <?php endif; ?>
  </dd>
  <?php endif; ?>

  <?php if (!empty($when_lg)): ?>
  <dt><?php print t('When'); ?></dt>
  <dd><?php print $when_lg; ?></dd>
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

</dl>

<img src="<?php print $main_picture; ?>" />

<?php foreach ($pictures as $picture): ?>
  <img src="<?php print $picture; ?>?width=160&height=120&crop=auto" />
<?php endforeach; ?>

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