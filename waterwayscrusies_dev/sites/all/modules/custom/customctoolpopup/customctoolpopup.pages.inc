<?php

/**
 * A modal Add popup callback.
 */
function modal_forms_add_popup($js = NULL) {
  // Fall back if $js is not set.
  if (!$js) {
    return drupal_get_form('open_popup_form');
  }

  ctools_include('modal');
  ctools_include('ajax');
  $form_state = array(
    'title' => t('Add popup'),
    'id'=>'open_popup_form',
    'ajax' => TRUE,
  );

  $output = ctools_modal_form_wrapper('open_popup_form', $form_state);
  if (!empty($form_state['executed'])) {
    // We'll just overwrite the form output if it was successful.
    $output = array();
    ctools_add_js('ajax-responder');
    $output[] = ctools_modal_command_dismiss(t('Popup Added Successfully'));
    
      $output[] = ctools_ajax_command_reload();

  }
  print ajax_render($output);
}



function modal_forms_add_one($js = NULL) {
  // Fall back if $js is not set.
  if (!$js) {
    return drupal_get_form('add_one_form');
  }

  ctools_include('modal');
  ctools_include('ajax');
  $form_state = array(
    'title' => t('Add Doctor'),
    'id'=>'add_one_form',
    'ajax' => TRUE,
  );

  $output = ctools_modal_form_wrapper('add_one_form', $form_state);
  if (!empty($form_state['executed'])) {
    // We'll just overwrite the form output if it was successful.
    $output = array();
    ctools_add_js('ajax-responder');
    $output[] = ctools_modal_command_dismiss(t('Doctor Added Successfully'));

      $output[] = ctools_ajax_command_reload();

  }
  print ajax_render($output);
}