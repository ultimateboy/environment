<?php

/**
 * @file Executes the environment_switch capabilities
 */

/**
 * Implementation of hook_drush_command().
 */
function environment_drush_command() {
  $items = array();

  $items['environment'] = array(
    'description' => 'Show the current environment state.',
    'drupal dependencies' => array('environment'),
    'aliases' => array('env'),
  );

  $items['environment-switch'] = array(
    'description' => 'Switch the environment to specified state.',
    'drupal dependencies' => array('environment'),
    'arguments' => array(
      'target_env' => 'The target environment to which the site will be switched.',
    ),
    'options' => array(
      '--force' => 'Whether an environment switch should be forced if the target enviornment is the same as the current environment.',
    ),
    'examples' => array(
      'drush environment-switch development' => 'Switch the environment to development.',
      'drush env-switch --force production' => 'Force the environment to switch to production even if the current environment already is production.',
    ),
    'aliases' => array('env-switch'),
  );

  return $items;
}

/**
 * Implementation of hook_drush_help().
 */
function environment_drush_help($section) {
  switch ($section) {
    case 'meta:environment:title':
      return dt('Environment commands');
    case 'meta:environment:summary':
      return dt('Review and control the site environment.');
    case 'drush:environment':
      return dt("Shows the current environment.");
    case 'drush:environment-switch':
      return dt("Switches the environment to the target environment.");
  }
}

/**
 * Implementation of drush_hook_COMMAND for environment.
 */
function drush_environment() {
  drush_print(environment_drush_render_environment());
}

/**
 * Implementation of drush_hook_COMMAND_validate() for environment_switch.
 */
function drush_environment_switch_validate($target_env = NULL) {
  $environments = environment_load();
  if (empty($environments)) {
    return drush_set_error('DRUSH_ENVIRONMENT_ERROR', dt('There are no defined environments available.'));
  }
}

/**
 * Implementation of drush_hook_COMMAND for environment_switch.
 */
function drush_environment_switch($target_env = NULL) {
  if ($target_env && !array_key_exists($target_env, environment_load())) {
    drush_set_error('DRUSH_ENVIRONMENT_ERROR', dt('Invalid environment specified.'));
    $target_env = NULL;
  }
  if (!$target_env) {
    $environments = environment_load();
    $options = array();
    foreach ($environments as $name => $env) {
      $options[$name] = environment_drush_render_environment($name);
    }
    $target_env = drush_choice($options);
  }
  if (!$target_env) {
    return;
  }

  $force = drush_get_option('force');
  $current_env  = environment_current();
  $env_override = variable_get('environment_override', NULL);

  if (!$force && $current_env == $target_env) {
    drush_log("The current environment is already set to '{$current_env}'.", 'error');
    drush_print("To force the environment switch to run anyway, use the '--force' flag.");
    return;
  }
  elseif (!$force && $env_override && $target_env != $env_override) {
    drush_log("The current environment is overriden with '{$env_override}'.", 'error');
    drush_print("To force the environment switch to run anyway, use the '--force' flag.");
    return;
  }

  drush_print("Switching the environment to {$target_env}...");

  if (!drush_get_context('DRUSH_SIMULATE')) {
    $result   = environment_switch($target_env, $force);
    $messages = $result['messages'];

    foreach ($messages as $message) {
      drush_print('- ' . $message);
    }
  }
  else {
    $result['result'] = TRUE;
  }

  if ($result['result']) {
    drush_log('Done.', 'success');
  }
  else {
    drush_log('Failed.', 'error');
  }
}

/**
 * Helper function to render an environment as a descriptive one-liner.
 *
 * @param $env
 *  Name of an environment to render.
 *
 * @return
 *  String describing the environment.
 */
function environment_drush_render_environment($env = NULL) {
  $environment = !$env ? environment_current(TRUE) : environment_load($env);
  $description = isset($environment['description']) ? ': ' . $environment['description'] : '';
  return $environment['label'] . ' (' . $environment['name'] . ')' . $description;
}
