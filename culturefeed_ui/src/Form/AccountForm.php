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

        $form['view-profile-link'] = array(
            '#id' => 'view-profile-link',
            '#url' => Url::fromRoute('culturefeed_ui.user_controller_profile'),
            '#title' => t('My profile'),
            '#type' => 'link'
        );

        $form['delete-account-link'] = array(
            '#id' => 'delete-account-link',
            '#url' => Url::fromRoute('culturefeed_ui.delete_account_form'),
            '#title' => t('Delete account'),
            '#type' => 'link'
        );

        // Account fieldset
        $form['account'] = array (
            '#type' => 'fieldset',
            '#title' => t('My UiTiD'),
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

        // 'Connected accounts' fieldset
        $form['connected-accounts'] = array(
            '#type' => 'fieldset',
            '#title' => t('Connected accounts'),
            '#attributes' => array(
                'collapsable' => 'collapsed'
            )
        );

        $form['submit'] = array(
            '#type' => 'submit',
            '#value' => t('Save'),
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
        $form['change-password-link'] = array(
            '#id' => 'change-password-link',
            '#title' => t('Change password'),
            '#type' => 'link',
            '#url' => Url::fromUri(
                $this->culturefeed->getUrlChangePassword($cf_account->id, $returnUrl),
                $options
            ),
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
