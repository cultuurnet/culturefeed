<?php
/**
 * @file 
 * Defines page callbacks which access the Search api.
 */


/**
 * Page callback to search.
 */
function culturefeed_agenda_search_page() {
  
  global $facetingComponent, $searchpoc_facets;
  
  $args = $_GET;
  $params = drupal_get_query_parameters();
  
  $limit = 10;
  
  $params += array(
    'sort' => 'relevancy',
    // @todo check if this is the right query parameter used by the drupal pager system
    'page' => 0,
    'search' => '',
    'facet' => array(),
  );
  
  $searchService = searchpoc_get_search_service();
  
  $facetingComponent = new \CultuurNet\Search\Component\Facet\FacetComponent;

  $start = $params['page'] * $limit;
  $parameters = array();
  $parameters[] = new \CultuurNet\Search\Parameter\Group();
  $parameters[] = new \CultuurNet\Search\Parameter\Rows($limit);
  $parameters[] = new \CultuurNet\Search\Parameter\Start($start);
  
  $q = array();
  
  $parameters[] = new \CultuurNet\Search\Parameter\FilterQuery('type:event');
  
  switch ($params['sort']) {
  
    case 'date':
      $parameters[] = new \CultuurNet\Search\Parameter\Sort('startdate', \CultuurNet\Search\Parameter\Sort::DIRECTION_ASC);
      break;
  
    case 'recommends':
      $parameters[] = new \CultuurNet\Search\Parameter\Sort('recommend_count', \CultuurNet\Search\Parameter\Sort::DIRECTION_DESC);
      break;
  
    case 'comments':
      $parameters[] = new \CultuurNet\Search\Parameter\Sort('comment_count', \CultuurNet\Search\Parameter\Sort::DIRECTION_DESC);
      break;
  
    default:
      // Ellen: tegelijkertijd boosten op recommend_count op bvb factor 500 en comment_count op pakweg 999
      // Erwin: met deze query zoek ik ALLE data (ik gebruik text:*) EN (waarvan de like_count 0 is OF
      // waarvan recomment_count groter dan 0 is, en deze laatste clausule geboost met 999)
      // http://searchv2.cultuurnet.lodgon.com/search-poc/rest/search?q=text:* AND (recommend_count:0 OR
      // recommend_count:[1 TO *]^999)&group=true&rows=10
  
      $q[] = '(recommend_count:0 OR recommend_count:[1 TO *]^500)';
      $q[] = '(comment_count:0 OR comment_count:[1 TO *]^999)';
  }
  
  $themeFacet = $facetingComponent->facetField('category_theme_name');
  $themeFacet->setExcludes(array('category_theme_name'));
  $whereFacet = $facetingComponent->facetField('city');
  $whereFacet->setExcludes(array('city'));
  
  foreach ($params['facet'] as $facetFieldName => $facetFilter) {
    // @todo check escaping properly
    array_walk($facetFilter, function (&$item) {
      $item = '"' . str_replace('"', '\"', $item) . '"';
    });
  
    $facetFilterQuery = new \CultuurNet\Search\Parameter\FilterQuery($facetFieldName . ':(' . implode(' OR ', $facetFilter) . ')');
    // tag this so we can exclude when calculating facets
    $facetFilterQuery->setTags(array($facetFieldName));

    $parameters[] = $facetFilterQuery;
  }
  
  $parameters[] = $themeFacet;
  $parameters[] = $whereFacet;
  
  if ('' == $params['search']) {
    $params['search'] = '*';
  }
  $q[] = 'text:(' . str_replace(')', '\\)', $params['search']) . ')';
  $parameters[] = new \CultuurNet\Search\Parameter\Query(implode(' AND ', $q));
  
  $result = $searchService->search($parameters);
  
  // store faceting component in global, for use in blocks
  $facetingComponent->obtainResults($result);
  
  pager_default_initialize($result->getTotalCount(), $limit);
  
  $build = array();

  $build['results'] = array(
    '#theme' => 'culturefeed_search_page',
    '#searchresult' => $result,
    //'#type' => 'markup',
    //'#markup' => theme('culturefeed_search_page', array('searchresult' => $results)),
  );
  
  if ($result->getTotalCount() > 0) {

    $end = $start + $result->getCurrentCount();
    $args = array(
      '@range' => ($start + 1) . '-' . $end,
    );
    $pager_summary = format_plural($result->getTotalCount(), '@range van @count resultaat', '@range van @count resultaten', $args);
    $pager_summary = '<div class="pager-summary">' . $pager_summary . '</div>';
  
    $build['pager-container'] =  array(
      '#type' => 'container',
      '#attributes' => array(),
      'pager_summary' => array(
        '#type' => 'markup',
        '#markup' => $pager_summary,
      ),
      'pager' => array(
        '#type' => 'markup',
        '#markup' => theme('pager', array('quantity' => 5)),
      ),
    );
  }
  
  return $build;
}
