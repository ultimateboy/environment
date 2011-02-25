<?php

/**
 * @file
 *  Hooks provided by Environment.
 */

/**
 * @addtogroup hooks
 * @{
 */

/**
 * React to an environment state change.
 *
 * Use this hook to specify changes to your site configuration depending on
 * what kind of environment the site is operating in. For example, production
 * environments should not have developer/site-builder oriented modules enabled,
 * such as administrative UI modules.
 *
 * When defining your state change actions, be careful to account for a given
 * state always consisting of the same behaviors and configuration, regardless
 * of how it returns to that state (which previous environment it was in.) Be
 * careful that you do not *disable* any modules in one environment that
 * implement a necessary instance of hook_environment_switch().
 *
 * @param $target_env
 *  The name of the environment being activated.
 * @param $current_env
 *  The name of the environment being deactivated.
 *
 * @return
 *  String summarizing changes made for drush user.
 */
function hook_environment_switch($target_env, $current_env) {
  // Declare each optional development-related module
  $devel_modules = array(
    'devel',
    'devel_generate',
    'devel_node_access',
  );

  // Disable all devel modules for production mode.
  if ($target_env == 'production') {
    module_disable($devel_modules);
    return '- Disabled development modules';
  }

  // Enable the modules in any other mode.
  elseif ($current_env == 'production') {
    module_enable($devel_modules);
    return '- Reenabled development modules';
  }
}

/**
 * Declare additional environments.
 *
 * This hook is to facilitate UI building and restricting environment switch to
 * known environments.
 *
 * @return
 *  Array of environment names in the format:
 *  - label: Human-readable name for the environment.
 *  - description: Description of the environment and it's purpose.
 *  - allowed: Central definition of permitted operations for the
 *    environment_allowed() function. Default FALSE indicates that something
 *    should not happen, such as show the user a debugging message. Different
 *    categories can be specified for different rulesets.
 *
 * @see environment_allowed
 */
function hook_environment() {
  $environments = array();
  $environments['stage'] = array(
    'label' => t('Staging'),
    'description' => t('Staging sites are for content creation before publication.'),
    'allowed' => array(
      'default' => FALSE,
      'email' => FALSE,
    ),       
  );
  return $environments;
}

/**
 * Alter the environments as defined.
 *
 * This is especially useful to modify the defined allowed operations.
 *
 * @param $environments
 *  Defined environment states.
 */
function hook_environment_alter(&$environments) {
  $environments['production'] = t('Production site');
}

/**
 * @} End of "addtogroup hooks".
 */
