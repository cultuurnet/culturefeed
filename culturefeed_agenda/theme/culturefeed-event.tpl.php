<?php
/**
 * @file
 * Template for the detail of an event.
 */
?>

<h2><?php print $title; ?></h2>

reacties:
<?php if ($comment_count > 0): ?>
  <span class="comments"><?php print $comment_count; ?></span>
  <a href="#lees">Lees reacties</a>
  <a href="#schrijf">Schrijf een reactie</a>
<?php else: ?>
  <span class="no-comments"><?php print $comment_count; ?></span>
  <a href="#schrijf">Schrijf als eerste een reactie</a>
<?php endif; ?>

<?php if (!empty($themes)): ?>
Thema: <?php print $themes[0] ?><br/>
<?php endif; ?>

Korte beschrijving: <?php print $shortdescription; ?><br/>

<dl class="clearfix">

  <?php if ($location): ?>
  <dt>Waar</dt>
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

  <?php if (!empty($when)): ?>
  <dt>Wanneer</dt>
  <dd><?php print $when; ?></dd>
  <?php endif; ?>

  <?php if ($organiser): ?>
  <dt>Organisatie</dt>
  <dd>
    <?php if (empty($organiser['link'])):?>
    <?php print $organiser['title']; ?>
    <?php else: ?>
    <?php print $organiser['link'] ?>
    <?php endif; ?>
  </dd>
  <?php endif; ?>

  <?php if (!empty($contact)) : ?>
    <dt>Contact</dt>
    <dd>
    <?php if (isset($contact['mail'])) : ?>
      Mail: <?php print $contact['mail'] ?><br />
    <?php endif; ?>
    <?php if (isset($contact['phone'])) : ?>
      Telefoon: <?php print $contact['phone'] ?><br />
    <?php endif; ?>
    <?php if (isset($contact['fax'])) : ?>
      Fax: <?php print $contact['fax'] ?>
    <?php endif; ?>
    </dd>
  <?php endif; ?>

  <?php if (!empty($age)): ?>
  <dt>Leeftijd</dt>
  <dd><?php print $age; ?></dd>
  <?php endif; ?>

  <?php if (!empty($themes)): ?>
  <dt>Thema</dt>
    <dd>
      <ul>
      <?php foreach ($themes as $theme): ?>
        <li><?php print $theme; ?></li>
      <?php endforeach; ?>
      </ul>
    </dd>
  <?php endif; ?>

  <?php if (!empty($price)): ?>
  <dt>Prijs</dt>
  <dd><?php print $price; ?><?php print $price_description; ?></dd>
  <?php endif; ?>

  <?php if (!empty($links)): ?>
  <dt>Links</dt>
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

<?php foreach ($videos as $video): ?>
  <?php print $video; ?>
<?php endforeach; ?>

<?php print culturefeed_social_activity_recommend_link($item); ?>