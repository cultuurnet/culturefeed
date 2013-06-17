<?php
/**
 * @file
 * Template for the detail of an actor.
 */
?>

<h1><?php print $title; ?></h1>

<?php print $recommend_link; ?>

Samengevat - <?php print $shortdescription; ?><br/>

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

  <?php if (!empty($contact)) : ?>
    <dt>Contact</dt>
    <dd>
    <?php if (!empty($contact['mail'])) : ?>
      Mail: <?php print $contact['mail'] ?><br />
    <?php endif; ?>
    <?php if (!empty($contact['phone'])) : ?>
      Telefoon: <?php print $contact['phone'] ?><br />
    <?php endif; ?>
    <?php if (!empty($contact['fax'])) : ?>
      Fax: <?php print $contact['fax'] ?>
    <?php endif; ?>
    </dd>
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

<?php print $recommend_link; ?>