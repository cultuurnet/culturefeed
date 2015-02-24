<?php
/**
 * @file
 * Template for showing the manage list of saved searches.
 * The id 'saved-searches-messages' is required for messages to popup.
 */
?>

<?php if (!empty($items)): ?>

  <div id="saved-searches-messages"></div>

  <table>
    <?php foreach ($items as $item): ?>
    <tr>
      <td>
        <a href="<?php print $item['search_url']; ?>"><?php print $item['title']; ?></a>
      </td>
      <td>
        <?php print t('alert frequency') ?>:
        <?php print drupal_render($item['form']); ?>
      </td>
      <td>
        <a href="<?php print $item['delete_url']; ?>"><?php print t('Delete'); ?></a>
      </td>

    </tr>
    <?php endforeach; ?>
  </table>

<?php else: ?>

  <p><?php print t('There are no saved searches yet') ?></p>

<?php endif; ?>

