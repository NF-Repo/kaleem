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
function theme_bookingstock_availability_form($vars) {
  
  $form = $vars['form'];
  
  $cruiseid=$form['nid']['#value'];
  $month=$form['month']['#value'];
  $year=$form['year']['#value'];
    
  
  //loading the Event Days availadates of that month for particular cruise
  $cruisesload= getSingleCruiseEventsByMonth($month,$year,$cruiseid);
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
  drupal_add_css(drupal_get_path('module', 'bookingstock') .'/css/bookingstock_calendars.css');
  $output = '';
  
  $output .= '<div class="container"><div class="contentbox-inner" id="calendarStockUpdates">';

  $header = array(
    array('data' => t('Sunday'), 'class' => 'calendar-day-name'),
    array('data' => t('Monday'), 'class' => 'calendar-day-name'),
    array('data' => t('Tuesday'), 'class' => 'calendar-day-name'),
    array('data' => t('Wednesday'), 'class' => 'calendar-day-name'),
    array('data' => t('Thursday'), 'class' => 'calendar-day-name'),
    array('data' => t('Friday'), 'class' => 'calendar-day-name'),
    array('data' => t('Saturday'), 'class' => 'calendar-day-name')
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
    $stockqty='<span class="availabilityStock">'.$form['calendar'][$day]['cruisestock']['ticketsavailable']['#default_value'].' - Available</span>';
    $row[] = '<div class="calendar-day-number active">'. $day .'</div>
              <div class="availableWrap">'.$stockqty.'</div>
              <div class="calendar-edit-wrap">'.
              render($form['calendar'][$day]).
             '</div>';
  }
    
   else
   {
     $row[] = '<div class="calendar-day-number">'. $day .'</div>
              <div class="calendar-edit-wrap">'.
              '</div>';
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
  
  

  //add disclaimer
  $output .= '<p>'. t('Dates older than the configured expiry time, or without a configured base rate value will be reset to 0 when this form is submit.') .'</p>';
  

  $output .= theme('table', array('header' => $header, 'rows' => $rows,
                                  'attributes' => array('class' => array('bookingstock-availability-edit'))));

 $output .= '</div></div>';
  return $output;
}

/**
 * Default implementation of theme_bookingstock_rates_form_calendar
*/
function theme_bookingstock_rates_form_calendar($vars) {
  $form = $vars['form'];
  drupal_add_css(drupal_get_path('module', 'bookingstock') .'/css/bookingstock_calendars.css');
  $output = '';

  $header = array(
    array('data' => t('Sunday'), 'class' => array('calendar-day-name')),
    array('data' => t('Monday'), 'class' => array('calendar-day-name')),
    array('data' => t('Tuesday'), 'class' => array('calendar-day-name')),
    array('data' => t('Wednesday'), 'class' => array('calendar-day-name')),
    array('data' => t('Thursday'), 'class' => array('calendar-day-name')),
    array('data' => t('Friday'), 'class' => array('calendar-day-name')),
    array('data' => t('Saturday'), 'class' => array('calendar-day-name'))
  );

  $rows = $row = array();
  $counter = 1;
  //start with our blanks
  $blank = (isset($form['#blank']) ? $form['#blank'] : 0);
  while ($blank > 0) {
    $row[] = '&nbsp;';
    $blank--;
    $counter++;
  }

  foreach (element_children($form) as $date) {
    //render the cell
    $row[] = '<div class="calendar-day-number">'. $form[$date]['#day'] .'</div>
              <div class="calendar-edit-wrap">'.
              render($form[$date])
              .'</div>';
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

  $output .= theme('table', array('header' => $header, 'rows' => $rows,
                                  'attributes' => array('class' => array('bookingstock-rate-edit'))));

  //render the rest of the form
  $output .= drupal_render_children($form);
  return $output;
}

/**
 * Default implementation of theme_bookingstock_calendars
 * @param $results
*/
function theme_bookingstock_calendars($vars) {
  $node = $vars['node'];
  $results = $vars['results'];
  $output = '<div id="bookingstock-calendars-outer">';

  foreach ($results as $year => $months) {
    $output .= theme('bookingstock_calendars_months', array('node' => $node,
                                                             'year' => $year,
                                                             'months' => $months));
  }

  $output .= theme('bookingstock_availability_key', array());
  $output .= "</div>";
  return $output;
}


/**
 * default implementation of theme_bookingstock_calendars_months
 * @param $year int year of calendar
 * @param $months array of months
 * @see theme_hotel_calendars_multi_results for format of months
 * @ingroup themeable
*/
function theme_bookingstock_calendars_months($vars) {
  $node = $vars['node'];
  $year = $vars['year'];
  $months = $vars['months'];
  $output = '';
  foreach ($months as $month => $days) {
    $output .= theme('bookingstock_calendars_month', array('node' => $node,
                                                            'year' => $year,
                                                            'month' => $month,
                                                            'days' => $days));
  }
  return $output;
}

/**
 * default implementation of theme_hotel_calendars_month
 * @param $year int year
 * @param $month int month
 * @param $days array of day data
 * @see bookingstock_calendars_node for format of days data
 * @ingroup themeable
*/
function theme_bookingstock_calendars_month($vars) {
  $node = $vars['node'];
  $year = $vars['year'];
  $month = $vars['month'];
  $days = $vars['days'];
  static $count;
  // Generate the first day of the month
  $first_day = mktime(0, 0, 0, $month, 1, $year);
  $title = t(date('F', $first_day), array(), array('context' => 'month_name'));

  // Find out what day of the week the first day of the month falls on
  $day_of_week = date('D', $first_day);
  $blank = date('w', $first_day);

  $end_day = mktime(0, 0, 0, $month + 1, 0, $year); //last day of previous month
  $days_in_month = date('j', $end_day);

  $show_prices = variable_get('bookingstock_display_calprices', TRUE);

  drupal_add_css(drupal_get_path('module', 'bookingstock') .'/css/bookingstock_calendars.css');
  $class = "bookingstock-calendar-". ($count + 1);
  if (($count + 1) % 4 == 0 && !$show_prices) {
    //add 'fourth' class to every fourth for themers
    $class .= ' bookingstock-calendar-fourth';
  }
  if (($count + 1) % 2 == 0 && $show_prices) {
    //add 'second' class to every second for themers when showing prices
    $class .= ' bookingstock-calendar-second';
  }


  $count++;
  $output = '<div class="bookingstock-calendar-outer '. $class .' bookingstock-calendar-with'.
              ($show_prices ? '' : 'out')
              .'-prices"><div class="bookingstock-calendar-month">'.
              $title .' '. $year;
  if (($menu_item = menu_get_item('node/'. $node->nid .'/availability/'. $year)) &&
      $menu_item['access']) {
    /*we do  it like this b/c other modules might want to
     override our access function using hook_menu_alter -
    doing this means we always check the menu_item so if
    it has been overriden, we get the overriden access callback*/
    $output .= ' '. l(t('edit'), 'node/'. $node->nid .'/availability/'. $year .'/'. $month);
  }
  $output .= '</div>';

  $header = array(
    array('data' => t('Sun'), 'class' => array('calendar-day-name')),
    array('data' => t('Mon'), 'class' => array('calendar-day-name')),
    array('data' => t('Tue'), 'class' => array('calendar-day-name')),
    array('data' => t('Wed'), 'class' => array('calendar-day-name')),
    array('data' => t('Thu'), 'class' => array('calendar-day-name')),
    array('data' => t('Fri'), 'class' => array('calendar-day-name')),
    array('data' => t('Sat'), 'class' => array('calendar-day-name'))
  );

  $rows = $row = array();
  $counter = 1;

  //start with our blanks
  while ($blank > 0) {
    $row[] = '&nbsp;';
    $blank--;
    $counter++;
  }

  // Set the first day of the month to 1
  $day_num = 1;

  // Count up the days, until the end of the month*/
  for ($i = 1; $i <= $days_in_month; $i ++) {
    $context = array(
      'subject' => array(
        'node' => $node
      ),
      'revision' => 'altered'
    );
    if (!isset($days[$i])) {
      continue;
    }
    $data = (array) $days[$i];
    //render the cell
    $data['day'] = $i;
    $data['id'] = 'bookingstock-cell-'. $node->nid .'-'. $year . $month . $i;

    if ($data['available'] > 0) {
      if ($data['minimum_occupancy'] > 1 ||
          $data['minimum_stay'] > 1 ||
          $data['no_check_in'] ||
          $data['no_check_out']) {
        $data['class'] = 'bookingstock-state-restricted';
        $data['restrictions'] = theme('bookingstock_calendar_cell_tip', array('day' => $data));
        if (module_exists('beautytips')) {
          $options[$data['id']] = array(
            'cssSelect' => '#'. $data['id'],
            'text' => $data['restrictions'],
          );
          beautytips_add_beautytips($options);
        }
      }
      else {
        $data['class'] = 'bookingstock-state-available';
      }
    }
    else {
      $data['class'] = 'bookingstock-state-unavailable';
    }
    if ($show_prices) {
      $data['rate'] = $data['rate'];
      $data['formatted_price'] = uc_currency_format($data['rate']);
      $data['price'] = theme('bookingstock_calendar_cell_price', array('day' => $data));
    }
    $row[] = '<div class="calendar-view-wrap">'.
              theme('bookingstock_calendar_cell', array('day' => $data))
              .'</div>';
    $counter++;

    // Make sure we start a new row every week
    if ($counter > 7) {
      $counter = 1;
      $rows[] = $row;
      $row = array();
    }
  }// Finaly we finish out the calendar with some blanks if needed
  while ($counter > 1 && $counter <= 7) {
    $row[] = '&nbsp;';
    $counter++;
  }
  //add the final row
  if (count($row)) {
    $rows[] = $row;
  }

  return $output .
          theme('table', array('header' => $header, 'rows' => $rows,
                               'attributes' => array('class' => array('bookingstock-calendar-view'))))
          .'</div>';
}
