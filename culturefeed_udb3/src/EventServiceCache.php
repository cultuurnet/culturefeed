<?php
/**
 * @file
 */

namespace Drupal\culturefeed_udb3;


use CultuurNet\UDB3\EventServiceDecoratorBase;
use CultuurNet\UDB3\EventServiceInterface;
use Drupal\Core\Cache\CacheBackendInterface;

class EventServiceCache extends EventServiceDecoratorBase{

  /**
   * @var CacheBackendInterface
   */
  protected $cache;

  /**
   * @var int
   */
  protected $lifetime = 3600;

  public function __construct(
    EventServiceInterface $eventService,
    CacheBackendInterface $cache
  ) {
    parent::__construct($eventService);
    $this->cache = $cache;
  }

  public function getEvent($id) {
    $cacheItem = $this->cache->get($id);
    if (FALSE === $cacheItem) {
      $data = parent::getEvent($id);
      $expire = REQUEST_TIME + $this->lifetime;
      $this->cache->set($id, $data, $expire);
    }
    else {
      $data = $cacheItem->data;
    }

    return $data;
  }
} 
