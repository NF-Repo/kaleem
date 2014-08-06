<?php

function cruisetypes_add() {

    $cruisetypes = (object) array(
                'id' => '',
                'name' => '',
               );

    return drupal_get_form('cruisetypes_add_form', $cruisetypes);
}

function cruisetypes_add_form($form, &$form_state, $cruisetypes) {

    $formstatus = $cruisetypes->id == '' ? "add" : "edit";
    $form['operation'] = array(
        '#type' => 'hidden',
        '#value' => $formstatus
    );

    $auto_id = $cruisetypes->id != '' ? $cruisetypes->id : "";
    $form['auto_id'] = array(
        '#type' => 'hidden',
        '#value' => $auto_id
    );


    $form['name'] = array(
        '#type' => 'textfield',
        '#title' => t('Name'),
        '#default_value' => isset($cruisetypes->name) ? $cruisetypes->name : '',
    	'#required'=>TRUE,
    );
    
    
    
    
    
    field_attach_form('cruisetypes', $cruisetypes, $form, $form_state);
    $form['submit'] = array(
        '#type' => 'submit',
        '#value' => t('Save'),
    );
    
    
    $form['cancel'] = array(
    		'#type'   => 'submit',
    		'#value'  => t('Cancel'),
    		'#submit' => array('cruisetype_cancel_button'),
    		'#limit_validation_errors' => array(),
    );
    
    
    
    
//     $form['cancel'] = array(
//     		'#type' => 'button',
//     		'#value' => t('Cancel'),
//     		'#attributes' => array('onClick' => 'history.go(-1); return true;'),
//     		'#post_render' => array('change_button_type'),
//     );
    
    
    return $form;
}


function cruisetype_cancel_button($form, &$form_state) {
	$url = 'admin/structure/cruisetypes/manage';
	drupal_goto($url);
}


function cruisetypes_add_form_validate($form, &$form_state) {
    $cruisetypes = (object) $form_state['values'];
    field_attach_form_validate('cruisetypes', $cruisetypes, $form, $form_state);
}

function cruisetypes_add_form_submit($form, &$form_state) {


    $name = $form_state['values']['name'];
    
    $auto_id = $form_state['values']['auto_id'];

    $cruisetypes = new stdClass();
    $cruisetypes->id = $auto_id;
    $cruisetypes->name = $name;
  

    field_attach_submit('cruisetypes', $cruisetypes, $form, $form_state);
    $cruisetypes = cruisetypes_save($cruisetypes);

    $operation = $form_state['values']['operation'];
    $msg = '';
    if ($operation == "add") {
        $msg = 'Cruise Type Created Successfully.';
    } else {
        $msg = 'Cruise Type Updated Successfully.';
    }

    drupal_set_message($msg);
    $form_state['redirect'] = 'admin/structure/cruisetypes/manage';
}



