<?php

require_once 'sites/all/libraries/PHPExcel/Classes/PHPExcel/IOFactory.php';

/**
 * Provides a wrapper on the edit form to add a new entity.
 */
function coupon_add($coupontype) {
    if (!isset($coupon)) {
        $coupon = new stdClass();
    }
    $coupon->createtype = $coupontype;
    // Create a basic entity structure to be used and passed to the validation
    // and submission functions.
//    $coupon = entity_get_controller('coupon')->create();
    if ($coupontype == 'groupon' || $coupontype == 'web' || $coupontype == 'livingsocial') {
        return drupal_get_form('groupon_form', $coupon);
    } else {
        return drupal_get_form('coupon_form', $coupon);
    }
}

/**
 * Form function to create an coupon entity.
 *
 * The pattern is:
 * - Set up the form for the data that is specific to your
 *   entity: the columns of your base table.
 * - Call on the Field API to pull in the form elements
 *   for fields attached to the entity.
 */
function coupon_form($form, &$form_state, $coupon) {


    drupal_add_css(drupal_get_path('module', 'coupon') . '/coupon.css');
    $formstatus = $coupon->id == '' ? "add" : "edit";
    $title = '';
    $vessels = getVesselsdropdown();
    if ($formstatus == 'add') {
        $createtype = $coupon->createtype;
        $title = 'Create New ' . $createtype;
        if ($createtype == 'coupon') {
            $title = 'Create New Discount';
        }
        if ($createtype == 'groupon') {
        	$title = 'Create New Deal';
        }
    }
    if ($formstatus == 'edit') {
        $coupon->createtype = $coupon->type;
        $createtype = $coupon->createtype;
        $title = 'Edit ' . $createtype;
        if ($createtype == 'coupon') {
            $title = 'Edit Discount';
        }
    }
    $createtype = isset($coupon->createtype) ? $coupon->createtype : '';
//    if ($createtype == '') {
//        $coupon->createtype = $coupon->type;
//        $createtype = $coupon->type;
//    }
    if (!($createtype == 'coupon' || $createtype == 'voucher')) {
        drupal_set_message('Page not found', $type = 'warning');
        $path = 'admin/structure/coupon/manage/list';
        drupal_goto($path);
    }
    $title = ucwords($title);
    drupal_set_title($title, $output = CHECK_PLAIN);
//    $events = events_load();
    $events = getallcruise_events();
//    var_dump($events);
//    exit();
    foreach ($events as $event) {
//        $crusieevent[$event->id] = $event->name;
        $scheduleddate = date('Y-m-d', $event->scheduleddate);
//        if($event->name ==''){
//            $event->name='N/A';
//        }
//        $crusieevent[$event->id] = '<div class="pull-left chkbox"><div class="pull-left chkboxevent">' . $event->name . '</div><div class="pull-left chkboxdate">' . $scheduleddate . ' ' . $event->boardingtime . '</div><div class="pull-left chkboxdate">' . $scheduleddate . ' ' . $event->returntime . '</div> </div>';
//        $crusieevent[$event->scheduledevent_id] = '<div class="pull-left chkbox"><div class="pull-left chkboxevent">' . $event->name . '</div><div class="pull-left chkboxdate">' . $scheduleddate . ' ' . $event->boardingtime . '</div><div class="pull-left chkboxdate">' . $scheduleddate . ' ' . $event->returntime . '</div> </div>';
        $portdata = array();
        $portdata = getPortdetails($event->port_id);
        $portname = isset($portdata['title']) ? $portdata['title'] : '';
        $vesselname = isset($vessels[$event->vessel_id]) ? $vessels[$event->vessel_id] : '';

        $crusieevent[$event->scheduledevent_id] = '<div class="pull-left chkbox"><div class="pull-left chkboxevent">' .
                $event->cruise_title . '</div><div class="pull-left chkboxdate">' . $scheduleddate . ' ... ' .
                $event->departuretime . ' .. ' . $event->boardingtime . ' .. ' . $event->returntime .
                '</div><div class="pull-left chkboxport">' . $portname . '</div><div class="pull-left chkboxport">' .
                $vesselname . '</div> </div>';
    }

    $form['coupondivstart'] = array(
        '#type' => 'markup',
        '#markup' => '<div id="couponForm">',
    );


    $form['operation'] = array(
        '#type' => 'hidden',
        '#value' => $formstatus
    );

    $auto_id = $coupon->id != '' ? $coupon->id : "";
    $form['auto_id'] = array(
        '#type' => 'hidden',
        '#value' => $auto_id
    );

    if ($formstatus == 'edit') {
        $satsus = TRUE;
    } else {
        $satsus = FALSE;
    }

    $createtypestyle = '';
    $type = array("coupon" => t('Coupon'), "voucher" => t('Voucher'));
    if ($createtype == 'voucher') {
        $coupon->type = 'voucher';
        $coupon->coupontype = 'flatfee';
        $createtypestyle = 'display:none;';
    }
    if ($createtype == 'coupon') {
        $coupon->type = 'coupon';
    }

    $form['createtype'] = array(
        '#type' => 'textfield',
        '#prefix' => '<div style="display:none;>',
        '#suffix' => '</div>',
        '#value' => $createtype
    );
    $form['type'] = array(
        '#type' => 'radios',
        '#title' => t('Type'),
        '#options' => $type,
        '#default_value' => isset($coupon->type) ? $coupon->type : 'coupon',
        '#title_display' => 'invisible',
        '#attributes' => array('class' => array('radio_buttons optional')),
        '#prefix' => '<div class="row-fluid" style="display:none;"><div class="span12">',
        '#suffix' => '</div></div><div class="row-fluid"><div class="span12">',
    );

    $form['coupon_code'] = array(
        '#type' => 'textfield',
        '#title' => t('Code'),
        '#required' => TRUE,
        '#default_value' => isset($coupon->coupon_code) ? $coupon->coupon_code : '',
        '#ajax' => array(
            'callback' => 'checkcouponexists',
        ),
        '#disabled' => $satsus,
        '#attributes' => array('class' => array('inputfiledStyle inputfieldMedium')),
        '#prefix' => '<div class="row-fluid"><div class="span12" style="margin-left: 0px;>',
        '#suffix' => '</div></div>',
    );



    $form['couponmessageWrapper'] = array(
        '#type' => 'markup',
        '#prefix' => '<div id="coupon_status_message">',
        '#suffix' => '</div>',
    );

    $coupontype = array("percentage" => t('Percentage'), "flatfee" => t('flat Fee'), "2 for 1" => t('2 for 1'));
    $form['coupontype'] = array(
        '#type' => 'radios',
        '#title' => t('Coupon Type'),
        '#options' => $coupontype,
        '#default_value' => isset($coupon->coupontype) ? $coupon->coupontype : 'percentage',
        '#attributes' => array('class' => array('radio_buttons optional')),
        '#prefix' => '<div class="row-fluid" style="' . $createtypestyle . '"><div class="span12">',
        '#suffix' => '</div></div><div class="row-fluid"><div class="span12">',
        '#states' => array(
            'invisible' => array(// action to take.
                ':input[name="type"]' => array('value' => 'vouchercode'),
            ),
        ),
    );
    if ($coupon->coupontype == 'percentage') {
        $couponrate = isset($coupon->rate) ? $coupon->rate : '';
    }
    if ($coupon->coupontype == 'flatfee') {
        $couponflatrate = isset($coupon->rate) ? $coupon->rate : '';
    }
    if ($coupon->coupontype == '2 for 1') {
        $coupontwoforonerate = isset($coupon->rate) ? $coupon->rate : '';
    }

    $form['rate'] = array(
        '#title' => t('Rate'),
        '#type' => 'textfield',
//        '#required' => TRUE,
        '#default_value' => isset($couponrate) ? $couponrate : '',
        '#attributes' => array('class' => array('inputfiledStyle inputfieldMedium')),
//        '#prefix' => '<div class="row-fluid"><div class="span12">',
//        '#suffix' => '</div></div>',
        '#states' => array(
            'visible' => array(// action to take.
                ':input[name="coupontype"]' => array('value' => 'percentage'),
            ),
        ),
    );
    $form['flatfeerate'] = array(
        '#title' => t('Rate'),
        '#type' => 'textfield',
        '#default_value' => isset($couponflatrate) ? $couponflatrate : '',
        '#attributes' => array('class' => array('inputfiledStyle inputfieldMedium')),
//        '#prefix' => '<div class="row-fluid"><div class="span12">',
//        '#suffix' => '</div></div>',
        '#states' => array(
            'visible' => array(// action to take.
                ':input[name="coupontype"]' => array('value' => 'flatfee'),
            ),
        ),
    );
    $form['twoforonerate'] = array(
        '#title' => t('Max number of tikets to discount'),
        '#type' => 'hidden',
        '#default_value' => isset($coupontwoforonerate) ? $coupontwoforonerate : '',
        '#attributes' => array('class' => array('inputfiledStyle inputfieldMedium')),
//        '#prefix' => '<div class="row-fluid"><div class="span12">',
        '#states' => array(
            'visible' => array(// action to take.
                ':input[name="coupontype"]' => array('value' => '2 for 1'),
            ),
        ),
//        '#suffix' => '</div></div>',
    );


    $couponvalidatetype = array('sequential' => t('Sequential'), 'multiple' => t('Multidaterange'));

    $form['coupon_validatetype'] = array(
        '#type' => 'radios',
        '#title' => t('Coupon Validate Type'),
        '#default_value' => isset($coupon->coupon_validatetype) ? $coupon->coupon_validatetype : 'sequential',
        '#prefix' => '</div></div><div class="row-fluid"><div class="span12">',
        '#suffix' => '</div></div>',
        '#options' => $couponvalidatetype,
    );



    $form['couponstartenddate'] = array(
        '#type' => 'markup',
        '#markup' => 'Promotion Start/End Dates',
        '#prefix' => '<label>',
        '#suffix' => '</label><div class="row-fluid customRowpromo">',
    );

    $form['promo_startdate'] = array(
        '#id' => 'promo_start_date',
        '#type' => 'date_popup',
        '#date_format' => 'Y-m-d',
        '#title' => t('Start Date'),
        '#description' => '',
        '#attributes' => array('class' => array('dateFieldStyle dateFieldMedium')),
        '#default_value' => isset($coupon->promo_startdate) ? $coupon->promo_startdate : '',
//        '#prefix' => '<div class="row-fluid"><div class="span3">',
        '#prefix' => '<div class="span3">',
        '#suffix' => '</div>',
        '#states' => array(
            'visible' => array(// action to take.
                ':input[name="coupon_validatetype"]' => array('value' => 'sequential'),
            ),
        ),
    );
    $form['promo_enddate'] = array(
        '#id' => 'promo_end_date',
        '#type' => 'date_popup',
        '#date_format' => 'Y-m-d',
        '#title' => t('End Date'),
        '#default_value' => isset($coupon->promo_enddate) ? $coupon->promo_enddate : '',
        '#description' => '',
        '#attributes' => array('class' => array('dateFieldStyle dateFieldMedium')),
        '#prefix' => '<div class="span9">',
//        '#suffix' => '</div></div>',
        '#suffix' => '</div>',
        '#states' => array(
            'visible' => array(// action to take.
                ':input[name="coupon_validatetype"]' => array('value' => 'sequential'),
            ),
        ),
    );

    $sel1ecteddates = isset($coupon->coupon_discounteddates) ? $coupon->coupon_discounteddates : '';
    $dates = array();
    if ($sel1ecteddates != '') {
        $dates = explode(',', $sel1ecteddates);
    }
    $datestring = '';
    foreach ($dates as $date) {
        if ($datestring != '') {
            $datestring.=",'" . $date . "'";
        } else {
            $datestring = "'" . $date . "'";
        }
    }
    if ($datestring != '') {
        $datestring = '{ addDates: [' . $datestring . '] }';
    }
    $form['multidate'] = array(
        '#title' => t('Multiple dates'),
        '#id' => 'datePick',
        '#type' => 'textfield',
        '#value' => '',
        '#attributes' => array('class' => array('inputfiledStyle inputfieldMedium')),
        '#prefix' => '

<script type="text/javascript"> 
$(function() {
                $("#datePick").multiDatesPicker(' . $datestring . ');
            });
</script></div><div class="row-fluid customRowpromo"><div class="span12">',
        '#suffix' => '</div>',
        '#states' => array(
            'visible' => array(// action to take.
                ':input[name="coupon_validatetype"]' => array('value' => 'multiple'),
            ),
        ),
    );
//    $form['multidate'] = array(
//        '#title' => t('Multiple dates'),
//        '#id' => 'datePick',
//        '#type' => 'textfield',
//        '#value' => '',
//        '#attributes' => array('class' => array('inputfiledStyle inputfieldMedium')),
//        '#prefix' => '</div><div class="row-fluid customRowpromo"><div class="span12">',
//        '#suffix' => '</div>',
//        '#states' => array(
//            'visible' => array(// action to take.
//                ':input[name="coupon_validatetype"]' => array('value' => 'multiple'),
//            ),
//        ),
//    );

    $form['crusiestartenddate'] = array(
        '#type' => 'markup',
        '#markup' => 'Cruise Start/End Dates',
        '#prefix' => '</div><label>',
        '#suffix' => '</label>',
    );
    $form['crusie_startdate'] = array(
        '#id' => 'crusie_start_date',
        '#type' => 'date_popup',
        '#date_format' => 'Y-m-d',
//        '#required' => TRUE,
        '#title' => t('Start Date'),
        '#default_value' => isset($coupon->crusie_startdate) ? $coupon->crusie_startdate : '',
        '#description' => '',
        '#attributes' => array('class' => array('dateFieldStyle dateFieldMedium')),
        '#prefix' => '<div class="row-fluid"><div class="span3">',
        '#suffix' => '</div>',
    );

    $form['crusie_enddate'] = array(
        '#id' => 'crusie_end_date',
        '#type' => 'date_popup',
        '#date_format' => 'Y-m-d',
        '#title' => t('End Date'),
        '#default_value' => isset($coupon->crusie_enddate) ? $coupon->crusie_enddate : '',
        '#description' => '',
        '#attributes' => array('class' => array('dateFieldStyle dateFieldMedium')),
        '#prefix' => '<div class="span9">',
        '#suffix' => '</div></div>',
    );


    $form['couponactive'] = array(
        '#title' => t('Active'),
        '#default_value' => isset($coupon->couponactive) ? $coupon->couponactive : '',
        '#type' => 'checkbox',
        '#prefix' => '<div class="row-fluid"><div class="span12">',
        '#suffix' => '</div></div>',
    );

    $form['company'] = array(
        '#title' => t('Company'),
        '#default_value' => isset($coupon->company) ? $coupon->company : '',
        '#type' => 'textfield',
        '#prefix' => '<div class="row-fluid"><div class="span12">',
        '#suffix' => '</div></div>',
    );
    $form['comments'] = array(
        '#title' => t('Comments'),
        '#default_value' => isset($coupon->comments) ? $coupon->comments : '',
        '#type' => 'textarea',
        '#prefix' => '<div class="row-fluid"><div class="span12">',
        '#suffix' => '</div></div>',
    );


    foreach ($crusieevent as $t) {
        $evntslist[]['Event'] = $t;
    }

//    $header = array('checkall' => t('<div class="pull-left chkbox"><div class="pull-left chkboxevent">Select / Unselect all</div><div class="pull-left chkboxdate">Start Date</div><div class="pull-left chkboxdate">End Date</div> </div>'));
    $options = array();
//    foreach ($evntslist as $evntslistss) {
//        $evnet = $evntslistss['Event'];
//
//        $options[$evntslistss['Event']["name"]]['checkall'] = $evntslistss['Event']["title"];
//    }
//
//   var_dump($options);
// exit();


    $form['crusievent'] = array(
        '#type' => 'fieldset',
        '#title' => t('Cruise Events'),
        '#collapsible' => TRUE, // Added
        '#collapsed' => FALSE, // Added
        '#prefix' => '<div class="row-fluid"><div class="span12">',
        '#suffix' => '</div></div>',
    );

    $couponevents = array();
    $couponid = isset($coupon->id) ? $coupon->id : '';
    if ($couponid != '') {
        $couponevents = array();
        $couponevents = getcouponevents($couponid);
        if (isset($couponevents[0]->Event_id)) {
            $evntids = explode(',', $couponevents[0]->Event_id);
            $options_checked = events_load_byid($evntids);
        }
    }
    $cruises = getallcruisedetails();
    $cruiseoptions = array();
    $cruiseoptions[''] = 'All';
    foreach ($cruises as $cruise) {
        $cruiseoptions[$cruise->id] = $cruise->cruise_title;
    }
//    getPortsWithcruises()
    $portslist = getPortslist();


    $form['crusievent']["cruisetype"] = array(
        '#type' => 'select',
        '#title' => t('Cruise'),
        '#options' => $cruiseoptions,
//        '#default_value' => 'defalut_value',
//        '#ajax' => array(
//            'event' => 'change',
//            'callback' => 'cruise_events',
//            'wrapper' => 'crusievent',
//            'method' => 'replace',
//        ),
        '#prefix' => '<div class="row-fluid"><div class="span12"><div class="pull-left cruiseeventsearch">',
        '#suffix' => '</div>',
//        '#attributes' => array('onchange' => "form.eventsubmit('cruisetype')"),
    );
    $portslist[''] = 'All';
    ksort($portslist);
    $form['crusievent']["chooseport"] = array(
        '#type' => 'select',
        '#title' => t('Port'),
        '#options' => $portslist,
//        '#default_value' => 'defalut_value',
//        '#ajax' => array(
//            'event' => 'change',
//            'callback' => 'cruise_events',
//            'wrapper' => 'crusievent',
//            'method' => 'replace',
//        ),
        '#prefix' => '<div class="pull-left cruiseeventsearch">',
        '#suffix' => '</div>',
//        '#attributes' => array('onchange' => "form.eventsubmit('cruisetype')"),
    );
    $vesselslist = $vessels;
    $vesselslist[''] = 'All';
    ksort($vesselslist);
    $form['crusievent']["choosevessel"] = array(
        '#type' => 'select',
        '#title' => t('Vessel'),
        '#options' => $vesselslist,
//        '#default_value' => 'defalut_value',
//        '#ajax' => array(
//            'event' => 'change',
//            'callback' => 'cruise_events',
//            'wrapper' => 'crusievent',
//            'method' => 'replace',
//        ),
        '#prefix' => '<div class="pull-left cruiseeventsearch">',
        '#suffix' => '</div></div></div>',
//        '#attributes' => array('onchange' => "form.eventsubmit('cruisetype')"),
    );

    $form['crusievent']["couponid"] = array(
        '#type' => 'textfield',
        '#title' => t('couponid'),
        '#default_value' => $couponid,
        '#prefix' => '<div style="display:none;">',
        '#suffix' => '</div>',
    );
//Submit button:

    $form['crusievent']['search_startdate'] = array(
        '#id' => 'promo_start_date',
        '#type' => 'date_popup',
        '#date_format' => 'Y-m-d',
        '#datepicker_options' => array('minDate' => "0"),
        '#title' => t('Start Date'),
        '#description' => '',
        '#attributes' => array('class' => array('dateFieldStyle dateFieldMedium')),
//        '#default_value' => isset($coupon->promo_startdate) ? $coupon->promo_startdate : '',
//        '#prefix' => '<div class="row-fluid"><div class="span3">',
        '#prefix' => '<div class="row-fluid"><div class="span12"><div class="pull-left cruiseeventsearch">',
        '#suffix' => '</div>',
        '#states' => array(
            'visible' => array(// action to take.
                ':input[name="coupon_validatetype"]' => array('value' => 'sequential'),
            ),
        ),
    );
    $form['crusievent']['search_enddate'] = array(
        '#id' => 'promo_end_date',
        '#type' => 'date_popup',
        '#date_format' => 'Y-m-d',
        '#title' => t('End Date'),
//        '#default_value' => isset($coupon->promo_enddate) ? $coupon->promo_enddate : '',
        '#description' => '',
        '#attributes' => array('class' => array('dateFieldStyle dateFieldMedium')),
        '#prefix' => '<div class="pull-left cruiseeventsearch">',
        '#suffix' => '</div>',
        '#datepicker_options' => array('minDate' => "0"),
        '#states' => array(
            'visible' => array(// action to take.
                ':input[name="coupon_validatetype"]' => array('value' => 'sequential'),
            ),
        ),
    );

    $form['crusievent']['eventsubmit'] = array(
        '#type' => 'submit',
        '#value' => t('Submit '),
        '#ajax' => array(
            'event' => 'click',
            'callback' => 'cruise_events',
            'wrapper' => 'crusievent',
            'method' => 'replace',
        ),
        '#submit' => array('cruise_events'),
        '#prefix' => '<div class="pull-left cruiseeventsearchbtn">',
        '#suffix' => '</div></div></div>',
    );
    $header = t('<div class="pull-left chkbox" style="margin-left:5%;"><div class="pull-left chkboxevent">Name</div><div class="pull-left chkboxdate">Schedule Date</div><div class="pull-left chkboxport">Port</div><div class="pull-left chkboxport">Vessel</div> </div>');

    $form['crusievent']['checkall'] = array(
        '#type' => 'checkbox',
        '#title' => t('Check All'),
        '#default_value' => '',
        '#ajax' => array(
            'event' => 'click',
            'callback' => 'cruise_check_events',
            'wrapper' => 'events',
            'method' => 'replace',
        ),
        '#suffix' => '<div class="row-fluid"><div class="span12">' . $header . '</div></div>',
    );

//    foreach ($events as $crusieeventrow) {
//        $form['crusievent']['events'][] = array(
//            '#id' => 'edit-events-' . $crusieeventrow->id,
//            '#type' => 'checkbox',
//            '#title' => t($crusieeventrow->name),
//            '#default_value' => '',
//        );
//    }
//    if (isset($form_state['values']['cruisetype']) && $form_state['values']['cruisetype'] != '') {
//        $selectedcruise = $form_state['values']['cruisetype'];
//        $crusieevent = array();
//        $crusitype_events=array();
//        $crusitype_events = getcruise_eventsbycruiseid($selectedcruise);
//        $test=array();
//        echo'test';
//        exit();
//        foreach ($crusitype_events as $crusitype_event) {
//            $scheduleddate = date('Y-m-d', $crusitype_event->scheduleddate);
////            $crusieevent[$crusitype_event->id] = '<div class="pull-left chkbox"><div class="pull-left chkboxevent">' . $crusitype_event->name . '</div><div class="pull-left chkboxdate">' . $scheduleddate . ' ' . $crusitype_event->boardingtime . '</div><div class="pull-left chkboxdate">' . $scheduleddate . ' ' . $crusitype_event->returntime . '</div> </div>';
////            $crusieevent[$crusitype_event->scheduledevent_id] = '<div class="pull-left chkbox"><div class="pull-left chkboxevent">' . $crusitype_event->name . '</div><div class="pull-left chkboxdate">' . $scheduleddate . ' ' . $crusitype_event->boardingtime . '</div><div class="pull-left chkboxdate">' . $scheduleddate . ' ' . $crusitype_event->returntime . '</div> </div>';
////            $crusieevent[$crusitype_event->scheduledevent_id] = '<div class="pull-left chkbox"><div class="pull-left chkboxevent">' . $crusitype_event->cruise_title . '</div><div class="pull-left chkboxdate">' . $scheduleddate . ' ' . $crusitype_event->boardingtime . '</div><div class="pull-left chkboxdate">' . $scheduleddate . ' ' . $crusitype_event->returntime . '</div> </div>';
//             $portdata=array();
//        $portdata=getPortdetails($event->port_id);
//        $portname=isset($portdata['title']) ? $portdata['title'] : '';
//        $vesselname=isset($vessels[$event->vessel_id]) ? $vessels[$event->vessel_id] : '';
//       
//        $crusieevent[$crusitype_event->scheduledevent_id] = '<div class="pull-left chkbox"><div class="pull-left chkboxevent">' . 
//                        $crusitype_event->cruise_title . '</div><div class="pull-left chkboxdate">' . $scheduleddate . ' ... ' . 
//                        $crusitype_event->departuretime.' .. '.$crusitype_event->boardingtime.' .. '. $crusitype_event->returntime. 
//                        '</div><div class="pull-left chkboxport">' . $portname . '</div><div class="pull-left chkboxport">' . 
//                        $vesselname . '</div> </div>';
//            
//        }
//        drupal_add_css(drupal_get_path('module', 'coupon') . '/coupon.css');
//    }

    $form['crusievent']['events'] = array(
        '#default_value' => array(),
//        '#required' => '0',
        '#key_type_toggled' => '0',
        '#weight' => '2',
        '#type' => 'checkboxes',
        '#options' => $crusieevent,
        '#default_value' => $evntids,
        '#title' => t(''),
    );
//    $form['crusievent']['events'] = array
//        (
//        '#type' => 'tableselect',
////         '#required' => TRUE,
//        '#header' => $header,
//        '#options' => $options,
//        '#default_value' => $options_checked,
//        '#empty' => t('No Events selected'),
//    );

    field_attach_form('coupon', $coupon, $form, $form_state);
    $form['actions'] = array('#type' => 'actions');
    $form['actions']['submit'] = array(
        '#type' => 'submit',
        '#value' => t('Save'),
    );
//    $form['actions']['cancel'] = array(
//        '#type' => 'submit',
//        '#value' => t('Cancel'),
//        '#submit' => array('callback_for_cancel_button'),
//        '#limit_validation_errors' => array(),
//    );
    if ($coupon->type == 'coupon') {
        $url = 'admin/structure/coupon/manage/list';
    }
    if ($coupon->type == 'voucher') {
        $url = 'admin/structure/voucher/manage/list';
    }
    $form['actions']['cancel'] = array(
        '#type' => 'link',
        '#title' => t('Cancel'),
        '#href' => $url,
        '#attributes' => array('class' => array('btn inputfieldMedium')),
    );

    $form['coupondivend'] = array(
        '#type' => 'markup',
        '#markup' => '</div>',
    );
    return $form;
}

/**
 * Validation handler for coupon_add_form form.
 * We pass things straight through to the Field API to handle validation
 * of the attached fields.
 */

/**
 * Form submit handler: submits basic_add_form information
 */
function coupon_form_submit($form, &$form_state) {
    $coupon = new stdClass();
    $coupon->coupon_code = $form_state['values']['coupon_code'];
    $coupon->promo_startdate = $form_state['values']['promo_startdate'];
    $coupon->promo_enddate = $form_state['values']['promo_enddate'];
    $coupon->crusie_startdate = $form_state['values']['crusie_startdate'];
    $coupon->crusie_enddate = $form_state['values']['crusie_enddate'];
    $coupon->type = $form_state['values']['type'];

    if ($coupon->type == 'voucher') {
        $coupon->coupontype = 'faltefee';
    }
    $coupon->coupontype = $form_state['values']['coupontype'];
    if ($coupon->coupontype == 'percentage') {
        $coupon->rate = $form_state['values']['rate'];
    }
    if ($coupon->coupontype == 'flatfee') {
        $coupon->rate = $form_state['values']['flatfeerate'];
    }
    if ($coupon->coupontype == '2 for 1') {
        $coupon->rate = $form_state['values']['twoforonerate'];
    }

    $coupon->couponactive = $form_state['values']['couponactive'];
    $coupon->id = $form_state['values']['auto_id'];
    $coupon->company = $form_state['values']['company'];
    $coupon->comments = $form_state['values']['comments'];
    $coupon->coupon_validatetype = $form_state['values']['coupon_validatetype'];

    $coupon->created = time();
    $coupon->coupon_discounteddates = isset($form_state['input']['multidate']) ? $form_state['input']['multidate'] : '';
    field_attach_submit('coupon', $coupon, $form, $form_state);
    $cruiseevents = array();
    $cruiseevents = $form_state['values']['events'];

    $coupon = coupon_save($coupon);
//    $evntname = $crusievent;
//    $evntdata = array();
//
//    if ($cruiseevents != '') {
//        $evntdata = events_load_byname($cruiseevents);
//    }
    $eventdata = '';

    $allcruiseevents = array();
    foreach ($cruiseevents as $rec) {
        if ($rec != 0) {
            $allcruiseevents[] = $rec;
        }
    }
    $eventdata = implode(',', $allcruiseevents);
//    foreach ($evntdata as $record) {
//        if ($eventdata == '') {
//            $eventdata = $record->id;
//        } else {
//            $eventdata .= ',' . $record->id;
//        }
//    }
//    $eventdata = implode(',', $eventsdata);
    $couponevents = array();
    $couponevents = getcouponevents($coupon->id);

    if (count($couponevents) > 0) {
        $eventdta = db_update('coupon_event') // Table name no longer needs {}
                ->fields(array('Event_id' => $eventdata,
                    'coupon_eventtype' => 'individual'))
                ->condition('coupon_id', $coupon->id, '=')
                ->execute();
    } else {
        $eventdta = db_insert('coupon_event')
                ->fields(array(
                    'coupon_id' => $coupon->id,
                    'Event_id' => $eventdata,
                    'created' => REQUEST_TIME,
                    'coupon_eventtype' => 'individual'
                ))
                ->execute();
    }


    $operation = $form_state['values']['operation'];
    $msg = '';
    if ($operation == "add") {
        $msg = $coupon->type . ' Created Successfully.';
    } else {
        $msg = $coupon->type . ' Updated Successfully.';
    }
    $msg = ucwords($msg);
    drupal_set_message($msg);
    if ($coupon->type == 'coupon') {
        $form_state['redirect'] = 'admin/structure/coupon/manage/list';
    }
    if ($coupon->type == 'voucher') {
        $form_state['redirect'] = 'admin/structure/voucher/manage/list';
    }
}

function callback_for_cancel_button($form, &$form_state) {
    global $base_url;
    if ($form['createtype'] ["#value"] == 'coupon') {
        $url = $base_url . '/admin/structure/coupon/manage/list';
    } else {
        $url = $base_url . '/admin/structure/voucher/manage/list';
    }
//    $url = $base_url . '/admin/structure/coupon/manage/list';
    drupal_goto($url);
}

function coupon_bulkupload_import_form($form, $form_state) {

    if (arg(1) == 'coupon') {
        $form['bulkimportcouponsample'] = array(
            '#prefix' => '<div class="row-fluid"><div class="span12" style="padding-top: 20px;">',
            '#suffix' => '</div></div>',
            '#markup' => '<a href="' . base_path() . 'sites/default/files/tmp/couponcodeexcel/couponcodes.xls" style="text-decoration:none; padding-right:30px; margin:auto - 0px;">Download Discounts Excel Template</a>',
        );
    } else if (arg(1) == 'voucher') {
        $form['bulkimportvouchersample'] = array(
            '#markup' => '<a href="' . base_path() . 'sites/default/files/tmp/couponcodeexcel/voucher.xls" style="text-decoration:none;">Download Voucher Excel Template</a>',
        );
    }
    $form['import'] = array(
        '#title' => t('Import'),
        '#type' => 'managed_file',
        '#description' => t('The uploaded csv will be imported and temporarily saved.'),
        '#upload_location' => 'public://tmp/',
        '#upload_validators' => array(
            'file_validate_extensions' => array('xls xlsx'),
        ),
    );

    $form['submit'] = array(
        '#type' => 'submit',
        '#value' => t('Import'),
    );
    return $form;
}

function coupon_bulkupload_import_form_submit($form, $form_state) {
    if (arg(1) == 'coupon') {
        $coupontypes = 'coupon';
    }
    if (arg(1) == 'voucher') {
        $coupontypes = 'voucher';
    }

    if ($form_state['values'] ['import'] != 0) {
        $excelFileName = db_query("SELECT uri FROM {file_managed} WHERE fid = :fid", array(
            ':fid' => $form_state['values'] ['import'],
                ))->fetchField();

        if (file_exists($excelFileName)) {

            $objPHPExcel = PHPExcel_IOFactory::load($excelFileName);
            $objWorksheet = $objPHPExcel->setActiveSheetIndex(0);
            $highestRow = $objWorksheet->getHighestRow();
            $highestColumn = $objWorksheet->getHighestColumn();
            $headingsArray = $objWorksheet->rangeToArray('A1:' . $highestColumn . '1', null, true, true, true);
            $headingsArray = $headingsArray[1];
            $r = -1;
            $couponsdata = array();
            try {
                for ($row = 2; $row <= $highestRow; ++$row) {
                    $dataRow = $objWorksheet->rangeToArray('A' . $row . ':' . $highestColumn . $row, null, true, true, true);
                    if ((isset($dataRow[$row]['A'])) && ($dataRow[$row]['A'] > '')) {
                        ++$r;
                        foreach ($headingsArray as $columnKey => $columnHeading) {
                            $couponsdata[$r][$columnHeading] = $dataRow[$row][$columnKey];
                        }
                    }
                }
            } catch (Exception $e) {
                watchdog_exception('Reading Excel File Error', $e, NULL, WATCHDOG_ERROR);
                return FALSE;
            }



            $resultkey = TRUE;
            try {
                if (count($couponsdata) > 0) {

                    $successcoupons = array();
                    $failurecoupons = array();
                    //Inserting Excel Records into Database Starts here
                    for ($i = 0; $i < count($couponsdata); $i++) {
                        $couponid = $couponsdata[$i]["code"];
                        $couponresult = array();
                        $couponresult = coupon_exist_check($couponid);



                        if (count($couponresult) > 0) {
                            $failurecoupons[] = $couponid;
                        } else {
                            $coupon = new stdClass();
                            $coupontype = isset($couponsdata[$i]["Type"]) ? $couponsdata[$i]["Type"] : '';
                            $coupon->type = $coupontypes;
                            $coupon->coupon_code = $couponsdata[$i]["code"];
                            if ($couponsdata[$i]["Promo Startdate"] != '') {
                                $coupon->promo_startdate = date('Y-m-d', strtotime($couponsdata[$i]["Promo Startdate"]));
                            } else {
                                $coupon->promo_startdate = '';
                            }
                            if ($couponsdata[$i]["Promo Enddate"] != '') {
                                $coupon->promo_enddate = date('Y-m-d', strtotime($couponsdata[$i]["Promo Enddate"]));
                            } else {
                                $coupon->promo_enddate = '';
                            }
                            if ($couponsdata[$i]["Cruise Startdate"] != '') {
                                $coupon->crusie_startdate = date('Y-m-d', strtotime($couponsdata[$i]["Cruise Startdate"]));
                            } else {
                                $coupon->crusie_startdate = '';
                            }
                            if ($couponsdata[$i]["Cruise Startdate"] != '') {
                                $coupon->crusie_enddate = date('Y-m-d', strtotime($couponsdata[$i]["Cruise Enddate"]));
                            } else {
                                $coupon->crusie_enddate = '';
                            }
                            if (arg(1) == 'voucher') {
                                $coupontype = 'flatfee';
                            }
                            $coupon->coupontype = $coupontype;
//                            $coupon->couponactive = $couponsdata[$i]["Active"];


                            if (strtoupper($couponsdata[$i]["Active"]) == 'YES') {
                                $coupon->couponactive = 1;
                            } else if (strtoupper($couponsdata[$i]["Active"]) == 'NO') {
                                $coupon->couponactive = 0;
                            }
                            $coupon->company = $couponsdata[$i]["Company"];
                            $coupon->comments = $couponsdata[$i]["Comments"];
                            $coupon->coupon_validatetype = $couponsdata[$i]["Validate Type"];
                            $coupon->rate = $couponsdata[$i]["Rate"];
                            $coupon->created = time();
                            $coupon->coupon_discounteddates = $couponsdata[$i]["DiscountedDates"];
                            $crusieEvents = $couponsdata[$i]["crusieEvents"];
                            $evntlist = explode(',', $crusieEvents);

                            $couponsave = coupon_save($coupon);
                            if ($couponsave) {
                                $successcoupons[] = $couponid;
                            } else {
                                $failurecoupons[] = $couponid;
                            }

                            /* events data Insertion
                              //                    $eventdata = '';
                              //                    foreach ($evntdata as $record) {
                              //                        if ($eventdata == '') {
                              //                            $eventdata = $record->id;
                              //                        } else {
                              //                            $eventdata .= ',' . $record->id;
                              //                        }
                              //                    } $evntdata = array();
                              //
                              //                    if ($evntlist != '') {
                              //                        $evntdata = events_load_byname($evntlist);
                              //                    }
                              //
                              //
                              //
                              //                    $couponevents = array();
                              //                    $couponevents = getcouponevents($couponsave->id);
                              //                    if (count($couponevents) > 0) {
                              //                        $eventdta = db_update('coupon_event') // Table name no longer needs {}
                              //                                ->fields(array('Event_id' => $eventdata,
                              //                                    'coupon_eventtype' => 'individual'))
                              //                                ->condition('coupon_id', $couponsave->id, '=')
                              //                                ->execute();
                              //                    } else {
                              //                        $eventdta = db_insert('coupon_event')
                              //                                ->fields(array(
                              //                                    'coupon_id' => $couponsave->id,
                              //                                    'Event_id' => $eventdata,
                              //                                    'created' => REQUEST_TIME,
                              //                                    'coupon_eventtype' => 'individual'
                              //                                ))
                              //                                ->execute();
                              //                    }
                             * 
                             */
                        }//end of insertion
                    }

//                var_dump($failurecoupons);
//                var_dump($successcoupons);
                    /*     here we need to create the excel file send to logged users regarding success giftcards and failure giftcards     */
                    $filename = rand('1111111', '9999999') . '.xls';
                    $attachfilename = $filename;
                    $directory_path = file_stream_wrapper_get_instance_by_uri('public://')->getDirectoryPath() . '/couponexceluploads/';
                    $filename = $directory_path . $filename;
                    $sep = "\t";        //tabbed character
                    $fp = fopen($filename, 'w') or die("can't open file");

                    $schema_insert_rows = "";
                    //printing column names 

                    $schema_insert_rows.="#" . $sep;
                    $schema_insert_rows.="coupon code" . $sep;
                    $schema_insert_rows.="Status" . $sep;
                    $schema_insert_rows.="\n";
                    fwrite($fp, $schema_insert_rows);

// printing data:
                    $row = 0;
                    $i = 0;

                    for ($i = 0; $i < count($successcoupons); $i++) {
                        $schema_insert_rows = "";
                        $schema_insert_rows .= ( ++$row) . "" . $sep;
                        $schema_insert_rows .= $successcoupons[$i] . "" . $sep;
                        $schema_insert_rows .= "Success";
                        $schema_insert_rows .= "\n";
                        fwrite($fp, $schema_insert_rows);
                    }

                    for ($i = 0; $i < count($failurecoupons); $i++) {
                        $schema_insert_rows = "";
                        $schema_insert_rows .= ( ++$row) . "" . $sep;
                        $schema_insert_rows .= $failurecoupons[$i] . "" . $sep;
                        $schema_insert_rows .= "Failed";
                        $schema_insert_rows .= "\n";
                        fwrite($fp, $schema_insert_rows);
                    }
                    fclose($fp);


                    $frommail = variable_get('site_mail');

                    /*     Here we build the email system for sending attachment     */
                    $attachmentfile = $directory_path;
                    coupon_notify($frommail, $attachfilename, $coupontypes);




//            $path = 'admin/structure/coupon/manage/list';
                    if (arg(1) == 'coupon') {
                        $path = 'admin/structure/coupon/manage/list';
                    }
                    if (arg(1) == 'voucher') {
                        $path = 'admin/structure/voucher/manage/list';
                    }
                    drupal_goto($path);
                } else {
                    return FALSE;
                }
            } catch (Exception $e) {
                watchdog_exception('Inserting Records into coupon table Error', $e, NULL, WATCHDOG_ERROR);
                return FALSE;
            }
        }
    } else {
        drupal_set_message('Please Upload Excel File and Try Again.', $type = 'error');
//        drupal_set_message('Please Upload Excel File and Try Again.');
    }
}

function coupon_notify($frommail, $filename, $coupontypes) {

    global $user;
    $tomail = $user->mail;

    if ($coupontypes == 'coupon') {
        $type = 'Discounts';
    } else if ($coupontypes == 'voucher') {
        $type = 'Vouchers';
    }

    $params['filename'] = $filename;
    // giftcard_mail()  will be called based on the first drupal_mail() parameter.
    drupal_mail('coupon', 'notice', $tomail, $frommail, $params);

    drupal_set_message($type . ' ' . 'Inserted and not Inserted List has been emailed to your Email Id');
}

function coupon_mail($key, &$message, $params) {
    global $user;

    $account = $user->name;
    $attachment = array(
        'filecontent' => file_get_contents(file_stream_wrapper_get_instance_by_uri('public://')->getDirectoryPath() . '/couponexceluploads/' . $params['filename']),
        'filename' => 'coupons Inserted and failed',
        'filemime' => 'application/xls',
    );

    switch ($key) {
        case 'notice':
            $langcode = $message['language']->language;
            $message['subject'] = 'coupons Data ' . $account;
            $message['body'][] = 'Inserted and falied coupons';
            $message['params']['attachments'][] = $attachment;
            break;
    }
}

function checkcouponexists($form, $form_state) {

    $couponcode = $form_state['values']['coupon_code'];
    $typeofdiscount = $form_state['values']['type'];
    $duplicatescoupons = couponexists($couponcode);

    if ($duplicatescoupons) {
        if ($typeofdiscount == 'coupon') {
            $couponstatus = "This coupon Exists please try with another coupon";
        } else if ($typeofdiscount == 'voucher') {
            $couponstatus = "This Voucher Exists please try with another Voucher";
        }
    }
    $commands[] = ajax_command_replace("#coupon_status_message", "<div id='coupon_status_message'>" . $couponstatus . "</div>");
    return array('#type' => 'ajax', '#commands' => $commands, 'couponcode' => $duplicatescoupons->coupon_code);
}

function cruise_check_events($form, $form_state) {
    $selectedcruise = $form_state['values']['cruisetype'];
    $couponid = $form_state['values']['couponid'];


    $selectedport = $form_state['values']['chooseport'];
    $selectedvessel = $form_state['values']['choosevessel'];
    $couponid = $form_state['values']['couponid'];
    $selectedstartdate = $form_state['values']['search_startdate'];
    $selectedenddate = $form_state['values']['search_enddate'];
    $selectedstartdatetime = '';
    if ($selectedstartdate != '') {
        $selectedstartdatetime = strtotime($selectedstartdate);
    }
    $selectedenddatetime = '';
    if ($selectedenddate != '') {
        $selectedenddatetime = strtotime($selectedenddate);
    }

    $crusieevent = getcruise_eventsbysearch($selectedcruise, $selectedport, $selectedvessel, $selectedstartdatetime, $selectedenddatetime);

//    $selectedcruise = $form_state['values']['cruisetype'];
//    $crusieevent = getcruise_eventsbycruiseid($selectedcruise);
//    if ($selectedcruise == '') {
////        $crusieevent = events_load();
//        $crusieevent = getallcruise_events();
//    }

    if ($form_state['values']['checkall'] == 1) {
        foreach ($crusieevent as $event) {
            $commands[] = ajax_command_invoke('#edit-events-' . $event->scheduledevent_id, 'attr', array('checked', TRUE));
        }
    } else {
        foreach ($crusieevent as $event) {
            $commands[] = ajax_command_invoke('#edit-events-' . $event->scheduledevent_id, 'attr', array('checked', FALSE));
        }
    }
    return array(
        '#type' => 'ajax',
        '#commands' => $commands
    );
}

function cruise_events($form, $form_state) {
    $_SESSION['messages'] = array();
    $selectedcruise = $form_state['values']['cruisetype'];
    $selectedport = $form_state['values']['chooseport'];
    $selectedvessel = $form_state['values']['choosevessel'];
    $couponid = $form_state['values']['couponid'];
    $selectedstartdate = $form_state['values']['search_startdate'];
    $selectedenddate = $form_state['values']['search_enddate'];
    $selectedstartdatetime = '';
    if ($selectedstartdate != '') {
        $selectedstartdate = $selectedstartdate . ' 00:00:00';
        $selectedstartdatetime = strtotime($selectedstartdate);
    }
    $selectedenddatetime = '';
    if ($selectedenddate != '') {
        $selectedenddate = $selectedenddate . ' 23:59:00';
        $selectedenddatetime = strtotime($selectedenddate);
    }
//    cruiseeventsdiv
    $couponevents = array();
    if ($couponid != '') {
        $couponevents = array();
        $couponevents = getcouponevents($couponid);
        if (isset($couponevents[0]->Event_id)) {
            $evntids = explode(',', $couponevents[0]->Event_id);
            $options_checked = events_load_byid($evntids);
        }
    }
//    $crusieevent = getcruise_eventsbycruiseid($selectedcruise);
    $crusieevent = getcruise_eventsbysearch($selectedcruise, $selectedport, $selectedvessel, $selectedstartdatetime, $selectedenddatetime);
    $options = array();
    $cruiseeventids = array();
    foreach ($crusieevent as $t) {
        $options[] = $t;
        $cruiseeventids[] = $t->scheduledevent_id;
    }
//    $allevents = events_load();
    $allevents = getallcruise_events();

    foreach ($allevents as $singleevent) {
//        if (!in_array($singleevent->id, $cruiseeventids) && $selectedcruise != '') {
//        if (!in_array($singleevent->scheduledevent_id, $cruiseeventids) && $selectedcruise != '') {
        if (!in_array($singleevent->scheduledevent_id, $cruiseeventids)) {
//            $commands[] = ajax_command_remove('#edit-events-' . $singleevent->id);
//            $commands[] = ajax_command_invoke('#edit-events-'.$singleevent->id, 'attr', array('display', 'none'));
//            $commands[] = ajax_command_invoke('#edit-events .form-item-events-1', 'attr', array('display','none'));
            $commands[] = ajax_command_css('#edit-events .form-item-events-' . $singleevent->scheduledevent_id, array('display' => 'none'));
            $commands[] = ajax_command_invoke('#edit-events-' . $singleevent->scheduledevent_id, 'attr', array('checked', FALSE));
        } else {
            $commands[] = ajax_command_css('#edit-events .form-item-events-' . $singleevent->scheduledevent_id, array('display' => 'block'));
        }
    }

    if ($selectedcruise == '') {
        $events = events_load();
        $options = array();
        foreach ($events as $event) {
            $options[] = $event;
//            $commands[] = ajax_command_invoke('#edit-events-'.$event->id, 'attr', array('checked', FALSE));            
        }
    }
    $retdata = '';

//    foreach ($options as $option) {
//        $scheduleddate = date('Y-m-d', $option->scheduleddate);
//        $crusieeventname = '';
//        $checckstatus = '';
//        if (in_array($option->id, $evntids)) {
//            $checckstatus = 'checked="checked"';
//        }
//        $crusieeventname = '<div class="pull-left chkbox"><div class="pull-left chkboxevent">' . $option->name . '</div><div class="pull-left chkboxdate">' . $scheduleddate . ' ' . $option->boardingtime . '</div><div class="pull-left chkboxdate">' . $scheduleddate . ' ' . $option->returntime . '</div> </div>';
//        $retdata.='<div class="form-type-checkbox form-item-events-1 form-item">
//  <label for="edit-events-' . $option->id . '" class="option checkbox control-label"> <input type="checkbox" ' . $checckstatus . ' class="form-checkbox" value="1" name="events[' . $option->id . ']" id="edit-events-' . $option->id . '">' . $crusieeventname . ' </label>
//
//</div>';
//    }
    $commands[] = ajax_command_invoke('#edit-checkall', 'attr', array('checked', FALSE));
//    $commands[] = ajax_command_replace("#edit-events", "<div id='edit-events'>" . $retdata . "</div>");


    return array(
        '#type' => 'ajax',
        '#commands' => $commands
    );
}

function groupon_form($form, &$form_state, $coupon) {
    drupal_add_css(drupal_get_path('module', 'coupon') . '/coupon.css');
    if (isset($coupon->id) && ($coupon->id != '')) {
        $formstatus = 'edit';
    } else {
        $formstatus = 'add';
    }
//    $formstatus = $coupon->id == '' ? "add" : "edit";
    $title = '';
    $vessels = getVesselsdropdown();
    if ($formstatus == 'add') {
        $createtype = $coupon->createtype;
        $title = 'Create New ' . $createtype;
        if ($createtype == 'coupon') {
            $title = 'Create New Discount';
        }
    }
    if ($formstatus == 'edit') {
        $coupon->createtype = $coupon->type;
        $createtype = $coupon->createtype;
        $title = 'Edit ' . $createtype;
    }
    $createtype = isset($coupon->createtype) ? $coupon->createtype : '';
    if (!($createtype == 'groupon' || $createtype == 'web' || $createtype == 'livingsocial')) {
        drupal_set_message('Page not found', $type = 'warning');
        $path = 'admin/structure/groupon/manage/list';
        drupal_goto($path);
    }
    $title = ucwords($title);
    drupal_set_title($title, $output = CHECK_PLAIN);


    $form['coupondivstart'] = array(
        '#type' => 'markup',
        '#markup' => '<div id="couponForm">',
    );


    $form['operation'] = array(
        '#type' => 'hidden',
        '#value' => $formstatus
    );

//    $auto_id = $coupon->id != '' ? $coupon->id : "";
    $auto_id = isset($coupon->id) ? $coupon->id : '';
    $form['auto_id'] = array(
        '#type' => 'hidden',
        '#value' => $auto_id
    );

    if ($formstatus == 'edit') {
        $satsus = TRUE;
    } else {
        $satsus = FALSE;
    }

    $type = array("groupon" => t('Groupon'), "livingsocial" => t("Living Social"), "web" => t('Web'));

    $coupon->coupontype = 'flatfee';
    $form['createtype'] = array(
        '#type' => 'textfield',
        '#prefix' => '<div style="display:none;>',
        '#suffix' => '</div>',
        '#value' => $createtype
    );


    $form['deal_name'] = array(
        '#type' => 'textfield',
        '#title' => t('Deal Name'),
        '#required' => TRUE,
        '#default_value' => isset($coupon->deal_name) ? $coupon->deal_name : '',
        '#disabled' => $satsus,
        '#attributes' => array('class' => array('inputfiledStyle inputfieldMedium')),
        '#prefix' => '<div class="row-fluid"><div class="span12" style="margin-left: 0px;>',
        '#suffix' => '</div></div>',
    );

    $form['deal_sell_price'] = array(
        '#title' => t('Deal Sell Price'),
        '#default_value' => isset($coupon->deal_sell_price) ? $coupon->deal_sell_price : '',
        '#type' => 'textfield',
        '#prefix' => '<div class="row-fluid"><div class="span12">',
        '#suffix' => '</div></div>',
    );
    $form['deal_desc'] = array(
        '#title' => t('Deal Description'),
        '#default_value' => isset($coupon->deal_desc) ? $coupon->deal_desc : '',
        '#type' => 'textarea',
        '#prefix' => '<div class="row-fluid"><div class="span12">',
        '#suffix' => '</div></div>',
    );
    $form['type'] = array(
        '#type' => 'radios',
        '#title' => t('Groupon Type'),
        '#options' => $type,
        '#default_value' => isset($coupon->type) ? $coupon->type : 'groupon',
//        '#title_display' => 'invisible',
        '#attributes' => array('class' => array('radio_buttons optional')),
        '#prefix' => '<div class="row-fluid"><div class="span12">',
        '#suffix' => '</div></div><div class="row-fluid"><div class="span12">',
    );

    $form['couponmessageWrapper'] = array(
        '#type' => 'markup',
        '#prefix' => '<div id="coupon_status_message">',
        '#suffix' => '</div>',
    );

    $coupontype = array("percentage" => t('Percentage'), "flatfee" => t('flat Fee'), "2 for 1" => t('2 for 1'));
    $form['coupontype'] = array(
        '#type' => 'radios',
        '#title' => t('Coupon Type'),
        '#options' => $coupontype,
        '#default_value' => isset($coupon->coupontype) ? $coupon->coupontype : 'percentage',
        '#attributes' => array('class' => array('radio_buttons optional')),
        '#prefix' => '<div class="row-fluid" style="display:none;"><div class="span12">',
        '#suffix' => '</div></div><div class="row-fluid"><div class="span12">',
        '#states' => array(
            'invisible' => array(// action to take.
                ':input[name="type"]' => array('value' => 'vouchercode'),
            ),
        ),
    );
    if ($coupon->coupontype == 'percentage') {
        $couponrate = isset($coupon->rate) ? $coupon->rate : '';
    }
    if ($coupon->coupontype == 'flatfee') {
        $couponflatrate = isset($coupon->rate) ? $coupon->rate : '';
    }
    if ($coupon->coupontype == '2 for 1') {
        $coupontwoforonerate = isset($coupon->rate) ? $coupon->rate : '';
    }

    $form['rate'] = array(
        '#title' => t('Rate'),
        '#type' => 'textfield',
//        '#required' => TRUE,
        '#default_value' => isset($couponrate) ? $couponrate : '',
        '#attributes' => array('class' => array('inputfiledStyle inputfieldMedium')),
//        '#prefix' => '<div class="row-fluid"><div class="span12">',
//        '#suffix' => '</div></div>',
        '#states' => array(
            'visible' => array(// action to take.
                ':input[name="coupontype"]' => array('value' => 'percentage'),
            ),
        ),
    );
    $form['flatfeerate'] = array(
        '#title' => t('Rate'),
        '#type' => 'textfield',
        '#default_value' => isset($couponflatrate) ? $couponflatrate : '',
        '#attributes' => array('class' => array('inputfiledStyle inputfieldMedium')),
//        '#prefix' => '<div class="row-fluid"><div class="span12">',
//        '#suffix' => '</div></div>',
        '#states' => array(
            'visible' => array(// action to take.
                ':input[name="coupontype"]' => array('value' => 'flatfee'),
            ),
        ),
    );
    $form['twoforonerate'] = array(
        '#title' => t('Max number of tikets to discount'),
        '#type' => 'hidden',
        '#default_value' => isset($coupontwoforonerate) ? $coupontwoforonerate : '',
        '#attributes' => array('class' => array('inputfiledStyle inputfieldMedium')),
//        '#prefix' => '<div class="row-fluid"><div class="span12">',
        '#states' => array(
            'visible' => array(// action to take.
                ':input[name="coupontype"]' => array('value' => '2 for 1'),
            ),
        ),
//        '#suffix' => '</div></div>',
    );


    $couponvalidatetype = array('sequential' => t('Sequential'), 'multiple' => t('Multidaterange'));

    $form['coupon_validatetype'] = array(
        '#type' => 'radios',
        '#title' => t('Coupon Validate Type'),
        '#default_value' => isset($coupon->coupon_validatetype) ? $coupon->coupon_validatetype : 'sequential',
        '#prefix' => '</div></div><div class="row-fluid"><div class="span12">',
        '#suffix' => '</div></div>',
        '#options' => $couponvalidatetype,
    );



    $form['couponstartenddate'] = array(
        '#type' => 'markup',
        '#markup' => 'Promotion Start/End Dates',
        '#prefix' => '<label>',
        '#suffix' => '</label><div class="row-fluid customRowpromo">',
    );

    $form['promo_startdate'] = array(
        '#id' => 'promo_start_date',
        '#type' => 'date_popup',
        '#date_format' => 'Y-m-d',
        '#title' => t('Start Date'),
        '#description' => '',
        '#attributes' => array('class' => array('dateFieldStyle dateFieldMedium')),
        '#default_value' => isset($coupon->promo_startdate) ? $coupon->promo_startdate : '',
//        '#prefix' => '<div class="row-fluid"><div class="span3">',
        '#prefix' => '<div class="span3">',
        '#suffix' => '</div>',
        '#states' => array(
            'visible' => array(// action to take.
                ':input[name="coupon_validatetype"]' => array('value' => 'sequential'),
            ),
        ),
    );
    $form['promo_enddate'] = array(
        '#id' => 'promo_end_date',
        '#type' => 'date_popup',
        '#date_format' => 'Y-m-d',
        '#title' => t('End Date'),
        '#default_value' => isset($coupon->promo_enddate) ? $coupon->promo_enddate : '',
        '#description' => '',
        '#attributes' => array('class' => array('dateFieldStyle dateFieldMedium')),
        '#prefix' => '<div class="span9">',
//        '#suffix' => '</div></div>',
        '#suffix' => '</div>',
        '#states' => array(
            'visible' => array(// action to take.
                ':input[name="coupon_validatetype"]' => array('value' => 'sequential'),
            ),
        ),
    );

    $sel1ecteddates = isset($coupon->coupon_discounteddates) ? $coupon->coupon_discounteddates : '';
    $dates = array();
    if ($sel1ecteddates != '') {
        $dates = explode(',', $sel1ecteddates);
    }
    $datestring = '';
    foreach ($dates as $date) {
        if ($datestring != '') {
            $datestring.=",'" . $date . "'";
        } else {
            $datestring = "'" . $date . "'";
        }
    }
    if ($datestring != '') {
        $datestring = '{ addDates: [' . $datestring . '] }';
    }
    $form['multidate'] = array(
        '#title' => t('Multiple dates'),
        '#id' => 'datePick',
        '#type' => 'textfield',
        '#value' => '',
        '#attributes' => array('class' => array('inputfiledStyle inputfieldMedium')),
        '#prefix' => '
            <script type="text/javascript"> 
            $(function() {
                $("#datePick").multiDatesPicker(' . $datestring . ');
            });
            </script></div><div class="row-fluid customRowpromo"><div class="span12">',
        '#suffix' => '</div>',
        '#states' => array(
            'visible' => array(// action to take.
                ':input[name="coupon_validatetype"]' => array('value' => 'multiple'),
            ),
        ),
    );

    $form['crusiestartenddate'] = array(
        '#type' => 'markup',
        '#markup' => 'Cruise Start/End Dates',
        '#prefix' => '</div><label>',
        '#suffix' => '</label>',
    );
    $form['crusie_startdate'] = array(
        '#id' => 'crusie_start_date',
        '#type' => 'date_popup',
        '#date_format' => 'Y-m-d',
//        '#required' => TRUE,
        '#title' => t('Start Date'),
        '#default_value' => isset($coupon->crusie_startdate) ? $coupon->crusie_startdate : '',
        '#description' => '',
        '#attributes' => array('class' => array('dateFieldStyle dateFieldMedium')),
        '#prefix' => '<div class="row-fluid"><div class="span3">',
        '#suffix' => '</div>',
    );

    $form['crusie_enddate'] = array(
        '#id' => 'crusie_end_date',
        '#type' => 'date_popup',
        '#date_format' => 'Y-m-d',
        '#title' => t('End Date'),
        '#default_value' => isset($coupon->crusie_enddate) ? $coupon->crusie_enddate : '',
        '#description' => '',
        '#attributes' => array('class' => array('dateFieldStyle dateFieldMedium')),
        '#prefix' => '<div class="span9">',
        '#suffix' => '</div></div>',
    );


    $form['couponactive'] = array(
        '#title' => t('Active'),
        '#default_value' => isset($coupon->couponactive) ? $coupon->couponactive : '',
        '#type' => 'checkbox',
        '#prefix' => '<div class="row-fluid"><div class="span12">',
        '#suffix' => '</div></div>',
    );

    $form['company'] = array(
        '#title' => t('Company'),
        '#default_value' => isset($coupon->company) ? $coupon->company : '',
        '#type' => 'textfield',
        '#prefix' => '<div class="row-fluid"><div class="span12">',
        '#suffix' => '</div></div>',
    );
    $form['comments'] = array(
        '#title' => t('Comments'),
        '#default_value' => isset($coupon->comments) ? $coupon->comments : '',
        '#type' => 'textarea',
        '#prefix' => '<div class="row-fluid"><div class="span12">',
        '#suffix' => '</div></div>',
    );


    $cruises = getallcruisedetails();
    $cruiseoptions = array();
    $cruiseoptions[''] = 'Select Cruise';
    foreach ($cruises as $cruise) {
        $cruiseoptions[$cruise->id] = $cruise->cruise_title;
    }
    $form['crusievent'] = array(
        '#type' => 'fieldset',
        '#title' => t('Cruise Tickets'),
        '#title_display' => 'invisible',
        '#collapsible' => TRUE, // Added
        '#collapsed' => FALSE, // Added
        '#prefix' => '<div class="row-fluid"><div class="span12">',
        '#suffix' => '</div></div>',
    );
    $form['crusievent']["deal_cruise_template"] = array(
        '#type' => 'select',
        '#title' => t('Cruise'),
        '#ajax' => array(
            'event' => 'change',
            'callback' => 'cruise_template_tickets',
            'wrapper' => 'crusievent',
            'method' => 'replace',
        ),
        '#options' => $cruiseoptions,
        '#default_value' => isset($coupon->deal_cruise_template) ? $coupon->deal_cruise_template : '',
        '#prefix' => '<div class="row-fluid"><div class="span12">',
        '#suffix' => '</div></div>',
    );
    $cruisetemplateid = isset($coupon->deal_cruise_template) ? $coupon->deal_cruise_template : '';
//    $cruisetemplateid=23;
    $cruisetickets = array();
    $cruiseticketsdata = getTicketslist();
    foreach ($cruiseticketsdata as $cruiseticketsrow) {
        $cruisetickets[$cruiseticketsrow->id] = $cruiseticketsrow->title;
    }

    $allticketsdata = getTicketslist();
    $alltickets = array();
    foreach ($allticketsdata as $ticketsrow) {
        $alltickets[] = $ticketsrow->id;
    }
    $cruisetemplatetickets = array();

    if ($cruisetemplateid != '') {
        $cruiseticketsdata = getselectedCruisetemplatetickets($cruisetemplateid);
        foreach ($cruiseticketsdata as $cruiseticketsrow) {
            if ($cruiseticketsrow->activestatus == '1') {
                $cruisetemplatetickets[] = $cruiseticketsrow->ticketid;
            }
        }
    }
    $grouponticketsdata = isset($coupon->deal_cruise_tickets) ? $coupon->deal_cruise_tickets : '';
    $groupontickets = array();
    if ($grouponticketsdata != '') {
        $groupontickets = explode(',', $grouponticketsdata);
    }
    $commands = array();
    $css = '';
    foreach ($alltickets as $singletickets) {
        if (!in_array($singletickets, $cruisetemplatetickets)) {
//            $commands[] = ajax_command_css('#edit-deal-cruise-tickets .form-item-deal-cruise-tickets-' . $singletickets, array('display' => 'none'));
            $css .= "$('#edit-deal-cruise-tickets .form-item-deal-cruise-tickets-" . $singletickets . "').css ('display', 'none' );";
//            $css .= '$("#edit-deal-cruise-tickets").css("display", "none");';
        }
    }
//    var_dump($alltickets);echo'<br/>';
//    var_dump($cruisetemplatetickets);echo'<br/>';
//    var_dump($css);
//    
//    exit();
//    }
//    $form['crusievent']['deal_cruise_tickets'] = array(
//        '#type' => 'radios',
//        '#title' => t('Type'),
//        '#options' => $cruisetickets,
//        '#default_value' => $groupontickets,
//        '#title_display' => 'invisible',
//        '#attributes' => array('class' => array('radio_buttons optional')),
//       '#prefix' => '<div class="row-fluid"><div class="span12"><div id="edit-deal-cruise-tickets">',
//        '#suffix' => '</div></div></div>',
//    );

    $form['crusievent']['deal_cruise_tickets'] = array(
        '#default_value' => array(),
//        '#required' => '0',
        '#key_type_toggled' => '0',
        '#weight' => '2',
        '#type' => 'checkboxes',
        '#options' => $cruisetickets,
        '#default_value' => $groupontickets,
        '#title' => t('Tickets'),
        '#prefix' => '<div class="row-fluid"><div class="span12"><div id="edit-deal-cruise-tickets">',
        '#suffix' => '</div></div></div>',
    );
    $data = '$("#edit-deal-cruise-tickets div label .form-checkbox").click(function() {
                $("#edit-deal-cruise-tickets div label .form-checkbox").each(function() {
                    $(this)[0].checked = false;});
                $(this)[0].checked = true;
            });';

    drupal_add_js('jQuery(document).ready(function () { '
            . $data . $css . '});', array('type' => 'inline', 'scope' => 'footer', 'weight' => 5));
    
    
    $form['crusievent']['downloadxls'] = array(
    		'#prefix' => '<div class="row-fluid">',
    		'#suffix' => '</div></div>',
    		'#markup' => '<a href="' . base_path() . 'sites/default/files/tmp/grouponcodes.xls" style="text-decoration:none; padding-right:30px; margin:auto - 0px;">Download Groupon Excel Template</a>',
    );
    
    $form['crusievent']['grouponcodeimport'] = array(
        '#title' => t('Import'),
        '#type' => 'managed_file',
        '#description' => t('The uploaded csv will be imported and temporarily saved.'),
        '#upload_location' => 'public://tmp/',
        '#upload_validators' => array(
            'file_validate_extensions' => array('xls xlsx'),
        ),
        '#prefix' => '<div class="row-fluid"><div class="span12">',
        '#suffix' => '</div></div>  ',
    );
    if ($formstatus == 'edit') {  

        drupal_add_css(drupal_get_path('module', 'coupon') . '/css/beautifuldata.css', array('group' => CSS_DEFAULT, 'type' => 'file'));
//        drupal_add_js(drupal_get_path('module', 'coupon') . '/js/beautiful-data.min.js');
        $grouponhtml = '';
        $grouponcodes = getgrouponcodesbydealid($coupon->id);
        $grouponhtml .= '<div id="grouponcodescontent" class="beautifulData"><table>
        <thead>
            <tr>   
                <th>Code</th>
                <th>Redeemed</th>
            </tr>
        </thead>
        <tbody>';
        foreach ($grouponcodes as $grouponcode) {
            $grouponhtml.='<tr><td><span class="pull-left grouponodelabel">' . $grouponcode->groupon_code . '</span></td>';
            if ($grouponcode->is_used == '0') {
                $grouponhtml.='<td><input type="checkbox" disabled="disabled" class="pull-left"></td></tr>';
            } else {
                $grouponhtml.='<td><input type="checkbox" checked="checked" class="pull-left" disabled="disabled"></td></tr>';
            }
        }
        $grouponhtml.='</tbody></table>';



        $grouphtmljs = 'function test(){}
                        var fileref=document.createElement("script");
                        fileref.setAttribute("type","text/javascript");
                        fileref.setAttribute("src", "' . base_path() . '/sites/all/modules/custom/coupon/js/beautiful-data.min.js");
                        document.getElementsByTagName("head")[0].appendChild(fileref);                      
                        $(window).bind("load", function() {
                                $("#grouponcodescontent").beautify({
                                    pageSize: 10,
                                    pagerSize: 3
                                });
                        });';
        drupal_add_js('jQuery(document).ready(function () { ' . $grouphtmljs . '});', array('type' => 'inline', 'scope' => 'footer', 'weight' => 5));
        $form['crusievent']['dealgrouponids'] = array(
//            '#type' => 'markup',
//            '#markup' => $grouponhtml,
            '#type' => 'fieldset',
            '#title' => t('Groupon Codes'),
            '#title_display' => 'invisible',
            '#collapsible' => TRUE, // Added
            '#collapsed' => FALSE, // Added
            '#prefix' => '<script>
                            function waitForjQuery() {
//                                alert("hyfhjgjkg");
                                
                            }</script>
                            <div class="row-fluid"><div class="span12">',
            '#suffix' => '</div></div>',
        );
        $form['crusievent']['dealgrouponids']['grouponids'] = array(
            '#type' => 'markup',
            '#markup' => $grouponhtml,
        );
    }


    field_attach_form('coupon', $coupon, $form, $form_state);
    $form['actions'] = array('#type' => 'actions');
    $form['actions']['submit'] = array(
        '#type' => 'submit',
        '#value' => t('Save'),
    );

    $url = 'admin/structure/groupon/manage/list';

    $form['actions']['cancel'] = array(
        '#type' => 'link',
        '#title' => t('Cancel'),
        '#href' => $url,
        '#attributes' => array('class' => array('btn inputfieldMedium')),
    );

    $form['coupondivend'] = array(
        '#type' => 'markup',
        '#markup' => '</div>',
    );
    return $form;
}

function groupon_form_submit($form, &$form_state) {

    $coupon = new stdClass();
    $coupon->coupon_code = '';
    $coupon->type = $form_state['values']['type'];
    $coupon->promo_startdate = $form_state['values']['promo_startdate'];
    $coupon->promo_enddate = $form_state['values']['promo_enddate'];
    $coupon->crusie_startdate = $form_state['values']['crusie_startdate'];
    $coupon->crusie_enddate = $form_state['values']['crusie_enddate'];
    $coupon->deal_name = $form_state['values']['deal_name'];
    $coupon->deal_desc = $form_state['values']['deal_desc'];
    $coupon->deal_sell_price = $form_state['values']['deal_sell_price'];

    $dealticketsarr = array();
    $dealticketsarr = isset($form_state['values']['deal_cruise_tickets']) ? $form_state['values']['deal_cruise_tickets'] : '';
    $dealtickets = array();
    $dealticketstr = '';
    if ($dealticketsarr != '') {
        foreach ($dealticketsarr as $dealticketsrow) {
            if ($dealticketsrow != 0) {
                $dealtickets[] = $dealticketsrow;
            }
        }
        $dealticketstr = implode(',', $dealtickets);
    }

    $coupon->deal_cruise_template = $form_state['values']['deal_cruise_template'];
    $coupon->deal_cruise_tickets = $dealticketstr;
    $coupon->coupontype = $form_state['values']['coupontype'];
    $coupon->rate = $form_state['values']['flatfeerate'];
    $coupon->couponactive = $form_state['values']['couponactive'];
    $coupon->id = $form_state['values']['auto_id'];
    $coupon->company = $form_state['values']['company'];
    $coupon->comments = $form_state['values']['comments'];
    $coupon->coupon_validatetype = $form_state['values']['coupon_validatetype'];

    $coupon->created = time();
    $coupon->coupon_discounteddates = isset($form_state['input']['multidate']) ? $form_state['input']['multidate'] : '';
    field_attach_submit('coupon', $coupon, $form, $form_state);
    $cruiseevents = array();
    $cruiseevents = $form_state['values']['events'];

    $coupon = coupon_save($coupon);

    if ($form_state['values'] ['grouponcodeimport'] != 0) {
        $excelFileName = db_query("SELECT uri FROM {file_managed} WHERE fid = :fid", array(
            ':fid' => $form_state['values'] ['grouponcodeimport'],
                ))->fetchField();

        if (file_exists($excelFileName)) {

            $objPHPExcel = PHPExcel_IOFactory::load($excelFileName);
            $objWorksheet = $objPHPExcel->setActiveSheetIndex(0);
            $highestRow = $objWorksheet->getHighestRow();
            $highestColumn = $objWorksheet->getHighestColumn();
            $headingsArray = $objWorksheet->rangeToArray('A1:' . $highestColumn . '1', null, true, true, true);
            $headingsArray = $headingsArray[1];
            $r = -1;
            $couponsdata = array();
            try {
                for ($row = 2; $row <= $highestRow; ++$row) {
                    $dataRow = $objWorksheet->rangeToArray('A' . $row . ':' . $highestColumn . $row, null, true, true, true);
                    if ((isset($dataRow[$row]['A'])) && ($dataRow[$row]['A'] > '')) {
                        ++$r;
                        foreach ($headingsArray as $columnKey => $columnHeading) {
                            $couponsdata[$r][$columnHeading] = $dataRow[$row][$columnKey];
                        }
                    }
                }
            } catch (Exception $e) {
                watchdog_exception('Reading Excel File Error', $e, NULL, WATCHDOG_ERROR);
                return FALSE;
            }

            if (count($couponsdata) > 0) {
                for ($i = 0; $i < count($couponsdata); $i++) {
                    if (isset($couponsdata[$i]['grouponcode']) && $couponsdata[$i]['grouponcode'] != '') {
                        /*  check the groupon code is existed or not  */
                        $isgrouonexists = array();
                        $isgrouonexists = getgroupondatabycode($couponsdata[$i]['grouponcode']);
                        $tt = count($isgrouonexists);
                        if (!count($isgrouonexists) > 0) {
                            $eventdta = db_insert('deal_grouponcodes')
                                    ->fields(array(
                                        'deal_coupon_id' => $coupon->id,
                                        'groupon_code' => $couponsdata[$i]['grouponcode'],
                                        'created' => REQUEST_TIME,
                                        'is_used' => '0'
                                    ))
                                    ->execute();
                        } else {
                            drupal_set_message($couponsdata[$i]['grouponcode'] . ' Already existed.', $type = 'error');
                        }
                    }
                }
            }
        }
    } else {
//        drupal_set_message('Please Upload Excel File and Try Again.', $type = 'error');
    }


    $operation = $form_state['values']['operation'];
    $msg = '';
    if ($operation == "add") {
        $msg = $coupon->type . ' Created Successfully.';
    } else {
        $msg = $coupon->type . ' Updated Successfully.';
    }
    $msg = ucwords($msg);
    drupal_set_message($msg);
    $form_state['redirect'] = 'admin/structure/groupon/manage/list';
}

function cruise_template_tickets($form, $form_state) {
    $_SESSION['messages'] = array();
    $cruisetemplateid = $form_state['values']['deal_cruise_template'];
    $couponid = $form_state['values']['auto_id'];
    $groupontickets = array();
    $grouponcoupontemplate = '';
    if ($couponid != '') {
        $coupon = coupon_load($couponid);
        $grouponticketsdata = isset($coupon->deal_cruise_tickets) ? $coupon->deal_cruise_tickets : '';
        $grouponcoupontemplate = isset($coupon->deal_cruise_template) ? $coupon->deal_cruise_template : '';
        if ($grouponticketsdata != '') {
            $groupontickets = explode(',', $grouponticketsdata);
        }
    }
    $allticketsdata = getTicketslist();
    $alltickets = array();
    foreach ($allticketsdata as $ticketsrow) {
        $alltickets[] = $ticketsrow->id;
    }
    $cruisetickets = array();
    if ($cruisetemplateid != '') {
        $cruiseticketsdata = getselectedCruisetemplatetickets($cruisetemplateid);

        foreach ($cruiseticketsdata as $cruiseticketsrow) {
            if ($cruiseticketsrow->activestatus == '1') {
                $cruisetickets[] = $cruiseticketsrow->ticketid;
            }
        }
    }
    $commands = array();
    foreach ($alltickets as $singletickets) {
        if (!in_array($singletickets, $cruisetickets)) {
            $commands[] = ajax_command_css('#edit-deal-cruise-tickets .form-item-deal-cruise-tickets-' . $singletickets, array('display' => 'none'));
            $commands[] = ajax_command_invoke('#edit-deal-cruise-tickets-' . $singletickets, 'attr', array('checked', FALSE));
        } else {
            $commands[] = ajax_command_css('#edit-deal-cruise-tickets .form-item-deal-cruise-tickets-' . $singletickets, array('display' => 'block'));
//            $commands[] = ajax_command_invoke('#edit-deal-cruise-tickets-' . $singletickets, 'attr', array('checked', FALSE));
            if (in_array($singletickets, $groupontickets) && $cruisetemplateid == $grouponcoupontemplate) {
                $commands[] = ajax_command_invoke('#edit-deal-cruise-tickets-' . $singletickets, 'attr', array('checked', TRUE));
            } else {
                $commands[] = ajax_command_invoke('#edit-deal-cruise-tickets-' . $singletickets, 'attr', array('checked', FALSE));
            }
        }
    }

    $commands[] = ajax_command_css('#edit-deal-cruise-tickets', array('display' => 'block'));
    return array(
        '#type' => 'ajax',
        '#commands' => $commands
    );
}

function groupon_deal_view($dealid) {
    return drupal_get_form('groupon_deal_view_form', $dealid);
}

function groupon_deal_view_form($form, &$form_state, $dealid) {
    drupal_add_js(drupal_get_path('module', 'coupon') . '/js/coupon.js');
    $groupondata = entity_load('coupon', $ids = FALSE, $conditions = array('id' => $dealid));
    $deal_title = isset($groupondata[$dealid]->deal_name) ? $groupondata[$dealid]->deal_name : '';
    $deal_desc = isset($groupondata[$dealid]->deal_desc) ? $groupondata[$dealid]->deal_desc : '';

    $form['deal_name'] = array(
        '#type' => 'markup',
        '#markup' => '<div class="row-fluid"><div class="span12">'
        . '<div class="control-label pull-left input-medium">Title :</div>'
        . '<div class="pull-left">' . $deal_title . '</div></div></div>',
    );
    $form['deal_desc'] = array(
        '#type' => 'markup',
        '#markup' => '<div class="row-fluid"><div class="span12">'
        . '<div class="control-label pull-left input-medium">Description :</div>'
        . '<div class="pull-left">' . $deal_desc . '</div></div></div>',
    );
    $form['addcode'] = array(
        '#type' => 'fieldset',
        '#title' => t('Add Code'),
        '#collapsible' => TRUE, // Added
        '#collapsed' => FALSE, // Added
        '#prefix' => '<div class="row-fluid"><div class="span12">',
        '#suffix' => '</div></div>',
    );
    $form['addcode']['code'] = array(
        '#title' => t('Code'),
        '#type' => 'textfield',
        '#attributes' => array('class' => array('inputfiledStyle inputfieldMedium')),
        '#prefix' => '<div class="row-fluid"><div class="span12">',
        '#suffix' => '</div></div>',
    );
    $form['addcode']['dealid'] = array(
        '#title' => t('dealid'),
        '#type' => 'textfield',
        '#default_value' => $dealid,
        '#prefix' => '<div style="display:none;">',
        '#suffix' => '</div>',
    );
    $form['addcode']['redeemed'] = array(
        '#title' => t('Redeemed'),
        '#type' => 'checkbox',
        '#attributes' => array('class' => array('inputfiledStyle inputfieldMedium')),
        '#prefix' => '<div class="row-fluid"><div class="span12">',
        '#suffix' => '</div></div>',
    );


//    $form['addcode']['actions'] = array('#type' => 'actions');
    $form['addcode']['submit'] = array(
        '#type' => 'submit',
        '#ajax' => array(
            'event' => 'click',
            'callback' => 'deal_add_code',
            'wrapper' => 'addcode',
            'method' => 'replace',
        ),
        '#value' => t('Save'),
    );

    $url = 'groupon/' . $dealid . '/view';
    $form['addcode']['cancel'] = array(
        '#type' => 'link',
        '#title' => t('Cancel'),
        '#href' => $url,
        '#attributes' => array('class' => array('btn inputfieldMedium')),
    );

    $form['#method'] = 'get';
    $form['grouponcode'] = array(
        '#title' => t('Code'),
        '#type' => 'textfield',
        '#default_value' => isset($_GET['grouponcode'])?$_GET['grouponcode']:'',
        '#attributes' => array('class' => array('inputfiledStyle inputfieldMedium')),
        '#prefix' => '<div class="row-fluid"><div class="span12"><div class="pull-left">',
        '#suffix' => '</div>',
    );

    $form['btn'] = array(
        '#type' => 'submit',
        '#value' => 'Search',
        '#prefix' => '<div class="pull-left" style="margin: 3.5%;">',
        '#suffix' => '</div></div></div>',
    );


    $output = '';

    $header = array(
        array(
            'data' => 'Code',
            'field' => 'groupon_code'
        ),
        array(
            'data' => 'Redeemed',
            'field' => 'is_used'
        ),
        array(
            'data' => 'Status',
        )
    );
    $grouponcode = isset($_GET ['grouponcode']) ? $_GET ['grouponcode'] : '';
    if (isset($_GET ['sort']) && isset($_GET ['order'])) {
        if ($_GET ['sort'] == 'asc')
            $sort = 'ASC';
        else
            $sort = 'DESC';

        switch ($_GET ['order']) {
            case 'Code' :
                $order = 'groupon_code';
                break;
            case 'Redeemed' :
                $order = 'is_used';
        }
    } else {
        $sort = 'ASC';
        $order = 'groupon_code';
    }

    $query = db_select("deal_grouponcodes", "s");
    $query->fields('s');
    $query->condition('s.deal_coupon_id', $dealid, '=');
    if ($grouponcode != '') {
        $query->condition('s.groupon_code', '%' . db_like($grouponcode) . '%', 'LIKE');
    }
    $query->orderBy($order, $sort);
    $query = $query->extend('TableSort')->extend('PagerDefault')->limit(10);
    $result = $query->execute()->fetchAll();

    $rows = array();
    if (count($result) > 0) {
        for ($i = 0; $i < count($result); $i ++) {
            $actions = '<a href="#" onclick="changestatus(' . $result [$i]->id . ')" class="button">Change</a>';
            $grouponhtml = '';
            if ($result[$i]->is_used == '0') {
                $grouponhtml = '<input type="checkbox" disabled="disabled" class="pull-left">';
            } else {
                $grouponhtml = '<input type="checkbox" checked="checked" class="pull-left" disabled="disabled">';
            }
            $rows [] = array(
                $result [$i]->groupon_code,
                $grouponhtml,
                $actions
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

function groupon_deal_view_form_validate($form, &$form_state) {
    if ($form_state['triggering_element']['#value'] == 'Search') {
        $form_state['rebuild'] = TRUE;
        return;
    }
}

function deal_add_code($form, &$form_state) {
    $dealid = isset($form_state['values']['dealid']) ? $form_state['values']['dealid'] : '';
    $dealcode = isset($form_state['values']['code']) ? $form_state['values']['code'] : '';
    $redeemed = isset($form_state['values']['redeemed']) ? $form_state['values']['redeemed'] : '';
    /*  check the groupon code is existed or not  */
    $isgrouonexists = array();
    $isgrouonexists = getgroupondatabycode($dealcode);

    $tt = count($isgrouonexists);
    $commands = array();
    if (!count($isgrouonexists) > 0 && $dealcode !='') {
        $eventdta = db_insert('deal_grouponcodes')
                        ->fields(array(
                            'deal_coupon_id' => $dealid,
                            'groupon_code' => $dealcode,
                            'created' => REQUEST_TIME,
                            'is_used' => $redeemed
                        ))->execute();
        drupal_set_message('Group on Code(' . $dealcode . ') Added successfully.');
    } else {
        if($dealcode == ''){
            drupal_set_message('Code is Required.', $type = 'error');
        }else{
            drupal_set_message($dealcode . 'Already existed.', $type = 'error');
        }        
    }

    $commands[] = ajax_command_invoke('#edit-redeemed', 'attr', array('checked', FALSE));
    $commands[] = ajax_command_invoke('#edit-code', 'attr', array('value', ''));
    $commands[] = array('command' => 'redirectlistview');
    return array(
        '#type' => 'ajax',
        '#commands' => $commands
    );
}

function grouponcode_changestatus() {
    $grouponcodeid = $_POST['grouponcodeid'];
    $grouponcodedata = array();
    $grouponcodedata = getgroupondatabyid($grouponcodeid);
    $status = 0;
    if (isset($grouponcodedata[0]->is_used) && $grouponcodedata[0]->is_used == '0') {
        $status = 1;
    }
    $deal_grouponcodes = db_update('deal_grouponcodes') // Table name no longer needs {}
            ->fields(array('is_used' => $status))
            ->condition('id', $grouponcodeid, '=')
            ->execute();
    drupal_set_message('Redeem status changed successfully');
    drupal_json_output('success');
}
