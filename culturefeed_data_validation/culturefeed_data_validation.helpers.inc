<?php

/**
 * @file
 * Helper functions for culturefeed data validation.
 */

use CultuurNet\DataValidation\DataValidationClient;
use Guzzle\Http\Client;

/**
 * Get the datavalidation client.
 * @return \CultuurNet\DataValidation\DataValidationClient
 */
function _culturefeed_data_validation_get_datavalidation_client() {
  $apiKey = variable_get('culturefeed_data_validation_api_key');
  $guzzleClient = new Client();
  return new DataValidationClient($guzzleClient, $apiKey);
}