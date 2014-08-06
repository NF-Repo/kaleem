<?php
function changecruise($eventid, $orderid, $date) {
	// drupal_json_output($changeCruiseObject); exit();
	// Getting Template Id
	$cruisetemplate_id = Scheduledevents::getCruiseTemplateIdByEventId ( $eventid );
	
	$reservationData = Reservation::getReservationData ( $orderid )->fetchObject ();
	$r_id = $reservationData->id;
	
	$changeCruiseObject = new stdClass ();
	$changeCruiseObject->order_id = $orderid;
	$changeCruiseObject->cruisetemplate_id = $cruisetemplate_id;
	$changeCruiseObject->event_id = $eventid;
	$changeCruiseObject->event_date = $date;
	$changeCruiseObject->r_id = $r_id;
	
	CartSession::removePromocode($changeCruiseObject->order_id);
	//CartSession::removeGiftCard($changeCruiseObject->order_id);
	CartSession::removeVoucher($changeCruiseObject->order_id);
	
	return drupal_get_form ( 'booking_cruise_change_form', $changeCruiseObject );
}

/**
 * Building the form for the edit setting page.
 */
function booking_cruise_change_form($form, &$form_state, $changeCruiseObject) {
	
	
	//Pre-filters
	//Removing the promocode
	
	
	
	drupal_add_css(drupal_get_path('module', 'booking_orders') . '/css/booking_orders.css', array('group' => CSS_DEFAULT, 'type' => 'file'));
	
	$currentorderid = "currentorderid" . $changeCruiseObject->order_id;
	$currentOrderObject = $_SESSION [$currentorderid];
	
	$oldOrderTicketQty = $currentOrderObject->oldOrderTicketQty;
	$oldOrderEnhancementQty = $currentOrderObject->oldOrderEnhancementQty;
	$oldOrderEventId = $currentOrderObject->oldOrderEventId;
	$oldOrderCruiseTemplateId = $currentOrderObject->oldOrderCruiseTemplateId;
	
	$form ['oldOrderTicketQty'] = array (
			'#id' => 'oldOrderTicketQty',
			'#type' => 'textfield',
			'#prefix' => '<div style="display:none;">',
			'#suffix' => '</div>',
			'#default_value' => $oldOrderTicketQty 
	);
	
	$form ['oldOrderEnhancementQty'] = array (
			'#id' => 'oldOrderEnhancementQty',
			'#type' => 'textfield',
			'#prefix' => '<div style="display:none;">',
			'#suffix' => '</div>',
			'#default_value' => $oldOrderEnhancementQty 
	);
	
	$form ['oldOrderEventId'] = array (
			'#id' => 'oldOrderEventId',
			'#type' => 'textfield',
			'#prefix' => '<div style="display:none;">',
			'#suffix' => '</div>',
			'#default_value' => $oldOrderEventId 
	);
	
	$form ['oldOrderCruiseTemplateId'] = array (
			'#id' => 'oldOrderCruiseTemplateId',
			'#type' => 'textfield',
			'#prefix' => '<div style="display:none;">',
			'#suffix' => '</div>',
			'#default_value' => $oldOrderCruiseTemplateId 
	);
	
	// Hidden Values
	$form ['hiddenreservationid'] = array (
			'#id' => 'hiddenreservationid',
			'#type' => 'textfield',
			'#prefix' => '<div style="display:none;">',
			'#suffix' => '</div>',
			'#default_value' => $changeCruiseObject->r_id 
	);
	
	$form ['hiddenorderid'] = array (
			'#id' => 'hiddenorderid',
			'#type' => 'textfield',
			'#prefix' => '<div style="display:none;">',
			'#suffix' => '</div>',
			'#default_value' => $changeCruiseObject->order_id 
	);
	$form ['hiddencruisetemplateid'] = array (
			'#id' => 'hiddencruisetemplateid',
			'#type' => 'textfield',
			'#prefix' => '<div style="display:none;">',
			'#suffix' => '</div>',
			'#default_value' => $changeCruiseObject->cruisetemplate_id 
	);
	
	$form ['hiddeneventid'] = array (
			'#id' => 'hiddeneventid',
			'#type' => 'textfield',
			'#prefix' => '<div style="display:none;">',
			'#suffix' => '</div>',
			'#default_value' => $changeCruiseObject->event_id 
	);
	$form ['hiddenselectedcruisedate'] = array (
			'#id' => 'hiddenselectedcruisedate',
			'#type' => 'textfield',
			'#prefix' => '<div style="display:none;">',
			'#suffix' => '</div>',
			'#default_value' => $changeCruiseObject->event_date 
	);
	
	$availableTickets = getCruiseTicketByWeight ( $changeCruiseObject->cruisetemplate_id );
	// Getting Ticket Entrees by Ticket and Template Id
	$ticketEntrees = array ();
	foreach ( $availableTickets as $ticket ) {
		$data = Cruise::getTicketEntrees ( $ticket->id, $changeCruiseObject->cruisetemplate_id );
		$arr = array (
				'' => '--Select--' 
		);
		foreach ( $data as $key => $val ) {
			$arr [$key] = $val;
			$ticketEntrees [$ticket->id] = $arr;
		}
	}
	
	$form ['changecurise'] ['containerfluidstart'] = array (
			'#type' => 'markup',
			'#markup' => '<!--container-fluid starts here--><div class="container-fluid"><!--row-fluid starts here--><div class="row-fluid">' 
	);
	
	$form ['changecurise'] ['rowfluidstart'] = array (
			'#type' => 'markup',
			'#markup' => '<!--row-fluid starts here--><div class="row-fluid">' 
	);
	
	// Span 6 : Col1 Starts here
	$form ['changecurise'] ['spansixcolonestart'] = array (
			'#type' => 'markup',
			'#markup' => ' <!--span6 starts here--><div class="span6">' 
	);
	
	$form ['changecurise'] ['eventdetails'] = array (
			'#type' => 'fieldset',
			'#title' => '<span class="icon-th-large"></span>&nbsp;'.t ( 'Event Details' ),
// 			'#collapsible' => TRUE,
// 			'#collapsed' => FALSE,
			'#prefix' => '<div class="row-fluid"><div class="span12">',
			'#suffix' => '</div></div>' 
	);
	
	$selectedEventObject = scheduledevents_load ( $changeCruiseObject->event_id );
	$selectedEventDate = getScheduleddatebyeventId ( $changeCruiseObject->event_id );
	
	// Loading the Cruise Template Object
	$cruiseObject = cruise_load ( $selectedEventObject->cruisetemplate_id );
	
	$portData = loadportById ( $selectedEventObject->port_id );
	
	$form ['changecurise'] ['eventdetails'] ['selectedeventdetails'] = array (
			'#type' => 'markup',
			'#markup' => '
		
		
			<div class="row-fluid">
			<div class="span12"><h4>Selected </h4></div>
		
			</div>
		
				
				
			<div class="row-fluid">
			<div class="span4"><div class="pull-right">Cruise Title :</div></div>
			<div class="span8 "> ' . $cruiseObject->cruise_title . ' </div>
			</div>
				
		
					<div class="row-fluid">
			<div class="span4"><div class="pull-right">Cruise Date :</div></div>
			<div class="span8">' . date ( "m-d-Y", $selectedEventDate ) . ' </div>
			</div>
								
								
							<div class="row-fluid">
			<div class="span4"><div class="pull-right">Port :</div></div>
			<div class="span8">' . $portData ['portname'] . ' </div>
			</div>
				
				
			<div class="row-fluid">
			<div class="span4"><div class="pull-right">Boarding :</div></div>
			<div class="span8">' . $selectedEventObject->boardingtime . ' </div>
			</div>
				
				
			<div class="row-fluid">
			<div class="span4"><div class="pull-right">Departure :</div></div>
			<div class="span8">' . $selectedEventObject->departuretime . ' </div>
			</div>
				
				
			<div class="row-fluid">
			<div class="span4"><div class="pull-right">Return :</div></div>
			<div class="span8">' . $selectedEventObject->returntime . ' </div>
			</div>
		
		
			<div class="row-fluid">
			<div class="span12"><h4>Available </h4></div>
			</div>
				
			<div class="row-fluid">
			<div class="span4"><div class="pull-right">Tickets :</div></div>
			<div class="span8">' . $selectedEventObject->availabletickets . ' </div>
			</div>
				
				
			<div class="row-fluid">
			<div class="span4"><div class="pull-right">Windows :</div></div>
			<div class="span8">' . $selectedEventObject->availablewindowseats . ' </div>
			</div>
				
			' 
	);
	
	// tickets
	$form ['changecurise'] ['tickets'] = array (
			'#type' => 'fieldset',
			'#title' => t ( 'Tickets' ),
			'#title' => '<span class="icon-th-large"></span>&nbsp;'.t('Tickets'),
            '#prefix' => '<div class="row-fluid"><div class="span12">',
			'#suffix' => '</div></div>',
	);
	
	$form['changecurise']['tickets']['ticketwidgetstart']=array(
			'#type'=>'markup',
			'#markup'=>'<table><tr><td>&nbsp;&nbsp;</td><td><b>Ticket</b></td><td><b>Price</b></td><td><b>Qty</b></td></tr>',
	);
	
	foreach ( $availableTickets as $ticket ) {
		$ticketlabel = $ticket->title . " $" . wwcFormatPrice ( $ticket->price );
		
		
		$form ['changecurise'] ['tickets'] ['ticketname' . $ticket->id] = array(
				'#type' => 'markup',
				'#prefix' => '<tr><td>&nbsp;&nbsp;</td><td>'	. $ticket->title . '</td>',
		);
		
		
// 		$form ['changecurise'] ['tickets'] ['ticketentreestitle' . $ticket->id] = array (
// 				'#type' => 'markup',
// 				'#prefix' => '<div class="row-fluid" > <div class="span4" style="margin-top:18px;">',
// 				'#suffix' => '</div>',
// 				'#markup' => $ticket->title 
// 		);
		
		
		$form ['changecurise'] ['tickets'] ['ticketentrees' . $ticket->id . "price"] = array(
				'#type' => 'textfield',
				'#default_value' => sprintf ( "%.2f", $ticket->price ),
				'#prefix' => '<td>',
				'#suffix' => '</td>',
		);
		
		$form ['changecurise'] ['tickets'] ['ticketentrees' . $ticket->id] = array (
				'#type'=>'select',
				'#prefix' => '<td>',
       			'#suffix' => '</td></tr>',
// 				'#options' => array (
// 						0 => 0,
// 						1 => 1,
// 						2 => 2,
// 						3 => 3,
// 						4 => 4,
// 						5 => 5,
// 						6 => 6,
// 						7 => 7,
// 						8 => 8,
// 						9 => 9 
// 				),
				'#options' =>   drupal_map_assoc(range(0, 50)),
				// '#default_value' => $defaultvalue,
				'#ajax' => array (
						'callback' => 'changecruise_ticketentrees_callback',
						'wrapper' => 'ticketentreesDiv',
						'method' => 'html',
						'effect' => 'fade' 
				),
				'#attributes' => array (
						'class' => array (
								'input-mini' 
						) 
				) 
		);
		
		$form ['changecurise'] ['tickets'] ['ticketentrees' . $ticket->id . "ticketname"] = array (
				'#type' => 'textfield',
				'#default_value' => $ticket->title,
				'#prefix' => '<div style="display:none;">',
				'#suffix' => '</div>' 
		);
		
		
		
		// attribute options
		$form ['changecurise'] ['tickets'] ['ticketentreesoption' . $ticket->id] = array (
				'#type' => 'hidden',
				'#value' => $ticket->id 
		);
	} // foreach
	
	
	$form['changecurise']['tickets']['ticketwidgetend']=array(
			'#type'=>'markup',
			'#markup'=>'</table>',
	);
	
	$form ['changecurise'] ['tickets'] ['ticketentrees_fieldset'] = array (
			'#type' => 'markup',
			'#prefix' => '<div id="ticketentreesDiv">',
			'#suffix' => '</div>' 
	);
	
	$form ['changecurise'] ['tickets'] ['ticketentrees_fieldset'] ['header'] = array (
			'#type' => 'markup',
			'#markup' => '<br/><br/>
		
			<div class="row-fluid">
			<div class="span12"><b>Select your Entree for each cruise ticket below:</b></div>
			</div>
		
			<div class="row-fluid">
		
			<div class="span2"><b>QTY</b></div>
		
			<div class="span3"><b>Ticket</b></div><div class="span7"><b>Entree</b></div>
		
			
				
				
				
				
			</div>' 
	)
	;
	
	foreach ( $availableTickets as $ticket ) {
		
		// $numoftickets = '5';
		
		$numoftickets = $form_state ['values'] ['ticketentrees' . $ticket->id];
		
		// var_dump($numoftickets);
		
		for($i = 0; $i < $numoftickets; $i ++) {
			
			$selectdefaultvalue = $values ["select" . $ticket->id . $i];
			$form ['changecurise'] ['tickets'] ['ticketentrees_fieldset'] ['entry' . $ticket->id] ["qty" . $ticket->id . $i] = array (
					'#type' => 'markup',
					'#markup' => '1',
					'#prefix' => '<div class="row-fluid"> <div class="span2">',
					'#suffix' => '</div>' 
			);
			
			$form['changecurise']['tickets']['ticketentrees_fieldset']['entry' . $ticket->id]["name" . $ticket->id . $i] = array(
			'#type' => 'markup',
			'#markup' => '<div class="span3">' . $ticket->title . "</div>",
			);
			
			$form ['changecurise'] ['tickets'] ['ticketentrees_fieldset'] ['entry' . $ticket->id] ["select" . $ticket->id . $i] = array (
					'#id' => 'option-entree-' . $ticket->id . '-' . $i,
					'#type' => 'select',
					'#options' => $ticketEntrees [$ticket->id],
					'#default_value' => '',
					'#prefix' => '<div class="span7">',
					'#suffix' => '</div></div>',
			
					//'#title' => $ticket->title 
			);
			
	
			
			$form ['changecurise'] ['tickets'] ['ticketentrees_fieldset'] ['entry' . $ticket->id] ["dietarycomments" . $ticket->id . $i] = array (
					'#id' => 'dietarycomments-' . $ticket->id . '-' . $i,
					'#type' => 'textarea',
					'#prefix' => '<div style="display:none;">',
					'#suffix' => '</div>',
					'#default_value' => '' 
			)
			;
		}
	}
	
	// enhancements
	$form ['changecurise'] ['enhancements'] = array (
			'#type' => 'fieldset',
			'#title' => '<span class="icon-th-large"></span>&nbsp;'.t ( 'Enhancements' ),
// 			'#collapsible' => TRUE,
// 			'#collapsed' => FALSE,
			'#prefix' => '<div class="row-fluid"><div class="span12">',
			'#suffix' => '</div></div>' 
	)
	;
	
	$totalproducts = loadCatalogProducts ();
	$cruiseTemplateSelectedEnhancments = CruiseEnhancements::getSelectedEnhancements ( $changeCruiseObject->cruisetemplate_id, $changeCruiseObject->event_id );
	foreach ( $totalproducts as $productkey => $productvalue ) {
		foreach ( $productvalue as $optionkey => $optionval ) {
			
			  $form['changecurise']['enhancements'][$optionkey."start"] = array(
                '#type' => 'markup',
                '#markup' => "<span class='icon-circle-arrow-right'></span>&nbsp;&nbsp;<b>".$optionkey."</b><br/>",
//                 '#collapsible' => FALSE,
//                 '#collapsed' => FALSE,
            );
		
			
			foreach ( $optionval as $enhancementvalue ) {
				if (in_array ( $enhancementvalue ['nid'], $cruiseTemplateSelectedEnhancments )) {
					$form ['changecurise'] ['enhancements'] [$optionkey] ['enhancementtitle' . $enhancementvalue ['nid']] = array (
							'#type' => 'markup',
							'#markup' => '<div class="row-fluid"><div class="span6">'.$enhancementvalue ['title'].'</div>'
					);
					
					$form ['changecurise'] ['enhancements'] [$optionkey] ['_enhancementtitle' . $enhancementvalue ['nid']] = array (
							'#type' => 'textfield',
							'#default_value' => $enhancementvalue ['title'],
							'#prefix' => '<div style="display:none;">',
							'#suffix' => '</div>' 
					);
					
					$form ['changecurise'] ['enhancements'] [$optionkey] ['enhancement' . $enhancementvalue ['nid']] = array (
							'#type' => 'textfield',
							'#default_value' => $enhancementvalue ['nid'],
							'#prefix' => '<div style="display:none;">',
							'#suffix' => '</div>' 
					);
					
					$form ['changecurise'] ['enhancements'] [$optionkey] ['price' . $enhancementvalue ['nid']] = array (
							'#type' => 'textfield',
							'#default_value' => sprintf ( "%.2f", $enhancementvalue ['price'] ) ,
							'#prefix' => '<div class="span4">',
							'#suffix' => '</div>',
							'#attributes' => array('style' => 'width:75% !important;'),
					);
					
					
					
					$form ['changecurise'] ['enhancements'] [$optionkey] ['qty' . $enhancementvalue ['nid']] = array (
							'#type' => 'select',
							'#options' =>   drupal_map_assoc(range(0, 20)),
							'#prefix' => '<div class="span2">',
							'#suffix' => '</div></div>',
							'#attributes' => array(
									'class' => array(
											'input-mini'
									)
							),
					)
					;
				} // if
			}
		} // foreach
	} // foreach
	
	$form ['changecurise'] ['spansixcoloneend'] = array (
			'#type' => 'markup',
			'#markup' => '</div><!--span6 ends here-->' 
	);
	
	// Span 6 : Col1 ends here
	
	$form ['changecurise'] ['spansixcoltwostart'] = array (
			'#type' => 'markup',
			'#markup' => ' <!--span6 starts here--><div class="span6">' 
	);
	
	$form ['changecurise'] ['cruisefees'] = array (
			'#type' => 'fieldset',
			'#title' => '<span class="icon-th-large"></span>&nbsp;'.t ( 'Cruise Fees' ),
// 			'#collapsible' => TRUE,
// 			'#collapsed' => FALSE,
			'#prefix' => '<div class="row-fluid" ><div class="span12">',
			'#suffix' => '</div></div>' 
	);
	
	// Load all cruise fee itesm
	$feeItems = CruiseFees::getAllFeeItemsByTemplateId ( $changeCruiseObject->cruisetemplate_id );
	
	foreach ( $feeItems as $feeitem ) {
		
		// ($feeitem);
		$chargeamt = false;
		$feeId = $feeitem->feeid;
		$feeTitle = $feeitem->title;
		$feeType = $feeitem->type;
		$a_feeValue = $feeitem->value;
		$feeValue = 0;
		
		// Check is Fee activated or not
		$feeStatus = CruiseFees::getCruiesFeeStatus ( $changeCruiseObject->cruisetemplate_id, $feeId );
		
		// var_dump($feeId);
		if ($feeStatus == "1") {
			$u_feeValue = CruiseFees::getCruiesFeePrice ( $changeCruiseObject->cruisetemplate_id, $feeId );
			if ($u_feeValue > 0 && $u_feeValue != "") {
				$feeValue = $u_feeValue;
			} else {
				$feeValue = $a_feeValue;
			}
			
			$fee_value = number_format ( $feeValue, 2 );
			
			$form ['changecurise'] ['cruisefees'] ['cruisefeetitle' . $feeId] = array (
					'#type' => 'item',
					'#markup' => $feeTitle,
					'#prefix' => '<div class="row-fluid"><div class="span4">',
					'#suffix' => '</div>' 
			);
			
			$form ['changecurise'] ['cruisefees'] ['cruisefeestype' . $feeId] = array (
					'#type' => 'item',
					'#markup' => $feeType,
					'#prefix' => '<div class="span4">',
					'#suffix' => '</div>' 
			);
			
			$form ['changecurise'] ['cruisefees'] ['cruisefeesactualrate_' . $feeId] = array (
					'#type' => 'textfield',
					'#default_value' => $fee_value,
					'#prefix' => '<div style="display:none;">',
					'#suffix' => '</div>' 
			);
			
			$form ['changecurise'] ['cruisefees'] ['cruisefeesmodifiedrate_' . $feeId] = array (
					'#type' => 'textfield',
					'#default_value' => $fee_value,
					'#prefix' => '<div class="span4">',
					'#suffix' => '</div></div>',
					'#attributes' => array (
							'class' => array (
									'input-mini' 
							) 
					) 
			);
			
			$form ['changecurise'] ['cruisefees'] ['cruisefeeid_' . $feeId] = array (
					'#type' => 'textfield',
					'#default_value' => $feeId,
					'#prefix' => '<div style="display:none;">',
					'#suffix' => '</div>' 
			);
			
			$form ['changecurise'] ['cruisefees'] ['cruisefeetitle_' . $feeId] = array (
					'#type' => 'textfield',
					'#default_value' => $feeTitle,
					'#prefix' => '<div style="display:none;">',
					'#suffix' => '</div>' 
			);
			
			$form ['changecurise'] ['cruisefees'] ['cruisefeestype_' . $feeId] = array (
					'#type' => 'textfield',
					'#default_value' => $feeType,
					'#prefix' => '<div style="display:none;">',
					'#suffix' => '</div>' 
			);
		}
	} // cruise fee items loop ends here
	
	$taxValue = variable_get ( 'WWCTAXVALUE', '' );
	
	$form ['changecurise'] ['tax'] = array (
			'#type' => 'fieldset',
			'#title' =>'<span class="icon-th-large"></span>&nbsp;'. t ( 'Tax' ),
			'#prefix' => '<div class="row-fluid"><div class="span12">',
			'#suffix' => '</div></div>',
// 			'#collapsible' => TRUE,
// 			'#collapsed' => FALSE 
	);
	
	$form ['changecurise'] ['tax'] ['taxvalue'] = array (
			'#type' => 'textfield',
			'#default_value' => $taxValue,
			'#prefix' => '<div class="row-fluid"><div class="span12">',
			'#suffix' => '</div></div>' 
	);
	
	$form ['changecurise'] ['lineitemcharge'] = array (
			'#type' => 'fieldset',
			'#title' => '<span class="icon-th-large"></span>&nbsp;'.t ( 'Line Item Charge / Discount' ),
// 			'#collapsible' => TRUE,
// 			'#collapsed' => FALSE,
			'#prefix' => '<br/><br/><div class="row-fluid" ><div class="span12">',
			'#suffix' => '</div></div>' 
	);
	
	$form ['changecurise'] ['lineitemcharge'] ['charge'] = array (
			'#type' => 'checkbox',
			'#title' => t ( 'Taxed' ),
			
			'#default_value' => '',
			'#prefix' => '<div class="row-fluid"> <div class="span1">',
			'#suffix' => '</div>' 
	);
	
	$form ['changecurise'] ['lineitemcharge'] ['chargedescription'] = array (
			'#type' => 'textfield',
			'#title' => t ( 'Charge (Description)' ),
			'#default_value' => '',
			'#prefix' => '<div class="span3">',
			'#suffix' => '</div>',
			'#attributes' => array (
					'class' => array (
							'input-mini' 
					) 
			) 
	);
	
	$form ['changecurise'] ['lineitemcharge'] ['chargetype'] = array (
			'#type' => 'select',
			'#title' => t ( 'Type' ),
			'#options' => array (
					'dollaramount' => t ( 'Dollar Amount' ),
					'percentage' => t ( 'Percentage' ) 
			),
			
			'#default_value' => '',
			'#prefix' => '<div class="span4">',
			'#suffix' => '</div>',
			'#attributes' => array (
					'class' => array (
							'input-medium' 
					) 
			) 
	)
	;
	
	$form ['changecurise'] ['lineitemcharge'] ['chargevalue'] = array (
			'#type' => 'textfield',
			'#title' => t ( 'Value' ),
			'#default_value' => '',
			'#prefix' => '<div class="span4">',
			'#suffix' => '</div></div>',
			'#attributes' => array (
					'class' => array (
							'input-mini' 
					) 
			) 
	)
	;
	
	$form ['changecurise'] ['lineitemcharge'] ['discount'] = array (
			'#type' => 'checkbox',
			'#title' => t ( 'Pre-Tax' ),
			'#default_value' => '',
			'#prefix' => '<div class="row-fluid" > <div class="span1">',
			'#suffix' => '</div>' 
	);
	
	$form ['changecurise'] ['lineitemcharge'] ['discountdescription'] = array (
			'#type' => 'textfield',
			'#title' => t ( 'Discount (Description)' ),
			'#default_value' => '',
			'#prefix' => '<div class="span3">',
			'#suffix' => '</div>',
			'#attributes' => array (
					'class' => array (
							'input-mini' 
					) 
			) 
	);
	
	$form ['changecurise'] ['lineitemcharge'] ['discounttype'] = array (
			'#type' => 'select',
			'#title' => t ( 'Type' ),
			'#options' => array (
					'dollaramount' => t ( 'Dollar Amount' ),
					'percentage' => t ( 'Percentage' ) 
			),
			'#default_value' => '',
			'#prefix' => '<div class="span4">',
			'#suffix' => '</div>',
			'#attributes' => array (
					'class' => array (
							'input-medium' 
					) 
			) 
	);
	
	$form ['changecurise'] ['lineitemcharge'] ['discountvalue'] = array (
			'#type' => 'textfield',
			'#title' => t ( 'Value' ),
			'#default_value' => '',
			'#prefix' => '<div class="span4">',
			'#suffix' => '</div></div>',
			'#attributes' => array (
					'class' => array (
							'input-mini' 
					) 
			) 
	);
	
	
	
	
	
	
	
	$form ['changecurise'] ['cartsummary'] = array (
			'#type' => 'fieldset',
			'#title' => '<span class="icon-shopping-cart"></span>&nbsp;'.t ( 'Cart Summary' ),
// 			'#collapsible' => TRUE,
// 			'#collapsed' => FALSE,
			'#prefix' => '<br/> <br/><br/><div class="row-fluid"><div class="span12">',
			'#suffix' => '</div></div>' 
	)
	;
	
	$ticketHtml = '<div id="TICKETKTDIV">';
	// $ticketHtml .= '<h3>Tickets <br/></h3>';
	$ticketHtml .= "</div>";
	
	$form ['changecurise'] ['cartsummary'] ['ticketwidget'] = array (
			'#type' => 'markup',
			'#markup' => $ticketHtml,
			'#prefix' => '<div class="row-fluid"> <div class="span12">',
			'#suffix' => '</div></div>' 
	);
	
	$enhancementHtml = '<div id="ENHANCEMENTDIV">';
	// $enhancementHtml .= '<h3>Enhancements <br/></h3>';
	$enhancementHtml .= "</div>";
	
	$form ['changecurise'] ['cartsummary'] ['enhancementwidget'] = array (
			'#type' => 'markup',
			'#markup' => $enhancementHtml,
			'#prefix' => '<div class="row-fluid"> <div class="span12">',
			'#suffix' => '</div></div>' 
	);
	
	$taxlineitemsHtml = '<div id="TAXLINTEITEMDIV">';
	// $taxlineitemsHtml .= '<h3>Cruise Fees <br/></h3>';
	$taxlineitemsHtml .= "</div>";
	
	$form ['changecurise'] ['cartsummary'] ['taxlineitemwidget'] = array (
			'#type' => 'markup',
			'#markup' => $taxlineitemsHtml,
			'#prefix' => '<div class="row-fluid"> <div class="span12">',
			'#suffix' => '</div></div>' 
	);
	
	$discountlineitemsHtml = '<div id="DISCOUNTLINEITEMMDIV">';
	// $discountlineitemsHtml .= '<h3>Discount Line Item <br/></h3>';
	$discountlineitemsHtml .= "</div>";
	
	$form ['changecurise'] ['cartsummary'] ['discountlineitemwidget'] = array (
			'#type' => 'markup',
			'#markup' => $discountlineitemsHtml,
			'#prefix' => '<div class="row-fluid"> <div class="span12">',
			'#suffix' => '</div></div>' 
	);
	
	$cruisefeeHtml = '<div id="CRUISEFEEDIV">';
	// $cruisefeeHtml .= '<h3>Cruise Fees <br/></h3>';
	$cruisefeeHtml .= "</div>";
	
	
	$coupondivhtml = '<div id="COUPONDIV">';
	$coupondivhtml .= "</div>";
	
	$form ['changecurise'] ['cartsummary'] ['couponwidget'] = array (
			'#type' => 'markup',
			'#markup' => $coupondivhtml,
			'#prefix' => '<div class="row-fluid"> <div class="span12">',
			'#suffix' => '</div></div>'
	);
	
	
	//var_dump($changeCruiseObject->order_id); exit();
	//giftcard
	$giftcardhtml='';
	$giftCardSerialized = CartSession::getGiftCard($changeCruiseObject->order_id);
	$giftcardhtml = '<div id="GIFTCARDDIV" style="margin-top:5%;">';
	//var_dump($giftCardSerialized); exit();
	if ($giftCardSerialized) {
		$giftCardArray = reset(unserialize($giftCardSerialized));
		
		$giftcardhtml.='<div class="row-fluid"><div class="span5 offset1">Gift Card </div> <div class="span6">-$'
				. wwcFormatPrice ( $giftCardArray['deductedamount'] ) . '</div></div>';
		
	}
	$giftcardhtml.='</div>';
	//var_dump($giftcardhtml); exit();

	$form ['changecurise'] ['cartsummary'] ['giftcardwidget'] = array (
			'#type' => 'markup',
			'#markup' => $giftcardhtml,
			
	);
	
	
	
	
	
	
	
	
	$form ['changecurise'] ['cartsummary'] ['cruisefeewidget'] = array (
			'#type' => 'markup',
			'#markup' => $cruisefeeHtml,
			'#prefix' => '<div class="row-fluid"> <div class="span12">',
			'#suffix' => '</div></div>' 
	);
	
	$subtotalhtml = '<div id="SUBTOTALDIV">';
	$subtotalhtml .= "</div>";
	
	$form ['changecurise'] ['cartsummary'] ['subtotalwidget'] = array (
			'#type' => 'markup',
			'#markup' => $subtotalhtml,
			'#prefix' => '<div class="row-fluid"> <div class="span12">',
			'#suffix' => '</div></div>' 
	);
	
	$taxhtml = '<div id="TAXDIV">';
	$taxhtml .= "</div>";
	
	$form ['changecurise'] ['cartsummary'] ['taxwidget'] = array (
			'#type' => 'markup',
			'#markup' => $taxhtml,
			'#prefix' => '<div class="row-fluid"> <div class="span12">',
			'#suffix' => '</div></div>' 
	);
	
	$totalhtml = '<div id="ORDERTOTALDIV">';
	$totalhtml .= "</div>";
	
	$form ['changecurise'] ['cartsummary'] ['totalwidget'] = array (
			'#type' => 'markup',
			'#markup' => $totalhtml,
			'#prefix' => '<div class="row-fluid"> <div class="span12">',
			'#suffix' => '</div></div>' 
	);
	
	$form ['changecurise'] ['buttons'] ['calculatebtn'] = array (
			'#type' => 'button',
			'#value' => 'Update Cart',
			'#ajax' => array (
					'callback' => 'calclatechangecuriseorderamount',
					'effect' => 'fade' 
			),
			'#prefix' => '<div class="row-fluid"><div class="span6">',
			'#suffix' => '</div>' 
	);
	
	$form ['changecurise'] ['buttons'] ['cancelbtn'] = array (
			'#type' => 'submit',
			'#value' => t ( 'Cancel Change Cruise' ),
			'#prefix' => '<div class="span6">',
			'#suffix' => '</div></div>',
			'#submit' => array (
					'cancelchangecruisepage' 
			) 
	);

	
	
		$form ['changecurise'] ['promoandgiftcard'] = array (
				'#type' => 'fieldset',
				'#title' => '<span class="icon-gift"></span>&nbsp;'.t ( 'Promotional Code, Gift Card' ),
				// 			'#collapsible' => TRUE,
		// 			'#collapsed' => FALSE,
				'#prefix' => '<div class="row-fluid"><div class="span12" style="margin-top:5%;">',
				'#suffix' => '</div></div>'
		);
	
		$form ['changecurise'] ['promoandgiftcard'] ['prmocode'] = array (
				'#id' => 'prmocode',
				'#type' => 'textfield',
				'#prefix' => '<div class="row-fluid"> <div class="span12"> <table><tr><td>Promocode</td><td>',
				'#suffix' => '</td>',
				'#default_value' => ''
		);
	
		$form ['changecurise'] ['promoandgiftcard'] ['prmobtn'] = array (
				'#limit_validation_errors' => array (),
				'#id'=>'prmobtn',
				'#name' => 'promocode-button-status',
				'#type' => 'button',
				'#value' => t ( 'Apply' ),
	
				'#ajax' => array (
						'callback' => 'validate_promocode'
				),
				'#prefix' => '<td>',
				'#suffix' => '</td></tr>',
				'#attributes'=>array('disabled'=>'disabled')
		);
	$form ['changecurise'] ['promoandgiftcard'] ['giftcardnumber'] = array (
			'#id' => 'prmocode',
			'#type' => 'textfield',
			'#prefix' => '<tr><td>Gift Card </td> <td>',
			'#suffix' => '</td>',
			'#default_value' => '' 
	);
	
	$form ['changecurise'] ['promoandgiftcard'] ['giftcardbtn'] = array (
			'#id' => 'giftcardbtn',
			'#limit_validation_errors' => array (),
			'#name' => 'giftcard-button-status',
			'#type' => 'button',
			'#value' => t ( 'Apply' ),
			
			'#ajax' => array (
					'callback' => 'validate_giftcard' 
			),
			'#prefix' => '<td>',
			'#suffix' => '</td></tr></table>',
			'#attributes' => array (
					'disabled' => 'disabled' 
			) 
	);
	
	$giftCardOldAmt=0;
	$giftCardSerialized = CartSession::getGiftCard($changeCruiseObject->order_id);
	if($giftCardSerialized){
		$giftCardArray = reset(unserialize($giftCardSerialized));
		$giftCardOldAmt= $giftCardArray['deductedamount'];
	}
	
	$form ['changecurise'] ['promoandgiftcard'] ['giftcardoldamt'] = array (
			'#type'=>'textfield',
			'#default_value'=>$giftCardOldAmt,
			'#prefix' => '<div style="display:none;">',
			'#suffix' => '</div>',
	);
	
	
	

	
	$form ['changecurise'] ['savecartprocessing'] ['savecartbtn'] = array (
			'#id'=>'changecruisesavecartbtn',
			'#type' => 'submit',
			'#value' => t ( 'Save Cart for Processing' ),
			'#prefix' => '<div class="row-fluid"><div class="span12">',
			'#suffix' => '</div></div>',
			// '#ajax' => array (
			// 'callback' => 'changecurisesavecartforprocessing',
			// 'effect' => 'fade'
			// ),
			
			'#submit' => array (
					'changecurisesavecartforprocessing' 
			),
			'#attributes'=>array('disabled'=>'disabled')
	);
	
	$form ['changecurise'] ['spansixcoltwoend'] = array (
			'#type' => 'markup',
			'#markup' => '</div><!--span6 ends here-->' 
	);
	
	$form ['changecurise'] ['rowfluidend'] = array (
			'#type' => 'markup',
			'#markup' => '</div> <!--row-fluid ends here--></div>' 
	);
	
	$form ['changecurise'] ['containerfluidend'] = array (
			'#type' => 'markup',
			'#sumarkupffix' => '<!--container-fluid ends here-->' 
	);
	
	return $form;
}



function changecruise_ticketentrees_callback($form, &$form_state) {
	
	// var_dump('test');
	return $form ['changecurise'] ['tickets'] ['ticketentrees_fieldset'];
}














function cancelchangecruisepage($form, $form_state) {
	
	
	
	
	$orderID = $form_state ['values'] ['hiddenorderid'];
	
	unset($_SESSION['bookingorderformflag']);
	try {
		$num_deleted = db_delete('cartsession')
		->condition('sessionid', $orderID)
		->execute();
	} catch (Exception $e) {
	
	}
	
	
	$path = "staffadmin/order/" . $orderID . "/info";
	drupal_goto ( $path );
}



function changecurisesavecartforprocessing($form, $form_state) {
	$oldOrderTicketQty = $form_state ['values'] ['oldOrderTicketQty'];
	$oldOrderWindowsEnhancementQty = $form_state ['values'] ['oldOrderEnhancementQty'];
	$oldOrderEventId = $form_state ['values'] ['oldOrderEventId'];
	$oldOrderCruiseTemplateId = $form_state ['values'] ['oldOrderCruiseTemplateId'];

	
	
	$ticketsinventoryupdresult = db_update ('scheduledevents')
	->expression ('soldtickets', 'soldtickets - :oldorderticketsqty', array (':oldorderticketsqty' => ($oldOrderTicketQty)) )
	->expression ( 'availabletickets', 'availabletickets + :oldorderticketsqty', array (':oldorderticketsqty' => ($oldOrderTicketQty)))
	->condition ( 'cruisetemplate_id', $oldOrderCruiseTemplateId, '=' )->condition ( 'id', $oldOrderEventId, '=' )->where ( 'soldtickets-' . $oldOrderTicketQty . '>=0' )->execute ();
	
	
	$enhancementsinventoryresult = db_update ( 'scheduledevents' )
	->expression ( 'windowseatssold', 'windowseatssold - :oldorderwindowenhancementsqty', array (':oldorderwindowenhancementsqty' => $oldOrderWindowsEnhancementQty) )
	->expression ( 'availablewindowseats', 'availablewindowseats + :oldorderwindowenhancementsqty', array (':oldorderwindowenhancementsqty' => $oldOrderWindowsEnhancementQty) )
	->condition ( 'cruisetemplate_id', $oldOrderCruiseTemplateId, '=' )
	->condition ( 'id', $oldOrderEventId, '=' )
	->where ( 'windowseatssold-' . $oldOrderWindowsEnhancementQty . '>=0' )
	->execute ();
	
	
	// Updating latest selected event inventory
	$orderID = $form_state ['values'] ['hiddenorderid'];
	$cruise_id = $form_state ['values'] ['hiddencruisetemplateid'];
	$event_id = $form_state ['values'] ['hiddeneventid'];
	$r_id = $form_state ['values'] ['hiddenreservationid'];
	
	// Getting Tickets Total Price
	$total_tickets_price = CartSession::getTotalTicketsPrice ( $orderID );

	
	
	// Loading the Ubercart Order
	$order = uc_order_load ( $orderID );
	
	
	
	
	$oldOrderObject = new OrderEdit ( $order->order_id );
	$oldOrderObject->deleteOrderData ();
	
	$total_ticketQty = 0;
	$selectedtickets = array ();
	$tickets = unserialize ( CartSession::getTickets ( $orderID ) );
	$total_tickets_price = CartSession::getTotalTicketsPrice ( $orderID );
	
	$currentOrderTicketQty = 0;
	foreach ( $tickets as $ticket ) {
		if ($ticket ['qty'] > 0) {
			$selectedtickets [] = $ticket;
			$currentOrderTicketQty += $ticket ['qty'];
		}
	}
	
	// Getting the Product ID of Event ID ( Event ID treated as a Product node)
	$scheduleEventObject = scheduledevents_load ( $event_id );
	$event_product_node = $scheduleEventObject->nid;
	$eventproductnodedata = node_load ( $event_product_node );
	
	$eventProduct = new stdClass ();
	$eventProduct->nid = $event_product_node;
	$eventProduct->qty = 1; // By Default Cruise Item qty is treating as single
	$eventProduct->price = $total_tickets_price;
	$eventProduct->title = $eventproductnodedata->title;
	$eventProduct->model = $eventproductnodedata->model;
	
	// $order = uc_order_new($userId, 'in_checkout'); //Creating the new Order
	$order_id = $order->order_id;
	$order->products [] = $eventProduct;
	
	
	
	$selectedenhancements = array ();
	$enhancements = unserialize ( CartSession::getEnhancements ( $orderID ) );
	$totalenhancementQty = 0;
	$currentOrderWindowsEnhancementQty = 0;
	foreach ( $enhancements as $enhancement ) {
		$totalenhancementQty += $enhancement ['qty'];
		$selectedenhancements [] = $enhancement;
		$cartenhancednode = node_load ( $enhancement ['enhancedproductnodeid'] );
		$cartenhancedproduct = uc_product_load ( $cartenhancednode );
		$cartenhancedproduct->nid = $cartenhancednode->nid;
		$cartenhancedproduct->qty = $enhancement ['qty']; // By Default Cruise Item qty is treating as single
		                                                 // $cartenhancedproduct->price=$enhancement['qty']* $enhancement['customprice'];
		$cartenhancedproduct->price = $enhancement ['price'];
		$cartenhancedproduct->title = $cartenhancednode->title;
		$cartenhancedproduct->model = $cartenhancednode->model;
		$order->products [] = $cartenhancedproduct;
		
		if (CruiseEnhancements::is_window_inventory_item ( $enhancement ['enhancedproductnodeid'] )) {
			$currentOrderWindowsEnhancementQty += $enhancement ['qty'];
		}
	}
	
	
	
	
	//Coupon Code
	$couponLineItem = unserialize(CartSession::getPromocode($order_id));
	foreach ($couponLineItem as $key => $val) {
		$cpncode = $key;
		$cpndiscount = (float) $val;
		//$order->line_items[] = uc_order_line_item_add($order->order_id, $cpncode,$cpncode,-($cpndiscount));
		$order->line_items[] = uc_order_line_item_add($order->order_id, 'generic', 'Promo Code ' . $cpncode, -($cpndiscount), _uc_line_item_data('coupon', 'weight'), array('name' => 'PROMO_CODE')
		);
	}
	
	
	
	//giftcard
	$giftCardSerialized = CartSession::getGiftCard($order_id);
	if ($giftCardSerialized != "") {
		$giftCardArray = unserialize($giftCardSerialized);
		$giftCardCode = '';
		$giftCardAmount = 0;
		$totalgiftcardprice = 0;
		$deductedamount = 0;
		foreach ($giftCardArray as $key => $values) {
			$giftCardCode = $key;
			$deductedamount = $giftCardArray[$key]['deductedamount'];
			$totalgiftcardprice = $giftCardArray[$key]['totalgiftcardprice'];
		}
		$order->line_items[] = uc_order_line_item_add($order->order_id, 'generic', 'Gift Card ' . $giftCardCode, -($deductedamount), _uc_line_item_data('giftcard', 'weight'), array('name' => 'GIFT_CARD')
		);
	}
	
	//Fee items
	$feeItems = CartSession::getCruiseFeeItems ( $orderID );
	foreach ( $feeItems as $feeitem ) {
		$feeitemname = $feeitem ['title'];
		$totalAmount = $feeitem ['totalamt'];
		// $order->line_items[] = uc_order_line_item_add($order->order_id, $feeitemname,$feeitemname,$totalAmount);
	
		if ($totalAmount != "") {
			$order->line_items [] = uc_order_line_item_add ( $order->order_id, 'generic', $feeitemname, $totalAmount, _uc_line_item_data ( 'cruise_fee_item', 'weight' ), array (
					'name' => $feeitemname
			) );
		}
	}
	
	// Tax
	$taxamt = CartSession::getTaxAmt ( $orderID );
	if ($taxamt != "") {
		$order->line_items [] = uc_order_line_item_add ( $order->order_id, 'generic', 'Tax ', ($taxamt), _uc_line_item_data ( 'Tax', 'weight' ), array (
				'name' => 'Tax'
		) );
	}
	
	
	$taxLineItemSerialized = CartSession::getTaxLineItem($order_id);
	if ($taxLineItemSerialized) {
		$taxLineItemArray = reset(unserialize($taxLineItemSerialized));
		if ($taxLineItemArray['amt'] != "") {
			$order->line_items[] = uc_order_line_item_add($order->order_id, 'generic', 'Additional Tax Line Item - ' . $taxLineItemArray['taxname'], ($taxLineItemArray['amt']), _uc_line_item_data('coupon', 'weight'), array('name' => 'ADDITIONAL_CHARGE_TAX')
			);
		}
	}
	
	
	//Line Item Discount
	$discountLineItemSerialized = CartSession::getDiscountLineItem($order_id);
	if ($discountLineItemSerialized) {
		$discountLineItemArray = reset(unserialize($discountLineItemSerialized));
		if ($taxLineItemArray['amt'] != "") {
			$order->line_items[] = uc_order_line_item_add($order->order_id, 'generic', 'Additional Discount Line Item - ' . $discountLineItemArray['discountname'], -($discountLineItemArray['amt']), _uc_line_item_data('coupon', 'weight'), array('name' => 'ADDITIONAL_DISCOUNT')
			);
		}
	}
	
	
	
	//Updating the Giftcard
	/*
	 * Gift Card
	*/
	$giftCardSerialized = CartSession::getGiftCard($order_id);
	$giftCardCode = '';
	$giftCardAmount = '';
	
	
	if (!empty($giftCardSerialized)) {
	$giftCardArray = unserialize($giftCardSerialized);
	
	foreach ($giftCardArray as $key => $values) {
	$giftCardCode = $key;
		$giftCardTotalAmt = $giftCardArray[$key]['totalgiftcardprice'];
		$giftCardAmount = $giftCardArray[$key]['paidamt'];
	}
	try {
		//Preparing GiftCard Data
		$giftCardData = array();
			$giftCardData['accountId'] = $giftCardCode;
			$giftCardData['entryType'] = 'K';
			$giftCardData['pin'] = null;
			$giftCardData['valueCode'] = 'USD';
			$giftCardData['enteredAmount'] = $giftCardAmount;
			$giftCardData['customerType'] = '1'; //primary::1,Alternate::2,Giver of Card::3
					$giftCardData['firstName'] = $order->billing_first_name;
			$giftCardData['middleName'] = "";
			$giftCardData['lastName'] = $order->billing_last_name;
			$giftCardData['address1'] = $order->billing_street1;
			$giftCardData['address2'] = "";
							$giftCardData['city'] = $order->billing_city;
								$giftCardData['state'] = uc_zone_get_by_id($order->billing_zone);
								$giftCardData['postal'] = $order->billing_postal_code;
								$giftCardData['country'] = '';
								$giftCardData['mailPref'] = 'I';
								$giftCardData['phone'] = $userphone; //$customerPhone;
								$giftCardData['isMobile'] = '';
								$giftCardData['phonePref'] = 'I'; //depends upon phone number availability if not available then it is o
								$giftCardData['email'] = $order->primary_email;
								$giftCardData['emailPref'] = 'I';
								$giftCardData['birthday'] = '';
								$giftCardData['anniversary'] = '';
								$giftCardData['gender'] = ''; //not available
										//Pre Log
										$giftCardObject = new GiftCardBookingOrder();
										$giftCardObject->giftcard_account = $giftCardCode;
										$giftCardObject->giftcard_totalamt = $giftCardTotalAmt;
										$giftCardObject->giftcard_deductedamt = $giftCardAmount;
										$giftCardObject->order_id = $order->order_id;
										$giftCardObject->uid = $userId;
										$giftCardObject->status = "Created";
										$giftCardObject->created = REQUEST_TIME;
										$giftCardObject->data = serialize($giftCardData);
	
										BookingLog::logGiftCard($giftCardObject);
	
										giftcard_profitpointapi_giftredemption("GiftRedemption", $giftCardData, $giftCardObject);
		} catch (Exception $e) {
		watchdog("Error occured in giftcard updation in booking orders", $e);
		}
		}
	
		
		
	
	
	
	
	
	
	
	// updating inventory
	Scheduledevents::updateInventory ( $cruise_id, $event_id, $currentOrderTicketQty, $currentOrderWindowsEnhancementQty );
	
	
	
	
	
	
	
	// Userchoices Log
	$userChoicesObj = new UserChoices ();
	
	$reservationData = Reservation::getReservationData ( $orderID )->fetchAll ();
	
	$id = $reservationData [0]->id;
	$bookingdate = $reservationData [0]->bookingdate;
	// $eventid=$event_id;
	$orderid = $reservationData [0]->orderid;
	$uid = $reservationData [0]->uid;
	$bookedby = $reservationData [0]->bookedby;
	$created = $reservationData [0]->created;
	
	$data = unserialize ( $reservationData [0]->data );
	$cartsessionid = $data->userchoice ['cartsessionid'];
	$producttickets = $data->userchoice ['cruiseproduct'] ['producttickets'];
	$cruiseitem = $data->userchoice ['cruiseproduct'] ['cruiseitem'];
	
	$enhacementproducts = $data->userchoice ['enhacementproducts'];
	$specialinstructions = $data->userchoice ['specialinstructions'];
	$celebrationtext = $data->userchoice ['celebrationtext'];
	$usersdata = $data->userchoice ['usersdata'];
	
	$cartsesison = user_password ( 8 ) . REQUEST_TIME . user_password ( 8 );
	$usercruiseinputs = array (
			'cartsessionid' => array (
					$cartsesison 
			),
			'cruiseproduct' => array (
					'producttickets' => $selectedtickets,
					'cruiseitem' => $cruise_id 
			),
			'enhacementproducts' => $selectedenhancements,
			'specialinstructions' => $specialinstructions,
			'celebrationtext' => $celebrationtext,
			'usersdata' => $usersdata 
	);
	
	// var_dump($usercruiseinputs); exit();
	
	$userchioce = ( object ) array (
			'userchoice' => $usercruiseinputs 
	);
	
	//
	
	// Reservation
	
	// var_dump($usercruiseinputs);
	
	$reservationObj = new stdClass ();
	// $reservationObj->bookingdate = REQUEST_TIME;
	// $reservationObj->bookingdate = strtotime($_SESSION['bookingInput']->date);
	$reservationObj->orderid = $order->order_id;
	$reservationObj->id = $r_id;
	$reservationObj->data = $userchioce;
	$reservationObj->eventid = $event_id;
	
	// drupal_json_output($reservationObj);
	// exit();
	
	$savereservation = reservation_save ( $reservationObj );
	
	uc_order_save ( $order );
	
	// $cartsesison = user_password(8) . REQUEST_TIME . user_password(8);
	
	// -> Tickets Log
	foreach ( $selectedtickets as $ticket ) {
		$ticketObj = new Tickets ();
		$ticketObj->ticket_id = $ticket ['ticketid'];
		$ticketObj->order_id = $order->order_id;
		$ticketObj->ticket_qty = $ticket ['qty'];
		$ticketObj->ticket_price = $ticket ['price'];
		$ticketObj->event_id = $reservationData [0]->eventid;
		$ticketObj->bookingtype = "";
		$id = BookingLog::logTickets ( $ticketObj );
		for($i = 0; $i < count ( $ticket ['ticketentreeids'] ); $i ++) {
			$ticketEntreesObj = new TicketEntrees ();
			$ticketEntreesObj->entree_id = ( int ) $ticket ['ticketentreeids'] [$i];
			$ticketEntreesObj->ref_id = ( int ) $id;
			$commentArray = explode ( '-', $ticket ['ticketentreescomments'] [$i] );
			$ticketEntreesObj->comments = isset ( $commentArray [2] ) ? $commentArray [2] : '';
			BookingLog::logTicketEntrees ( $ticketEntreesObj );
		}
	}
	
	// -> Enhancements Log
	foreach ( $selectedenhancements as $enhancement ) {
		$enhancementObj = new Enhancements ();
		$enhancementObj->comments = "";
		$enhancementObj->enhancement_id = $enhancement ['enhancedproductnodeid'];
		$enhancementObj->enhancement_price = $enhancement ['price'];
		$enhancementObj->enhancement_qty = $enhancement ['qty'];
		$enhancementObj->event_id = $reservationData [0]->eventid;
		$enhancementObj->order_id = $order->order_id;
		BookingLog::logEnhancements ( $enhancementObj );
	}
	
	$cart_updated = db_update ( 'cartsession' )->fields ( array (
			'sessionid' => $cartsesison 
	) )->condition ( 'sessionid', $order->order_id, '=' )->execute ();
	
	
	
	
	$path = 'staffadmin/order/' . $order->order_id . '/payment';
	drupal_goto($path);
	
	exit();
	
	// $commands=array();
	// $commands [] = ajax_command_invoke('#savecartbtn', 'hide');
	
	// return array(
	// '#type' => 'ajax',
	// '#commands' => $commands
	// );
}














function validate_promocode($form, &$form_state) {
	
	
	$order_id = $form_state ['values'] ['hiddenorderid'];
	$tickettotalamt = (float) CartSession::getTotalTicketsPrice($order_id);
	$couponamt = '0';
	$couponObj = false;
	$cpnstatus = 'This promo code is invalid or has expired.';
	$commands = array();
	
	
	
	if ($tickettotalamt > 0) {
		$cpnObject=CartSession::getPromocode($order_id);
		$discountamt = 0;
		$couponcode = $form_state ['values'] ['prmocode'];
		$event_id = $form_state ['values'] ['hiddeneventid'];
		if ($couponcode != "" || !empty($couponcode)) {
			
			
			if($cpnObject){
				$cpnstatus = "Promocode already applied.";
			}else{
				$ticketobject = CartSession::getTickets($order_id);
				try {
					$eventdata = new stdclass ();
					$eventdata->eventid = $event_id;
					$eventdata->tickets = $ticketobject;
				
					$orderobj = new stdClass ();
					$orderobj->ordertype = "booking";
					$orderobj->events [] = $eventdata;
					$orderobj->ordersubtotal = $tickettotalamt;
					$orderobj->coupontype = "coupon";
					$couponObj = entity_get_controller('coupon')->applycoupon($couponcode, $orderobj);
				
					$discountamt = $couponObj [0]->discountamount;
					if ($discountamt > 0) {
						$promocode = array($couponcode => $discountamt);
						CartSession::insertPromoCode($order_id, $promocode);
						$cpnstatus = "Promocode has been applied to your order.";
				
						//Update Order Total
						$totalOrderAmount = CartSession::getOrderTotal($order_id);
						$updatedTotalAmt = $totalOrderAmount - $discountamt;
							
						CartSession::insertOrderTotal($order_id, $updatedTotalAmt);
						$coupondivtext = '-$' . wwcFormatPrice($discountamt);
						$totalamtdivtext = '$' . wwcFormatPrice($updatedTotalAmt);
							
						if ($discountamt) {
							$cpndiscounthtml = '<div id="COUPONDIV" style="margin-top:5%;">';
							$cpndiscounthtml.='<div class="row-fluid"><div class="span5 offset1">Promocode '.$couponcode.'  </div> <div class="span6">-$'
									. wwcFormatPrice($discountamt) . '</div></div>';
							$cpndiscounthtml.='</div>';
						}
						$commands [] = ajax_command_replace("#COUPONDIV", "<div id='COUPONDIV'>" . $cpndiscounthtml . "</div>");
							
							
						//Order Total
						$totalhtml = '<div id="ORDERTOTALDIV" style="margin-top:5%;">';
						$orderTotal = CartSession::getOrderTotal($order_id);
						if ($orderTotal != "") {
							$totalhtml.='<div class="row-fluid"><div class="span5 offset1"> <b>Total</b>'
									. ' </div> <div class="span6">$' . wwcFormatPrice($orderTotal) . '</div></div>';
						}
							
						$totalhtml.='</div>';
						$commands [] = ajax_command_replace("#ORDERTOTALDIV", "<div id='ORDERTOTALDIV'>" . $totalhtml . "</div>");
						$commands [] = ajax_command_invoke('#prmobtn', 'attr', array('disabled', 'disabled'));
					} else {
						$cpnstatus = 'Invalid Promocode';
					}
				} catch (Exception $e) {
				
				}
			}
			
		} else {
			$cpnstatus = 'Please enter valid promocode';
		}
	} else {
		$cpnstatus = 'The promo code could not be used for your purchase';
	}
	
	$commands[] = ajax_command_alert($cpnstatus);
	
	return array(
			'#type' => 'ajax',
			'#commands' => $commands
	);
}




function validate_giftcard($form, $form_state) {
	
	$giftcardoldamt = $form_state ['values'] ['giftcardoldamt'];
	
	$order_id = $form_state ['values'] ['hiddenorderid'];
	$couponcode = $form_state ['values'] ['prmocode'];
	$event_id = $form_state ['values'] ['hiddeneventid'];

	$commands = array();
	$cardNumber = $form_state ['values'] ['giftcardnumber'];  //2
	if ($cardNumber != "" || !empty($cardNumber)) {
		$totalAmount = (float) CartSession::getOrderTotal($order_id);
	
		//var_dump($totalAmount); exit();
		if ($totalAmount > 0) {
			$requestType = 'Inquiry';
			$entryType = 'K';
			$giftcardbalancecheckers = giftcard_profitpointapi_balancechecker($requestType, $entryType, $cardNumber); //5
			$giftcardstatus = 'This Gift Card code is invalid or has expired.';
			if ($giftcardbalancecheckers ['message'] == 'sucess') {
				$giftcardamount = (float) $giftcardbalancecheckers ['results'] ['amount'];
	
				//var_dump($giftcardamount); exit();
				if ($giftcardamount > 0) {
					$giftcardArray = '';
					$deductgiftcardamt = 0;
					if ($giftcardamount > $totalAmount) {
						$deductgiftcardamt = $totalAmount;
						$giftcardArray = array($cardNumber => array('totalgiftcardprice' => sprintf("%.2f",$giftcardamount), 'deductedamount' => sprintf("%.2f",($totalAmount+$giftcardoldamt)),'paidamt' => sprintf("%.2f",$totalAmount)));
					} else {
						$deductgiftcardamt = $giftcardamount;
						$giftcardArray = array($cardNumber => array('totalgiftcardprice' => sprintf("%.2f",$giftcardamount), 'deductedamount' => sprintf("%.2f",($giftcardamount+$giftcardoldamt)),'paidamt' => sprintf("%.2f",$giftcardamount)));
					}
					CartSession::insertGiftCard($order_id, $giftcardArray); //7
	
	
					//var_dump($giftcardArray); exit();
					$giftcarddivtext = '-$' . wwcFormatPrice($deductgiftcardamt);
					$updatedAmount = $totalAmount - $deductgiftcardamt;
	
	
					CartSession::insertOrderTotal($order_id, $updatedAmount);
					$totalamtdivtext = '$' . wwcFormatPrice($updatedAmount);
	
	
					$giftcardstatus = "$ ".$deductgiftcardamt. " Gift card has been applied to your order.";
					$giftcardhtml = '<div id="GIFTCARDDIV" style="margin-top:5%;">';
					$giftcardhtml.='<div class="row-fluid"><div class="span5 offset1">Gift Card </div> <div class="span6">-$'
							. wwcFormatPrice ($deductgiftcardamt+$giftcardoldamt ) . '</div></div>';
					$giftcardhtml.='</div>';
	
	
					//Order Total
					$totalhtml = '<div id="ORDERTOTALDIV" style="margin-top:5%;">';
					$totalhtml.='<div class="row-fluid"><div class="span5 offset1"> <b>Total</b>'
							. ' </div> <div class="span6">$' . wwcFormatPrice($updatedAmount) . '</div></div>';
					$totalhtml.='</div>';
	
	
	
	
	
					$commands [] = ajax_command_invoke('#giftcardbtn', 'attr', array('disabled', 'disabled'));
	
					//$commands [] = ajax_command_invoke('#giftcardmaindiv', 'show');
					//$commands [] = ajax_command_replace("#giftcardamountdiv", "<div id='giftcardamountdiv'>" . $giftcarddivtext . "</div>");
					$commands [] = ajax_command_replace("#GIFTCARDDIV", "<div id='GIFTCARDDIV'>" . $giftcardhtml . "</div>");
					$commands [] = ajax_command_replace("#ORDERTOTALDIV", "<div id='ORDERTOTALDIV'>" . $totalhtml . "</div>");
	
				} else {
				$giftcardstatus = 'No balance exists for this card';
				}
				} else {
				// $commands[] = ajax_command_replace("#giftcardwidget", "<div id='giftcardwidget'>" . '' . "</div>");
				}
	
		}
	} else {
	$giftcardstatus = 'Please Enter Valid Gift Card Number';
		}
		
		$commands[] = ajax_command_alert($giftcardstatus);
	return array(
			'#type' => 'ajax',
			'#commands' => $commands
	);
}











//Calc fun
function calclatechangecuriseorderamount($form, $form_state) {


	$taxvalue = $form_state ['values'] ['taxvalue'];
	$event_id = $form_state ['values'] ['hiddeneventid'];
	$r_id = $form_state ['values'] ['hiddenreservationid'];
	$order_id = $form_state ['values'] ['hiddenorderid'];
	$cruise_id = $form_state ['values'] ['hiddencruisetemplateid'];
	
	$giftcardoldamt = $form_state ['values'] ['giftcardoldamt'];


	$availabletickets = Scheduledevents::getAvailableTicketsCount ( $cruise_id, $event_id );
	$availablewindows = Scheduledevents::getAvailableWindowsCount ( $cruise_id, $event_id );

	$result = array ();
	$totalresults = array ();
	$arr = array ();
	$totalticketsprice = 0;
	$tickets_tax = 0;
	$feeitems_tax = 0;


	//**********************  TICKETS *****************************
	foreach ( $form_state ['values'] as $key => $value ) {
		$reg1 = "/^(ticketentrees)?\d+$/";
		if (preg_match ( $reg1, $key )) {
			if ($form_state ['values'] [$key] > 0) {
				$ticketenteeeid = explode ( 'ticketentrees', $key );
				$result = array ();
				$ticketID = $ticketenteeeid [1];
				$result ['ticketid'] = $ticketenteeeid [1];
				$result ['qty'] = $form_state ['values'] [$key];
				$result ['ticketname'] = $form_state ['values'] [$key . "ticketname"];
				$result ['price'] = $form_state ['values'] [$key . "price"];

				$cnt += ( int ) $form_state ['values'] [$key];

				// ---------------Ticket Entrees----------------
				$entrees = array ();
				for($i = 0; $i < $result ['qty']; $i ++) {
					$entrees [] = $form_state ['values'] ['select' . $result ['ticketid'] . $i];
				}
				$result ['ticketentreeids'] = $entrees;

				// ------------Ticket Entrees Comments (Dietary Restrictions)--------------
				$entreesComments = array ();
				for($i = 0; $i < $result ['qty']; $i ++) {
					$entreesComments [] = $form_state ['values'] ['select' . $result ['ticketid'] . $i] . '-' . $i . '-' . $form_state ['values'] ['dietarycomments' . $result ['ticketid'] . $i];
				}

				// -------- Ticket Entree Comments ends here--------------------
				$result ['ticketentreescomments'] = $entreesComments;

				$taxableStatus = CruiseTickets::isTaxableTicket ( $ticketID );
				if ($taxableStatus) {
					$tickets_tax += $form_state ['values'] [$key] * $form_state ['values'] [$key . "price"];
				}
				$totalresults [] = $result;
				$totalticketsprice += $form_state ['values'] [$key] * $form_state ['values'] [$key . "price"];
			}
		}
	} // foreach



	
	 
	// ****************  ENHANCEMENT PROCESS  *********************
	$enhancementtotalprice = 0;
	$window_cnt = 0;
	$enhancementsresult = array ();
	foreach ( $form_state ['values'] as $key => $value ) {
		// Key -> ticketentrees2
		$reg1 = "/^(enhancement)?\d+$/";
		if (preg_match ( $reg1, $key )) {
			$enhancementid = explode ( 'enhancement', $key );
			$eid = $enhancementid [1];
			if ($form_state ['values'] ['qty' . $eid] > 0) {
				if (CruiseEnhancements::is_window_inventory_item ( $eid )) {
					$window_cnt += $form_state ['values'] ['qty' . $eid];
				}
				$result = array ();
				$result ['enhancedproductnodeid'] = $eid;
				$result ['qty'] = $form_state ['values'] ['qty' . $eid];
				$result ['price'] = $form_state ['values'] ['price' . $eid];
				$result ['name'] = $form_state ['values'] ['_enhancementtitle' . $eid];
				$productprice = $result ['qty'] * ( float ) $result ['price'];
				$enhancementtotalprice += $productprice;
				$enhancementsresult [] = $result;
			}
		}
	} // foreach
	 


	
	
	//**************** Validating Tickets *****************************
	if ($cnt > $availabletickets) {
		$commands [] = ajax_command_alert ( $availabletickets . " tickets are only available!" );
		$commands [] = ajax_command_invoke ( '#changecruisesavecartbtn', 'hide' );
	}

	else if ($window_cnt > $availablewindows) {
		$commands [] = ajax_command_alert ( $availablewindows . " windows are only available!" );
		$commands [] = ajax_command_invoke ( '#changecruisesavecartbtn', 'hide' );
	}
	else {
		CartSession::insertTickets ( $order_id, $totalresults );
		CartSession::insertTotalTicketsPrice ( $order_id, $totalticketsprice );


		
		CartSession::removePromocode($order_id);
		CartSession::removePromocode($order_id);
		
		$cpndiscounthtml='';
			
			
		//**** Fee Items
		$cruisefeeitems = array ();
		$ticket_tax_feeids = array ();
		$cruiseFeeItemPrice = 0;
		foreach ( $form_state ['values'] as $key => $value ) {
			$reg1 = "/^(cruisefeeid_)?\d+$/";
			if (preg_match ( $reg1, $key )) {
				$arr = explode ( 'cruisefeeid_', $key );
				$feeid = $arr [1];
				$title = $form_state ['values'] ['cruisefeetitle_' . $feeid];
				$updatedRate = $form_state ['values'] ['cruisefeesmodifiedrate_' . $feeid];
				$actualRate = $form_state ['values'] ['cruisefeesactualrate_' . $feeid];
				$type = $form_state ['values'] ['cruisefeestype_' . $feeid];

				// calculating total tickets price
				$tickets = unserialize ( CartSession::getTickets ( $order_id ) );

				$tickets_tax = 0;
				$chargeamt = 0;
				foreach ( $tickets as $ticket ) {
					if ($ticket ['qty'] > 0) {
						$ticket_id = $ticket ['ticketid'];
						$ticket_qty = $ticket ['qty'];
						$ticket_price = $ticket ['price'];

						$ticket_feeids = Fees::get_ticket_feeids ( $ticket_id );

						if (in_array ( $feeid, $ticket_feeids )) {
							$ticketfeeids [] = $feeid;
								
							if ($type == "flatfee") {
								$m_feevalue = $updatedRate;
								$u_feeValue = ($m_feevalue != '') ? $m_feevalue : $actualRate;
								$totalcharge = $ticket_qty * $u_feeValue;
								$chargeamt += $totalcharge;
							}
								
							if ($type == "percentage") {
								$totalticketpriceamt = $ticket_qty * $ticket_price;
								// Cruise Fee Updated Price
								$m_feevalue = $updatedRate;
								$u_feeValue = ($m_feevalue != '') ? $m_feevalue : $actualRate;
								$chargeamt += ($totalticketpriceamt * $u_feeValue) / 100;
							}
						}

						$istaxableTicket = CruiseTickets::isTaxableTicket ( $ticket_id );
						if ($istaxableTicket) {
							$tickets_tax += $ticket_qty * $ticket_price;
						}
					}
				}

				$cruiseFeeItemPrice += $chargeamt;
				$cruisefeeitems [$feeid] = array (
						'feeid' => $feeid,
						'title' => $title,
						'type' => $type,
						'actualrate' => $actualRate,
						'updatedrate' => $updatedRate,
						'totalamt' => $chargeamt
				);
			}
		}
		CartSession::insertCruiseFeeItems ( $order_id, $cruisefeeitems );

		$feeitems_tax = 0;
		foreach ( $cruisefeeitems as $key => $values ) {
			$feeid = $values ['feeid'];
			if (in_array ( $feeid, $ticketfeeids )) {
				$feeTaxableStatus = Fees::is_taxable_cruisefee_item ( $feeid );
				if ($feeTaxableStatus) {
					$feeitems_tax += $values ['totalamt'];
				}
			}
		}

			
		CartSession::insertEnhancements ( $order_id, $enhancementsresult );
		CartSession::insertTotalEnhancementsPrice ( $order_id, $enhancementtotalprice );

		
		


		// Tax and Discount Line Items
		$totalTicketsamt = $totalticketsprice;
		$chargeTax = 0;
		$chargeAmt = 0;
		$chargetype = $form_state ['values'] ['chargetype'];
		$charge = $form_state ['values'] ['charge'];
		$chargedescription = $form_state ['values'] ['chargedescription'];
		$chargevalue = is_numeric ( $form_state ['values'] ['chargevalue'] ) ? $form_state ['values'] ['chargevalue'] : 0;

		$chargeTaxName = '';
		$chargeTaxName = $chargedescription;
		if ($chargetype == "dollaramount") {
			// checkbox is checked and type is dollar amount
			if ($charge == 1) {
				// done
				$chargeTax = $chargevalue;
				$chargeTaxValue = $chargeTax;
			} // checkbox unchecked and type is dollar amount
			else {
				$chargeAmt = $chargevalue;
				$chargeTaxValue = $chargeAmt;
			}
		} else {
			// PERCENTAGE
			// checkbox checked and type is percentage
			if ($charge == 1) {

				$percentagefee = ($totalTicketsamt + $enhancementtotalprice) * $chargevalue / 100;
				$chargeTax = $percentagefee;
				$chargeTaxValue = $chargeTax;
			}// checkbox unchecked and type is percentage
			else {
				$percentagefee = ($totalTicketsamt + $enhancementtotalprice) * $chargevalue / 100;
				$chargeAmt = $percentagefee;
				$chargeTaxValue = $chargeAmt;
			}
		}
		
		

		//  Charge
		$tax_lineitem = array ();
		$tax_lineitem [$chargeTaxName] = array (
				'taxname' => $chargeTaxName,
				'chargetype' => $chargetype,
				'checkbox' => $charge,
				'amt' => $chargeTaxValue
		);
		CartSession::insertTaxLineItem ( $order_id, $tax_lineitem );

		
		
		
		//  Discounts
		$discounttype = $form_state ['values'] ['discounttype'];
		$discount = $form_state ['values'] ['discount'];
		$discountdescription = $form_state ['values'] ['discountdescription'];
		$discountvalue = is_numeric ( $form_state ['values'] ['discountvalue'] ) ? $form_state ['values'] ['discountvalue'] : 0;
		$descriptionDiscountName = $discountdescription;

		$discountTax = 0;
		$discountAmt = 0;
		// checkbox checked and type is dollaramount
		if ($discounttype == "dollaramount") {
			if ($discount == 1) {
				$discountTax = $discountvalue;
				$descriptionDiscountValue = $discountTax;
			}
			// checkbox unchecked and type is dollar amount
			else {
				$discountAmt = $discountvalue;
				$descriptionDiscountValue = $discountAmt;
			}
		} else {
			// percentage
			// checkbox checked and type is percentage
			if ($discount == 1) {
				$percentagefee = ($totalTicketsamt + $enhancementtotalprice) * $discountvalue / 100;
				$discountTax = $percentagefee;
				$descriptionDiscountValue = $discountTax;
			} else {
				// checkbox unchecked adn type is percentage
				$percentagefee = ($totalTicketsamt + $enhancementtotalprice) * $discountvalue / 100;
				$discountAmt = $percentagefee;
				$descriptionDiscountValue = $discountAmt;
			}
		}
		// insertion
		$discount_lineitem = array ();
		$discount_lineitem [$descriptionDiscountName] = array (
				'discountname' => $descriptionDiscountName,
				'discounttype' => $discounttype,
				'checkbox' => $discount,
				'amt' => $descriptionDiscountValue
		);
		CartSession::insertDiscountLineItem ( $order_id, $discount_lineitem );

			

		$orderSubTotal = $totalticketsprice + $enhancementtotalprice + $cruiseFeeItemPrice + $chargeTax + $chargeAmt - $discountTax - $discountAmt;
		CartSession::insertOrderSubTotal ( $order_id, $orderSubTotal );

		$taxarr = array ();
		$taxamt = sprintf ( "%.2f",(($tickets_tax + $feeitems_tax + $enhancementtotalprice + $chargeTax) - $discountTax) * $taxvalue / 100);
		$taxarr ['taxvalue'] = $taxvalue;
		$taxarr ['tickets_tax'] = $tickets_tax;
		$taxarr ['feeitems_tax'] = $feeitems_tax;
		$taxarr ['enhancements_tax'] = $enhancementtotalprice;
		$taxarr ['total_tax'] = $taxamt;
		CartSession::insertTaxItems ( $order_id, serialize ( $taxarr ) );


		
		
		
		//giftcard
		$giftcardhtml='';
		if ($giftcardoldamt>0) {
			$giftcardhtml = '<div id="GIFTCARDDIV" style="margin-top:5%;">';
			$giftcardhtml.='<div class="row-fluid"><div class="span5 offset1">Gift Card </div> <div class="span6">-$'
					. wwcFormatPrice ( $giftcardoldamt ) . '</div></div>';
			$giftcardhtml.='</div>';
		}
		$commands [] = ajax_command_replace("#GIFTCARDDIV", "<div id='GIFTCARDDIV'>" . $giftcardhtml . "</div>");
	
		$orderTotalPrice = $orderSubTotal + $taxamt-$cpndiscountamt-$giftcardoldamt;
		CartSession::insertOrderTotal ( $order_id, $orderTotalPrice );
		
		
		//***   1 Tickets Div ****
		if (CartSession::getTickets ( $order_id )) {
			$tickets = unserialize(CartSession::getTickets($order_id));
			$ticketHtml = '<div id="TICKETKTDIV">';
			$ticketHtml .= '<div class="row-fluid"><div class="span11 offset1"><b>Tickets</b> </div></div>';
			foreach ($tickets as $ticket) {
				if ($ticket ['qty'] > 0) {
					$amount = $ticket['qty'] * $ticket['price'];
					$ticketHtml .= '<div class="row-fluid"><div class="span5 offset1">'
							. $ticket['qty'] . '- ' . $ticket ['ticketname'] . '</div><div class="span6">$'
									. wwcFormatPrice($amount) . '</div></div>';
				}
			}
			$ticketHtml .= "</div>";
		}



		//*** 2 Enhancements Div *****
		$enhancementhtml = '<div id="ENHANCEMENTDIV" style="margin-top:5%;">';
		$enhancementhtml .= '<div class="row-fluid"><div class="span11 offset1" ><b>Enhancements</b></div></div>';
		$enhancmentsitems = unserialize(CartSession::getEnhancements($order_id));
		foreach ($enhancmentsitems as $enhancmentwidget) {
			$amount = $enhancmentwidget['qty'] * $enhancmentwidget['price'];
			$enhancementhtml.='<div class="row-fluid"><div class="span5 offset1">'
					. $enhancmentwidget['qty'] . '-' . $enhancmentwidget['name']
					. '</div><div class="span6">$' . wwcFormatPrice($amount) . '</div></div>';
		}
		$enhancementhtml.='</div>';



		//*** 3 Cruise Fee Line Items Div *****
		$cruiseFeeItems = CartSession::getCruiseFeeItems($order_id);
		$cruisefeehtml = '<div id="CRUISEFEEDIV" style="margin-top:5%;">';
		$cruisefeehtml .= '<div class="row-fluid"><div class="span11 offset1"><b>Cruise Fee</b></div></div>';
		foreach ($cruiseFeeItems as $item) {
			$chargeamt = $item['totalamt'];
			if ($chargeamt > 0) {
				$cruisefeehtml.='<div class="row-fluid"><div class="span5 offset1">'
						. $item['title'] . '</div><div class="span6"> $'
								. wwcFormatPrice($chargeamt) . '</div></div>';
			}
		}
		$cruisefeehtml.='</div>';


		//*** 4 tax Line Item
		$taxLineItemSerialized = CartSession::getTaxLineItem($order_id);
		if ($taxLineItemSerialized) {
			$taxlineitemsHtml = '<div id="TAXLINTEITEMDIV">';
			$taxLineItemArray = unserialize($taxLineItemSerialized);
			foreach ($taxLineItemArray as $key => $values) {
				if ($values['amt'] > 0) {
					$taxlineitemsHtml.='<div class="row-fluid"><div class="span5 offset1">'
							. $values['taxname'] . '</div><div class="span6">$' . wwcFormatPrice($values['amt'])
							. '</div></div>';
				}
			}
			$taxlineitemsHtml.='</div>';
			$commands [] = ajax_command_replace("#TAXLINTEITEMDIV", "<div id='TAXLINTEITEMDIV'>" . $taxlineitemsHtml . "</div>");
		}

		//*** 5discount line item
		$discountLineItemSerialized = CartSession::getDiscountLineItem($order_id);
		if ($discountLineItemSerialized) {
			$discountlineitemHtml = '<div id="DISCOUNTLINEITEMMDIV">';
			$discountLineItemArray = unserialize($discountLineItemSerialized);
			foreach ($discountLineItemArray as $key => $values) {
				if ($values['amt'] > 0) {
					$discountlineitemHtml.='<div class="row-fluid"><div class="span5 offset1">'
							. $values['discountname'] . '</div><div class="span6">-$'
									. wwcFormatPrice($values['amt']) . '</div></div>';
				}
			}
			$discountlineitemHtml.='</div>';
			$commands [] = ajax_command_replace("#DISCOUNTLINEITEMMDIV", "<div id='DISCOUNTLINEITEMMDIV'>" . $discountlineitemHtml . "</div>");
		}


		//** 6 Sub Total
		$orderSubTotal = CartSession::getOrderSubTotal($order_id);
		$subtotalhtml = '<div id="SUBTOTALDIV" style="margin-top:5%;">';
		if ($orderSubTotal != "") {
			$subtotalhtml.='<div class="row-fluid"><div class="span5 offset1">Subtotal  </div> <div class="span6">$'
					. wwcFormatPrice($orderSubTotal) . '</div></div>';
		}
		$subtotalhtml.='</div>';



		//**  Tax Total
		$taxamthtml = '<div id="TAXDIV">';
		$taxAmt = CartSession::getTaxAmt($order_id);
		if ($taxAmt != "") {
			$taxamthtml.='<div class="row-fluid"><div class="span5 offset1">'
					. ' Tax </div><div class="span6">$'
							. wwcFormatPrice($taxAmt) . '</div></div>';
		}
		$taxamthtml.='</div>';






		//Order Total

		$orderTotal = CartSession::getOrderTotal($order_id);
		$totalhtml = '<div id="ORDERTOTALDIV" style="margin-top:5%;">';
		$totalhtml.='<div class="row-fluid"><div class="span5 offset1"> <b>Total</b>'
				. ' </div> <div class="span6">$' . wwcFormatPrice($orderTotal) . '</div></div>';
		$totalhtml.='</div>';
	}
	$ticketsamt=CartSession::getTotalTicketsPrice($order_id);
	if($ticketsamt>0){
		$commands [] = ajax_command_replace("#TICKETKTDIV", "<div id='TICKETKTDIV'>" . $ticketHtml . "</div>");
		$commands [] = ajax_command_replace("#ENHANCEMENTDIV", "<div id='ENHANCEMENTDIV'>" . $enhancementhtml . "</div>");
		$commands [] = ajax_command_replace("#CRUISEFEEDIV", "<div id='CRUISEFEEDIV'>" . $cruisefeehtml . "</div>");
		$commands [] = ajax_command_replace("#SUBTOTALDIV", "<div id='SUBTOTALDIV'>" . $subtotalhtml . "</div>");
		$commands [] = ajax_command_replace("#TAXDIV", "<div id='TAXDIV'>" . $taxamthtml . "</div>");
		$commands [] = ajax_command_replace("#ORDERTOTALDIV", "<div id='ORDERTOTALDIV'>" . $totalhtml . "</div>");
		$commands [] = ajax_command_replace("#COUPONDIV", "<div id='COUPONDIV'>" . $cpndiscounthtml . "</div>");
			
		//Enabling the Promocode apply button
		$commands [] = ajax_command_invoke('#prmobtn', 'attr', array('disabled', false));
		$commands [] = ajax_command_invoke('#giftcardbtn', 'attr', array('disabled', false));
		$commands [] = ajax_command_invoke('#changecruisesavecartbtn', 'attr', array('disabled', false));
			
			
			
			
			
	}else{
		$commands[] = ajax_command_alert("Please Select Tickets!");
		$commands [] = ajax_command_replace("#TICKETKTDIV", "<div id='TICKETKTDIV'></div>");
		$commands [] = ajax_command_replace("#ENHANCEMENTDIV", "<div id='ENHANCEMENTDIV'></div>");
		$commands [] = ajax_command_replace("#CRUISEFEEDIV", "<div id='CRUISEFEEDIV'></div>");
		$commands [] = ajax_command_replace("#SUBTOTALDIV", "<div id='SUBTOTALDIV'></div>");
		$commands [] = ajax_command_replace("#TAXDIV", "<div id='TAXDIV'></div>");
		$commands [] = ajax_command_replace("#ORDERTOTALDIV", "<div id='ORDERTOTALDIV'></div>");
		$commands [] = ajax_command_replace("#COUPONDIV", "<div id='COUPONDIV'>" . $cpndiscounthtml . "</div>");
		$commands [] = ajax_command_invoke('#prmobtn', 'attr', array('disabled', true));
		$commands [] = ajax_command_invoke('#giftcardbtn', 'attr', array('disabled', true));
		$commands [] = ajax_command_invoke('#changecruisesavecartbtn', 'attr', array('disabled', true));
		
			
	}
	return array (
			'#type' => 'ajax',
			'#commands' => $commands
	);
}
