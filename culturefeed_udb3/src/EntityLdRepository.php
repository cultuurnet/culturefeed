<?php

/**
 * @file
 * Contains \Drupal\culturefeed_udb3\EntityLdRepository.
 */

namespace Drupal\culturefeed_udb3;

use CultuurNet\UDB3\Event\ReadModel\DocumentRepositoryInterface;
use CultuurNet\UDB3\Event\ReadModel\JsonDocument;
use Drupal\Core\Cache\CacheBackendInterface;

/**
 * Class EntityLdRepository.
 *
 * @package Drupal\culturefeed_udb3
 */
class EntityLdRepository implements DocumentRepositoryInterface {

  /**
   * The cache.
   *
   * @var \Drupal\Core\Cache\CacheBackendInterface
   */
  protected $cache;

  /**
   * Constructs the entity ld repository.
   *
   * @param CacheBackendInterface $cache
   *   The cache.
   */
  public function __construct(CacheBackendInterface $cache) {
    $this->cache = $cache;
  }

  /**
   * {@inheritdoc}
   */
  public function get($id) {

    $cache_item = $this->cache->get($id);
    if (FALSE === $cache_item) {
      return NULL;
    }
    else {
      $value = $cache_item->data;
      return new JsonDocument($id, $value);
    }

  }

  /**
   * {@inheritdoc}
   */
  public function save(JsonDocument $document) {

    $this->cache->set($document->getId(), $document->getRawBody());

  }

}
