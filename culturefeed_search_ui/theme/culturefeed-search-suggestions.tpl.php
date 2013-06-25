<?php
/**
 * @vars
 *   - $suggestions (key = search string, value = search url)
 */
?>
<di>
<h2 class="block-title">Bedoelde u: </h2>
<?php foreach ($suggestions as $suggestion_words => $suggestion_url): ?>
  <a href="<?php print $suggestion_url; ?>"><?php print $suggestion_words ?></a>
<?php endforeach;?>
</div>