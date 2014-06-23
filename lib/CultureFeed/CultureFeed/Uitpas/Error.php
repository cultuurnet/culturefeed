<?php
/**
 * @file
 */

/**
 * Container for error codes.
 */
class CultureFeed_Uitpas_Error
{
  const ACCESS_DENIED = 'ACCESS_DENIED';

  const ACTION_FAILED = 'ACTION_FAILED';

  const MISSING_REQUIRED_FIELDS = 'MISSING_REQUIRED_FIELDS';

  const INSZ_ALREADY_USED = 'INSZ_ALREADY_USED';

  const EMAIL_ALREADY_USED = 'EMAIL_ALREADY_USED';

  const UNKNOWN_VOUCHER = 'UNKNOWN_VOUCHER';

  const UNKNOWN_CARD = 'UNKNOWN_CARD';

  const CARD_NOT_ASSIGNED_TO_BALIE = 'CARD_NOT_ASSIGNED_TO_BALIE';

  const INVALID_CARD = 'INVALID_CARD';

  const INVALID_CARD_STATUS = 'INVALID_CARD_STATUS';

  const INVALID_VOUCHER_STATUS = 'INVALID_VOUCHER_STATUS';

  const UNKNOWN_SCHOOL = 'UNKNOWN_SCHOOL';

  const PARSE_INVALID_CITY_NAME = 'PARSE_INVALID_CITY_NAME';

  const PARSE_INVALID_INSZ = 'PARSE_INVALID_INSZ';

  const PARSE_INVALID_UITPASNUMBER = 'PARSE_INVALID_UITPASNUMBER';

  const PARSE_INVALID_VOUCHERNUMBER = 'PARSE_INVALID_VOUCHERNUMBER';

  const PARSE_INVALID_GENDER = 'PARSE_INVALID_GENDER';

  const PARSE_INVALID_DATE = 'PARSE_INVALID_DATE';

  const PARSE_INVALID_DATE_OF_BIRTH = 'PARSE_INVALID_DATE_OF_BIRTH';

  const BALIE_NOT_AUTHORIZED = 'BALIE_NOT_AUTHORIZED';

  const PARSE_INVALID_BIGDECIMAL = 'PARSE_INVALID_BIGDECIMAL';

  // Undocumented. Not sure this still applies to any method.
  const INVALID_NUMBER = 'INVALID_NUMBER';

  // Undocumented. Not sure this still applies to any method.
  const INVALID_CITY_NAME = 'INVALID_CITY_NAME';

  const ACTION_NOT_ALLOWED = 'ACTION_NOT_ALLOWED';

  const UNKNOWN_UITPASNUMBER = 'UNKNOWN_UITPASNUMBER';

  const UNKNOWN_BALIE_CONSUMERKEY = 'UNKNOWN_BALIE_CONSUMERKEY';

  const INVALID_PARAMETERS = 'INVALID_PARAMETERS';

  const UNKNOWN_CHIPNUMBER = 'UNKNOWN_CHIPNUMBER';

  const INVALID_DATE_CONSTRAINTS = 'INVALID_DATE_CONSTRAINTS';

  public static function allRelevantFor($path, $method = 'POST') {
    $errors = array();

    switch ($path) {

      case 'passholder/register':
        $errors[] = self::ACTION_FAILED;
        $errors[] = self::MISSING_REQUIRED_FIELDS;
        $errors[] = self::INSZ_ALREADY_USED;
        $errors[] = self::EMAIL_ALREADY_USED;
        $errors[] = self::UNKNOWN_VOUCHER;
        $errors[] = self::UNKNOWN_CARD;
        $errors[] = self::CARD_NOT_ASSIGNED_TO_BALIE;
        $errors[] = self::INVALID_CARD;
        $errors[] = self::ACCESS_DENIED;
        $errors[] = self::INVALID_CARD_STATUS;
        $errors[] = self::INVALID_VOUCHER_STATUS;
        $errors[] = self::UNKNOWN_SCHOOL;
        $errors[] = self::PARSE_INVALID_CITY_NAME;
        $errors[] = self::PARSE_INVALID_INSZ;
        $errors[] = self::PARSE_INVALID_UITPASNUMBER;
        $errors[] = self::PARSE_INVALID_VOUCHERNUMBER;
        $errors[] = self::PARSE_INVALID_GENDER;
        $errors[] = self::PARSE_INVALID_DATE;
        $errors[] = self::PARSE_INVALID_DATE_OF_BIRTH;
        $errors[] = self::BALIE_NOT_AUTHORIZED;
        // Undocumented. Not sure this still applies to passholder/register.
        $errors[] = self::PARSE_INVALID_BIGDECIMAL;
        // Undocumented. Not sure this still applies to passholder/register.
        $errors[] = self::INVALID_NUMBER;
        // Undocumented. Not sure this still applies to passholder/register.
        $errors[] = self::INVALID_CITY_NAME;
        // Undocumented.
        $errors[] = self::INVALID_DATE_CONSTRAINTS;

        break;

      case 'passholder/{uitpasNumber}':
        $errors[] = self::ACTION_NOT_ALLOWED;
        $errors[] = self::MISSING_REQUIRED_FIELDS;
        $errors[] = self::UNKNOWN_UITPASNUMBER;
        $errors[] = self::PARSE_INVALID_INSZ;
        $errors[] = self::PARSE_INVALID_UITPASNUMBER;
        $errors[] = self::PARSE_INVALID_BIGDECIMAL;
        $errors[] = self::PARSE_INVALID_GENDER;
        $errors[] = self::PARSE_INVALID_DATE;
        $errors[] = self::PARSE_INVALID_DATE_OF_BIRTH;
        break;

      case 'uitpas/passholder/eventActions':
        if ($method == 'GET') {
          $errors[] = self::UNKNOWN_BALIE_CONSUMERKEY;
          $errors[] = self::PARSE_INVALID_UITPASNUMBER;
          $errors[] = self::INVALID_PARAMETERS;
          $errors[] = self::MISSING_REQUIRED_FIELDS;
          $errors[] = self::UNKNOWN_UITPASNUMBER;
          $errors[] = self::UNKNOWN_CHIPNUMBER;
          $errors[] = self::INVALID_CARD_STATUS;
        }
        else {
          // POST not yet implemented.
        }
        break;

      case 'uitpas/card':
        if ($method == 'GET') {
          $errors[] = self::MISSING_REQUIRED_FIELDS;
          $errors[] = self::UNKNOWN_CHIPNUMBER;
        }
        break;

    }

    return $errors;
  }

}
