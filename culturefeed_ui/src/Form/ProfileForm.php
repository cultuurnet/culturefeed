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
        $cf_account = $this->user;
        $form = array();

        $form['#theme'] = 'ui_profile_form';

        $profileUrl = Url::fromRoute('culturefeed_ui.user_controller_profile');
        $form['view-profile'] = array(
            '#prefix' => '<div id="view-profile">',
            '#markup' => \Drupal::l(t('View profile'), $profileUrl),
            '#suffix' => '</div>',
        );

        // Firstname.
        $form['givenName'] = array(
            '#type' => 'textfield',
            '#title' => t('First name'),
            '#default_value' => $cf_account->givenName,
        );
        $form['givenNamePrivacy'] = array(
            '#type' => 'checkbox',
            '#title' => t('Hide \'first name\' in public profile'),
            '#default_value' => $cf_account->privacyConfig->givenName == CultureFeed_UserPrivacyConfig::PRIVACY_PRIVATE,
        );

        // Name.
        $form['familyName'] = array(
            '#type' => 'textfield',
            '#title' => t('Family name'),
            '#default_value' => $cf_account->familyName,
        );
        $form['familyNamePrivacy'] = array(
            '#type' => 'checkbox',
            '#title' => t('Hide \'family name\' in public profile'),
            '#default_value' => $cf_account->privacyConfig->familyName == CultureFeed_UserPrivacyConfig::PRIVACY_PRIVATE,
        );

        // Picture.
        $form_state->set('#old_picture', 0);
        $form['picture'] = array(
            '#type' => 'managed_file',
            '#title' => t('Choose picture'),
            '#description' => t('Allowed extensions: jpg, jpeg, gif or png'),
            '#process' => array('file_managed_file_process', 'culturefeed_image_file_process'),
            '#upload_validators' => array(
                'file_validate_extensions' => array('jpg jpeg png gif'),
            ),
            '#upload_location' => 'public://culturefeed',
        );

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

        // Gender.
        $form['gender'] = array(
            '#type' => 'radios',
            '#title' => t('Gender'),
            '#options' => array('male' => t('Male'), 'female' => t('Female')),
            '#default_value' => $cf_account->gender,
        );
        $form['genderPrivacy'] = array(
            '#type' => 'checkbox',
            '#title' => t('Hide \'gender\' in public profile'),
            '#default_value' => $cf_account->privacyConfig->gender == CultureFeed_UserPrivacyConfig::PRIVACY_PRIVATE,
        );

        // Address
        $form['street'] = array(
            '#type' => 'textfield',
            '#title' => t('Street and number'),
            '#default_value' => $cf_account->street,
        );
        $form['zip'] = array(
            '#type' => 'textfield',
            '#title' => t('Zipcode'),
            '#default_value' => $cf_account->zip,
        );
        $form['city'] = array(
            '#type' => 'textfield',
            '#title' => t('City'),
            '#default_value' => $cf_account->city,
        );
        $form['country'] = array(
            '#type' => 'select',
            '#options' => $this->countryManager->getList(),
            '#title' => t('Country'),
            '#default_value' => !empty($cf_account->country) ? $cf_account->country : 'BE',
        );
        $form['homeAddressPrivacy'] = array(
            '#type' => 'checkbox',
            '#title' => t('Hide \'address\' in public profile'),
            '#default_value' => $cf_account->privacyConfig->homeAddress == CultureFeed_UserPrivacyConfig::PRIVACY_PRIVATE,
        );

        // Date of birth.
        $form['dob'] = array(
            '#title' => t('Date of birth'),
            '#type' => 'textfield',
            '#default_value' => $cf_account->dob ? date('d/m/Y', $cf_account->dob) : '',
            '#description' => t('Format : dd/mm/yyyy'),
            '#size' => 10,
        );
        $form['dobPrivacy'] = array(
            '#type' => 'checkbox',
            '#title' => t('Hide \'date of birth\' in public profile'),
            '#default_value' => $cf_account->privacyConfig->dob == CultureFeed_UserPrivacyConfig::PRIVACY_PRIVATE,
        );

        // Bio
        $form['bio'] = array(
            '#type' => 'textarea',
            '#title' => t('Biography'),
            '#default_value' => $cf_account->bio,
            '#description' => t('Maximum 250 characters'),
        );
        $form['bioPrivacy'] = array(
            '#type' => 'checkbox',
            '#title' => t('Hide \'biography\' in public profile'),
            '#default_value' => $cf_account->privacyConfig->bio == CultureFeed_UserPrivacyConfig::PRIVACY_PRIVATE,
        );

        // Default language.
        $form['preferredLanguage'] = array(
            '#type' => 'select',
            '#title' => t('Preferred language'),
            '#default_value' => !empty($cf_account->preferredLanguage) ? $cf_account->preferredLanguage : '',
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
