<?php

/**
 * @file
 *  Token provider integration for Environment module.
 */

/**
 * Implementation of hook_token_list().
 */
function environment_token_list($type = 'all') {
  $tokens = array();
  if ($type == 'all' || $type == 'global') {
    foreach (environment_workflow_load() as $name => $workflow) {
      $tokens['global']['environment_' . $name] = t('The current environment state for @title workflow.', array('@title' => $workflow['label']));
      $tokens['global']['environment_' . $name . '-label'] = t('The human-readable version of the current environment state for @title workflow.', array('@title' => $workflow['label']));
    }
  }
  return $tokens;
}

/**
 * Implementation of hook_token_values().
 */
function environment_token_values($type, $object = NULL, $options = array()) {
  $values = array();
  if ($type == 'global') {
    foreach (environment_workflow_load() as $name => $workflow) {
      $state = environment_current($name, NULL, TRUE);
      $values['environment_' . $name] = $state['name'];
      $values['environment_' . $name . '-label'] = $state['label'];
    }
  }
  return $values;
}
