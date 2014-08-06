<?php

/**
 * Displays the product reports.
 */
function salescommission_order_reports() {
    return drupal_render(drupal_get_form('salescommission_order_reports_form'));
}

function salescommission_order_reports_form_validate($form, &$form_state) {
    if ($form_state['triggering_element']['#value'] == 'Generate Report') {
        $form_state['rebuild'] = TRUE;
        return;
    }
}

function salescommission_order_reports_form($form, &$form_state) {

    drupal_add_css(drupal_get_path('module', 'reports') . '/reports.css');
//    $option = 'completed';
    $option = 'payment_received';
    if (isset($_GET['option'])) {
        $option = $_GET['option'];
    }

    $agentname = isset($_GET['agentname']) ? $_GET['agentname'] : '';
    $agentname = trim($agentname);
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
    $orderstatusoptions = uc_order_status_options_list();
    global $user;
    if (in_array('staff admin', $user->roles)) {
//        $orderstatusoptions = array('completed' => 'Completed', 'payment_received' => 'Payment received');
        $orderstatusoptions = array('payment_received' => 'Payment received', 'pending' => 'Pending');
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
    $form['agentname'] = array(
        '#id' => 'agentname',
        '#type' => 'textfield',
        '#title' => t(''),
        '#description' => '',
        '#default_value' => isset($agentname) ? $agentname : '',
        '#prefix' => '<div class="row-fluid"><div class="span12"><div class="pull-left">',
        '#suffix' => '</div>',
        '#attributes' => array('class' => array('input-medium'), 'placeholder' => array('Agent Name')),
//        '#attributes' => array('placeholder' => array('Cruise Title')),
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
        '#submit' => array('salescommission_reports_export'),
        '#prefix' => '<div class="pull-left" style="margin:14px 0px 0px 6px;">',
        '#suffix' => '</div></div></div>',
    );


    if (isset($_GET['export_btn'])) {
        $_GET['agentid'] = $agentid;
        salescommission_reports_export($_GET);
    }

    $output = '';

    $header = array(
        array(
            'data' => 'SNo',
            'field' => 'position'
        ),
        array(
            'data' => 'Agent Name',
            'field' => 'first_name'
        ),
        array(
            'data' => 'OrderId'
//            'field' => 'order_id'
        ),
        array(
            'data' => 'Sub Total',
        ),
        array(
            'data' => 'Total',
            'field' => 'order_total'
        ),
        array(
            'data' => 'Commission',
        ),
    );
    if (isset($_GET ['sort']) && isset($_GET ['order'])) {
        if ($_GET ['sort'] == 'asc')
            $sort = 'ASC';
        else
            $sort = 'DESC';

        switch ($_GET ['order']) {
            case 'SNo' :
                $order = 'position';
                break;
            case 'OrderId' :
                $order = 'order_id';
            case 'Agent Name' :
                $order = 'first_name';
            case 'Total' :
                $order = 'order_total';
                break;
        }
    } else {
        $sort = 'ASC';
        $order = 'position';
    }

    $commissionrate = variable_get('WWCAGENTCOMMVALUE', '10');
    $query = db_select("uc_orders", "c");
    $query->Join('reservation', 'r', 'c.order_id = r.orderid');
    $query->Join('user_accounts', 'a', 'r.uid = a.uid');
//    $query->leftJoin('uc_order_line_items', 'l', 'l.order_id = r.orderid');
    if ($order_starttime != '' && $order_endtime != '') {
        if ($order_starttime < $order_endtime != '') {
            $order_enddate = $order_enddate . " 23:59:59";
            $order_startdate = $order_startdate . " 00:00:00";
            $query->where("c.created >= UNIX_TIMESTAMP('$order_startdate') and c.created <= UNIX_TIMESTAMP('$order_enddate')");
        } else {
            drupal_set_message("End date must be Greater than start date");
        }
    }

    $query->fields('c', array(
        'order_id',
        'uid',
        'order_total',
        'delivery_first_name',
        'delivery_last_name',
        'primary_email',
        'created',
        'delivery_phone',
        'order_status',
    ));
    $query->fields('a', array(
        'first_name',
        'last_name',
        'phone'
    ));
//     $query->fields('l', array(
//        'amount',
//        'title',
//    ));
    $query->addExpression('@row := @row + 1', 'position');
    $query->groupBy('c.uid');
    if ($option != '') {
        $query->condition('order_status', $option, '=');
    }

    if ($agentname != '') {
        $db_or = db_or();
        $db_or->condition('a.first_name', '%' . db_like($agentname) . '%', 'LIKE');
        $db_or->condition('a.last_name', '%' . db_like($agentname) . '%', 'LIKE');
        $query->condition($db_or);
    }
    $query->condition('r.bookedby', 'agent', '=');
//    $query->condition('l.title', '%'.db_like('Tax') . '%', 'LIKE');
    $query->orderBy('first_name', 'ASC');
    $query->orderBy($order, $sort);
    $query = $query->extend('TableSort')->extend('PagerDefault')->limit(10);
    db_query("set @row=0");
    $result = $query->execute()->fetchAll();
    $rows = array();
    if (count($result) > 0) {
        for ($i = 0; $i < count($result); $i ++) {
            $subtotal = '';
            $subtotal = getordersubtotal($result [$i]->order_id);
            $ordertaxdata = getcruiseordertax($result [$i]->order_id);
            $ordertax = isset($ordertaxdata[0]->amount) ? $ordertaxdata[0]->amount : 0;
            $rows [] = array(
                $result [$i]->position,
                $result [$i]->first_name . ' ' . $result [$i]->last_name,
                $result [$i]->order_id,
//                uc_currency_format($subtotal),
                uc_currency_format($result [$i]->order_total - $ordertax),
                uc_currency_format($result [$i]->order_total),
                uc_currency_format(($result [$i]->order_total - $ordertax) * $commissionrate / 100),
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

function salescommission_reports_export($params) {
    $option = 'payment_received';
    if (isset($params['option'])) {
        $option = $params['option'];
    }

    $agentname = isset($params['agentname']) ? $params['agentname'] : '';
    $agentname = trim($agentname);
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

    $header = array(
        0 => 'SNo',
        1 => 'Agent Name',
        2 => 'OrderId',
        3 => 'Sub Total',
        4 => 'Total',
        5 => 'Commission'
    );

    $commissionrate = 10;
    $query = db_select("uc_orders", "c");
    $query->Join('reservation', 'r', 'c.order_id = r.orderid');
    $query->Join('user_accounts', 'a', 'r.uid = a.uid');
    $query->leftJoin('uc_order_line_items', 'l', 'l.order_id = r.orderid');
    if ($order_starttime != '' && $order_endtime != '') {
        if ($order_starttime < $order_endtime != '') {
            $order_enddate = $order_enddate . " 23:59:59";
            $order_startdate = $order_startdate . " 00:00:00";
            $query->where("c.created >= UNIX_TIMESTAMP('$order_startdate') and c.created <= UNIX_TIMESTAMP('$order_enddate')");
        } else {
            drupal_set_message("End date must be Greater than start date");
        }
    }

    $query->fields('c', array(
        'order_id',
        'uid',
        'order_total',
        'delivery_first_name',
        'delivery_last_name',
        'primary_email',
        'created',
        'delivery_phone',
        'order_status',
    ));
    $query->fields('a', array(
        'first_name',
        'last_name',
        'phone'
    ));
//     $query->fields('l', array(
//        'amount',
//        'title',
//    ));
    $query->addExpression('@row := @row + 1', 'position');
    $query->groupBy('c.uid');
    if ($option != '') {
        $query->condition('order_status', $option, '=');
    }

    if ($agentname != '') {
        $db_or = db_or();
        $db_or->condition('a.first_name', '%' . db_like($agentname) . '%', 'LIKE');
        $db_or->condition('a.last_name', '%' . db_like($agentname) . '%', 'LIKE');
        $query->condition($db_or);
    }
    $query->condition('r.bookedby', 'agent', '=');
//    $query->condition('l.title', '%'.db_like('Tax') . '%', 'LIKE');
//    $query->orderBy('first_name', 'ASC');
//    $query->orderBy($order, $sort);
//    $query = $query->extend('TableSort')->extend('PagerDefault')->limit(10);
    db_query("set @row=0");
    $result = $query->execute()->fetchAll();
    $rows = array();
    if (count($result) > 0) {
        for ($i = 0; $i < count($result); $i ++) {
            $subtotal = '';
            $subtotal = getordersubtotal($result [$i]->order_id);
            $ordertaxdata = getcruiseordertax($result [$i]->order_id);
            $ordertax = isset($ordertaxdata[0]->amount) ? $ordertaxdata[0]->amount : 0;
            $rows [] = array(
                $result [$i]->position,
                $result [$i]->first_name . ' ' . $result [$i]->last_name,
                $result [$i]->order_id,
                uc_currency_format($subtotal),
                uc_currency_format($result [$i]->order_total),
                uc_currency_format(($result [$i]->order_total - $ordertax) * $commissionrate / 100),
            );
        }
    }

    excel_export_data('Sales By Agent Commission', $header, $rows);
}
