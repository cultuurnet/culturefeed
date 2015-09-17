<?php
/**
 * @vars
 *   - $suggestions (key = search string, value = search url)
 */
?>
<p>
  <strong><?php print t('Did you mean'); ?></strong>
  <a class="parent-region-suggestion-link" href="<?php print $parent_region_suggestion_url; ?>">
  <?php print $parent_region_suggestion ?> (+  <?php print t('boroughs') ?>)
  </a>
</p>
