<?php
/**
 * @vars
 *   - $suggestions (key = search string, value = search url)
 */
?>
<p>
  <strong><?php print t('Did you mean'); ?></strong>
  <?php foreach ($suggestions as $suggestion_words => $suggestion_url): ?>
    <a href="<?php print $suggestion_url; ?>"><?php print $suggestion_words ?></a>
    <?php endforeach;?>
</p>
