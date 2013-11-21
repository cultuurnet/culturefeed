<?php
/**
 * @file
 */ 

/**
 * Card system info enriched with data for a specific employee.
 */
class CultureFeed_Uitpas_Counter_EmployeeCardSystem extends CultureFeed_CardSystem {

  /**
   * The permissions of the member in the card system.
   *
   * @var array
   */
  public $permissions = array();

  /**
   * The groups of the memeber in the card system.
   *
   *  @var array
   */
  public $groups;

  public function createFromXml(CultureFeed_SimpleXMLElement $object) {
    $instance = new static();

    foreach ($object->xpath('permissions/permission') as $permission) {
      $instance->permissions[] = (string) $permission;
    }

    foreach ($object->xpath('groups/group') as $group) {
      $instance->groups[] = (string) $group;
    }
  }
}
