<?php

interface CultureFeedSearchPageInterface {
  public function setResultsPerPage($resultsPerPage);
  public function setFullPage($fullPage);
  public function setPagerType($pagerType);
  public function loadPage();
  public function getDrupalTitle();
}