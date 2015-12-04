<?php

use \CultuurNet\Search\Parameter;

/**
 * A handler to provide proper displays for the location of a cdb item.
 */
class views_handler_field_cdb_item_location extends views_handler_field {

  /**
   * @see views_handler_field::option_definition()
   */
  function option_definition() {

    $options = parent::option_definition();
    $options['show'] = array('default' => array('city' => 'city'));

    return $options;

  }

  /**
   * @see views_handler_field::options_form()
   */
  function options_form(&$form, &$form_state) {

    $form['show'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Show'),
      '#default_value' => isset($this->options['show']) ? $this->options['show'] : array('title'),
      '#options' => array(
        'title' => t('Title'),
        'city' => t('City'),
        'address' => t('Address'),
      ),
      '#required' => TRUE,
    );

    parent::options_form($form, $form_state);

  }

  /**
   * @see views_handler_field::render()
   */
  function render($extended_entity) {

    $entity_type = $extended_entity->getType();
    $cdb_item = $extended_entity->getEntity();
    $location = array();

    if ($entity_type == 'event') {
      $location = culturefeed_agenda_get_location_of_event($extended_entity->getEntity());
    }
    // Use the preprocess function, we don't want to program that logic 2 times.
    elseif ($entity_type == 'production') {

      $relations = $cdb_item->getRelations();
      $total_relations = count($relations);
      // No event data.
      if ($total_relations == 0) {
        return;
      }
      // Multiple events. No fixed location.
      elseif ($total_relations > 1) {
        $location['title'] = 'meerdere locaties';
      }
      // 1 event. Use events location.
      else {

        try {
          $event = culturefeed_agenda_event_load(current($relations)->getCdbid());
          $location = culturefeed_agenda_get_location_of_event($event->getEntity());

        }
        catch (Exception $e) {
          watchdog_exception('culturefeed_search_views', $e);
        }

      }

    }

    // Actor = show the contact info address.
    else {

      if ($cdb_item->getContactInfo()) {

        $contact_info = $cdb_item->getContactInfo();
        $addresses = $contact_info->getAddresses();

        if ($addresses) {

          $location['title'] = $addresses[0]->getLabel();
          $physical_address = $addresses[0]->getPhysicalAddress();
          if ($physical_address) {
            $location['city'] = check_plain($physical_address->getCity());
            $location['street'] = check_plain($physical_address->getStreet() . ' ' . $physical_address->getHouseNumber());
            $location['zip'] = check_plain($physical_address->getZip());
          }

        }
      }

    }

    $show = $this->options['show'];

    $output = array();
    if (!empty($show['title']) && !empty($location['title'])) {
      $output[] = $location['title'];
    }

    if (!empty($show['address'])) {
      $output[] = $location['street'] . ' ' . $location['zip'] . ' ' . $location['city'];
    }
    elseif (!empty($show['city']) && !empty($location['city'])) {
      $output[] = $location['city'];
    }

    return implode(' ', $output);

  }

  /**
   * Called to determine what to tell the clicksorter.
   */
  function click_sort($order) {

    $parameter = new Parameter\Sort('location_contactinfo_city', strtolower($order));
    $this->query->add_parameter($parameter);

  }

}
