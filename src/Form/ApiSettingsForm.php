<?php

/**
 * @file
 * Contains \Drupal\culturefeed\Form\ApiSettings.
 */

namespace Drupal\culturefeed\Form;

use Drupal\Core\Form\ConfigFormBase;
use Drupal\Core\Form\FormStateInterface;

/**
 * Defines a form to configure maintenance settings for this site.
 */
class ApiSettingsForm extends ConfigFormBase {

  /**
   * {@inheritdoc}
   */
  public function getFormID() {
    return 'culturefeed_api_settings';
  }

  /**
   * {@inheritdoc}
   */
  public function buildForm(array $form, FormStateInterface $form_state) {

    $config = $this->config('culturefeed.api');

    $form['api_location'] = array(
      '#type' => 'textfield',
      '#title' => t('API location'),
      '#description' => t('The URL where the CultureFeed API resides. End with a slash. Example: http://acc.uitid.be/uitid/rest/'),
      '#default_value' => $config->get('api_location'),
    );
    $form['application_key'] = array(
      '#type' => 'textfield',
      '#title' => t('Application key'),
      '#description' => t('Your CultureFeed Application key.'),
      '#default_value' => $config->get('application_key'),
      '#size' => 40,
      '#maxlength' => 40,
    );
    $form['shared_secret'] = array(
      '#type' => 'textfield',
      '#title' => t('Shared secret'),
      '#description' => t('Your CultureFeed Shared Secret.'),
      '#default_value' => $config->get('shared_secret'),
      '#size' => 40,
      '#maxlength' => 40,
    );
    $form['entry_api_path'] = array(
      '#type' => 'textfield',
      '#title' => t('Entry API path'),
      '#description' => t('The path where the CultureFeed Entry API resides. End with a slash. Example: entry/test.rest.uitdatabank.be/api/v2/'),
      '#default_value' => $config->get('entry_api_path'),
    );
    $form['http_client_timeout'] = array(
      '#type' => 'textfield',
      '#title' => t('HTTP client timeout'),
      '#description' => t('Timeout on requests by the HTTP client.'),
      '#default_value' => $config->get('http_client_timeout'),
      '#field_suffix' => t('seconds'),
    );

    return parent::buildForm($form, $form_state);

  }

  /**
   * {@inheritdoc}
   */
  public function submitForm(array &$form, FormStateInterface $form_state) {
    $values = $form_state->getValues();
    $this->config('culturefeed.api')
      ->set('api_location', $values['api_location'])
      ->set('application_key', $values['application_key'])
      ->set('shared_secret', $values['shared_secret'])
      ->set('entry_api_path', $values['entry_api_path'])
      ->set('http_client_timeout', $values['http_client_timeout'])
      ->save();
    parent::submitForm($form, $form_state);
  }

}
