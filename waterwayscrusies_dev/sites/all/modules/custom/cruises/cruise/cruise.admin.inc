<?php



function cruise_tickets_assign_form($form, &$form_state, $cruise) {
	
	drupal_set_title ($cruise->cruise_title );
	drupal_add_css ( drupal_get_path ( 'module', 'cruise' ) . '/cruise.css' );
	//getting the tickets list
	
	$ticketlist=getTicketslist();
	//$selectedids=getSelectedids($cruise->id);
	$entreelist=getEntreeslist();
	//check for cruise assignement list
	$entreesArray=array();
	foreach($entreelist as $entree){
		$entreesArray[$entree->id]=$entree->title;
	}
	
	
	//Ticket Entrees
	//Check Ticket Existence
	
	//Tickets Count
	$totalweight=range(0,count($entreesArray));
	
	
	foreach ( $ticketlist as $ticket ) {
		
		$ticketexistence=Cruise::getCruiseTicketEntreeDetails($ticket->id, $cruise->id);
		//drupal_json_output($ticketexistence->ticketid);exit();
		
		$ticketPrice='';
		$ticketID='';
		$ticketWeight='';
		$ticketEntreeIDSArray=array();
		$collapsedoption=TRUE;
		
		$ticketStatus=false;
		if($ticketexistence){
			
			$collapsedoption=FALSE;
			$ticketStatus=$ticketexistence->activestatus;
			$ticketID=$ticketexistence->ticketid;
			$ticketPrice=$ticketexistence->price;
			$ticketWeight=$ticketexistence->weight;
			$ticketEntreeSerialized=$ticketexistence->entreeids;
			if($ticketEntreeSerialized){
				$ticketEntreeIDSArray=unserialize($ticketEntreeSerialized);
			}
		}
		
		
		
		$form ['tickets' . $ticket->id] = array (
				'#type' => 'fieldset',
				'#title' => t ( $ticket->title ),
				'#weight' => 5,
				'#collapsible' => TRUE,
				'#collapsed' => $collapsedoption, 
		);
		
		
		if($ticketStatus){
			$form ['tickets' . $ticket->id] ['ticket-' . $ticket->id] = array (
					'#type' => 'checkbox',
					'#return_value' => $ticket->id,
					'#title' => t ( 'Active' ),
					'#default_value'=>$ticketID,
					//'#attributes' => array ('checked' => false),
			);
		}else{
			$form ['tickets' . $ticket->id] ['ticket-' . $ticket->id] = array (
					'#type' => 'checkbox',
					'#return_value' => $ticket->id,
					'#title' => t ( 'Active' ),
					//'#default_value'=>$ticketID,
					//'#attributes' => array ('checked' => false),
			);
		}
		
		
		$form ['tickets' . $ticket->id] ['price-' . $ticket->id] = array (
				'#type' => 'textfield',
				'#title' => t ( 'Price' ),
				'#size' => 60,
				'#maxlength' => 128,
				'#title_display' => 'invisible',
				'#attributes' => array ('placeholder' => 'Price'),
				'#field_prefix' => '$',
				'#default_value'=>sprintf("%.2f",$ticketPrice),
		);
		
		$form ['tickets' . $ticket->id] ['entree-' . $ticket->id] = array (
				'#type' => 'checkboxes',
				'#title' => t ( 'Entrees' ),
				'#options' => $entreesArray,
				'#default_value' =>$ticketEntreeIDSArray,
		);
		
		$form ['tickets' . $ticket->id] ['weight-' . $ticket->id] = array (
				//'#type' => 'select',
				//'#size' => 60,
				// '#attributes' => array ('placeholder' => 'Ticket Order'),
				'#type' => 'select',
				'#title' => t ( 'Order' ),
				'#options' =>$totalweight,
				'#default_value' => $ticketWeight,
				//'#description' => t ( 'Set this to <em>Yes</em> if you would like this category to be selected by default.' ) 
		);
		
		
	}
		
		
	$form['cruisetemplateid'] = array(
		'#type' => 'hidden',
		 '#value' => $cruise->id
	);
	
	
	
	$form['actions'] = array('#type' => 'actions');
	$form['actions']['save'] = array(
			'#type' => 'submit',
			'#value' => t('Save changes'),
	);
        
        $form['actions']['cancel'] = array(
        '#type'   => 'submit',
        '#value'  => t('Cancel'),
        '#submit' => array('back_ticket_for_cancel_button'),
         '#limit_validation_errors' => array(),
       );
	
	return $form;
	
	
	
}


function back_ticket_for_cancel_button($form, &$form_state) {
  $url =  $base_url . 'admin/structure/cruise/manage';
  drupal_goto($url);
}

function cruise_tickets_assign_form_submit($form, &$form_state)
{
	
	//drupal_json_output($form_state['values']);
	//exit();
	
	//Delete Records (Cruise template ID)
	try{
		$num_deleted=db_delete('cruise_template_tickets')
		->condition('cruisetemplateid',$form_state['values']['cruisetemplateid'],'=')
		->execute();
	}catch(Exception $e){
	
	}
	$selctedtickets=$form_state['values'];
	foreach ($selctedtickets as $key=>$val)
	{
		$regex = "/^(ticket-)?\d+$/";
		if (preg_match($regex, $key)) {
			//ticket-991
			$ticketarr=explode('-', $key);
			$ticketID=$ticketarr[1];
			//if($form_state['values']['ticket-'.$ticketID]){
				$tiketPrice=$form_state['values']['price-'.$ticketID];
				$ticketStatus=($form_state['values']['ticket-'.$ticketID]!="") ? 1 : 0;
				$weight=$form_state['values']['weight-'.$ticketID];
				if($tiketPrice!="" && !is_null($tiketPrice)){
					$ticketEntreesArr=$form_state['values']['entree-'.$ticketID];
					$ticketEntrees=array();
					foreach($ticketEntreesArr as $entreekey => $entreVal){
						if($entreVal!=0){
							$ticketEntrees[]=$entreVal;
						}
					}
					$cruiseticketsobj=new stdClass();
					$cruiseticketsobj->activestatus=$ticketStatus;
					$cruiseticketsobj->cruisetemplateid=$selctedtickets['cruisetemplateid'];
					$cruiseticketsobj->price=$tiketPrice;
					$cruiseticketsobj->ticketid=$ticketID;
					$cruiseticketsobj->entreeids=serialize($ticketEntrees);
					$cruiseticketsobj->created=REQUEST_TIME;
					$cruiseticketsobj->weight=$weight;
					drupal_write_record('cruise_template_tickets', $cruiseticketsobj);
					//drupal_json_output($cruiseticketsobj);
				}
				
			//}
		}
	}

$redirect='cruise/'.$selctedtickets['cruisetemplateid'].'/edit/tickets';

drupal_goto($redirect);



}


//End of ticket section

// Enhacments section

/**
 * Form builder for product stock edit form.
 *
 * @see cruise_enhancements_assign_form_submit()
 * @see theme_cruise_enhancements_assign_form() @ingroup forms
 */
function cruise_enhancements_assign_form($form, &$form_state, $cruise) {
	
	
	
	drupal_set_title ($cruise->cruise_title );
	drupal_add_css ( drupal_get_path ( 'module', 'cruise' ) . '/cruise.css' );
	

	
	
	$result = db_query('SELECT *
FROM {cruise_template_enhancements} pe WHERE pe.cruisetemplateid = :cruisetemplateid', array(':cruisetemplateid' => $cruise->id));
	foreach ($result as $value) {
		//  $response['bookitems']=json_decode(unserialize($value->bookitems));
// 	var_dump($value);
		
		$response = (array) (unserialize($value->templateenhancements));
	}
	//return array(nodeids)
// 	var_dump($response['enhancedproducts']);
	
	$selectedproductEnhancments=array();
	if(isset($response['enhancedproducts'])){
		$selectedproductEnhancments =$response['enhancedproducts'];
	}
	
	
// 	$selectedproductEnhancments=array();//later no we will get replace
	$result = loadCatalogProducts ();
	
	$form = array ();
	
	
	$form['cruisetemplateid'] = array(
			'#type' => 'hidden',
			'#value' => $cruise->id);
	
	
	$form ['productenhancements'] = array (
			'#type' => 'item',
// 			'#prefix' => '<ul>',
// 			'#suffix' => '</ul>',
			'#weight' => 1 
	);
	
	// $form['productenhancements']['ALL'] = array(
	// '#type' =>'checkbox',
	// '#prefix' => '<li>',
	// '#suffix' => '</li>',
	// '#weight' =>2,
	// '#title'=>t('ALL'),
	//
	//
	// );
	 $colweight=1;
	foreach ( $result as $productvalue ) {
 		
		foreach ($productvalue as $key => $productdetails ) {
//                 var_dump($key);
                 if(count($productdetails) > 0)
                        {
                          $collapse=FALSE;
                        }
                        else {
                           $collapse=TRUE;
                        }
			$form ['productenhancements'] [$key] = array (
					'#type' => 'fieldset',
// 					'#prefix' => '<ul>',
// 					'#suffix' => '</ul>',
					'#weight' =>$colweight+1,
					'#title' => t ( $key ),
					'#collapsible' =>TRUE,
                            '#collapsed' => $collapse, 
			);
			// $form['productenhancements'][$key]['packages-' . $key] = array(
			// '#type' => 'checkbox',
			// '#prefix' => '<li>',
			// '#suffix' => '</li>',
			// '#weight' => 4,
			// '#return_value' => $key,
			// '#title' => t($key),
			// );
			
			$form ['productenhancements'] [$key] ['packages-' . $key] = array (
					'#type' => 'item',
// 					'#prefix' => '<li>',
// 					'#suffix' => '</li>',
					'#weight' => 4,
					'#return_value' => $key,
					//'#title' => t ( $key ) 
			);
			
			$form ['productenhancements'] [$key] ['subs'] = array (
					'#type' => 'fieldset',
					// '#prefix' => '<ul>',
					// '#suffix' => '</ul>',
					'#weight' => 5 
			);
			$w = 5;
			foreach ( $productdetails as $productform ) {
				
				$producttitle = $string = preg_replace ( '/\s+/', '', $productform ['title'] );
				if (in_array ( $productform ['nid'], $selectedproductEnhancments )) {
					$form ['productenhancements'] [$key] ['subs'] [$producttitle] = array (
							'#type' => 'checkbox',
// 							'#prefix' => '<li>',
// 							'#suffix' => '</li>',
							'#weight' => $w + 1,
							'#title' => t ( $productform ['title'] ),
							'#return_value' => $productform ['nid'],
							'#default_value' => 0,
							'#attributes' => array (
							'checked' => 'checked' 
							) 
					);
				} else {
					$form ['productenhancements'] [$key] ['subs'] [$producttitle] = array (
							'#type' => 'checkbox',
// 							'#prefix' => '<li>',
// 							'#suffix' => '</li>',
							'#weight' => $w + 1,
							'#title' => t ( $productform ['title'] ),
							'#return_value' => $productform ['nid'],
							'#default_value' => 0,
							'#states' => array (
							'checked' => array (
							':input[name="packages-' . $key . '"]' => array (
							'checked' => TRUE 
							) 
							) 
							) 
					);
				}
			}
                        
                        
                      
                        
		}
	}
	$form ['selectednodeid'] = array (
			'#type' => 'hidden',
			'#value' => $cruise->id 
	);
	$form ['actions'] = array (
			'#type' => 'actions' 
	);
	$form ['actions'] ['save'] = array (
			'#type' => 'submit',
			'#value' => t ( 'Save changes' ) 
	);
	
        $form['actions']['cancel'] = array(
       '#type'   => 'submit',
       '#value'  => t('Cancel'),
       '#submit' => array('back_enhancements_for_cancel_button'),
       '#limit_validation_errors' => array(),
       );

  
        
	return $form;
}


function back_enhancements_for_cancel_button($form, &$form_state) {
  $link =  $base_url . 'admin/structure/cruise/manage';
  drupal_goto($link);
}
/**
 * Form submission handler for cruise_enhancements_assign_form().
 *
 * @see cruise_enhancements_assign_form()
 * @see theme_cruise_enhancements_assign_form()
 */
function cruise_enhancements_assign_form_submit($form, &$form_state) {
	
	
	
	//Delete Records (Cruise template ID)
	try{
		$num_deleted=db_delete('cruise_template_enhancements')
		->condition('cruisetemplateid',$form_state['values']['cruisetemplateid'],'=')
		->execute();
	}catch(Exception $e){
	
	}
	
	
	
// 	var_dump($form_state['values']);exit();

	$notrequiredarray = array (
			'packages',
			'ALL',
			'save',
			'form_build_id',
			'form_token',
			'form_id',
			'op' 
	);
	
	$enhancednodes = array ();
	$productids = array ();
	foreach ( $form_state ['values'] as $valkey => $value ) {
		$arr = explode ( '-', $valkey );
		if (! in_array ( $arr [0], $notrequiredarray )) {
			$selectedproductnode = ( int ) $value;
			if ($selectedproductnode > 0)
				$productids [] = $selectedproductnode;
		}
	}
	$enhancednodes ['enhancedproducts'] = $productids;
// 	var_dump($enhancednodes);exit();
	
	/**
	 * Getting the Entire Product Enhancement selected Options
	 * id,auto incrmented
	 * nid,selected product nid
	 * productenhancements
	 * {enhancedproducts:array(productnodeid,productnodeid,productnodeid);
	 */
	
// 	id
// 	cruisetemplateid
// 	templateenhancements
	
	$cruisetemplate_enhancementobject=new stdClass();
	$cruisetemplate_enhancementobject->cruisetemplateid= $form_state ['values'] ['selectednodeid'];
	
	$cruisetemplate_enhancementobject->templateenhancements=( object ) $enhancednodes;
	$cruisetemplate_enhancementobject->created=REQUEST_TIME;
	drupal_write_record ( 'cruise_template_enhancements', $cruisetemplate_enhancementobject );
// 	$newob = new stdClass ();
// 	$newob->nid = $form_state ['values'] ['selectednodeid'];
// 	$newob->productenhancements = ( object ) $enhancednodes;
	
// 	$result = db_select ( 'uc_product_enhancements', 'c' )->fields ( 'c' )->condition ( 'nid', $form_state ['values'] ['selectednodeid'], '=' )->execute ();
// 	$num_of_results = $result->rowCount ();
// 	$update = ( int ) $result->fetchField ( 0 );
	
// 	if ($num_of_results >= 1) {
// 		drupal_write_record ( 'uc_product_enhancements', $newob, $update );
// 	} else {
// 		drupal_write_record ( 'uc_product_enhancements', $newob );
// 	}
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	drupal_set_message ( t ( 'Product Enhancements settings saved.' ) );
}

// End of Enhancments section
function cruise_fees_assign_form($form, &$form_state, $cruise) {
	$cruisefeelist=array();

	$query = db_select("cruisefee", "d");
	// Adding fields
	$query->fields('d');
	
	$result = $query->execute();
	
	
	foreach ($result as $code)
	{
	$cruisefeelist[$code->id]=array(
			$code->title,
			$code->rate,
			$code->type,
			
			drupal_render($form['checkboxes'][$code->id] = array(
			'#type' => 'textfield',
			
			'#default_value' => $node->title,
			'#size' => 60,
			'#maxlength' => 128,
			'#required' => TRUE,
			
			)),
			
	);
	
	}
	
	
	
// 	$disputes = array(
// 			1 => array('Joe', 'Smith'),//row1
// 			2 => array('Susan', 'Doe'),
// 			3 => array('John', 'Doe'),
// 			4 => array('John', 'Doe')
// 	);
	
	
	$header = array(
			
			t('Fee'),
			t('Default rate'),
			t('type'),
			t('Applied Rate'),
			
	);
	
	$form['checkboxes'] = array(
			'#type' => 'tableselect',
			'#header' => $header,
			'#options' => $cruisefeelist,
			'#empty' => 'No users were found.',
			'#title'=>t('Select')
	);
	
	
	
	

	$form ['actions'] = array (
			'#type' => 'actions'
	);
	
	$form ['actions'] ['submit'] = array (
			'#type' => 'submit',
			'#value' => t ( 'Save' )
	);
	
	$form ['actions'] ['cancel'] = array (
			'#type' => 'submit',
			'#value' => t ( 'Cancel' ),
			'#submit' => array (
					'enhancementcategories_form_cancel'
			)
	);
	return $form;
}



/***
 * 
 * Cruise fee assignment form submission
 * 
 * 
 */
function cruise_fees_assign_form_submit($form, &$form_state, $cruise) {
	var_dump($form_state['values']);
	exit();
}








/**
 * 
 * 
 * 
 */
function cruise_dsicount_assign_form($form, &$form_state, $cruise)
{
    
     $path="admin/structure/coupon/manage/list";
    drupal_goto($path);
    exit();
	$promolistview=array();
	$query = db_select("discountcodes", "d");
	// Adding fields
	$query->fields('d');
	$result = $query->execute();
	foreach ($result as $code)
	{
	$promolistview[$code->id]=array(
			$code->code,
			$code->rate,
			$code->type,
			$code->validfrom.'/'.$code->validuntil,
	);
	
	}
	
	
	
// 	$disputes = array(
// 			1 => array('Joe', 'Smith'),//row1
// 			2 => array('Susan', 'Doe'),
// 			3 => array('John', 'Doe'),
// 			4 => array('John', 'Doe')
// 	);
	
	
	$header = array(
			
			t('Promo Code'),
			t('Rate'),
			t('Type'),
			t('Start /End dates'),
	);
	
	$form['checkboxes'] = array(
			'#type' => 'tableselect',
			'#header' => $header,
			'#options' => $promolistview,
			'#empty' => 'No users were found.',
			'#title'=>t('Select')
	);
	
	
	
	

	$form ['actions'] = array (
			'#type' => 'actions'
	);
	
	$form ['actions'] ['submit'] = array (
			'#type' => 'submit',
			'#value' => t ( 'Save' )
	);
	
	$form ['actions'] ['cancel'] = array (
			'#type' => 'submit',
			'#value' => t ( 'Cancel' ),
			'#submit' => array (
					'enhancementcategories_form_cancel'
			)
	);
	return $form;
}
