<?php

/**
 * @file
 * Contains Drupal\culturefeed_ui\Form\ProfileForm.
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
 * Provides a profile editing form.
 */
class ProfileForm extends FormBase
{
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
     * {@inheritdoc}
     */
    public static function create(ContainerInterface $container) {
        return new static(
            $container->get('culturefeed.current_user'),
            $container->get('culturefeed'),
            $container->get('country_manager')
        );
    }

    /**
     * Constructs a ProfileForm
     *
     * @param CultureFeed_User $user
     * @param CultureFeed $culturefeedService
     * @param CountryManagerInterface $countryManager
     */
    public function __construct(
        CultureFeed_User $user,
        CultureFeed $culturefeedService,
        CountryManagerInterface $countryManager
    ) {
        $this->user = $user;
        $this->culturefeed = $culturefeedService;
        $this->countryManager = $countryManager;
    }

    /**
     * {@inheritdoc}
     */
    public function getFormId()
    {
        return 'profile_form';
    }

    /**
     * {@inheritdoc}
     */
    public function buildForm(array $form, FormStateInterface $form_state)
    {
        $user = $this->user;
        $form = array();

        $form['#theme'] = 'ui_profile_form';

        $form['view-profile'] = array(
            '#id' => 'view-profile',
            '#url' => Url::fromRoute('culturefeed_ui.user_controller_profile'),
            '#title' => t('My profile'),
            '#type' => 'link'
        );

        // 'About me' fieldset
        $form['about-me'] = array(
          '#type' => 'fieldset',
          '#title' => t('About me')
        );
        $form['about-me']['givenName'] = array(
            '#type' => 'textfield',
            '#title' => t('First name'),
            '#default_value' => $user->givenName,
        );
        $form['about-me']['familyName'] = array(
            '#type' => 'textfield',
            '#title' => t('Family name'),
            '#default_value' => $user->familyName,
        );
        $form['about-me']['bio'] = array(
            '#type' => 'textarea',
            '#title' => t('Biography'),
            '#default_value' => $user->bio,
            '#description' => t('Maximum 250 characters'),
        );
        // Picture.
//        $form_state->set('#old_picture', 0);
//        $form['picture'] = array(
//            '#type' => 'managed_file',
//            '#title' => t('Choose picture'),
//            '#description' => t('Allowed extensions: jpg, jpeg, gif or png'),
//            '#process' => array('file_managed_file_process', 'culturefeed_image_file_process'),
//            '#upload_validators' => array(
//                'file_validate_extensions' => array('jpg jpeg png gif'),
//            ),
//            '#upload_location' => 'public://culturefeed',
//        );

// TODO: find an alternative for the helper function "culturefeed_create_temporary_image".
// I think a library like flysystem would be more suitable to manage these local images.
//        // Check if the depiction is not the default one.
//        if (!empty($cf_account->depiction) && !strstr($cf_account->depiction, '/' . CULTUREFEED_UI_DEFAULT_DEPICTION)) {
//            $file = culturefeed_create_temporary_image($cf_account->depiction, file_default_scheme() . '://culturefeed');
//            if ($file) {
//                $form_state->set('#old_picture', $file->fid);
//                $form['picture']['#default_value'] = $file->fid;
//            }
//        }
        $form['about-me']['dob'] = array(
            '#title' => t('Date of birth'),
            '#type' => 'textfield',
            '#default_value' => $user->dob ? date('d/m/Y', $user->dob) : '',
            '#description' => t('Format : dd/mm/yyyy'),
            '#size' => 10,
        );
        $form['about-me']['gender'] = array(
            '#type' => 'radios',
            '#title' => t('Gender'),
            '#options' => array('male' => t('Male'), 'female' => t('Female')),
            '#default_value' => $user->gender,
        );

        // Address fieldset
        $form['address'] = array (
            '#type' => 'fieldset',
            '#title' => t('Address'),
            '#attributes' => array(
                'collapsable' => '',
            )
        );
        $form['address']['street'] = array(
            '#type' => 'textfield',
            '#title' => t('Street and number'),
            '#default_value' => $user->street,
        );
        $form['address']['zip'] = array(
            '#type' => 'textfield',
            '#title' => t('Zipcode'),
            '#default_value' => $user->zip,
        );
        $form['address']['city'] = array(
            '#type' => 'textfield',
            '#title' => t('City'),
            '#default_value' => $user->city,
        );
        $form['address']['country'] = array(
            '#type' => 'select',
            '#options' => $this->countryManager->getList(),
            '#title' => t('Country'),
            '#default_value' => !empty($user->country) ? $user->country : 'BE',
        );

        // 'Privacy settings' fieldset

        $form['privacy-settings'] = array(
            '#type' => 'fieldset',
            '#title' => t('Privacy settings'),
            '#attributes' => array(
                'collapsable' => 'collapsed'
            )
        );
        $form['privacy-settings']['givenNamePrivacy'] = array(
            '#type' => 'checkbox',
            '#title' => t('Hide \'first name\' in public profile'),
            '#default_value' => $user->privacyConfig->givenName == CultureFeed_UserPrivacyConfig::PRIVACY_PRIVATE,
        );
        $form['privacy-settings']['familyNamePrivacy'] = array(
            '#type' => 'checkbox',
            '#title' => t('Hide \'family name\' in public profile'),
            '#default_value' => $user->privacyConfig->familyName == CultureFeed_UserPrivacyConfig::PRIVACY_PRIVATE,
        );
        $form['privacy-settings']['genderPrivacy'] = array(
            '#type' => 'checkbox',
            '#title' => t('Hide \'gender\' in public profile'),
            '#default_value' => $user->privacyConfig->gender == CultureFeed_UserPrivacyConfig::PRIVACY_PRIVATE,
        );
        $form['privacy-settings']['homeAddressPrivacy'] = array(
            '#type' => 'checkbox',
            '#title' => t('Hide \'address\' in public profile'),
            '#default_value' => $user->privacyConfig->homeAddress == CultureFeed_UserPrivacyConfig::PRIVACY_PRIVATE,
        );
        $form['privacy-settings']['dobPrivacy'] = array(
            '#type' => 'checkbox',
            '#title' => t('Hide \'date of birth\' in public profile'),
            '#default_value' => $user->privacyConfig->dob == CultureFeed_UserPrivacyConfig::PRIVACY_PRIVATE,
        );
        $form['privacy-settings']['bioPrivacy'] = array(
            '#type' => 'checkbox',
            '#title' => t('Hide \'biography\' in public profile'),
            '#default_value' => $user->privacyConfig->bio == CultureFeed_UserPrivacyConfig::PRIVACY_PRIVATE,
        );

        // 'Language settings' fieldset
        $form['language-settings'] = array(
            '#type' => 'fieldset',
            '#title' => t('Language settings'),
            '#attributes' => array(
                'collapsable' => 'collapsed'
            )
        );
        $form['language-settings']['preferredLanguageField'] = array(
            '#type' => 'select',
            '#title' => t('Preferred language'),
            '#default_value' => !empty($user->preferredLanguage) ? $user->preferredLanguage : '',
            '#options' => array(
              'nl' => t('Dutch'),
              'fr' => t('French'),
              'en' => t('English'),
              'de' => t('German'),
            ),
        );

        $form['submit'] = array(
            '#type' => 'submit',
            '#value' => t('Save'),
        );

        return $form;
    }

    /**
     * {@inheritdoc}
     */
    public function validateForm(array &$form, FormStateInterface $form_state)
    {
        $culturefeed = $this->culturefeed;
        $values = $form_state->getValues();

        // Custom validations first.
        if (Unicode::strlen($values['bio']) > 250) {
            $form_state->setErrorByName('bio', t('The maximum of 250 characters is exceeded'));
            return;
        }

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

        $fields = array('id', 'givenName', 'familyName', 'gender', 'bio', 'street', 'zip', 'city', 'country', 'dob');

        try {
            $culturefeed->updateUser($user_update, $fields);
        }
        catch (Exception $e) {
            watchdog_exception('culturefeed_ui', $e);
            $form_state->setErrorByName('submit', t('Error occurred while saving your personal data.'));
        }

        // Remove the profile picture if requested.
        if (empty($values['picture']) && $form_state->getValue('#old_picture') > 0) {
            try {
                $culturefeed->removeUserDepiction($this->user->id);
            }
            catch (Exception $e) {
                watchdog_exception('culturefeed_ui', $e);
            }
        }

        // Upload profile picture.
        $picture = $form_state->getValue('picture');
        $oldPicture = $form_state->getValue('#old_picture');

        if ($picture && $oldPicture != $picture) {

            $file = file_load($picture);
            if ($file) {
                try {
                    $file_upload = culturefeed_prepare_curl_upload_from_file($file);
                    $culturefeed->uploadUserDepiction($this->user->id, $file_upload);
                }
                catch (Exception $e) {
                    watchdog_exception('culturefeed_ui', $e);
                    $form_state->setErrorByName('picture', t('Error occurred while saving your picture.'));
                }
            }
        }

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
        }
        catch (Exception $e) {
            $form_state->setErrorByName('submit', t('Error occurred while saving your privacy settings.'));
        }
    }

    /**
     * {@inheritdoc}
     */
    public function submitForm(array &$form, FormStateInterface $form_state)
    {
        drupal_set_message(t('Changes succesfully saved.'));
    }

}
