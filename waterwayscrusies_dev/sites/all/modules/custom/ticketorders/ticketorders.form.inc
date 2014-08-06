<?php

function ticketorders_add() {

    $ticketorders = (object) array(
                'id' => '',
                'ticket_id' => '',
                'order_id' => '',
                'ticket_qty' => '',
    );

    return drupal_get_form('ticketorders_add_form', $ticketorders);
}

function ticketorders_add_form($form, &$form_state, $ticketorders) {

    $formstatus = $ticketorders->id == '' ? "add" : "edit";
    $form['operation'] = array(
        '#type' => 'hidden',
        '#value' => $formstatus
    );

    $auto_id = $ticketorders->id != '' ? $ticketorders->id : "";
    $form['auto_id'] = array(
        '#type' => 'hidden',
        '#value' => $auto_id
    );


    $form['ticket_id'] = array(
        '#type' => 'textfield',
        '#title' => t('Ticket ID'),
        '#default_value' => isset($ticketorders->ticket_id) ? $ticketorders->ticket_id : '',
    );
    
    
    $form['order_id'] = array(
        '#type' => 'textfield',
        '#title' => t('Order ID'),
        '#default_value' => isset($ticketorders->order_id) ? $ticketorders->order_id : '',
    );
    
    
     $form['ticket_qty'] = array(
        '#type' => 'textfield',
        '#title' => t('Ticket Quantity'),
        '#default_value' => isset($ticketorders->ticket_qty) ? $ticketorders->ticket_qty : '',
    );
    
    
    field_attach_form('ticketorders', $ticketorders, $form, $form_state);
    $form['submit'] = array(
        '#type' => 'submit',
        '#value' => t('Save'),
    );
    return $form;
}

function ticketorders_add_form_validate($form, &$form_state) {
    $ticketorders = (object) $form_state['values'];
    field_attach_form_validate('ticketorders', $ticketorders, $form, $form_state);
}

function ticketorders_add_form_submit($form, &$form_state) {


    $ticket_id = $form_state['values']['ticket_id'];
    $order_id = $form_state['values']['order_id'];
    $ticket_qty = $form_state['values']['ticket_qty'];
    $auto_id = $form_state['values']['auto_id'];

    $ticketorders = new stdClass();
    $ticketorders->id = $auto_id;
    $ticketorders->ticket_id = $ticket_id;
    $ticketorders->order_id = $order_id;
    $ticketorders->ticket_qty = $ticket_qty;

    field_attach_submit('ticketorders', $ticketorders, $form, $form_state);
    $ticketorders = ticketorders_save($ticketorders);

    $operation = $form_state['values']['operation'];
    $msg = '';
    if ($operation == "add") {
        $msg = 'Ticket Orders Created Successfully.';
    } else {
        $msg = 'Ticket Orders Updated Successfully.';
    }

    drupal_set_message($msg);
    $form_state['redirect'] = 'admin/structure/ticketorders/manage';
}