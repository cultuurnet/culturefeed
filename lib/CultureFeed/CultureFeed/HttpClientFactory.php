<?php
/**
 * @file
 */ 

interface CultureFeed_HttpClientFactory {

  /**
   * Creates a new HTTP client instance.
   *
   * @return CultureFeed_HttpClient The new HTTP client instance.
   */
  public function createHttpClient();

}
