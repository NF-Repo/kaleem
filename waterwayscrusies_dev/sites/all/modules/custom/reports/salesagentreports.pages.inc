<?php

/**
 * Displays the product reports.
 */
function salesagent_order_reports() {
    return drupal_render(drupal_get_form('store_salesagentreports_products_form'));
}

function store_salesagentreports_products_form_validate($form, &$form_state) {
    if ($form_state['triggering_element']['#value'] == 'Generate Report') {
        $form_state['rebuild'] = TRUE;
        return;
    }
}

function store_salesagentreports_products_form($form, &$form_state) {

    drupal_add_css(drupal_get_path('module', 'reports') . '/reports.css');
//    $option = 'completed';
    $option = 'payment_received';
    $getparams = '';
    if (isset($_GET['option'])) {
        $option = $_GET['option'];
        $getparams.='option=' . $option;
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
            if ($getparams != '') {
                $getparams .= '&order_startdate[date]=' . $order_startdate;
            } else {
                $getparams.='order_startdate[date]=' . $order_startdate;
            }
        }
    }
    if (isset($_GET['order_enddate']['date'])) {
        $order_enddate = '';
        $order_endtime = '';
        if ($_GET['order_enddate']['date'] != '') {
            $order_enddate = $_GET['order_enddate']['date'];
            $order_endtime = strtotime($order_enddate . " 23:59:59");
            if ($getparams != '') {
                $getparams.='&order_enddate[date]=' . $order_enddate;
            } else {
                $getparams.='order_enddate[date]=' . $order_enddate;
            }
        }
    }
    $orderstatusoptions = uc_order_status_options_list();

    global $user;
    if (in_array('staff admin', $user->roles)) {
//        $orderstatusoptions = array('completed' => 'Completed', 'payment_received' => 'Payment received');
        $orderstatusoptions = array('payment_received' => 'Payment received', 'pending' => 'Pending');
    }
    if ($getparams != '') {
        $getparams = '?' . $getparams;
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
        '#suffix' => '</div></div></div>',
    );

    $output = '';
    $mainoutput = '';

    $mainheader = array(
        array(
            'data' => 'Title',
        ),
        array(
            'data' => 'Orders',
        ),
        array(
            'data' => 'Tickets',
        ),
        array(
            'data' => 'Total',
        )
    );
    $header = array(
        array(
            'data' => 'Id',
            'field' => 'uid'
        ),
        array(
            'data' => 'Name',
            'field' => 'delivery_last_name'
        ),
        array(
            'data' => 'Email',
            'field' => 'primary_email'
        ),
        array(
            'data' => 'Phone',
//            'field' => 'delivery_phone'
        ),
        array(
            'data' => 'Orders',
//            'field' => 'orders'
        ),
        array(
            'data' => 'Tickets',
//            'field' => 'products'
        ),
        array(
            'data' => 'Total Amount',
//            'field' => 'total'
        ),
        array(
            'data' => 'Action',
        ),
    );
    if (isset($_GET ['sort']) && isset($_GET ['order'])) {
        if ($_GET ['sort'] == 'asc')
            $sort = 'ASC';
        else
            $sort = 'DESC';

        switch ($_GET ['order']) {
            case 'Id' :
                $order = 'uid';
                break;
//            case 'Total Amount' :
//                $order = 'total';
//                break;
            case 'Name' :
                $order = 'last_name';
            case 'Email' :
                $order = 'mail';
//            case 'Phone' :
//                $order = 'delivery_phone';
//            case 'Orders' :
//                $order = 'orders';
//            case 'Products' :
//                $order = 'products';
                break;
        }
    } else {
        $sort = 'ASC';
        $order = 'uid';
    }

    $mainquery = db_select("uc_orders", "c");
    $mainquery->Join('reservation', 'r', 'c.order_id = r.orderid');
    if ($order_starttime != '' && $order_endtime != '') {
        if ($order_starttime < $order_endtime != '') {
            $order_enddate = $order_enddate . " 23:59:59";
            $order_startdate = $order_startdate . " 00:00:00";
//        $query->where("c.created >= $order_starttime and c.created <= $order_endtime");
            $mainquery->where("c.created >= UNIX_TIMESTAMP('$order_startdate') and c.created <= UNIX_TIMESTAMP('$order_enddate')");
        }
    }
    $mainquery->addExpression('COUNT(order_id)', 'orders');
    $mainquery->addExpression('SUM(order_total)', 'total');
    $mainquery->addExpression('GROUP_CONCAT(order_id)', 'orderslist');
    $mainquery->addExpression('SUM(product_count)', 'products');
    if ($option != '') {
        $mainquery->condition('order_status', $option, '=');
    }
    $mainquery->condition('r.bookedby', 'agent', '=');
    $mainresult = $mainquery->execute()->fetchAll();

    $mainrows = array();
    if (count($mainresult) > 0) {
        for ($i = 0; $i < count($mainresult); $i ++) {
            if(isset($mainresult[$i]->orderslist) && $mainresult[$i]->orderslist !=''){
                $tickets = getticketsbyorders($mainresult[$i]->orderslist);
            }else{
                $tickets = 0;
            }
            
            $mainrows [] = array(
                'Total By Agents',
                $mainresult [$i]->orders,
                $tickets,
                uc_currency_format($mainresult [$i]->total),
            );
        }
    }
    $mainoutput .= '<div class="">';
    $mainoutput .= '<div class="">';

    $mainoutput .= '</div>';
    $mainoutput .= theme_table(array(
                'header' => $mainheader,
                'rows' => $mainrows,
                'attributes' => array(
                    'class' => array()
                ),
                'sticky' => true,
                'caption' => '',
                'colgroups' => array(),
                'empty' => t("No Records!")
            )) . theme('pager');

    $mainoutput .= '</div>';


    $query = db_select("uc_orders", "c");
    $query->Join('reservation', 'r', 'c.order_id = r.orderid');
    $query->Join('user_accounts', 'a', 'r.uid = a.uid');
    $query->Join('users', 'u', 'u.uid = a.uid');
    if ($order_starttime != '' && $order_endtime != '') {
        if ($order_starttime < $order_endtime != '') {
            $order_enddate = $order_enddate . " 23:59:59";
            $order_startdate = $order_startdate . " 00:00:00";
//        $query->where("c.created >= $order_starttime and c.created <= $order_endtime");
            $query->where("c.created >= UNIX_TIMESTAMP('$order_startdate') and c.created <= UNIX_TIMESTAMP('$order_enddate')");
        } else {
            drupal_set_message("End date must be Greater than start date", 'warning');
        }
    }
    
    $query->fields('c', array(
//        'order_id',
        'uid',
//        'order_total',
//        'delivery_first_name',
//        'delivery_last_name',
//        'primary_email',
//        'created',
//        'delivery_phone',
//        'order_status',
    ));
    $query->fields('a', array(
        'first_name',
        'last_name',
        'phone'
    ));
    $query->fields('u', array(
        'mail',
    ));
    $query->addExpression('GROUP_CONCAT(order_id)', 'orderslist');
    $query->addExpression('COUNT(order_id)', 'orders');
    $query->addExpression('SUM(order_total)', 'total');
    $query->addExpression('r.uid', 'agentid');
    $query->addExpression('SUM(product_count)', 'products');
//    $query->addExpression("(SELECT COUNT(DISTINCT(order_id)) FROM {uc_orders} o WHERE o.uid = u.uid AND o.order_status = $option");
    $query->groupBy('r.uid');
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
    $query->orderBy($order, $sort);
    $query = $query->extend('TableSort')->extend('PagerDefault')->limit(10);
    $result = $query->execute()->fetchAll();

    $rows = array();
    if (count($result) > 0) {
        for ($i = 0; $i < count($result); $i ++) {
            $detailsview = '<a href="' . base_path() . 'salesagent/' . $result [$i]->agentid . '/orders' . $getparams . '"><input type="button" class="button" value="Detail View"></a>';
            $tickets=getticketsbyorders($result[$i]->orderslist);
            $rows [] = array(
                $result [$i]->agentid,
                $result [$i]->last_name . ' ' . $result [$i]->first_name,
                $result [$i]->mail,
                $result [$i]->phone,
                $result [$i]->orders,
                $tickets,
                uc_currency_format($result [$i]->total),
                $detailsview,
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
        '#markup' => $mainoutput . $output
    );
    return $form;
}

/*  Orders list view which are booked by sales Agent starts here     */

function store_salesagent_orders($agentid) {
    return drupal_render(drupal_get_form('store_salesagent_orders_form', $agentid));
}

function store_salesagent_orders_form_validate($form, &$form_state) {
    if ($form_state['triggering_element']['#value'] == 'Go') {
        $form_state['rebuild'] = TRUE;
        return;
    }
}

function store_salesagent_orders_form($form, &$form_state, $agentid) {

    drupal_add_css(drupal_get_path('module', 'reports') . '/reports.css');
//    $option = 'completed';
    $option = 'payment_received';
    if (isset($_GET["option"])) {
        $option = $_GET["option"];
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
//    if (isset($form_state['values']['option'])) {
//        $option = $form_state['values']['option'];
//    }
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
        '#value' => 'Go',
        '#prefix' => '<div class="pull-left" style="margin-top:14px;">',
        '#suffix' => '</div>',
    );
    $form['export_btn'] = array(
        '#name' => 'export_btn',
        '#type' => 'submit',
        '#value' => t('Export'),
        '#submit' => array('salesagent_reports_export'),
        '#prefix' => '<div class="pull-left" style="margin:14px 0px 0px 6px;">',
        '#suffix' => '</div></div></div>',
    );


    if (isset($_GET['export_btn'])) {
        $_GET['agentid'] = $agentid;
        salesagent_reports_export($_GET);
    }

    $output = '';

    $header = array(
        array(
            'data' => 'Order Id',
            'field' => 'order_id'
        ),
        array(
            'data' => 'Customer',
        ),
        array(
            'data' => 'Total Amount',
            'field' => 'order_total'
        ),
        array(
            'data' => 'Purchase Date',
            'field' => 'created'
        ),
        array(
            'data' => 'Status',
            'field' => 'order_status'
        ),
        array(
            'data' => 'Action'
        ),
    );
    if (isset($_GET ['sort']) && isset($_GET ['order'])) {
        if ($_GET ['sort'] == 'asc')
            $sort = 'ASC';
        else
            $sort = 'DESC';

        switch ($_GET ['order']) {
            case 'Order Id' :
                $order = 'order_id';
                break;
            case 'Total Amount' :
                $order = 'order_total';
                break;
            case 'Purchase Date' :
                $order = 'created';
            case 'Status' :
                $order = 'order_status';
                break;
        }
    } else {
        $sort = 'ASC';
        $order = 'order_id';
    }

    $query = db_select("uc_orders", "c");
    $query->Join('reservation', 'r', 'c.order_id = r.orderid');
    $query->fields('c', array(
        'order_id',
        'order_total',
        'delivery_first_name',
        'delivery_last_name',
        'primary_email',
        'created',
        'order_status',
    ));
    if ($option != '') {
        $query->condition('order_status', $option, '=');
    }
    if ($order_starttime != '' && $order_endtime != '') {
        if ($order_starttime < $order_endtime != '') {
            $order_enddate = $order_enddate . " 23:59:59";
            $order_startdate = $order_startdate . " 00:00:00";
//        $query->where("c.created >= $order_starttime and c.created <= $order_endtime");
            $query->where("c.created >= UNIX_TIMESTAMP('$order_startdate') and c.created <= UNIX_TIMESTAMP('$order_enddate')");
        }
    }
    $query->condition('r.uid', $agentid, '=');
    $query->orderBy($order, $sort);
    $query = $query->extend('TableSort')->extend('PagerDefault')->limit(10);
    $result = $query->execute()->fetchAll();
//    var_dump($form_state["input"]["option"]);
//    exit();
    global $base_url;
    $rows = array();
    if (count($result) > 0) {
        for ($i = 0; $i < count($result); $i ++) {
            $orderstatus = str_replace('_', ' ', $result [$i]->order_status);
            $rows [] = array(
                $result [$i]->order_id,
                $result [$i]->delivery_last_name . ' ' . $result [$i]->delivery_first_name,
                uc_currency_format($result [$i]->order_total),
                date('m-d-y', $result [$i]->created),
                ucwords($orderstatus),
                '<a href="' . base_path() . 'customer/' . $result [$i]->order_id . '/detailview' . '"><input type="button" class="button" value="Detail View"></a>',
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

/*  Orders list view which are booked by sales Agent ends here     */

function salesagent_reports_export($params) {
    $option = 'payment_received';
    if (isset($params["option"])) {
        $option = $params["option"];
    }
    $agentid = isset($params['agentid']) ? $params['agentid'] : '';
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
        0 => 'Order Id',
        1 => 'Customer',
        2 => 'Total Amount',
        3 => 'Purchase Date',
        4 => 'Status'
    );

    $query = db_select("uc_orders", "c");
    $query->Join('reservation', 'r', 'c.order_id = r.orderid');
    $query->fields('c', array(
        'order_id',
        'order_total',
        'delivery_first_name',
        'delivery_last_name',
        'primary_email',
        'created',
        'order_status',
    ));
    if ($option != '') {
        $query->condition('order_status', $option, '=');
    }
    if ($order_starttime != '' && $order_endtime != '') {
        if ($order_starttime < $order_endtime != '') {
            $order_enddate = $order_enddate . " 23:59:59";
            $order_startdate = $order_startdate . " 00:00:00";
//        $query->where("c.created >= $order_starttime and c.created <= $order_endtime");
            $query->where("c.created >= UNIX_TIMESTAMP('$order_startdate') and c.created <= UNIX_TIMESTAMP('$order_enddate')");
        }
    }
    $query->condition('r.uid', $agentid, '=');
//    $query->orderBy($order, $sort);
    $result = $query->execute()->fetchAll();
    global $base_url;
    $rows = array();
    if (count($result) > 0) {
        for ($i = 0; $i < count($result); $i ++) {
            $orderstatus = str_replace('_', ' ', $result [$i]->order_status);
            $rows [] = array(
                $result [$i]->order_id,
                $result [$i]->delivery_last_name . ' ' . $result [$i]->delivery_first_name,
                uc_currency_format($result [$i]->order_total),
                date('m-d-y', $result [$i]->created),
                ucwords($orderstatus)
            );
        }
    }


    excel_export_data('Sales Agent Orders', $header, $rows);
}
