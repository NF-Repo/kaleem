<?php



function updateorder($order_id){
	
	//var_dump($_SESSION['bookingorderformflag']); exit();
	
	if(!isset($_SESSION['bookingorderformflag'])){
		//Step 1: Removing the Order id if Exists in cartsession Table
		try {
			$num_deleted = db_delete('cartsession')
			->condition('sessionid', $order_id)
			->execute();
		} catch (Exception $e) {
		
		}
		
	}
		$orderObject=uc_order_load($order_id);
		$order_id = $orderObject->order_id;
		
		
		
		
		//Step 2:
		$cart_reservation_data = Reservation::getCompleteReservationData($order_id);
		$itemsobj = unserialize($cart_reservation_data->data);
		$oldCartSessionid = $itemsobj->userchoice['cartsessionid'][0];
		
		
		
		//Step 3:
		try {
			$existingCartObject = db_select('cartsession', 'c')
			->fields('c')
			->condition('sessionid', $oldCartSessionid, '=')
			->execute()->fetchObject();
		} catch (Exception $e) {
		
		}
		
		
		//Step 4:
		try {
			$insertresults = db_insert('cartsession')
			->fields(array(
					'cartid' => $existingCartObject->cartid,
					'sessionid' => $order_id,
					'bookitems' => $existingCartObject->bookitems,
					'createdtime' => time(),
					'feeitems' => $existingCartObject->feeitems,
					'promocode' => $existingCartObject->promocode,
					'giftcard' => $existingCartObject->giftcard,
					'vouchercodes' => $existingCartObject->vouchercodes,
					'tickets' => $existingCartObject->tickets,
					'enhancements' => $existingCartObject->enhancements,
					'ticketstotalprice' => $existingCartObject->ticketstotalprice,
					'enhancementstotalprice' => $existingCartObject->enhancementstotalprice,
					'subtotal' => $existingCartObject->subtotal,
					'tax' => $existingCartObject->tax,
					'total' => $existingCartObject->total,
					'tax_lineitem' => $existingCartObject->tax_lineitem,
					'discount_lineitem' => $existingCartObject->discount_lineitem,
			))
			->execute();
		} catch (Exception $ex) {
		
		}
	
	return drupal_get_form("booking_update_order_form", $order_id,$existingCartObject);
	
}




// function booking_update_order_form_validate($form, &$form_state) {
	
// // 	//var_dump($form_state['triggering_element']['#value']); exit();
// 	if ($form_state['triggering_element']['#value'] == 'Apply') {
// 		$form_state['rebuild'] = FALSE;
// 		return $form;
// 	}
// // 	if ($form_state['triggering_element']['#value'] == 'Update Tickets / Enhancements') {
// // 		return;
// // 	}
// }





function booking_update_order_form($form, &$form_state,$order_id,$existingCartObject) {

	
	
	
	//Loading Ubercart Order
	
	
	drupal_add_css(drupal_get_path('module', 'booking_orders') . '/css/booking_orders.css', array('group' => CSS_DEFAULT, 'type' => 'file'));
    drupal_add_js(drupal_get_path('module', 'booking_orders') . '/booking_orders.js');
    drupal_add_css(drupal_get_path('module', 'booking_orders') . '/css/datepicker.css', array('group' => CSS_DEFAULT, 'type' => 'file'));
    drupal_add_css(drupal_get_path('module', 'booking_orders') . '/css/mdp.css', array('group' => CSS_DEFAULT, 'type' => 'file'));

    
 


    
    

    //hidden values for tickets and enhancements
    $oldorderTickets = unserialize($existingCartObject->tickets);
    $oldOrderTicketQty = 0;
    foreach ($oldorderTickets as $ticket) {
        $oldOrderTicketQty+=$ticket["qty"];
    }

    $oldorderEnhancements = unserialize($existingCartObject->enhancements);
    $oldOrderEnhancementQty = 0;
    foreach ($oldorderEnhancements as $enhancement) {
        if (CruiseEnhancements::is_window_inventory_item($enhancement['enhancedproductnodeid'])) {
            $oldOrderEnhancementQty+=$enhancement['qty'];
        }
    }

    
       

    //Step 5: Getting The Order Data
    $reservationData = Reservation :: getReservationData($order_id)->fetchObject();
    $r_id = $reservationData->id;
    $r_eventid = $reservationData->eventid;

    
    
    
    $cruiseeventid = $r_eventid;
    $eventdetails = scheduledevents_load($cruiseeventid);
    $eventsacheduleddate = getScheduleddatebyeventId($cruiseeventid);

    $portId = $eventdetails->port_id;
    $cruisedeparturetime = $eventdetails->departuretime;
    $cruisereturntime = $eventdetails->returntime;
    $cruiseborading = $eventdetails->boardingtime;
    $availablestock = $eventdetails->availabletickets;
    $availablewindowstock = $eventdetails->availablewindowseats;
    $portData = loadportById($portId);


    
    //Getting Template id by Event id
    $cruise_template_id='';
    try {
        $scheduledeventsObject = db_select('scheduledevents', 's')
                        ->fields('s', array('cruisetemplate_id'))
                        ->condition('id', $r_eventid)
                        ->execute()->fetchObject();
        $cruise_template_id = $scheduledeventsObject->cruisetemplate_id;
    } catch (Exception $e) {
        watchdog("scheduleevents", $e);
    }
    
    
    //Get the cruise Data
    $cruiseObject = cruise_load($scheduledeventsObject->cruisetemplate_id);
    $cruisetitle = isset($cruiseObject->cruise_title) ? $cruiseObject->cruise_title : '';
    
    
    $cruiseoptions = array();
    $eventsacheduleddatestr = date('Y-m-d', $eventsacheduleddate);
    $result = array();
    $result = getCruiseEventsDataBySelected_Date($eventsacheduleddatestr);
    foreach ($result as $row) {
    	$cruiseoptions[$row['id']] = $row['cruise_title'].' ('.$row['port'].') --'.$row['cruisedeparturetime'];
    }
    
    
    
	//Unset the Session Variable
    $currentorderid="currentorderid".$order_id;
	if(isset($_SESSION[$currentorderid])){
		unset($_SESSION[$currentorderid]);
	}
    //Session for Change Cruise functionality
    $oldOrderObject = (object) array(
        'oldOrderTicketQty' => $oldOrderTicketQty,
        'oldOrderEnhancementQty' => $oldOrderEnhancementQty,
        'oldOrderEventId'=>$r_eventid,
        'oldOrderCruiseTemplateId'=>$cruise_template_id,
    );
    $_SESSION[$currentorderid] = $oldOrderObject;
    

    
    
    //Getting the Event ID Based on OrderID
    $eventId = Reservation::reservation_get_eventid($order_id);

    
    
    $form['oldOrderTicketQty'] = array(
        '#id' => 'oldOrderTicketQty',
        '#type' => 'textfield',
        '#prefix' => '<div style="display:none;">',
        '#suffix' => '</div>',
        '#default_value' => $oldOrderTicketQty,
    );


    $form['oldOrderEnhancementQty'] = array(
        '#id' => 'oldOrderEnhancementQty',
        '#type' => 'textfield',
        '#prefix' => '<div style="display:none;">',
        '#suffix' => '</div>',
        '#default_value' => $oldOrderEnhancementQty,
    );

    $form['hiddenreservationid'] = array(
        '#id' => 'hiddenreservationid',
        '#type' => 'textfield',
        '#prefix' => '<div style="display:none;">',
        '#suffix' => '</div>',
        '#default_value' => $r_id,
    );


    $form['hiddenorderid'] = array(
        '#id' => 'hiddenorderid',
        '#type' => 'textfield',
        '#prefix' => '<div style="display:none;">',
        '#suffix' => '</div>',
        '#default_value' => $order_id,
    );


    $form['hiddencruisetemplateid'] = array(
        '#id' => 'hiddencruisetemplateid',
        '#type' => 'textfield',
        '#prefix' => '<div style="display:none;">',
        '#suffix' => '</div>',
        '#default_value' => $cruise_template_id,
    );


    $form['hiddeneventid'] = array(
        '#id' => 'hiddeneventid',
        '#type' => 'textfield',
        '#prefix' => '<div style="display:none;">',
        '#suffix' => '</div>',
        '#default_value' => $r_eventid,
    );



    //for event js
    $form['hiddenselectedcruisedate'] = array(
        '#id' => 'hiddenselectedcruisedate',
        '#type' => 'textfield',
        '#prefix' => '<div style="display:none;">',
        '#suffix' => '</div>',
    );



    $form['orderheader']=array(
    		'#type'=>'markup',
    		'#markup'=>'<div class="row-fluid"><nav class="span12"><ul style="display:block;" class="breadcrumb">
			<li ><a href="'.base_path().'staffadmin/order/'.$order_id.'/info">Order Info</a></li> <span class="icon-chevron-right"></span>
			<li class="active"><h4>Cart</h4></li> <span class="icon-chevron-right"></span>
    				<li><a href="'.base_path().'staffadmin/order/'.$order_id.'/payment">Payment</a></li> <span class="icon-chevron-right"></span>
			<li><a href="'.base_path().'staffadmin/order/'.$order_id.'/history">History</a></li>
	
			</nav></div>',
    );

    
    
    $form['cartinfo']['eventdetails'] = array(
        '#type' => 'fieldset',
        '#title' => '<span class="icon-th-large"></span>&nbsp;'.t('Event Details'),
        '#prefix' => '<div class="row-fluid">',
        '#suffix' => '</div>',
    );

    $eventdetailshtml = '<table>
    					<tr><td colspan="2"></td></tr>     
    					<tr><td class="tdalignright">Cruise Title :</td><td>'.$cruisetitle.'</td></tr>
    					<tr><td class="tdalignright">Cruise Date :</td><td>'. date('m/d/Y', $eventsacheduleddate).'</td></tr>
    					<tr><td class="tdalignright">Port :</td><td>'.$portData['portname'].'</td></tr>
    					<tr><td class="tdalignright">Boarding Time :</td><td>'.$cruiseborading.'</td></tr>
    					<tr><td class="tdalignright">Departure Time :</td><td>'.$cruisedeparturetime.'</td></tr>
    					<tr><td class="tdalignright">Return Time :</td><td>'.$cruisereturntime.'</td></tr>
						<tr><td>&nbsp;</td></tr>
    					<tr><td class="tdalignright">Available Tickets :</td><td>'.$availablestock.'</td></tr>
						<tr><td class="tdalignright">Available Windows :</td><td>'.$availablewindowstock.'</td></tr></table>';



    $form ['cartinfo']['eventdetails']['span41end'] = array(
        '#type' => 'markup',
        '#markup' => '<div class="span5"><div id="eventdetailsdiv">' . $eventdetailshtml . '</div></div>',
    );


    


    
    
    $form['cartinfo']['eventdetails']['crusiestartenddate'] = array(
        '#type' => 'markup',
        '#markup' => '',
    );

    $form['cartinfo']['eventdetails']["cruisetype"] = array(
        '#type' => 'select',
        '#options' => $cruiseoptions,
        '#default_value' => $eventId,
        '#attributes' => array('multiple' => array('multiple'), 'onclick' => 'geteventhtmldata()','class'=>array('eventslist')),
        '#prefix' => '<div class="span7"><b>Cruise</b>',
        '#suffix'=>'',
    );
    
    $form['cartinfo']['eventdetails']["cruisedate"] = array(
    		'#type' => 'markup',
    		'#markup' => ' 
                  <div id="datePick">
    		      <script type="text/javascript">
                  var eventday = new Date(' . $eventsacheduleddate . '* 1000);
                  $("#hiddenselectedcruisedate").val("' . $hiddencruis_event_edate . '");
                      $(function() {
                             $("#datePick").multiDatesPicker({
                                   addDates :[eventday],
                                   defaultDate:eventday,
                                   mode: "daysRange",
                                   autoselectRange: [0,1],
                                   onSelect : function(dateText, inst){
                                      getcruiseevents(dateText);
                                   }
                                   });
                                 });
                      </script>',
    );

    


    //Redirection to Cruise Change Page
    $form['cartinfo']['eventdetails']['changecruise'] = array(
        '#id' => 'changecruisebtn',
        '#type' => 'button',
        '#value' => t('Change Cruise'),
        '#attributes' => array('onclick' => 'changecruise()'),
        '#prefix' => '',
        '#suffix' => '</div> </div>',
    );
   
    
    
    $templateid=$cruise_template_id;

    //Loading all the Tickets by Cruise Template ID
    $form ['cartinfo']['widgetsrowfluidstart'] = array(
        '#type' => 'markup',
        '#markup' => '<div class="row-fluid">',
    );


    $form ['cartinfo']['widgetscolumnonestart'] = array(
        '#type' => 'markup',
        '#markup' => '<div class="span7">',
    );

    $availableTickets = getCruiseTicketByWeight($templateid);
    $form['cartinfo']['TICKETS'] = array(
        '#type' => 'fieldset',
        '#title' => '<span class="icon-th-large"></span>&nbsp;'.t('Tickets'),
        '#prefix' => '<div class="row-fluid"><div class="span12">',
        '#suffix' => '</div></div>',
    );

    

    //Ticketentrees
    $ticketEntrees = array();
    foreach ($availableTickets as $ticket) {
        $data = Cruise::getTicketEntrees($ticket->id, $templateid);
        $arr = array('' => '--Select--');
        foreach ($data as $key => $val) {
            $arr[$key] = $val;
            $ticketEntrees[$ticket->id] = $arr;
        }
    }
    
    $form['cartinfo']['TICKETS']['ticketwidgetstart']=array(
    		'#type'=>'markup',
    		'#markup'=>'<table><tr><td>&nbsp;&nbsp;</td><td><b>Ticket</b></td><td><b>Price</b></td><td><b>Qty</b></td></tr>',
    );



    foreach ($availableTickets as $ticket) {
    	
        $defaultvalue = Reservation::reservation_get_ticket_qty($ticket->id, $order_id);
        $ticketlabel = $ticket->title . " $" . wwcFormatPrice($ticket->price);
        
        $form ['cartinfo'] ['TICKETS'] ['ticketname' . $ticket->id] = array(
        		'#type' => 'markup',
        		'#prefix' => '<tr><td>&nbsp;&nbsp;</td><td>'	. $ticket->title . '</td>',
        );
        
        $customprice = Reservation::reservation_get_ticket_custom_price($ticket->id, $order_id);
        if (!is_null($customprice)) {
        	$price = $customprice;
        	if ($price > 0) {
        	} else {
        		$price = $ticket->price;
        	}
        } else {
        	$price = $ticket->price;
        }
        
        $form ['cartinfo'] ['TICKETS'] ['ticketentrees' . $ticket->id . "price"] = array(
        		'#type' => 'textfield',
        		'#default_value' => sprintf("%.2f", $price),
        		'#prefix' => '<td>',
        		'#suffix' => '</td>',
        );
        
        $form ['cartinfo'] ['TICKETS'] ['ticketentrees' . $ticket->id] = array(
            '#type' => 'select',
       		'#prefix' => '<td>',
       		'#suffix' => '</td></tr>',
//             '#options' => array(
//                 0 => 0,
//                 1 => 1,
//                 2 => 2,
//                 3 => 3,
//                 4 => 4,
//                 5 => 5,
//                 6 => 6,
//                 7 => 7,
//                 8 => 8,
//                 9 => 9,
//             ),
        	'#options' =>   drupal_map_assoc(range(0, 50)),
            '#default_value' => $defaultvalue,
            '#attributes' => array(
                'class' => array(
                    'input-mini'
                )
            ),
            '#ajax' => array(
                'callback' => 'ticketentrees_updateorder_callback',
                'wrapper' => 'ticketentreesDiv',
                'method' => 'html',
                'effect' => 'fade'
            ),
        );


        $form ['cartinfo'] ['TICKETS'] ['ticketentrees' . $ticket->id . "ticketname"] = array(
            '#type' => 'textfield',
            '#default_value' => $ticket->title,
            '#prefix' => '<div style="display:none;">',
            '#suffix' => '</div>'
        );


        
        $form['cartinfo']['TICKETS']['ticketentreesoption' . $ticket->id] = array(
            '#type' => 'hidden',
            '#value' => $ticket->id,
        );
    }//foreach

    
    
    $form['cartinfo']['TICKETS']['ticketwidgetend']=array(
    		'#type'=>'markup',
    		'#markup'=>'</table>',
    );
    
    

    $form['cartinfo']['TICKETS']['ticketentrees_fieldset'] = array(
        '#type' => 'markup',
        '#prefix' => '<div id="ticketentreesDiv">',
        '#suffix' => '</div>',
    );
    
   
    $displayoption = "none;";
    foreach ($availableTickets as $ticket) {
        $numoftickets = 0;
        $numoftickets = Reservation::reservation_get_ticket_qty($ticket->id, $order_id);
        if (isset($form_state['values']['ticketentrees' . $ticket->id])) {
            $numoftickets = $form_state['values']['ticketentrees' . $ticket->id];
            
        } 
        //} else
//         if (isset($values['ticketentrees' . $ticket->id])) {
//             $numoftickets = $values['ticketentrees' . $ticket->id];
//         }
        // Showing the div if number of tickets > 0
        if ($numoftickets > 0) {
            $displayoption = "block;";
            break;
        }
    }
    
    
    

    $form['cartinfo']['TICKETS']['ticketentrees_fieldset']['entrystart'] = array(
        '#type' => 'markup',
        '#markup' => '<div class="row-fluid" id="entreestartdiv" style="float:left;display:' . $displayoption . '"><div class="span12 purchaseTicketDetails">'
    );

    $entrhtml='';
    $entrhtml .= '<span class="icon-hand-right"></span><span> &nbsp; <b>Select your Entree for each cruise ticket below </b></span><table><tr><td>&nbsp;&nbsp;</td>';
    $entrhtml .= '<td><b>Qty</b></td>';
    $entrhtml .= '<td><b>Ticket</b></td>';
    $entrhtml .= '<td><b>Entree</b></td>';
    $entrhtml .= '</tr>';

    $form['cartinfo']['TICKETS']['ticketentrees_fieldset']['header'] = array(
        '#type' => 'markup',
        '#markup' => $entrhtml,
    );



    foreach ($availableTickets as $ticket) {
        $numoftickets = '';
        if (isset($form_state['values']['ticketentrees' . $ticket->id])) {
            $numoftickets = $form_state['values']['ticketentrees' . $ticket->id];
        } else if (isset($values['ticketentrees' . $ticket->id])) {
            $numoftickets = $values['ticketentrees' . $ticket->id];
        } else {
            $numoftickets = Reservation::reservation_get_ticket_qty($ticket->id, $order_id);
            $selectedEntrees = Reservation::reservation_get_ticket_entrees($ticket->id, $order_id);
            $selectedEntreeComments=Reservation::reservation_get_ticket_entree_comments($ticket->id, $order_id);
      }
        
        

        for ($i = 0; $i < $numoftickets; $i++) {
            $selectdefaultvalue = $selectedEntrees[0][$i];
            $enteredcomment=$selectedEntreeComments[0][$i];
            
            $expstr=$selectdefaultvalue."-".$i."-";
            $commentstrarr=explode($expstr, $enteredcomment);
            if($commentstrarr){
            	$commentvalue=$commentstrarr[1];
            }
           
            $form['cartinfo']['TICKETS']['ticketentrees_fieldset']['entry' . $ticket->id]["qty" . $ticket->id . $i] = array(
                '#type' => 'markup',
                '#markup' => '<tr><td>&nbsp;&nbsp;</td><td>1</td>',
            );

            $form['cartinfo']['TICKETS']['ticketentrees_fieldset']['entry' . $ticket->id]["name" . $ticket->id . $i] = array(
                '#type' => 'markup',
                '#markup' => "<td>" . $ticket->title . "</td>"
            );

            $form ['cartinfo'] ['TICKETS'] ['ticketentrees_fieldset'] ['entry' . $ticket->id] ["select" . $ticket->id . $i] = array(
                '#id' => 'option-entree-' . $ticket->id . '-' . $i,
                '#type' => 'select',
                '#options' => isset($ticketEntrees[$ticket->id]) ? $ticketEntrees[$ticket->id] : '',
                '#default_value' => $selectdefaultvalue,
                '#prefix' => "<td>",
                '#suffix' => "</td></tr>",
            );
        

            $form['cartinfo']['TICKETS']['ticketentrees_fieldset']['entry' . $ticket->id]["dietarycomments" . $ticket->id . $i] = array(
              '#id' => 'dietarycomments-' . $ticket->id . '-' . $i,
              '#type' => 'textarea',
              '#default_value' => $commentvalue,
	      		'#prefix' => "<tr><td colspan=3 style='text-align:right;'><i></i></td><td>",
          		'#suffix' => "</td></tr>",
           		'#attributes'=>array('class'=>array('textareafield')),
           );
        }
    }





    $form['cartinfo']['TICKETS']['ticketentrees_fieldset']['entryend'] = array(
        '#type' => 'markup',
        '#markup' => '</table></div></div>',
    );


    /*
     * Enhancements
     */


    $form['cartinfo']['enhancements'] = array(
        '#type' => 'fieldset',
        '#title' => '<span class="icon-th-large"></span>&nbsp;'.t('Enhancement Information'),
        '#prefix' => '<div class="row-fluid"><div class="span12">',
        '#suffix' => '</div></div></div>',
    );


    //$cruiseTemplateSelectedEnhancments = CruiseEnhancements::getSelectedEnhancements($templateid, $eventId);
    $cruiseTemplateSelectedEnhancments = CruiseEnhancements::getCruiseSelectedEnhancements($templateid, $eventId);
    $orderedenhancements = Reservation::getReservationEnhancementsDataByGroup($order_id);

    $totalproducts = loadCatalogProducts();
    
    $form['cartinfo']['enhancements']["widgetstart"] = array(
    	'#type' => 'markup',
    	'#markup' => "<table>",
    );
    
    

    foreach ($totalproducts as $productkey => $productvalue) {
        foreach ($productvalue as $optionkey => $optionval) {

            $form['cartinfo']['enhancements'][$optionkey."start"] = array(
                '#type' => 'markup',
                '#markup' => "<tr><td>&nbsp;&nbsp;</td><td><span class='icon-circle-arrow-right'></span>&nbsp;&nbsp;<b>".$optionkey."</b></td></tr>",
            );
            
            
             
            foreach ($optionval as $enhancementvalue) {
                if (in_array($enhancementvalue ['nid'], $cruiseTemplateSelectedEnhancments)) {
                    $form ['cartinfo'] ['enhancements'] [$optionkey]['enhancementtitle' . $enhancementvalue ['nid']] = array(
                        '#type' => 'markup',
                        '#markup' => "<tr><td>&nbsp;&nbsp;</td><td>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;".$enhancementvalue ['title']."</td>",
                    );


                    $form ['cartinfo'] ['enhancements'] [$optionkey]['_enhancementtitle' . $enhancementvalue ['nid']] = array(
                        '#type' => 'textfield',
                        '#default_value' => $enhancementvalue ['title'],
                        '#prefix' => '<div style="display:none;">',
                        '#suffix' => '</div>'
                    );



                    $form ['cartinfo'] ['enhancements'] [$optionkey]['enhancement' . $enhancementvalue ['nid']] = array(
                        '#type' => 'textfield',
                        '#default_value' => $enhancementvalue ['nid'],
                        '#prefix' => '<div style="display:none;">',
                        '#suffix' => '</div>',
                    );

                    $enhancementPrice = '';
                    $customPrice = isset($orderedenhancements[$enhancementvalue['nid']]["price"]) ? $orderedenhancements[$enhancementvalue['nid']]["price"] : 0;
                    if ($customPrice) {
                        $enhancementPrice = $customPrice;
                    } else {
                        $enhancementPrice = $enhancementvalue['price'];
                    }


                    $form ['cartinfo'] ['enhancements'] [$optionkey] ['price' . $enhancementvalue ['nid']] = array(
                        '#type' => 'textfield',
                        '#default_value' => sprintf("%.2f", $enhancementPrice),
                        '#prefix' => '<td>',
                        '#suffix' => '</td>',
                        '#attributes' => array('style' => 'width:75% !important;'),
                    );
                    
                    
                    $form ['cartinfo'] ['enhancements'] [$optionkey] ['qty' . $enhancementvalue ['nid']] = array(
                        '#type' => 'select',
                    	'#options' =>   drupal_map_assoc(range(0, 20)),
                        '#default_value' => $orderedenhancements[$enhancementvalue['nid']]['qty'],
                        '#prefix' => '<td>',
                        '#suffix' => '</td><td>&nbsp;&nbsp;</td></tr>',
                    		'#attributes' => array(
                    				'class' => array(
                    						'input-mini'
                    				)
                    		),
                    );
                    
                    
                    $form ['cartinfo'] ['enhancements'] [$optionkey] ['comments' . $enhancementvalue ['nid']] = array(
                    		'#type' => 'textarea',
                    		'#default_value' => $orderedenhancements[$enhancementvalue['nid']]['comments'],
                    		'#prefix' => '<tr><td colspan="4">',
                    		'#suffix' => '</td></tr>',
                    );
                }
            }
        }
    }
    
    
    
    $form['cartinfo']['enhancements']["widgetend"] = array(
    		'#type' => 'markup',
    		'#markup' => "</table>",
    );
    
    

    
    
    $form['cartinfo']['cruisefees'] = array(
        '#type' => 'fieldset',
        '#title' => '<span class="icon-th-large"></span>&nbsp;'.t('Cruise Fees'),
        '#prefix' => '<div class="span5"><div class="row-fluid"><div class="span12">',
        '#suffix' => '</div></div>',
    );
    
    
    $form['cartinfo']['cruisefees']['start'] = array(
    		'#type' => 'markup',
    		'#markup'=>'<table>'
    );



    /*     * *********************************
     * 
     * FUNC : FEE LINE ITEMS
     *   Load all cruise fee items
     *   Note : Get only Fee items if it is included in the order 
     * 
     * ********************************* */
    $feeItems = CartSession::getCruiseFeeItems($order_id);
    foreach ($feeItems as $feeitem) {
        $feeid = $feeitem['feeid'];
        $title = $feeitem['title'];
        $actualrate = $feeitem['actualrate'];
        $feeValue = $feeitem['updatedrate'];
        $totalamt = $feeitem['totalamt'];
        $form['cartinfo']['cruisefees']['cruisefee' . $feeid] = array(
            '#type' => 'textfield',
            '#default_value' => $feeValue,
            '#prefix' => '<tr><td>' . $title . '</td><td>',
            '#suffix' => '</td></tr>',
        );
    }
    $form['cartinfo']['cruisefees']['end'] = array(
    		'#type' => 'markup',
    		'#markup'=>'</table>'
    );
    
     
     

      
      
      
      
      
      

      
   
      
      
   

    //Getting Additional Line Items
    $taxLineItemsSerialized = CartSession::getTaxLineItem($order_id);
    $taxLineItemName = '';
    $taxChargeType = '';
    $taxCheckBoxValue = '';
    $taxLineItemAmt = '';
    if ($taxLineItemsSerialized) {
        $taxLineItemsArray = unserialize($taxLineItemsSerialized);
        foreach ($taxLineItemsArray as $taxname => $values) {
            $taxLineItemName = $values['taxname'];
            $taxChargeType = $values['chargetype'];
            $taxCheckBoxValue = $values['checkbox'];
            $taxLineItemAmt = $values['amt'];
        }
    }
    
    $form['cartinfo']['lineitemcharges'] = array(
    		'#type' => 'fieldset',
    		'#title' =>'<span class="icon-th-large"></span>&nbsp;'. t('Line Item Charge / Discount'),
    );
    
   
    $form['cartinfo']['lineitemcharges']['chargedescription'] = array(
    		'#type' => 'textfield',
    		'#title' => t('Charge'),
    		'#attributes' => array('style' => 'width:74% !important;'),
    		'#prefix' => '<div class="row-fluid"><div class="span12" style="margin-left:5% !important;">',
    		'#suffix' => '</div></div>',
    		'#default_value' => $taxLineItemName,
    );
    
    


    $form['cartinfo']['lineitemcharges']['charge'] = array(
        '#type' => 'checkbox',
        '#title' => t('Taxed'),
        '#prefix' => '<div class="row-fluid"><div class="span4" style="padding-left:12px; !important">',
    	'#suffix' => '</div>',
        '#default_value' => $taxCheckBoxValue,
    );

    $form['cartinfo']['lineitemcharges']['chargetype'] = array(
        '#type' => 'select',
        '#title' => t(''),
        '#options' => array(
            'dollaramount' => t('Dollar Amount'),
            'percentage' => t('Percentage')),
         '#prefix' => '<div class="span4">',
    	'#suffix' => '</div>',
       '#attributes' => array('style' => 'width:100% !important;'),
        '#default_value' => $taxCheckBoxValue,
    	
    );

    $form['cartinfo']['lineitemcharges']['chargevalue'] = array(
        '#type' => 'textfield',
        '#title' => t(''),
        '#prefix' => '<div class="span4">',
    	'#suffix' => '</div></div>',
       '#attributes' => array('style' => 'width:74% !important;'),
        '#default_value' => $taxLineItemAmt,
    		
    );


    //Getting Additional Line Items
    $discountLineItemsSerialized = CartSession::getDiscountLineItem($order_id);

    $discountName = '';
    $discountType = '';
    $discountCheckBoxValue = '';
    $discountLineItemAmt = '';
    if ($discountLineItemsSerialized) {
        $discountLineItemsArray = unserialize($discountLineItemsSerialized);
        foreach ($discountLineItemsArray as $discountname => $values) {
            $discountName = $values['discountname'];
            $discountType = $values['discounttype'];
            $discountCheckBoxValue = $values['checkbox'];
            $discountLineItemAmt = $values['amt'];
        }
    }


    
    $form['cartinfo']['lineitemcharges']['discountdescription'] = array(
    		'#type' => 'textfield',
    		'#title' => t('Discount'),
    		'#prefix' => '<div class="row-fluid"><div class="span12" style="margin-left:5% !important;">',
    		'#suffix' => '</div></div>',
    		'#default_value' => $discountName,
    		'#attributes' => array('style' => 'width:74% !important;'),
    );
    
    

    $form['cartinfo']['lineitemcharges']['discount'] = array(
        '#type' => 'checkbox',
        '#title' => t('Pre-Tax'),
        '#prefix' => '<div class="row-fluid"><div class="span4" style="padding-left:12px; !important">',
    	'#suffix' => '</div>',
        '#default_value' => $discountCheckBoxValue,
    );


  

    $form['cartinfo']['lineitemcharges']['discounttype'] = array(
        '#type' => 'select',
        '#title' => t(''),
        '#options' => array(
            'dollaramount' => t('Dollar Amount'),
            'percentage' => t('Percentage')),
         '#prefix' => '<div class="span4">',
    	'#suffix' => '</div>',
        '#default_value' => $discountType,
    	'#attributes' => array('style' => 'width:100% !important;'),
    );

    $form['cartinfo']['lineitemcharges']['discountvalue'] = array(
        '#type' => 'textfield',
        '#title' => t(''),
       '#prefix' => '<div class="span4">',
    	'#suffix' => '</div></div>',
       '#attributes' => array('style' => 'width:74% !important;'),
        '#default_value' => $discountLineItemAmt,
    		
    );
    
    



    
    //Tax
    $taxItems = unserialize(CartSession::getTaxAmtItems($order_id));
    
    if (isset($taxItems['taxvalue'])) {
        $taxValue = $taxItems['taxvalue'];
    } else {
        $taxValue = variable_get('WWCTAXVALUE', '');
    }

    $form['cartinfo']['tax'] = array(
        '#type' => 'fieldset',
        '#title' => '<span class="icon-th-large"></span>&nbsp;'.t('Tax'),
        '#prefix' => '<div class="row-fluid"><div class="span12">',
        '#suffix' => '</div></div>',
    );


    $form['cartinfo']['tax']['taxvalue'] = array(
        '#type' => 'textfield',
        '#default_value' => $taxValue,
        '#prefix' => '<table><tr><td>Tax</td><td>',
        '#suffix' => '</td></tr></table>',
    );

    
    //**** Ticket Div *****
    if (CartSession::getTickets($order_id)) {
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
    
    
    
    //**** Enhancements Div *****
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
    
    
    
    
    //**** Cruise Fee Line Items Div *****
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
    
    
    
    
    //Coupon Code
    $cpndiscounthtml='';
    $cpnarray = unserialize ( CartSession::getPromocode ( $order_id ) );
    $cpncode = false;
    $cpndiscount = false;
    foreach ( $cpnarray as $key => $value ) {
    	$cpncode = $key;
    	$cpndiscount = $value;
    }
    
    if ($cpndiscount) {
    	$cpndiscounthtml = '<div id="COUPONDIV" style="margin-top:5%;">';
    		$cpndiscounthtml.='<div class="row-fluid"><div class="span5 offset1">Promocode '.$cpncode.'  </div> <div class="span6">-$'
    				. wwcFormatPrice($cpndiscount) . '</div></div>';
    	$cpndiscounthtml.='</div>';
    	
    }
    
    
    
    

 
    
    
    //Cruise Fee Line Items
    //var_dump($order_id); exit();
    $vouchershtml='';
    $vouchercodesserialized=CartSession::getVoucherCodes($order_id);
    $vouchershtml .= '<div id="VOUCHERSDIV">';
    if($vouchercodesserialized){
    	$vouchers=unserialize($vouchercodesserialized);
    	$vouchershtml .= '<div class="row-fluid"><div class="span11 offset1"><b></b></div></div>';
    	foreach($vouchers as $key=>$value){
    		$vouchershtml.='<div class="row-fluid"><div class="span5 offset1"> Voucher -'
    				. $key . '</div><div class="span6"> -$'
    						. wwcFormatPrice($value) . '</div></div>';
    		 
    	}
    }
    
    $vouchershtml.='</div>';
    
    
    
    //giftcard
    $giftcardhtml='';
    $giftCardSerialized = CartSession::getGiftCard($order_id);
    $giftcardhtml = '<div id="GIFTCARDDIV" style="margin-top:5%;">';
    if ($giftCardSerialized) {
    	$giftCardArray = reset(unserialize($giftCardSerialized));
    	
    	$giftcardhtml.='<div class="row-fluid"><div class="span5 offset1">Gift Card </div> <div class="span6">-$'
    			. wwcFormatPrice ( $giftCardArray['deductedamount'] ) . '</div></div>';
    	
    }
    $giftcardhtml.='</div>';
    
    
    
    
    
    
    
    //Sub Total
    $orderSubTotal = CartSession::getOrderSubTotal($order_id);
    $subtotalhtml = '<div id="SUBTOTALDIV" style="margin-top:5%;">';
    if ($orderSubTotal != "") {
    	$subtotalhtml.='<div class="row-fluid"><div class="span5 offset1">Subtotal  </div> <div class="span6">$'
    					. wwcFormatPrice($orderSubTotal) . '</div></div>';
    }
    $subtotalhtml.='</div>';
    
    
    //Tax
    $taxamthtml = '<div id="TAXDIV">';
    $taxAmt = CartSession::getTaxAmt($order_id);
    if ($taxAmt != "") {
    	$taxamthtml.='<div class="row-fluid"><div class="span5 offset1">Tax </div> <div class="span6">$' . wwcFormatPrice($taxAmt) . '</div></div>';
    }
    
    $taxamthtml.='</div>';
    
    
    //Order Total
    $totalhtml = '<div id="ORDERTOTALDIV" style="margin-top:5%;">';
    $orderTotal = CartSession::getOrderTotal($order_id);
    if ($orderTotal != "") {
    	$totalhtml.='<div class="row-fluid"><div class="span5 offset1"> <b>Total</b>'
    			. ' </div> <div class="span6">$' . wwcFormatPrice($orderTotal) . '</div></div>';
    }
    
    $totalhtml.='</div>';
    
    
    
    //tax Line Item
    $taxlineitemsHtml='';
    $taxlineitemsHtml.= '<div id="TAXLINTEITEMDIV">';
    $taxLineItemSerialized = CartSession::getTaxLineItem($order_id);
    if ($taxLineItemSerialized) {
    	
    	$taxLineItemArray = unserialize($taxLineItemSerialized);
    	foreach ($taxLineItemArray as $key => $values) {
    		if ($values['amt'] > 0) {
    			$taxlineitemsHtml.='<div class="row-fluid"><div class="span5 offset1">' . $values['taxname'] . '</div> <div class="span6">$' . wwcFormatPrice($values['amt']) . '</div></div>';
    		}
    	}
    }
    $taxlineitemsHtml.='</div>';
    
    //discount line item
    $discountLineItemSerialized = CartSession::getDiscountLineItem($order_id);
    
    
    $discountlineitemHtml='';
    $discountlineitemHtml.= '<div id="DISCOUNTLINEITEMMDIV">';
    if ($discountLineItemSerialized) {
    	$discountLineItemArray = unserialize($discountLineItemSerialized);
    	foreach ($discountLineItemArray as $key => $values) {
    		if ($values['amt'] > 0) {
    			$discountlineitemHtml.='<div class="row-fluid"><div class="span5 offset1">' . $values['discountname'] . '</div><div class="span6">-$' . wwcFormatPrice($values['amt']) . '</div></div>';
    		}
    	}
    }
    $discountlineitemHtml.='</div>';
    
    


    $form['cartinfo']['orderwidgetdivsummary'] = array(
        '#type' => 'fieldset',
        '#title' => '<span class="icon-shopping-cart"></span>&nbsp;'.t ( 'Cart Summary' ),
        '#collapsed' => FALSE,
        '#prefix' => '<div clss="row-fluid"><div class="span12">',
        '#suffix' => '</div></div>',
    );

    $form['cartinfo']['orderwidgetdivsummary']['markup'] = array(
        '#type' => 'markup',
        '#markup' => '<div>' . $ticketHtml . '' . $taxlineitemsHtml . '' . $discountlineitemHtml . '' . $enhancementhtml . '' . $cruisefeehtml . '' .$cpndiscounthtml.''.$vouchershtml.''.$giftcardhtml.''. $subtotalhtml . '' . $taxamthtml . '' . $totalhtml . '</div>'
    );

    $form ['cartinfo'] ['finishorder'] ['calculateAmount'] = array(
        '#type' => 'button',
        '#value' => 'Update Tickets / Enhancements',
        '#prefix' => '<div class="row-fluid"><div class="span12">',
        '#suffix' => '</div></div>',
        '#ajax' => array(
            'callback' => 'calclateorderamount',
            'effect' => 'fade'
        )
    );
    
    
    $form['cartinfo'] ['promoandgiftcard']  = array(
    	'#type' => 'fieldset',
    	'#title' =>'<span class="icon-gift"></span>&nbsp;'. t('Promotional Code, Gift Card'),
    	'#prefix' => '<div class="row-fluid" style="margin-top:5%;"><div class="span12">',
		'#suffix' => '</div></div>' 
    );
    
    
    
//     //If Promocode Exists Disable the Apply and Textbox fields
    $promoserialized=CartSession::getPromocode($order_id);
    $promocode='';
    $promoprice='';
    
    if($promoserialized){
    	$promoArr=unserialize($promoserialized);
    	foreach($promoArr as $key=>$val){
    		$promocode=$key;
    		$promoprice=$val;
    	}
    }
    
    
    $form['cartinfo'] ['promoandgiftcard'] ['promocode'] = array(
    		'#type' => 'textfield',
    		'#default_value'=>$promocode,
    		'#prefix' => '<table><tr><td style="width:20% !important;">Promo Code</td><td>',
    		'#suffix' => '</td>',
    		'#attributes' => array('style' => 'width:75% !important;'),
    );
    
    $form ['cartinfo'] ['promoandgiftcard'] ['prmobtn'] = array (
    		'#limit_validation_errors' => array (),
    		'#id'=>'prmobtn',
    		'#type' => 'button',
    		'#name' => 'promocodeapplybutton',
    		'#value' => t ( 'Apply' ),
    		
    		'#ajax' => array (
    				'callback' => 'validatePromocodeInUpdateOrder'
    		),
    		'#attributes'=>array('disabled'=>'disabled'),
    		'#prefix' => '<td>',
    		'#suffix' => '</td></tr>',
  
    		
    );
    
    $giftCardOldAmt=0;
    $giftCardSerialized = CartSession::getGiftCard($order_id);
    if($giftCardSerialized){
    	$giftCardArray = reset(unserialize($giftCardSerialized));
    	$giftCardOldAmt= $giftCardArray['deductedamount'];
    }
    
    $form ['cartinfo'] ['promoandgiftcard'] ['giftcardoldamt'] = array (
    		'#type'=>'textfield',
    		'#default_value'=>$giftCardOldAmt,
    		'#prefix' => '<div style="display:none;">',
    		'#suffix' => '</div>',
    );
    
   
    
    
    $form['cartinfo'] ['promoandgiftcard'] ['giftcardnumber'] = array(
    		'#type' => 'textfield',
    		'#default_value'=>'',
    		'#prefix' => '<tr><td style="width:20% !important;">Gift Card</td><td>',
    		'#suffix' => '</td>',
    		'#attributes' => array('style' => 'width:75% !important;'),
    );
    
    $form ['cartinfo'] ['promoandgiftcard'] ['giftcardbtn'] = array (
//     		'#limit_validation_errors' => array (),
    		'#id'=>'giftcardbtn',
    		'#type' => 'button',
    		'#value' => t ( 'Apply' ),
    		'#attributes'=>array('disabled'=>'disabled'),
    		'#prefix' => '<td>',
    		'#suffix' => '</td></tr></table>',
    		'#ajax' => array (
    				'callback' => 'validateGiftcardInUpdateOrder'
    		),
    		
    		
    );
    	
    
//   



    $form ['cartinfo'] ['completeorder'] ['savecartforprocessing'] = array(
        '#type' => 'submit',
    	'#id'=>'updateordersavecartbtn',
        '#value' => 'Save Cart for Processing',
        '#prefix' => '<div class="row-fluid"><div class="span12">',
        '#suffix' => '</div></div>',
    	'#attributes'=>array('disabled'=>'disabled'),
        //'#submit' => array('savecartforprocessing'),
    );
    
    
  
    
	    return $form;
}




function ticketentrees_updateorder_callback($form, $form_state) {
	return $form['cartinfo']['TICKETS']['ticketentrees_fieldset'];
}






function calclateorderamount($form, &$form_state) {
	
	
	$_SESSION['bookingorderformflag']=true;
	
	
	$giftcardoldamt = $form_state ['values'] ['giftcardoldamt'];
	$taxamt = 0;
	$enhancementtotalprice = 0;
	$cruiseFeeItemPrice = 0;



	$order_id = $form_state ['values'] ['hiddenorderid'];
	$cruise_id = $form_state ['values'] ['hiddencruisetemplateid'];
	$event_id = $form_state ['values'] ['hiddeneventid'];
	
	

	$availabletickets = Scheduledevents::getAvailableTicketsCount($cruise_id, $event_id);
	$availablewindows = Scheduledevents::getAvailableWindowsCount($cruise_id, $event_id);

	$result = array();
	$totalresults = array();
	$arr = array();
	$totalticketsprice = 0;
	$tickets_tax = 0;
	$feeitems_tax = 0;
	
	
	//************************ TICKETS ***************************
	$currentselectticketcnt = 0;
	foreach ($form_state['values'] as $key => $value) {
		$reg1 = "/^(ticketentrees)?\d+$/";
		if (preg_match($reg1, $key)) {
			if ($form_state['values'][$key] > 0) {

				$ticketenteeeid = explode('ticketentrees', $key);
				$result = array();
				$ticketID = $ticketenteeeid[1];
				$result['ticketid'] = $ticketenteeeid[1];
				$result['qty'] = $form_state['values'][$key];
				$result['ticketname'] = $form_state['values'][$key . "ticketname"];
				$result['price'] = $form_state['values'][$key . "price"];


				$currentselectticketcnt += (int) $form_state['values'][$key];

				//---------------Ticket Entrees----------------
				$entrees = array();
				for ($i = 0; $i < $result['qty']; $i++) {
					$entrees[] = $form_state['values']['select' . $result['ticketid'] . $i];
				}
				$result['ticketentreeids'] = $entrees;

				//------------Ticket Entrees Comments (Dietary Restrictions)--------------
				$entreesComments = array();
				for ($i = 0; $i < $result['qty']; $i++) {
					$entreesComments[] = $form_state['values']['select' . $result['ticketid'] . $i] . '-' . $i . '-' . $form_state['values']['dietarycomments' . $result['ticketid'] . $i];
				}

				//-------- Ticket Entree Comments ends here--------------------
				$result['ticketentreescomments'] = $entreesComments;

				//1.2
				$taxableStatus = CruiseTickets::isTaxableTicket($ticketID);
				if ($taxableStatus) {
					$tickets_tax+=$form_state['values'][$key] * $form_state['values'][$key . "price"];
				}
				$totalresults[] = $result;
				$totalticketsprice+= $form_state['values'][$key] * $form_state['values'][$key . "price"];
			}
		}
	}//foreach
	
	
	
	
	//************************ ENHANCEMENTS ***************************
	$selectedwindowcnt = 0;
	$enhancementtotalprice = 0;

	$enhancementsresult = array();
	foreach ($form_state['values'] as $key => $value) {
		$reg1 = "/^(enhancement)?\d+$/";
		if (preg_match($reg1, $key)) {
			$enhancementid = explode('enhancement', $key);
			$eid = $enhancementid[1];
			if ($form_state['values']['qty' . $eid] > 0) {
				if (CruiseEnhancements::is_window_inventory_item($eid)) {
					$selectedwindowcnt+=$form_state['values']['qty' . $eid];
				}
				$result = array();
				$result['enhancedproductnodeid'] = $eid;
				$result['qty'] = $form_state['values']['qty' . $eid];
				$result['price'] = $form_state['values']['price' . $eid];
				$result['name'] = $form_state['values']['_enhancementtitle' . $eid];
				$result['comments']=$form_state['values']['comments' . $eid];
				$productprice = $result['qty'] * (float) $result['price'];
				$enhancementtotalprice+=$productprice;
				$enhancementsresult[] = $result;
			}
		}
	}//foreach
	
	
	//************************ Validating Tickets and Enhancements ***************************
	//validating tickets
	if($currentselectticketcnt==0){
		
		
		$commands [] = ajax_command_invoke('#updateordersavecartbtn', 'attr', array('disabled', true));
		$commands [] = ajax_command_invoke('#giftcardbtn', 'attr', array('disabled', true));
		$commands [] = ajax_command_invoke('#prmobtn', 'attr', array('disabled', true));
		
		$commands[] = ajax_command_alert("Please Select Tickets!");
		$commands [] = ajax_command_replace("#TICKETKTDIV", "<div id='TICKETKTDIV'></div>");
		$commands [] = ajax_command_replace("#ENHANCEMENTDIV", "<div id='ENHANCEMENTDIV'></div>");
		$commands [] = ajax_command_replace("#DISCOUNTLINEITEMMDIV", "<div id='DISCOUNTLINEITEMMDIV'></div>");
		$commands [] = ajax_command_replace("#CRUISEFEEDIV", "<div id='CRUISEFEEDIV'></div>");
		$commands [] = ajax_command_replace("#COUPONDIV", "<div id='COUPONDIV'></div>");
		$commands [] = ajax_command_replace("#VOUCHERSDIV", "<div id='VOUCHERSDIV'></div>");
		$commands [] = ajax_command_replace("#GIFTCARDDIV", "<div id='GIFTCARDDIV'></div>");
		$commands [] = ajax_command_replace("#SUBTOTALDIV", "<div id='SUBTOTALDIV'></div>");
		$commands [] = ajax_command_replace("#TAXDIV", "<div id='TAXDIV'></div>");
		$commands [] = ajax_command_replace("#ORDERTOTALDIV", "<div id='ORDERTOTALDIV'></div>");
		
	}
	
	elseif ($currentselectticketcnt > $availabletickets) {
		$commands[] = ajax_command_alert($availabletickets . " tickets are only available!");
	} else if ($selectedwindowcnt > $availablewindows) {
		$commands[] = ajax_command_alert($availablewindows . " windows are only available!");
	} else {

		CartSession::insertTickets($order_id, $totalresults);
		CartSession::insertTotalTicketsPrice($order_id, $totalticketsprice);
		CartSession::insertEnhancements($order_id, $enhancementsresult);
		CartSession::insertTotalEnhancementsPrice($order_id, $enhancementtotalprice);
		$feeItems = CartSession::getCruiseFeeItems($order_id);
		foreach ($feeItems as $feeitem) {
			$chargeamt = 0;
			$feeid = $feeitem['feeid'];
			$title = $feeitem['title'];
			$actualrate = $feeitem['actualrate'];
			$feeValue = $feeitem['updatedrate'];
			$totalamt = $feeitem['totalamt'];
			$feeType = $feeitem['type'];
			$new_value = $form_state['values']['cruisefee' . $feeid];
			foreach ($totalresults as $ticket) {
				if ($ticket ['qty'] > 0) {
					$ticket_id = $ticket ['ticketid'];
					$ticket_qty = $ticket ['qty'];
					$ticket_price = $ticket ['price'];
					$ticket_feeids = Fees::get_ticket_feeids($ticket_id);
					if ($ticket_feeids != "") {
						if (in_array($feeid, $ticket_feeids)) {
							$ticketfeeids[] = $feeid;
							if ($feeType == "flatfee") {
								$totalcharge = $ticket_qty * $new_value;
								$chargeamt += $totalcharge;
							}

							if ($feeType == "percentage") {
								$totalticketpriceamt = $ticket_qty * $ticket_price;
								$chargeamt += ($totalticketpriceamt * $new_value) / 100;
							}
						}
					}
					}
			}//tickets loop ends here

			$cruiseFeeItemPrice+=$chargeamt;
			$cruisefeeitems[$feeid] = array('feeid' => $feeid, 'title' => $title, 'type' => $feeType, 'actualrate' => $actualrate, 'updatedrate' => $new_value, 'totalamt' => $chargeamt);
		}
		CartSession::insertCruiseFeeItems($order_id, $cruisefeeitems);

		$feeitems_tax = 0;
		foreach ($cruisefeeitems as $key => $values) {
			$feeid = $values['feeid'];
			if (in_array($feeid, $ticketfeeids)) {
				$feeTaxableStatus = Fees::is_taxable_cruisefee_item($feeid);
				if ($feeTaxableStatus) {
					$feeitems_tax+=$values['totalamt'];
				}
			}
		}


		
		$cpndiscountamt = 0;
		
		//**********************  Additional Discount / Charge Line Items ***************
		$chargeTax = 0;
		$chargeAmt = 0;
		$chargetype = $form_state ['values']['chargetype'];
		$charge = $form_state ['values']['charge'];
		$chargedescription = $form_state ['values']['chargedescription'];
		$chargevalue = is_numeric($form_state ['values']['chargevalue']) ? $form_state ['values']['chargevalue'] : 0;
	
		$chargeTaxName = '';
		$chargeTaxName = $chargedescription;

		//Type is Dollar Amount

		/*
		 * Case 1: (Charge) Checkbox checked and Type is Dollar Amount, Add Charge Tax to Tax calculations
		*
		*/
		if ($chargetype == "dollaramount") {
			//checkbox is checked and type is dollar amount
			if ($charge == 1) {
				$chargeTax = $chargevalue;
				$chargeTaxValue = $chargeTax;
			}
			else {
				$chargeAmt = $chargevalue;
				$chargeTaxValue = $chargeAmt;
			}
		} else {
			//PERCENTAGE
			//checkbox checked and type is percentage
			if ($charge == 1) {
				$percentagefee = ($totalTicketsamt + $enhancementtotalprice) * $chargevalue / 100;
				$chargeTax = $percentagefee;
				$chargeTaxValue = $chargeTax;
			}
			//checkbox unchecked and type is percentage
			else {
				$percentagefee = ($totalTicketsamt + $enhancementtotalprice) * $chargevalue / 100;
				$chargeAmt = $percentagefee;
				$chargeTaxValue = $chargeAmt;
			}
		}


		
		//insertion
		$tax_lineitem = array();
		$tax_lineitem[$chargeTaxName] = array('taxname' => $chargeTaxName, 'chargetype' => $chargetype, 'checkbox' => $charge, 'amt' => $chargeTaxValue);
		CartSession::insertTaxLineItem($order_id, $tax_lineitem);


		//Discounts
		$discounttype = $form_state ['values']['discounttype'];
		$discount = $form_state ['values']['discount'];
		$discountdescription = $form_state ['values']['discountdescription'];
		$discountvalue = is_numeric($form_state ['values']['discountvalue']) ? $form_state ['values']['discountvalue'] : 0;
		
		

		$descriptionDiscountName = $discountdescription;
		$discountTax = 0;
		$discountAmt = 0;
		//checkbox checked and type is dollaramount
		if ($discounttype == "dollaramount") {
			if ($discount == 1) {
				$discountTax = $discountvalue;
				$descriptionDiscountValue = $discountTax;
			}

			//checkbox unchecked and type is dollar amount
			else {
				$discountAmt = $discountvalue;
				$descriptionDiscountValue = $discountAmt;
			}
		} else {
			//percentage
			//checkbox checked and type is percentage
			if ($discount == 1) {
				$percentagefee = ($totalTicketsamt + $enhancementtotalprice) * $discountvalue / 100;
				$discountTax = $percentagefee;
				$descriptionDiscountValue = $discountTax;
			} else {
				//checkbox unchecked adn type is percentage
				$percentagefee = ($totalTicketsamt + $enhancementtotalprice) * $discountvalue / 100;
				$discountAmt = $percentagefee;
				$descriptionDiscountValue = $discountAmt;
			}
		}

		//insertion
		$discount_lineitem = array();
		$discount_lineitem[$descriptionDiscountName] = array('discountname' => $descriptionDiscountName, 'discounttype' => $discounttype, 'checkbox' => $discount, 'amt' => $descriptionDiscountValue);
		CartSession::insertDiscountLineItem($order_id, $discount_lineitem);

		
		
		
	
		$orderSubTotal = $totalticketsprice + $enhancementtotalprice + $cruiseFeeItemPrice + $chargeTax + $chargeAmt - $discountTax - $discountAmt;
		CartSession::insertOrderSubTotal($order_id, $orderSubTotal);

	

		$taxvalue = $form_state ['values']['taxvalue'];
		$taxarr = array();
		$taxamt = sprintf("%.2f", (($tickets_tax + $feeitems_tax + $enhancementtotalprice + $chargeTax) - $discountTax) * $taxvalue / 100);
		
		$taxarr['taxvalue'] = $taxvalue;
		$taxarr['tickets_tax'] = $tickets_tax;
		$taxarr['feeitems_tax'] = $feeitems_tax;
		$taxarr['enhancements_tax'] = $enhancementtotalprice;
		$taxarr['total_tax'] = (float) $taxamt;
		CartSession::insertTaxItems($order_id, serialize($taxarr));

		$voucheramt=0;
		CartSession::removePromocode($order_id);
		CartSession::removeVoucher($order_id);
		CartSession::removeGiftCard($order_id);
		
		
		
		


		//giftcard
// 		$giftCardSerialized = CartSession::getGiftCard($order_id);
// 		$giftCardArray = reset(unserialize($giftCardSerialized));
// 		$giftcardAmt=0;
// 		if($giftCardArray){
// 			$giftcardAmt=$giftCardArray['deductedamount'];
// 		}
		
		$giftcardAmt=$giftcardoldamt;
		
		//giftcard
		$giftcardhtml='';
		if ($giftcardAmt>0) {
			$giftcardhtml = '<div id="GIFTCARDDIV" style="margin-top:5%;">';
			$giftcardhtml.='<div class="row-fluid"><div class="span5 offset1">Gift Card </div> <div class="span6">-$'
					. wwcFormatPrice ( $giftcardAmt ) . '</div></div>';
			$giftcardhtml.='</div>';
		}
		$commands [] = ajax_command_replace("#GIFTCARDDIV", "<div id='GIFTCARDDIV'>" . $giftcardhtml . "</div>");
		
		
		
		
		
		//promocode
		$discountlineitemstotal=$promocodediscount+$voucheramt+$giftcardAmt;
		$orderTotalPrice = $orderSubTotal + $taxamt -$discountlineitemstotal;
		CartSession::insertOrderTotal($order_id, $orderTotalPrice);
		$tickets = unserialize ( CartSession::getTickets ( $order_id ) );
		
		
		
		//Ticket DIV
		$ticketHtml = '<div id="TICKETKTDIV">';
		$ticketHtml .= '<div class="row-fluid"><div class="span11 offset1"><b>Tickets</b> </div></div>';
		foreach ( $tickets as $ticket ) {
			if ($ticket ['qty'] > 0) {
				$amount = $ticket ['qty'] * $ticket ['price'];
				$ticketHtml .= '<div class="row-fluid"><div class="span5 offset1">' . $ticket ['qty'] . '- ' . $ticket ['ticketname'] . '</div><div class="span6">$' . wwcFormatPrice ( $amount ) . '</div></div>';
			}
		}
		$ticketHtml .= "</div>";
		$commands [] = ajax_command_replace("#TICKETKTDIV", "<div id='TICKETKTDIV'>" . $ticketHtml . "</div>");
		
				
	
		//Enhancements Div
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
		$commands [] = ajax_command_replace("#ENHANCEMENTDIV", "<div id='ENHANCEMENTDIV'>" . $enhancementhtml . "</div>");
		
		


		//Charge Tax Line Item Div
		$taxLineItemSerialized = CartSession::getTaxLineItem($order_id);
		$taxlineitemsHtml = '<div id="TAXLINTEITEMDIV">';
		if ($taxLineItemSerialized) {
			$taxLineItemArray = unserialize($taxLineItemSerialized);
			foreach ($taxLineItemArray as $key => $values) {
				if ($values['amt'] > 0) {
					$taxlineitemsHtml.='<div class="row-fluid"><div class="span5 offset1">'
							. $values['taxname'] . '</div><div class="span6">$' . wwcFormatPrice($values['amt'])
							. '</div></div>';
				}
			}
		}
		$taxlineitemsHtml.='</div>';
		$commands [] = ajax_command_replace("#TAXLINTEITEMDIV", "<div id='TAXLINTEITEMDIV'>" . $taxlineitemsHtml . "</div>");
		


		//Discount line item
		$discountLineItemSerialized = CartSession::getDiscountLineItem($order_id);
		$discountlineitemHtml = '<div id="DISCOUNTLINEITEMMDIV">';
		if ($discountLineItemSerialized) {
			$discountLineItemArray = unserialize($discountLineItemSerialized);
			foreach ($discountLineItemArray as $key => $values) {
				if ($values['amt'] > 0) {
					$discountlineitemHtml.='<div class="row-fluid"><div class="span5 offset1">'
							. $values['discountname'] . '</div><div class="span6">-$'
									. wwcFormatPrice($values['amt']) . '</div></div>';
				}
			}
		}
		$discountlineitemHtml.='</div>';
		$commands [] = ajax_command_replace("#DISCOUNTLINEITEMMDIV", "<div id='DISCOUNTLINEITEMMDIV'>" . $discountlineitemHtml . "</div>");
		

	
		
		//Cruise Fee Line Items
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
		$commands [] = ajax_command_replace("#CRUISEFEEDIV", "<div id='CRUISEFEEDIV'>" . $cruisefeehtml . "</div>");
		
		
		
		//Coupon Code
		$cpndiscounthtml='';
		$commands [] = ajax_command_replace("#COUPONDIV", "<div id='COUPONDIV'>" . $cpndiscounthtml . "</div>");
		
		$vouchershtml='';
		$commands [] = ajax_command_replace("#VOUCHERSDIV", "<div id='VOUCHERSDIV'>" . $vouchershtml . "</div>");
		
		
		
		
		
		
		
		//Sub Total Div
		$orderSubTotal = CartSession::getOrderSubTotal($order_id);
		$subtotalhtml = '<div id="SUBTOTALDIV" style="margin-top:5%;">';
		if ($orderSubTotal != "") {
			$subtotalhtml.='<div class="row-fluid"><div class="span5 offset1">Subtotal  </div> <div class="span6">$'
					. wwcFormatPrice($orderSubTotal) . '</div></div>';
		}
		$subtotalhtml.='</div>';
		$commands [] = ajax_command_replace("#SUBTOTALDIV", "<div id='SUBTOTALDIV'>" . $subtotalhtml . "</div>");
		
		
	
		//Tax Div
		$taxamthtml = '<div id="TAXDIV">';
		$taxAmt = CartSession::getTaxAmt($order_id);
		if ($taxAmt != "") {
			$taxamthtml.='<div class="row-fluid"><div class="span5 offset1">'
					. ' Tax </div><div class="span6">$'
							. wwcFormatPrice($taxAmt) . '</div></div>';
		}
		$taxamthtml.='</div>';
		$commands [] = ajax_command_replace("#TAXDIV", "<div id='TAXDIV'>" . $taxamthtml . "</div>");
		
		
		
		//Order Total Div
		$totalhtml = '<div id="ORDERTOTALDIV" style="margin-top:5%;">';
		$orderTotal = CartSession::getOrderTotal($order_id);
		if ($orderTotal != "") {
			$totalhtml.='<div class="row-fluid"><div class="span5 offset1"> <b>Total</b>'
					. ' </div> <div class="span6">$' . wwcFormatPrice($orderTotal) . '</div></div>';
		}
		
		$totalhtml.='</div>';
		$commands [] = ajax_command_replace("#ORDERTOTALDIV", "<div id='ORDERTOTALDIV'>" . $totalhtml . "</div>");
		
		
		$commands [] = ajax_command_invoke('#updateordersavecartbtn', 'attr', array('disabled', false));
		$commands [] = ajax_command_invoke('#giftcardbtn', 'attr', array('disabled', false));
		$commands [] = ajax_command_invoke('#prmobtn', 'attr', array('disabled', false));
		
		
	}
	return array(
			'#type' => 'ajax',
			'#commands' => $commands
	);
}









function booking_update_order_form_submit($form, &$form_state) {


	//Hidden Values
	$order_id = $form_state ['values'] ['hiddenorderid'];
	$cruise_id = $form_state ['values'] ['hiddencruisetemplateid'];
	$event_id = $form_state ['values'] ['hiddeneventid'];
	$r_id = $form_state ['values'] ['hiddenreservationid'];
	

	
	
	//$total_tickets_price = CartSession::getTotalTicketsPrice($order_id);
	//old order data
	$oldOrderTicketQty = $form_state ['values'] ['oldOrderTicketQty'];
	$oldOrderWindowsEnhancementQty = $form_state ['values'] ['oldOrderEnhancementQty'];

	
	
	
	
	
	try {

		//->1Deleting Old Order Data
		$order = uc_order_load($order_id);
	
		
		
		$oldOrderObject = new OrderEdit($order->order_id);
		$oldOrderObject->deleteOrderData();
		
		
		//dump($order); exit();

		//->2
		$currentOrderTicketQty = 0;
		$selectedtickets = array();
		$tickets = unserialize(CartSession::getTickets($order_id));
		$total_tickets_price = CartSession::getTotalTicketsPrice($order_id);

		foreach ($tickets as $ticket) {
			if ($ticket['qty'] > 0) {
				$selectedtickets[] = $ticket;
				$currentOrderTicketQty+=$ticket['qty'];
			}
		}
		
		

		//->3 Getting the Product ID of Event ID ( Event ID treated as a Product node)
		$scheduleEventObject = scheduledevents_load($event_id);
		$event_product_node = $scheduleEventObject->nid;
		$eventproductnodedata = node_load($event_product_node);

		$eventProduct = new stdClass();
		$eventProduct->nid = $event_product_node;
		$eventProduct->qty = 1; //By Default Cruise Item qty is treating as single
		$eventProduct->price = $total_tickets_price;
		$eventProduct->title = $eventproductnodedata->title;
		$eventProduct->model = $eventproductnodedata->model;
		$order->products[] = $eventProduct;
		
		
		


		//->4
		$selectedenhancements = array();
		$enhancements = unserialize(CartSession::getEnhancements($order_id));
		$totalenhancementQty = 0;
		$currentOrderWindowsEnhancementQty = 0;
		foreach ($enhancements as $enhancement) {
			$totalenhancementQty+=$enhancement['qty'];
			$selectedenhancements[] = $enhancement;
			$cartenhancednode = node_load($enhancement['enhancedproductnodeid']);
			$cartenhancedproduct = uc_product_load($cartenhancednode);
			$cartenhancedproduct->nid = $cartenhancednode->nid;
			$cartenhancedproduct->qty = $enhancement['qty']; //By Default Cruise Item qty is treating as single
			//$cartenhancedproduct->price=$enhancement['qty']* $enhancement['customprice'];
			$cartenhancedproduct->price = $enhancement['price'];
			$cartenhancedproduct->title = $cartenhancednode->title;
			$cartenhancedproduct->model = $cartenhancednode->model;
			$order->products[] = $cartenhancedproduct;

			if (CruiseEnhancements::is_window_inventory_item($enhancement['enhancedproductnodeid'])) {
				$currentOrderWindowsEnhancementQty+=$enhancement['qty'];
			}
		}
		
		
		
		//promocode
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
				
// 				$oldorderamt=0;
// 				if(isset($giftCardArray[$key]['giftcardoldamt'])){
// 					$oldorderamt=$giftCardArray[$key]['giftcardoldamt'];
// 				}
				$deductedamount = $giftCardArray[$key]['deductedamount'];
				$totalgiftcardprice = $giftCardArray[$key]['totalgiftcardprice'];
			}
			$order->line_items[] = uc_order_line_item_add($order->order_id, 'generic', 'Gift Card ' . $giftCardCode, -($deductedamount), _uc_line_item_data('giftcard', 'weight'), array('name' => 'GIFT_CARD')
			);
		}
		
	
		//->5 Fee Items
		$feeItems = CartSession::getCruiseFeeItems($order_id);


		foreach ($feeItems as $feeitem) {
			$feeitemname = $feeitem['title'];
			$totalAmount = $feeitem['totalamt'];
			if ($totalAmount != "") {
				//                 $order->line_items[] = uc_order_line_item_add(
				//                         $order->order_id, 'generic', $feeitemname, $totalAmount, _uc_line_item_data('cruise_fee_item', 'weight'), array('name' => $feeitemname)
				//                 );
				$order->line_items[] = uc_order_line_item_add(
						$order->order_id, 'generic', $feeitemname, $totalAmount, _uc_line_item_data('cruise_fee_item', 'weight'), array('name' => 'FEE_ITEMS')
				);
			}
		}

		$order_id = $order->order_id;
		 
		
		 

		 
		//->6 Tax
		$taxamt = CartSession::getTaxAmt($order_id);
		if ($taxamt != "") {
			//             $order->line_items[] = uc_order_line_item_add($order->order_id, 'generic', 'Tax ', ($taxamt), _uc_line_item_data('Tax', 'weight'), array('name' => 'Tax')
			//             );
			$order->line_items[] = uc_order_line_item_add($order->order_id, 'generic', 'Tax ', ($taxamt), _uc_line_item_data('Tax', 'weight'), array('name' => 'TAX')
			);
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

		$order->payment_method = 'credit';
		$order->payment_details['cc_number'] = "";
		$order->payment_details['cc_exp_month'] = ""; //'02';
		$order->payment_details['cc_exp_year'] = ""; //'15';
		$order->payment_details['cc_cvv'] =""; //'521';
		$order->payment_details['cc_type'] = ""; //'visa';//$_SESSION['payments']['cc_type'];
		
		
		
		
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
		



		 
		uc_order_save($order);

		try {
			db_update('uc_orders')
			->fields(
			array('order_status' => 'pending'))
			->condition('order_id', $order->order_id)
			->execute();
		} catch (Exception $e) {
			 
		}

		 


		//$orderTotal=CartSession::getOrderTotal($order_id);



		//Userchoices Log
		$userChoicesObj = new UserChoices();
		$userChoicesObj->order_total = $order->order_total;
		 
		 
		 
		 
		$reservationData = Reservation::getReservationData($order->order_id)->fetchAll();
		$id = $reservationData[0]->id;
		$bookingdate = $reservationData[0]->bookingdate;
		$eventid = $reservationData[0]->eventid;
		$orderid = $reservationData[0]->orderid;
		$uid = $reservationData[0]->uid;
		$bookedby = $reservationData[0]->bookedby;
		$created = $reservationData[0]->created;



		//-> Tickets Log
		foreach ($selectedtickets as $ticket) {
			$ticketObj = new Tickets();
			$ticketObj->ticket_id = $ticket['ticketid'];
			$ticketObj->order_id = $order->order_id;
			$ticketObj->ticket_qty = $ticket['qty'];
			$ticketObj->ticket_price = $ticket['price'];
			$ticketObj->event_id = $reservationData[0]->eventid;
			$ticketObj->bookingtype = "";
			$id = BookingLog::logTickets($ticketObj);
			for ($i = 0; $i < count($ticket['ticketentreeids']); $i++) {
				$ticketEntreesObj = new TicketEntrees();
				$ticketEntreesObj->entree_id = (int) $ticket['ticketentreeids'][$i];
				$ticketEntreesObj->ref_id = (int) $id;
				$commentArray = explode('-', $ticket['ticketentreescomments'][$i]);
				$ticketEntreesObj->comments = isset($commentArray[2]) ? $commentArray[2] : '';
				BookingLog::logTicketEntrees($ticketEntreesObj);
			}
		}


		//-> Enhancements Log
		foreach ($selectedenhancements as $enhancement) {
			$enhancementObj = new Enhancements();
			$enhancementObj->comments = "";
			$enhancementObj->enhancement_id = $enhancement['enhancedproductnodeid'];
			$enhancementObj->enhancement_price = $enhancement['price'];
			$enhancementObj->enhancement_qty = $enhancement['qty'];
			$enhancementObj->event_id = $reservationData[0]->eventid;
			$enhancementObj->order_id = $order->order_id;
			BookingLog::logEnhancements($enhancementObj);
		}

		$data = unserialize($reservationData[0]->data);
		//$cartsessionid = $data->userchoice['cartsessionid'];
		//$producttickets = $data->userchoice['cruiseproduct']['producttickets'];
		//$cruiseitem = $data->userchoice['cruiseproduct']['cruiseitem'];
		//$enhacementproducts = $data->userchoice['enhacementproducts'];
		$specialinstructions = $data->userchoice['specialinstructions'];
		$celebrationtext = $data->userchoice['celebrationtext'];
		$usersdata = $data->userchoice['usersdata'];

		//-> Updating the cart session
		$cartsesison = user_password(8) . REQUEST_TIME . user_password(8);
		$cart_updated = db_update('cartsession')
		->fields(array(
				'sessionid' => $cartsesison,
		))
		->condition('sessionid', $order->order_id, '=')
		->execute();

		$usercruiseinputs = array(
				'cartsessionid' => array($cartsesison),
				'cruiseproduct' => array(
						'producttickets' => $selectedtickets,
						'cruiseitem' => $cruise_id),
				'enhacementproducts' => $selectedenhancements,
				'specialinstructions' => $specialinstructions,
				'celebrationtext' => $celebrationtext,
				'usersdata' => $usersdata,
		);

		$userchioce = (object) array(
				'userchoice' => $usercruiseinputs
		);

		$reservationObj = new stdClass();
		//$reservationObj->bookingdate = REQUEST_TIME;
		//$reservationObj->bookingdate = strtotime($_SESSION['bookingInput']->date);
		$reservationObj->orderid = $order->order_id;
		$reservationObj->id = $r_id;
		$reservationObj->data = $userchioce;
		$savereservation = reservation_save($reservationObj);
		 
		 

		
		



		//inventory updation
		//Tickets

		if ($currentOrderTicketQty > $oldOrderTicketQty) {
			$totalTickets = $currentOrderTicketQty - $oldOrderTicketQty;

			$res = db_update('scheduledevents')
			->expression('soldtickets', 'soldtickets + :ticketssold', array(':ticketssold' => $totalTickets))
			->expression('availabletickets', 'ticketinventory - soldtickets', array())
			->condition('cruisetemplate_id', $cruise_id, '=')
			->condition('id', $event_id, '=')
			->execute();

			//var_dump('>');
		} else if ($currentOrderTicketQty < $oldOrderTicketQty) {
			$totalTickets = $oldOrderTicketQty - $currentOrderTicketQty;
			$res = db_update('scheduledevents')
			->expression('soldtickets', 'soldtickets - :ticketssold', array(':ticketssold' => ($totalTickets)))
			->expression('availabletickets', 'availabletickets + :tickets', array(':tickets' => ($totalTickets)))
			->condition('cruisetemplate_id', $cruise_id, '=')
			->condition('id', $event_id, '=')
			->execute();

			//var_dump('<');
		} else {

		}

		//Enhancements
		if ($currentOrderWindowsEnhancementQty > $oldOrderWindowsEnhancementQty) {
			$totalwindowEnhancements = $currentOrderWindowsEnhancementQty - $oldOrderWindowsEnhancementQty;

			//var_dump($totalwindowEnhancements); exit();

			$res = db_update('scheduledevents')
			->expression('windowseatssold', 'windowseatssold + :windowssold', array(':windowssold' => $totalwindowEnhancements))
			->expression('availablewindowseats', 'windowseatinventory - windowseatssold', array())
			->condition('cruisetemplate_id', $cruise_id, '=')
			->condition('id', $event_id, '=')
			->execute();
			//var_dump('en >');
		} else if ($currentOrderWindowsEnhancementQty < $oldOrderWindowsEnhancementQty) {
			$totalwindowEnhancements = $oldOrderWindowsEnhancementQty - $currentOrderWindowsEnhancementQty;

			// var_dump($totalwindowEnhancements); exit();
			$res = db_update('scheduledevents')
			->expression('windowseatssold', 'windowseatssold - :windowssold', array(':windowssold' => $totalwindowEnhancements))
			->expression('availablewindowseats', 'availablewindowseats + :enhancements', array(':enhancements' => $totalwindowEnhancements))
			->condition('cruisetemplate_id', $cruise_id, '=')
			->condition('id', $event_id, '=')
			->execute();

			//var_dump('en <');
		} else {

		}
	}catch(Exception $e){
		watchdog("Error in Deleting the order", $e);

	}


	 unset($_SESSION['bookingorderformflag']);
	//$_SESSION['default_tab'] = 'edit-payment';
	$path = 'staffadmin/order/' . $order_id . '/payment';
	drupal_goto($path); 

}





//Validating Promocode
function validatePromocodeInUpdateOrder($form, &$form_state) {


	//var_dump('test'); exit();
	$order_id = $form_state ['values'] ['hiddenorderid'];
	$tickettotalamt = (float) CartSession::getTotalTicketsPrice($order_id);
	$event_id = $form_state ['values'] ['hiddeneventid'];
	
	$couponamt = '0';
	$couponObj = false;
	$cpnstatus = 'This promo code is invalid or has expired.';
	$totalAmount = (float) CartSession::getOrderTotal($order_id);
	$commands = array();
	if ($tickettotalamt > 0 && $totalAmount>0) {
		$cpnObject=CartSession::getPromocode($order_id);
		$discountamt = 0;
		$couponcode = $form_state ['values'] ['promocode'];
		
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



//validating giftcard
function validateGiftcardInUpdateOrder($form, $form_state) {


	$commands = array();
	$order_id = $form_state ['values'] ['hiddenorderid'];
	$event_id = $form_state ['values'] ['hiddeneventid'];
	$cardNumber = $form_state ['values'] ['giftcardnumber'];  //2
	
	
	//Giftcard Old amt
	$giftcardoldamt = $form_state ['values'] ['giftcardoldamt'];
	
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

		//$commands [] = ajax_command_replace("#giftcard_status_message", "<div id='giftcard_status_message'>" . $giftcardstatus . "</div>");
		return array(
		'#type' => 'ajax',
		'#commands' => $commands
		);
}