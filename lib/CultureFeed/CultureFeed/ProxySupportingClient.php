<?php
/**
 * @file
 */

interface CultureFeed_ProxySupportingClient {

  /**
   * Set the proxy server URI.
   *
   * @param string $proxy_server
   */
  public function setProxyServer($proxy_server);

  /**
   * Set the proxy server port.
   *
   * @param integer $proxy_port
   */
  public function setProxyPort($proxy_port);

  /**
   * Set the proxy server username.
   *
   * @param string $proxy_username
   */
  public function setProxyUsername($proxy_username);

  /**
   * Set the proxy server password
   *
   * @param string $proxy_password
   */
  public function setProxyPassword($proxy_password);

}
