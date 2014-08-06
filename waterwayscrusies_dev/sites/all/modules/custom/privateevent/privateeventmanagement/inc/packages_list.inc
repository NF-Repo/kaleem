<?php


function redirect_to_packages_list($packageId){
	
	$packageObject=PrivateEventPackages::getPrivateEventPackageDataByPackageId($packageId);
	
	//var_dump($data); exit();
	$path="private-events/packages/".$packageObject->eventId."/list";
	drupal_goto($path); exit();
// 	echo $packageId; exit();
}
function private_events_packages_list_page($eventId){
	//var_dump($packageId); exit();
	return drupal_get_form("private_events_packages_form",$eventId);
	
	
}

function private_events_packages_form_validate($form, &$form_state) {
	if ($form_state['triggering_element']['#value'] == 'Go') {
		$form_state['rebuild'] = TRUE;
		return;
	}

}


function private_events_packages_form($form, &$form_state,$eventId) {

	
	PrivateEventPackages::deleteUnusedPackages();
	$output = '';
	global $base_url;
	
	$privateEventMenus = PrivateEvents::getPrivateEventMenus($eventId );
	$default_private_event_fields=explode(',', $privateEventMenus->menusIds);
	
	$menuHtml=PrivateEventHeaderMenu::getHeaderMenu($default_private_event_fields,$eventId,'4');
	$form['menuHtml']=array(
			'#type'=>'markup',
			'#markup'=>$menuHtml,
	);
	
	$form['add_link'] = array(
			'#type' => 'markup',
			'#markup' =>'<a href="'.$base_url.'/private-event/packages/'.$eventId.'/new" class="btn btn-primary">Add New Package</a>
			<a target="blank" href="'.$base_url.'/private-events/packagescategories?pageAction=view" class="btn">Package Categories</a>
			',
	);
	
	
	$filterValue = isset($_GET['filteroptions']) ? $_GET['filteroptions'] : 'active';
	$form['#method'] = 'get';
	
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
	 
	
	$header = array(
			array(
					'data' => 'Package Name',
					//'field' => 'names'
			),
			
			array(
					'data' => 'Package Category',
					//'field' => 'names'
			),
			array(
					'data' => 'Actions',
					//'field' => 'names'
			),
	
			 
	);
	$sort = 'DESC';
	$order = 'eventId';
	
// 	$query = db_select("private_event_packages", "p");
// 	$query->fields('p');
// 	$query->orderBy($order, $sort);
// 	//$query = $query->extend('TableSort')->extend('PagerDefault')->limit(20);
// 	$query->condition('eventId',$eventId, '=');
// 	$result = $query->execute()->fetchAll();
	
	
	$query = db_select('private_event_packages', '');
	$query->fields('private_event_package_categories', array('packageCategoryTitle'));
	$query->fields('private_event_packages');
	$query->innerJoin('private_event_package_categories', '', 'private_event_package_categories.packageCategoryId = private_event_packages.packageCategoryId');
	$query->condition('eventId',$eventId, '=');
	
	if($filterValue=="active"){
		$query->condition('packageStatus',1,'=');
	}
	else if($filterValue=="inactive"){
		$query->condition('packageStatus',0,'=');
	}
	
	
	$result = $query->execute()->fetchAll();
	
	

	$rows=array();
	if (count($result) > 0) {
		for ($i = 0; $i < count($result); $i ++) {
			
			
			$status=$result [$i]->packageStatus;
			if($status==1){
				$statusHtml='<a title="Hide" href="' . $base_url . '/private-event/package/' . $result [$i]->packageId . '/status?action=hide&eventid='.$eventId.'"><span class="icon-eye-open"></span></a>  |';
			}
				
			if($status==0){
				$statusHtml='<a title="Show" href="' . $base_url . '/private-event/package/' . $result [$i]->packageId . '/status?action=show&eventid='.$eventId.'"><span class="icon-eye-close"></span></a>  |';
			}
			
			$rows [] = array(
					//$result [$i]->packageId,
					$result [$i]->packageTitle,
					$result [$i]->packageCategoryTitle,
					'<a title="Edit" href="' . $base_url . '/private-event/package/' . $result [$i]->packageId . '/manage"><span class="icon-edit"></span> </a> | '
                	.$statusHtml
					
			);
		}
	}
	$output .= theme_table(array(
			'header' => $header,
			'rows' => $rows,
			'attributes' => array(
					'class' => array('table table-bordered')
			),
			'sticky' => true,
			'caption' => '',
			'colgroups' => array(),
			'empty' => t("No Records!")
	)) . theme('pager');


			$form['outputmarkup'] = array(
					'#type' => 'markup',
					'#markup' => $output
			);
			return $form;
}