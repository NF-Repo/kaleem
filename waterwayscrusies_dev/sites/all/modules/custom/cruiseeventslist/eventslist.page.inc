<?php



function events_list_page(){
	$build['userfilterform']=drupal_get_form('events_filteration_form');
	$build['userslistform']=drupal_get_form('view_events_form');
	return $build;
}


//filter form
function events_filteration_form(){
	drupal_add_css ( drupal_get_path ( 'module', 'user_accounts' ) . '/user_accounts.css' );
	$query = db_select('cruise', 'c');
	$query->fields('c',array('id','cruise_title'));
	$query->condition('cruise_status', 1);
	$result = $query->execute();
	
	$cruisetitlearr=array();
	
	$cruisetitlearr['ALL']='ALL';
	foreach($result as $cruise){
		$cruisetitlearr[$cruise->id]=$cruise->cruise_title;
	}
	
	
	
	$form['search_by']=array(
			'#id'=>'search_by',
			'#type'=>'select',
			'#options'=>$cruisetitlearr,
			'#default_value'=>$_SESSION['search_by'],
			'#prefix' => '<div class="row-fluid"><div class="span8">',
			'#attributes'=>array('class'=>array('pull-left input-medium searchfielsmargin')),
			
	);
	
	
	$form ['go'] = array (
			'#type' => 'submit',
			'#value' => 'Find',
			'#suffix' => '</div>',
			'#attributes'=>array('class'=>array('pull-left searchfielsmargin')),
	);
	
	
	
	return $form;
}



function events_filteration_form_submit($form, &$form_state) {
	$_SESSION['search_by']=$form_state['values']['search_by'];
	$form_state['redirect'] = 'cruiseeventlist';
	return;
}

function view_events_form($form, &$form_state) {
	$output = '';
	$header = array(
			array('data' => 'Event Name'),
			array('data' => 'Cruise Title', 'field' => 'scheduleddate'),
			array('data' => 'Event Date', 'field' => 'cruise_title'),
			array('data' => 'Port'),
			array('data' => 'Vessel'),
			array('data' => 'Available Tickets'),
			array('data' => 'Actions'),
	);
	if (isset ( $_GET ['sort'] ) && isset ( $_GET ['order'] )) {
		if ($_GET ['sort'] == 'asc')
			$sort = 'ASC';
		else
			$sort = 'DESC';
		
		switch ($_GET ['order']) {
			case 'Event Date' :
				$order = 'scheduleddate';
				break;
			
			case 'Cruise Title' :
				$order = 'cruise_title';
				break;
		}
	} else {
		$sort = 'ASC';
		$order = 'id';
	}
	
$query = db_select('cruise', '');
$query->fields('scheduledevents', array('name', 'departuretime', 'returntime', 'boardingtime', 'availabletickets', 'soldtickets', 'windowseatinventory', 'availablewindowseats', 'port_id', 'vessel_id', 'ticketinventory', 'id','onsaleshoursprior'));
$query->fields('cruise', array('cruise_title', 'id'));
$query->fields('scheduledevent_dates', array('scheduleddate'));
$query->leftJoin('scheduledevents', '', 'scheduledevents.cruisetemplate_id = cruise.id');
$query->leftJoin('scheduledevent_dates', '', 'scheduledevent_dates.scheduledevent_id = scheduledevents.id');

if(isset($_SESSION['search_by'])){
	if($_SESSION['search_by']!="ALL"){
		$query->condition('cruise.id', $_SESSION['search_by']);
	}	
}

$query->condition('scheduledevents.name','','!=');
$query->orderBy ( $order, $sort );
$query = $query->extend('TableSort')->extend('PagerDefault')->limit(10);
$result = $query->execute()->fetchAll();

//drupal_json_output($result); exit();

	global $base_url;
	$rows = array();
	if (count($result) > 0) {
		for ($i = 0; $i < count($result); $i++) {
			$portData = loadportById($result[$i]->port_id);
			$portDetails=$portData['portname']." , ".$portData['portCity'];
			$vesselData=loadvesselById($result[$i]->vessel_id);
			$vesselDetails=$vesselData['vesselname'];
			$eventdate=date('Ymd',$result[$i]->scheduleddate);
			
			$availableTickets=$result[$i]->availabletickets;
			
			
			
			$eventYMD=date("Y-m-d",$result[$i]->scheduleddate); //2014-01-31
			$departuretime=$result[$i]->departuretime;
			$onsaleshoursprior=$result[$i]->onsaleshoursprior;  //2
			$eventDate=$eventYMD." ".$departuretime;
			$unixtmstmp=strtotime($eventDate);
			$eventDateRestricted=$unixtmstmp-($onsaleshoursprior*60*60);
			$currentTime=time();
			
			
			if($currentTime>$eventDateRestricted || $availableTickets==0 ){
				$links='<a target="_blank" href="' . $base_url . '/cruise/' . $result[$i]->cruise_id . '/schedule">Schedule</a>';
			}else{
				$links='<a target="_blank" href="' . $base_url . '/cruise/' . $result[$i]->cruise_id . '/schedule">Schedule</a> |
					<a  target="_blank" href="' . $base_url . '/booknow/date/' . $eventdate.'/item/'.$result[$i]->id.'">Book Now</a>';
			}
			
			
			
			
			$eventName=ucwords($result[$i]->name);
			$cruise=ucwords($result[$i]->cruise_title);
			$evnetDate=date('d-m-Y',$result[$i]->scheduleddate)." ".$result[$i]->departuretime;
			
			$rows[] = array(
					$eventName,
					$cruise,
					$evnetDate,
					$portDetails,
					$vesselDetails,
					$availableTickets,
					$links,
               
			);
		}
	}
	
	$output .= '<div class="">';
	$output .= '<div class="">';
	
	$output .= '</div>';
	$output .= theme_table(
			array(
					'header' => $header,
					'rows' => $rows,
					'attributes' => array('class' => array('')),
					'sticky' => true,
					'caption' => '',
					'colgroups' => array(),
					'empty' => t("No Records!"),
			)
	) . theme('pager');
	
	$output .= '</div>';
	
	$form['mkp']=array(
		'#type'=>'markup',
		'#markup'=>$output,
	);
	
	return $form;
	
	
}


