<?php

/*
 * ------------------Agent Booking------------------------
* Booking Form Stages
* 1)TICKETS
* 2)ENHANCEMENTS
* 3)PAYMENT
* 4)REVIEWORDER
* 5)CONFIRMATION
*/


function bookingagent_form($form, &$form_state)
{
 	drupal_add_css(drupal_get_path('module', 'bookingagent') . '/css/bookingagent-mediaqueries.css');
    drupal_add_css(drupal_get_path('module', 'bookingagent') . '/css/bookingagent.css');
    drupal_add_js(drupal_get_path('module', 'bookingagent') . '/js/bookingagent.js');
    drupal_add_js(drupal_get_path('module', 'booking') . '/js/mask.js');
    
    if (!isset($_SESSION['bookingInput'])) {
    	drupal_goto("wwccalendar");
    	exit();
    }
    
    if (! isset($form_state['stage'])) {
        $form_state['stage'] = 'TICKETS';
    }


    $cartsesison = user_password(8) . REQUEST_TIME . user_password(8);
    $form = bookingagent_get_header($form, $form_state);
	switch ($form_state ['stage']) {
		
		case 'TICKETS' :
			if (! isset ( $_SESSION ['cruisecardsession'] )) {
				$_SESSION ['cruisecardsession'] = $cartsesison;
			}
			return bookingagent_tickets_form ( $form, $form_state );
		
		case 'ENHANCEMENTS' :
			return bookingagent_enhancements_form ( $form, $form_state );
		
		case 'PAYMENT' :
			return bookingagent_payments_form ( $form, $form_state );
		
		case 'REVIEW_ORDER' :
			return bookingagent_review_form ( $form, $form_state );
		
		default :
			return bookingagent_tickets_form ( $form, $form_state );
	}
    return $form;
}









//***********************  Page 1: Tickets(form) Starts here  ***************************
function bookingagent_tickets_form($form, &$form_state) {

	$values = isset($form_state['multistep_values']['TICKETS']) ? $form_state['multistep_values']['TICKETS'] : array();
    $inputbookingdata = $_SESSION['bookingInput'];
    $eventid = $inputbookingdata->eventid;
    $cruisetemplateid = $inputbookingdata->templateid;

    //Getting the Available tickets based on cruise id
    $availableTickets = getCruiseTicketByWeight($cruisetemplateid);
    $form['stepone'] = array(
        '#type' => 'fieldset',
        '#prefix' => '<div class="row-fluid">',
        '#suffix' => '</div>',
        '#title' => ''
    );

    $form['stepone']['TICKETS'] = array(
        '#type' => 'fieldset',
        '#prefix' => '<div class="span7">',
        '#suffix' => '</div>',
        '#description' => '<div class="bookingSubTitle">Advanced purchase required on all dining cruises</div><div class="purchasecontent">Select the number of tickets you would like to purchase. To purchase tickets for groups of eight or more guests, to reserve an entire deck for a private event, or to book a private yacht charter, go to</div><div class="groupresButton"><a href="#myModalrequest" role="button" data-toggle="modal"><b>GROUP RESERVATIONS</b></a> </div><div class="cruisedetailviewBtn"><b><a target="_blank" href="cruisesdetailview/category/' . $inputbookingdata->templateid . '">CRUISE DESCRIPTION</a></b></div>'
    );

    // Order Summary Widget
    $ticketsoutput = orderSummaryWidget('TICKETS');
    $form['stepone']['TICKETSORDERWIDGET'] = array(
        '#type' => 'fieldset',
        '#prefix' => '<div class="span5">',
        '#suffix' => '</div>',
        '#value' => $ticketsoutput
    );

    //Ticketentrees
    $ticketEntrees = array();
    foreach ($availableTickets as $ticket) {
        $data = Cruise::getTicketEntrees($ticket->id, $cruisetemplateid);
        $arr = array('' => '--Select--');
        foreach ($data as $key => $val) {
            $arr[$key] = $val;
            $ticketEntrees[$ticket->id] = $arr;
        }
    }

    //Preparing the Drop Down for the Available Tickets
    foreach ($availableTickets as $ticket) {
        $price = isset($values['ticketentrees' . $ticket->id . 'price']) ? $values['ticketentrees' . $ticket->id . 'price'] : $ticket->price;
        $fprice = sprintf("%.2f", $price);
        $defaultvalue = isset($values['ticketentrees' . $ticket->id]) ? $values['ticketentrees' . $ticket->id] : '';
        $ticketlabel = $ticket->title . " $" . _wwcFormatPrice($ticket->price);

        $form['stepone']['TICKETS']['ticketentrees' . $ticket->id] = array(
            '#type' => 'select',
            '#prefix' => '<div class="bookfield form-horizontal">',
            '#title' => t($ticketlabel),
            '#suffix' => '</div>',
//             '#options' => array(
//                 0 => 0,
//                 1 => 1,
//                 2 => 2,
//                 3 => 3,
//                 4 => 4,
//                 5 => 5,
//                 6 => 6,
//                 7 => 7,
//             	8 => 8,
//             	9 => 9,
//             ),
        	'#options' =>   drupal_map_assoc(range(0, 50)),
            '#default_value' => $defaultvalue,
            '#attributes' => array('class' => array('ticketpriceqty')),
            '#ajax' => array(
                'callback' => '_ticketentrees_callback',
                'wrapper' => 'ticketentreesDiv',
                'method' => 'html',
                'effect' => 'fade'
            )
        );

        $form ['stepone'] ['TICKETS'] ['ticketentrees' . $ticket->id . "ticketname"] = array(
            '#type' => 'textfield',
            '#default_value' => $ticket->title,
            '#prefix' => '<div style="display:none;">',
            '#suffix' => '</div>'
        );



        $form['stepone']['TICKETS']['ticketentrees' . $ticket->id . "price"] = array(
            '#type' => 'textfield',
            '#title' => t($ticket->title . ' Price:'),
            '#attributes' => array('class' => array('input-mini')),
            '#default_value' => $fprice
        );


        $form['stepone']['TICKETS']['ticketentreesoption' . $ticket->id] = array(
            '#type' => 'hidden',
            '#value' => $ticket->id,
        );
    }//foreach



    $form['stepone']['TICKETS']['ticketentrees_fieldset'] = array(
        '#type' => 'markup',
        '#prefix' => '<div id="ticketentreesDiv">',
        '#suffix' => '</div>',
        '#weight' => 2,
    );

    //Displaying the ticketentrees 
    $displayoption = "none;";
    foreach ($availableTickets as $ticket) {
        $numoftickets = 0;
        if ($form_state['values']['ticketentrees' . $ticket->id] != '') {
            $numoftickets = $form_state['values']['ticketentrees' . $ticket->id];
        } else if ($values['ticketentrees' . $ticket->id]) {
            $numoftickets = $values['ticketentrees' . $ticket->id];
        }
        //Showing the div if number of tickets > 0
        if ($numoftickets > 0) {
            $displayoption = "block;";
            break;
        }
    }

    $form['stepone']['TICKETS']['ticketentrees_fieldset']['entrystart'] = array(
        '#type' => 'markup',
        '#markup' => '<div class="row-fluid" id="entreestartdiv" style="float:left;display:' . $displayoption . '"><div class="span12 purchaseTicketDetails">',
    );



    $entrhtml .= '<table><tr>';
    $entrhtml .= '<td class="bookingagentheader2">Quantity</td>';
    $entrhtml .= '<td class="bookingagentheader2">Ticket</td>';
    $entrhtml .= '<td class="bookingagentheader2">Entree</td>';
    $entrhtml .= '<td class="bookingagentheader2"></td>';
    $entrhtml .= '<td class="bookingagentheader2">Price</td>';
    $entrhtml .= '</tr>';

    $form['stepone']['TICKETS']['ticketentrees_fieldset']['header'] = array(
        '#type' => 'markup',
        '#markup' => $entrhtml,
    );

    $form['stepone']['TICKETS']['entry'] = array(
        '#type' => 'fieldset',
        '#prefix' => '<div class="entrywidget" id="entrywidget" style="display:none;"><div id="entryContent" class="divTable">',
        '#suffix' => '</div></div>',
    );


    foreach ($availableTickets as $ticket) {
        $numoftickets = '';
        if ($form_state['values']['ticketentrees' . $ticket->id] != '') {
            $numoftickets = $form_state['values']['ticketentrees' . $ticket->id];
        } 
        else if ($values['ticketentrees' . $ticket->id]) {
            $numoftickets = $values['ticketentrees' . $ticket->id];
        }

        for ($i = 0; $i < $numoftickets; $i++) {
            $selectdefaultvalue = $values["select" . $ticket->id . $i];
            $form['stepone']['TICKETS']['ticketentrees_fieldset']['entry' . $ticket->id]["qty" . $ticket->id . $i] = array(
                '#type' => 'markup',
                '#markup' => '<tr><td>1</td>'
            );
            $form['stepone']['TICKETS']['ticketentrees_fieldset']['entry' . $ticket->id]["name" . $ticket->id . $i] = array(
                '#type' => 'markup',
                '#markup' => "<td>" . $ticket->title . "</td>"
            );
            $form['stepone']['TICKETS']['ticketentrees_fieldset']['entry' . $ticket->id]["select" . $ticket->id . $i] = array(
                '#id' => 'option-entree-' . $ticket->id . '-' . $i,
                '#type' => 'select',
                '#options' => $ticketEntrees[$ticket->id],
                '#default_value' => $selectdefaultvalue,
                '#prefix' => "<td>",
                '#suffix' => "</td>",
                '#attributes' => array("onchange" => "saveTicketEntree('option-entree-" . $ticket->id . "-" . $i . "','" . $ticket->id . "','" . $ticket->id . "-" . $i . "','" . $eventid . "')"
                )
            );
            $chkdefaultvalue = $values["checkbox" . $ticket->id . $i];
            $form['stepone']['TICKETS']['ticketentrees_fieldset']['entry' . $ticket->id]["checkbox" . $ticket->id . $i] = array(
                '#id' => 'dietary-' . $ticket->id . '-' . $i,
                '#type' => 'checkbox',
                '#title' => "<i>Dietary restrictions</i>",
                '#default_value' => $chkdefaultvalue,
                '#prefix' => "<td>",
                '#suffix' => "</td>",
                '#attributes' => array("onclick" => "dietarycheckfieldEvent('" . $ticket->id . "-" . $i . "')"                ),
                '#states' => array(
                    'disabled' => array(
                        ':input[name="select' . $ticket->id . $i . '"]' => array(
                            'value' => ''
                        )
                    )
                )
            );
            
            $price = '$' . _wwcFormatPrice($ticket->price);
            $form['stepone']['TICKETS']['ticketentrees_fieldset']['entry' . $ticket->id]["price" . $ticket->id . $i] = array(
                '#type' => 'markup',
                '#markup' => "<td>" . $price . "</td><tr>"
            );
            
            $commentvalue = ($chkdefaultvalue==1) ? $values["dietarycomments" . $ticket->id . $i] : "";
            $form['stepone']['TICKETS']['ticketentrees_fieldset']['entry' . $ticket->id]["dietarycomments" . $ticket->id . $i] = array(
                '#id' => 'dietarycomments-' . $ticket->id . '-' . $i,
                '#type' => 'textarea',
                '#prefix' => '<div style="display:none;">',
                '#suffix' => '</div>',
                '#default_value' => $commentvalue,
             );
        }
    }


    $form['stepone']['TICKETS']['ticketentrees_fieldset']['entryend'] = array(
        '#type' => 'markup',
        '#markup' => '</table></div></div>',
    );


    $specialinstructionOptions=array(
    		'limitedmobility'=>t('Limited Mobility (wheelchair, cane, etc.)'),
    		'foodallergy'=>t('Food Allergy (seafood, peanuts, etc)'),
    		'anotherparty'=>t('Would like to be seated with another party (specify below)')
    );
    
    $specialinstructions = isset($values['specialinstructions']) ? $values['specialinstructions'] : "";
    $form['stepone']['TICKETS']['specialinstructions'] = array(
    		'#type' => 'checkboxes',
    		'#options'=>$specialinstructionOptions,
    		'#title' => t('<b>Let us know if you have any special instructions or concerns:</b>'),
    		'#default_value' => $specialinstructions,
    		'#weight' => 6,
    );
    
    $additionalcomments = isset($values['additionalcomments']) ? $values['additionalcomments'] : "";
    $form['stepone']['TICKETS']['additionalcomments'] = array(
        '#title' => t(''),
        '#type' => 'textarea',
        '#attributes' => array('placeholder' => t('Please explain, or provide additional comments:'), 'class' => array('textareaStyleMedium')),
        '#default_value' => $additionalcomments,
        '#weight' => 7,
    );

    $form ['modalpopup'] = array(
        '#type' => 'markup',
        '#markup' => '<div id="reservationCheckboxPopup" class="modal hide" role="dialog" aria-labelledby="myModalLabel" aria-hidden="true">
            <div class="modal-header">
                <h4>Dietary Restrictions <button type="button" id="reservationCheckboxClose"  class="close">x</button></h4>
            </div>
            <div class="modal-body">
                <textarea rows="4" id="dietarytext" name="dietarytext" class="span6"></textarea>
            </div>
            <div class="modal-footer">
                <span id="loaderText" style="display: none;">Please Wait...</span>
                <div class="booknow resCheckboxshare span4" id="shareBtn">
                        <input type="button" name="dietarysubmitbtn" id="dietarysubmitbtn"
                        value="Submit" class="resCheckbox-booknow-inner"/>
                </div>
                <input type="hidden" name="hiddenTicketId" id="hiddenTicketId">
                <input type="hidden" name="hiddenTicketEntreeId" id="hiddenTicketEntreeId">
                <input type="hidden" name="hiddenSelectedTicket" id="hiddenSelectedTicket">
			<input type="hidden" name="hiddenEventId" id="hiddenEventId">
            </div>
        </div><div class="modal-backdrop in reservationCheckBackdrop"></div>'
    );
    
    
    $form ['next'] = array(
        '#type' => 'submit',
        '#value' => t('Save & Continue'),
        '#prefix' => '<div class="paymentSubmitButton groupmarginTop"><div class="row-fluid"><div class="span4"><div class="line"></div></div><div class="booknow share span4" id="shareBtn"><span class="booknow-inner">',
        '#suffix' => '</span></div><div class="span4"><div class="line"></div></div></div></div>'
    );


    return $form;
}

function _ticketentrees_callback($form, $form_state)
{
    return $form['stepone']['TICKETS']['ticketentrees_fieldset'];
}


//---------------------- Page 1: Tickets(form) ends here  ----------------------------





//***********************  Page 2: Enhancements(form) starts here  ***************************
/*
 * ->Getting Tickets
 * ->Load all cruise fee itesm
 */
function bookingagent_enhancements_form($form, &$form_state) {
	
	$sessionid=$_SESSION['cruisecardsession'];
    $values = isset($form_state['multistep_values']['ENHANCEMENTS']) ? $form_state['multistep_values']['ENHANCEMENTS'] : array();

    if (!empty($values)) {
        $bookingagentinputenarray = array();
        array_walk_recursive($values, function ($value) use(& $bookingagentinputenarray) {
            $bookingagentinputenarray[] = $value;
        });

        $enchunkresults = array_chunk($bookingagentinputenarray, 7);
        $enhancementkeyorder = array(
            'qty',
            'price',
            'comments',
            'actualprice',
            'name',
            'parentproduct',
            'enhancedproductnodeid'
        );
        
        foreach ($enchunkresults as $enchunkarray) {
            list ($qty) = $enchunkarray;
            if ($qty > 0) {
                list (,,,, $nodeenid) = $enchunkarray;
                $enproductenhancments[$nodeenid] = array_combine($enhancementkeyorder, $enchunkarray);
            }
        }
    }
    
	if (isset ( $enproductenhancments ) && ! empty ( $enproductenhancments )) {
		$form ['multistep_values'] = _buildformbyCategoryId ( $enproductenhancments );
	} else {
		$form ['multistep_values'] = _buildformbyCategoryId ();
	}


    $form['ENHANCEMENTS']['occasiontype'] = array(
        '#type' => 'checkbox',
        '#weight' => 6,
        '#title' => t('<span class="paymentLable">Celebration (BirthDay, Anniversary, Special Occasions, etc.)</span>'),
        '#default_value' => isset($values['occasiontype']) ? $values['occasiontype'] : "",
    );
    
    $form['ENHANCEMENTS']['occasionadditionalcomments'] = array(
        '#title' => t(''),
        '#type' => 'textarea',
        '#weight' => 7,
        '#attributes' => array('placeholder' => t('Please explain, or provide additional comments:')),
        '#default_value' => isset($values['occasionadditionalcomments']) ? $values['occasionadditionalcomments'] : "",
    );
    
    
    $form['ENHANCEMENTS']['cruisefeestabletitle'] = array(
        '#type' => 'markup',
        '#markup' => '<h3 class="feeTitle">Cruise Fees</h3>',
    );


    $form['ENHANCEMENTS']['cruisefeestablestart'] = array(
        '#type' => 'markup',
        '#markup' => '<table class="table-striped"> 
    		<tr>
    		<td class="feeitemfields"><b>Fee</b></td>
    		<td class="feeitemfields"><b>Type</b></td>
    		<td class="feeitemfields"><b>Default Rate</b></td>
    		<td class="feeitemfields"><b>Modified Rate</b></td></tr>'
    );




    //Getting Tickets 
    $tickets=unserialize(CartSession::getTickets($sessionid));
    
   
    //Load all cruise fee itesm
    $feeitems=Fees::load_fee_items();
    foreach($feeitems as $feeitem){
    	$chargeamt=false;
    	$feeId=$feeitem->id;
    	$feeTitle=$feeitem->title;
    	//$feeValue=$feeitem->value;
    	
    	$a_feeValue = $feeitem->value;
    	$feeValue=0;
    	
    	
    	//Getting the Fee Value
    	$inputbookingdata = $_SESSION['bookingInput'];
    	$cruise_id=$inputbookingdata->templateid;
    	//Check is Fee activated or not
    	$feeStatus=CruiseFees::getCruiesFeeStatus($cruise_id, $feeId);
    	 if($feeStatus=="1"){
    	 	
    	 $u_feeValue=CruiseFees::getCruiesFeePrice($cruise_id, $feeId);
                	if($u_feeValue>0 && $u_feeValue!=""){
                		$feeValue=$u_feeValue;
                	}else{
                		$feeValue=$a_feeValue;
                	}
                	
                	//var_dump($u_feeValue); exit();
    	 	
    	 	
    		

    		$feeType=$feeitem->type;
    		foreach ($tickets as $ticket) {
    			if ($ticket ['qty'] > 0) {
    				$ticket_id = $ticket ['ticketid'];
    				$ticket_qty = $ticket ['qty'];
    				$ticket_price = $ticket ['price'];
    				$ticket_feeids = Fees::get_ticket_feeids($ticket_id);
    				if (in_array($feeId, $ticket_feeids)) {
    					if ($feeType == "flatfee") {
    						$totalcharge = $ticket_qty * $feeValue;
    						$chargeamt += $totalcharge;
    					}
    		
    					if ($feeType == "percentage") {
    						$totalamt = $ticket_qty * $ticket_price;
    						$chargeamt += ($totalamt * $feeValue) / 100;
    					}
    				}
    			}
    		}//tickets loop ends here
    		if ($chargeamt) {
    			//$fee_value = number_format($feeValue, 2);
    			$fee_value=sprintf('%.2f',$feeValue );
    			$form['ENHANCEMENTS']['cruisefeeid_' . $feeId] = array(
    					'#type' => 'textfield',
    					'#default_value' => $feeId,
    					'#prefix' => '<div style="display:none;">',
    					'#suffix' => '</div>',
    			);
    		
    			$form['ENHANCEMENTS']['cruisefeetitle_' . $feeId] = array(
    					'#type' => 'textfield',
    					'#default_value' => $feeTitle,
    					'#prefix' => '<div style="display:none;">',
    					'#suffix' => '</div>',
    			);
    		
    			$form['ENHANCEMENTS']['cruisefeetitle' . $feeId] = array(
    					'#type' => 'item',
    					'#markup' => '<span class="feeitemfields">'.$feeTitle.'</span>',
    					'#prefix' => '<tr><td>',
    					'#suffix' => '</td>',
    			);
    		
    			$form['ENHANCEMENTS']['cruisefeestype' . $feeId] = array(
    					'#type' => 'item',
    					'#markup' => '<span class="feeitemfields">'.$feeType.'</span>',
    					'#prefix' => '<td>',
    					'#suffix' => '</td>',
    			);
    		
    			$form['ENHANCEMENTS']['cruisefeestype_' . $feeId] = array(
    					'#type' => 'textfield',
    					'#default_value' => $feeType,
    					'#prefix' => '<div style="display:none;">',
    					'#suffix' => '</div>',
    			);
    		
    		
    			$form['ENHANCEMENTS']['cruisefeesactualrate_' . $feeId] = array(
    					'#type' => 'textfield',
    					'#markup' => $feeType,
    					'#default_value' => $fee_value,
    					'#prefix' => '<td>',
    					'#suffix' => '</td>',
    					'#attributes' => array('readonly' => 'readonly'),
    			);
    		
    			$form['ENHANCEMENTS']['cruisefeesmodifiedrate_' . $feeId] = array(
    					'#type' => 'textfield',
    					'#markup' => $feeType,
    					'#default_value' => isset($values['cruisefeesmodifiedrate_' . $feeId]) ? $values['cruisefeesmodifiedrate_' . $feeId] : $fee_value,
    					'#prefix' => '<td>',
    					'#suffix' => '</td></tr>',
    			);
    		}
    		
    		
    	}
    	
    	
    	
    	
    	
    }//cruise fee items loop ends here
    
    
   
    
    
    
    
    

   


    $form['ENHANCEMENTS']['lineitemcharges']['lineitemchargehwSWE'] = array(
        '#type' => 'item',
        '#markup' => '<table class="lineitemcharge table-striped"><caption><h3 class="feeTitle">Line Item Charge / Discount</h3></caption>'
        );


    $form['ENHANCEMENTS']['lineitemcharges']['charge'] = array(
        '#type' => 'checkbox',
        '#title' =>'<span class="feeitemfields">'. t('Taxed').'</span>',
        '#prefix' => '<tr><td>',
        '#suffix' => '</td>',
        '#default_value' => isset($values['charge'])? $values['charge'] : '',
    );


    $form['ENHANCEMENTS']['lineitemcharges']['chargedescription'] = array(
        '#type' => 'textfield',
        '#title' =>'<span class="feeitemfields">'. t('Charge (Description)').'</span>',
        '#prefix' => '<td>',
        '#suffix' => '</td>',
        '#default_value' => isset($values['chargedescription'])? $values['chargedescription'] : '',
    );
	$form ['ENHANCEMENTS'] ['lineitemcharges'] ['chargetype'] = array (
			'#type' => 'select',
			'#title' => '<span class="feeitemfields">'.t ( 'Type' ).'</span>',
			'#options' => array (
					'dollaramount' => t ( 'Dollar Amount' ),
					'percentage' => t ( 'Percentage' ) 
			),
			'#prefix' => '<td>',
			'#suffix' => '</td>',
			'#default_value' => isset ( $values ['chargetype'] ) ? $values ['chargetype'] : '' 
	);
	
	
	
	$form ['ENHANCEMENTS'] ['lineitemcharges'] ['chargevalue'] = array (
			'#type' => 'textfield',
			'#title' => '<span class="feeitemfields">'.t ( 'Value' ).'</span>',
			'#prefix' => '<td>',
			'#suffix' => '</td></tr>',
			'#default_value' => isset ( $values ['chargevalue'] ) ? $values ['chargevalue'] : '' 
	);
    
    
    
    
    $form['ENHANCEMENTS']['lineitemcharges']['discount'] = array(
        '#type' => 'checkbox',
        '#title' => '<span class="feeitemfields">'.t('Pre-Tax').'</span>',
        '#prefix' => '<tr><td>',
        '#suffix' => '</td>',
         '#default_value' => isset($values['discount'])? $values['discount'] : '',
    );


    $form['ENHANCEMENTS']['lineitemcharges']['discountdescription'] = array(
        '#type' => 'textfield',
        '#title' => '<span class="feeitemfields">'.t('Discount (Description)').'</span>',
        '#prefix' => '<td>',
        '#suffix' => '</td>',
        '#default_value' => isset($values['discountdescription'])? $values['discountdescription'] : '',
    );
	$form ['ENHANCEMENTS'] ['lineitemcharges'] ['discounttype'] = array (
			'#type' => 'select',
			'#title' => '<span class="feeitemfields">'.t ( 'Type' ).'</span>',
			'#options' => array (
					'dollaramount' => t ( 'Dollar Amount' ),
					'percentage' => t ( 'Percentage' ) 
			),
			'#prefix' => '<td>',
			'#suffix' => '</td>',
			'#default_value' => isset ( $values ['discounttype'] ) ? $values ['discounttype'] : '' 
	);
	$form ['ENHANCEMENTS'] ['lineitemcharges'] ['discountvalue'] = array (
			'#type' => 'textfield',
			'#title' => '<span class="feeitemfields">'.t ( 'Value' ).'</span>',
			'#prefix' => '<td>',
			'#suffix' => '</td></tr>',
			'#default_value' => isset ( $values ['discountvalue'] ) ? $values ['discountvalue'] : '' 
	);
     
     
    
    
  
    
    $form['ENHANCEMENTS']['lineitemcharges']['lineitemchargesend'] = array(
        '#type' => 'markup',
        '#markup' => '</table>'
    );
    
    
    
    
    
    
    
    $taxvalue=variable_get('WWCTAXVALUE',''); //1
    
    $form['ENHANCEMENTS']['tax']['taxdivstart'] = array(
    		'#type' => 'markup',
    		'#markup' => '<table class="table-striped"><tr><td><h3 class="feeTitle">Tax : '.$taxvalue.'</h3></td></tr>'
    );
    
    $form['ENHANCEMENTS']['tax']['taxvalue'] = array(
    		'#type' => 'textfield',
    		//'#title' => t('Tax'),
    		'#prefix' => '<tr><td>',
    		'#suffix' => '</td></tr>',
    		'#default_value' => isset($values['taxvalue'])? $values['taxvalue'] : $taxvalue,
    );
    
   $form['ENHANCEMENTS']['tax']['taxdivend'] = array(
        '#type' => 'markup',
        '#markup' => '</table>'
    );
   
   
   
   
   
   

    $form['btnwrapper'] = array(
        '#type' => 'markup',
        '#prefix' => '<div class="bookingagentPaymentBtnWrapper groupmarginTop"><div class="row-fluid">',
        '#suffix' => '</div></div>'
    );

    $form['btnwrapper']['back'] = array(
        '#id' => 'enhancementsbackbutton',
        '#type' => 'submit',
        '#value' => t('Go Back'),
        '#prefix' => '<div class="span3"><div class="line"></div></div><div class="booknowBack share span3" id="shareBtn"><span class="booknowBack-inner">',
        '#suffix' => '</span></div>',
        '#attributes' => array('class' => array('goBackbtn')),
    );

    $form['btnwrapper']['next'] = array(
        '#type' => 'submit',
        '#value' => t('Save & Continue'),
        '#prefix' => '<div class="booknow share span3" id="shareBtn"><span class="booknow-inner">',
        '#suffix' => '</span></div><div class="span3"><div class="line"></div></div>',
    );
    return $form;
}




function _buildformbyCategoryId($enhancedinputs = null) {
    
	$form['steptwo'] = array(
        '#type' => 'fieldset',
        '#prefix' => '<div class="row-fluid enhancementWrapper">',
        '#suffix' => '</div>',
        '#title' => ''
    );
    $form['steptwo']['ENHANCEMENTS'] = array(
        '#type' => 'fieldset',
        '#prefix' => '<div class="span7">',
        '#suffix' => '</div>',
        '#description' => '<div class="bookingSubTitle">To enrich your cruising experience,include enhancements to your cruise</div>'
    );

    $enhancementsoutput = orderSummaryWidget('ENHANCEMENTS');
    $form['steptwo']['ENHANCEMENTSORDERWIDGET'] = array(
        '#type' => 'fieldset',
        '#prefix' => '<div class="span5">',
        '#suffix' => '</div>',
        '#description' => $enhancementsoutput,
    );

    $result = _loadCatalogProducts();
    
    
    $inputbookingdata = $_SESSION['bookingInput'];
    $eventId = $inputbookingdata->eventid;
    $templateid=$inputbookingdata->templateid;
    $selectedproductEnhancments= CruiseEnhancements::getSelectedEnhancements($templateid, $eventId);
    
    //$selectedproductEnhancments=cruiseselectedEnhancments($_SESSION['bookingInput']->templateid);
    $i = 0;
    foreach ($result as $productvalue) {
        foreach ($productvalue as $key => $productdetails) {
            if ($i == 0) {
                $collapsed = FALSE; //By Default First one is in Opened Mode
            } else {
                $collapsed = TRUE; //Closed Mode
            }

            $ehancementsTitle=$key;
//            $key = str_replace(' ', '-', $key);
            $key = 'fieldset'.$productdetails[0]['categoryid'];
            $form['steptwo']['ENHANCEMENTS'][$key] = array(
                '#type' => 'fieldset',
                '#id' => $key,
                '#title' => t($ehancementsTitle),
                '#weight' => $i + 1,
                '#collapsible' => TRUE,
                '#collapsed' => $collapsed,
                '#tree' => TRUE,
                '#prefix' => '<div class="row-fluid groupmarginTop optionItemBorder"><div class="span12">',
                '#suffix' => '</div></div>',
            );


            $rowfluidcount = 1;

            foreach (array_chunk($productdetails, 2) as $optionsform) {

                $form['steptwo']['ENHANCEMENTS'][$key][$rowfluidcount] = array(
                    '#type' => 'markup',
                    '#prefix' => '<div class="row-fluid">',
                    '#suffix' => '</div>',
                    '#title' => '',
                );
                foreach ($optionsform as $productform) {
                    //checking the Enhancment productid are in selected arrays
                    //Enhancments products are getting filtered based product selected enhancements
                    $titleoption = str_replace(' ', '-', $productform['title']);
                    if (in_array($productform['nid'], $selectedproductEnhancments)) {

                        $thumbhtml = '<div class="blogInner">
                             <div class="blogTitle">
                             <span>' . $productform['title'] . '</span>
                    		 <br/><span class="blogtitleCaption">' . $productform['caption'] . '</span>
                    		 </div><img src="' . file_create_url($productform['enhancements_img_fid']) . '"></div>';
                        $form['steptwo']['ENHANCEMENTS'][$key][$rowfluidcount]['options'][$productform['nid']] = array(
                            '#type' => 'fieldset',
                            '#prefix' => '<div class="optionItemWrapper span6">',
                            '#suffix' => '</div>',
                            '#title' => '',
                        );


                        $form['steptwo']['ENHANCEMENTS'][$key][$rowfluidcount]['options'][$productform['nid']][$titleoption][$productform['nid']] = array(
                            '#type' => 'fieldset',
                            '#title' => '',
                            '#description' => $thumbhtml,
                            '#attributes' => array('class' => array('optionItemthumb'))
                        );

                        $form['steptwo']['ENHANCEMENTS'][$key][$rowfluidcount]['options'][$productform['nid']][$titleoption]['item-' . $productform['nid']]['qty'] = array(
                            '#type' => 'select',
                            '#title' => uc_currency_format($productform['price']), //later we need to change this one as dynamic
                           '#options' => array(
                                1 => 1,
                                2 => 2,
                                3 => 3,
                                4 => 4,
                                5 => 5,
                                6 => 6,
                                7 => 7
                            ),
                            '#default_value' => (count($enhancedinputs) > 0) ? $enhancedinputs[$productform['title']]['qty'] : '',
                            "#empty_option" => t('- Select -'),
                            '#prefix' => '<div class="optionQuantityWrapper">',
                            '#suffix' => '</div>',
                            '#attributes' => array('class' => array('selectFieldStyle selectFieldSmall'))
                        );

                        $customprice=isset($enhancedinputs[$productform['title']]['price']) ? $enhancedinputs[$productform['title']]['price'] : $productform['price'];

                        $fprice=sprintf("%.2f", $customprice);
                        $form['steptwo']['ENHANCEMENTS'][$key][$rowfluidcount]['options'][$productform['nid']][$titleoption]['item-' . $productform['nid']]['price'] = array(
                            '#type' => 'textfield',
                            '#default_value'=>$fprice,
                        );


                        $comments=isset($enhancedinputs[$productform['title']]['comments']) ? $enhancedinputs[$productform['title']]['comments'] :'';
                        $form['steptwo']['ENHANCEMENTS'][$key][$rowfluidcount]['options'][$productform['nid']][$titleoption]['item-' . $productform['nid']]['comments'] = array(
                            '#type' => 'textarea',
                             '#id' => $key.$productform['nid'],
                            '#default_value'=>$comments,
                        	'#attributes' => array('placeholder' => array('Comments')), 
                        );
                            
                        // Hidden field for price section
                        $form['steptwo']['ENHANCEMENTS'][$key][$rowfluidcount]['options'][$productform['nid']][$titleoption]['item-' . $productform['nid']]['actualprice'] = array(
                            '#type' => 'hidden',
                            '#value' => $productform['price']
                        );
                        
                        // Name of Enhancement
                        $form['steptwo']['ENHANCEMENTS'][$key][$rowfluidcount]['options'][$productform['nid']][$titleoption]['item-' . $productform['nid']]['enhancementname'] = array(
                            '#type' => 'hidden',
                            '#value' => $productform['title']
                        );
                            
                        // Enhancement Parent ProductId
                        $form['steptwo']['ENHANCEMENTS'][$key][$rowfluidcount]['options'][$productform['nid']][$titleoption]['item-' . $productform['nid']]['selectednode'] = array(
                            '#type' => 'hidden',
                            '#value' => 80
                        );

                        //Enhancement nodeId
                        $form['steptwo']['ENHANCEMENTS'][$key][$rowfluidcount]['options'][$productform['nid']][$titleoption]['item-' . $productform['nid']]['enhacmentselectednode'] = array(
                            '#type' => 'hidden',
                            '#value' => $productform['nid']
                        );
                    }
                }
                $rowfluidcount++;
            }
            $i++;
        }
    }
    return $form;
}

//------------------------- Page 2: Enhancements(form) ends here -------------------------






//***********************  Page 3: Payments(form) starts here  ***************************
function bookingagent_payments_form($form, &$form_state) {
    
    
    $values = isset($form_state['multistep_values']['PAYMENT']) ? $form_state['multistep_values']['PAYMENT'] : array();
    $form['stepthree'] = array(
        '#type' => 'markup',
        '#prefix' => '<div class="row-fluid">',
        '#suffix' => '</div>',
        '#title' => ''
    );

    $form['stepthree']['PAYMENT'] = array(
        '#type' => 'fieldset',
        '#prefix' => '<div class="span7 bookingagentPaymentDetails">',
        '#suffix' => '</div>',
        '#title' => '',
        '#description' => '<div class="bookingagentSubTitle">
        Promotional Code, Gift Card, Voucher
        </div>'
    );

    $enhancementsoutput = orderSummaryWidget('PAYMENT');

    $form['stepthree']['paymentwidget'] = array(
        '#type' => 'fieldset',
        '#prefix' => '<div class="span5">',
        '#suffix' => '</div>',
        '#title' => '',
        '#description' => $enhancementsoutput
    );



    $form['stepthree']['PAYMENT']['codes'] = array(
        '#type' => 'fieldset',
        '#prefix' => '<div class="headercontent textmarginTop">',
        '#suffix' => '</div>',
        '#title' => '',
        '#weight' => 1,
        //'#value' => $stages_content,
    );

    $form['stepthree']['PAYMENT']['codes']['promocodeWrapper'] = array(
        '#type' => 'markup',
        '#prefix' => '<div class="promocodeVoucherWrapper">',
        '#suffix' => '</div>'
    );

    $form['stepthree']['PAYMENT']['codes']['promocodeWrapper']['messageWrapper'] = array(
        '#type' => 'markup',
        '#prefix' => '<div id="promocode_status_message">',
        '#suffix' => '</div>',
        '#weight' => 2,
    );


    
    /*
     * @satya
    * Issue : applied drupal ajax frwk commands not working if page is refreshed
    */
    //Showing the Textbox with readonly attribute (if coupon is valid)
    
    $sessionid=$_SESSION['cruisecardsession'];
    $promocoderesult=CartSession::getPromocode($sessionid);
    //if(isset($_SESSION['cpnamount'])){
    if($promocoderesult){
    	$promocode='';
    	$promocodediscount='';
    	$promocodeArr=unserialize($promocoderesult);
    	foreach($promocodeArr as $key=>$value){
    		$promocode=$key;
    		$promocodediscount=$value;
    	}
    	$form['stepthree']['PAYMENT']['codes']['promocodeWrapper']['prmocode'] = array(
    			'#id'=>'prmocode',
    			'#type' => 'textfield',
    			'#title' => '',
    			'#weight' => 3,
    			'#attributes' => array('placeholder' => t('Promo Code'), 'class' => array('inputfiledStyle inputfieldLarge'),'readonly'=>true),
    			'#prefix' => '<div class="paymentPromoCode">',
    			'#suffix' => '</div>',
    			//'#default_value' => isset($values['prmocode']) ? $values['prmocode'] : "",
    			'#default_value' => $promocode,
    	);
    }else{
    	$form['stepthree']['PAYMENT']['codes']['promocodeWrapper']['prmocode'] = array(
    			'#id'=>'prmocode',
    			'#type' => 'textfield',
    			'#title' => '',
    			'#weight' => 3,
    			'#attributes' => array('placeholder' => t('Promo Code'), 'class' => array('inputfiledStyle inputfieldLarge')),
    			'#prefix' => '<div class="paymentPromoCode">',
    			'#suffix' => '</div>',
    			'#default_value' => isset($values['prmocode']) ? $values['prmocode'] : "",
    			
    	);
    }
    
    
    /*
     * Issue
    * We implemented hide/show for apply and delete buttons
    * Issue : if the pare is refreshed then..... form state is chagned
    */
    
    //$promocoderesult
    //if(isset($_SESSION['cpnamount'])){
    if($promocoderesult){
    	$promocode='';
    	$promocodediscount='';
    	$promocodeArr=unserialize($promocoderesult);
    	foreach($promocodeArr as $key=>$value){
    		$promocode=$key;
    		$promocodediscount=$value;
    	}
    	 
    	$form['stepthree']['PAYMENT']['codes']['promocodeWrapper']['prmobtn'] = array(
            '#limit_validation_errors' => array(),
            '#name' => 'promocode-button-status',
            '#type' => 'button',
            '#value' => t('Apply'),
            '#weight' => 4,
            '#prefix' => '<div class="booknow share span2" id="promoapplybtn" style="display:none;"><span class="booknow-inner">',
            '#suffix' => '</span></div>',
            '#ajax' => array(
                'callback' => '_ajax_isvalidpromocode',
            ),
        );


//         $form['stepthree']['PAYMENT']['codes']['promocodeWrapper']['prmocodeprice'] = array(
//             '#id' => 'promocodepriceval',
//             '#type' => 'textfield',
//             '#title' => '',
//             '#weight' => 4,
//             '#attributes' => array('placeholder' => t('Promo Code'), 'class' => array('inputfiledStyle inputfieldLarge')),
//             '#prefix' => '<div class="paymentPromoCode" id="promocodeprice">',
//             '#suffix' => '</div>',
//             //'#default_value' => isset($_SESSION['cpnamount']) ? $_SESSION['cpnamount'] : "",
//             '#default_value' => $promocodediscount,
//         );


        $form['stepthree']['PAYMENT']['codes']['promocodeWrapper']['prmodeletebtn'] = array(
            '#limit_validation_errors' => array(),
            '#name' => 'promocode-button-status',
            '#type' => 'button',
            '#value' => t('Delete'),
            '#weight' => 5,
            '#prefix' => '<div class="booknow share span2" id="promodeletebtn"><span class="booknow-inner">',
            '#suffix' => '</span></div>',
            '#ajax' => array(
                'callback' => '_ajax_remove_promocode',
            ),
        );
    }else{
    	 
        $form['stepthree']['PAYMENT']['codes']['promocodeWrapper']['prmodeletebtn'] = array(
            '#limit_validation_errors' => array(),
            '#name' => 'promocode-button-status',
            '#type' => 'button',
            '#value' => t('Delete'),
            '#weight' => 5,
            '#prefix' => '<div class="booknow share span2" id="promodeletebtn" style="display:none;"><span class="booknow-inner">',
            '#suffix' => '</span></div>',
            '#ajax' => array(
                'callback' => '_ajax_remove_promocode',
            ),
        );


//         $form['stepthree']['PAYMENT']['codes']['promocodeWrapper']['prmocodeprice'] = array(
//             '#id' => 'promocodepriceval',
//             '#type' => 'textfield',
//             '#title' => '',
//             '#weight' => 4,
//             '#attributes' => array('placeholder' => t('Promo Code'), 'class' => array('inputfiledStyle inputfieldLarge')),
//             '#prefix' => '<div class="paymentPromoCode" id="promocodeprice" style="display:none;">',
//             '#suffix' => '</div>',
//             //'#default_value' => isset($_SESSION['cpnamount']) ? $_SESSION['cpnamount'] : "",
//             '#default_value' => isset($values['prmocode']) ? $values['prmocode'] : "",
//         );



        $form['stepthree']['PAYMENT']['codes']['promocodeWrapper']['prmobtn'] = array(
            '#limit_validation_errors' => array(),
            '#name' => 'promocode-button-status',
            '#type' => 'button',
            '#value' => t('Apply'),
            '#weight' => 4,
            '#prefix' => '<div class="booknow share span2" id="promoapplybtn"><span class="booknow-inner">',
            '#suffix' => '</span></div>',
            '#ajax' => array(
                'callback' => '_ajax_isvalidpromocode',
            ),
        );
    }
    
    
    

    $form['promostatus'] = array(
        '#type' => 'hidden',
        '#value' => '',
    );


    $form['stepthree']['PAYMENT']['codes']['giftcardWrapper'] = array(
        '#type' => 'markup',
        '#prefix' => '<div class="promocodeVoucherWrapper giftcardVoucherWrapper">',
        '#suffix' => '</div>'
    );

    $form['stepthree']['PAYMENT']['codes']['giftcardWrapper']['promocodemessageWrapper'] = array(
        '#type' => 'markup',
        '#prefix' => '<div id="giftcard_status_message">',
        '#suffix' => '</div>',
        '#weight' => 1,
    );
    $form['stepthree']['PAYMENT']['codes']['giftcardWrapper']['giftcard'] = array(
        '#type' => 'textfield',
        '#title' => '',
        '#weight' => 2,
        '#attributes' => array('placeholder' => t('Gift Card Number'), 'class' => array('inputfiledStyle inputfieldLarge')),
        '#prefix' => '<div class="paymentPromoCode">',
        '#suffix' => '</div>',
        '#default_value' => isset($values['giftcard']) ? $values['giftcard'] : "",
    );



    $form['stepthree']['PAYMENT']['codes']['giftcardWrapper']['giftcardbtn'] = array(
        '#type' => 'button',
        '#name' => 'giftcard-button-balancecher',
        '#value' => t('Apply'),
        '#weight' => 3,
        '#prefix' => '<div class="booknow share span2" id="shareBtn"><span class="booknow-inner">',
        '#suffix' => '</span></div>',
        '#limit_validation_errors' => array(),
        '#ajax' => array(
            'callback' => '_ajax_isvalidgiftcard',
        ),
    );

    //laxmi ajax callback functions ends here
    $form['stepthree']['PAYMENT']['codes']['giftcardWrapper']['giftcardbalance'] = array(
        '#type' => 'markup',
        '#markup' => '<div style="width: 100%; height: 23px; float: left;">
              <a href="#balanceCheckerPopup" role="button" data-toggle="modal" style="font-size: 13px;">CHECK GIFT CARD BALANCE:</a></div>',
        '#weight' => 4,
    );

    $form['stepthree']['PAYMENT']['codes']['giftcardWrapper']['balancecheckpopup'] = array(
        '#type' => 'markup',
        '#markup' => '<div id="balanceCheckerPopup" class="modal hide" role="dialog" aria-labelledby="myModalLabel" aria-hidden="true" style="display: none;">
                <div class="modal-header">
                    <h4>Gift Card Balance Checker<button type="button" id="balanceCheckerPopupClose"  class="close">x</button></h4>
                </div>
                <div class="modal-body">
                    <div class="row-fluid">
                        <div class="span3"><label>Balance Checker: </label></div>
                        <div class="span9"><input type="text" name="giftcardpop_number" id="giftcardpop_number" /></div>
                </div>
                    <div class="row-fluid">
                        <div class="span3"><label>Balance Amount: </label></div>
                        <div class="span9"><span id="giftcardbalance_amount"></span></div>
                    </div>
                </div>
                <div class="modal-footer">
                    <span id="loaderText" style="display: none;">Loading...</span>
                    <div class="booknow resCheckboxshare" id="shareBtn">
                        <input type="button" name="submit" value="Submit" class="resCheckbox-booknow-inner" onclick="checkbalance();"/>
                    </div>
                </div>
            </div>'
    );

    $form['stepthree']['PAYMENT']['codes']['giftcardvoucherWrapper'] = array(
        '#type' => 'markup',
        '#prefix' => '<div class="promocodeVoucherWrapper giftcardVoucherWrapper" style="margin-bottom: -6px;" id="vocuhers-fieldset-wrapper">',
        '#suffix' => '</div>',
    );


    if (empty($form_state['num_vouchers'])) {
        $form_state['num_vouchers'] = 1;
    }
    for ($i = 1; $i <= $form_state['num_vouchers']; $i++) {
        $form['stepthree']['PAYMENT']['codes']['giftcardvoucherWrapper']['voucher' . $i] = array(
            '#type' => 'textfield',
            '#attributes' => array('placeholder' => t('Voucher Code'), 'class' => array('inputfiledStyle inputfieldLarge')),
            '#prefix' => '<div class="paymentPromoCode voucherWrap">',
            '#suffix' => '</div>',
            '#default_value' => isset($values['voucher' . $i]) ? $values['voucher' . $i] : "",
        );

        $form['stepthree']['PAYMENT']['codes']['giftcardvoucherWrapper']['vocuher_applybutton' . $i] = array(
            '#type' => 'button',
            '#name' => $i,
            '#value' => t('Apply'),
            '#prefix' => '<div class="booknow share span2" id="shareBtn"><span class="booknow-inner">',
            '#suffix' => '</span></div>',
            '#limit_validation_errors' => array(),
            '#ajax' => array(
                'callback' => '_ajax_isvalidvoucher',
            ),
        );
    }//for

    $form['stepthree']['PAYMENT']['codes']['vocuher_addbutton'] = array(
        '#type' => 'submit',
        '#value' => t('ADD ANOTHER VOUCHER CODE'),
        '#weight' => 7,
        '#limit_validation_errors' => array(),
        '#prefix' => '<div class="voucherSubmit">',
        '#suffix' => '</div>',
        '#submit' => array('_ajax_add_textfield_action'),
        '#ajax' => array(
            'callback' => '_ajax_textfield_add_callback',
            'wrapper' => 'vocuhers-fieldset-wrapper',
        ),
    );



    //Guest Details
    $form['stepthree']['PAYMENT']['guest'] = array(
        '#type' => 'fieldset',
        '#prefix' => '<div class="guestDetailscontent legendTitle groupmarginTop">',
        '#suffix' => '</div>',
        '#title' => 'Guest Details',
        '#weight' => 8,
    );

    $form['stepthree']['PAYMENT']['guest']['guest_firstname'] = array(
        '#type' => 'textfield',
        '#title' => '',
        '#weight' => 9,
        '#attributes' => array('placeholder' => t('*First Name'), 'class' => array('inputfiledStyle inputfieldMedium')),
        '#default_value' => isset($values['guest_firstname']) ? $values['guest_firstname'] : "",
    );

    $form['stepthree']['PAYMENT']['guest']['guest_lastname'] = array(
        '#type' => 'textfield',
        '#title' => '',
        '#weight' => 10,
        '#attributes' => array('placeholder' => t('*Last Name'), 'class' => array('inputfiledStyle inputfieldMedium')),
        '#default_value' => isset($values['guest_lastname']) ? $values['guest_lastname'] : "",
    );

    $form['stepthree']['PAYMENT']['guest']['guest_address'] = array(
        '#type' => 'textfield',
        '#title' => '',
        '#weight' => 11,
        '#attributes' => array('placeholder' => t('*Address'), 'class' => array('inputfiledStyle inputfieldMedium')),
        '#default_value' => isset($values['guest_address']) ? $values['guest_address'] : "",
    );

    $form['stepthree']['PAYMENT']['guest']['guest_otheraddress'] = array(
        '#type' => 'textfield',
        '#title' => '',
        '#weight' => 12,
        '#attributes' => array('class' => array('inputfiledStyle inputfieldMedium')),
        '#default_value' => isset($values['guest_otheraddress']) ? $values['guest_otheraddress'] : "",
    );


    $form['stepthree']['PAYMENT']['guest']['guest_city'] = array(
        '#type' => 'textfield',
        '#title' => '',
        '#weight' => 13,
        '#attributes' => array('placeholder' => t('*City'), 'class' => array('inputfiledStyle inputfieldMedium')),
        '#default_value' => isset($values['guest_city']) ? $values['guest_city'] : "",
    );


    $usStatesObject = uc_zone_select();
    $usStatesList = $usStatesObject['#options'];

    $form['stepthree']['PAYMENT']['guest']['guest_state'] = array(
        '#type' => 'select',
        '#title' => t(''), //later we need to change this one as dynamic
        '#weight' => 14,
        '#options' => $usStatesList,
        '#default_value' => 1,
        '#attributes' => array('placeholder' => t('*State'), 'class' => array('selectFieldStyle selectFieldVerySmall pull-left')),
        '#default_value' => isset($values['guest_state']) ? $values['guest_state'] : "",
        '#empty_option' => t('*State'),
    );


    //zip code
    $form['stepthree']['PAYMENT']['guest']['guest_zipcode'] = array(
        '#type' => 'textfield',
        '#title' => '',
        '#weight' => 15,
        '#attributes' => array('placeholder' => t('*Zipcode'), 'class' => array('inputfiledStyle inputfieldSmallxl pull-left')),
        '#default_value' => isset($values['guest_zipcode']) ? $values['guest_zipcode'] : "",
    );
  
    $form['stepthree']['PAYMENT']['guest']['guestphoneWrapper'] = array(
        '#type' => 'markup',
        '#prefix' => '<div>',
        '#suffix' => '</div>',
        '#weight' => 16,
    );

    $form['stepthree']['PAYMENT']['guest']['guestphoneWrapper']['guest_areacode'] = array(
    	'#id'=>'guestAreaCode',
        '#type' => 'textfield',
        '#title' => t('*Phone Number'),
        '#attributes' => array('placeholder' => t(''), 'class' => array('inputfiledStyle inputfieldVerySmall pull-left')),
        '#weight' => 17,
        //'#default_value' => isset($values['billing_phone']) ? $values['billing_phone'] : "",
        '#default_value' => isset($values['guest_areacode']) ? $values['guest_areacode'] : "",
        '#maxlength' => 5,
    );
    $form['stepthree']['PAYMENT']['guest']['guestphoneWrapper']['guest_prefix'] = array(
    	'#id'=>'guestPrefix',
    	'#type' => 'textfield',
        '#title_display' => 'invisible',
        '#title' => t('Phone Number'),
        '#attributes' => array('placeholder' => t(''), 'class' => array('inputfiledStyle inputfieldVerySmall pull-left')),
        '#default_value' => isset($values['guest_prefix']) ? $values['guest_prefix'] : "",
        '#weight' => 18,
        '#maxlength' => 3,
    );
    $form['stepthree']['PAYMENT']['guest']['guestphoneWrapper']['guest_linenumber'] = array(
    	'#id'=>'guestLineNumber',
    	'#type' => 'textfield',
        '#title_display' => 'invisible',
        '#title' => t('Phone Number'),
        '#attributes' => array('placeholder' => t(''), 'class' => array('inputfiledStyle inputfieldVerySmall')),
        '#weight' => 19,
        '#default_value' => isset($values['guest_linenumber']) ? $values['guest_linenumber'] : "",
        '#maxlength' => 4,
    );



    //Email Address
    $form['stepthree']['PAYMENT']['guest']['guest_email'] = array(
        '#type' => 'textfield',
        '#title' => '',
        '#weight' => 20,
        '#attributes' => array('placeholder' => t('*Email Address'), 'class' => array('inputfiledStyle inputfieldMedium')),
        '#default_value' => isset($values['guest_email']) ? $values['guest_email'] : "",
    );


    $form['stepthree']['PAYMENT']['guest']['guest_confirmemail'] = array(
        '#type' => 'textfield',
        '#title' => '',
        '#weight' => 21,
        '#attributes' => array('placeholder' => t('*Confirm Email Address'), 'class' => array('inputfiledStyle inputfieldMedium')),
        '#default_value' => isset($values['guest_confirmemail']) ? $values['guest_confirmemail'] : "",
    );

    $form['stepthree']['PAYMENT']['guest']['desc'] = array(
        '#type' => 'markup',
        '#markup' => '<div style="display:block;"><span class="paymentLable">Let us know your special occasion dates to receive special offers for your celebrations</span></div>',
        '#weight' => 22,
    );
    
    $form['stepthree']['PAYMENT']['guest']['guest_dob_month'] = array(
    		'#type' => 'select',
    		'#title' => t('Date of Birth'),
    		'#weight' => 23,
    		'#options' => array(1 => t('January'), 2 => t('February'),
    				3 => t('March'), 4 => t('April'),
    				5 => t('May'), 6 => t('June'),
    				7 => t('July'), 8 => t('August'),
    				9 => t('September'), 10 => t('October'),
    				11 => t('November'), 12 => t('December')
    		),
    		'#attributes' => array('placeholder' => t('Month'), 'class' => array('selectFieldStyle selectFieldVerySmall pull-left')),
    		'#default_value' => isset($values['guest_dob_month']) ? $values['guest_dob_month'] : '',
    		'#empty_option' => t('Month'),
    );
     
     
    
     
    $form['stepthree']['PAYMENT']['guest']['guest_dob_day'] = array(
    		'#type' => 'select',
    		'#title' => t(''),
    		'#weight' => 24,
    		'#options' => drupal_map_assoc(range(1, 31)),
    		'#attributes' => array('placeholder' => t('Day'), 'class' => array('selectFieldStyle selectFieldVerySmall')),
    		'#default_value' => isset($values['guest_dob_day']) ? $values['guest_dob_day'] : '',
    		'#empty_option' => t('Day'),
    );


    $form['stepthree']['PAYMENT']['guest']['guest_aniverssary_month'] = array(
    		'#type' => 'select',
    		'#title' => t('Anniversary Date'),
    		'#weight' => 25,
    		'#options' => array(1 => t('January'), 2 => t('February'),
    				3 => t('March'), 4 => t('April'),
    				5 => t('May'), 6 => t('June'),
    				7 => t('July'), 8 => t('August'),
    				9 => t('September'), 10 => t('October'),
    				11 => t('November'), 12 => t('December')
    		),
    		'#attributes' => array('placeholder' => t('Month'), 'class' => array('selectFieldStyle selectFieldVerySmall pull-left')),
    		'#default_value' => isset($values['guest_aniverssary_month']) ? $values['guest_aniverssary_month'] : '',
    		'#empty_option' => t('Month'),
    );
    
    
    
    
    $form['stepthree']['PAYMENT']['guest']['guest_aniverssary_day'] = array(
    		'#type' => 'select',
    		'#title' => t(''), 
    		'#weight' => 26,
    		'#options' => drupal_map_assoc(range(1, 31)),
    		'#attributes' => array('placeholder' => t('Day'), 'class' => array('selectFieldStyle selectFieldVerySmall')),
    		'#default_value' => isset($values['guest_aniverssary_day']) ? $values['guest_aniverssary_day'] : '',
    		'#empty_option' => t('Day'),
    );
     
    
    

    //Billing Address

    $form['stepthree']['PAYMENT']['billing'] = array(
        '#type' => 'fieldset',
        '#prefix' => '<div class="billingDetailscontent legendTitle groupmarginTop">',
        '#suffix' => '</div>',
        '#title' => 'Billing Details',
        '#weight' => 27,
//        '#attributes' => array('class' => array('inputfiledStyle inputfieldMedium'))
    );

    $form['stepthree']['PAYMENT']['billing']['sameascheckbox'] = array(
        '#type' => 'checkbox',
        '#weight' => 28,
        '#title' => t('<span class="paymentLable">My billing address is the same as my guest details</span>'),
        '#attributes' => array('class' => array('ticketsameas')),
        '#default_value' => isset($values['sameascheckbox']) ? $values['sameascheckbox'] : "",
    );

    $form['stepthree']['PAYMENT']['billing']['billing_firstname'] = array(
        '#type' => 'textfield',
        '#title' => '',
        '#weight' => 29,
        '#attributes' => array('placeholder' => t('*First Name'), 'class' => array('inputfiledStyle inputfieldMedium')),
        '#default_value' => isset($values['billing_firstname']) ? $values['billing_firstname'] : "",
    );

    $form['stepthree']['PAYMENT']['billing']['billing_lastname'] = array(
        '#type' => 'textfield',
        '#title' => '',
        '#weight' => 30,
        '#attributes' => array('placeholder' => t('*Last Name'), 'class' => array('inputfiledStyle inputfieldMedium')),
        '#default_value' => isset($values['billing_lastname']) ? $values['billing_lastname'] : "",
    );

    $form['stepthree']['PAYMENT']['billing']['billing_address'] = array(
        '#type' => 'textfield',
        '#title' => '',
        '#weight' => 31,
        '#attributes' => array('placeholder' => t('*Address'), 'class' => array('inputfiledStyle inputfieldMedium')),
        '#default_value' => isset($values['billing_address']) ? $values['billing_address'] : "",
    );

    $form['stepthree']['PAYMENT']['billing']['billing_otheraddress'] = array(
        '#type' => 'textfield',
        '#title' => '',
        '#weight' => 32,
        '#attributes' => array('class' => array('inputfiledStyle inputfieldMedium')),
        '#default_value' => isset($values['billing_otheraddress']) ? $values['billing_otheraddress'] : "",
    );


    $form['stepthree']['PAYMENT']['billing']['billing_city'] = array(
        '#type' => 'textfield',
        '#title' => 'City',
        '#title_display' => 'invisible',
        '#weight' => 33,
        '#attributes' => array('placeholder' => t('*City'), 'class' => array('inputfiledStyle inputfieldMedium')),
        '#default_value' => isset($values['billing_city']) ? $values['billing_city'] : "",
    );




    $form['stepthree']['PAYMENT']['billing']['billing_state'] = array(
        '#type' => 'select',
        '#title' => t(''), //later we need to change this one as dynamic
        '#weight' => 34,
        '#options' => $usStatesList,
        '#default_value' => 1,
        '#attributes' => array('placeholder' => t('*State'), 'class' => array('selectFieldStyle selectFieldVerySmall pull-left')),
        '#default_value' => isset($values['billing_state']) ? $values['billing_state'] : "",
        '#empty_option' => t('*State'),
    );


    //zip code
    $form['stepthree']['PAYMENT']['billing']['billing_zipcode'] = array(
        '#type' => 'textfield',
        '#title' => '',
        '#weight' => 35,
        '#attributes' => array('placeholder' => t('*Zipcode'), 'class' => array('inputfiledStyle inputfieldSmallxl pull-left')),
        '#default_value' => isset($values['billing_zipcode']) ? $values['billing_zipcode'] : "",
    );
    

    $form['stepthree']['PAYMENT']['billing']['billingphoneWrapper'] = array(
        '#type' => 'markup',
        '#prefix' => '<div>',
        '#suffix' => '</div>',
        '#weight' => 36,
    );

    $form['stepthree']['PAYMENT']['billing']['billingphoneWrapper']['billing_areacode'] = array(
    	'#id'=>'billingAreaCode',
    	'#type' => 'textfield',
//        '#title_display' => 'invisible',
        '#title' => t('*Phone Number'),
        '#attributes' => array('placeholder' => t(''), 'class' => array('inputfiledStyle inputfieldVerySmall pull-left')),
        '#weight' => 37,
        '#default_value' => isset($values['billing_areacode']) ? $values['billing_areacode'] : "",
        '#maxlength' => 5,
    );
    $form['stepthree']['PAYMENT']['billing']['billingphoneWrapper']['billing_prefix'] = array(
    	'#id'=>'billingPrefix',
    	'#type' => 'textfield',
        '#title_display' => 'invisible',
        '#title' => t('Phone Number'),
        '#attributes' => array('placeholder' => t(''), 'class' => array('inputfiledStyle inputfieldVerySmall pull-left')),
        '#default_value' => isset($values['billing_prefix']) ? $values['billing_prefix'] : "",
        '#weight' => 38,
        '#maxlength' => 3,
    );
    $form['stepthree']['PAYMENT']['billing']['billingphoneWrapper']['billing_linenumber'] = array(
    	'#id'=>'billingLineNumber',
    	'#type' => 'textfield',
        '#title_display' => 'invisible',
        '#title' => t('Phone Number'),
        '#attributes' => array('placeholder' => t(''), 'class' => array('inputfiledStyle inputfieldVerySmall')),
        '#weight' => 39,
        '#default_value' => isset($values['billing_linenumber']) ? $values['billing_linenumber'] : "",
        '#maxlength' => 4,
    );



    //Email Address
    $form['stepthree']['PAYMENT']['billing']['billing_email'] = array(
        '#type' => 'textfield',
        '#title' => '',
        '#weight' => 40,
        '#attributes' => array('placeholder' => t('*Email Address'), 'class' => array('inputfiledStyle inputfieldMedium')),
        '#default_value' => isset($values['billing_email']) ? $values['billing_email'] : "",
    );


    $form['stepthree']['PAYMENT']['billing']['billing_confirmemail'] = array(
        '#type' => 'textfield',
        '#title' => '',
        '#weight' => 41,
        '#attributes' => array('placeholder' => t('*Confirm Email Address'), 'class' => array('inputfiledStyle inputfieldMedium')),
        '#default_value' => isset($values['billing_confirmemail']) ? $values['billing_confirmemail'] : "",
    );
    
    global $user;
    if(in_array('staff admin',$user->roles) ){
    	$active = array(0 => t('No Charge Order'), 1 => t('Credit Card'));
    	$form['stepthree']['PAYMENT']['paymentMethodType'] = array(
    			'#type' => 'radios',
    			'#title' => '<h3 class="paymentpagetitle">Payment Method </h3>',
    			'#default_value' => isset($values['paymentMethodType']) ? $values['paymentMethodType'] : 1,
    			'#options' => $active,
    			'#weight' => 42,
    			'#prefix' => '<div class="creditDetailscontent legendTitle groupmarginTop">',
    			'#suffix' => '</div>',
    	);
    	
    	
    	$form['stepthree']['PAYMENT']['credit'] = array(
    			'#type' => 'fieldset',
    			'#prefix' => '<div class="creditDetailscontent legendTitle groupmarginTop">',
    			'#suffix' => '</div>',
    			'#title' => 'Credit Card Details',
    			'#weight' => 42,
    			'#states' => array(
    					'visible' => array(
    							':input[name="paymentMethodType"]' => array('value' => '1'),
    					),
    			),
    	
    	);
    }else{
    	
    	$active = array(1 => t('Credit Card'));
    	$form['stepthree']['PAYMENT']['paymentMethodType'] = array(
    			'#type' => 'radios',
    			'#title' => '<h3 class="paymentpagetitle">Payment Method </h3>',
    			'#default_value' =>1,
    			'#options' => $active,
    			'#weight' => 42,
    			'#prefix' => '<div style="display:none;" class="creditDetailscontent legendTitle groupmarginTop">',
    			'#suffix' => '</div>',
    	);
    	
    	$form['stepthree']['PAYMENT']['credit'] = array(
    			'#type' => 'fieldset',
    			'#prefix' => '<div class="creditDetailscontent legendTitle groupmarginTop">',
    			'#suffix' => '</div>',
    			'#title' => 'Credit Card Details',
    			'#weight' => 42,
    	);
    	
    }


   


    $form['stepthree']['PAYMENT']['credit']['creditFirstName'] = array(
        '#type' => 'textfield',
        '#title' => '',
        '#weight' => 43,
        '#attributes' => array('placeholder' => t('*First Name as it appears on card'), 'class' => array('inputfiledStyle inputfieldMedium')),
        '#default_value' => isset($values['creditFirstName']) ? $values['creditFirstName'] : "",
    );

    $form['stepthree']['PAYMENT']['credit']['creditLastName'] = array(
        '#type' => 'textfield',
        '#title' => '',
        '#weight' => 44,
        '#attributes' => array('placeholder' => t('*Last Name as it appears on card'), 'class' => array('inputfiledStyle inputfieldMedium')),
        '#default_value' => isset($values['creditLastName']) ? $values['creditLastName'] : "",
    );

    $form['stepthree']['PAYMENT']['credit']['creditCardType'] = array(
        '#type' => 'select',
        '#title' => t(''), //later we need to change this one as dynamic
        '#weight' => 45,
        '#options' => array(
           'visa' => 'Visa',
            'mastercard' => 'Master Card',
            'discover' => 'Discover',
            'americanexpress' => 'American Express',
        ),
        //'#default_value' => 'visa',
        '#attributes' => array('placeholder' => t('*Card Type'), 'class' => array('selectFieldStyle selectFieldMedium')),
        '#default_value' => isset($values['creditCardType']) ? $values['creditCardType'] : 'visa',
        '#empty_option' => t('*Card Type'),
    );
    $form['stepthree']['PAYMENT']['credit']['creditCardNumber'] = array(
        '#type' => 'textfield',
        '#title' => '',
        '#weight' => 46,
        '#attributes' => array('placeholder' => t('*Card Number'), 'class' => array('inputfiledStyle inputfieldMedium pull-left')),
        '#default_value' => isset($values['creditCardNumber']) ? $values['creditCardNumber'] : "",
    );

    $form['stepthree']['PAYMENT']['credit']['creditdatesWrapper'] = array(
        '#type' => 'markup',
        '#prefix' => '<div class="creditdatesWrapholder">',
        '#suffix' => '</div>',
        '#weight' => 47,
    );

    $form['stepthree']['PAYMENT']['credit']['creditdatesWrapper']['creditExpMonth'] = array(
        '#type' => 'select',
        '#title' => t(''), //later we need to change this one as dynamic
        '#weight' => 48,
//        '#options' => array(
//            '02' => '02',
//        ),
//        
        '#options' => array(1 => t('01 - January'), 2 => t('02 - February'),
            3 => t('03 - March'), 4 => t('04 - April'),
            5 => t('05 - May'), 6 => t('06 - June'),
            7 => t('07 - July'), 8 => t('08 - August'),
            9 => t('09 - September'), 10 => t('10 - October'),
            11 => t('11 - November'), 12 => t('12 - December')
        ),
        //'#default_value' => '02',
        '#attributes' => array('placeholder' => t('*ExpMonth'), 'class' => array('selectFieldStyle selectFieldVerySmall pull-left')),
        '#default_value' => isset($values['creditExpMonth']) ? $values['creditExpMonth'] : '',
        '#empty_option' => t('*Month'),
    );

    $min = is_null($min) ? intval(date('Y')) : $min;
    $max = is_null($max) ? intval(date('Y')) + 20 : $max;



    $form['stepthree']['PAYMENT']['credit']['creditdatesWrapper']['creditExpYear'] = array(
        '#type' => 'select',
        '#title' => t(''), //later we need to change this one as dynamic
        '#weight' => 49,
        '#options' => drupal_map_assoc(range($min, $max)),
        //'#default_value' => '02',
        '#attributes' => array('placeholder' => t('ExpYear'), 'class' => array('selectFieldStyle selectFieldVerySmall')),
        '#default_value' => isset($values['creditExpYear']) ? $values['creditExpYear'] : '',
        '#empty_option' => t('*Year'),
    );

    $form['stepthree']['PAYMENT']['credit']['creditcid'] = array(
        '#type' => 'textfield',
        '#title' => '',
        '#weight' => 50,
        '#attributes' => array('placeholder' => t('*CID'), 'class' => array('inputfiledStyle inputfieldMedium')),
        '#default_value' => isset($values['creditcid']) ? $values['creditcid'] : '',
        //'#description' => '<span><abbr title="Customer Identification Number" class="initialism">?</abbr></span>',
    	'#description'=>'<a onclick="window.open(this.href, \'CVV_Info\', \'toolbar=0,scrollbars=1,location=0,statusbar=0,menubar=0,resizable=1,width=480,height=460\'); return false;" href="'.base_path().'cart/checkout/credit/cvv_info"><span><abbr title="Customer Identification Number" class="initialism">?</abbr></span></a>',
    	'#maxlength' => 4,
    );

    $form['stepthree']['PAYMENT']['promotions'] = array(
        '#type' => 'fieldset',
        '#prefix' => '<div class="headercontent">',
        '#suffix' => '</div>',
        '#title' => '',
        //'#value' => $stages_ocass,
        '#weight' => 51,
    );

    $form['stepthree']['PAYMENT']['promotions']['findingusby'] = array(
        '#type' => 'select',
        '#title' => t(''), //later we need to change this one as dynamic
        '#options' => array(
            'How did you hear about us?' => 'How did you hear about us?',
            'Internet Search' => 'Internet Search',
            'Newspaper' => 'Newspaper',
            'Magazine' => 'Magazine',
            'Email' => 'Email',
            'Brochure' => 'Brochure',
            'Radio or TV' => 'Radio or TV',
            'Concierge' => 'Concierge',
            'Word of Mouth' => 'Word of Mouth',
            'Return Customer' => 'Return Customer',
            'Online Ads' => 'Online Ads',
            'Daily Deal' => 'Daily Deal',
            'Seattle Times' => 'Seattle Times',
            'Pandora' => 'Pandora',
            'Gift Certificate' => 'Gift Certificate'
        ),
        '#default_value' => 'How did you hear about us?',
        '#attributes' => array('class' => array('selectFieldStyle selectFieldMedium groupmarginTop')),
        '#default_value' => isset($values['findingusby']) ? $values['findingusby'] : '',
    );

    $form['stepthree']['PAYMENT']['promotions']['promotiondiscounts'] = array(
        '#type' => 'checkbox',
        '#weight' => 52,
        '#title' => t('<span class="paymentLable">I would like to recieve Waterways special promotions and discounts by email</span>'),
        '#default_value' => isset($values['promotiondiscounts']) ? $values['promotiondiscounts'] : '',
//         '#attributes' => array('checked' => TRUE),
    );
    //Not Inserting any card details that would be handled at bookingagent Page section

    $form['btnwrapper'] = array(
        '#type' => 'markup',
        '#prefix' => '<div class="bookingagentPaymentBtnWrapper textmarginTop"><div class="row-fluid">',
        '#suffix' => '</div></div>'
    );

    $form['btnwrapper']['back'] = array(
        '#id'=>'paymentbackbutton',
        '#type' => 'submit',
        '#value' => t('Go Back'),
        '#prefix' => '<div class="span3"><div class="line"></div></div><div class="booknowBack share span3" id="shareBtn"><span class="booknowBack-inner">',
        '#suffix' => '</span></div>',
        '#attributes' => array('class' => array('goBackbtn')),
    );
    
    
    $form['btnwrapper']['backtoticketorder'] = array(
        '#id'=>'backtoticketorderbutton',
        '#type' => 'submit',
        '#value' => t('Go Back To Tickets'),
        '#prefix' => '<div style="display:none;">',
        '#suffix' => '</div>',
        //'#attributes' => array('class' => array('goBackbtn')),
    );

    $form['btnwrapper']['next'] = array(
        '#type' => 'submit',
        '#value' => t('Save & Continue'),
        '#prefix' => '<div class="booknow share span3" id="shareBtn"><span class="booknow-inner">',
        '#suffix' => '</span></div><div class="span3"><div class="line"></div></div>',
    );



    return $form;
}

//***********************  Page 3: Payments(form) ends here  ***************************





//***********************  Page 4: Confirmation starts here  ***************************

function bookingagent_review_form($form, &$form_state) {
    $values = isset($form_state['multistep_values']['REVIEW_ORDER']) ? $form_state['multistep_values']['REVIEW_ORDER'] : array();

    $paymentStepvalues = isset($form_state['multistep_values']['PAYMENT']) ? $form_state['multistep_values']['PAYMENT'] : array();
    $receiveEmailNotifications=isset($paymentStepvalues['promotiondiscounts']) ? $paymentStepvalues['promotiondiscounts'] : '';
    
    $paymentMethodType=$paymentStepvalues['paymentMethodType'];
    
    
    
    $guestDetails = '';
    $guestDetails .= '<h3>Guest Details</h3>';
    $guestDetails .= '<div class="row-fluid"><div class="span12">' . $form_state['values']['guest_firstname'] . ' ' . $form_state['values']['guest_lastname'] . ' </div></div>';
    $guestDetails .= '<div class="row-fluid"><div class="span12">' . $form_state['values']['guest_address'] . '</div></div>';
    
    if(!empty($form_state['values']['guest_otheraddress'])){
    	$guestDetails .= '<div class="row-fluid"><div class="span12">' .$form_state['values']['guest_otheraddress'].'</div></div>';
    }
    $satecityzip = $form_state['values']['guest_city'] . ' , ' . uc_zone_get_by_id($form_state['values']['guest_state']) . ', ' . $form_state['values']['guest_zipcode'];
    $guestDetails .= '<div class="row-fluid"><div class="span12">' . $satecityzip . '</div></div>';
    
    $guestPhone=$form_state['values']['guest_areacode']." ".$form_state['values']['guest_prefix']."  ".$form_state['values']['guest_linenumber'];
    $guestDetails .= '<div class="row-fluid"><div class="span12">' . $guestPhone . '</div></div>';
     
    //$guestDetails .= '<div class="row-fluid"><div class="span12">' . $form_state['values']['guest_phone'] . '</div></div>';
    $guestDetails .= '<div class="row-fluid"><div class="span12">' . $form_state['values']['guest_email'] . '</div></div>';
    $billingaddress = '';
    $billingaddress .= '<h3>Billing Details</h3>';
    $billingaddress .= '<div class="row-fluid"><div class="span12">' . $form_state['values']['billing_firstname'] . ' ' . $form_state['values']['billing_lastname'] . '</div></div>';
    $billingaddress .= '<div class="row-fluid"><div class="span12">' . $form_state['values']['billing_address'] . '</div></div>';

    if(!empty($form_state['values']['billing_otheraddress'])){
    	$billingaddress .= '<div class="row-fluid"><div class="span12">' . $form_state['values']['billing_otheraddress']. '</div></div>';
    }
    
    
    $satecityzipbilling = $form_state['values']['billing_city'] . ', ' . uc_zone_get_by_id($form_state['values']['billing_state']) . ', ' . $form_state['values']['billing_zipcode'];
    $billingaddress .= '<div class="row-fluid"><div class="span12">' . $satecityzipbilling . '</div></div>';
    $billingPhone=$form_state['values']['billing_areacode']." ".$form_state['values']['billing_prefix']."  ".$form_state['values']['billing_linenumber'];
    
    $billingaddress .= '<div class="row-fluid"><div class="span12">' . $billingPhone. '</div></div>';
    $billingaddress .= '<div class="row-fluid"><div class="span12">' . $form_state['values']['billing_email'] . '</div></div>';
    $creaditDetails = '';

    if($paymentMethodType=="1"){
    	$creaditDetails .= '<h3>Credit Card Details</h3>';
    	$creaditDetails .= '<div class="row-fluid"><div class="span12">' . ucwords($form_state['values']['creditCardType']) . '</div></div>';
    	$cc_number=$form_state['values']['creditCardNumber'];
    	$creaditDetails .= '<div class="row-fluid"><div class="span12">Credit Card: <span class="creditcardvalue">' . _maskCreditCard($cc_number) . '</span></div></div>';
    	$cardexpdate = $form_state['values']['creditExpMonth'] . '/' . $form_state['values']['creditExpYear'];
    	$creaditDetails .= '<div class="row-fluid"><div class="span12">Exp date: <span class="creditcardvalue">' . $cardexpdate . '</span></div></div>';
    	
    }
    
    if($paymentMethodType=="0"){
    	$creaditDetails .= '<h3>No Charge Order</h3>';
    }

    
    $policyconditions = '';
    $policyconditions .= '<h3>Cancellation Policy</h3>';
    $policyconditions .= '<p>Actually cliche authentic, raw denim keytar marfa flexitarian farm-to-table church-key VHS fap selfies biodiesel next level.</p>';
    $ordersidewidget = orderSummaryWidget('REVIEW');

    
    $form['REVIEW_ORDER'] = array(
        '#type' => 'fieldset',
        '#title' => '',
        '#weight' => 1,
        '#prefix' => '<div class="row-fluid">',
        '#suffix' => '</div>',
    );
    $form['REVIEW_ORDER']['reviewdetails'] = array(
        '#type' => 'fieldset',
        '#title' => '',
        '#weight' => 2,
        '#prefix' => '<div class="span7">',
        '#suffix' => '</div>',
    );

    $form['REVIEW_ORDER']['reviewdetails']['guest'] = array(
        '#type' => 'fieldset',
        '#title' => '',
        '#weight' => 3,
        '#prefix' => '<div class="row-fluid">',
        '#suffix' => '</div>',
    );

    $form['REVIEW_ORDER']['reviewdetails']['guest']['address'] = array(
        '#type' => 'fieldset',
        '#title' => '',
        '#value' => $guestDetails,
        '#weight' => 4,
        '#prefix' => '<div class="span6">',
        '#suffix' => '</div>',
    );

//     $form['REVIEW_ORDER']['reviewdetails']['guest']['creditcard'] = array(
//         '#type' => 'fieldset',
//         '#title' => '',
//         '#value' => $creaditDetails,
//         '#weight' => 5,
//         '#prefix' => '<div class="span6">',
//         '#suffix' => '</div>',
//     );

    $form['REVIEW_ORDER']['reviewdetails']['guest']['billingaddress'] = array(
    		'#type' => 'fieldset',
    		'#title' => '',
    		'#value' => $billingaddress,
    		'#weight' => 5,
    		'#prefix' => '<div class="span6">',
    		'#suffix' => '</div>',
    );


    $form['REVIEW_ORDER']['reviewdetails']['billing'] = array(
        '#type' => 'fieldset',
        '#title' => '',
        '#weight' => 7,
        '#prefix' => '<div class="row-fluid">',
        '#suffix' => '</div>',
    );
    
    $form['REVIEW_ORDER']['reviewdetails']['billing']['creditcard'] = array(
    		'#type' => 'fieldset',
    		'#title' => '',
    		'#value' => $creaditDetails,
    		'#weight' => 8,
    		'#prefix' => '<div class="span6">',
    		'#suffix' => '</div><div class="span6"></div>',
    );
    

//     $form['REVIEW_ORDER']['reviewdetails']['billing']['billingaddress'] = array(
//         '#type' => 'fieldset',
//         '#title' => '',
//         '#value' => $billingaddress,
//         '#weight' => 8,
//         '#prefix' => '<div class="span6">',
//         '#suffix' => '</div><div class="span6"></div>',
//     );





    $form['REVIEW_ORDER']['reviewdetails']['conditional'] = array(
        '#type' => 'fieldset',
        '#title' => '',
        '#weight' => 9,
        '#prefix' => '<div class="row-fluid">',
        '#suffix' => '</div>',
    );


    $form['REVIEW_ORDER']['reviewdetails']['conditional']['ploicydescription'] = array(
        '#type' => 'fieldset',
        '#title' => '',
        '#value' => $policyconditions,
        '#weight' => 10,
        '#prefix' => '<div class="span12 cancellationPolicy">',
        '#suffix' => '</div>',
    );

    $form['REVIEW_ORDER']['reviewdetails']['conditional']['promotionaldiscountsenable'] = array(
        '#type' => 'checkbox',
        '#title' => t('I would like to recieve Waterways special promotions and discounts by email.'),
        '#prefix' => '<div class="span12" style="display:none;">',
        '#suffix' => '</div>',
        '#weight' => 11,
    	'#default_value' =>$receiveEmailNotifications
    );

    $form['REVIEW_ORDER']['reviewdetails']['conditional']['agreement'] = array(
        '#type' => 'checkbox',
        '#title' => t('I have  read and agree to the terms and conditions.'),
        '#prefix' => '<div class="span12">',
        '#suffix' => '</div>',
        '#weight' => 12,
        //'#attributes'=>array('checked'=>'checked'),
    );

      $form['REVIEW_ORDER']['orderwidget'] = array(
        '#type' => 'fieldset',
        '#title' => '',
        '#description' => $ordersidewidget,
        '#weight' => 13,
        '#prefix' => '<div class="span5">',
        '#suffix' => '</div>',
    );

     $form['btnwrapper'] = array(
        '#type' => 'markup',
        '#prefix' => '<div class="bookingagentPaymentBtnWrapper groupmarginTop"><div class="row-fluid">',
        '#suffix' => '</div></div>',
        '#weight' => 15
    );

    $form['btnwrapper']['back'] = array(
        '#type' => 'submit',
        '#value' => t('Go Back'),
        '#prefix' => '<div class="span3"><div class="line"></div></div><div class="booknowBack share span3" id="shareBtn"><span class="booknowBack-inner">',
        '#suffix' => '</span></div>',
        '#weight' => 16,
        '#attributes' => array('class' => array('goBackbtn')),
    );

    $form['btnwrapper']['next'] = array(
        '#type' => 'submit',
        '#value' => t('Purchase'),
        '#prefix' => '<div class="booknow share span3" id="shareBtn"><span class="booknow-inner">',
        '#suffix' => '</span></div><div class="span3"><div class="line"></div></div>',
        '#weight' => 17,
        '#states' => array(
            'enabled' => array(
                ':input[name = "agreement"]' => array('checked' => TRUE),
            ),
        ),
    );

    return $form;
}




/*
 * Promocode Validation
 * Input Parameters : Promocode
 * Return Type : Message (valid/invalid)
 * 
 * Steps :
 * 1. Getting the Promocode
 * 2. Using uc_coupon_find() function we can validate the coupon
 * 3. if coupon object is valid then we can retrieve the coupon amount
 * 4. 
 */

function _ajax_isvalidpromocode($form, &$form_state) {
    $sessionid = $_SESSION['cruisecardsession']; //1
    $tickettotalamt = (float) CartSession::getTotalTicketsPrice($sessionid); //2
    $couponObj = false;
    $cpnstatus = 'This promo code is invalid or has expired.';

    $commands = array();
    if ($tickettotalamt > 0) { //3
        $discountamt = 0;

        $couponcode = $form_state ['values'] ['prmocode']; //4
        if ($couponcode != "" || !empty($couponcode)) {
            $ticketobject = CartSession::getTickets($sessionid); //5
            try {
                $inputbookingdata = $_SESSION ['bookingInput']; //6
                $eventdata = new stdclass ();
                $eventdata->eventid = $inputbookingdata->eventid;
                $eventdata->tickets = $ticketobject;

                $orderobj = new stdClass ();
                $orderobj->ordertype = "booking";
                $orderobj->events [] = $eventdata;
                $orderobj->ordersubtotal = $tickettotalamt;
                $orderobj->coupontype = "coupon";
                $couponObj = entity_get_controller('coupon')->applycoupon($couponcode, $orderobj); //7 coupon
                $discountamt = $couponObj [0]->discountamount;  //8

                if ($discountamt > 0) {  //9
                    $promocode = array($couponcode => $discountamt);
                    CartSession::insertPromoCode($sessionid, $promocode);  //10
                    $cpnstatus = "Promocode has been applied to your order.";

                    //Update Order Total
                    $totalOrderAmount = CartSession::getOrderTotal($sessionid); //11
                    $updatedTotalAmt = $totalOrderAmount - $discountamt;  //12
                    CartSession::insertOrderTotal($sessionid, $updatedTotalAmt); //13

                    $coupondivtext = '-$' . _wwcFormatPrice($discountamt);
                    $totalamtdivtext = '$' . _wwcFormatPrice($updatedTotalAmt);
                    $commands [] = ajax_command_invoke('#promoapplybtn', 'hide');
                    $commands [] = ajax_command_invoke('#promodeletebtn', 'show');
                    $commands [] = ajax_command_invoke('#cpnmaindiv', 'show');
                    $commands [] = ajax_command_invoke('#prmocode', 'attr', array('readonly', true));
                    $commands [] = ajax_command_replace("#cpnamountdiv", "<div id='cpnamountdiv'>" . $coupondivtext . "</div>");
                    $commands [] = ajax_command_replace("#totalamtdiv", "<div id='totalamtdiv'>" . $totalamtdivtext . "</div>");
                } else {
                    $cpnstatus = 'Invalid Promocode';
                }
            } catch (Exception $e) {
                watchdog("Error in ajax_isvalidpromocode", $e);
            }
        } else {
            $cpnstatus = 'Please enter valid promocode';
        }
    } else {
        $cpnstatus = 'the promo code could not be used for your purchase';
    }
    $commands [] = ajax_command_replace("#promocode_status_message", "<div id='promocode_status_message'>" . $cpnstatus . "</div>");
    return array(
        '#type' => 'ajax',
        '#commands' => $commands
    );
}

/*
 * Removing Promocode
* Steps
* 1. Fetching the COUPON Amount (Stored in Session Variable)
* 2. Fetching the SUBTOTAL Amount (Stored in Session Variable)
* 3. Update -> SUBTOTAL= SUBTOTAL+COUPON Amount
* 4. Unset Session Variable ( Coupon Amount)
*
*/
function _ajax_remove_promocode($form, &$form_state) {
    $sessionid = $_SESSION['cruisecardsession'];
    $promocoderesult = CartSession::getPromocode($sessionid);
    if ($promocoderesult) {
        $promocode = '';
        $promocodediscount = false;
        $promocodeArr = unserialize($promocoderesult);
        CartSession::removePromocode($sessionid);
        
        foreach ($promocodeArr as $key => $value) {
            $promocode = $key;
            $promocodediscount = $value;
        }
        if ($promocodediscount) {
            $discountamt = $promocodediscount;
            $totalAmount = CartSession::getOrderTotal($sessionid);
            $updatedTotalAmt = $totalAmount + $discountamt;
            CartSession::insertOrderTotal($sessionid, $updatedTotalAmt);
            $totalamtdivtext = '$' . _wwcFormatPrice($updatedTotalAmt);
            $commands [] = ajax_command_invoke('#cpnmaindiv', 'hide');
            $commands [] = ajax_command_invoke('#promoapplybtn', 'show');
            $commands [] = ajax_command_invoke('#promodeletebtn', 'hide');
            $commands [] = ajax_command_invoke('#prmocode', 'attr', array('readonly', false));
            $commands [] = ajax_command_invoke('#prmocode', 'attr', array('value', ''));
            $commands [] = ajax_command_replace("#totalamtdiv", "<div id='totalamtdiv'>" . $totalamtdivtext . "</div>");
        } else {
            $cpnstatus = 'Illegal operation performed';
        }

        $commands [] = ajax_command_replace("#promocode_status_message", "<div id='promocode_status_message'>" . $cpnstatus . "</div>");
        return array(
            '#type' => 'ajax',
            '#commands' => $commands
        );
    }
}




//********************* Add more vouchers / Remove Vouchers functionality ************************
function _ajax_textfield_add_callback($form, $form_state) {
    return $form['stepthree']['PAYMENT']['codes']['giftcardvoucherWrapper'];
}

function _ajax_add_textfield_action($form, &$form_state) {
    $form_state['num_vouchers']++;
    $form_state['rebuild'] = TRUE;
}

//Remove Action
function _ajax_textfield_remove_callbacks($form, $form_state) {
    return $form['vouchers_fieldset'];
}

function _ajax_remove_textfield_action($form, &$form_state) {
    $form_state['num_vouchers']--;
    $form_state['rebuild'] = TRUE;
}



//---------------- Gift Card ---------
/*
 * Gift Card Validation
* Input Params : Gift Card Number
* Return Type : Message (Valid / Invalid)

*/
function _ajax_isvalidgiftcard($form, $form_state) {
    $sessionid = $_SESSION['cruisecardsession']; //1
    $commands = array();
    $cardNumber = $form_state ['values'] ['giftcard'];  //2
    if ($cardNumber != "" || !empty($cardNumber)) { //3
        $totalAmount = (float) CartSession::getOrderTotal($sessionid);
        if ($totalAmount > 0) {
            $giftcardresult = CartSession::checkGiftCardExistence($sessionid); //4
            if ($giftcardresult == "") { //do
                $requestType = 'Inquiry';
                $entryType = 'K';
                $giftcardbalancecheckers = giftcard_profitpointapi_balancechecker($requestType, $entryType, $cardNumber); //5
                $giftcardstatus = 'This Gift Card code is invalid or has expired.';
                if ($giftcardbalancecheckers ['message'] == 'sucess') {
                    $giftcardamount = (float) $giftcardbalancecheckers ['results'] ['amount'];  //6
                    if ($giftcardamount > 0) {
                        $giftcardArray = '';
                        $deductgiftcardamt = 0;
                        if ($giftcardamount > $totalAmount) {
                            $deductgiftcardamt = $totalAmount;
                            $giftcardArray = array($cardNumber => array('totalgiftcardprice' => $giftcardamount, 'deductedamount' => $totalAmount));
                        } else {
                            $deductgiftcardamt = $giftcardamount;
                            $giftcardArray = array($cardNumber => array('totalgiftcardprice' => $giftcardamount, 'deductedamount' => $giftcardamount));
                        }
                        CartSession::insertGiftCard($sessionid, $giftcardArray); //7

                        $giftcarddivtext = '-$' . _wwcFormatPrice($deductgiftcardamt);
                        $updatedAmount = $totalAmount - $deductgiftcardamt;
                        CartSession::insertOrderTotal($sessionid, $updatedAmount);
                        $totalamtdivtext = '$' . _wwcFormatPrice($updatedAmount);

                        $giftcardstatus = "Gift card has been applied to your order.";
                        $commands [] = ajax_command_invoke('#giftcardmaindiv', 'show');
                        $commands [] = ajax_command_replace("#giftcardamountdiv", "<div id='giftcardamountdiv'>" . $giftcarddivtext . "</div>");
                        $commands [] = ajax_command_replace("#totalamtdiv", "<div id='totalamtdiv'>" . $totalamtdivtext . "</div>");
                    } else {
                        $giftcardstatus = 'No balance exists for this card';
                    }
                } else {
                    // $commands[] = ajax_command_replace("#giftcardwidget", "<div id='giftcardwidget'>" . '' . "</div>");
                }
            } else {
                $giftcardstatus = 'You have already entered giftcard';
            }
        }
    } else {
        $giftcardstatus = 'Please Enter Valid Gift Card Number';
    }
    $commands [] = ajax_command_replace("#giftcard_status_message", "<div id='giftcard_status_message'>" . $giftcardstatus . "</div>");
    return array(
        '#type' => 'ajax',
        '#commands' => $commands
    );
}

/*
 * Voucher code
* ajax_isvalidvoucher
*/

function _ajax_isvalidvoucher($form, &$form_state) {
   $sessionid = $_SESSION['cruisecardsession']; //1
    $voucheramount = '';
    $msg = false;
    $totalOrderAmount = (float) CartSession::getOrderTotal($sessionid); //2
    $id = $form_state['triggering_element']['#name'];   //3 
    $vocuhervalue = $form_state['values']['voucher' . $id]; //4
    if ($vocuhervalue == '' || strlen($vocuhervalue) == 0) { //5
        $msg = "Please enter valid vouchercode";
    } else if($totalOrderAmount > 0) {
        $existenceresult = CartSession::checkVoucherExistence($vocuhervalue, $sessionid); //6
        if ($existenceresult) {
            $msg = "voucher is already redeemed";
        } else {
            $inputbookingdata = $_SESSION ['bookingInput'];
            $ticketobject = CartSession::getTickets($sessionid);
            $orderobj = new stdClass ();
            $orderobj->ordertype = "booking";
            $eventdata = new stdclass ();
            $eventdata->eventid = $inputbookingdata->eventid;
            $eventdata->tickets = $ticketobject;
            $orderobj->events [] = $eventdata;
            $orderobj->ordersubtotal = $totalOrderAmount;
            $orderobj->coupontype = "groupon";
            $couponObj = entity_get_controller('coupon')->applycoupon($vocuhervalue, $orderobj); //7
            
            //$discountamt = $couponObj [0]->discountamount;
            
            $discountamt =$couponObj;
            

            if ($discountamt > 0) {
                $vouchercode = array($vocuhervalue => $discountamt);
                CartSession::insertVoucherCode($sessionid, $vouchercode); //8
                //$voucheramount = $discountamt;
                $msg = "Voucher has been applied to your order";
                //Total Voucher Codes price
                $total_voucheramount = CartSession::getVoucherCodesPrice($sessionid);
                $voucherdivtext = '-$' . _wwcFormatPrice($total_voucheramount);

                //Total Amount
                $totalAmount = CartSession::getOrderTotal($sessionid);
                $updatedTotalAmt = $totalAmount - $discountamt;
                CartSession::insertOrderTotal($sessionid, $updatedTotalAmt); //9
                $totalamtdivtext = '$' ._wwcFormatPrice($updatedTotalAmt);
                $commands[] = ajax_command_invoke('#vouchermaindiv', 'show');
                $commands[] = ajax_command_replace("#voucheramtdiv", "<div id='voucheramtdiv'>" . $voucherdivtext . "</div>");
                $commands[] = ajax_command_replace("#totalamtdiv", "<div id='totalamtdiv'>" . $totalamtdivtext . "</div>");
            } else {
                $msg = "This Voucher code is invalid or has expired";
            }
        }
    }else{
    	$cpnstatus = 'the voucher code could not be used for your purchase';
    }
    if ($msg) {
        $commands[] = ajax_command_alert($msg);
    }
    return array(
        '#type' => 'ajax',
        '#commands' => $commands
    );
}






