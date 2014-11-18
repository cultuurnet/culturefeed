<?php

/**
 * @file
 * Contains \Drupal\culturefeed_udb3\Form\SettingsForm.
 */

namespace Drupal\culturefeed_udb3\Form;

use Drupal\Core\Form\ConfigFormBase;
use Drupal\Core\Form\FormStateInterface;

/**
 * Defines a form to configure culturefeed udb3.
 */
class SettingsForm extends ConfigFormBase {

  /**
   * {@inheritdoc}
   */
  public function getFormID() {
    return 'culturefeed_udb3_settings';
  }

  /**
   * {@inheritdoc}
   */
  public function buildForm(array $form, FormStateInterface $form_state) {

    $config = $this->config('culturefeed_udb3.settings');

    $form['sync_with_udb2'] = array(
      '#type' => 'checkbox',
      '#title' => $this->t('Sync with UDB2'),
      '#default_value' => $config->get('sync_with_udb2'),
    );

    return parent::buildForm($form, $form_state);

  }

  /**
   * {@inheritdoc}
   */
  public function submitForm(array &$form, FormStateInterface $form_state) {

    $values = $form_state->getValues();
    $this->config('culturefeed_udb3.settings')
      ->set('sync_with_udb2', $values['sync_with_udb2'])
      ->save();
    parent::submitForm($form, $form_state);
  }

}
