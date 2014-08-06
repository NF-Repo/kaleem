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

function theme_orderedit_calendar_add_form($vars) {
  
  ctools_include('modal');
    ctools_modal_add_js();
    
  $form = $vars['form'];
  
//  var_dump($form['calendar']['#reservationdate']);
  $month=$form['calendar']['#currentmonth'];
  $year=$form['calendar']['#currentyear'];

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
    
      
      //laxmi due to dynamic part replacement we are not using drupal ajax
      //becuase of there are in not same form.
// $day = l($day, 'eventdatebased/ajax',
//         array('attributes' => array('class' => array('use-ajax'))));
//     
     $row[] = '<div class="calendar-day-number"><a onclick="checkEvents('.$day.','.$month.','.$year.')">'. $day.'</a></div>';
//              <div class="calendar-edit-wrap">'.
//              '</div>';
     unset($form['calendar'][$day]);
   
    
    
    
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
