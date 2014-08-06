<?php
function private_events_list_page() {
	drupal_set_title ( "Private Event Categories" );
	return drupal_get_form ( "private_events_list_form" );
}


function private_events_list_form_validate($form, &$form_state) {
if ($form_state['triggering_element']['#value'] == 'Go') {
        $form_state['rebuild'] = TRUE;
        return;
    }
	
}



function private_events_list_form($form, &$form_state) {
	global $base_url;
	
	$filterValue = isset($_GET['filteroptions']) ? $_GET['filteroptions'] : 'active';
	$form['#method'] = 'get';
	$form ['add_private_event'] = array (
			'#type' => 'markup',
			'#markup' => '<a href="' . $base_url . '/private-event/new" class="btn btn-primary">Add Private Event Category</a>' 
	);
	
	
	$form['filteroptions']=array(
			'#id'=>'filteroptions',
			'#type'=>'select',
			'#options'=>array('all'=>'All','active'=>'Active','inactive'=>'Inactive'),
			"#limit_validation_errors" => array(),
			'#default_value'=>$filterValue,
	);
	
	$form['btn'] = array(
			'#type' => 'submit',
			'#value' => 'Go',
			 
	);
	
	
	
	$output = '';
	
	$header = array (
			array (
					'data' => 'ID' 
			// 'field' => 'id'
						),
			array (
					'data' => 'Name' 
			// 'field' => 'names'
						),
			
			array (
					'data' => 'Actions' 
			)
			 
	)
	;
	$sort = 'DESC';
	if (isset ( $_GET ['sort'] ) && isset ( $_GET ['order'] )) {
		if ($_GET ['sort'] == 'asc')
			$sort = 'ASC';
		else
			$sort = 'DESC';
		
		switch ($_GET ['order']) {
			case 'ID' :
				$order = 'eventId';
				break;
		}
	} else {
		$sort = 'DESC';
		$order = 'eventId';
	}
	
	$query = db_select ( "private_events", "p" );
	$query->fields ( 'p');
	$query->orderBy ( $order, $sort );
	
	if($filterValue=="active"){
		$query->condition('status',1,'=');
	}
	else if($filterValue=="inactive"){
		$query->condition('status',0,'=');
	}
	
	
	$query = $query->extend ( 'TableSort' )->extend ( 'PagerDefault' )->limit ( 10 );
	$result = $query->execute ()->fetchAll ();
	
	$rows=array();
	
	// var_dump($result); exit();'<a href="' . $base_url . '/private-events/' . $result [$i]->id . '/manage
	if (count ( $result ) > 0) {
		for($i = 0; $i < count ( $result ); $i ++) {
			
			$status=$result [$i]->status;
			if($status==1){
				$statusHtml='<a title="Hide" href="' . $base_url . '/private-event/' . $result [$i]->eventId . '/status?action=hide"><span class="icon-eye-open"></span></a>  |';
			}
			
			if($status==0){
				$statusHtml='<a title="Show" href="' . $base_url . '/private-event/' . $result [$i]->eventId . '/status?action=show"><span class="icon-eye-close"></span></a>  |';
			}
			
			$rows [] = array (
					$result [$i]->eventId,
					$result [$i]->eventName,
					'<a title="Edit" href="' . $base_url . '/private-event/' . $result [$i]->eventId . '/manage"><span class="icon-edit"></span> </a> | '.$statusHtml
                	  
			);
					//<a href="' . $base_url . '/private-event/' . $result [$i]->eventId . '/delete"><span class="icon-trash"></span>  </a>'
		}
	}
	$output .= theme_table ( array (
			'header' => $header,
			'rows' => $rows,
			'attributes' => array (
					'class' => array (
							'table table-bordered' 
					) 
			),
			'sticky' => true,
			'caption' => '',
			'colgroups' => array (),
			'empty' => t ( "No Records!" ) 
	) ) . theme ( 'pager' );
	
	$form ['outputmarkup'] = array (
			'#type' => 'markup',
			'#markup' => $output 
	);
	return $form;
}