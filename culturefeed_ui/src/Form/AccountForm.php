<?php

/**
 * @file
 * Contains Drupal\culturefeed_ui\Form\AccountForm.
 */

namespace Drupal\culturefeed_ui\Form;

use Drupal\Component\Utility\Unicode;
use Drupal\Core\Form\ConfigFormBase;
use Drupal\Core\Form\FormBase;
use Drupal\Core\Form\FormStateInterface;
use CultureFeed_User;
use Drupal\Core\Locale\CountryManager;
use Drupal\Core\Locale\CountryManagerInterface;
use Drupal\Core\Url;
use Symfony\Component\DependencyInjection\ContainerInterface;
use CultureFeed_UserPrivacyConfig;
use CultureFeed;

/**
 * Provides an account editing form.
 */
class AccountForm extends FormBase
{
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
    public function getFormId()
    {
        return 'account_form';
    }

    /**
     * {@inheritdoc}
     */
    public function buildForm(array $form, FormStateInterface $form_state)
    {
        $cf_account = $this->user;
        $form = array();

        $form['#theme'] = 'ui_account_form';

        $profileUrl = Url::fromRoute('culturefeed_ui.user_controller_profile');
        $form['view-profile'] = array(
            '#prefix' => '<div id="view-profile">',
            '#markup' => \Drupal::l(t('View profile'), $profileUrl),
            '#suffix' => '</div>',
        );

        $form['nick'] = array(
            '#type' => 'textfield',
            '#title' => t('Username'),
            '#disabled' => TRUE,
            '#value' => $cf_account->nick,
        );

        $form['mbox'] = array(
            '#type' => 'email',
            '#title' => t('Email address'),
            '#default_value' => $cf_account->mbox,
            '#required' => TRUE,
        );

        $form['submit'] = array(
            '#type' => 'submit',
            '#value' => t('Save'),
        );

        $returnUrl = Url::fromRoute(
            'culturefeed_ui.account_form',
            [],
            ['absolute' => TRUE]
        );
        $changePasswordUrl = Url::fromUri($this->culturefeed->getUrlChangePassword($cf_account->id, $returnUrl));
        $options = array(
            'attributes' => array('class' => array('culturefeedconnect')),
            'query' => drupal_get_destination()
        );
        $form['change_password'] = array(
            '#prefix' => '<div id="change-password">',
            '#markup' => \Drupal::l(t('Change password'), $changePasswordUrl, $options),
            '#suffix' => '</div>',
        );

        $form['remove_account'] = array(
            '#prefix' => '<div id="remove-account">',
            // Switch out $profileUrl for a remove account link once a
            // route is available for it.
            '#markup' => \Drupal::l(t('Delete account'), $profileUrl),
            '#suffix' => '</div>',
        );

        return $form;
    }

    /**
     * {@inheritdoc}
     */
    public function validateForm(array &$form, FormStateInterface $form_state)
    {
        if (\Drupal::service('email.validator')->isValid($form_state->getValue('mbox'))) {
            $form_state->setErrorByName('mbox', t('Invalid email'));
        }
    }

    /**
     * {@inheritdoc}
     */
    public function submitForm(array &$form, FormStateInterface $form_state)
    {
        $cf_account = new CultureFeed_User();
        $cf_account->id = $this->user->id;
        $cf_account->mbox = $form_state->getValue('mbox');

        try {
            $this->culturefeed->updateUser($cf_account);
            drupal_set_message(t('Changes succesfully saved.'));
        }
        catch (Exception $e) {
            watchdog_exception('culturefeed_ui', $e);
            drupal_set_message(t('Error occurred'), 'error');
        }
    }

}
