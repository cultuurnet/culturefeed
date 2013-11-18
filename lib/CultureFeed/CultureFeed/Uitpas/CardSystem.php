<?php
/**
 * @file
 */

class CultureFeed_Uitpas_CardSystem
{
    /**
     * @var integer Unique ID of the card system.
     */
    public $id;

    /**
     * @var string Name of the card system.
     */
    public $name;

    public function createFromXML(CultureFeed_SimpleXMLElement $object) {
        $card_system = new self();

        $card_system->id = $object->xpath_int('id');
        $card_system->name = $object->xpath_str('name');
    }
}
