<?php

/**
 * @file
 * Contains Drupal\culturefeed_search\Controller\AutocompleteController.
 */

namespace Drupal\culturefeed_search\Controller;

use Drupal\Core\Controller\ControllerBase;
use Symfony\Component\HttpFoundation\JsonResponse;



/**
 * Class AutocompleteController.
 *
 * @package Drupal\culturefeed_search\Controller
 */
class AutocompleteController extends ControllerBase {

  /**
   * Page callback for the city suggestions autocomplete.
   */
  function cityAutocompletePage($search_string) {

    $matches = array();
    if ($search_string) {

      $query = db_select('culturefeed_search_cities', 'csc');
      $query->fields('csc', array('cid', 'name', 'zip'));
      $query->condition('cid', '%' . db_like($search_string) . '%', 'LIKE');
      $query->condition('cid', '%(' . db_like($search_string) . '%)', 'NOT LIKE');

      $result = $query->execute();

      foreach ($result as $row) {
        $matches[] = array('key' => $row->cid,'label' => $row->zip . ' ' . $row->name);
      }

    }

    return new JsonResponse($matches);
  }

  /**
   * Page callback for the locations suggestions autocomplete.
   */
  function locationsForCityAutocompletePage($search_string) {

    $matches = array();
    if ($search_string) {

      $query = db_select('culturefeed_search_cities', 'csc');
      $query->fields('csc', array('name', 'zip'));
      $query->condition('cid', '%' . db_like($search_string) . '%', 'LIKE');
      $query->condition('cid', '%(' . db_like($search_string) . '%)', 'NOT LIKE');

      $result = $query->execute();

      foreach ($result as $row) {
        $matches[] = $row->zip . ' ' . $row->name;
      }

    }

    return new JsonResponse($matches);
  }

}




