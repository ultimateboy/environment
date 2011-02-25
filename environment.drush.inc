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
    'description' => 'Show the environment mode',
    'drupal dependencies' => array('environment'),
    'aliases' => array('env'),
  );

  $items['environment-switch'] = array(
    'description' => 'Switch the environment to the given mode',
    'drupal dependencies' => array('environment'),
    'arguments' => array(
      'target_env' => 'The target environment to switch to',
    ),
    'options' => array(
      '--force' => 'Whether an environment switch should be forced if the target enviornment is the same as the current environment.',
    ),
    'examples' => array(
      'drush env-switch development' => 'Switch the environment to development',
      'drush env-switch --force production' => 'Force the environment to switch to production even if the current environment already is production',
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
    case 'drush:environment':
      $help = dt("Shows the current environment.");
      break;
    case 'drush:environment-switch':
      $help = dt("Switches the environment to the target environment.");
      break;
  }
  return $help;
}

/**
 * Drush callback function to show environment
 */
function drush_environment() {
  drush_print(environment_current(TRUE, 'label'));
}

/**
 * Drush callback function to switch an environment
 */
function drush_environment_switch() {
  if ($args = func_get_args()) {
    $force = drush_get_option('force');

    $target_env   = $args[0];
    $current_env  = environment_current();
    $env_override = variable_get('environment_override', NULL);

    if (!$force && $current_env == $target_env) {
      drush_print("WARNING: The current environment is already set to '{$current_env}'.");
      drush_print("To force the environment switch to run anyway, use the '--force' flag.");
    }
    else if (!$force && $env_override && $target_env != $env_override) {
      drush_print("WARNING: The current environment is overriden with '{$env_override}'.");
      drush_print("To force the environment switch to run anyway, use the '--force' flag.");
    }
    else {
      drush_print("Switching the environment to {$target_env}...");

      $result   = environment_switch($target_env, $force);
      $messages = $result['messages'];

      foreach ($messages as $message) {
        drush_print('- ' . $message);
      }

      if ($result['result']) {
        drush_print("Done.");
      }
      else {
        drush_print("Failed.");
      }
    }
  }
  else {
    return drush_set_error('DRUSH_ENVIRONMENT_ERROR', dt('You must provide the environment to switch to.'));
  }
}
