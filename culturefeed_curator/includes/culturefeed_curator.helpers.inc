<?php
/**
 * @file
 * Settings file for culturefeed entry ui.
 */

/**
 * Gets the curator API client as a singleton.
 *
 * @param bool $use_cache
 * @return DrupalCultureFeedCuratorClient
 */
function culturefeed_get_curator_client($use_cache = TRUE) {
  return DrupalCultureFeedCuratorClient::getClient($use_cache);
}
