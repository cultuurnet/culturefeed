<?php

namespace Drupal\culturefeed_ui\Form;

use Drupal\Core\Entity\EntityManager;
use Drupal\Core\Form\ConfirmFormBase;
use Drupal\Core\Form\FormStateInterface;
use Drupal\Core\Session\AccountProxyInterface;
use Drupal\user\UserInterface;
use Drupal\Core\Url;
use Psr\Log\LoggerAwareInterface;
use Psr\Log\LoggerAwareTrait;
use Psr\Log\LoggerInterface;
use Symfony\Component\DependencyInjection\ContainerInterface;
use CultureFeed;
use CultureFeed_User;

class DeleteAccountForm extends ConfirmFormBase implements LoggerAwareInterface {

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
   * @var UserInterface
   */
  protected $drupalUser;

  /**
   * @var EntityManager
   */
  protected $entityManager;

  /**
   * {@inheritdoc}
   */
  public static function create(ContainerInterface $container) {
    return new static(
      $container->get('culturefeed.current_user'),
      $container->get('culturefeed'),
      $container->get('logger.channel.culturefeed'),
      $container->get('current_user'),
      $container->get('entity.manager')
    );
  }

  /**
   * Constructs a ProfileForm
   *
   * @param CultureFeed_User $user
   * @param CultureFeed $culturefeedService
   * @param LoggerInterface $logger
   * @param AccountProxyInterface $drupalUser
   * @param EntityManager $entityManager
   */
  public function __construct(
    CultureFeed_User $user,
    CultureFeed $culturefeedService,
    LoggerInterface $logger,
    AccountProxyInterface $drupalUser,
    EntityManager $entityManager
  ) {
    $this->user = $user;
    $this->culturefeed = $culturefeedService;
    $this->setLogger($logger);
    $this->drupalUser = $entityManager->getStorage('user')->load($drupalUser->id());
    $this->entityManager = $entityManager;
  }

  /**
   * {@inheritdoc}
   */
  public function getQuestion() {
    return $this->t('Are you sure you want to delete your account?');
  }

  /**
   * {@inheritdoc}
   */
  public function getCancelUrl() {
    $destination = $this->getRequest()->get('destination');
    if ($destination) {
      $cancelUrl = Url::fromUri($destination);
    }
    else {
      $cancelUrl = new Url('culturefeed_ui.account_form');
    }

    return $cancelUrl;
  }

  /**
   * {@inheritdoc}
   */
  public function getDescription() {
    return $this->t('This action cannot be undone.');
  }

  /**
   * {@inheritdoc}
   */
  public function getConfirmText() {
    return $this->t('Delete');
  }

  /**
   * {@inheritdoc}
   */
  public function getCancelText() {
    return $this->t('Cancel');
  }

  /**
   * {@inheritdoc}
   */
  public function getFormId() {
    return 'delete_account_form';
  }

  /**
   * {@inheritdoc}
   */
  public function submitForm(array &$form, FormStateInterface $form_state) {
    $uitId = $this->user->id;

    try {
      $this->culturefeed->deleteUser($uitId);
    } catch (\Exception $e) {
      if ($this->logger) {
        $this->logger->error(
          'Error occurred while deleting user account',
          array('exception' => $e)
        );
      }
      drupal_set_message($this->t('An error occurred while deleting your account.'));
      return;
    }

    db_query("DELETE FROM {culturefeed_token} WHERE uitid = :uitid", array(':uitid' => $uitId));
    db_query("DELETE FROM {culturefeed_user} WHERE uitid = :uitid", array(':uitid' => $uitId));

    $this->drupalUser->set('status', 0);
    $this->drupalUser->save();

    user_logout();
  }

}

