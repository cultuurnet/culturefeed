<?php

/**
 * @file
 * Contains Drupal\culturefeed_ui\Controller\ConnectedAccountsController.
 */

namespace Drupal\culturefeed_ui\Controller;

use Drupal\Core\Controller\ControllerBase;
use Psr\Log\LoggerAwareInterface;
use Psr\Log\LoggerAwareTrait;
use Psr\Log\LoggerInterface;
use Symfony\Component\DependencyInjection\ContainerInterface;
use CultureFeed_User;
use CultureFeed;
use CultureFeed_OnlineAccount;
use Symfony\Component\HttpFoundation\RedirectResponse;

/**
 * Class ConnectedAccountsController.
 *
 * @package Drupal\culturefeed_ui\Controller
 */
class ConnectedAccountsController extends ControllerBase implements LoggerAwareInterface
{
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
     *
     * @return RedirectResponse
     */
    public function disconnect($account_type, $account_name)
    {
        $userId = $this->user->id;
        try {
            $this->culturefeed->deleteUserOnlineAccount($userId, $account_type, $account_name);
        }
        catch (Exception $e) {
            $this->logger->error(
              'An error occurred when trying to disconnect an external account.',
              array('exception' => $e)
            );
            drupal_set_message(t('Error occurred'), 'error');
        };

        return $this->redirect('culturefeed_ui.account_form');
    }

    public function makePrivate($account_type, $account_name)
    {
        $userId = $this->user->id;
        $connectedAccount = $this->getConnectedAccount($account_type, $account_name);

        $connectedAccount->publishActivities = false;
        $this->culturefeed->updateUserOnlineAccount($userId, $connectedAccount);

        return $this->redirect('culturefeed_ui.account_form');
    }

    public function makePublic($account_type, $account_name)
    {
        $userId = $this->user->id;
        $connectedAccount = $this->getConnectedAccount($account_type, $account_name);

        $connectedAccount->publishActivities = true;
        $this->culturefeed->updateUserOnlineAccount($userId, $connectedAccount);

        return $this->redirect('culturefeed_ui.account_form');
    }

    /**
     * @param $accountType
     * @param $accountName
     * @return CultureFeed_OnlineAccount
     */
    private function getConnectedAccount($accountType, $accountName) {
        $connectedAccount = new CultureFeed_OnlineAccount();
        $connectedAccount->accountName = $accountType;
        $connectedAccount->accountType = $accountName;

        return $connectedAccount;
    }

}
