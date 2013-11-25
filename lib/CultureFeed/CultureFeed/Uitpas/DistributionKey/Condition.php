<?php

class CultureFeed_Uitpas_DistributionKey_Condition extends CultureFeed_Uitpas_ValueObject {

  const DEFINITION_KANSARM = 'KANSARM';

  const DEFINITION_PRICE = 'PRICE';

  const OPERATOR_IN = 'IN';

  const OPERATOR_LESS_THAN = 'LESS_THAN';

  /**
   * @var string The definition of the condition.
   */
  public $definition;

  /**
   * @var string The operator of the condition.
   */
  public $operator;

  /**
   * @var string The value of the condition.
   */
  public $value;

  public static function createFromXML(CultureFeed_SimpleXMLElement $object) {

    $condition = new self();

    $condition->definition = $object->xpath_str('definition');
    $condition->operator = $object->xpath_str('operator');
    $condition->value = $object->xpath_str('value');

    return $condition;

  }

}
