<?php

/**
 * Displays the sales by cruise order reports.
 */
function salesbydayofweek_order_reports() {
    return drupal_render(drupal_get_form('salesbydayofweek_reports_form'));
}

function salesbydayofweek_reports_form_validate($form, &$form_state) {
    if ($form_state['triggering_element']['#value'] == 'Generate Report') {
        $form_state['rebuild'] = TRUE;
        return;
    }
}

function salesbydayofweek_reports_form($form, &$form_state) {
    drupal_add_css(drupal_get_path('module', 'reports') . '/reports.css');
//    $option = 'completed';
    $option = 'payment_received';
//    if (isset($form_state['values']['option'])) {
//        $option = $form_state['values']['option'];
//    }
//    if (isset($form_state['values']['order_startdate'])) {
//        $order_startdate = $form_state['values']['order_startdate'];
//        $order_starttime = strtotime($order_startdate . " 00:00:00");
//    }
//    if (isset($form_state['values']['order_enddate'])) {
//        $order_enddate = $form_state['values']['order_enddate'];
//        $order_endtime = strtotime($order_enddate . " 23:59:59");
//    }
    $orderstatusoptions = uc_order_status_options_list();

    global $user;
    if (in_array('staff admin', $user->roles)) {
//        $orderstatusoptions = array('completed' => 'Completed', 'payment_received' => 'Payment received');
        $orderstatusoptions = array('payment_received' => 'Payment received', 'pending' => 'Pending');
    }
//    $form['option'] = array(
//        '#type' => 'select',
//        '#options' => $orderstatusoptions,
//        '#default_value' => $option,
//        '#prefix' => '<div class="row-fluid"><div class="span12"><div class="pull-left">',
//        '#suffix' => '</div>',
//        '#attributes' => array('class' => array('input-medium')),
//    );
//    $form['order_startdate'] = array(
//        '#id' => 'order_start_date',
//        '#type' => 'date_popup',
//        '#date_format' => 'Y-m-d',
//        '#title' => t(''),
//        '#description' => '',
//        '#attributes' => array('class' => array('dateFieldStyle dateFieldMedium')),
//        '#default_value' => isset($order_startdate) ? $order_startdate : '',
////        '#prefix' => '<div class="row-fluid"><div class="span3">',
//        '#prefix' => '<div class="pull-left">',
//        '#suffix' => '</div>',
//        '#attributes' => array('placeholder' => array('Start Date')),
//    );
//    $form['order_enddate'] = array(
//        '#id' => 'order_end_date',
//        '#type' => 'date_popup',
//        '#date_format' => 'Y-m-d',
//        '#title' => t(''),
//        '#default_value' => isset($order_enddate) ? $order_enddate : '',
//        '#description' => '',
//        '#attributes' => array('class' => array('dateFieldStyle dateFieldMedium')),
//        '#prefix' => '<div class="pull-left">',
//        '#suffix' => '</div>',
//        '#attributes' => array('placeholder' => array('End Date')),
//    );
    $selectedmonth = 0;
    $currentyear = date('Y');
    $selectedyear = $currentyear;
    if (isset($_GET['month']) && $_GET['month'] != '') {
        $selectedmonth = $_GET['month'];
    }
    if (isset($_GET['year']) && $_GET['year'] != '') {
        $selectedyear = $_GET['year'];
    }
    $form['#method'] = 'get';
    $months = array('0' => 'Select Month', '1' => 'January', '2' => 'February', '3' => 'March', '4' => 'April',
        '5' => 'May', '6' => 'June', '7' => 'July', '8' => 'August',
        '9' => 'September', '10' => 'October', '11' => 'November', '12' => 'December');
    $form['month'] = array(
        '#type' => 'select',
        '#options' => $months,
        '#default_value' => $selectedmonth,
        '#prefix' => '<div class="row-fluid"><div class="span12"><div class="pull-left" style="margin-right:13px;">',
        '#suffix' => '</div>',
        '#attributes' => array('class' => array('input-medium')),
    );

    $yearoptions = array();
    for ($z = 0; $z < 50; $z++) {
        $yearoptions[$currentyear] = $currentyear;
        $currentyear--;
    }
    $form['year'] = array(
        '#type' => 'select',
        '#options' => $yearoptions,
        '#default_value' => $selectedyear,
        '#prefix' => '<div class="pull-left" style="margin-right:13px;">',
        '#suffix' => '</div>',
        '#attributes' => array('class' => array('input-medium')),
    );


    $order_startdate = date("$selectedyear-1-01");
    $order_enddate = date("$selectedyear-12-31");
    if ($selectedmonth != 0) {
        $order_startdate = date("$selectedyear-$selectedmonth-01");
        $order_enddate = date("$selectedyear-$selectedmonth-t");
    }



    $order_starttime = strtotime($order_startdate . " 00:00:00");
    $order_endtime = strtotime($order_enddate . " 23:59:59");


    $form['btn'] = array(
        '#type' => 'submit',
        '#value' => 'Generate Report',
        '#prefix' => '<div class="pull-left" style="margin-top:15px;">',
        '#suffix' => '</div>',
    );

    $form['export_btn'] = array(
        '#name' => 'export_btn',
        '#type' => 'submit',
        '#value' => t('Export'),
        '#submit' => array('salesbydayofweek_reports_export'),
        '#prefix' => '<div class="pull-left" style="margin:14px 0px 0px 6px;">',
        '#suffix' => '</div></div></div>',
    );


    if (isset($_GET['export_btn'])) {
        salesbydayofweek_reports_export($_GET);
    }
    $output = '';

    $header = array(
        array(
            'data' => 'Sno',
//            'field' => 'position'
        ),
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
        }
    } else {
        $sort = 'ASC';
        $order = 'position';
    }

//    $query = db_select("uc_orders", "o");
//    $condition = "";
//    if ($option != '') {
//        $condition = "and order_status='$option'";
//    }
//    if ($order_starttime != '' && $order_endtime != '') {
//        if ($order_starttime < $order_endtime != '') {
//            $order_enddate = $order_enddate . " 23:59:59";
//            $order_startdate = $order_startdate . " 00:00:00";
//            $condition.=" and created >= UNIX_TIMESTAMP('$order_startdate') and created <= UNIX_TIMESTAMP('$order_enddate')";
//        } else {
//            drupal_set_message("End date must be Greater than start date");
//        }
//    }
//    $query->addExpression('@dayweekrow := @dayweekrow + 1', 'position');
//    $query->addExpression('@timestamp:=@timestamp+(24*60*60)', 'stamptime');
//    $query->addExpression('DAYOFWEEK(from_unixtime(@timestamp))', 'dayweek');
//    $query->addExpression('DAYNAME(from_unixtime(@timestamp))', 'dayname');
//    $query->addExpression("(select count(order_id) from uc_orders where DAYOFWEEK(from_unixtime(created))=dayweek $condition)", 'orders');
//    $query->addExpression("(select SUM(order_total) from uc_orders where DAYOFWEEK(from_unixtime(created))=dayweek $condition) ", 'total');
//    $query->addExpression("(select SUM(product_count) from uc_orders where DAYOFWEEK(from_unixtime(created))=dayweek $condition) ", 'products');
////    $query->addExpression("(select DISTINCT(DAYNAME(from_unixtime(@timestamp))) from uc_orders where DAYOFWEEK(from_unixtime(created))=dayweek $condition) ", 'dayname');
////    $query->addExpression('DAYOFWEEK(from_unixtime(created))', 'daynum');
////    $query->groupBy('daynum');    
//    $query->where("@dayweekrow<7");
//    $query->orderBy('dayweek', $sort);
//
//    /*   for creating mysql custom index column for the orders    */
//    db_query("set @dayweekrow=0");
//    db_query("set @timestamp=UNIX_TIMESTAMP()");
//    $result = $query->execute()->fetchAll();


    $weeks = array('1' => 'Sunday', '2' => 'Monday', '3' => 'Tuesday', '4' => 'wednesday',
        '5' => 'Thursday', '6' => 'Friday', '7' => 'Saturday');

    $query = db_select("uc_orders", "o");
    $query->addExpression('sum(order_total)', 'total');
    $query->addExpression('count(order_id)', 'orders');
    $query->addExpression('sum(product_count)', 'products');
    $query->addExpression('GROUP_CONCAT(order_id)', 'orderslist');
    $query->addExpression('DAYOFWEEK(from_unixtime(created))', 'daynum');
    $query->orderBy('daynum', 'ASC');
    if ($option != '') {
//        $condition = "and order_status='$option'";
        $query->condition('order_status', $option, '=');
    }
    if ($order_starttime != '' && $order_endtime != '') {
        if ($order_starttime < $order_endtime != '') {
            $order_enddate = $order_enddate . " 23:59:59";
            $order_startdate = $order_startdate . " 00:00:00";
            $condition = "created >= UNIX_TIMESTAMP('$order_startdate') and created <= UNIX_TIMESTAMP('$order_enddate')";
            $query->where($condition);
        } else {
            drupal_set_message("End date must be Greater than start date", 'warning');
        }
    }
    $query->groupBy('daynum');
//    $queryresult = $query->execute();
    $queryresult = $query->execute()->fetchAll();
//    var_dump($queryresult);exit();
    $order_result = array();
    if (count($queryresult) > 0) {
        for ($i = 0; $i < count($queryresult); $i ++) {
            $order_result[$queryresult [$i]->daynum] = $queryresult [$i];
        }
        $result = array();
//        if (count($order_result) > 0) {
        for ($j = 1; $j <= 7; $j ++) {
            if (isset($order_result[$j])) {
                $order_result[$j]->dayname = $weeks[$j];

                $tickets = getticketsbyorders($order_result[$j]->orderslist);
                $order_result[$j]->products = $tickets;
                $result[] = $order_result[$j];
            } else {
//                    if ($selectedmonth == 0) {
                $resobj = new stdClass();
                $resobj->products = '0';
                $resobj->total = '0';
                $resobj->dayname = $weeks[$j];
                $resobj->orders = '0';
                $result[] = $resobj;
//                    }
            }
        }
//        }
    }
    $rows = array();
    if (count($result) > 0) {
        for ($i = 0; $i < count($result); $i ++) {
            $products = 0;
            $totalamount = 0;
            if ($result [$i]->products != '') {
                $products = $result [$i]->products;
            }
            if ($result [$i]->total != '') {
                $totalamount = $result [$i]->total;
            }
            $rows [] = array(
                $i + 1,
                $result [$i]->dayname,
                $result [$i]->orders,
                $products,
                uc_currency_format($totalamount),
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

function salesbydayofweek_reports_export($params) {

    $option = 'payment_received';
    $selectedmonth = 0;
    $currentyear = date('Y');
    $selectedyear = $currentyear;
    if (isset($params['month']) && $params['month'] != '') {
        $selectedmonth = $params['month'];
    }
    if (isset($params['year']) && $params['year'] != '') {
        $selectedyear = $params['year'];
    }


    $order_startdate = date("$selectedyear-1-01");
    $order_enddate = date("$selectedyear-12-31");
    if ($selectedmonth != 0) {
        $order_startdate = date("$selectedyear-$selectedmonth-01");
        $order_enddate = date("$selectedyear-$selectedmonth-t");
    }



    $order_starttime = strtotime($order_startdate . " 00:00:00");
    $order_endtime = strtotime($order_enddate . " 23:59:59");

    $header = array(
        0 => 'Sno',
        1 => 'Title',
        2 => 'Orders',
        3 => 'Tickets',
        4 => 'Total'
    );
    $weeks = array('1' => 'Sunday', '2' => 'Monday', '3' => 'Tuesday', '4' => 'wednesday',
        '5' => 'Thursday', '6' => 'Friday', '7' => 'Saturday');

    $query = db_select("uc_orders", "o");
    $query->addExpression('sum(order_total)', 'total');
    $query->addExpression('count(order_id)', 'orders');
    $query->addExpression('sum(product_count)', 'products');
    $query->addExpression('GROUP_CONCAT(order_id)', 'orderslist');
    $query->addExpression('DAYOFWEEK(from_unixtime(created))', 'daynum');
    $query->orderBy('daynum', 'ASC');
    if ($option != '') {
//        $condition = "and order_status='$option'";
        $query->condition('order_status', $option, '=');
    }
    if ($order_starttime != '' && $order_endtime != '') {
        if ($order_starttime < $order_endtime != '') {
            $order_enddate = $order_enddate . " 23:59:59";
            $order_startdate = $order_startdate . " 00:00:00";
            $condition = "created >= UNIX_TIMESTAMP('$order_startdate') and created <= UNIX_TIMESTAMP('$order_enddate')";
            $query->where($condition);
        } else {
            drupal_set_message("End date must be Greater than start date",'warning');
        }
    }
    $query->groupBy('daynum');
    $queryresult = $query->execute()->fetchAll();

    $order_result = array();
    if (count($queryresult) > 0) {
        for ($i = 0; $i < count($queryresult); $i ++) {
            $order_result[$queryresult [$i]->daynum] = $queryresult [$i];
        }
        $result = array();
//        if (count($order_result) > 0) {
        for ($j = 1; $j <= 7; $j ++) {
            if (isset($order_result[$j])) {
                $order_result[$j]->dayname = $weeks[$j];
                $tickets = getticketsbyorders($order_result[$j]->orderslist);
                $order_result[$j]->products = $tickets;
                $result[] = $order_result[$j];
            } else {
//                    if ($selectedmonth == 0) {
                $resobj = new stdClass();
                $resobj->products = '0';
                $resobj->total = '0';
                $resobj->dayname = $weeks[$j];
                $resobj->orders = '0';
                $result[] = $resobj;
//                    }
            }
        }
//        }
    }
    $rows = array();
    if (count($result) > 0) {
        for ($i = 0; $i < count($result); $i ++) {
            $products = 0;
            $totalamount = 0;
            if ($result [$i]->products != '') {
                $products = $result [$i]->products;
            }
            if ($result [$i]->total != '') {
                $totalamount = $result [$i]->total;
            }
            $rows [] = array(
                $i + 1,
                $result [$i]->dayname,
                $result [$i]->orders,
                $products,
                uc_currency_format($totalamount),
            );
        }
    }

    excel_export_data('Sales By Week', $header, $rows);
}
