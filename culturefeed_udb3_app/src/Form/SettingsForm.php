<?php

/**
 * @file
 * Contains \Drupal\culturefeed_udb3\Form\SettingsForm.
 */

namespace Drupal\culturefeed_udb3_app\Form;

use Drupal\Core\Form\ConfigFormBase;
use Drupal\Core\Form\FormStateInterface;

/**
 * Defines a form to configure culturefeed udb3 app.
 */
class SettingsForm extends ConfigFormBase {

  /**
   * {@inheritdoc}
   */
  public function getFormId() {
    return 'culturefeed_udb3_app_settings';
  }

  /**
   * {@inheritdoc}
   */
  public function buildForm(array $form, FormStateInterface $form_state) {

    $config = $this->config('culturefeed_udb3_app.settings');

    $form['websocket_server'] = array(
      '#type' => 'textfield',
      '#title' => $this->t('WebSocket server'),
      '#default_value' => $config->get('websocket_server'),
    );

    return parent::buildForm($form, $form_state);

  }

  /**
   * {@inheritdoc}
   */
  public function submitForm(array &$form, FormStateInterface $form_state) {

    $values = $form_state->getValues();
    $this->config('culturefeed_udb3_app.settings')
      ->set('websocket_server', $values['websocket_server'])
      ->save();
    parent::submitForm($form, $form_state);
  }

}
