
<?php if (count($tabs) > 1): ?>
<ul class="tabs">
<?php foreach ($tabs as $tab): ?>
  <li class="tab <?php print $tab['class']; ?>" ><?php print $tab['name']; ?></li>
<?php endforeach; ?>
</ul>
<?php endif; ?>

<?php foreach ($tabs as $tab): ?>
  <?php foreach ($tab['children'] as $content): ?>
    <div class="<?print $tab['class']; ?>">

      <a href="<?php print $content['url'] ?>"><?php print $content['title']; ?></a>
      <?php print $content['city']; ?>
      <?php print $content['calendar']; ?>
      <?php if (isset($content['all_url'])): ?>
      <a href="<?print $content['all_url']; ?>"><?php print t('Show all'); ?></a>
      <?php endif; ?>

    </div>
  <?php endforeach; ?>
<?php endforeach; ?>

