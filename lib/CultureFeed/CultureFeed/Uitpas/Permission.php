<?php
/**
 * Created by PhpStorm.
 * User: Kristof
 * Date: 14/12/13
 * Time: 09:32
 */

/**
 * Container for static permission metadata.
 *
 */
class CultureFeed_Uitpas_Permission
{
  /**
   * Permission 'PERMISSION_PASSHOLDER_OTHER_CITY - ACTION_CREATE'.
   */
  const PASSHOLDER_OTHER_CITY_ACTION_CREATE = 'PERMISSION_PASSHOLDER_OTHER_CITY - ACTION_CREATE';

  const PASSHOLDER_ACTION_UPDATE = 'PERMISSION_PASSHOLDER - ACTION_UPDATE';

  private function __construct() {}

  /**
   * Get all permissions relevant for a specific path on the uitpas API.
   *
   * @param string $path
   *   The path on the API, relative to the UiTPAS subsystem. For example: 'passholder/register'.
   *   Ensure you do not include the 'uitpas' prefix.
   * @param string $method
   *  The HTTP method, default 'POST'.
   *
   * @return array
   */
  public static function allRelevantFor($path, $method = "POST") {
    $permissions = array();

    switch ($path) {
      case 'passholder/register':
        $permissions[] = self::PASSHOLDER_OTHER_CITY_ACTION_CREATE;
        break;

      case 'passholder/{uitpasNumber}':
        $permissions[] = self::PASSHOLDER_ACTION_UPDATE;
        break;
    }

    return $permissions;
  }
}
