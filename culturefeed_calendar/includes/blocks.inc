<?php
/**
 * @file
 * Block callbacks for culturefeed calendar.
 */

/**
 * Block to show the culturefeed calendar teaser.
 */
function culturefeed_calendar_teaser_block() {

  $block['content'] = theme('culturefeed_calendar_teaser');
  
  return $block;
}