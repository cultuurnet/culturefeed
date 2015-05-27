<?php

/**
 * @file
 * Contains Drupal\culturefeed_ui\Form\ProfileForm.
 */

namespace Drupal\culturefeed_ui\Form;

use Drupal\Core\Form\FormBase;
use Drupal\Core\Form\FormStateInterface;
use Drupal\file\Entity\File;
use CultureFeed_User;
use Drupal\Core\Locale\CountryManagerInterface;
use Drupal\Core\Url;
use Drupal\file\FileInterface;
use Psr\Log\LoggerAwareInterface;
use Psr\Log\LoggerAwareTrait;
use Psr\Log\LoggerInterface;
use Symfony\Component\DependencyInjection\ContainerInterface;
use CultureFeed_UserPrivacyConfig;
use CultureFeed;

/**
 * Provides a profile editing form.
 */
class ProfileForm extends FormBase implements LoggerAwareInterface {

  use LoggerAwareTrait;

  /**
   * The culturefeed user service.
   *
   * @var CultureFeed_User;
   */
  protected $user;

  /**
   * @var CountryManagerInterface
   */
  protected $countryManager;

  /**
   * The culturefeed service
   *
   * @var CultureFeed
   */
  protected $culturefeed;

  /**
   * @var FileInterface
   */
  protected $picture;

  /**
   * {@inheritdoc}
   */
  public static function create(ContainerInterface $container) {
    return new static(
      $container->get('culturefeed.current_user'),
      $container->get('culturefeed'),
      $container->get('country_manager'),
      $container->get('logger.channel.culturefeed')
    );
  }

  /**
   * Constructs a ProfileForm
   *
   * @param CultureFeed_User $user
   * @param CultureFeed $culturefeedService
   * @param CountryManagerInterface $countryManager
   * @param LoggerInterface $logger
   */
  public function __construct(
    CultureFeed_User $user,
    CultureFeed $culturefeedService,
    CountryManagerInterface $countryManager,
    LoggerInterface $logger
  ) {
    $this->user = $user;
    $this->culturefeed = $culturefeedService;
    $this->countryManager = $countryManager;
    $this->setLogger($logger);
  }

  /**
   * {@inheritdoc}
   */
  public function getFormId() {
    return 'profile_form';
  }

  /**
   * {@inheritdoc}
   */
  public function buildForm(array $form, FormStateInterface $form_state) {
    $user = $this->user;
    $form = array();

    $form['#theme'] = 'ui_profile_form';

    $form['view-profile-link'] = array(
      '#id' => 'view-profile-link',
      '#url' => Url::fromRoute('culturefeed_ui.user_controller_profile'),
      '#title' => $this->t('My profile'),
      '#type' => 'link'
    );

    // 'About me' fieldset
    $form['about-me'] = array(
      '#type' => 'fieldset',
      '#title' => $this->t('About me')
    );
    $form['about-me']['givenName'] = array(
      '#type' => 'textfield',
      '#title' => $this->t('First name'),
      '#default_value' => $user->givenName,
    );
    $form['about-me']['familyName'] = array(
      '#type' => 'textfield',
      '#title' => $this->t('Family name'),
      '#default_value' => $user->familyName,
    );
    $form['about-me']['bio'] = array(
      '#type' => 'textarea',
      '#title' => $this->t('Biography'),
      '#default_value' => $user->bio,
      '#description' => $this->t('Maximum 250 characters'),
      '#maxlength' => 250,
    );

    $file_validators = array(
      'file_validate_extensions' => array('jpg jpeg gif png'),
      'file_validate_size' => array(file_upload_max_size()),
    );

    $depictionDestination = 'public://culturefeed/user_depictions/' . $user->id . '.jpg';
    /** @var File $currentDepiction */
    $currentDepiction = system_retrieve_file(
      'http:' . $user->depiction,
      $depictionDestination,
      true,
      FILE_EXISTS_REPLACE
    );
    $form['about-me']['picture'] = array(
      '#type' => 'managed_image',
      '#title' => $this->t('Picture'),
      '#description' => array(
        '#theme' => 'file_upload_help',
        '#description' => $this->t('An image file.'),
        '#upload_validators' => $file_validators,
      ),
      '#size' => 50,
      '#upload_validators' => $file_validators,
      '#upload_location' => $depictionDestination,
      '#multiple' => FALSE,
      '#default_value' => [$currentDepiction->id()],
    );

    $form['about-me']['dob'] = array(
      '#title' => $this->t('Date of birth'),
      '#type' => 'textfield',
      '#default_value' => $user->dob ? date('d/m/Y', $user->dob) : '',
      '#description' => $this->t('Format : dd/mm/yyyy'),
      '#size' => 10,
    );
    $form['about-me']['gender'] = array(
      '#type' => 'radios',
      '#title' => $this->t('Gender'),
      '#options' => array('male' => $this->t('Male'), 'female' => $this->t('Female')),
      '#default_value' => $user->gender,
    );

    // Address fieldset
    $form['address'] = array(
      '#type' => 'fieldset',
      '#title' => $this->t('Address'),
      '#attributes' => array(
        'collapsable' => '',
      )
    );
    $form['address']['street'] = array(
      '#type' => 'textfield',
      '#title' => $this->t('Street and number'),
      '#default_value' => $user->street,
    );
    $form['address']['zip'] = array(
      '#type' => 'textfield',
      '#title' => $this->t('Zipcode'),
      '#default_value' => $user->zip,
    );
    $form['address']['city'] = array(
      '#type' => 'textfield',
      '#title' => $this->t('City'),
      '#default_value' => $user->city,
    );
    $form['address']['country'] = array(
      '#type' => 'select',
      '#options' => $this->countryManager->getList(),
      '#title' => $this->t('Country'),
      '#default_value' => !empty($user->country) ? $user->country : 'BE',
    );

    // 'Privacy settings' fieldset

    $form['privacy-settings'] = array(
      '#type' => 'fieldset',
      '#title' => $this->t('Privacy settings'),
      '#attributes' => array(
        'collapsable' => 'collapsed'
      )
    );
    $form['privacy-settings']['givenNamePrivacy'] = array(
      '#type' => 'checkbox',
      '#title' => $this->t('Hide \'first name\' in public profile'),
      '#default_value' => $user->privacyConfig->givenName == CultureFeed_UserPrivacyConfig::PRIVACY_PRIVATE,
    );
    $form['privacy-settings']['familyNamePrivacy'] = array(
      '#type' => 'checkbox',
      '#title' => $this->t('Hide \'family name\' in public profile'),
      '#default_value' => $user->privacyConfig->familyName == CultureFeed_UserPrivacyConfig::PRIVACY_PRIVATE,
    );
    $form['privacy-settings']['genderPrivacy'] = array(
      '#type' => 'checkbox',
      '#title' => $this->t('Hide \'gender\' in public profile'),
      '#default_value' => $user->privacyConfig->gender == CultureFeed_UserPrivacyConfig::PRIVACY_PRIVATE,
    );
    $form['privacy-settings']['homeAddressPrivacy'] = array(
      '#type' => 'checkbox',
      '#title' => $this->t('Hide \'address\' in public profile'),
      '#default_value' => $user->privacyConfig->homeAddress == CultureFeed_UserPrivacyConfig::PRIVACY_PRIVATE,
    );
    $form['privacy-settings']['dobPrivacy'] = array(
      '#type' => 'checkbox',
      '#title' => $this->t('Hide \'date of birth\' in public profile'),
      '#default_value' => $user->privacyConfig->dob == CultureFeed_UserPrivacyConfig::PRIVACY_PRIVATE,
    );
    $form['privacy-settings']['bioPrivacy'] = array(
      '#type' => 'checkbox',
      '#title' => $this->t('Hide \'biography\' in public profile'),
      '#default_value' => $user->privacyConfig->bio == CultureFeed_UserPrivacyConfig::PRIVACY_PRIVATE,
    );

    // 'Language settings' fieldset
    $form['language-settings'] = array(
      '#type' => 'fieldset',
      '#title' => $this->t('Language settings'),
      '#attributes' => array(
        'collapsable' => 'collapsed'
      )
    );
    $form['language-settings']['preferredLanguage'] = array(
      '#type' => 'select',
      '#title' => $this->t('Preferred language'),
      '#default_value' => !empty($user->preferredLanguage) ? $user->preferredLanguage : '',
      '#options' => array(
        'nl' => $this->t('Dutch'),
        'fr' => $this->t('French'),
        'en' => $this->t('English'),
        'de' => $this->t('German'),
      ),
    );

    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => $this->t('Save'),
    );

    return $form;
  }

  /**
   * {@inheritdoc}
   */
  public function validateForm(array &$form, FormStateInterface $form_state) {
    $culturefeed = $this->culturefeed;
    $values = $form_state->getValues();

    // Update profile information.
    $user_update = new CultureFeed_User();

    $user_update->id = $this->user->id;
    $user_update->givenName = $values['givenName'];
    $user_update->familyName = $values['familyName'];
    $user_update->gender = $values['gender'];
    $user_update->bio = $values['bio'];
    $user_update->street = $values['street'];
    $user_update->zip = $values['zip'];
    $user_update->city = $values['city'];
    $user_update->country = $values['country'];
    $user_update->preferredLanguage = $values['preferredLanguage'];

    if (empty($values['dob'])) {
      $user_update->dob = '';
    }
    else {
      $dob_parts = explode('/', $values['dob']);

      if (count($dob_parts) == 3) {
        list($day, $month, $year) = $dob_parts;

        if (is_numeric($day) && is_numeric($month) && is_numeric($year)) {
          if ($date = mktime(0, 0, 0, (int) $month, (int) $day, (int) $year)) {
            $user_update->dob = $date;
          }
        }
      }
    }

    $fields = array(
      'id',
      'givenName',
      'familyName',
      'gender',
      'bio',
      'street',
      'zip',
      'city',
      'country',
      'dob'
    );

    if (!$form_state->hasAnyErrors()) {
      try {
        $culturefeed->updateUser($user_update, $fields);
      } catch (\Exception $e) {
        if ($this->logger) {
          $this->logger->error(
            'Error occurred while updating user',
            array('exception' => $e)
          );
        }
        $form_state->setErrorByName(
          'submit',
          $this->t('Error occurred while saving your personal data.')
        );
      }
    }

    // Remove or update the profile picture
    if (empty($values['picture'])) {
      try {
        $culturefeed->removeUserDepiction($this->user->id);
      } catch (\Exception $e) {
        if ($this->logger) {
          $this->logger->error(
            'Error occurred while removing the user depiction',
            array('exception' => $e)
          );
        }
      }
    } else {
      $depictionFid = $values['picture'][0];
      if ($depictionFid) {

        /** @var FileInterface $file **/
        $file = File::load($depictionFid);
        if ($file) {
          try {
            $depictionContent = file_get_contents($file->getFileUri());
            $culturefeed->uploadUserDepiction($this->user->id, $depictionContent);
          } catch (\Exception $e) {
            if ($this->logger) {
              $this->logger->error(
                'Error occurred while saving the user depiction',
                array('exception' => $e)
              );
            }
            $form_state->setErrorByName('picture',
              $this->t('Error occurred while saving your picture.'));
          }
        }
      }
    }

    if (!$form_state->hasAnyErrors()) {
      // Update field privacy status.
      $privacy_config = new CultureFeed_UserPrivacyConfig();

      $privacy_config->givenName = $values['givenNamePrivacy'] ? CultureFeed_UserPrivacyConfig::PRIVACY_PRIVATE : CultureFeed_UserPrivacyConfig::PRIVACY_PUBLIC;
      $privacy_config->familyName = $values['familyNamePrivacy'] ? CultureFeed_UserPrivacyConfig::PRIVACY_PRIVATE : CultureFeed_UserPrivacyConfig::PRIVACY_PUBLIC;
      $privacy_config->gender = $values['genderPrivacy'] ? CultureFeed_UserPrivacyConfig::PRIVACY_PRIVATE : CultureFeed_UserPrivacyConfig::PRIVACY_PUBLIC;
      $privacy_config->homeAddress = $values['homeAddressPrivacy'] ? CultureFeed_UserPrivacyConfig::PRIVACY_PRIVATE : CultureFeed_UserPrivacyConfig::PRIVACY_PUBLIC;
      $privacy_config->dob = $values['dobPrivacy'] ? CultureFeed_UserPrivacyConfig::PRIVACY_PRIVATE : CultureFeed_UserPrivacyConfig::PRIVACY_PUBLIC;
      $privacy_config->bio = $values['bioPrivacy'] ? CultureFeed_UserPrivacyConfig::PRIVACY_PRIVATE : CultureFeed_UserPrivacyConfig::PRIVACY_PUBLIC;

      try {
        $culturefeed->updateUserPrivacy($this->user->id, $privacy_config);
      } catch (\Exception $e) {
        $form_state->setErrorByName(
          'submit',
          $this->t('Error occurred while saving your privacy settings.')
        );
      }
    }
  }

  /**
   * {@inheritdoc}
   */
  public function submitForm(array &$form, FormStateInterface $form_state) {
    drupal_set_message($this->t('Changes succesfully saved.'));
  }

}
