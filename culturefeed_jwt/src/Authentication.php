<?php

namespace Drupal\culturefeed_jwt;

use CultuurNet\UDB3\Jwt\JwtDecoderService;
use Drupal\Core\Entity\EntityTypeManagerInterface;
use Drupal\Core\Entity\Query\QueryFactory;
use Drupal\culturefeed\UserMapInterface;
use Exception;
use Symfony\Component\HttpFoundation\Request;
use ValueObjects\String\String as StringLiteral;

/**
 * Class Authentication.
 *
 * @package Drupal\culturefeed_jwt
 */
class Authentication implements AuthenticationInterface {

  /**
   * The entity type manager.
   *
   * @var \Drupal\Core\Entity\EntityTypeManagerInterface;
   */
  protected $entityTypeManager;

  /**
   * The jwt decoder service.
   *
   * @var \CultuurNet\UDB3\Jwt\JwtDecoderService
   */
  protected $jwtDecoderService;

  /**
   * The user map.
   *
   * @var \Drupal\culturefeed\UserMapInterface
   */
  protected $userMap;

  /**
   * The query factory.
   *
   * @var \Drupal\Core\Entity\Query\QueryFactory;
   */
  protected $entityQuery;

  /**
   * Constructs a Authentication object.
   *
   * @param \CultuurNet\UDB3\Jwt\JwtDecoderService $jwt_decoder_service
   *   The decoder service.
   * @param \Drupal\culturefeed\UserMapInterface $user_map
   *   The user map.
   * @param EntityTypeManagerInterface $entity_type_manager
   *   The entity type manger.
   * @param QueryFactory $entity_query
   *   The query factory.
   */
  public function __construct(
      JwtDecoderService $jwt_decoder_service,
      UserMapInterface $user_map,
      EntityTypeManagerInterface $entity_type_manager,
      QueryFactory $entity_query
  ) {

    $this->jwtDecoderService = $jwt_decoder_service;
    $this->userMap = $user_map;
    $this->entityTypeManager = $entity_type_manager;
    $this->entityQuery = $entity_query;

  }

  /**
   * {@inheritdoc}
   */
  public function authorize(Request $request) {

    $query = $request->query;
    $token_string = $query->get('jwt');

    if ($token_string) {

      try {

        $jwt = $this->jwtDecoderService->parse(new StringLiteral($token_string));

        $cf_user = new \CultureFeed_User();
        $cf_user->id = $jwt->getClaim('uid');
        $cf_user->nick = $jwt->getClaim('nick');
        $cf_user->mbox = $jwt->getClaim('email');

      }
      catch (Exception $e) {
        drupal_set_message(t('An error occurred while logging in. Please try again later.'), 'error');
        watchdog_exception('culturefeed', $e);
        return;

      }

      $account = $this->userMap->get($cf_user);

      $storage = $this->entityTypeManager->getStorage('culturefeed_jwt_token');

      $query = $this->entityQuery->get('culturefeed_jwt_token')->condition('uitid', $cf_user->id);
      $result = $query->execute();
      $entities = $storage->loadMultiple(array_keys($result));
      $storage->delete($entities);

      $storage->create(array(
        'uitid' => $cf_user->id,
        'token' => $token_string,
      ))->save();

      if ($account) {
        user_login_finalize($account);
      }

    }

  }

}
