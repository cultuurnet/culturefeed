<?php
/**
 * @file
 *
 */

?>
<h2><?php print $title; ?></h2>

beoordelingen:
<?php if ($comment_count > 0): ?>
  <span class="comments"><?php print $comment_count; ?></span>
  <a href="#lees">Lees beoordelingen</a>
  <a href="#schrijf">Schrijf een beoordeling</a>
<?php else: ?>
  <span class="no-comments"><?php print $comment_count; ?></span>
  <a href="#schrijf">Schrijf als eerste een beoordeling</a>
<?php endif; ?>

<?php if ($themes): ?>
Thema: <?php print $themes[0] ?><br/>
<?php endif; ?>

Korte beschrijving: <?php print $shortdescription; ?><br/>

<dl class="clearfix">

  <?php if ($has_location): ?>
  <dt>Waar</dt>
  <dd>
    <?php if (!empty($where_linked)): ?>
    <?php print $where_linked; ?><br/>
    <?php elseif (!empty($where)): ?>
    <?php print $where;?><br/>
    <?php endif; ?>
    <?php print $street ?>, <?php print $zip; ?> <?php print $city; ?>
  </dd>
  <?php endif; ?>

  <?php if (!empty($when)): ?>
  <dt>Wanneer</dt>
  <dd><?php print $when; ?></dd>
  <?php endif; ?>

  <?php if (!empty($organisation)): ?>
  <dt>Organisatie</dt>
  <dd><?php print $organisation; ?></dd>
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
  <? endif; ?>

  <?php if (!empty($price)): ?>
  <dt>Price</dt>
  <dd><?php print $price; ?><?php print $price_description; ?></dd>
  <?php endif; ?>

</dl>
