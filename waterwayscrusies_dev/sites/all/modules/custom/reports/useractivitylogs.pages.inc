<?php

/**
 * Displays the product reports.
 */
function user_activitylog_reports() {
    return drupal_render(drupal_get_form('user_activitylog_reports_form'));
}

function user_activitylog_reports_form_validate($form, &$form_state) {
    if ($form_state['triggering_element']['#value'] == 'Generate Report') {
        $form_state['rebuild'] = TRUE;
        return;
    }
}

function user_activitylog_reports_form($form, &$form_state) {
    drupal_add_css(drupal_get_path('module', 'reports') . '/reports.css');

    if (isset($_GET['order_startdate']['date'])&& $_GET['order_startdate']['date']!='') {
        $order_startdate = $_GET['order_startdate']['date'];
        $order_starttime = strtotime($order_startdate . " 00:00:00");
    }
    if (isset($_GET['order_enddate']['date'])&& $_GET['order_enddate']['date']!='') {
        $order_enddate = $_GET['order_enddate']['date'];
        $order_endtime = strtotime($order_enddate . " 23:59:59");
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

    $form['btn'] = array(
        '#type' => 'submit',
        '#value' => 'Generate Report',
        '#prefix' => '<div class="pull-left" style="margin-left: 0px;">',
        '#suffix' => '</div></div></div>',
    );

    $output = '';

    $header = array(
        array(
            'data' => 'Id',
            'field' => 'id'
        ),
        array(
            'data' => 'Activity Time',
//            'field' => 'delivery_last_name'
        ),
        array(
            'data' => 'User IP Address',
//            'field' => 'primary_email'
        ),
        array(
            'data' => 'User Full Name',
            'field' => 'last_name'
        ),
        array(
            'data' => 'Role',
//            'field' => 'orders'
        ),
        array(
            'data' => 'Activity',
//            'field' => 'products'
        ),
        array(
            'data' => 'Status',
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
                $order = 'id';
                break;
//            case 'Total Amount' :
//                $order = 'total';
//                break;
            case 'User Full Name' :
                $order = 'last_name';
//            case 'Email' :
//                $order = 'primary_email';

                break;
        }
    } else {
        $sort = 'DESC';
        $order = 'id';
    }
    $query = db_select("user_logs", "ul");
    $query->leftJoin('users_roles', 'ur', 'ur.uid = ul.uid');
    $query->leftJoin('role', 'r', 'ur.rid = r.rid');
    $query->leftJoin('user_accounts', 'a', 'a.uid = ul.uid');
    if ($order_starttime != '' && $order_endtime != '') {
        if ($order_starttime < $order_endtime != '') {
            $order_enddate = $order_enddate . " 23:59:59";
            $order_startdate = $order_startdate . " 00:00:00";
            $query->where("ul.timestamp >= UNIX_TIMESTAMP('$order_startdate') and ul.timestamp <= UNIX_TIMESTAMP('$order_enddate')");
        } else {
            drupal_set_message("End date must be Greater than start date");
        }
    }
    $query->fields('ul', array(
        'id',
        'uid',
        'type',
        'message',
        'ipaddress',
        'timestamp',
        'status'
    ));
    $query->fields('a', array(
        'first_name',
        'last_name',
        'phone'
    ));
    $query->fields('r', array(
        'name'
    ));
//    $query->addExpression('COUNT(order_id)', 'orders');
   
//    $query->addExpression("(SELECT COUNT(DISTINCT(order_id)) FROM {uc_orders} o WHERE o.uid = u.uid AND o.order_status = $option");
//    $query->groupBy('r.uid');

//    $query->condition('r.bookedby', 'agent', '=');
    $query->orderBy($order, $sort);
    $query = $query->extend('TableSort')->extend('PagerDefault')->limit(10);
    $result = $query->execute()->fetchAll();

    $rows = array();
    if (count($result) > 0) {
        for ($i = 0; $i < count($result); $i ++) {
            $detailsview = '<a href="' . base_path() . 'useractivitylog/' . $result [$i]->id . '/detailview"><input type="button" class="button" value="Details View"></a>';
            $status='';
        if($result [$i]->status == 1){
            $status='<span style="color:green;">Success</span>';
        }else{
            $status='<span style="color:red;">Failed</span>';
        }
            $rows [] = array(
                $result [$i]->id,
                date('D d M Y h:i A',$result [$i]->timestamp),
                $result [$i]->ipaddress,
                $result [$i]->last_name.' '.$result [$i]->first_name,
                $result [$i]->name,
                $result [$i]->type,
                $status,                
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


/*  User activity log detail view   */
function user_activitylog_detailview($logId){
    $query = db_select("user_logs",'ul');
    $query->condition('ul.id', $logId, '=');
    $query->fields('ul', array(
        'data'
    ));
    $result = $query->execute()->fetchAll();
    $userlogdata=  unserialize($result[0]->data);
    $html='<div class="container-fluid">
    <div class="row-fluid"><div class="span12"></div></div>';
    foreach ($userlogdata as $row){
        $html.='<div class="row-fluid"><div class="span12"># '.$row.'</div></div>';
    }
    $html.='</div>';
    return $html;
}