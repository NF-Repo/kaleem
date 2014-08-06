<?php

/**
 * Displays the product reports.
 */
function store_customer_orders($customerid) {
    return drupal_render(drupal_get_form('store_customer_orders_form', $customerid));
}

function store_customer_orders_form_validate($form, &$form_state) {
    if ($form_state['triggering_element']['#value'] == 'Go') {
        $form_state['rebuild'] = TRUE;
        return;
    }
}

function store_customer_orders_form($form, &$form_state, $customerid) {

    drupal_add_css(drupal_get_path('module', 'reports') . '/reports.css');
//    $option = 'completed';
    $option = 'payment_received';

    if (isset($_GET['option'])) {
        $option = $_GET['option'];
    }
    $orderstatusoptions = uc_order_status_options_list();

    global $user;
    if (in_array('staff admin', $user->roles)) {
//        $orderstatusoptions = array('completed' => 'Completed', 'payment_received' => 'Payment received');
         $orderstatusoptions = array( 'payment_received' => 'Payment received',  'pending' => 'Pending');
    }
    
//    $form['#method'] = 'get';
//    $form['option'] = array(
//        '#type' => 'select',
//        '#options' => $orderstatusoptions,
//        '#default_value' => $option,
//        '#prefix' => '<div class="row-fluid"><div class="span2">',
//        '#suffix' => '</div>',
//        '#attributes' => array('class' => array('input-medium')),
//    );
//
//    $form['btn'] = array(
//        '#type' => 'submit',
//        '#value' => 'Go',
//        '#prefix' => '<div class="span10" style="margin-left: 0px;">',
//        '#suffix' => '</div></div>',
//    );

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
    $query->condition('uid', $customerid, '=');
    $query->orderBy($order, $sort);
    $query = $query->extend('TableSort')->extend('PagerDefault')->limit(10);
    $result = $query->execute()->fetchAll();

    global $base_url;
    $rows = array();
    if (count($result) > 0) {
        for ($i = 0; $i < count($result); $i ++) {
            $orderstatus=  str_replace('_', ' ', $result [$i]->order_status);
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

/*   Customer Order Detail view @params : orderId   */

function store_customer_order_detailview($orderId) {
    drupal_add_css(drupal_get_path('module', 'reports') . '/reports.css');
    $order = uc_order_load($orderId);
    $reservationicketdata = Reservation::getReservationTicketData($orderId);
    $reservationenhancedata = Reservation::getReservationEnhancementsData($orderId);
    $reservationcommentdata = Reservation::getCompleteReservationData($orderId);

    $serializedata = $reservationcommentdata->serializedata;
    $recievemail = $serializedata->userchoice['usersdata']['waterwayspromotionsAnddiscounts'];
    if ($recievemail == 0) {
        $recievemailstatus = 'No';
    } else {
        $recievemailstatus = 'Yes';
    }
    $eventId = $reservationcommentdata->eventid;
    $eventdata = array();

    try {
        $result = db_select('scheduledevents', 'c')
                ->fields('c')
                ->condition('id', $eventId, '=')
                ->execute();
        foreach ($result as $value) {
            $eventdata[] = $value;
        }
    } catch (Exception $e) {
        
    }

    $tickets = getTicketslist();
    $reservationtickets = array();
    foreach ($tickets as $ticket) {
        $reservationtickets[$ticket->id] = $ticket->title;
    }

    $tickethtml = '';
    foreach ($reservationicketdata as $reservedata) {
        $tickethtml.='<div class="row-fluid">
                    <div class="span6 custompara" align="center">' . $reservationtickets[$reservedata['ticketid']] . ':</div>
                    <div class="span6 custompara" align="center">' . uc_currency_format($reservedata['price']) . '</div>
                </div>';
    }
    $enhancehtml = '';
    foreach ($reservationenhancedata as $enhancedata) {
        $enhancehtml.='<div class="row-fluid">
                    <div class="span6 custompara" align="center">' . $enhancedata['name'] . ':</div>
                    <div class="span6 custompara" align="center">' . $enhancedata['qty'] . '</div>
                </div>';
    }
    $name = $order->delivery_first_name . ' ' . $order->delivery_last_name;
    $zone = uc_zone_get_by_id($order->delivery_zone);
    $html = '';
    $html.='<div class="container-fluid">
    <div class="row-fluid">
        <div class="span6 customerdetails">
            <h3>Customer Details</h3>
            <div class="row-fluid">
                <div class="span3 customheading"><b>Name:</b></div>
                <div class="span8 custompara">' . $name . '</div>
            </div>                        
            <div class="row-fluid">
                <div class="span3 customheading"><b>Email:</b></div>
                <div class="span8 custompara">' . $order->primary_email . '</div>
            </div>
            <div class="row-fluid">
                <div class="span3 customheading"><b>Phone:</b></div>
                <div class="span8 custompara">' . $order->delivery_phone . '</div>
            </div>
            <div class="row-fluid">
                <div class="span3 customheading"><b>Address:</b></div>
                <div class="span8 custompara">' . $order->delivery_street1 . '<br/>' . $order->delivery_city . ' ' . $zone . ' ' . $order->delivery_postal_code . '</div>
            </div>

        </div>
        <div class="span6 specialmain">
            <div class="row-fluid">
                <div class="span12 specialdetails">
                    <h3>Special Dates</h3>
                    <div class="row-fluid">
                        <div class="span5 customheading"><b>Birth Date:</b></div>
                        <div class="span7 custompara">' . $serializedata->userchoice['usersdata']['dob'] . '</div>
                    </div>
                    <div class="row-fluid">
                        <div class="span5 customheading"><b>Anniversary Date:</b></div>
                        <div class="span7 custompara">' . $serializedata->userchoice['usersdata']['aniversary'] . '</div>
                    </div>
                </div>
            </div>
            <div class="row-fluid">
                <div class="span12 specialemail">
                    <h3></h3>
                    <div class="row-fluid">
                        <div class="span7 customheading"><b>How did you hear about us:</b></div>
                        <div class="span5 custompara">' . $serializedata->userchoice['usersdata']['findingusby'] . '</div>
                    </div>
                    <div class="row-fluid">
                        <div class="span7 customheading"><b>Receive Notifications by email:</b></div>
                        <div class="span5 custompara">' . $recievemailstatus . '</div>
                    </div>
                </div>
            </div>
        </div>
        <div class="row-fluid">
            <div class="span12 sunsetdinner">
                <h3>' . $eventdata[0]->name . '</h3>
                <h4>TICKETS</h4>
                <div class="row-fluid">
                    <div class="span6 innerheading" align="center">Ticket Type</div>
                    <div class="span6 innerheading" align="center">Ticket Count</div>
                </div>
                ' . $tickethtml . '
                <h4>ENHANCEMENTS</h4>
                ' . $enhancehtml . '
                <div class="row-fluid">
                    <div class="span6"><h4>ORDER TOTAL:</h4></div>
                    <div class="span6 custompara" align="center">' . uc_currency_format($order->order_total) . '</div>
                </div>
            </div>
        </div>
    </div>
</div>';
    return $html;
}
