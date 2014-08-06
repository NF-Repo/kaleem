<?php

/**
 * Displays the product reports.
 */
function store_reports_products() {
    return drupal_render(drupal_get_form('store_reports_products_form'));
}

function store_reports_products_form_validate($form, &$form_state) {
    if ($form_state['triggering_element']['#value'] == 'Go') {
        $form_state['rebuild'] = TRUE;
        return;
    }
}

function store_reports_products_form($form, &$form_state) {

    drupal_add_css(drupal_get_path('module', 'reports') . '/reports.css');
//    $option = 'completed';
    $option = 'payment_received';
    $firstname = isset($_GET['firstname']) ? $_GET['firstname'] : '';
    $email = isset($_GET['email']) ? $_GET['email'] : '';
    $phone = isset($_GET['phone']) ? $_GET['phone'] : '';
    $firstname = trim($firstname);
    $email = trim($email);
    $phone = trim($phone);
    $getparams = '';

    if (isset($_GET['option'])) {
        $option = $_GET['option'];
        $getparams.='option=' . $option;
    }
    if ($getparams != '') {
        $getparams = '?' . $getparams;
    }
    $orderstatusoptions = uc_order_status_options_list();
    global $user;
    if (in_array('staff admin', $user->roles)) {
//        $orderstatusoptions = array('completed' => 'Completed', 'payment_received' => 'Payment received');
        $orderstatusoptions = array('payment_received' => 'Payment received', 'pending' => 'Pending');
    }
    $form['#method'] = 'get';
    $form['firstname'] = array(
        '#id' => 'firstname',
        '#type' => 'textfield',
        '#title' => t('Name'),
        '#description' => '',
        '#default_value' => isset($firstname) ? $firstname : '',
        '#prefix' => '<div class="row-fluid"><div class="span12"><div class="pull-left" style="margin-right:10px;">',
        '#suffix' => '</div>',
        '#attributes' => array('class' => array('input-medium')),
//        '#attributes' => array('placeholder' => array('Search')),
    );
    $form['email'] = array(
        '#id' => 'email',
        '#type' => 'textfield',
        '#title' => t('Email'),
        '#description' => '',
        '#default_value' => isset($email) ? $email : '',
        '#prefix' => '<div class="pull-left" style="margin-right:10px;">',
        '#suffix' => '</div>',
        '#attributes' => array('class' => array('input-medium')),
//        '#attributes' => array('placeholder' => array('Search')),
    );
    $form['phone'] = array(
        '#id' => 'phone',
        '#type' => 'textfield',
        '#title' => t('Phone'),
        '#description' => '',
        '#default_value' => isset($phone) ? $phone : '',
        '#prefix' => '<div class="pull-left" style="margin-right:10px;">',
        '#suffix' => '</div>',
        '#attributes' => array('class' => array('input-medium')),
//        '#attributes' => array('placeholder' => array('Search')),
    );
//    $form['option'] = array(
//        '#type' => 'select',
//        '#options' => $orderstatusoptions,
//        '#default_value' => $option,
//        '#prefix' => '<div class="row-fluid"><div class="span2">',
//        '#suffix' => '</div>',
//        '#attributes' => array('class' => array('input-medium')),
//    );

    $form['btn'] = array(
        '#type' => 'submit',
        '#value' => 'Go',
        '#prefix' => '<div class="pull-left" style="margin-top:28px;margin-right:6px;">',
        '#suffix' => '</div>',
    );
    $form['export_btn'] = array(
        '#name' => 'export_btn',
        '#type' => 'submit',
        '#value' => t('Export'),
        '#submit' => array('customer_reports_export'),
        '#prefix' => '<div class="pull-left" style="margin:28px 0px 0px 0px;">',
        '#suffix' => '</div></div></div>',
    );


    if (isset($_GET['export_btn'])) {
        customer_reports_export($_GET);
    }

    $output = '';

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
            'data' => 'Products',
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
                $order = 'delivery_last_name';
            case 'Email' :
                $order = 'primary_email';
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

    $query = db_select("uc_orders", "c");
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
    $query->addExpression('COUNT(order_id)', 'orders');
    $query->addExpression('SUM(order_total)', 'total');
    $query->addExpression('SUM(product_count)', 'products');
//    $query->addExpression("(SELECT COUNT(DISTINCT(order_id)) FROM {uc_orders} o WHERE o.uid = u.uid AND o.order_status = $option");
    $query->groupBy('c.uid');
//    if ($option != '') {
//        $query->condition('order_status', $option, '=');
//    }
    if ($firstname != '') {

        $db_or = db_or();
        $db_or->condition('delivery_first_name', '%' . db_like($firstname) . '%', 'LIKE');
        $db_or->condition('delivery_last_name', '%' . db_like($firstname) . '%', 'LIKE');
        $query->condition($db_or);
//        $query->condition('delivery_first_name', '%' . db_like($firstname) . '%', 'LIKE');
//        $query->where('delivery_first_name LIKE "%' . $firstname . '%" or delivery_last_name LIKE "%' . $firstname . '%"');
    }
    if ($email != '') {
        $query->condition('primary_email', '%' . db_like($email) . '%', 'LIKE');
    }
    if ($phone != '') {
        $query->condition('delivery_phone', '%' . db_like($phone) . '%', 'LIKE');
    }
//    $query->condition('uid', $customerid, '=');
    $query->orderBy($order, $sort);
    $query = $query->extend('TableSort')->extend('PagerDefault')->limit(10);
    $result = $query->execute()->fetchAll();

    global $base_url;
//    $rows = array();
//     $rows[] = array(t('#Id'), t('Name'), t('Email'), t('Phone'), t('Orders'), t('Products'), t('Total Amount'));
    if (count($result) > 0) {
        for ($i = 0; $i < count($result); $i ++) {
            $detailsview = '<a href="' . base_path() . 'customer/' . $result [$i]->uid . '/orders' . $getparams . '"><input type="button" class="button" value="Detail View"></a>';
            $rows [] = array(
                $result [$i]->uid,
                $result [$i]->delivery_last_name . ' ' . $result [$i]->delivery_first_name,
                $result [$i]->primary_email,
                $result [$i]->delivery_phone,
                $result [$i]->orders,
                $result [$i]->products,
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
        '#markup' => $output
    );
    return $form;
}

function customer_reports_export($params) {
    $option = 'payment_received';
    $firstname = isset($params['firstname']) ? $params['firstname'] : '';
    $email = isset($params['email']) ? $params['email'] : '';
    $phone = isset($params['phone']) ? $params['phone'] : '';
    $firstname = trim($firstname);
    $email = trim($email);
    $phone = trim($phone);
    $getparams = '';

    if (isset($params['option'])) {
        $option = $params['option'];
        $getparams.='option=' . $option;
    }
    if ($getparams != '') {
        $getparams = '?' . $getparams;
    }

    $header = array(
        0 => 'Id',
        1 => 'Name',
        2 => 'Email',
        3 => 'Phone',
        4 => 'Orders',
        5 => 'Products',
        6 => 'Total Amount',
    );
    $query = db_select("uc_orders", "c");
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
    $query->addExpression('COUNT(order_id)', 'orders');
    $query->addExpression('SUM(order_total)', 'total');
    $query->addExpression('SUM(product_count)', 'products');
//    $query->addExpression("(SELECT COUNT(DISTINCT(order_id)) FROM {uc_orders} o WHERE o.uid = u.uid AND o.order_status = $option");
    $query->groupBy('c.uid');
//    if ($option != '') {
//        $query->condition('order_status', $option, '=');
//    }
    if ($firstname != '') {

        $db_or = db_or();
        $db_or->condition('delivery_first_name', '%' . db_like($firstname) . '%', 'LIKE');
        $db_or->condition('delivery_last_name', '%' . db_like($firstname) . '%', 'LIKE');
        $query->condition($db_or);
//        $query->condition('delivery_first_name', '%' . db_like($firstname) . '%', 'LIKE');
//        $query->where('delivery_first_name LIKE "%' . $firstname . '%" or delivery_last_name LIKE "%' . $firstname . '%"');
    }
    if ($email != '') {
        $query->condition('primary_email', '%' . db_like($email) . '%', 'LIKE');
    }
    if ($phone != '') {
        $query->condition('delivery_phone', '%' . db_like($phone) . '%', 'LIKE');
    }
//    $query->orderBy($order, $sort);
    $result = $query->execute()->fetchAll();

    global $base_url;
    $rows = array();
//     $rows[] = array(t('#Id'), t('Name'), t('Email'), t('Phone'), t('Orders'), t('Products'), t('Total Amount'));
    if (count($result) > 0) {
        for ($i = 0; $i < count($result); $i ++) {
            $rows [] = array(
                $result [$i]->uid,
                $result [$i]->delivery_last_name . ' ' . $result [$i]->delivery_first_name,
                $result [$i]->primary_email,
                $result [$i]->delivery_phone,
                $result [$i]->orders,
                $result [$i]->products,
                uc_currency_format($result [$i]->total)
            );
        }
    }
    excel_export_data('Customer orders Reports', $header, $rows);
}

/*
 * Displays the product reports.
 */

function store_orders_list() {
    return drupal_get_form('store_orders_list_form');
}

function store_orders_list_form_validate($form, &$form_state) {
    if ($form_state['triggering_element']['#value'] == 'Go') {
        $form_state['rebuild'] = TRUE;
        return;
    }
}

function store_orders_list_form($form, &$form_state) {

    drupal_add_css(drupal_get_path('module', 'reports') . '/reports.css');
//    $option = 'completed';
    $option = 'payment_received';
    $firstname = isset($_GET['firstname']) ? $_GET['firstname'] : '';
    $email = isset($_GET['email']) ? $_GET['email'] : '';
    $phone = isset($_GET['phone']) ? $_GET['phone'] : '';
    $firstname = trim($firstname);
    $email = trim($email);
    $phone = trim($phone);
    $getparams = '';

    if (isset($_GET['option'])) {
        $option = $_GET['option'];
        $getparams.='option=' . $option;
    }
    if ($getparams != '') {
        $getparams = '?' . $getparams;
    }
    $orderstatusoptions = uc_order_status_options_list();
    global $user;
    if (in_array('staff admin', $user->roles)) {
//        $orderstatusoptions = array('completed' => 'Completed', 'payment_received' => 'Payment received');
        $orderstatusoptions = array('payment_received' => 'Payment received', 'pending' => 'Pending');
    }
    $form['#method'] = 'get';
    $form['firstname'] = array(
        '#id' => 'firstname',
        '#type' => 'textfield',
        '#title' => t('Name'),
        '#description' => '',
        '#default_value' => isset($firstname) ? $firstname : '',
        '#prefix' => '<div class="row-fluid"><div class="span12"><div class="pull-left" style="margin-right:10px;">',
        '#suffix' => '</div>',
        '#attributes' => array('class' => array('input-medium')),
//        '#attributes' => array('placeholder' => array('Search')),
    );
    $form['email'] = array(
        '#id' => 'email',
        '#type' => 'textfield',
        '#title' => t('Email'),
        '#description' => '',
        '#default_value' => isset($email) ? $email : '',
        '#prefix' => '<div class="pull-left" style="margin-right:10px;">',
        '#suffix' => '</div>',
        '#attributes' => array('class' => array('input-medium')),
//        '#attributes' => array('placeholder' => array('Search')),
    );
    $form['phone'] = array(
        '#id' => 'phone',
        '#type' => 'textfield',
        '#title' => t('Phone'),
        '#description' => '',
        '#default_value' => isset($phone) ? $phone : '',
        '#prefix' => '<div class="pull-left" style="margin-right:10px;">',
        '#suffix' => '</div>',
        '#attributes' => array('class' => array('input-medium')),
//        '#attributes' => array('placeholder' => array('Search')),
    );
//    $form['option'] = array(
//        '#type' => 'select',
//        '#options' => $orderstatusoptions,
//        '#default_value' => $option,
//        '#prefix' => '<div class="row-fluid"><div class="span2">',
//        '#suffix' => '</div>',
//        '#attributes' => array('class' => array('input-medium')),
//    );

    $form['btn'] = array(
        '#type' => 'submit',
        '#value' => 'Go',
        '#prefix' => '<div class="pull-left" style="margin-top:3.2%;margin-right:6px;">',
        '#suffix' => '</div>',
    );
    $form['export_btn'] = array(
        '#name' => 'export_btn',
        '#type' => 'submit',
        '#value' => t('Export'),
        '#submit' => array('store_orders_list_export'),
        '#prefix' => '<div class="pull-left" style="margin:3.2% 0px 0px 0px;">',
        '#suffix' => '</div></div></div>',
    );


    if (isset($_GET['export_btn'])) {
        store_orders_list_export($_GET);
    }

    $output = '';

    $header = array(
        array(
            'data' => 'OrderId',
            'field' => 'uid'
        ),
    		array(
    				'data' => 'Payment Method',
    				//'field' => 'uid'
    		),
        array(
            'data' => 'Customer',
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
            'data' => 'Purchase date',
//            'field' => 'orders'
        ),
        array(
            'data' => 'Total Amount',
//            'field' => 'total'
        ),
        array(
            'data' => 'Status',
//            'field' => 'products'
        ),
        array(
            'data' => 'Action',
        ),
    );
    $sort = 'DESC';
    if (isset($_GET ['sort']) && isset($_GET ['order'])) {
        if ($_GET ['sort'] == 'asc')
            $sort = 'ASC';
        else
            $sort = 'DESC';

        switch ($_GET ['order']) {
            case 'OrderId' :
                $order = 'order_id';
                break;
//            case 'Total Amount' :
//                $order = 'total';
//                break;
            case 'Customer' :
                $order = 'delivery_last_name';
            case 'Email' :
                $order = 'primary_email';
//            case 'Phone' :
//                $order = 'delivery_phone';
//            case 'Orders' :
//                $order = 'orders';
//            case 'Products' :
//                $order = 'products';
                break;
        }
    } else {
        $sort = 'DESC';
        $order = 'order_id';
    }

    $query = db_select("uc_orders", "c");
    $query->innerJoin('booking_cart', 'b', 'c.order_id = b.orderId');
    $query->distinct();
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
    	'payment_method',
    ));
    if ($firstname != '') {

        $db_or = db_or();
        $db_or->condition('delivery_first_name', '%' . db_like($firstname) . '%', 'LIKE');
        $db_or->condition('delivery_last_name', '%' . db_like($firstname) . '%', 'LIKE');
        $query->condition($db_or);
    }
    if ($email != '') {
        $query->condition('primary_email', '%' . db_like($email) . '%', 'LIKE');
    }
    if ($phone != '') {
        $query->condition('delivery_phone', '%' . db_like($phone) . '%', 'LIKE');
    }

    $db_or1 = db_or();
    $db_or1->condition('order_status', 'payment_received', '=');
    $db_or1->condition('order_status', 'pending', '=');
    $query->condition($db_or1);

    $query->orderBy($order, $sort);
    $query = $query->extend('TableSort')->extend('PagerDefault')->limit(25);
    $result = $query->execute()->fetchAll();

    global $base_url;
    $detailsview='';
//    $rows = array();
//     $rows[] = array(t('#Id'), t('Name'), t('Email'), t('Phone'), t('Orders'), t('Products'), t('Total Amount'));
    if (count($result) > 0) {
        for ($i = 0; $i < count($result); $i ++) {
         	//$detailsview = '<a target="_blank" href="' . base_path() . 'booking-order/' . $result [$i]->order_id . '" style="margin-left:10px;">View</a>';
         	
         	if($result [$i]->payment_method=="credit"){
         		//$detailsview .= '<a href="' . base_path() . 'staffadmin/order/' . $result [$i]->order_id . '/info" style="margin-left:10px;">|  Edit</a>';
         	}
            //  $detailsview .= '<a href="' . base_path() . 'admin/store/orders/' . $result [$i]->order_id . '/delete" style="margin-left:10px;">Delete</a>';
            $orderstatus = str_replace('_', ' ', $result [$i]->order_status);
            $paymentMethod="";
            if($result [$i]->payment_method=="free_order"){
            	$paymentMethod="No Charge Order";
            }
            
            if($result [$i]->payment_method=="credit"){
            	$paymentMethod="Credit Card";
            }
            
            $rows [] = array(
                $result [$i]->order_id,
            	$paymentMethod,
                $result [$i]->delivery_last_name . ' ' . $result [$i]->delivery_first_name,
                $result [$i]->primary_email,
                $result [$i]->delivery_phone,
                date('m-d-Y', $result [$i]->created),
                uc_currency_format($result [$i]->order_total),
                ucwords($orderstatus),
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
        '#markup' => $output
    );
    return $form;
}

function store_orders_list_export($params) {
    $option = 'payment_received';
    $firstname = isset($params['firstname']) ? $params['firstname'] : '';
    $email = isset($params['email']) ? $params['email'] : '';
    $phone = isset($params['phone']) ? $params['phone'] : '';
    $firstname = trim($firstname);
    $email = trim($email);
    $phone = trim($phone);
    $getparams = '';

    if (isset($params['option'])) {
        $option = $params['option'];
        $getparams.='option=' . $option;
    }
    if ($getparams != '') {
        $getparams = '?' . $getparams;
    }

    $header = array(
        0 => 'OrderId',
    	1=>'Payment Method',
        2 => 'Customer',
        3 => 'Email',
        4 => 'Phone',
        5 => 'Purchase date',
        6 => 'Total Amount',
        7 => 'Status',
    );
    $query = db_select("uc_orders", "c");
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
    	'payment_method'
    ));
//    if ($firstname != '') {
//
//        $db_or = db_or();
//        $db_or->condition('delivery_first_name', '%' . db_like($firstname) . '%', 'LIKE');
//        $db_or->condition('delivery_last_name', '%' . db_like($firstname) . '%', 'LIKE');
//        $query->condition($db_or);
//    }
//    if ($email != '') {
//        $query->condition('primary_email', '%' . db_like($email) . '%', 'LIKE');
//    }
//    if ($phone != '') {
//        $query->condition('delivery_phone', '%' . db_like($phone) . '%', 'LIKE');
//    }

    $db_or1 = db_or();
    $db_or1->condition('order_status', 'payment_received', '=');
    $db_or1->condition('order_status', 'pending', '=');
    $query->condition($db_or1);
    $result = $query->execute()->fetchAll();

    global $base_url;
    $rows = array();
    if (count($result) > 0) {
        for ($i = 0; $i < count($result); $i ++) {
            $orderstatus = str_replace('_', ' ', $result [$i]->order_status);
            
            $paymentMethod="";
            if($result [$i]->payment_method=="free_order"){
            	$paymentMethod="No Charge Order";
            }
            
            if($result [$i]->payment_method=="credit"){
            	$paymentMethod="Credit Card";
            }
            
            
            $rows [] = array(
                $result [$i]->order_id,
            	$paymentMethod,
                $result [$i]->delivery_last_name . ' ' . $result [$i]->delivery_first_name,
                $result [$i]->primary_email,
                $result [$i]->delivery_phone,
                date('m-d-Y', $result [$i]->created),
                uc_currency_format($result [$i]->order_total),
                ucwords($orderstatus),
            );
        }
    }
    excel_export_data('Customer Orders List', $header, $rows);
}
