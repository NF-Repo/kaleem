<?php

/**
 * @file
 * Administrative page callbacks for the modal_forms module.
 */

/**
 * General configuration form for controlling the modal_forms behaviour.
 */
function modal_forms_admin_settings() {

  $form['modal_forms_popup_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('popup links settings'),
  );
  
 
 $form['modal_forms_popup_settings']['modal_forms_popup'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable for popup links'),
    '#default_value' => variable_get('modal_forms_popup', 0),
    '#description' => t('Automatically activate Modal forms for links to popup.'),
  );
  return system_settings_form($form);
}
