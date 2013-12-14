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

  private function __construct() {}

  /**
   * Get all permissions relevant for a specific path on the uitpas API.
   *
   * @param string $path
   *   The path on the API, relative to the UiTPAS subsystem. For example: 'passholder/register'.
   *   Ensure you do not include the 'uitpas' prefix.
   */
  public static function allRelevantFor($path) {
    $permissions = array();

    switch ($path) {
      case 'passholder/register':
        $permissions[] = self::PASSHOLDER_OTHER_CITY_ACTION_CREATE;
        break;

    }

    return $permissions;
  }
}
