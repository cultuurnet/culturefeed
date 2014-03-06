<?php
/**
 * Created by PhpStorm.
 * User: Kristof
 * Date: 13/12/13
 * Time: 20:25
 */

class CultureFeed_AccessDeniedException extends CultureFeed_Exception
{
  /**
   * @var string
   */
  public $requiredPermission;
}
