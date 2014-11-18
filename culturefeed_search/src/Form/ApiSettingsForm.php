<?php

/**
 * @file
 * Contains \Drupal\culturefeed\Form\ApiSettingsForm.
 */

namespace Drupal\culturefeed_search\Form;

use Drupal\Core\Form\ConfigFormBase;
use Drupal\Core\Form\FormStateInterface;

/**
 * Defines a form to configure the culturefeed search api.
 */
class ApiSettingsForm extends ConfigFormBase {

  /**
   * {@inheritdoc}
   */
  public function getFormID() {
    return 'culturefeed_search_api_settings';
  }

  /**
   * {@inheritdoc}
   */
  public function buildForm(array $form, FormStateInterface $form_state) {

    $config = $this->config('culturefeed_search.api');

    $form['api_location'] = array(
      '#type' => 'textfield',
      '#title' => $this->t('API location'),
      '#description' => $this->t('The URL where the CultureFeed API resides. End with a slash. Example: http://acc.uitid.be/uitid/rest/'),
      '#default_value' => $config->get('api_location'),
    );
    $form['application_key'] = array(
      '#type' => 'textfield',
      '#title' => $this->t('Application key'),
      '#description' => $this->t('Your CultureFeed Application key.'),
      '#default_value' => $config->get('application_key'),
      '#size' => 40,
      '#maxlength' => 40,
    );
    $form['shared_secret'] = array(
      '#type' => 'textfield',
      '#title' => $this->t('Shared secret'),
      '#description' => $this->t('Your CultureFeed Shared Secret.'),
      '#default_value' => $config->get('shared_secret'),
      '#size' => 40,
      '#maxlength' => 40,
    );

    return parent::buildForm($form, $form_state);

  }

  /**
   * {@inheritdoc}
   */
  public function submitForm(array &$form, FormStateInterface $form_state) {
    $values = $form_state->getValues();
    $this->config('culturefeed_search.api')
      ->set('api_location', $values['api_location'])
      ->set('application_key', $values['application_key'])
      ->set('shared_secret', $values['shared_secret'])
      ->save();
    parent::submitForm($form, $form_state);
  }

}
