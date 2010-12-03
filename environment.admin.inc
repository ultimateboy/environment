<?php
// $Id$

/**
 * @file
 * Settings for Environment module
 */

function environment_admin_settings() {
  $form = array();

  $env_override = variable_get('environment_override', NULL);
  $env_current = environment_current();
  if ($env_override) {
    $form['environment'] = array(
      '#type'  => 'item',
      '#title' => t('Current Environment'),
      '#value' => $env_current,
    );
    $form['environment_override'] = array(
      '#type'  => 'item',
      '#title' => t('Environment Override'),
      '#value' => t('Active'),
    );
  }
  else {
    $form['environment'] = array(
      '#type'          => 'select',
      '#title'         => t('Environment'),
      '#options'       => _environment_options(),
      '#default_value' => $env_current,
    );
  }
  $form['environment_require_override'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Require environment override'),
    '#description'   => t('Used to require that an environment is set in the settings.php file.'),
    '#default_value' => variable_get('environment_require_override', FALSE),
  );
  $form['#submit'][] = 'environment_switch_environment_submit';
  return system_settings_form($form);
}

/**
 * Submit callback to switch environment if changed.
 */
function environment_switch_environment_submit($form, &$form_state) {
  if (!$form_state['values']['environment_require_override']
    && $form['environment']['#default_value'] != $form_state['values']['environment']) {

    $result = environment_switch($values['environment']);
    foreach ($result['messages'] as $message) {
      drupal_set_message(t(check_plain($message)));
    }
  }
}
