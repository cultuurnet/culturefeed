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
   * The default operator to use when connecting the WHERE groups. May be
   * AND or OR.
   */
  var $group_operator = 'AND';

  /**
   * add_field method to support field row plugin and click sorting.
   */
  function add_field($table, $field, $alias = '', $params = array()) {
    return $field;
  }

  /**
   * {@inheritdoc}
   */
  function add_orderby($table, $field = NULL, $order = 'ASC', $alias = '', $params = array()) {
    $parameter = new Parameter\Sort($alias, strtolower($order));
    $this->add_parameter($parameter);
  }

  /**
   * return $parameters
   */
  function getParameters() {
    return $this->parameters;
  }

  /**
   * @see views_plugin_query::build()
   */
  public function build(&$view) {

    $view->init_pager();

    // Let the pager modify the query to add limits.
    $this->pager->query();

    if ($this->pager->use_pager()) {
      $this->pager->set_current_page($view->current_page);
    }

  }

  /**
   * Execute the query
   * @see views_plugin_query::execute()
   */
  function execute(&$view) {

    $start = microtime(TRUE);

    // Add grouping so returned events are not duplicate.
    $this->parameters[] = new Parameter\Group();

    if (isset($this->where)) {
      $this->build_search_parameters($this->where);
    }

    // Add pager parameters.
    $items_per_page = 9999; // Server doesn't support: show all
    if ($this->pager->use_pager()) {

      $items_per_page = $this->pager->options['items_per_page'];

      // Add start index (page number we want)
      $curr_page = isset($_GET['page']) ? $_GET['page'] : 0;
      $start = $curr_page * $items_per_page;
      if (!empty($this->pager->options['offset'])) {
        $start += $this->pager->options['offset'];
      }
      $this->parameters[] = new Parameter\Start($start);

    }
    elseif ($this->pager->options['items_per_page']) {
      $items_per_page = $this->pager->options['items_per_page'];
    }

    // Add items / page.
    $this->parameters[] = new Parameter\Rows($items_per_page);

    // Make sure there is at least 1 q=
    if (!$this->has_query_search) {
      $this->parameters[] = new Parameter\Query('*:*');
    }

    $searchService = culturefeed_get_search_service();

    try {

      $result = $searchService->search($this->parameters);

      // Give the result to the view.
      $view->result = $result->getItems();
      $view->total_rows = $result->getTotalCount();
      $this->pager->total_items = $result->getTotalCount();
      $this->pager->update_page_info();
      $view->execute_time = microtime() - $start;

    }
    catch (Exception $e) {
      watchdog_exception(WATCHDOG_ERROR, $e);
    }

  }

  /**
   * Construct the all the parameters for the search request.
   */
  function build_search_parameters($where) {

    $fq_groups = array();
    if (is_array($where)) {
      foreach ($where as $group => $group_where) {

        // Add query search for this group.
        if (!empty($group_where['conditions']['q'])) {
          $this->has_query_search = TRUE;
          $this->parameters[] = new Parameter\Query(implode(' ' . $group_where['type'] . ' ', $group_where['conditions']['q']));
        }

        // Add filter query search for this group.
        if (!empty($group_where['conditions']['fq'])) {
          $fq_groups[] = '(' . implode(' ' . $group_where['type'] . ' ', $group_where['conditions']['fq']) . ')';
        }

      }
    }

    $this->parameters[] = new Parameter\FilterQuery(implode($fq_groups, ' ' . $this->group_operator . ' '));

  }

  /**
   * Add an additional parameter to the query.
   */
  function add_parameter($parameter) {
    $this->parameters[] = $parameter;
  }

  function add_where($group, $operator, $value) {
    // Check for a group.
    if (!isset($this->where[$group])) {
      $this->set_where_group('AND', $group);
    }

    $this->where[$group]['conditions'][$operator][] = $value;
  }

}
