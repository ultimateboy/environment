<?php

/**
 * @file
 *  Describe context plugins for Environment module.
 */

/**
 * Implementation of hook_ctools_plugin_api().
 */
function environment_ctools_plugin_api($module, $api) {
  if ($module == 'context' && $api == 'plugins') {
    return array('version' => 3);
  }
}

/**
 * Implementation of hook_context_registry().
 */
function environment_context_registry() {
  $registry = array();
  $registry['conditions'] = array(
    'environment' => array(
      'title' => t('Environment'),
      'description' => t('Set this context when viewing a page with the given environment.'),
      'plugin' => 'environment_context_condition',
    ),
  );
  return $registry;
}

/**
 * Implementation of hook_context_plugins().
 */
function environment_context_plugins() {
 $plugins = array();
  $plugins['environment_context_condition'] = array(
    'handler' => array(
      'path' => drupal_get_path('module', 'environment') .'/plugins',
      'file' => 'environment_context_condition.inc',
      'class' => 'environment_context_condition',
      'parent' => 'context_condition',
    ),
  );
  return $plugins;
}
