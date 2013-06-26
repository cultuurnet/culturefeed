<?php
/**
 * @file 
 * Template file for the list of promotions.
 */


$output = '<div class="row-fluid margin-bottom">';
$counter = 0;

foreach ($items as $item) {
  if($counter < 3) {
    $output .= '<div class="span4">' . $item . '</div>';
    $counter++;
  }
  else {
    $output .= '</div>';
    $output .= '<div class="row-fluid margin-bottom">';
    $output .= '<div class="span4">' . $item . '</div>';
    $counter = 1;
  }
}
$output .= '</div>';
?>

<h2>1. Kies één of meerdere geschenken</h2>
<div class="thumbnails">
  <?php print $output; ?>
</div>


