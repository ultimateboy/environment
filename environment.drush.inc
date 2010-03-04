<?php
// $Id$

/**
 * @file Executes the environment_switch capabilities
 */

/**
 * Implementation of hook_drush_command().
 */
function environment_drush_command() {
  $items = array();

  $items['env switch'] = array(
    'description' => 'Switch the environment to the given mode',
    'callback' => 'environment_switch',
    'arguments' => array(
      'target_env' => 'The target environment to switch to',
      'force' => 'Whether or not the environment switch should be forced if the target enviornment is the same as the current environment.',
    ),
    'examples' => array(
      'drush env switch development' => 'Switch the environment to development',
      'drush env switch production force' => 'Force the environment to switch to production even if the current environment already is production',
    ),
  );

  return $items;
}

/**
 * Implementation of hook_drush_help().
 */
function environment_drush_help($section) {
  switch ($section) {
    case 'drush:env switch':
      $help = dt("Switches the environment to the target environment.");
      break;
  }
  return $help;
}

/**
 * Drush callback function to switch an environment
 */
function environment_switch() {
  $args = func_get_args();

  // Figure out if the user set the force flag
  $force = FALSE;
  if (in_array('force', $args)) {
    $force = TRUE;
    // Remove the flag from the args and reset the array
    foreach ($args as $key => $arg) {
      if ($arg == 'force') {
        unset($args[$key]);
      }
    }
    sort($args);
  }

  // Warn the user if  no environment arg was set
  if (count($args) < 1) {
    drush_print("ERROR: You must provide the environment to switch to.\n");
    drush_print(environment_drush_help('drush:env_switch'));
    return;
  }

  $target_env = $args[0];

  $current_env = variable_get('environment', 'unset');
  if ($current_env == $target_env && !$force) {
    drush_print("WARNING: The current environment is already set to '{$current_env}'.\n");
    drush_print("To force the environment switch to run, use the 'force' flag.\n");
    return;
  }

  drush_print("Switching the environment to {$target_env}...\n");

  // Register the new environment
  variable_set('environment', $target_env);

  // In many cases, simply clearing the cache will work
  drupal_flush_all_caches();;
  drush_print("- Cleared cache");

  // Some settings will require special handling.  Allow other modules to
  // run their own logic to properly switch the mode.
  // Invokes hook_environment_switch().
  $messages = array();
  $messages = module_invoke_all('environment_switch', $target_env, $current_env);
  foreach ($messages as $message) {
    drush_print($message);
  }

  // Clearing the cache one last time
  if (count($messages) > 0) {
    drupal_flush_all_caches();
  }

  drush_print("\nDone.\n");
}
