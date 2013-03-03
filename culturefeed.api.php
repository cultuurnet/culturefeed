<?php
/**
 * @file
 * Hook documentation for the culturefeed module.
 */

/**
 * Gets the shared secret for a given OAuth consumer application key.
 *
 * Modules can implement this hook when they want to provide their
 * own OAuth consumer credentials, instead of the single consumer
 * that is managed by the culturefeed module itself.
 *
 * In order to let users authorize a specific consumer, modules can
 * append their own application key as an additional argument to the
 * following oauth paths provided by the culturefeed module:
 * - culturefeed/oauth/connect
 * - culturefeed/oauth/connect/register
 *
 * @param string $application_key
 *   The application key of the OAuth consumer.
 *
 * @return string
 *   The shared secret of the OAuth consumer.
 */
function hook_culturefeed_consumer_shared_secret($application_key) {
  return 'shared secret';
}
