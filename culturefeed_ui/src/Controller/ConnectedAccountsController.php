<?php

/**
 * @file
 * Contains Drupal\culturefeed_ui\Controller\ConnectedAccountsController.
 */

namespace Drupal\culturefeed_ui\Controller;

use Drupal\Core\Controller\ControllerBase;
use Drupal\Core\Routing\RouteMatch;
use Psr\Log\LoggerAwareInterface;
use Psr\Log\LoggerAwareTrait;
use Psr\Log\LoggerInterface;
use Symfony\Component\DependencyInjection\ContainerInterface;
use CultureFeed_User;
use CultureFeed;
use CultureFeed_OnlineAccount;
use Symfony\Component\HttpFoundation\RedirectResponse;
use Symfony\Component\HttpFoundation\Request;

/**
 * Class ConnectedAccountsController.
 *
 * @package Drupal\culturefeed_ui\Controller
 */
class ConnectedAccountsController extends ControllerBase implements LoggerAwareInterface {

  use LoggerAwareTrait;

  /**
   * The culturefeed user service.
   *
   * @var CultureFeed_User;
   */
  protected $user;

  /**
   * The culturefeed service
   *
   * @var CultureFeed
   */
  protected $culturefeed;

  /**
   * {@inheritdoc}
   */
  public static function create(ContainerInterface $container) {
    return new static(
      $container->get('culturefeed.current_user'),
      $container->get('culturefeed'),
      $container->get('logger.channel.culturefeed')
    );
  }

  /**
   * Constructs a ProfileForm
   *
   * @param CultureFeed_User $user
   * @param CultureFeed $culturefeed
   * @param LoggerInterface $logger
   */
  public function __construct(
    CultureFeed_User $user,
    CultureFeed $culturefeed,
    LoggerInterface $logger
  ) {
    $this->user = $user;
    $this->culturefeed = $culturefeed;
    $this->setLogger($logger);
  }

  /**
   * Disconnect an external account from a culturefeed user.
   *
   * @param $account_type
   * @param $account_name
   * @param Request $request
   *
   * @return RedirectResponse
   */
  public function disconnect(Request $request, $account_type, $account_name) {
    $userId = $this->user->id;
    try {
      $this->culturefeed->deleteUserOnlineAccount($userId, $account_type,
        $account_name);
    } catch (\Exception $e) {
      if ($this->logger) {
        $this->logger->error(
          'An error occurred when trying to disconnect an external account.',
          array('exception' => $e)
        );
      }
      drupal_set_message($this->t('Error occurred'), 'error');
    };

    $route_match = RouteMatch::createFromRequest($request);
    return $this->redirect($route_match->getRouteName(), $route_match->getRawParameters()->all());
  }

  /**
   * Makes culturefeed activities private for a connected account.
   *
   * @param $account_type
   * @param $account_name
   * @param Request $request
   *
   * @return RedirectResponse
   */
  public function makePrivate(Request $request, $account_type, $account_name) {
    $userId = $this->user->id;
    $connectedAccount = $this->getConnectedAccount($account_type,
      $account_name);

    $connectedAccount->publishActivities = FALSE;
    $this->culturefeed->updateUserOnlineAccount($userId, $connectedAccount);

    $route_match = RouteMatch::createFromRequest($request);
    return $this->redirect($route_match->getRouteName(), $route_match->getRawParameters()->all());
  }

  /**
   * Make culturefeed activities public for a connected account.
   *
   * @param $account_type
   * @param $account_name
   * @param Request $request
   *
   * @return RedirectResponse
   */
  public function makePublic(Request $request, $account_type, $account_name) {
    $userId = $this->user->id;
    $connectedAccount = $this->getConnectedAccount($account_type,
      $account_name);

    $connectedAccount->publishActivities = TRUE;
    $this->culturefeed->updateUserOnlineAccount($userId, $connectedAccount);

    $route_match = RouteMatch::createFromRequest($request);
    return $this->redirect($route_match->getRouteName(), $route_match->getRawParameters()->all());
  }

  /**
   * @param $accountType
   * @param $accountName
   * @return CultureFeed_OnlineAccount
   */
  private function getConnectedAccount($accountType, $accountName) {
    $connectedAccount = new CultureFeed_OnlineAccount();
    $connectedAccount->accountName = $accountName;
    $connectedAccount->accountType = $accountType;

    return $connectedAccount;
  }

}
