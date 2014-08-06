<?php

function scheduledevents_add($cruisetemplateid) {

    $cruise = cruise_load($cruisetemplateid);
    $title = ucwords($cruise->cruise_title);
    drupal_set_title($title, $output = CHECK_PLAIN);

// $_SESSION['fromtemplateview']=true;
    $scheduledevents = (object) array(
                // fields
                'id' => '',
                'name' => '',
                'cruisetemplate_id' => $cruisetemplateid,
                'vessel_id' => '',
                'port_id' => '',
                'departuretime' => '',
                'returntime' => '',
                'boardingtime' => '',
                'availabletickets' => '',
                'soldtickets' => '',
                'availablewindowseats' => '',
                'windowseatssold' => '',
                'onsalesdaysprior' => '',
                'onsaleshoursprior' => ''
    );

    return drupal_get_form('scheduledevents_add_form', $scheduledevents);
}

/*
 * Schedule Event Form
 */

function scheduledevents_add_form($form, &$form_state, $scheduledevents) {


    /* Here we are maintain the 2 sections 
     *      1.Event form secion
     *      2.scheduled date sectio
     * Seperate the event form elements and scheduled date elemts
     * replace the hidden fields with the textfields because of the hidden fields are no having the id.
     * Based on the form element id we are replacing the values in edit section by using the ajax_command_invoke
     * 
     */
    if (!isset($form_state['page'])) {
        // if($_SESSION['flag']==false){
        if (isset($_SERVER['HTTP_REFERER'])) {
            $str = $_SERVER['HTTP_REFERER'];
            $t = explode('admin', $str);
            $schedulepagepath = 'admin' . @$t[1];
            $form_state['page']['flag'] = true;
            if ($schedulepagepath == "admin/structure/cruise/manage") {
                $form_state['page'] = "admin/structure/cruise/manage";
                //var_dump('tre');
            } else {
                //  var_dump('fa');
                $form_state['page'] = "cruiseeventlist";
            }
        }
        //}
    }
    drupal_add_css(drupal_get_path('module', 'scheduledevents') . '/scheduledevents.css', array('group' => CSS_DEFAULT, 'type' => 'file'));
    drupal_add_css(drupal_get_path('module', 'scheduledevents') . '/datepicker.css', array('group' => CSS_DEFAULT, 'type' => 'file'));
    drupal_add_css(drupal_get_path('module', 'scheduledevents') . '/mdp.css', array('group' => CSS_DEFAULT, 'type' => 'file'));
    drupal_add_js(drupal_get_path('module', 'scheduledevents') . '/scheduledevents.js');

    //getting the schudled event and dates information

    $results = getScheduledresults($scheduledevents->cruisetemplate_id);
    $schedulesresults = array();
    foreach ($results as $schedules) {
        $scheduledeventdate = date('m/d/Y', $schedules->scheduleddate);
        $departuretime = $schedules->departuretime;
        $boradingtime = $schedules->boradingtime;
        $returntime = $schedules->returntime;
        $schedulestr = $scheduledeventdate . '.... ' . $departuretime . ' . ' . $boradingtime . ' . ' . $returntime;
        $schedulesresults[$schedules->eventdatescheduledid] = $schedulestr;
    }


    if (count($schedulesresults) < 1) {
        $scheduledlist = array(
            '0' => 'No scheduled events');
    } else {
        $scheduledlist = $schedulesresults;
    }

    $formstatus = $scheduledevents->id == '' ? "add" : "edit";
    $form ['operation'] = array(
//        '#type' => 'hidden',
        '#type' => 'textfield',
        '#id' => 'operation',
        '#prefix' => '<div style="display:none;">',
        '#suffix' => '</div>',
        '#value' => $formstatus
    );

    $auto_id = $scheduledevents->id != '' ? $scheduledevents->id : "";
    $form ['auto_id'] = array(
//        '#type' => 'hidden',
        '#type' => 'textfield',
        '#id' => 'auto_id',
        '#prefix' => '<div style="display:none;">',
        '#suffix' => '</div>',
        '#value' => $auto_id
    );

    global $base_url;

//    $form['back'] = array(
//        '#type' => 'markup',
//        '#markup' => '<div> <a style="font-size:120%;" href="' . $GLOBALS['base_url'] . '/' . $form_state['page'] . '"> < Back </a> </div>',
//    );
    $form ['eventform'] = array(
        '#title' => t("User Information"),
        '#prefix' => '<div class="row-fluid"><div class="span6"><div id="eventformfields">',
        '#suffix' => '</div>',
    );


//    $form ['eventform'] ['markupstepone'] = array(
//        '#type' => 'markup',
//        '#markup' => '<div class="row-fluid"><div class="span6">'
//    );


    $form ['eventform']['name'] = array(
        '#type' => 'textfield',
        '#title' => t('Event Name'),
//        '#required' => TRUE,
        '#prefix' => '<div class="row-fluid"><div class="span12">',
        '#suffix' => '</div></div>',
        '#default_value' => isset($scheduledevents->name) ? $scheduledevents->name : ''
    );

    //loading the cruise templates


    $form['eventform']['status'] = array(
        '#type' => 'checkbox',
//        '#title' => t('Active'),
        '#prefix' => '<div class="row-fluid"><div class="span12"><div class="pull-left">',
        '#suffix' => '</div><div class="pull-left" style="padding:18px 0px 0px 10px">Active</div></div></div>',
        '#default_value' => isset($scheduledevents->status) ? $scheduledevents->status : '1'
    );

    $form ['eventform']['cruisetemplate_id'] = array(
//        '#type' => 'hidden',
        '#type' => 'textfield',
        '#prefix' => '<div style="display:none;">',
        '#suffix' => '</div>',
        '#id' => 'cruisetemplate_id',
        '#title' => t('Cruise Template ID'),
        //'#value' =>$_SESSION['cruisetemplateid'],
        '#value' => $scheduledevents->cruisetemplate_id,
    );


    //loading the available vessels form the fleet section
    //vessel seciton


    $vessels = getVesselsdropdown();
// 	var_dump($vessels);
    $form['eventform']['vessel_id'] = array(
        '#type' => 'select',
        '#title' => t('Vessel'),
        '#options' => $vessels,
        '#prefix' => '<div class="row-fluid"><div class="span12">',
        '#suffix' => '</div></div>',
        '#default_value' => isset($scheduledevents->vessel_id) ? $scheduledevents->vessel_id : '',
    );




    //end of vessel section
    //getting the portlist
    $portslist = getDropdownportslist();
// 	var_dump($portslist);


    $form['eventform']['port_id'] = array(
        '#type' => 'select',
        '#title' => t('Port'),
        '#options' => $portslist,
        '#prefix' => '<div class="row-fluid"><div class="span12">',
        '#suffix' => '</div></div>',
        '#default_value' => isset($scheduledevents->port_id) ? $scheduledevents->port_id : '',
    );



// 	$form ['port_id'] = array (
// 			'#type' => 'textfield',
// 			'#title' => t ( 'Port' ),
// 			'#default_value' => isset ( $scheduledevents->port_id ) ? $scheduledevents->port_id : '' 
// 	);


    $form['eventform']['boardingtime'] = array(
        '#type' => 'textfield',
        '#title' => t('Boarding Time'),
        '#required' => TRUE,
        '#prefix' => '<div class="row-fluid"><div class="span12">',
        '#suffix' => '</div></div>',
        '#default_value' => isset($scheduledevents->boardingtime) ? $scheduledevents->boardingtime : '',
    	'#attributes' =>array('placeholder' => t('4:00 PM'))
    );

    $form['eventform']['departuretime'] = array(
        '#type' => 'textfield',
        '#title' => t('Departure Time'),
        '#required' => TRUE,
        '#prefix' => '<div class="row-fluid"><div class="span12">',
        '#suffix' => '</div></div>',
        '#default_value' => isset($scheduledevents->departuretime) ? $scheduledevents->departuretime : '',
    	'#attributes' =>array('placeholder' => t('4:15 PM'))
    );

    $form ['eventform']['returntime'] = array(
        '#type' => 'textfield',
        '#title' => t('Return Time'),
        '#required' => TRUE,
        '#prefix' => '<div class="row-fluid"><div class="span12">',
        '#suffix' => '</div></div>',
        '#default_value' => isset($scheduledevents->returntime) ? $scheduledevents->returntime : '',
    	'#attributes' =>array('placeholder' => t('8:00 PM'))
    );


    $form ['eventform']['ticketinventory'] = array(
        '#type' => 'textfield',
        '#title' => t('Ticket Inventory'),
        '#required' => TRUE,
        '#prefix' => '<div class="row-fluid"><div class="span12">',
        '#suffix' => '</div></div>',
        '#default_value' => isset($scheduledevents->ticketinventory) ? $scheduledevents->ticketinventory : '',
    );


    $form ['eventform']['availabletickets'] = array(
        '#type' => 'textfield',
        '#title' => t('Ticket Available'),
        '#prefix' => '<div class="row-fluid"><div class="span12">',
        '#suffix' => '</div></div>',
        '#default_value' => isset($scheduledevents->availabletickets) ? $scheduledevents->availabletickets : '',
        '#attributes' => array('readonly' => array('readonly')),
    );

    $form ['eventform']['soldtickets'] = array(
        '#type' => 'textfield',
        '#title' => t('Tickets Sold'),
        '#prefix' => '<div class="row-fluid"><div class="span12">',
        '#suffix' => '</div></div>',
        '#default_value' => isset($scheduledevents->soldtickets) ? $scheduledevents->soldtickets : '',
        '#attributes' => array('readonly' => array('readonly')),
    );
    $form ['eventform']['windowseatinventory'] = array(
        '#type' => 'textfield',
        '#title' => t('Window Inventory'),
        '#required' => TRUE,
        '#prefix' => '<div class="row-fluid"><div class="span12">',
        '#suffix' => '</div></div>',
        '#default_value' => isset($scheduledevents->windowseatinventory) ? $scheduledevents->windowseatinventory : '',
    );
    $form ['eventform']['availablewindowseats'] = array(
        '#type' => 'textfield',
        '#prefix' => '<div class="row-fluid"><div class="span12">',
        '#suffix' => '</div></div>',
        '#title' => t('Window Available'),
        '#default_value' => isset($scheduledevents->availablewindowseats) ? $scheduledevents->availablewindowseats : '',
        '#attributes' => array('readonly' => array('readonly')),
    );

    $form ['eventform']['windowseatssold'] = array(
        '#type' => 'textfield',
        '#prefix' => '<div class="row-fluid"><div class="span12">',
        '#suffix' => '</div></div>',
        '#title' => t('Window Sold'),
        '#default_value' => isset($scheduledevents->windowseatssold) ? $scheduledevents->windowseatssold : '',
        '#attributes' => array('readonly' => array('readonly')),
    );

    /* $form ['eventform']['onsalesdaysprior'] = array (
      '#type' => 'textfield',
      '#title' => t ( 'onsalesdaysprior' ),
      '#default_value' => isset ( $scheduledevents->onsalesdaysprior ) ? $scheduledevents->onsalesdaysprior : ''
      ); */

    $form ['eventform']['onsaleshoursprior'] = array(
        '#type' => 'textfield',
        '#title' => t('Cut Off Sale (in hours)'),
        '#prefix' => '<div class="row-fluid"><div class="span12">',
        '#suffix' => '</div></div>',
        '#default_value' => isset($scheduledevents->onsaleshoursprior) ? $scheduledevents->onsaleshoursprior : '',
        '#description' => '<span class="help-inline">Time to cut off sales</span>',
    );
    $form['eventform']['addtoscheduled'] = array(
        '#type' => 'button',
        '#value' => t('Update Details'),
        '#ajax' => array(
            'callback' => 'ajax_dataupdate',
        ),
        '#prefix' => '<div class="row-fluid"><div class="updateDetails span12">',
        '#suffix' => '</div></div></div>',
    );


    $form ['eventform']['eventid'] = array(
//        '#type' => 'hidden',
        '#type' => 'textfield',
        '#prefix' => '<div style="display:none;">',
        '#suffix' => '</div>',
        '#id' => 'eventid',
        '#default_value' => ''
    );

//    $form['eventform']['markupsteptwo'] = array(
//        '#type' => 'markup',
//        '#markup' => '</div><div class="span6">'
//    );


    $form['scheduleeventsdates'] = array(
        '#type' => 'select',
        '#title' => t(''),
        '#options' => $scheduledlist,
        '#attributes' => array('multiple' => array('multiple')),
        '#default_value' => 0,
        '#ajax' => array(
            'callback' => 'ajax_scheduleddates',
            'wrapper' => 'eventformfields',
        ),
        '#prefix' => '<div class="span6"><div class="row-fluid"><div class="span12"><b>Schedule:</b> <span>Select date to load details</span>',
        '#suffix' => '</div></div>',
    );




    $form['removeschduled'] = array(
        '#type' => 'button',
        '#value' => t('Remove Selected'),
        '#ajax' => array(
            'callback' => 'ajax_removeschduleddate',
        ),
        '#prefix' => '<div class="row-fluid"><div class="span12">',
        '#suffix' => '</div></div>',
    );
    //laxmi
    drupal_add_library('system', 'ui.datepicker');
//    drupal_add_js('jQuery(document).ready(function(){       
//        $("#with-altField").multiDatesPicker({
//            altField: "#selectedscheduleddate",
//            addDates: [dates]
//        });});', 'inline'
//    );
    $form['selectedscheduleddatemarkup'] = array(
        '#type' => 'markup',
        '#prefix' => '<script type="text/javascript"> 


$(function() {
$("#with-altField").multiDatesPicker({
            altField: "#selectedscheduleddate",
        });});
  
</script><div class="row-fluid"><div class="span12">',
        '#suffix' => '</div></div>',
        '#markup' => '<label class="control-label" for="edit-boardingtime">Scheduled Dates: </label><div id="with-altField"></div>',
    );
    $form['selectedscheduleddate'] = array(
        '#type' => 'textarea',
        '#id' => 'selectedscheduleddate',
        '#title' => t(''),
        '#prefix' => '<div class="row-fluid"><div class="span12">',
        '#suffix' => '</div></div>',
//        '#prefix' => '<label class="control-label" for="edit-boardingtime">Scheduled Dates: </label><div id="with-altField"></div>',
        '#attributes' => array('class' => array('inputfiledStyle inputfieldMedium'), 'readonly' => 'readonly'),
    );

    //laxmi
//    $form['eventform']['selectedscheduleddate'] = array(
//        '#title' => t('Arrival date'),
//        '#type' => 'date_popup', // Provided by the date_popup module
//        '#date_format' => 'j F Y', // Uses the PHP date() format - http://php.net/manual/en/function.date.php
//        '#date_year_range' => '0:+2', // Limits the year range to the next two upcoming years
//        '#required' => TRUE,
//        '#default_value' => date('Y-m-d', time()), // Default value must be in 'Y-m-d' format.
//        '#title_display' => 'invisible'
//    );

    field_attach_form('scheduledevents', $scheduledevents, $form, $form_state);
    $form['submit'] = array(
        '#type' => 'submit',
        '#prefix' => '<div class="row-fluid"><div class="span12">',
        '#suffix' => '</div></div></div>',
        '#value' => t('Add to Schedule')
    );

//    $form['eventform']['markupstepthree'] = array(
//        '#type' => 'markup',
//        '#markup' => '</div></div>'
//    );
//
//    $form['eventform']['markupstepfour'] = array(
//        '#type' => 'markup',
//        '#markup' => '<div class="row-fluid"><div class="span12">'
//    );
//
//
//    $form['eventform']['markupstepfive'] = array(
//        '#type' => 'markup',
//        '#markup' => '</div></div>'
//    );



    return $form;
}

function ajax_dataupdate($form, &$form_state) {

    //Update button by default it would be disable state
    //once after the data saved then only we can add the scheduleddates



    $commands = array();

    $auto_id = $form_state['values']['auto_id'];
    $eventid = $form_state['values']['eventid'];
    $name = $form_state['values']['name'];
    $cruisetemplateid = $form_state['values']['cruisetemplate_id'];
    $vesselid = $form_state['values']['vessel_id'];
    $portid = $form_state['values']['port_id'];
    $departuretime = $form_state['values']['departuretime'];
    $returntime = $form_state['values']['returntime'];
    $boradingtime = $form_state['values']['boardingtime'];
    $ticketinventory = $form_state['values']['ticketinventory'];


    //$availabletickets=$form_state['values']['availabletickets'];
    $availabletickets = $form_state['values']['ticketinventory'];



    $solidtickets = $form_state['values']['soldtickets'];
    $windowseatinventory = $form_state['values']['windowseatinventory'];

    //$availablewindowseats=$form_state['values']['availablewindowseats'];
    $availablewindowseats = $form_state['values']['windowseatinventory'];


    $windowsold = $form_state['values']['windowseatssold'];
    $onsalepriordays = $form_state['values']['onsalesdaysprior'];
    $onslaepriorhours = $form_state['values']['onsaleshoursprior'];
    $scheduledeventsdates = $form_state['values']['scheduleeventsdates'];
    $selectedscheduleddate = $form_state['values']['selectedscheduleddate'];

    $eventstatus = $form_state['values']['status'];





    //creating the event object for data storage
    $scheduledeventObj = new Scheduledevents();

    $scheduledeventObj->id = $eventid;
    $scheduledeventObj->name = $name;
    $scheduledeventObj->cruisetemplate_id = $cruisetemplateid;
    $scheduledeventObj->vessel_id = $vesselid;
    $scheduledeventObj->port_id = $portid;
    $scheduledeventObj->departuretime = $departuretime;
    $scheduledeventObj->returntime = $returntime;
    $scheduledeventObj->boardingtime = $boradingtime;


    $scheduledeventObj->ticketinventory = $ticketinventory;
    $availTickets = $ticketinventory - $solidtickets;
    $scheduledeventObj->availabletickets = $availTickets;
    $scheduledeventObj->soldtickets = $solidtickets;

    $scheduledeventObj->windowseatinventory = $windowseatinventory;
    $availseats = $windowseatinventory - $windowsold;
    $scheduledeventObj->availablewindowseats = $availseats;
    $scheduledeventObj->windowseatssold = $windowsold;



    $scheduledeventObj->status = $eventstatus;


    $scheduledeventObj->onsalesdaysprior = $onsalepriordays;
    $scheduledeventObj->onsaleshoursprior = $onslaepriorhours;
    $scheduledeventObj->scheduleeventsdates = $scheduledeventsdates;
    $scheduledeventObj->selectedscheduleddate = $selectedscheduleddate;

// 	drupal_write_record('scheduledevents', $scheduledeventObj, array('id'));

    field_attach_submit('scheduledevents', $scheduledeventObj, $form, $form_state);
    $results = scheduledevents_save($scheduledeventObj);

    //Updating Date
    try {
        $query = db_update('scheduledevent_dates');
        $query->fields(array(
            'scheduleddate' => strtotime($form_state['values']['selectedscheduleddate']),
        ));
        $query->condition('scheduledevent_id', $form_state['values']['eventid'], '=');
        $num_updated = $query->execute();
    } catch (Exception $e) {
        
    }




// 	drupal_json_output($scheduledeventObj);exit();

    drupal_set_message('Event updated sucessfully');



    // Remove the old messages div, clearing existing messages.

    $commands[] = ajax_command_remove('div.messages');
    $commands[] = ajax_command_before('#main-content', theme('status_messages'));
    //$commands[] = ajax_command_invoke(NULL, "test", array('test') );
    //$commands[] = ajax_command_invoke(NULL, "test", array('test') );
    $myCustomParameters = $cruisetemplateid;
//    $commands[] = ajax_command_invoke(NULL, "reloadpage", array($myCustomParameters));
    $commands[] = array('command' => 'reloadafterdeleteevent');
    return array('#type' => 'ajax', '#commands' => $commands);
}

function ajax_removeschduleddate($form, &$form_state) {
    $commands = array();
// 	var_dump($form_state['values']);
// 	exit();

    /**
     * Functionality usage
     * we need to implement the event delete operation
     * from two dn tables scheduled events and scheduled event data
     * as well as what are the precations need to be taken in case of removed scheduled event action 
     * 
     * 
     */
    //delete scheduled events
    $eventid = $form_state['values']['eventid'];
    $event_deleted = db_delete('scheduledevents')
            ->condition('id', $eventid)
            ->execute();

    $eventdate_deleted = db_delete('scheduledevent_dates')
            ->condition('scheduledevent_id', $eventid)
            ->execute();


//    drupal_set_message('Records deleted sucessfully');
    // Remove the old messages div, clearing existing messages.
    $cruisetemplateid = $form_state['values']['cruisetemplate_id'];
    $myCustomParameters = $cruisetemplateid;
//    $commands[] = ajax_command_invoke(NULL, "reloadpage", array($myCustomParameters));
    $commands[] = ajax_command_remove('div.messages');
    $commands[] = ajax_command_before('#main-content', theme('status_messages'));
    $commands[] = array('command' => 'reloadafterdeleteevent');
    drupal_set_message('Event removed Successfully');
    return array('#type' => 'ajax', '#commands' => $commands);

// 	drupal_goto("scheduledevents/add");
// 	drupal_goto("cruise/".$cruisetemplateid."/schedule");
}

function ajax_scheduleddates($form, &$form_state) {


    $cruiseTemplateID = $form_state['values']['cruisetemplate_id'];
    $scheduled_date_id = $form_state['values']['scheduleeventsdates'];  //42

    $eventObject = false;
    try {
        $query = db_select('scheduledevent_dates', 'sd');
        $query->join('scheduledevents', 'se', 'sd.scheduledevent_id = se.id');
        $query->fields('sd', array('scheduleddate'));
        $query->fields('se', array('id', 'name', 'cruisetemplate_id', 'vessel_id', 'port_id', 'departuretime', 'returntime', 'boardingtime', 'ticketinventory', 'availabletickets', 'soldtickets', 'windowseatinventory', 'availablewindowseats', 'windowseatssold', 'onsalesdaysprior', 'onsaleshoursprior', 'nid', 'status'));
        $query->condition('sd.id', $scheduled_date_id, '=');
        $query->condition('se.cruisetemplate_id', $cruiseTemplateID, '=');
        $eventObject = $query->execute()->fetchObject();
    } catch (Exception $e) {
        watchdog("scheduleevents", $e);
    }




//    if ($eventObject) {
//
//        $form ['eventform'] ['eventid'] ['#value'] = $eventObject->id;
//        $form ['eventform'] ['name'] ['#value'] = $eventObject->name;
//        $form ['eventform'] ['cruisetemplate_id'] ['#value'] = $eventObject->cruisetemplate_id;
//        $form ['eventform'] ['vessel_id'] ['#value'] = $eventObject->vessel_id;
//        $form ['eventform'] ['port_id'] ['#value'] = $eventObject->port_id;
//        $form ['eventform'] ['boardingtime'] ['#value'] = $eventObject->boardingtime;
//        $form ['eventform'] ['departuretime'] ['#value'] = $eventObject->departuretime;
//        $form ['eventform'] ['returntime'] ['#value'] = $eventObject->returntime;
//        $form ['eventform'] ['ticketinventory'] ['#value'] = $eventObject->ticketinventory;
//        $form ['eventform'] ['availabletickets'] ['#value'] = $eventObject->availabletickets;
//        $form ['eventform'] ['soldtickets'] ['#value'] = $eventObject->soldtickets;
//        $form ['eventform'] ['windowseatinventory'] ['#value'] = $eventObject->windowseatinventory;
//        $form ['eventform'] ['availablewindowseats'] ['#value'] = $eventObject->availablewindowseats;
//        $form ['eventform'] ['windowseatssold'] ['#value'] = $eventObject->windowseatssold;
//        $form ['eventform'] ['onsalesdaysprior'] ['#value'] = $eventObject->onsalesdaysprior;
//        $form ['eventform'] ['onsaleshoursprior'] ['#value'] = $eventObject->onsaleshoursprior;
//
//        $dt = date('j F Y', $eventObject->scheduleddate);
//        $form ['eventform'] ['selectedscheduleddate']['date'] ['#value'] = $dt;
//
//        //selectedscheduleddate[date]
//    }

    $commands = array();
    if ($eventObject) {
        $dt = date('m/d/Y', $eventObject->scheduleddate);
        $commands[] = ajax_command_invoke('#eventid', 'attr', array('value', $eventObject->id));
        $commands[] = ajax_command_invoke('#edit-name', 'attr', array('value', $eventObject->name));
        $commands[] = ajax_command_invoke('#cruisetemplate_id', 'attr', array('value', $eventObject->cruisetemplate_id));
        $commands[] = ajax_command_invoke('#edit-vessel-id', 'attr', array('value', $eventObject->vessel_id));
        $commands[] = ajax_command_invoke('#edit-port-id', 'attr', array('value', $eventObject->port_id));
        $commands[] = ajax_command_invoke('#edit-boardingtime', 'attr', array('value', $eventObject->boardingtime));
        $commands[] = ajax_command_invoke('#edit-departuretime', 'attr', array('value', $eventObject->departuretime));
        $commands[] = ajax_command_invoke('#edit-returntime', 'attr', array('value', $eventObject->returntime));
        $commands[] = ajax_command_invoke('#edit-ticketinventory', 'attr', array('value', $eventObject->ticketinventory));
        $commands[] = ajax_command_invoke('#edit-availabletickets', 'attr', array('value', $eventObject->availabletickets));
        $commands[] = ajax_command_invoke('#edit-soldtickets', 'attr', array('value', $eventObject->soldtickets));
        $commands[] = ajax_command_invoke('#edit-windowseatinventory', 'attr', array('value', $eventObject->windowseatinventory));
        $commands[] = ajax_command_invoke('#edit-availablewindowseats', 'attr', array('value', $eventObject->availablewindowseats));
        $commands[] = ajax_command_invoke('#edit-windowseatssold', 'attr', array('value', $eventObject->windowseatssold));
// $commands[] = ajax_command_invoke('#edit-onsalesdaysprior', 'attr', array('value', $eventObject->onsalesdaysprior));
        $commands[] = ajax_command_invoke('#edit-onsaleshoursprior', 'attr', array('value', $eventObject->onsaleshoursprior));
        $commands[] = ajax_command_invoke('#selectedscheduleddate', 'attr', array('value', $dt));
        $commands[] = ajax_command_invoke('#edit-status', 'attr', array('checked', TRUE));
        if ($eventObject->status == 1) {
            $commands[] = ajax_command_invoke('#edit-status', 'attr', array('checked', TRUE));
        } else {
            $commands[] = ajax_command_invoke('#edit-status', 'attr', array('checked', FALSE));
        }
        drupal_add_js('jQuery(document).ready(function () { '
                        . 'test(); '
                        . '});', array('type' => 'inline', 'scope' => 'footer', 'weight' => 5));
//        $commands[] = ajax_command_invoke(NULL, "changescheduledate", array() );
       
        $commands[] = array('command' => 'changescheduledate');
    }

//    drupal_add_js('changecalenderdates(' . $dt . ');', 'inline');
    return array(
        '#type' => 'ajax',
        '#commands' => $commands
    );







// 	$commands = array();
// // 	var_dump($form_state['values']['scheduleeventsdates']);
// 	//loading the scheduled event entities
// 	$query = db_select('scheduledevent_dates','sd');
// 	// 	$query->join('scheduledevent_dates','e','s.id = e.scheduledevent_id');
// 	$query->fields('sd',array('scheduledevent_id'));
// 	// 	$query->fields('e');
// 	$query->condition('sd.id',$form_state['values']['scheduleeventsdates']);
// 	$eventid = $query->execute()->fetchField();
// 	//foreach ($result as $value) {
// 		$scheduledevent=entity_load('scheduledevents',$ids = FALSE,
//                         $conditions = array('id' =>$value->scheduledevent_id));
// 		$scheduleevents=$scheduledevent[$value->scheduledevent_id];
// 		$form ['eventform']['eventid']['#value']=$scheduleevents->id;
// 		$form ['eventform']['name']['#value']=$scheduleevents->name;
// 		$form ['eventform']['cruisetemplate_id']['#value']=$scheduleevents->cruisetemplate_id;
// 		$form['eventform']['vessel_id']['#value']=$scheduleevents->vessel_id;
// 		$form['eventform']['port_id']['#value']=$scheduleevents->port_id;
// 		$form['eventform']['boardingtime']['#value']=$scheduleevents->boardingtime;
// 		$form['eventform']['departuretime']['#value']=$scheduleevents->departuretime;
// 		$form ['eventform']['returntime']['#value']=$scheduleevents->returntime;
// 		$form ['eventform']['ticketinventory']['#value']=$scheduleevents->ticketinventory;
// 		$form ['eventform']['availabletickets']['#value']=$scheduleevents->availabletickets;
// 		$form ['eventform']['soldtickets']['#value']=$scheduleevents->soldtickets;
// 		$form ['eventform']['windowseatinventory']['#value']=$scheduleevents->windowseatinventory;
// 		$form ['eventform']['availablewindowseats']['#value']=$scheduleevents->availablewindowseats;
// 		$form ['eventform']['windowseatssold']['#value']=$scheduleevents->windowseatssold;
// 		$form ['eventform']['onsalesdaysprior']['#value']=$scheduleevents->onsalesdaysprior;
// 		$form ['eventform']['onsaleshoursprior']['#value']=$scheduleevents->onsaleshoursprior;
    // 		$credentials = entity_load('checkfrontadmin', $ids = FALSE, $conditions = array('id' => 1));
    //}
    //end of loading the scheduled events entity
// 	$form['eventform']['ticketinventory']['#value']=180;
// 	$form['eventform']['windowseatinventory']['#value']=150;
    return $form['eventform'];
// 	exit();
}

function scheduledevents_add_form_validate($form, &$form_state) {
    $scheduledevents = (object) $form_state ['values'];
    field_attach_form_validate('scheduledevents', $scheduledevents, $form, $form_state);
}

function scheduledevents_add_form_submit($form, &$form_state) {

    $selectedscheduleddates = $form_state['values']['selectedscheduleddate'];

    if ($selectedscheduleddates != '') {
        $selectedscheduleddatesarray = explode(',', $selectedscheduleddates);

        foreach ($selectedscheduleddatesarray as $selectedscheduleddate) {
            $auto_id = $form_state['values']['auto_id'];
            $name = $form_state['values']['name'];
            $cruisetemplateid = $form_state['values']['cruisetemplate_id'];
            $vesselid = $form_state['values']['vessel_id'];
            $portid = $form_state['values']['port_id'];
            $departuretime = $form_state['values']['departuretime'];
            $returntime = $form_state['values']['returntime'];
            $boradingtime = $form_state['values']['boardingtime'];
            $ticketinventory = $form_state['values']['ticketinventory'];
            $availabletickets = $form_state['values']['ticketinventory'];
//            $solidtickets = $form_state['values']['soldtickets'];
            $solidtickets = 0;
            $windowseatinventory = $form_state['values']['windowseatinventory'];
            $availablewindowseats = $form_state['values']['windowseatinventory'];
//            $windowsold = $form_state['values']['windowseatssold'];
            $windowsold = 0;
            $onsalepriordays = isset($form_state['values']['onsalesdaysprior']) ? $form_state['values']['onsalesdaysprior'] : '';
            $onslaepriorhours = $form_state['values']['onsaleshoursprior'];
            $scheduledeventsdates = $form_state['values']['scheduleeventsdates'];
            $scheduledeventsstatus=$form_state['values']['status'];
//    $selectedscheduleddate = $form_state['values']['selectedscheduleddate'];
            //creating the event object for data storage 
            $scheduledeventObj = new Scheduledevents();

            $scheduledeventObj->id = $auto_id;
            $scheduledeventObj->name = $name;
            $scheduledeventObj->cruisetemplate_id = $cruisetemplateid;
            $scheduledeventObj->vessel_id = $vesselid;
            $scheduledeventObj->port_id = $portid;
            $scheduledeventObj->departuretime = $departuretime;
            $scheduledeventObj->returntime = $returntime;
            $scheduledeventObj->boardingtime = $boradingtime;

            $scheduledeventObj->ticketinventory = $ticketinventory;

            $scheduledeventObj->availabletickets = $availabletickets;
            $scheduledeventObj->soldtickets = $solidtickets;

            $scheduledeventObj->windowseatinventory = $windowseatinventory;
            $scheduledeventObj->availablewindowseats = $availablewindowseats;
            $scheduledeventObj->windowseatssold = $windowsold;
            $scheduledeventObj->onsalesdaysprior = $onsalepriordays;
            $scheduledeventObj->onsaleshoursprior = $onslaepriorhours;
            $scheduledeventObj->scheduleeventsdates = $scheduledeventsdates;
            $scheduledeventObj->selectedscheduleddate = $selectedscheduleddate;
            $scheduledeventObj->status=$scheduledeventsstatus;
            
            $cruisescheduledata = array();
            $cruisescheduledata = getScheduledeventby_template_and_date($cruisetemplateid, $selectedscheduleddate);
            $isvalidevent = TRUE;
            /*   check whether the event has same vessel in the selected date   */
            foreach ($cruisescheduledata as $scheduledata) {
                if ($vesselid == $scheduledata->vessel_id) {

                    /*   check the date and time is available for this event or not   */
                    $existdate = date('M d Y', $scheduledata->scheduleddate);
                    $existboardingtime = $scheduledata->boardingtime;
                    $existreturntime = $scheduledata->returntime;
                    $existdeparturetime = $scheduledata->returntime;

                    $scheduleboardingtime = strtotime($existdate . ' ' . $existboardingtime);
                    $schedulereturntime = strtotime($existdate . ' ' . $existreturntime);
                    $scheduledeparturetime = strtotime($existdate . ' ' . $existdeparturetime);

                    /*   Current Event boarding time and return times   */
                    $currentboardingtime = strtotime($existdate . ' ' . $boradingtime);
                    $currentreturntime = strtotime($existdate . ' ' . $returntime);
                    $currentdeparturetime = strtotime($existdate . ' ' . $departuretime);


                    if ($currentboardingtime > $scheduleboardingtime && $currentboardingtime < $schedulereturntime) {
                        $isvalidevent = FALSE;
                    }
                    if ($currentreturntime > $scheduleboardingtime && $currentreturntime < $schedulereturntime) {
                        $isvalidevent = FALSE;
                    }
                    if ($currentdeparturetime > $scheduleboardingtime && $currentdeparturetime < $schedulereturntime) {
                        $isvalidevent = FALSE;
                    }


                    if ($scheduleboardingtime > $currentboardingtime && $scheduleboardingtime < $currentreturntime) {
                        $isvalidevent = FALSE;
                    }
                    if ($schedulereturntime > $currentboardingtime && $schedulereturntime < $currentreturntime) {
                        $isvalidevent = FALSE;
                    }
                    if ($scheduledeparturetime > $currentboardingtime && $scheduledeparturetime < $currentreturntime) {
                        $isvalidevent = FALSE;
                    }

//                    $isvalidevent=FALSE;
                }
            }
//            if ($isvalidevent) {
//                echo'Valid Event';
//            }else{
//                echo'Not a Valid Event';
//            }
//            var_dump($cruisescheduledata);
//            exit();
//            if (!count($cruisescheduledata) > 0) {
            if ($isvalidevent) {
                //once create the scheduled event we create the products as event
                //product creation gets here
                //creating the product using ubercart api
                require_once 'modules/node/node.pages.inc'; // Required for node_object_prepare();
                $node = new stdClass();
                $node->type = 'product';
                node_object_prepare($node); // This sets up all the default node fields so we don't accidentally leave something off.
                // Copy over all the existing settings from Drupal 5.
                $node->uid = 1;
                $node->status = 1; //by default we are using  nas active mode
                $node->title = $name; //should be unique
                $node->created = REQUEST_TIME;
                $node->changed = REQUEST_TIME;
                $node->language = 'und';
                //Set Ubercart variables
                //$node->model =E1001P; // the SKU is a required field, so I generated a SKU based on the node title
                $node->list_price = 0;
                $node->cost = 0;
                $node->sell_price = 0;
                $node->default_qty = 1;
                $node->pkg_qty = 1;

                // Set taxonomy + menu etc if you need to
                $node->taxonomy_catalog['und'][0]['tid'] = 235;
                // 	$node->menu = array();
// 	$filepath='C:/Dev/xampp/htdocs/works/dev/waterways/trunk/web/sites/default/files/pictures/test.jpg';
// 	// Create managed File object and associate with Image field.
// 	$file = new StdClass();
// 	$file->uid = 1;
// 	$file->uri = $filepath;
// 	$file->filemime = file_get_mimetype($file->uri);
// 	$file->status = 1;
// 	$dest = file_default_scheme() . '://pictures'; // Subdirectory name within files directory. ($dest is optional.)
// 	$file = file_copy($file, $dest);
// 	$node->uc_product_image['und']['0']=(array)$file;
                // Save the node
                node_save($node);  // This will update the $node object with the $node->nid which is important for the next step.
                //end product creating
                //end of product creation here
                $scheduledeventObj->nid = $node->nid;
                field_attach_submit('scheduledevents', $scheduledeventObj, $form, $form_state);
                $results = scheduledevents_save($scheduledeventObj);

                $selectedscheduleddatetime = strtotime($selectedscheduleddate);
                //inserting scheduled event dates
                $eventdates = array(
                    'scheduledevent_id' => $results->id,
//                'scheduleddate' => strtotime($scheduledeventObj->selectedscheduleddate),
                    'scheduleddate' => $selectedscheduleddatetime,
                );
                $datewithtime=$selectedscheduleddate.'  '.$departuretime.'  '.$boradingtime.'  '.$returntime;
                $eventdates = (object) $eventdates;
                $eventresults = scheduled_events_dates_save($eventdates);
                drupal_set_message('Event created Successfully on '.$datewithtime);
            } else {
                $datewithtime=$selectedscheduleddate.'  '.$departuretime.'  '.$boradingtime.'  '.$returntime;
                drupal_set_message('Event Already scheduled with same Vessel on '.$datewithtime,'warning');
            }
        }
    } else {
        
    }






    // $ticket_id = $form_state['values']['ticket_id'];
    // $order_id = $form_state['values']['order_id'];
    // $ticket_qty = $form_state['values']['ticket_qty'];
    // $auto_id = $form_state['values']['auto_id'];
    // $scheduledevents = new stdClass();
    // $scheduledevents->id = $auto_id;
    // $scheduledevents->ticket_id = $ticket_id;
    // $scheduledevents->order_id = $order_id;
    // $scheduledevents->ticket_qty = $ticket_qty;
    // field_attach_submit('scheduledevents', $scheduledevents, $form, $form_state);
    // $scheduledevents = scheduledevents_save($scheduledevents);
    // $operation = $form_state['values']['operation'];
    // $msg = '';
    // if ($operation == "add") {
    // $msg = 'Ticket Orders Created Successfully.';
    // } else {
    // $msg = 'Ticket Orders Updated Successfully.';
    // }
    // drupal_set_message($msg);
    // $form_state['redirect'] = 'admin/structure/scheduledevents/manage';
}

//scheduledevent_dates data





