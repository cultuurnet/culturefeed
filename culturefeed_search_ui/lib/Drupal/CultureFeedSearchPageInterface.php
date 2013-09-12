<?php

interface CultureFeedSearchPageInterface {

  /**
   * Gets the default sortkey.
   * @return String $sortKey
   */
  public function getDefaultSort();

  /**
   * Gets the search facets.
   */
  public function getFacetComponent();

  /**
   * Gets the search result.
   */
  public function getSearchResult();

  /**
   * Sets the fullPage property.
   */
  public function setFullPage($fullPage);

  /**
   * Sets the default sortkey.
   * @param String $sortKey
   */
  public function setDefaultSort($sortKey);

  /**
   * Set the default title.
   * @param string $title
   */
  public function setDefaultTitle($title);

  /**
   * Sets the resultsPerPage property.
   */
  public function setResultsPerPage($resultsPerPage);

  /**
   * Sets the pagerType property.
   */
  public function setPagerType($pagerType);

  /**
   * Sets the search query.
   *
   * @param array $query
   *   The search query array.
   */
  public function setQuery(array $query);

  /**
   * Gets the search query.
   *
   * @return array
   *   The search query array.
   */
  public function getQuery();

  /**
   * Adds a search term to the search query.
   *
   * @param string $term
   *   The search term to add. This will be a required term.
   *
   * @return array
   *   The updated search query array.
   */
  public function addQueryTerm($term);

  /**
   * Gets the Apache Solr local parameters.
   *
   * @return array
   *   An array containing Apache Solr local parameters as key-value pairs.
   */
  public function getLocalParams();

  /**
   * Sets the Apache Solr local parameters.
   *
   * @param array $local_params
   *   An array containing Apache Solr local parameters as key-value pairs.
   */
  public function setLocalParams(array $localParams);

  /**
   * Sets an Apache Solr local parameter pair.
   *
   * @param string $key
   *   The local parameter key.
   * @param string $value
   *   The local parameter value.
   *
   * @return array
   *   The updated local parameters array.
   */
  public function setLocalParam($key, $value);

  /**
   * Unsets an Apache Solr local parameter pair.
   *
   * @param string $key
   *   The key of the local parameter to unset.
   *
   * @return array
   *   The updated local parameters array.
   */
  public function unsetLocalParam($key);

  /**
   * Add an Apache Solr input parameter.
   *
   * @param \CultuurNet\Search\Parameter\AbstractParameter $parameter
   *   The parameter to add.
   *
   * @return array
   *   The updated parameters array.
   */
  public function addParameter(\CultuurNet\Search\Parameter\AbstractParameter $parameter);

  /**
   * Unsets an Apache Solr input parameter.
   *
   * @param string $key
   *   The key of the parameter to unset.
   *
   * @return array
   *   The updated parameters array.
   */
  public function unsetParameter($key);

  /**
   * Initializes the search with data from the URL query parameters.
   */
  public function initialize();

  /**
   * Get the title to show.
   */
  public function getPageTitle();

  /**
   * Get the active trail to show.
   */
  public function getActiveTrail();

}
