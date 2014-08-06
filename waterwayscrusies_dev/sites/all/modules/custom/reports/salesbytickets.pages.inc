<?php

/**
 * Displays the sales by cruise order reports.
 */
function salesbytickets_order_reports() {
    return drupal_render(drupal_get_form('salesbytickets_reports_form'));
}

function salesbytickets_reports_form_validate($form, &$form_state) {
    if ($form_state['triggering_element']['#value'] == 'Generate Report') {
        $form_state['rebuild'] = TRUE;
        return;
    }
}

function salesbytickets_reports_form($form, &$form_state) {
    drupal_add_css(drupal_get_path('module', 'reports') . '/reports.css');
    //getting the portlist
    $ticketslist = array();
    $ticketslist = getTicketslist();
    $ticketsarray = array();
    $ticketsarray[0]='Select ticket';
    foreach ($ticketslist as $ticket) {
        $ticketsarray[$ticket->id] = $ticket->title;
    }
    drupal_add_css(drupal_get_path('module', 'reports') . '/reports.css');
//    $option = 'completed';
    $option = 'payment_received';
//    if (isset($_GET['option'])) {
//        $option = $_GET['option'];
//    }
//    if (isset($_GET['order_startdate']['date']) && $_GET['order_startdate']['date'] != '') {
//        $order_startdate = $_GET['order_startdate']['date'];
//        $order_starttime = strtotime($order_startdate . " 00:00:00");
//    }
//    if (isset($_GET['order_enddate']['date']) && $_GET['order_enddate']['date'] != '') {
//        $order_enddate = $_GET['order_enddate']['date'];
//        $order_endtime = strtotime($order_enddate . " 23:59:59");
//    }
    $orderstatusoptions = uc_order_status_options_list();
    global $user;
    if (isset($_GET['export_btn'])) {
        salesbytickets_reports_export($_GET);
    }
    if (in_array('staff admin', $user->roles)) {
//        $orderstatusoptions = array('completed' => 'Completed', 'payment_received' => 'Payment received');
         $orderstatusoptions = array( 'payment_received' => 'Payment received',  'pending' => 'Pending');
    }
    $form['#method'] = 'get';
//    $form['option'] = array(
//        '#type' => 'select',
//        '#options' => $orderstatusoptions,
//        '#default_value' => $option,
//        '#prefix' => '<div class="row-fluid"><div class="span12"><div class="pull-left">',
//        '#suffix' => '</div>',
//        '#attributes' => array('class' => array('input-medium')),
//    );
    $order_startdate = date('Y-m-01');
    $order_enddate = date('Y-m-t');
    $order_starttime = strtotime($order_startdate . " 00:00:00");
    $order_endtime = strtotime($order_enddate . " 23:59:59");

    if (isset($_GET['order_startdate']['date'])) {
        $order_startdate = '';
        $order_starttime = '';
        if ($_GET['order_startdate']['date'] != '') {
            $order_startdate = $_GET['order_startdate']['date'];
            $order_starttime = strtotime($order_startdate . " 00:00:00");
        }
    }
    if (isset($_GET['order_enddate']['date'])) {
        $order_enddate = '';
        $order_endtime = '';
        if ($_GET['order_enddate']['date'] != '') {
            $order_enddate = $_GET['order_enddate']['date'];
            $order_endtime = strtotime($order_enddate . " 23:59:59");
        }
    }
    $ticket = '0';
    if (isset($_GET['ticket'])) {
        $ticket = $_GET['ticket'];
    }
    $form['ticket'] = array(
        '#type' => 'select',
        '#options' => $ticketsarray,
        '#default_value' => $ticket,
        '#prefix' => '<div class="row-fluid"><div class="span12"><div class="pull-left">',
        '#suffix' => '</div>',
//        '#attributes' => array('class' => array('input-medium')),
    );
    $form['order_startdate'] = array(
        '#id' => 'order_start_date',
        '#type' => 'date_popup',
        '#date_format' => 'Y-m-d',
        '#title' => t(''),
        '#description' => '',
        '#attributes' => array('class' => array('dateFieldStyle dateFieldMedium')),
        '#default_value' => isset($order_startdate) ? $order_startdate : '',
//        '#prefix' => '<div class="row-fluid"><div class="span3">',
        '#prefix' => '<div class="pull-left" style="margin:13px 0 0 13px;">',
        '#suffix' => '</div>',
        '#attributes' => array('placeholder' => array('Start Date')),
    );
    $form['order_enddate'] = array(
        '#id' => 'order_end_date',
        '#type' => 'date_popup',
        '#date_format' => 'Y-m-d',
        '#title' => t(''),
        '#default_value' => isset($order_enddate) ? $order_enddate : '',
        '#description' => '',
        '#attributes' => array('class' => array('dateFieldStyle dateFieldMedium')),
        '#prefix' => '<div class="pull-left" style="margin-top:13px;">',
        '#suffix' => '</div>',
        '#attributes' => array('placeholder' => array('End Date')),
    );


    $form['btn'] = array(
        '#type' => 'submit',
        '#value' => 'Generate Report',
        '#prefix' => '<div class="pull-left" style="margin-top:13px;">',
        '#suffix' => '</div>',
    );
    $form['export_btn'] = array(
        '#name' => 'export_btn',
        '#type' => 'submit',
        '#value' => t('Export'),
        '#submit' => array('salesbytickets_reports_export'),
        '#prefix' => '<div class="pull-left" style="margin:14px 0px 0px 6px;">',
        '#suffix' => '</div></div></div>',
    );

    $output = '';

    $header = array(
        array(
            'data' => 'Sno',
            'field' => 'position'
        ),
        array(
            'data' => 'Title',
        ),
        array(
            'data' => 'Tickets',
            'field' => 'totaltickets'
        ),
        array(
            'data' => 'Total',
            'field' => 'total'
        ),
    );
    if (isset($_GET ['sort']) && isset($_GET ['order'])) {
        if ($_GET ['sort'] == 'asc')
            $sort = 'ASC';
        else
            $sort = 'DESC';

        switch ($_GET ['order']) {
            case 'Sno' :
                $order = 'position';
                break;
            case 'Tickets' :
                $order = 'totaltickets';
            case 'Total' :
                $order = 'total';
                break;
        }
    } else {
        $sort = 'ASC';
        $order = 'position';
    }

    $query = db_select("bookinglog_tickets", "b");
//    $query->Join('reservation', 'r', 's.id = r.eventid');
    $query->Join('uc_orders', 'c', 'b.order_id = c.order_id');
    if ($order_starttime != '' && $order_endtime != '') {
        if ($order_starttime < $order_endtime != '') {
            $order_enddate = $order_enddate . " 23:59:59";
            $order_startdate = $order_startdate . " 00:00:00";
            $query->where("c.created >= UNIX_TIMESTAMP('$order_startdate') and c.created <= UNIX_TIMESTAMP('$order_enddate')");
        } else {
            drupal_set_message("End date must be Greater than start date",'warning');
        }
    }
    $query->fields('b', array(
        'ticket_id'
    ));

    $query->addExpression('@row := @row + 1', 'position');
    $query->addExpression('COUNT(c.order_id)', 'orders');
    $query->addExpression('SUM(b.ticket_qty)', 'totaltickets');
    $query->addExpression('b.ticket_price*SUM(b.ticket_qty)', 'total');
    if ($option != '') {
        $query->condition('c.order_status', $option, '=');
    }
    if ($ticket != 0) {
        $query->condition('b.ticket_id', $ticket, '=');
    }
    $query->groupBy('b.ticket_id');
//    $query->orderBy($order, $sort);
    $query = $query->extend('TableSort')->extend('PagerDefault')->limit(10);
    /*   for creating mysql custom index column for the orders    */
    db_query("set @row=0");
    $result = $query->execute()->fetchAll();

    $rows = array();
    if (count($result) > 0) {
        for ($i = 0; $i < count($result); $i ++) {
            $rows [] = array(
                $result [$i]->position,
                $ticketsarray[$result [$i]->ticket_id],
                $result [$i]->totaltickets,
                uc_currency_format($result [$i]->total),
            );
        }
    }

    $output .= '<div class="">';
    $output .= '<div class="">';

    $output .= '</div>';
    $output .= theme_table(array(
                'header' => $header,
                'rows' => $rows,
                'attributes' => array(
                    'class' => array()
                ),
                'sticky' => true,
                'caption' => '',
                'colgroups' => array(),
                'empty' => t("No Records!")
            )) . theme('pager');

    $output .= '</div>';


    $form['outputmarkup'] = array(
        '#type' => 'markup',
        '#markup' => $output
    );
    return $form;
}


function salesbytickets_reports_export($params){
    $ticketslist = array();
    $ticketslist = getTicketslist();
    $ticketsarray = array();
    $ticketsarray[0]='Select ticket';
    foreach ($ticketslist as $ticket) {
        $ticketsarray[$ticket->id] = $ticket->title;
    }
    drupal_add_css(drupal_get_path('module', 'reports') . '/reports.css');
//    $option = 'completed';
    $option = 'payment_received';
    
    $order_startdate = date('Y-m-01');
    $order_enddate = date('Y-m-t');
    $order_starttime = strtotime($order_startdate . " 00:00:00");
    $order_endtime = strtotime($order_enddate . " 23:59:59");

    if (isset($params['order_startdate']['date'])) {
        $order_startdate = '';
        $order_starttime = '';
        if ($params['order_startdate']['date'] != '') {
            $order_startdate = $params['order_startdate']['date'];
            $order_starttime = strtotime($order_startdate . " 00:00:00");
        }
    }
    if (isset($params['order_enddate']['date'])) {
        $order_enddate = '';
        $order_endtime = '';
        if ($params['order_enddate']['date'] != '') {
            $order_enddate = $params['order_enddate']['date'];
            $order_endtime = strtotime($order_enddate . " 23:59:59");
        }
    }
    $ticket = '0';
    if (isset($params['ticket'])) {
        $ticket = $params['ticket'];
    }
    

    $output = '';

    $header = array(
            0 => 'Sno',
            1 => 'Title',
            2 => 'Tickets',
            3 => 'Total'
    );
   

    $query = db_select("bookinglog_tickets", "b");
//    $query->Join('reservation', 'r', 's.id = r.eventid');
    $query->Join('uc_orders', 'c', 'b.order_id = c.order_id');
    if ($order_starttime != '' && $order_endtime != '') {
        if ($order_starttime < $order_endtime != '') {
            $order_enddate = $order_enddate . " 23:59:59";
            $order_startdate = $order_startdate . " 00:00:00";
            $query->where("c.created >= UNIX_TIMESTAMP('$order_startdate') and c.created <= UNIX_TIMESTAMP('$order_enddate')");
        } else {
            drupal_set_message("End date must be Greater than start date",'warning');
        }
    }
    $query->fields('b', array(
        'ticket_id'
    ));

    $query->addExpression('@row := @row + 1', 'position');
    $query->addExpression('COUNT(c.order_id)', 'orders');
    $query->addExpression('SUM(b.ticket_qty)', 'totaltickets');
    $query->addExpression('b.ticket_price*SUM(b.ticket_qty)', 'total');
    if ($option != '') {
        $query->condition('c.order_status', $option, '=');
    }
    if ($ticket != 0) {
        $query->condition('b.ticket_id', $ticket, '=');
    }
    $query->groupBy('b.ticket_id');
//    $query->orderBy($order, $sort);
    $query = $query->extend('TableSort')->extend('PagerDefault')->limit(10);
    /*   for creating mysql custom index column for the orders    */
    db_query("set @row=0");
    $result = $query->execute()->fetchAll();

    $rows = array();
    if (count($result) > 0) {
        for ($i = 0; $i < count($result); $i ++) {
            $rows [] = array(
                $result [$i]->position,
                $ticketsarray[$result [$i]->ticket_id],
                $result [$i]->totaltickets,
                uc_currency_format($result [$i]->total),
            );
        }
    }
    
    
    excel_export_data('Sales By Tickets', $header, $rows);
}