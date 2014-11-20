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
    $types = array('' => '', 'DEBUG' => t('Debug'), 'ALERT' => t('Alert'));

    // Sync with UDB2.
    $form['sync_with_udb2'] = array(
      '#type' => 'checkbox',
      '#title' => $this->t('Sync with UDB2'),
      '#default_value' => $config->get('sync_with_udb2'),
    );

    // Command bus logging.
    $form['log.command_bus'] = array(
      '#type' => 'fieldset',
      '#title' => $this->t('Command bus logging settings'),
    );

    // Command bus logging: hipchat.
    $form['log.command_bus']['hipchat'] = array(
      '#type' => 'fieldset',
      '#title' => $this->t('Hipchat'),
    );
    $form['log.command_bus']['hipchat']['hipchat_type'] = array(
      '#type' => 'checkbox',
      '#title' => $this->t('Enable'),
      '#default_value' => $config->get('log.command_bus.hipchat'),
    );
    $form['log.command_bus']['hipchat']['hipchat_room'] = array(
      '#type' => 'textfield',
      '#title' => $this->t('Room'),
      '#default_value' => $config->get('log.command_bus.hipchat_room'),
      '#states' => array(
        'invisible' => array(
          ':input[name="hipchat_type"]' => array('checked' => FALSE),
        ),
        'required' => array(
          ':input[name="hipchat_type"]' => array('checked' => TRUE),
        ),
      ),
    );
    $form['log.command_bus']['hipchat']['hipchat_token'] = array(
      '#type' => 'textfield',
      '#title' => $this->t('Token'),
      '#default_value' => $config->get('log.command_bus.hipchat_token'),
      '#states' => array(
        'invisible' => array(
          ':input[name="hipchat_type"]' => array('checked' => FALSE),
        ),
        'required' => array(
          ':input[name="hipchat_type"]' => array('checked' => TRUE),
        ),
      ),
    );

    // Command bus logging: file.
    $form['log.command_bus']['file'] = array(
      '#type' => 'fieldset',
      '#title' => $this->t('File'),
    );
    $form['log.command_bus']['file']['file_type'] = array(
      '#type' => 'checkbox',
      '#title' => $this->t('Enable'),
      '#default_value' => $config->get('log.command_bus.file'),
    );
    $form['log.command_bus']['file']['file_path'] = array(
      '#type' => 'textfield',
      '#title' => $this->t('Path'),
      '#default_value' => $config->get('log.command_bus.file_path'),
      '#states' => array(
        'invisible' => array(
          ':input[name="file_type"]' => array('checked' => FALSE),
        ),
        'required' => array(
          ':input[name="file_type"]' => array('checked' => TRUE),
        ),
      ),
    );

    // Command bus logging: socketioemitter.
    $form['log.command_bus']['socketioemitter'] = array(
      '#type' => 'fieldset',
      '#title' => $this->t('Socket'),
    );
    $form['log.command_bus']['socketioemitter']['socketioemitter_type'] = array(
      '#type' => 'checkbox',
      '#title' => $this->t('Enable'),
      '#default_value' => $config->get('log.command_bus.socketioemitter'),
      '#disabled' => TRUE,
    );
    $form['log.command_bus']['socketioemitter']['socketioemitter_redis_host'] = array(
      '#type' => 'textfield',
      '#title' => $this->t('Redis host'),
      '#default_value' => $config->get('log.command_bus.socketioemitter_redis_host'),
      '#states' => array(
        'invisible' => array(
          ':input[name="socketioemitter_type"]' => array('checked' => FALSE),
        ),
        'required' => array(
          ':input[name="socketioemitter_type"]' => array('checked' => TRUE),
        ),
      ),
    );
    $form['log.command_bus']['socketioemitter']['socketioemitter_redis_port'] = array(
      '#type' => 'textfield',
      '#title' => $this->t('Redis port'),
      '#default_value' => $config->get('log.command_bus.socketioemitter_redis_port'),
      '#states' => array(
        'invisible' => array(
          ':input[name="socketioemitter_type"]' => array('checked' => FALSE),
        ),
        'required' => array(
          ':input[name="socketioemitter_type"]' => array('checked' => TRUE),
        ),
      ),
    );

    $form['log.command_bus']['level'] = array(
      '#type' => 'select',
      '#title' => $this->t('Level'),
      '#options' => $types,
      '#required' => TRUE,
      '#default_value' => $config->get('log.command_bus.level'),
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
      ->set('log.command_bus.hipchat', $values['hipchat_type'])
      ->set('log.command_bus.hipchat_room', $values['hipchat_room'])
      ->set('log.command_bus.hipchat_token', $values['hipchat_token'])
      ->set('log.command_bus.file', $values['file_type'])
      ->set('log.command_bus.file_path', $values['file_path'])
      ->set('log.command_bus.socketioemitter', $values['socketioemitter_type'])
      ->set('log.command_bus.socketioemitter_redis_host', $values['socketioemitter_redis_host'])
      ->set('log.command_bus.socketioemitter_redis_port', $values['socketioemitter_redis_port'])
      ->set('log.command_bus.level', $values['level'])
      ->save();
    parent::submitForm($form, $form_state);
  }

}
