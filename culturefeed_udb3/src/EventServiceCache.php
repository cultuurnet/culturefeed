<?php
/**
 * @file
 * Contains Drupal\culturefeed_udb3\EventServiceCache.
 */

namespace Drupal\culturefeed_udb3;

use CultuurNet\UDB3\EventServiceDecoratorBase;
use CultuurNet\UDB3\EventServiceInterface;
use Drupal\Core\Cache\CacheBackendInterface;

/**
 * Class EventServiceCache.
 *
 * @package Drupal\culturefeed_udb3
 */
class EventServiceCache extends EventServiceDecoratorBase {

  /**
   * The cache backend.
   *
   * @var CacheBackendInterface
   */
  protected $cache;

  /**
   * The cache lifetime.
   *
   * @var int
   */
  protected $lifetime = 3600;

  /**
   * Constructs a new EventsServiceCache.
   *
   * @param EventServiceInterface $event_service
   *   The event service.
   * @param CacheBackendInterface $cache
   *   The cache backend.
   */
  public function __construct(
    EventServiceInterface $event_service,
    CacheBackendInterface $cache
  ) {
    parent::__construct($event_service);
    $this->cache = $cache;
  }

  /**
   * Returns a cached event.
   *
   * @param string $id
   *   The event id.
   *
   * @return mixed
   *   The event.
   */
  public function getEvent($id) {
    $cache_item = $this->cache->get($id);
    if (FALSE === $cache_item) {
      $data = parent::getEvent($id);
      $expire = REQUEST_TIME + $this->lifetime;
      $this->cache->set($id, $data, $expire);
    }
    else {
      $data = $cache_item->data;
    }

    return $data;
  }

}
