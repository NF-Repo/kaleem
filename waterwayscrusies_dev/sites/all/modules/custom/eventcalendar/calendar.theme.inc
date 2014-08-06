<?php
/*
 * @file bookingstock.theme.inc
 * Provides theme functions for bookingstock module
 * @copyright Copyright(c) 2010 Lee Rowlands
 * @license GPL v2 http://www.fsf.org/licensing/licenses/gpl.html
 * @author Lee Rowlands leerowlands at rowlands-bcs dot com
 *
 */

/**
 * Default implementation of theme_bookingstock_availability_form
*/
function theme_calendar_add_form($vars) {
  
  ctools_include('modal');
    ctools_modal_add_js();
    
  $form = $vars['form'];
  
//  var_dump($form['calendar']['#reservationdate']);
  $month=$form['calendar']['#currentmonth'];
  $year=$form['calendar']['#currentyear'];
//  $cruise_event_id=$form['calendar']['#cruiseid'];
  //loading the Event Days availadates of that month for particular cruise event
  
  //loading the selected event other available dates using eventid in
  //scheduled_event_dates
  $cruisesload= getSingleCruiseEventsByMonth($month,$year,$cruise_event_id);
//  $eventdates=getMultipleEventDatesByeventId($cruise_event_id);
//  var_dump($eventdates);exit();
  
  $dates=array_keys($cruisesload);
  $cruiseeventweekdays_dates=array();
  foreach ($dates as $daydate) {
    $dttimestamp = strtotime($daydate);
    $daynumber=date('d',$dttimestamp);
    $cruiseeventweekdays_dates[]=(int)$daynumber;
  }
//  var_dump($eventdays);
//  $cruiseeventweekdays_dates=array(5,12,19,26,6,13,20,27);
  
//  var_dump(element_children($form['calendar']));
  drupal_add_css(drupal_get_path('module', 'eventcalendar') .'/eventcalendar.css');
  $output = '';
  
  $output .= '<div class="calendarWrapper" id="calendarStockUpdates">';

  $header = array(
    array('data' => t('Sun'), 'class' => 'calendar-day-name'),
    array('data' => t('Mon'), 'class' => 'calendar-day-name'),
    array('data' => t('Tue'), 'class' => 'calendar-day-name'),
    array('data' => t('Wed'), 'class' => 'calendar-day-name'),
    array('data' => t('Thu'), 'class' => 'calendar-day-name'),
    array('data' => t('Fri'), 'class' => 'calendar-day-name'),
    array('data' => t('Sat'), 'class' => 'calendar-day-name')
  );

  $rows = $row = array();
  $counter = 1;

  //start with our blanks
  $blank = $form['calendar']['#blank'];
  while ($blank > 0) {
    $row[] = '&nbsp;';
    $blank--;
    $counter++;
  }

  foreach (element_children($form['calendar']) as $day) {
    //render the cell
    
  if(in_array($day, $cruiseeventweekdays_dates))
  {
//   return _bctoolpopup_make_link('Magical modal');
    
    if($form['calendar']['#reservationdate']==$day)
      $class="highlightcell";
    else
      $class="";
    $row[] = '<div class="'.$class.'"><div id="magical-modal-link" class="calendar-day-number active">' . l($day, 'ctoolpopups/nojs',
            array('attributes' => array('class' => 'ctools-use-modal'))) . '</div>'.
               render($form['calendar'][$day]).
             '</div></div>';
  }
    
   else
   {
     
     $row[] = '<div class="calendar-day-number">'. $day .'</div>';
//              <div class="calendar-edit-wrap">'.
//              '</div>';
     unset($form['calendar'][$day]);
   }
    
    
    
    $counter++;

    // Make sure we start a new row every week
    if ($counter > 7) {
      $counter = 1;
      $rows[] = $row;
      $row = array();
    }
  }

  // Finaly we finish out the calendar with some blanks if needed
  while ($counter > 1 && $counter <= 7) {
    $row[] = '&nbsp;';
    $counter++;
  }
  //add the final row
  if (count($row)) {
    $rows[] = $row;
  }
  //laxmi todo need to render remaining form fields instead of evrythingd rendered
  $output .= drupal_render_children($form);//commntted by laxmi
   //render the rest of the form
  
  

  
  $output .= theme('table', array('header' => $header, 'rows' => $rows,
 'attributes' => array('class' => array('bookingstock-availability-edit'))));

 $output .= '</div>';
  return $output;
}
