<?php
/**
 * @file 
 * Exception for invalid search pages.
 *
 */
class InvalidSearchPageException extends Exception {
  
  public function __construct($message, $code = NULL) {
    $this->message = 'Searchresults could not be loaded.' . '<br />' . $message;
    watchdog('culturefeed_search_ui', $message, array(), WATCHDOG_CRITICAL);
  }
}