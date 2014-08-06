<?php

/**
 * Output coupon report.
 */
function store_coupon_reports() {
    return drupal_render(drupal_get_form('store_coupon_reports_form'));
}

function store_coupon_reports_form_validate($form, &$form_state) {
    if ($form_state['triggering_element']['#value'] == 'Display report') {
        $form_state['rebuild'] = TRUE;
        return;
    }
}

/**
 * Coupon report options form.
 */
function store_coupon_reports_form($form, &$form_state) {

//if($form_state['clicked_button']['#value'] == $form_state['values']['submit_one'])    //if button 1      is clicked
//     $form_state['redirect'] = 'mypath/page_one';   //redirect to whatever page you want
// else if($form_state['clicked_button']['#value'] == $form_state['values']['submit_two'])  /if button      2 is clicked
//    $form_state['redirect'] = 'mypath/page_two';
// }

    if (isset($_GET['export_btn'])) {
        couponreports_export($_GET);
    }

    drupal_add_css(drupal_get_path('module', 'reports') . '/reports.css');
//    $option = 'completed';
    $option = 'payment_received';
    $getparams = '';
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


    $form['#method'] = 'get';
    $form['order_startdate'] = array(
        '#id' => 'order_start_date',
        '#type' => 'date_popup',
        '#date_format' => 'Y-m-d',
        '#title' => t(''),
        '#description' => '',
        '#attributes' => array('class' => array('dateFieldStyle dateFieldMedium')),
        '#default_value' => isset($order_startdate) ? $order_startdate : '',
//        '#prefix' => '<div class="row-fluid"><div class="span3">',
        '#prefix' => '<div class="row-fluid"><div class="span12"><div class="pull-left">',
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
        '#prefix' => '<div class="pull-left">',
//        '#suffix' => '</div></div>',
        '#suffix' => '</div>',
        '#attributes' => array('placeholder' => array('End Date')),
    );
    $orderstatusoptions = uc_order_status_options_list();

    if ($getparams != '') {
        $getparams = '?' . $getparams;
    }
    global $user;
    if (in_array('staff admin', $user->roles)) {
//        $orderstatusoptions = array('completed' => 'Completed', 'payment_received' => 'Payment received');
        $orderstatusoptions = array('payment_received' => 'Payment received', 'pending' => 'Pending');
    }

    $form['btn'] = array(
        '#type' => 'submit',
        '#value' => 'Generate Report',
        '#prefix' => '<div class="pull-left" style="margin-left: 0px;">',
        '#suffix' => '</div>',
    );
    $form['export_btn'] = array(
        '#name' => 'export_btn',
        '#type' => 'submit',
        '#value' => t('Export'),
        '#submit' => array('couponreports_export'),
        '#prefix' => '<div class="pull-left" style="margin-left: 0px;">',
        '#suffix' => '</div></div></div>',
    );
    $output = '';

    $header = array(
        array(
            'data' => 'Id',
            'field' => 'uid'
        ),
        array(
            'data' => 'Title',
//            'field' => 'name'
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
            'data' => 'Total Discount',
//            'field' => 'total'
        ),
    );
    if (isset($_GET ['sort']) && isset($_GET ['order'])) {
        if ($_GET ['sort'] == 'asc')
            $sort = 'ASC';
        else
            $sort = 'DESC';

        switch ($_GET ['order']) {
            case 'Id' :
                $order = 'position';
                break;
            case 'Title' :
                $order = 'name';
        }
    } else {
        $sort = 'ASC';
        $order = 'position';
    }

    $query = db_select("uc_order_line_items", "l");
    $query->Join('uc_orders', 'o', 'l.order_id = o.order_id');
    $query->condition('l.type', 'generic', '=');
    $query->condition('l.title', '%' . db_like('Promo Code') . '%', 'LIKE');
    if ($order_starttime != '' && $order_endtime != '') {
        if ($order_starttime < $order_endtime != '') {
            $order_enddate = $order_enddate . " 23:59:59";
            $order_startdate = $order_startdate . " 00:00:00";
            $query->where("o.created >= UNIX_TIMESTAMP('$order_startdate') and o.created <= UNIX_TIMESTAMP('$order_enddate')");
        } else {
            drupal_set_message("End date must be Greater than start date", 'warning');
        }
    }
    $query->fields('o', array(
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
    $query->fields('l', array('title', 'amount'));
    $query->addExpression('@row := @row + 1', 'position');
    $query->addExpression('COUNT(o.order_id)', 'orders');
    $query->addExpression('SUM(o.order_total)', 'total');
    $query->addExpression('SUM(o.product_count)', 'products');
    $query->addExpression('SUM(l.amount)', 'totaldiscount');
    $query->groupBy('l.title');
    if ($option != '') {
        $query->condition('order_status', $option, '=');
    }
    $query->orderBy($order, $sort);
    $query = $query->extend('TableSort')->extend('PagerDefault')->limit(10);

    /*   for creating mysql custom index column for the orders    */
    db_query("set @row=0");

    $result = $query->execute()->fetchAll();

    $rows = array();
    if (count($result) > 0) {
        for ($i = 0; $i < count($result); $i ++) {
            $title = '';
            $title = str_replace('Promo Code', '', $result [$i]->title);
            $title = trim($title);
            $rows [] = array(
                $result [$i]->position,
                $title,
                $result [$i]->orders,
                $result [$i]->products,
                uc_currency_format($result [$i]->total),
                uc_currency_format($result [$i]->totaldiscount),
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

/**
 * Submit handler for uc_coupon_reports form.
 */
//function store_coupon_reports_form_submit($form, &$form_state) {
//  $start = gmmktime(0, 0, 0, $form_state['values']['start']['month'], $form_state['values']['start']['day'], $form_state['values']['start']['year']);
//  $end = gmmktime(23, 59, 59, $form_state['values']['end']['month'], $form_state['values']['end']['day'], $form_state['values']['end']['year']);
//  $statuses = implode(',', array_keys($form_state['values']['status']));
//
//  $form_state['redirect'] = 'store/' . $start . '/' . $end . '/' . $statuses.'/couponreports';
//}


/*     Export functionality for coupon reports     */
function couponreports_export($params) {
   
    $option = 'payment_received';
    $getparams = '';
    $order_startdate = date('Y-m-01');
    $order_enddate = date('Y-m-t');
    $order_starttime = strtotime($order_startdate . " 00:00:00");
    $order_endtime = strtotime($order_enddate . " 23:59:59");
    $header = array(0 => 'Id',1 => 'Title',2 => 'Orders',3 => 'Tickets',
                    4 => 'Total Amount',5 => 'Total Discount');
    if (isset($params ['sort']) && isset($params ['order'])) {
        if ($params ['sort'] == 'asc')
            $sort = 'ASC';
        else
            $sort = 'DESC';

        switch ($params ['order']) {
            case 'Id' :
                $order = 'position';
                break;
            case 'Title' :
                $order = 'name';
        }
    } else {
        $sort = 'ASC';
        $order = 'position';
    }

    $query = db_select("uc_order_line_items", "l");
    $query->Join('uc_orders', 'o', 'l.order_id = o.order_id');
    $query->condition('l.type', 'generic', '=');
    $query->condition('l.title', '%' . db_like('Promo Code') . '%', 'LIKE');
    if ($order_starttime != '' && $order_endtime != '') {
        if ($order_starttime < $order_endtime != '') {
            $order_enddate = $order_enddate . " 23:59:59";
            $order_startdate = $order_startdate . " 00:00:00";
            $query->where("o.created >= UNIX_TIMESTAMP('$order_startdate') and o.created <= UNIX_TIMESTAMP('$order_enddate')");
        } else {
            drupal_set_message("End date must be Greater than start date", 'warning');
        }
    }
    $query->fields('o', array(
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
    $query->fields('l', array('title', 'amount'));
    $query->addExpression('@row := @row + 1', 'position');
    $query->addExpression('COUNT(o.order_id)', 'orders');
    $query->addExpression('SUM(o.order_total)', 'total');
    $query->addExpression('SUM(o.product_count)', 'products');
    $query->addExpression('SUM(l.amount)', 'totaldiscount');
    $query->groupBy('l.title');
    if ($option != '') {
        $query->condition('order_status', $option, '=');
    }
//    $query->orderBy($order, $sort);

    /*   for creating mysql custom index column for the orders    */
    db_query("set @row=0");

    $result = $query->execute()->fetchAll();

    $rows = array();
    if (count($result) > 0) {
        for ($i = 0; $i < count($result); $i ++) {
            $title = '';
            $title = str_replace('Promo Code', '', $result [$i]->title);
            $title = trim($title);
            $rows [] = array(
                $result [$i]->position,
                $title,
                $result [$i]->orders,
                $result [$i]->products,
                uc_currency_format($result [$i]->total),
                uc_currency_format($result [$i]->totaldiscount),
            );
        }
    }
    excel_export_data('Coupon Reports', $header, $rows);
}
