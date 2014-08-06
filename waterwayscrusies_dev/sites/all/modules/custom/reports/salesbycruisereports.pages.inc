<?php

/**
 * Displays the sales by cruise order reports.
 */
function salesbycruise_order_reports() {
    return drupal_render(drupal_get_form('salesbycruise_reports_form'));
}

function salesbycruise_reports_form_validate($form, &$form_state) {
    if ($form_state['triggering_element']['#value'] == 'Generate Report') {
        $form_state['rebuild'] = TRUE;
        return;
    }
}

function salesbycruise_reports_form($form, &$form_state) {
    drupal_add_css(drupal_get_path('module', 'reports') . '/reports.css');
//    $option = 'completed';
    $option = 'payment_received';
    $cruisetitle = isset($_GET['cruisetitle']) ? $_GET['cruisetitle'] : '';
    $cruisetitle = trim($cruisetitle);
    if (isset($_GET['option'])) {
        $option = $_GET['option'];
    }
    if (isset($_GET['export_btn'])) {
        salesbycruise_reports_export($_GET);
    }

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
    $form['cruisetitle'] = array(
        '#id' => 'cruisetitle',
        '#type' => 'textfield',
        '#title' => t(''),
        '#description' => '',
        '#default_value' => isset($cruisetitle) ? $cruisetitle : '',
        '#prefix' => '<div class="row-fluid"><div class="span12"><div class="pull-left">',
        '#suffix' => '</div>',
        '#attributes' => array('class' => array('input-medium'), 'placeholder' => array('Cruise Title')),
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
        '#attributes' => array('class' => array('dateFieldStyle dateFieldMedium ')),
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
        '#submit' => array('salesbycruise_reports_export'),
        '#prefix' => '<div class="pull-left" style="margin-left: 0px;margin-top:13px;">',
        '#suffix' => '</div></div></div>',
    );

    $output = '';

    $header = array(
//        array(
//            'data' => 'Id',
////            'field' => 'uid'
//        ),
        array(
            'data' => 'Title',
            'field' => 'name'
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

    $query = db_select("cruisetypes", "ct");
    $query->Join('cruise', 'c', 'ct.id = c.cruise_type');
    $query->Join('scheduledevents', 's', 'c.id = s.cruisetemplate_id');
    $query->Join('reservation', 'r', 's.id = r.eventid');
    $query->Join('uc_orders', 'o', 'r.orderid = o.order_id');

    if ($order_starttime != '' && $order_endtime != '') {
        if ($order_starttime < $order_endtime != '') {
            $order_enddate = $order_enddate . " 23:59:59";
            $order_startdate = $order_startdate . " 00:00:00";
            $query->where("o.created >= UNIX_TIMESTAMP('$order_startdate') and o.created <= UNIX_TIMESTAMP('$order_enddate')");
        } else {
            drupal_set_message("End date must be Greater than start date", 'warning');
        }
    }
//    $query->fields('o', array(
////        'order_id',
//        'uid',
//        'order_total',
//        'delivery_first_name',
//        'delivery_last_name',
//        'primary_email',
//        'created',
//        'delivery_phone',
//        'order_status',
//    ));
    $query->fields('ct', array('name',));
    $query->addExpression('@row := @row + 1', 'position');
    $query->addExpression('COUNT(order_id)', 'orders');
    $query->addExpression('GROUP_CONCAT(order_id)', 'orderslist');
    $query->addExpression('SUM(order_total)', 'total');
    $query->addExpression('SUM(product_count)', 'products');
    $query->groupBy('ct.name');
    if ($option != '') {
        $query->condition('order_status', $option, '=');
    }
    if ($cruisetitle != '') {
        $query->condition('ct.name', '%' . db_like($cruisetitle) . '%', 'LIKE');
    }
    $query->orderBy($order, $sort);
    $query = $query->extend('TableSort')->extend('PagerDefault')->limit(10);

    /*   for creating mysql custom index column for the orders    */
    db_query("set @row=0");

    $result = $query->execute()->fetchAll();

    $rows = array();
    if (count($result) > 0) {
        for ($i = 0; $i < count($result); $i ++) {
            $detailsview = '<a href="' . base_path() . 'customer/' . $result [$i]->uid . '/orders"><input type="button" class="button" value="Detail View"></a>';

            $tickets = getticketsbyorders($result [$i]->orderslist);

            $rows [] = array(
//                $result [$i]->position,
//                $i+1,
                $result [$i]->name,
                $result [$i]->orders,
                $tickets,
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

/*   orders by cruise --- manifest orders list view    */

function orders_by_cruise() {
    return drupal_render(drupal_get_form('orders_by_cruise_form'));
}

function orders_by_cruise_form_validate($form, &$form_state) {
    if ($form_state['triggering_element']['#value'] == 'Generate Report') {
        $form_state['rebuild'] = TRUE;
        return;
    }
}

function orders_by_cruise_form($form, &$form_state) {
    drupal_add_css(drupal_get_path('module', 'reports') . '/reports.css');

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
            'data' => 'Cruise Date',
            'field' => 'cruisedate'
        ),
        array(
            'data' => 'Title',
            'field' => 'eventname'
        ),
        array(
            'data' => 'Cruise Type',
            'field' => 'cruisetype'
        ),
        array(
            'data' => 'Inventory',
            'field' => 'ticketinventory'
        ),
        array(
            'data' => 'Available',
            'field' => 'availabletickets'
        ),
        array(
            'data' => 'Sold',
            'field' => 'soldtickets'
        ),
        array(
            'data' => 'Sales Total',
            'field' => 'total'
        ),
        array(
            'data' => '',
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
            case 'Title' :
                $order = 'eventname';
            case 'Cruise Type' :
                $order = 'cruisetype';
            case 'Inventory' :
                $order = 'ticketinventory';
            case 'Available' :
                $order = 'availabletickets';
            case 'Sold' :
                $order = 'soldtickets';
            case 'Sales Total' :
                $order = 'total';
            case 'Cruise Date' :
                $order = 'cruisedate';
        }
    } else {
        $sort = 'ASC';
        $order = 'id';
    }

    $query = db_select("scheduledevents", "s");
    $query->Join('scheduledevent_dates', 'sd', 's.id = sd.scheduledevent_id');
    $query->Join('reservation', 'r', 's.id = r.eventid');
    $query->Join('uc_orders', 'o', 'r.orderid = o.order_id');
    $query->Join('cruise', 'c', 'c.id = s.cruisetemplate_id ');
    $query->Join('cruisetypes', 'ct', 'ct.id = c.cruise_type');
    if ($order_starttime != '' && $order_endtime != '') {
        if ($order_starttime < $order_endtime != '') {
            $order_enddate = $order_enddate . " 23:59:59";
            $order_startdate = $order_startdate . " 00:00:00";
            $query->where("sd.scheduleddate >= UNIX_TIMESTAMP('$order_startdate') and sd.scheduleddate <= UNIX_TIMESTAMP('$order_enddate')");
        } else {
            drupal_set_message("End date must be Greater than start date", 'warning');
        }
    }
    $query->fields('s', array(
        'id',
        'ticketinventory',
        'availabletickets',
        'soldtickets',
        'boardingtime'
    ));
    $query->fields('sd', array('scheduleddate',));

    $query->addExpression('SUM(order_total)', 'total');
    $query->addExpression("CONCAT(FROM_UNIXTIME( sd.scheduleddate,'%Y-%d-%m'),' ',s.boardingtime)", 'cruisedate');
    $query->addExpression('s.name', 'eventname');
    $query->addExpression('ct.name', 'cruisetype');
    $query->groupBy('s.id');

    $query->orderBy($order, $sort);
    $query = $query->extend('TableSort')->extend('PagerDefault')->limit(10);
    $result = $query->execute()->fetchAll();

    $rows = array();
    if (count($result) > 0) {
        for ($i = 0; $i < count($result); $i ++) {
            $manifestview = '<a href="' . base_path() . 'cruise/' . $result [$i]->id . '/manifest" class="button">Manifest</a>';
            $rows [] = array(
                $result [$i]->id,
                $result [$i]->cruisedate,
//                date('Y-m-d',$result [$i]->scheduleddate).' '.$result [$i]->boardingtime,
                $result [$i]->eventname,
                $result [$i]->cruisetype,
                $result [$i]->ticketinventory,
                $result [$i]->availabletickets,
                $result [$i]->soldtickets,
                uc_currency_format($result [$i]->total),
                $manifestview
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

/*     cruise event manifest view     */

function cruise_manifest($cruise_eventid) {
    drupal_add_css(drupal_get_path('module', 'reports') . '/reports.css');
    $eventdata = getcruiseeventdata($cruise_eventid);
    $totalguest = 0;
    $totalentrees = 0;
    $totalenhancements = 0;
    $totalwindowtables = 0;

    /*  get all the vessels list   */
    $vesselslist = getVesselsdropdown();

    /*   Get the all ports list   */
    $portsdata = getPortsevents();
    $ports = array();
    foreach ($portsdata as $port) {
        if (isset($port[0]['tid'])) {
            $ports[$port[0]['tid']] = $port[0]['name'];
        }
    }

    /*   Get the Tickets list   */
    $ticketslist = array();
    $ticketslist = getTicketslist();
    $ticketsarray = array();
    foreach ($ticketslist as $ticket) {
        $ticketsarray[$ticket->id] = $ticket->title;
    }



    /*   Get the all Enhancements   */
    $enhanementlist = getenhancementslist();
    $enhanements = array();
    for ($i = 0; $i < count($enhanementlist); $i++) {
        $enhanementobj = new stdClass();
        $enhanementobj->title = $enhanementlist[$i]->title;
        $enhanementobj->nid = $enhanementlist[$i]->nid;
        $enhanementobj->qty = 0;
        $enhanements[$enhanementlist[$i]->nid] = $enhanementobj;
    }

    /*   get all entrees list   */
    $entreeslist = array();
    $entreeslist = getEntreeslist();
    $entrees = array();
    for ($i = 0; $i < count($entreeslist); $i++) {
        $entryobj = new stdClass();
        $entryobj->title = $entreeslist[$i]->title;
        $entryobj->id = $entreeslist[$i]->id;
        $entryobj->qty = 0;
        $entrees[$entreeslist[$i]->id] = $entryobj;
    }


    $guestorderhtml = '';
    $enhancementhtml = '';
    $entryhtml = '';


    /* Based on the event data by orders we will get the
     * Entrees
     * Enhancements
     * and guest order view
     */
    foreach ($eventdata as $eventdatarow) {

        $cruise_event_enhancements = array();
        $cruise_event_enhancements = $eventdatarow->enhancements;
        $curwindowtables = 0;
        $curguests = 0;

        /* get the Guest order html starts */
        $guestorderhtml_enhancementhtml = '';
        foreach ($cruise_event_enhancements as $cruise_event_enhancement) {
            $enhancement_id = $cruise_event_enhancement->enhancement_id;

            /* find the selected enhancement is winow enhancement or not for maintaining the 
             * window type enhancements
             */
            $iswindowenhance = CruiseEnhancements::is_window_inventory_item($enhancement_id);
            if ($iswindowenhance) {
                $totalwindowtables = $totalwindowtables + $cruise_event_enhancement->enhancement_qty;
                $curwindowtables = $curwindowtables + $cruise_event_enhancement->enhancement_qty;
            }
            $enhanements[$enhancement_id]->qty = $enhanements[$enhancement_id]->qty + $cruise_event_enhancement->enhancement_qty;
            $totalenhancements = $totalenhancements + $cruise_event_enhancement->enhancement_qty;
            $guestorderhtml_enhancementhtml.='<div class="row-fluid">
                        <div class="span2"></div>
                        <div class="span10">' . $cruise_event_enhancement->enhancement_qty . ' ' . $enhanements[$enhancement_id]->title . '</div>
                    </div>';
        }
        /* get the Guest order html ends */

        /* guest order tickets hml starts */
        $guestorderhtml_tickets = '';
        $ordertickets = array();
        $ordertickets = $eventdatarow->tickets;
        foreach ($ordertickets as $orderticket) {
            $orderticketentrees = array();
            $orderticketentrees = $orderticket->entrees;

            $totalguest = $totalguest + $orderticket->ticket_qty;
            $curguests = $curguests + $orderticket->ticket_qty;
//            $totalentrees = $totalentrees + count($orderticketentrees);
            $entrycomments = '';

            if (count($orderticketentrees) == 0) {
                $entrytitle = "no options";
                $guestorderhtml_tickets.='<div class="row-fluid">
                        <div class="span2"></div>
                        <div class="span10"> 1 ' . $ticketsarray[$orderticket->ticket_id] . ' - ' . $entrytitle . '</div>
                    </div>';
            }
            foreach ($orderticketentrees as $orderticketentry) {
                if (isset($orderticketentry->entree_id) && $orderticketentry->entree_id != 0) {
                    $totalentrees=$totalentrees+1;
                    $entrees[$orderticketentry->entree_id]->qty = $entrees[$orderticketentry->entree_id]->qty + 1;
                    $entrytitle = isset($entrees[$orderticketentry->entree_id]->title) ? $entrees[$orderticketentry->entree_id]->title : "no options";
                    $entrycomments .='  ' . $orderticketentry->comments;
                    $guestorderhtml_tickets.='<div class="row-fluid">
                        <div class="span2"></div>
                        <div class="span10"> 1 ' . $ticketsarray[$orderticket->ticket_id] . ' - ' . $entrytitle . '</div>
                    </div>';
                }else{
                    $entrycomments .='  ' . $orderticketentry->comments;
                    $guestorderhtml_tickets.='<div class="row-fluid">
                        <div class="span2"></div>
                        <div class="span10"> 1 ' . $ticketsarray[$orderticket->ticket_id] . ' - no options</div>
                    </div>';
                }
            }
        }
        /* guest order tickets hml ends */

        $iswindowavailable = '';
        if ($curwindowtables > 0) {
            $iswindowavailable = '<br/>W';
        }

        /* guest order html view individually starts */
        $guestorderhtml.='<div class="row-fluid" style="margin-bottom:10px;">
                <div class="span12 contentborder fullcontent">
                    <div class="row-fluid">
                        <div class="span2" align="center"><h3>' . $curguests . $iswindowavailable . '</h3></div>
                        <div class="span4">Guest: <b>' . $eventdatarow->delivery_last_name . ', ' . $eventdatarow->delivery_first_name . '</b> </div>
                        <div class="span3">Phone: ' . $eventdatarow->delivery_phone . '</div>
                        <div class="span3">order no: ' . $eventdatarow->order_id . '</div>
                    </div>';
        $guestorderhtml.=$guestorderhtml_enhancementhtml . $guestorderhtml_tickets;

        $another_party_comments = isset($eventdatarow->userchoices[0]->another_party_comments) ? $eventdatarow->userchoices[0]->another_party_comments : "";
        $celebration_comments = isset($eventdatarow->userchoices[0]->celebration_comments) ? $eventdatarow->userchoices[0]->celebration_comments : "";
        $groupons = array();
        $groupons = $eventdatarow->groupons;
        $grouponsused = '';
        foreach ($groupons as $groupon) {
            if ($grouponsused != '') {
                $grouponsused.=', ' . $groupon->title;
            } else {
                $grouponsused.=$groupon->title;
            }
        }
        $guestorderhtml.='<div class="row-fluid" align="center">
                        <div class="span2">Special Concerns:</div>
                        <div class="span10" align="left">' . $another_party_comments . '</div>
                    </div>
                    <div class="row-fluid" align="center">
                        <div class="span2">Comments:</div>
                        <div class="span10" align="left">' . $celebration_comments . '</div>
                    </div>
                    <div class="row-fluid" align="center">
                        <div class="span2">Groupon:</div>
                        <div class="span10" align="left">' . $grouponsused . '</div>
                    </div>
                </div>
            </div>';
        /* guest order html view individually ends */
    }

    /*   manifest view params starts here   */
    $cruise_eventname = isset($eventdata[0]->name) ? $eventdata[0]->name : "";
    $scheduleddate = isset($eventdata[0]->scheduleddate) ? $eventdata[0]->scheduleddate : "";
    $vesselname = isset($vesselslist[$eventdata[0]->vessel_id]) ? $vesselslist[$eventdata[0]->vessel_id] : "";
    $portdata = loadportById($eventdata[0]->port_id);
    $portname = isset($portdata['portname']) ? $portdata['portname'] : "";
    $boardingtime = isset($eventdata[0]->boardingtime) ? $eventdata[0]->boardingtime : "";
    $departuretime = isset($eventdata[0]->departuretime) ? $eventdata[0]->departuretime : "";
    $returntime = isset($eventdata[0]->returntime) ? $eventdata[0]->returntime : "";

    /*  Total enhancements list view individually starts    */
    foreach ($enhanements as $enhanement) {
        if ($enhanement->qty != '' && $enhanement->qty > 0) {
            $enhancementhtml.='<div class="row-fluid">
                                <div class="span12 specialmain">' . $enhanement->qty . ':' . $enhanement->title . '</div>
                            </div>';
        }
    }
    /*  Total enhancements list view individually ends    */

    /*  Total Entrees list view individually starts    */
    foreach ($entrees as $entry) {
        if ($entry->qty != '' && $entry->qty > 0) {
            $entryhtml.='<div class="row-fluid">
                                <div class="span12 specialmain">' . $entry->qty . ':' . $entry->title . '</div>
                            </div>';
        }
    }
    if ($entryhtml == '') {
        $entryhtml.='<div class="row-fluid">
                                <div class="span12 specialmain">' . $totalguest . ': no options</div>
                            </div>';
    }
    /*  Total Entrees list view individually ends    */


    /* Total Manifest view by combining the all html contents starts here  */
    $html = '';
    $html = '<div class="container-fluid">
            <div class="row-fluid">
                <div class="span6 content" style="border:none;" align="left"><h3>Cruise Manifest / BEO</h3></div>
                <div class="span6 content" align="right"><h3>' . $cruise_eventname . '<br/>' . date('m/d/Y', $scheduleddate) . '</h3></div>
            </div>
            <div class="row-fluid">
                <div class="span6 specialmain">
                    <div class="row-fluid">
                        <div class="span4 specialmain" align="right">Event #:</div>
                    </div>
                    <div class="row-fluid">
                        <div class="span4 specialmain" align="right">Vessel:</div>
                        <div class="span8 specialmain">' . $vesselname . '</div>
                    </div>
                    <div class="row-fluid">
                        <div class="span4 specialmain" align="right">Port:</div>
                        <div class="span8 specialmain">' . $portname . '</div>
                    </div>
                    <div class="row-fluid">
                        <div class="span4 specialmain" align="right">Total Guests:</div>
                        <div class="span8 specialmain">' . $totalguest . '</div>
                    </div>
                    <div class="row-fluid">
                        <div class="span4 specialmain" align="right">Window Tables:</div>
                        <div class="span8 specialmain">' . $totalwindowtables . '</div>
                    </div>
                </div>
                <div class="span6 specialmain">
                    <div class="row-fluid">
                        <div class="span4 specialmain" align="right">Boarding:</div>
                        <div class="span8 specialmain">' . $boardingtime . '</div>
                    </div>
                    <div class="row-fluid">
                        <div class="span4 specialmain" align="right">Departure:</div>
                        <div class="span8 specialmain">' . $departuretime . '</div>
                    </div>
                    <div class="row-fluid">
                        <div class="span4 specialmain" align="right">Return:</div>
                        <div class="span8 specialmain">' . $returntime . '</div>
                    </div>
                    <div class="row-fluid">
                        <div class="span4 specialmain" align="right">Total Entrees:</div>
                        <div class="span8 specialmain">' . $totalentrees . '</div>
                    </div>
                    <div class="row-fluid">
                        <div class="span4 specialmain" align="right">Total Enhancments:</div>
                        <div class="span8 specialmain">' . $totalenhancements . '</div>
                    </div>
                </div>
            </div>
            <div class="row-fluid">
                <div class="span6 specialmain">
                    <div class="row-fluid">
                        <div class="span4 specialmain" align="right">Entrees:</div>
                        <div class="span8 specialmain" align="left">
                            ' . $entryhtml . '
                        </div>
                    </div>                    
                </div>                
            </div>
            <div class="row-fluid">
                <div class="span6 specialmain">
                    <div class="row-fluid">
                        <div class="span4 specialmain" align="right">Enhancements:</div>
                        <div class="span8 specialmain" align="left">
                            ' . $enhancementhtml . '
                        </div>
                    </div>                    
                </div>                
            </div>            
            ' . $guestorderhtml . '
           </div>';
    /* Total Manifest view by combining the all html contents ends here  */
    return $html;
}

function salesbycruise_reports_export($params) {
    $option = 'payment_received';
    $cruisetitle = isset($params['cruisetitle']) ? $params['cruisetitle'] : '';
    $cruisetitle = trim($cruisetitle);
    if (isset($params['option'])) {
        $option = $params['option'];
    }

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

    $output = '';

    $header = array(
        0 => 'Title',
        1 => 'Orders',
        2 => 'Tickets',
        3 => 'Total Amount',
    );


    $query = db_select("cruisetypes", "ct");
    $query->Join('cruise', 'c', 'ct.id = c.cruise_type');
    $query->Join('scheduledevents', 's', 'c.id = s.cruisetemplate_id');
    $query->Join('reservation', 'r', 's.id = r.eventid');
    $query->Join('uc_orders', 'o', 'r.orderid = o.order_id');

    if ($order_starttime != '' && $order_endtime != '') {
        if ($order_starttime < $order_endtime != '') {
            $order_enddate = $order_enddate . " 23:59:59";
            $order_startdate = $order_startdate . " 00:00:00";
            $query->where("o.created >= UNIX_TIMESTAMP('$order_startdate') and o.created <= UNIX_TIMESTAMP('$order_enddate')");
        } else {
            drupal_set_message("End date must be Greater than start date", 'warning');
        }
    }
//    $query->fields('o', array(
////        'order_id',
//        'uid',
//        'order_total',
//        'delivery_first_name',
//        'delivery_last_name',
//        'primary_email',
//        'created',
//        'delivery_phone',
//        'order_status',
//    ));
    $query->fields('ct', array('name',));
    $query->addExpression('@row := @row + 1', 'position');
    $query->addExpression('COUNT(order_id)', 'orders');
    $query->addExpression('GROUP_CONCAT(order_id)', 'orderslist');
    $query->addExpression('SUM(order_total)', 'total');
    $query->addExpression('SUM(product_count)', 'products');
    $query->groupBy('ct.name');
    if ($option != '') {
        $query->condition('order_status', $option, '=');
    }
    if ($cruisetitle != '') {
        $query->condition('ct.name', '%' . db_like($cruisetitle) . '%', 'LIKE');
    }
    $query->orderBy($order, $sort);
    $query = $query->extend('TableSort')->extend('PagerDefault')->limit(10);

    /*   for creating mysql custom index column for the orders    */
    db_query("set @row=0");

    $result = $query->execute()->fetchAll();

    $rows = array();
    if (count($result) > 0) {
        for ($i = 0; $i < count($result); $i ++) {
            $detailsview = '<a href="' . base_path() . 'customer/' . $result [$i]->uid . '/orders"><input type="button" class="button" value="Detail View"></a>';

            $tickets = getticketsbyorders($result [$i]->orderslist);

            $rows [] = array(
                $result [$i]->name,
                $result [$i]->orders,
                $tickets,
                uc_currency_format($result [$i]->total),
            );
        }
    }
}
