<?php

/*
 * @file hotel_booking.calendars.inc
 * Provides availability functions for hotel booking
 * @copyright Copyright(c) 2010 Lee Rowlands
 * @license GPL v2 http://www.fsf.org/licensing/licenses/gpl.html
 * @author Lee Rowlands leerowlands at rowlands-bcs dot com
 *
 */


/**
 * Cruise Availability Calendars page
 * @param $form_state array std fapi form state var
 * @param $node object node object
 * @param $year int year to display
 * @param $month int month to display
 */
function bookingstock_availability_form($form, $form_state, $node, $year = NULL, $month = NULL) {
  
   
  if (!$node || $node->type != 'product') {
    return drupal_not_found();
  }
  if ($node->tnid && $node->nid != $node->tnid) {
    drupal_set_message(t('This room is part of a translation set, availability can only be set against the
                         translation master, you have been redirected to the translation master to edit the availability'));
    drupal_goto('node/'. $node->tnid .'/availability');
  }
  //set up defaults
  if (!$year) {
    $year = date('Y');
  }
  if (!$month) {
    $month = date('m');
  }

  if (!is_numeric($year) || !is_numeric($month)) {
    return drupal_not_found();
  }

  $ref_dt = new DateObject("$year-$month-01", date_default_timezone(), DATE_FORMAT_ISO);

  drupal_set_title(t(
    'Availability for !name in !month-name '. $ref_dt->format('F') .' !year',
    array(
      '!name' => $node->title,
      '!year' => $year,
      '!month-name' => '')
    )
  );

  getCalendarstocks($month,$year);
  $form = array(
    '#theme' => 'bookingstock_availability_form'
  );

  $form['nid'] = array(
    '#type'  => 'value',
    '#value' => $node->nid,
  );
  $form['year'] = array(
    '#type'  => 'value',
    '#value' => $year,
  );
  $form['month'] = array(
    '#type'  => 'value',
    '#value' => $month,
  );
  

  $current_month = (int)date('m');
  $items = array();
  for ($i = 0; $i < 12; $i++ ) {
    $new_date = strtotime('+ '. $i .' months');
    $items['node/'. $node->nid .'/availability/'. date('Y', $new_date) .'/'.
           date('n', $new_date)] =
    t(date('F', $new_date), array(), array('context' => 'month_name')) .' '. date('Y', $new_date);
  }

  $items['node/'. $node->nid] = t('Return to node page');


//  $form = array(
//      '#type' => 'markup',
//      '#prefix' => '<div class="container"><div class="contentbox-inner" id="calendarStockUpdates">',
//      '#suffix' => '</div></div>',
//  );
  
  $form['goto'] = array(
    '#type' => 'select',
    '#title' => t('Choose the month'),
    '#default_value' => 'node/'. $node->nid .'/availability/'. $year .'/'. $month,
    '#options' => $items,
//    '#description' => t('Choose the month you wish to edit next.'),
      
  );

$form['calendar'] = bookingstock_calendars_day_fields($month, $year, $node);
  $form['submit'] = array(
    '#type'  => 'submit',
    '#value' => t('Apply'),
  );

  return $form;

}

/**
 * Hotel Calendars Month Table function
 * @param $month int month to render
 * @param $year int year to render
 * @param $node object hotel room type node object
 * @return form array
 */
function bookingstock_calendars_day_fields($month, $year, $node) {
  // Generate the first day of the month
  $first_day = mktime(0, 0, 0, $month, 1, $year);
  $break = mktime(0, 0, 0, $month + 1, 1, $year);//break the month for end day
 
  
  

  // Find out what day of the week the first day of the month falls on
  $day_of_week = date('D', $first_day);
  $blank = date('w', $first_day);

  $end_day = mktime(0, 0, 0, $month + 1, 0, $year); //last day of previous month
  $days_in_month = date('j', $end_day);
  
  //store these for the theme function
  $form = array(
    '#first_day' => $first_day,
    '#day_of_week' => $day_of_week,
    '#blank' => $blank,
    '#end_day' => $end_day,
    '#tree' => TRUE,
    '#days_in_month' => $days_in_month
  );

  // Build select box options for restrictions
  $stay_options = array(0 => t('None'));
  for ($i = 2; $i <= 6; $i++) {
    $stay_options[$i] = format_plural($i, '1 night', '@count nights');
  }

  $occupancy_options = array(0 => t('None'));
  for ($i = 2; $i <= $node->capacity; $i++) {
    $occupancy_options[$i] = format_plural($i, '1 guest', '@count guests');
  }

  $check_in = array(
    t('Check in OK'),
    t("Can't check in")
  );

  $check_out = array(
    t('Check out OK'),
    t("Can't check out")
  );

  // Gather up all existing availability info
  $calendarsdays=getstockcalendarrows($node->nid,$first_day,$end_day);
  $days=$calendarsdays['results'];

//var_dump($days->rowCount());
  
  //make assoc array of data existing availability
  $assoc = array();
  foreach ($days as $day) {
       $day = (array)$day;
       $dateTime = new DateTime($day['calendar_dt']); 
$ref_day=$dateTime->format('j'); 
$assoc[$ref_day] = $day;
    
  }
  
  $rows = $row = array();
  $counter = new DateObject(date(DATE_FORMAT_ISO, $first_day), date_default_timezone(), DATE_FORMAT_ISO);
  while ((int)$counter->format('U') < $break) {
    $ref_day = $counter->format('j');
    $existing = (isset($assoc[$ref_day]) ? $assoc[$ref_day] : FALSE);
    $form[$ref_day] = array(
      '#tree' => TRUE,
        'cruisestock' => array(
        '#type'         => 'fieldset',
        '#collapsible'  => TRUE,
        '#title'        => t('Stockupdates'),
        '#collapsed'    => TRUE,
        'minimums_caption' => array(
          '#markup' => '<div>'. t('Stock Updates') .'</div>'
        ),
      
              'ticketsavailable' => array(
          '#type' => 'textfield',
            '#title' => t('Available'),
             '#size' =>4,
          '#default_value' => $existing['available'],
//          '#options' => $check_in
        ),
            
        'soldtickets' => array(
          '#type' => 'textfield',
            '#title' => t('Sold Tickets'),
             '#size' =>4,
          '#default_value' => $existing['cruisesold'],
//          '#options' => $check_in
        ),
        'totaltickets' => array(
          '#type' => 'textfield',
             '#title' => t('Total Tickets'),
            '#size' =>4,
          '#default_value' => $existing['totalstockavailability'],
//          '#options' => $check_out
        ),
      )
    );

    //loop to next day
    $counter->modify('+ 1 day');
  }

  return $form;
}


/**
 *Submit handler for availability editing
*/
function bookingstock_availability_form_submit($form, &$form_state) {

  $values = $form_state['values'];
  $nid=$form_state['values']['nid'];
//  var_dump($values);
  foreach ($values['calendar'] as $day => $record)
  {
    $ref_date = mktime(0, 0, 0, $values['month'], $day, $values['year']);
//    var_dump($day);
//    var_dump($record['cruisestock']);
//    var_dump($record['cruisestock']['totaltickets']);
//    var_dump($ref_date);
    $calendardate=date('Y-m-d  00:00:00',$ref_date);
//    var_dump($calendardate);
//    var_dump($form_state['values']['nid']);
    //updating the records using nid and timstamp
 
    $bookingstock_updated = db_update('bookingstock') // Table name no longer needs {}
  ->fields(array(
    'available' =>$record['cruisestock']['ticketsavailable'],
    'totalstockavailability' =>$record['cruisestock']['totaltickets'],
      'cruisesold' =>$record['cruisestock']['soldtickets'],
  ))
  ->condition('calendar_dt', $calendardate, '=')
  ->condition('nid', $nid, '=')
  ->execute();
    
//    id	
//   nid
//  calendar_dt	
//    available	
//    totalstockavailability
//    cruisesold

    
  }

 drupal_set_message(t('Cruise Availability settings have been saved.'));

  $form_state['redirect'] = $form_state['values']['goto'];
}

/**
 * Display a list of node dates.
 *
 * @param $node object node object
 * @param $year int start year of calendars
 * @param $month int start month of calendars
 * @param $show int number of months to show
 * @return HTML output.
 */
function bookingstock_calendars_node($node, $year = FALSE, $month = FALSE, $show = FALSE) {
  //if we are looking at a translated version of a node, get
  //the availability from the translation master (tnid)
//  if ($node->tnid && $node->tnid != $node->nid) {
//    $translated = clone $node;
//    $node = node_load($node->tnid);
//  }
//  if (!$show) {
//    $show = variable_get('hotel_booking_display_monthcount', 12);
//  }
//  if (!$year) {
//    $year = (int)date('Y');
//  }
//  if (!$month) {
//    $month = (int)date('m');
//  }
//
//  $start_date = date('Y-m-d', mktime(0, 0, 0, $month, 1, $year));
//  $end_time = mktime(0, 0, 0, $month + $show, 0, $year);
//  $end_date = date('Y-m-d', $end_time); //zero for day gives last day
//
//  $where = $params = $results = array();
//  $query = "
//    SELECT
//      n.nid,
//      n.vid,
//      hbrt.hbrmid,
//      hbrt.hbrid,
//      hbrm.method,
//      hbrm.rate AS modifier_rate,
//      hbrc.calendar_dt,
//      hbrc.rate,
//      hbac.available,
//      hbac.minimum_occupancy,
//      hbac.minimum_stay,
//      hbac.no_check_in,
//      hbac.no_check_out
//    FROM {hotel_booking_room_types} hbrt
//      LEFT JOIN {hotel_booking_rate_modifiers} hbrm
//      ON hbrt.hbrmid = hbrm.hbrmid
//      INNER JOIN {hotel_booking_rate_calendars} hbrc
//      ON hbrc.hbrid = hbrt.hbrid
//      INNER JOIN {node} n
//      ON n.vid = hbrt.vid
//      LEFT JOIN {hotel_booking_availability_calendars} hbac
//      ON hbac.nid = n.nid
//      AND hbrc.calendar_dt = hbac.calendar_dt
//    WHERE (hbrc.calendar_dt BETWEEN :start AND :end OR hbrc.calendar_dt IS NULL)
//      AND n.type = 'hotel_room_type'
//      AND n.nid = :nid
//    ORDER BY hbrc.calendar_dt ";
//
//  $params = array(':start' => $start_date, ':end' => $end_date, ':nid' => $node->nid);
//
//  $res = db_query($query, $params);
//
//  //prepopulate the results so something shows if no availability or rates
//  $ref_dt = new DateObject($start_date, date_default_timezone(), DATE_FORMAT_ISO);
//  while ($ref_dt->format('U') <= $end_time) {
//    $results[$ref_dt->format('Y')][$ref_dt->format('n')] = array();
//    $ref_dt->modify('+1 months');
//  }
//
//  foreach ($res as $row) {
//    $date = new DateObject($row->calendar_dt, date_default_timezone(), DATE_FORMAT_DATETIME);
//    $rate = $row->rate;
//    if ($row->method == 'value') {
//      $rate += $row->modifier_rate;
//    }
//    else {
//      $rate += ($rate * $row->modifier_rate / 100);
//    }
//    $row->rate = $rate;
//
//    $results[$date->format('Y')][$date->format('n')][$date->format('j')] = $row;
//  }
//  return theme('hotel_booking_calendars', array('node' => $node, 'results' => $results));
}








/**
*
 * 
 * For the Stock Updates we need to handle different cases
 * Following process::We are updating the the stock information 
 * on monthly bases like whenever the admin access the product availability link
 * we are checking for monthly stock records if not available then we are inserting monthly records
 * (30/31) rows based on default uc stock cruise availability.
 * 
 * 
 * Case::1
 * Calendar display::
 * Whenever we are displaying the front-end monthly calendar for user then we need to
 * check for the default stock records availablity for selected month .if not available
 * for selected month then we have to insert the records for that month using the uc default stock options .
 * 
 * 
 * case::2
 * The stock updates need to be done whenever the order status get changed
 * using orderid we need to get track the purchased cruise information
 * based on those things we need to update the stock.
 * 
 * case 3::
 * Running the cron for removing the past date stock records to improve the system performance
 * 
 * 
 */

/**
 * calendar 
 * checking for the available nodes
 * Calling the stock availability function
 * if the stock available for that month then return the stock
 * or else insert the default stock
 * 
 * 
 * 
 * Get the List of Nodeids of that monthday
 * 
 */
function updatestockbycalendarmonth()
{
  
}


