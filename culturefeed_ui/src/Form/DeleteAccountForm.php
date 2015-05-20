<?php

namespace Drupal\culturefeed_ui\Form;

use Drupal\Core\Form\ConfirmFormBase;
use Drupal\Core\Form\FormStateInterface;
use Drupal\Core\Url;
use Symfony\Component\DependencyInjection\ContainerInterface;
use CultureFeed;
use CultureFeed_User;

class DeleteAccountForm extends ConfirmFormBase {
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
      $container->get('culturefeed')
    );
  }

  /**
   * Constructs a ProfileForm
   *
   * @param CultureFeed_User $user
   * @param CultureFeed $culturefeedService
   */
  public function __construct(
    CultureFeed_User $user,
    CultureFeed $culturefeedService
  ) {
    $this->user = $user;
    $this->culturefeed = $culturefeedService;
  }

  /**
   * {@inheritdoc}
   */
  public function getQuestion() {
    return t('Are you sure you want to delete your account?');
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
    return t('This action cannot be undone.');
  }

  /**
   * {@inheritdoc}
   */
  public function getConfirmText() {
    return t('Delete');
  }

  /**
   * {@inheritdoc}
   */
  public function getCancelText() {
    return t('Cancel');
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
    try {
      $this->culturefeed->deleteUser($this->user->id);
    } catch (\Exception $e) {
      watchdog_exception('culturefeed_ui', $e);
      drupal_set_message(t('An error occurred while deleting your account.'));
      return;
    }

    // TODO: Figure out what should happen to the Drupal user
    // The old Culturefeed changes the status of the user and does some
    // database deletes but does not really delete it.
    $drupalUser = \Drupal::currentUser();
    user_delete($drupalUser->id());
    user_logout();
  }

}

