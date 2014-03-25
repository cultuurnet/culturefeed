<?php
/**
 * $class_name : The name of the selected class "active"
 * $classes : A String of class names by core (typically machine names).
 * $class_names : A String of (derived) class names.
 *   E.G. message message-READ active
 */

?>
<a href="<?php print $url; ?>" class="<?php print $class_names ?>">
  <?php $status == 'NEW' ? print '<span class="new">' . t('New') . '</span>' : '' ?>
  <strong><?php print $sender; ?></strong><br />
  <?php isset($recipient_page) ? print t('To') . ':' . ' ' . $recipient_page . '<br />' : '' ; ?>
  <small><?php print $date; ?></small>
</a>