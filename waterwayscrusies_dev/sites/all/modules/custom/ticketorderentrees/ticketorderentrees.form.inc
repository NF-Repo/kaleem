<?php

function ticketorderentrees_add() {

    $ticketorderentrees = (object) array(
                'id' => '',
                'ticket_id' => '',
                'order_id' => '',
                'entree_id' => '',
                'dietary_text' => '',
    );

    return drupal_get_form('ticketorderentrees_add_form', $ticketorderentrees);
}

function ticketorderentrees_add_form($form, &$form_state, $ticketorderentrees) {

    $formstatus = $ticketorderentrees->id == '' ? "add" : "edit";
    $form['operation'] = array(
        '#type' => 'hidden',
        '#value' => $formstatus
    );

    $auto_id = $ticketorderentrees->id != '' ? $ticketorderentrees->id : "";
    $form['auto_id'] = array(
        '#type' => 'hidden',
        '#value' => $auto_id
    );


    $form['ticket_id'] = array(
        '#type' => 'textfield',
        '#title' => t('Ticket ID'),
        '#default_value' => isset($ticketorderentrees->ticket_id) ? $ticketorderentrees->ticket_id : '',
    );


    $form['order_id'] = array(
        '#type' => 'textfield',
        '#title' => t('Order ID'),
        '#default_value' => isset($ticketorderentrees->order_id) ? $ticketorderentrees->order_id : '',
    );


    $form['entree_id'] = array(
        '#type' => 'textfield',
        '#title' => t('Entree ID'),
        '#default_value' => isset($ticketorderentrees->entree_id) ? $ticketorderentrees->entree_id : '',
    );

    $form['dietary_text'] = array(
        '#type' => 'textfield',
        '#title' => t('Dietary Text'),
        '#default_value' => isset($ticketorderentrees->dietary_text) ? $ticketorderentrees->dietary_text : '',
    );


    field_attach_form('ticketorderentrees', $ticketorderentrees, $form, $form_state);
    $form['submit'] = array(
        '#type' => 'submit',
        '#value' => t('Save'),
    );
    return $form;
}

function ticketorderentrees_add_form_validate($form, &$form_state) {
    $ticketorderentrees = (object) $form_state['values'];
    field_attach_form_validate('ticketorderentrees', $ticketorderentrees, $form, $form_state);
}

function ticketorderentrees_add_form_submit($form, &$form_state) {

    $auto_id = $form_state['values']['auto_id'];
    $ticket_id = $form_state['values']['ticket_id'];
    $order_id = $form_state['values']['order_id'];
    $entree_id = $form_state['values']['entree_id'];
    $dietary_text = $form_state['values']['dietary_text'];


    $ticketorderentrees = new stdClass();
    $ticketorderentrees->id = $auto_id;
    $ticketorderentrees->ticket_id = $ticket_id;
    $ticketorderentrees->order_id = $order_id;
    $ticketorderentrees->entree_id = $entree_id;
    $ticketorderentrees->dietary_text = $dietary_text;

    field_attach_submit('ticketorderentrees', $ticketorderentrees, $form, $form_state);
    $ticketorderentrees = ticketorderentrees_save($ticketorderentrees);

    $operation = $form_state['values']['operation'];
    $msg = '';
    if ($operation == "add") {
        $msg = 'Ticket Order Entrees Created Successfully.';
    } else {
        $msg = 'Ticket Order Entrees Updated Successfully.';
    }

    drupal_set_message($msg);
    $form_state['redirect'] = 'admin/structure/ticketorderentrees/manage';
}