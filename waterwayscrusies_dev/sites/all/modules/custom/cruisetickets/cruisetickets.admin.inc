<?php
function tickets_entrees_assign_form($form, &$form_state) {



	//getting the tickets list
	

	$ticketlist=getTicketslist();
	$entreelist=getEntreeslist();
	
	
	// 	var_dump($ticketlist);

	//header part
	$tabmenuhtml='<ul class="nav nav-pills">
	
    		<li class="">
    		<a href="'.base_path().'cruisetickets">Tickets</a>
    		</li>
	
	<!--
  	<li class="active">
    		<a href="'.base_path().'cruisetickets/entrees">Entrees</a>
    		</li>-->
    				
    				<li class="active">
    		<a href="'.base_path().'cruisetickets/fees">Fees</a>
    		</li>
   </ul>';
	
	
	//end of header part


// 	drupal_set_title($node->title);
	drupal_add_css(drupal_get_path('module', 'uc_entrees') . '/uc_entrees.css');

	
	$form = array();
	
	
	
	
	$entress = getEntreenodes();
	$result = loadCatalogProducts();
	
	$form['headermenu']=array(
			'#type'=>'item',
			'#markup'=>$tabmenuhtml
	);
	
	$form['entree'] = array(
			'#type' => 'fieldset',
			'#title' => t('Entree Configuration'),
			'#weight' => 1,
	);
	
	
	
	
	
	
	
	// 	$productattributes = uc_attribute_load_product_attributes(126);

	// 	$selectedattribentrees=getProductAttributesbynodeid($node->nid);
	$selectedattribentrees=array();
	$collapsefirst = 0;
	foreach ($ticketlist as $key => $atrrib) {
		if ($collapsefirst == 0) {
			$collapsed = FALSE;
		}
		else {
			$collapsed = TRUE;
		}

		$form['entree'][$atrrib->id] = array(
				'#type' => 'fieldset',
				'#title' => t($atrrib->title),
				'#weight' => $atrrib->ordering,
				'#collapsible' => TRUE,
				'#collapsed' => $collapsed,
				'#tree' => TRUE,
		);

		
		$ticketentreelist=getEntreelistbyTicket($atrrib->id);
		//Creating the Node Entree check boxes
		foreach ($entreelist as $nodeentrees) {
			
			
			//checking for already selected ticket entrees
			if(in_array($nodeentrees->id,$ticketentreelist))
			{
				
				$returnvalues=$nodeentrees->id;//ticketid-entreeid
				$form['entree'][$atrrib->id][$nodeentrees->id] = array(
						'#type' => 'checkbox',
						//      '#weight' =>2,
						'#return_value' => $returnvalues,
						'#title' => t($nodeentrees->title),
					'#attributes'=>array('checked' => 'checked'),);
			}
			else
			{
			
				$returnvalues=$nodeentrees->id;//ticketid-entreeid
				$form['entree'][$atrrib->id][$nodeentrees->id] = array(
						'#type' => 'checkbox',
						//      '#weight' =>2,
						'#return_value' => $returnvalues,
						'#title' => t($nodeentrees->title),
						
				);
			}
				
		
			
			
			

		}

		//End of Creating node Entree checkboxes


		$collapsefirst++;
	}


	$form['actions'] = array('#type' => 'actions');
	$form['actions']['save'] = array(
			'#type' => 'submit',
			'#value' => t('Save changes'),
	);

	return $form;



}

function tickets_entrees_assign_form_submit($form, &$form_state)
{
	
	
	
	//Emptying the entire table data before insertion
	//issue : data replication occured in the below code
	db_query("TRUNCATE TABLE {" . ticket_entrees . "}");
	
	foreach ($form_state['values'] as $ticket=>$entreevalue)
	{
		
		foreach ($entreevalue as $entreeval)
		{
			
			if($entreeval!=0){
				$entreeobj=new stdClass();
				$entreeobj->ticketid=$ticket;
				$entreeobj->entreeid=$entreeval;
				$entreeobj->created=REQUEST_TIME;
				drupal_write_record('ticket_entrees', $entreeobj);
			}
			
			
			
			
			
		}
		
		
		
	}

	
	
	
}


function tickets_assign_fees_form($form,&$form_state)
{
	//getting the tickets list
	
	
	$ticketlist=getTicketslist();
	
	$feelist=getFeesList();
	
// 	$entreelist=getEntreeslist();
	
	// 	var_dump($ticketlist);
	
	//header part
	$tabmenuhtml='<ul class="nav nav-pills">
	
    		<li class="">
    		<a href="'.base_path().'cruisetickets">Tickets</a>
    		</li>
	
	
  	<!--<li>
    		<a href="'.base_path().'cruisetickets/entrees">Entrees</a>
    		</li>-->
    				
    				
    				
    				<li class="active">
    		<a href="'.base_path().'cruisetickets/fees">Fees</a>
    		</li>
   </ul>';
	
	
	//end of header part
	
	
	// 	drupal_set_title($node->title);
	drupal_add_css(drupal_get_path('module', 'uc_entrees') . '/uc_entrees.css');
	
	
	$form = array();
	
	
	
	
	$entress = getEntreenodes();
	$result = loadCatalogProducts();
	
	$form['headermenu']=array(
			'#type'=>'item',
			'#markup'=>$tabmenuhtml
	);
	
	$form['fee'] = array(
			'#type' => 'fieldset',
			'#title' => t('Fee Configuration'),
			'#weight' => 1,
	);
	
	
	
	
	
	
	
	// 	$productattributes = uc_attribute_load_product_attributes(126);
	
	// 	$selectedattribentrees=getProductAttributesbynodeid($node->nid);
	$selectedattribentrees=array();
	
	
	
	
	
	$collapsefirst = 0;
	foreach ($ticketlist as $key => $atrrib) {
		if ($collapsefirst == 0) {
			$collapsed = FALSE;
		}
		else {
			$collapsed = TRUE;
		}
	
		$form['fee'][$atrrib->id] = array(
				'#type' => 'fieldset',
				'#title' => t($atrrib->title),
				'#weight' => $atrrib->ordering,
				'#collapsible' => FALSE,
				'#collapsed' => $collapsed,
				'#tree' => TRUE,
		);
	
		$selectedticketfeeids=getticketFeesByTicketId($atrrib->id);
// 		var_dump($selectedticketfeeids);
		//getb the ticket fees list 
		foreach ($feelist as $feevalue) {
			
			
// 			var_dump($feevalue->id);
// 			var_dump($feevalue->title);
// 			var_dump($feevalue->type);
// 			var_dump($feevalue->active);
			
			
			if(in_array($feevalue->id,$selectedticketfeeids))
			{
			
				
				$form['fee'][$atrrib->id][$feevalue->id] = array(
					'#type' => 'checkbox',
					//'#weight' =>2,
					'#return_value' => $feevalue->id,
					'#title' => t($feevalue->title),
				'#attributes'=>array('checked' => 'checked'),);
			
			
			}
			else
			{
				$form['fee'][$atrrib->id][$feevalue->id] = array(
						'#type' => 'checkbox',
						//'#weight' =>2,
						'#return_value' => $feevalue->id,
						'#title' => t($feevalue->title),
							
				);
			}
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
		}
		
// 		var_dump($feelist);
		
		
		
		
		
		
		
		
		
		//End of Creating node Entree checkboxes
	
	
		$collapsefirst++;
	}
	
	
	$form['actions'] = array('#type' => 'actions');
	$form['actions']['save'] = array(
			'#type' => 'submit',
			'#value' => t('Save changes'),
	);
	
	return $form;
	
}



/**ticket fee assignment submission**/
function tickets_assign_fees_form_submit($form,&$form_state)
{
	


	//Emptying the entire table data before insertion
	//issue : data replication occured in the below code
	db_query("TRUNCATE TABLE tickets_fees");
	
	
	foreach ($form_state['values'] as $ticketid=>$feeid)
	{
// 		var_dump($ticketid);
		
		if(@is_array($feeid)){
			foreach ($feeid as $feevalcheckedval)
			{
					
				if($feevalcheckedval!=0){
					// 				var_dump($feevalcheckedval);
			
			
					//creating the ticket fee object
					$newfeeobj=new stdClass();
					$newfeeobj->feeid=$feevalcheckedval;
					$newfeeobj->ticket_id=$ticketid;
					try
					{
						drupal_write_record('tickets_fees',$newfeeobj);
							
					}
					catch (Exception $ex)
					{
						watchdog('ticket_fees','message'.$ex->getMessage());
					}
				}
					
					
					
					
					
			}
		}
	
	}
	
	
	
	
}





