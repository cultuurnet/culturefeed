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

/**
 * Is the given email a valid email.
 *
 * @param $email
 *   Email to validate.
 */
function culturefeed_data_validation_is_valid_email($email) {

  try {
    $datavalidationClient = _culturefeed_data_validation_get_datavalidation_client();
    $response = $datavalidationClient->validateEmail($email);
    if ($response && (!$response->isOK() || $response->getGrade() === 'F') ) {
      return FALSE;
    }
  }
  catch (Exception $e) {
    // Only trigger an error if the value was the cause.
    if ($e->getResponse()->getStatusCode() == 422) {
      return FALSE;
    }
  }

  return TRUE;
}