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
    $tokens['global']['environment'] = t('The current environment state.');
    $tokens['global']['environment-label'] = t('The human-readable version of the current environment state.');
  }
  return $tokens;
}

/**
 * Implementation of hook_token_values().
 */
function environment_token_values($type, $object = NULL, $options = array()) {
  $values = array();
  if ($type == 'global') {
    $values['environment'] = environment_current();
    $values['environment-label'] = environment_current(TRUE, 'label');
  }
  return $values;
}
