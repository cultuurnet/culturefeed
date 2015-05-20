<?php

/**
 * @file
 * Contains Drupal\culturefeed_ui\Form\AccountForm.
 */

namespace Drupal\culturefeed_ui\Form;

use Drupal\Core\Config\ConfigFactory;
use Drupal\Core\Form\FormBase;
use Drupal\Core\Form\FormStateInterface;
use CultureFeed_User;
use Drupal\Core\Url;
use Psr\Log\LoggerAwareInterface;
use Psr\Log\LoggerAwareTrait;
use Psr\Log\LoggerInterface;
use Symfony\Component\DependencyInjection\ContainerInterface;
use CultureFeed;

/**
 * Provides an account editing form.
 */
class AccountForm extends FormBase implements LoggerAwareInterface {

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
   * A list of valid culturefeed connected account types.
   * @var string[]
   */
  protected $connectedAccountTypes;

  /**
   * @var \Drupal\Core\Config\Config|\Drupal\Core\Config\ImmutableConfig
   */
  protected $siteConfig;

  /**
   * {@inheritdoc}
   */
  public static function create(ContainerInterface $container) {
    return new static(
      $container->get('culturefeed.current_user'),
      $container->get('culturefeed'),
      $container->get('config.factory'),
      $container->get('logger.channel.culturefeed')
    );
  }

  /**
   * Constructs a ProfileForm
   *
   * @param CultureFeed_User $user
   * @param CultureFeed $culturefeedService
   * @param ConfigFactory $config
   * @param LoggerInterface $logger
   */
  public function __construct(
    CultureFeed_User $user,
    CultureFeed $culturefeedService,
    ConfigFactory $config,
    LoggerInterface $logger
  ) {
    $this->user = $user;
    $this->culturefeed = $culturefeedService;
    $this->connectedAccountTypes = ['twitter', 'facebook', 'google'];
    $this->siteConfig = $config->get('core.site_information');
    $this->setLogger($logger);
  }

  /**
   * {@inheritdoc}
   */
  public function getFormId() {
    return 'account_form';
  }

  /**
   * {@inheritdoc}
   */
  public function buildForm(array $form, FormStateInterface $form_state) {
    $cf_account = $this->user;
    $form = array();

    $form['#theme'] = 'ui_account_form';

    // Account fieldset
    $form['account'] = array(
      '#type' => 'fieldset',
      '#title' => t('My UiTiD'),
    );
    $form['account']['view-profile-link'] = array(
      '#id' => 'view-profile-link',
      '#url' => Url::fromRoute('culturefeed_ui.user_controller_profile'),
      '#title' => t('My profile'),
      '#type' => 'link'
    );
    $form['account']['delete-account-link'] = array(
      '#id' => 'delete-account-link',
      '#url' => Url::fromRoute('culturefeed_ui.delete_account_form'),
      '#title' => t('Delete account'),
      '#type' => 'link'
    );
    $form['account']['nick'] = array(
      '#type' => 'textfield',
      '#title' => t('Username'),
      '#disabled' => TRUE,
      '#value' => $cf_account->nick,
    );
    $form['account']['mbox'] = array(
      '#type' => 'email',
      '#title' => t('Email address'),
      '#default_value' => $cf_account->mbox,
      '#required' => TRUE,
    );
    $form['account']['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Change Email address'),
    );
    // change password link
    $returnUrl = Url::fromRoute(
      'culturefeed_ui.account_form',
      [],
      ['absolute' => TRUE]
    );
    $options = array(
      'attributes' => array('class' => array('culturefeedconnect')),
      'query' => drupal_get_destination()
    );
    $form['account']['change-password-link'] = array(
      '#id' => 'change-password-link',
      '#title' => t('Change password'),
      '#type' => 'link',
      '#url' => Url::fromUri(
        $this->culturefeed->getUrlChangePassword($cf_account->id, $returnUrl),
        $options
      ),
    );

    // 'Connected accounts' fieldset
    $form['connected-accounts'] = array(
      '#type' => 'fieldset',
      '#title' => t('Connected accounts'),
      '#attributes' => array(
        'collapsable' => 'collapsed'
      )
    );
    $form['connected-accounts']['accounts'] = $this->getConnectedAccountFields($this->connectedAccountTypes);

    return $form;
  }

  /**
   * {@inheritdoc}
   */
  public function validateForm(array &$form, FormStateInterface $form_state) {
    if (\Drupal::service('email.validator')
      ->isValid($form_state->getValue('mbox'))
    ) {
      $form_state->setErrorByName('mbox', t('Invalid email'));
    }
  }

  /**
   * {@inheritdoc}
   */
  public function submitForm(array &$form, FormStateInterface $form_state) {
    $cf_account = new CultureFeed_User();
    $cf_account->id = $this->user->id;
    $cf_account->mbox = $form_state->getValue('mbox');

    try {
      $this->culturefeed->updateUser($cf_account);
      drupal_set_message(t('Changes successfully saved.'));
    } catch (\Exception $e) {
      if ($this->logger) {
        $this->logger->error(
          'Error occurred while updating user account',
          array('exception' => $e)
        );
      }
      drupal_set_message(t('Error occurred'), 'error');
    }
  }

  public function getConnectedAccountFields($accountTypes) {

    $items = array();
    /** @var \CultureFeed_OnlineAccount[] $userAccounts */
    $userAccounts = $this->user->holdsAccount;
    $connectedAccounts = array();
    foreach ($userAccounts as $userAccount) {
      $connectedAccounts[$userAccount->accountType] = $userAccount;
    }

    foreach ($accountTypes as $accountType) {
      $renderedAccount = $this->renderConnectedAccount(
        $accountType,
        $connectedAccounts[$accountType]
      );

      $items[] = array(
        '#markup' => $renderedAccount,
        '#id' => 'onlineaccount-' . $accountType,
      );
    }

    return array(
      '#theme' => 'item_list',
      '#items' => $items,
    );
  }

  /**
   * @param $connectedAccountType
   * @param \CultureFeed_OnlineAccount|null $connectedAccount
   * @return mixed
   */
  private function renderConnectedAccount(
    $connectedAccountType,
    $connectedAccount = NULL
  ) {
    $disconnectLink = '';

    if ($connectedAccount) {
      $disconnectUrl = new Url('culturefeed_ui.connected_accounts.disconnect',
        array(
          'account_type' => $connectedAccountType,
          'account_name' => $connectedAccount->accountName
        ));
      $disconnectLink = array(
        '#type' => 'link',
        '#title' => t('Disconnect'),
        '#url' => $disconnectUrl,
        '#attributes' => array('class' => 'disconnect-link'),
        '#options' => array('query' => drupal_get_destination()),
      );

      $disconnectLink = \Drupal::service('renderer')->render($disconnectLink);
    }

    $privacyStatement = t('I accept that my UiTiD actions on ')
      . ($this->siteConfig->get('site_name') ?: 'Drupal')
      . t(' will be published automatically on ')
      . $connectedAccountType . '.';
    // user agreement was linked to this specific node: l(t('User agreement'), 'node/2512')

    $publishLink = '';
    $makePrivateUrl = new Url('culturefeed_ui.connected_accounts.make_private',
      array(
        'account_type' => $connectedAccountType,
        'account_name' => $connectedAccount->accountName
      ));
    $makePublicUrl = new Url('culturefeed_ui.connected_accounts.make_public',
      array(
        'account_type' => $connectedAccountType,
        'account_name' => $connectedAccount->accountName
      ));
    if ($connectedAccount) {
      $publishLink = array(
        '#type' => 'link',
        '#title' => $connectedAccount->publishActivities ? t('Public') : t('Private'),
        '#url' => ($connectedAccount->publishActivities ? $makePrivateUrl : $makePublicUrl),
        '#attributes' => array(
          'id' => 'onlineaccount-privacy-' . $connectedAccount->accountName,
          'class' => 'privacy-link ' . ($connectedAccount->publishActivities ? 'status-publiek' : 'status-prive'),
          'title' => ($connectedAccount->publishActivities ? t('Switch off') : t('Switch on'))
        ),
        '#options' => array('query' => drupal_get_destination()),
        '#ajax' => array(),
      );

      $publishLink = \Drupal::service('renderer')->render($publishLink);
    }

    $redirectUrl = new Url('culturefeed_ui.account_form', [],
      array('absolute' => TRUE));
    $connectLinkUri = $this->culturefeed->getUrlAddSocialNetwork(
      $connectedAccountType,
      $redirectUrl->toString(),
      array('attributes' => array('class' => 'culturefeedconnect'))
    );
    $connectLink = array(
      '#type' => 'link',
      '#url' => Url::fromUri($connectLinkUri),
      '#title' => t('Add account'),
    );

    $connectedAccountVariables = array(
      '#accountType' => $connectedAccountType,
      '#account' => $connectedAccount,
      '#publishLink' => $publishLink,
      '#disconnectLink' => $disconnectLink,
      '#connectLink' => $connectLink,
      '#privacyStatement' => $privacyStatement,
      '#theme' => 'culturefeed_ui_connected_account',
    );

    return \Drupal::service('renderer')->render($connectedAccountVariables);
  }

}
