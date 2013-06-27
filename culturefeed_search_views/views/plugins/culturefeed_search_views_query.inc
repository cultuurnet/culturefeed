<?php
use \CultuurNet\Search\Parameter;

/**
 * @class
 * Views query plugin for culturefeed search api.
 */
class culturefeed_search_views_query extends views_plugin_query {

  private $has_query_search = FALSE;
  private $parameters = array();

  /**
   * Execute the query
   * @see views_plugin_query::execute()
   */
  function execute(&$view) {

    $start = microtime(TRUE);

    if (isset($this->where)) {
      $parameters = $this->build_search_parameters($this->where);
    }
    else {
      $parameters = array();
    }

    // pager
    /*if ($this->pager->use_pager()) {
      $pager_options = TRUE;
    }
    elseif ($this->pager->options['items_per_page']) {
      $pager_options = TRUE;
    }*/

    // sorting
    if (!empty($this->orderby)) {
    }

    //$this->start = $params['page'] * $this->resultsPerPage;
    //$this->parameters[] = new Parameter\Start($this->start);

    // Add items / page.
    //$this->parameters[] = new Parameter\Rows($this->resultsPerPage);

    // Add grouping so returned events are not duplicate.
    $this->parameters[] = new Parameter\Group();

    if (!$this->has_query_search) {
      $this->parameters[] = new Parameter\Query('*:*');
    }

    $searchService = culturefeed_get_search_service();
    $result = $searchService->search($this->parameters);

    if ($result->getTotalCount() > 0) {
      $view->result = $result->getItems();
      $view->total_rows = $result->getTotalCount();
      //$this->pager->set_total_items($result->getTotalCount());
      //$this->pager->update_page_info();
      //$view->pager['current_page'] = $this->pager->current_page;
    }

    $view->execute_time = microtime() - $start;

  }

  /**
   * Construct the all the parameters for the search request.
   */
  function build_search_parameters($where) {

    if (is_array($where)) {
      foreach ($where as $group => $group_where) {

        // Add query search for this group.
        if (!empty($group_where['conditions']['q'])) {
          $this->has_query_search = TRUE;
          $this->parameters[] = new Parameter\Query(implode(' ' . $group_where['type'] . ' ', $group_where['conditions']['q']));
        }

        // Add filter query search for this group.
        if (!empty($group_where['conditions']['fq'])) {
          $this->parameters[] = new Parameter\FilterQuery(implode(' ' . $group_where['type'] . ' ', $group_where['conditions']['fq']));
        }

      }
    }

  }

  function add_where($group, $operator, $value) {
    $this->where[$group]['conditions'][$operator][] = $value;
  }


}
