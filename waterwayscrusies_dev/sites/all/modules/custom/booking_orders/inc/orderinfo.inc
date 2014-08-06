<?php

function orderinfo($order_id){
	$orderObject = uc_order_load($order_id);
    if ($orderObject) {
        return drupal_get_form("booking_order_information", $orderObject);
    }
    return "invalid operation";
}


function booking_order_information($form, &$form_state,$orderObject) {
	
	
	drupal_add_css(drupal_get_path('module', 'booking_orders') . '/css/booking_orders.css', array('group' => CSS_DEFAULT, 'type' => 'file'));
	$order_id = $orderObject->order_id;
	
	
	//Step 1: Removing the Order id if Exists in cartsession Table
	try {
		$num_deleted = db_delete('cartsession')
		->condition('sessionid', $order_id)
		->execute();
	} catch (Exception $e) {
	
	}
	
	$cart_reservation_data = Reservation::getCompleteReservationData($order_id);
	$itemsobj = unserialize($cart_reservation_data->data);
	$oldCartSessionid = $itemsobj->userchoice['cartsessionid'][0];
	
	

    //Step 2:
    $cart_reservation_data = Reservation::getCompleteReservationData($order_id);
    $itemsobj = unserialize($cart_reservation_data->data);
    

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
    
    
    
    //Session for Change Cruise functionality
    $oldOrderObject = (object) array(
        'oldOrderTicketQty' => $oldOrderTicketQty,
        'oldOrderEnhancementQty' => $oldOrderEnhancementQty,
        'oldOrderEventId'=>$r_eventid,
        'oldOrderCruiseTemplateId'=>$cruise_template_id,
    );

    $currentorderid="currentorderid".$orderObject->order_id;
  
    
    $form['orderheader']=array(
    		'#type'=>'markup',
    		'#markup'=>'<div class="row-fluid"><nav class="span12"><ul style="display:block;" class="breadcrumb">
			<li class="active"><h4>Order Info</h4></li> <span class="icon-chevron-right"></span>
			<li ><a href="'.base_path().'staffadmin/order/'.$order_id.'/updateorder">Cart</a></li> <span class="icon-chevron-right"></span>
			<li><a href="'.base_path().'staffadmin/order/'.$order_id.'/payment">Payment</a></li> <span class="icon-chevron-right"></span>
			<li><a href="'.base_path().'staffadmin/order/'.$order_id.'/history">History</a></li>
			</nav></div>',
    );
    
    
    

    //Get the cruise Data
    $cruiseObject = cruise_load($scheduledeventsObject->cruisetemplate_id);
    $cruisetitle = isset($cruiseObject->cruise_title) ? $cruiseObject->cruise_title : '';
  
    $form['orderinfo']['orderdetails'] = array(
        '#type' => 'fieldset',
        '#title' => '<span class="icon-info-sign"></span>&nbsp;&nbsp; '.t('Order Information'),
//         '#prefix' => '<div class="row-fluid"><div class="span6">',
//         '#suffix' => '</div></div>',
//         '#collapsible' => TRUE,
//         '#collapsed' => FALSE,
    );

    
    //$emptyrowfluid = '<div class="row-fluid"><div class="span12"></div></div>';


    
    $form['hiddenorderid'] = array(
    		'#id' => 'hiddenorderid',
    		'#type' => 'textfield',
    		'#prefix' => '<div style="display:none;">',
    		'#suffix' => '</div>',
    		'#default_value' => $order_id,
    );

    $form['orderinfo']['orderdetails']['ordernumber'] = array(
        '#type' => 'markup',
        '#markup' => '<b>'.$orderObject->order_id.'</b>',
        '#prefix' => '<div class="row-fluid"><div class="span6"><table><tr><td class="tdalignright">Order Number :</td><td>',
        '#suffix' => '</td></tr>',
    );


    $form['orderinfo']['orderdetails']['orderdate'] = array(
        '#type' => 'markup',
        '#markup' => date('m/d/Y', $orderObject->created),
    	 '#prefix' => '<tr><td class="tdalignright">Order Date :</td><td>',
        '#suffix' => '</td></tr>',
    );

    $form['orderinfo']['orderdetails']['eventtitle'] = array(
        '#type' => 'markup',
        '#markup' => $cruisetitle,
        '#prefix' => '<tr><td class="tdalignright">Cruise Title :</td><td>',
        '#suffix' => '</td></tr>',
    );
    $form['orderinfo']['orderdetails']['eventdate'] = array(
        '#type' => 'markup',
        '#markup' => date('m/d/Y', $eventsacheduleddate),
        '#prefix' => '<tr><td class="tdalignright"> Cruise Date :</td><td>',
        '#suffix' => '</td></tr>',
    );



    $form['orderinfo']['orderdetails']['eventport'] = array(
        '#type' => 'markup',
        '#markup' => $portData['portname'],
        '#prefix' => '<tr><td class="tdalignright">Port :</td><td>',
        '#suffix' => '</td></tr>',
    );


    $form['orderinfo']['orderdetails']['eventboarding'] = array(
        '#type' => 'markup',
        '#markup' => $cruiseborading,
        '#prefix' => '<div class="row-fluid"><div class="span12">',
        '#prefix' => '<tr><td class="tdalignright">Boarding Time :</td><td>',
        '#suffix' => '</td></tr>',
    );

    $form['orderinfo']['orderdetails']['eventdeparture'] = array(
        '#type' => 'markup',
        '#markup' => $cruisedeparturetime,
        '#prefix' => '<tr><td class="tdalignright">Departure Time :</td><td>',
        '#suffix' => '</td></tr>',
    );


    $form['orderinfo']['orderdetails']['eventreturn'] = array(
        '#type' => 'markup',
        '#markup' => $cruisereturntime,
        '#prefix' => '<tr><td class="tdalignright">Return Time :</td><td>',
        '#suffix' => '</td></tr>',
    );



    //Tickets
    $selectedTickets = unserialize(CartSession::getTickets($oldCartSessionid));
    $ticketQty = 0;
    foreach ($selectedTickets as $ticket) {
        if ($ticket['qty'] > 0) {
            $ticketQty+=$ticket['qty'];
        }
    }

    //Enhancements
    $selectedEnhancements = unserialize(CartSession::getEnhancements($oldCartSessionid));
    $enhancementsQty = 0;
    foreach ($selectedEnhancements as $enhancement) {
        if ($enhancement['qty'] > 0) {
            $enhancementsQty+=$enhancement['qty'];
        }
    }



    $form['orderinfo']['orderdetails']['ticketdetails'] = array(
        '#type' => 'markup',
        '#markup' => $ticketQty,
        '#prefix' => '<tr><td>&nbsp;&nbsp;</td></tr><tr><td class="tdalignright">Total Tickets :</td><td>',
        '#suffix' => '</td></tr>',
    );


    $form['orderinfo']['orderdetails']['enhancementdetails'] = array(
        '#type' => 'markup',
        '#markup' => $enhancementsQty,
        '#prefix' => '<tr><td class="tdalignright">Total Enhancements :</td><td>',
        '#suffix' => '</td></tr> </table></div>',
    );

    $userChoiceObject = BookingLog::getUserChoices($order_id);
    $form['orderinfo']['orderdetails']['specialconcerns'] = array(
        '#type' => 'markup',
        '#prefix' => '<div class="span6"><table><tr><td colspan="2"><b>Special Concerns</b></td>',
        '#suffix' => '</td></tr>',
    );

    $l_flag = ($userChoiceObject->limited_mobility_flag == 1) ? array('checked' => 'checked') : array();
    $form['orderinfo']['orderdetails']['limitedmobility'] = array(
        '#type' => 'checkbox',
        '#title' => t('Limited Mobility (wheelchair, cane, etc.)'),
        '#prefix' => '<tr><td>',
        '#suffix' => '</td></tr>',
        '#attributes' => $l_flag
    );

    $f_flag = ($userChoiceObject->food_allergy_flag == 1) ? array('checked' => 'checked') : array();
    $form['orderinfo']['orderdetails']['foodallergy'] = array(
        '#type' => 'checkbox',
        '#title' => t('Food Allergy (seafood, peanuts, etc)'),
        '#prefix' => '<tr><td>',
        '#suffix' => '</td></tr>',
        '#attributes' => $f_flag
    );

    $a_flag = ($userChoiceObject->another_party_flag == 1) ? array('checked' => 'checked') : array();
    $form['orderinfo']['orderdetails']['anotherparty'] = array(
        '#type' => 'checkbox',
        '#prefix' => '<tr><td>',
        '#suffix' => '</td></tr>',
        '#title' => t('Would like to be seated with another party (specify below)'),
        '#attributes' => $a_flag
    );



    $form['orderinfo']['orderdetails']['specialinstructionscomments'] = array(
        '#type' => 'textarea',
        '#rows' => 3,
        '#default_value' => $userChoiceObject->another_party_comments,
        '#prefix' => '<tr><td>',
        '#suffix' => '</td></tr>',
        '#title' => 'Special Instruction Comments'
    );





    $c_flag = ($userChoiceObject->celebration_flag == 1) ? array('checked' => 'checked') : array();
    $form['orderinfo']['orderdetails']['ocassiontypeenabled'] = array(
        '#type' => 'checkbox',
        '#prefix' => '<tr><td>',
        '#suffix' => '</td></tr>',
        '#title' => t('Celebration (Birthday, Anniversary, Special Occasion, etc.)'),
        '#attributes' => $c_flag
    );



    $form['orderinfo']['orderdetails']['celebrationscomments'] = array(
        '#type' => 'textarea',
        '#rows' => 3,
        '#prefix' => '<tr><td>',
        '#suffix' => '</td></tr></table></div>',
        '#default_value' => $userChoiceObject->celebration_comments,
        '#title' => 'Celebration Comments'
    );




   

    $form['orderinfo']['guest'] = array(
        '#type' => 'fieldset',
        '#title' => '<span class="icon-user"></span>&nbsp;&nbsp; '.t('Guest Information'),
        '#collapsible' => FALSE,
        '#collapsed' => FALSE,
    	'#prefix' => '<div class="row-fluid"><div class="span6">',
    	'#suffix' => '</div>',
    );
    $form['orderinfo']['guest']['guest_firstname'] = array(
        '#type' => 'textfield',
        '#prefix' => '<table><tr><td class="tdalignright">First Name</td><td>',
        '#suffix' => '</td></tr>',
        '#default_value' => $orderObject->delivery_first_name,
    );

    $form['orderinfo']['guest']['guest_lastname'] = array(
        '#type' => 'textfield',
        '#prefix' => '<tr><td class="tdalignright">Last Name</td><td>',
        '#suffix' => '</td></tr>',
        '#default_value' => $orderObject->delivery_last_name,
    );

    $form['orderinfo']['guest']['guest_address'] = array(
        '#type' => 'textfield',
        '#prefix' => '<tr><td class="tdalignright">Address</td><td>',
        '#suffix' => '</td></tr>',
        '#default_value' => $orderObject->delivery_street1,
    );

    $form['orderinfo']['guest']['guest_city'] = array(
        '#type' => 'textfield',
       '#prefix' => '<tr><td class="tdalignright">City</td><td>',
        '#suffix' => '</td></tr>',
        
        '#default_value' => $orderObject->delivery_city,
    );



    $usStatesObject = uc_zone_select();
    $usStatesList = $usStatesObject['#options'];
    $form['orderinfo']['guest']['guest_state'] = array(
        '#type' => 'select',
        '#prefix' => '<tr><td class="tdalignright">State</td><td>',
        '#suffix' => '</td></tr>',
        '#options' => $usStatesList,
        '#default_value' => $orderObject->delivery_zone,
    );



    $form['orderinfo']['guest']['guest_phone'] = array(
        '#type' => 'textfield',
       '#prefix' => '<tr><td class="tdalignright">Phone</td><td>',
        '#suffix' => '</td></tr>',
        '#default_value' => $orderObject->delivery_phone,
    );


    $form['orderinfo']['guest']['guest_zipcode'] = array(
        '#type' => 'textfield',
        '#prefix' => '<tr><td class="tdalignright">Zip code</td><td>',
        '#suffix' => '</td></tr>',
        '#default_value' => $orderObject->delivery_postal_code,
    );
    
    
    $guestmail='';
    $account=user_load($orderObject->uid);
    $guestmail=isset($account->mail) ? $account->mail : '';

    $form['orderinfo']['guest']['guest_email'] = array(
        '#type' => 'textfield',
       '#prefix' => '<tr><td class="tdalignright"> Email</td><td>',
        '#suffix' => '</td></tr>',
        '#default_value' => $guestmail,
    );

    $referal = array(
    	'empty'=>'',
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
    );


    $defaultReferalValue = !empty($userChoiceObject->referal) ? $userChoiceObject->referal : 'empty';
    $form['orderinfo']['guest']['referal'] = array(
        '#type' => 'select',
        '#options' => $referal,
        '#group' => 'orderedit',
        '#default_value' => $defaultReferalValue,
        '#prefix' => '<tr><td class="tdalignright">Referral</td><td>',
        '#suffix' => '</td></tr>',
      
    );



    $e_flag = ($userChoiceObject->receive_email_notification_flag == 1) ? array('checked' => 'checked') : array();
    $form['orderinfo']['guest']['emailpromotions'] = array(
        '#type' => 'checkbox',
        '#title' => t('Yes <i>-if checked</i>'),
        '#attributes' => $e_flag,
        '#group' => 'orderedit',
        '#prefix' => '<tr><td class="tdalignright">Email Promotions</td><td>',
        '#suffix' => '</td></tr><tr><td>&nbsp;&nbsp;</td></tr><tr>',
    );


    
    $form ['orderinfo']['guest']['resendemail'] = array(
    		'#type' => 'submit',
    		'#value' => 'Resend Email',
    		'#prefix' => '<td class="pull-right">',
    		'#suffix' => '</td>',
    		'#ajax' => array(
    				'callback' => 'resendOrderInvoiceEmailToGuest',
    				'effect' => 'fade'
    		),
    );
    
    

    $form['orderinfo']['guest']['updateuserdetails'] = array(
    		'#type' => 'submit',
    		'#value' => 'Update Guest Details',
    		'#prefix' => '<td class="">',
    		'#suffix' => '</td>',
    		'#ajax' => array(
    				'callback' => 'updateExistingUserDetails',
    				'effect' => 'fade'
    		)
    );
    
    
   
    
    $form['orderinfo']['guest']['end'] = array(
    		'#type' => 'markup',
    		'#markup'=>'</tr></table>'
    );
    
 
    
    $form['orderinfo']['purchase'] = array(
    		'#type' => 'fieldset',
    		 '#title' =>'<span class="icon-calendar"></span>&nbsp;&nbsp; '. t('Purchase On - ') . date('m/d/Y', $orderObject->created),
        '#collapsible' => FALSE,
        '#collapsed' => FALSE,
    		'#prefix' => '<div class="span6">',
    		'#suffix' => '</div>',
    );
    
    
    $form['orderinfo']['purchase']['start'] = array(
    		'#type' => 'markup',
    		'#markup'=>'<table>'
    );
    
    
    
    $ticketData = unserialize(CartSession::getTickets($oldCartSessionid));
    for ($i = 0; $i < count($ticketData); $i++) {
        for ($j = 0; $j < $ticketData[$i]['qty']; $j++) {
            $form['orderinfo']['purchase']['ticketinfo' . $i . $j] = array(
                '#type' => 'markup',
                '#markup' => '<tr><td class="tdalignright">'.' 1- ' . $ticketData[$i]['ticketname'] .' :</td><td>'.'$' . sprintf("%.2f", $ticketData[$i]['price']).'</td></tr>',
            );


            $entreeid = $ticketData[$i]['ticketentreeids'][$j];
            $entreeName = CruiseEntrees::getCruiseEntreeNamebyId($entreeid);

            if ($entreeName == "") {
                $entreeName = "no options";
            }

            $form['orderinfo']['purchase']['ticketentreeinfo' . $i . $j] = array(
                '#type' => 'markup',
             	'#markup' => '<tr><td class="" style="text-align:right; margin:0px !important;" ><i>Entree :</i></td><td >'.$entreeName.'</td></tr>',
            );
        }
    }
    
    
    $form['orderinfo']['purchase']['enhancementsblankspace'] = array(
    		'#type' => 'markup',
    		'#markup' => '<tr><td>&nbsp;&nbsp;</td></tr>',
    );
    
    
  
    
    
    

    
    
    

    $enhancementserializeddata = unserialize(CartSession::getEnhancements($oldCartSessionid));
    $enhancementhtml='';
    if ($enhancementserializeddata) {
        $enhancmentsitems = unserialize(CartSession::getEnhancements($oldCartSessionid));
        foreach ($enhancmentsitems as $enhancement) {
            $amount = $enhancement['qty'] * $enhancement['price'];
            $enhancementhtml.='';
            $enhancementhtml.='<tr><td class="tdalignright">' . $enhancement['qty'] . '-' . $enhancement['name'] . ' :</td><td>$' . wwcFormatPrice($amount) . '</td></tr>';
        }

        $form['orderinfo']['purchase']['enhancementsinfo' . $enhancement['enhancedproductnodeid']] = array(
            '#type' => 'markup',
            '#markup' => $enhancementhtml,
        );
    }

    //tax line item
    $taxLineItemSerialized = CartSession::getTaxLineItem($oldCartSessionid);
    $taxLineItemArray = reset(unserialize($taxLineItemSerialized));
    if($taxLineItemArray['amt']!=0){
    	$taxlinetitemhtml = '<tr><td class="tdalignright">'.$taxLineItemArray['taxname'].':</td><td>$' . wwcFormatPrice($taxLineItemArray['amt']) . '</td></tr>';
    	$form['orderinfo']['purchase']['additionaltaxlineitem'] = array(
    			'#type' => 'markup',
    			'#markup' => $taxlinetitemhtml,
    	);
    }
   
    
    //discount line item
    $discountLineItemSerialized = CartSession::getDiscountLineItem($oldCartSessionid);
    $discountLineItemArray = reset(unserialize($discountLineItemSerialized));
    if($discountLineItemArray['amt']!=0){
    	$discountlinetitemhtml = '<tr><td class="tdalignright">'.$discountLineItemArray['discountname'].':</td><td> -$' . wwcFormatPrice($discountLineItemArray['amt']) . '</td></tr>';
    	$form['orderinfo']['purchase']['discountlineitem'] = array(
    			'#type' => 'markup',
    			'#markup' => $discountlinetitemhtml,
    	);
    }
   
    
    $form['orderinfo']['purchase']['feeitemsspace'] = array(
    		'#type' => 'markup',
    		'#markup' => '<tr><td>&nbsp;&nbsp;</td></tr>',
    );
    //fee items
    $cruiseFeeItems = CartSession::getCruiseFeeItems($oldCartSessionid);
    $feeitemcount = 0;
    $cruisefeehtml='';
    foreach ($cruiseFeeItems as $item) {


        $chargeamt = $item['totalamt'];
        if ($feeitemcount == 0) {
            $cruisefeehtml.='<tr><td class="tdalignright">' . $item['title'] . ':</td><td>$' . wwcFormatPrice($chargeamt) . '</td></tr>';
        } else {
            $cruisefeehtml.='<tr><td class="tdalignright">' . $item['title'] . ':</td><td>$' . wwcFormatPrice($chargeamt) . '</td></tr>';
        }

        $form['orderinfo']['purchase']['cruisefees' . $enhancement['enhancedproductnodeid']] = array(
            '#type' => 'markup',
            '#markup' => '<div style="margin-top:30px;">' . $cruisefeehtml . '</div>',
        );
        $feeitemcount++;
    }
		
	// promocodes
	$cpnarray = unserialize ( CartSession::getPromocode ( $oldCartSessionid ) );
	$cpncode = false;
	$cpndiscount = false;
	foreach ( $cpnarray as $key => $value ) {
		$cpncode = $key;
		$cpndiscount = $value;
	}
	
	if ($cpndiscount) {
		$cpndiscounthtml = '<tr><td class="tdalignright">Promocode ' . $cpncode . ':</td><td>-$' . wwcFormatPrice ( $cpndiscount ) . '</td></tr>';
		$form ['orderinfo'] ['purchase'] ['coupondiscount'] = array (
				'#type' => 'markup',
				'#markup' => $cpndiscounthtml 
		);
	}
    
	
	//gift card
	$giftCardSerialized = CartSession::getGiftCard($oldCartSessionid);
	if ($giftCardSerialized) {
		$giftCardArray = reset(unserialize($giftCardSerialized));
		$giftcardhtml = '<tr><td class="tdalignright">Gift Card :</td><td>-$' . wwcFormatPrice ( $giftCardArray['deductedamount'] ) . '</td></tr>';
		$form ['orderinfo'] ['purchase'] ['giftcard'] = array (
				'#type' => 'markup',
				'#markup' => $giftcardhtml
		);
	}
	
	
	//voucher codes
	$vouchercodesserialized=CartSession::getVoucherCodes($oldCartSessionid);
	
	//drupal_json_output($vouchercodesserialized); exit();
	if($vouchercodesserialized){
		$vouchers=unserialize($vouchercodesserialized);
		
		foreach($vouchers as $key=>$value){
			$vouchershtml = '<tr><td class="tdalignright">Voucher - '.$key.' :</td><td>-$' . wwcFormatPrice ( $value ) . '</td></tr>';
			$form ['orderinfo'] ['purchase'] ['voucher'.$key] = array (
					'#type' => 'markup',
					'#markup' => $vouchershtml
			);
			
		}
		
	}
	
	
    	
    	
    	
    

    
    





    $orderSubTotal = CartSession::getOrderSubTotal($oldCartSessionid);
    $subtotalhtml = '<tr><td>&nbsp;</td></tr><tr><td class="tdalignright">Subtotal:</td><td>$' . wwcFormatPrice($orderSubTotal) . '</td></tr>';
    $form['orderinfo']['purchase']['subtotal'] = array(
        '#type' => 'markup',
        '#markup' => $subtotalhtml,
    );


    $taxAmt = CartSession::getTaxAmt($oldCartSessionid);
    $taxhtml = '<tr><td class="tdalignright">Tax:</td><td>$' . wwcFormatPrice($taxAmt) . '</td></tr>';
    $form['orderinfo']['purchase']['tax'] = array(
        '#type' => 'markup',
        '#markup' => $taxhtml,
    );




    $orderTotal = CartSession::getOrderTotal($oldCartSessionid);
    $ordertotalhtml = '<tr><td class="tdalignright"><b>Total</b>:</td><td><b>$' . wwcFormatPrice($orderTotal) . '</b></td></tr>';
    $form['orderinfo']['purchase']['ordertotal'] = array(
        '#type' => 'markup',
        '#markup' => $ordertotalhtml,
    );





    $form['orderinfo']['purchase']['end'] = array(
    		'#type' => 'markup',
    		'#markup'=>'</table>'
    );


	
	return $form;
	
	
}


function updateExistingUserDetails($form, $form_state) {
	
	
	
	$order_id = $form_state['values']['hiddenorderid'];
	
	
	$guest_firstname = $form_state['values']['guest_firstname'];
	$guest_lastname = $form_state['values']['guest_lastname'];
	$guest_address = $form_state['values']['guest_address'];
	$guest_city = $form_state['values']['guest_city'];
	$guest_state = $form_state['values']['guest_state'];
	$guest_phone = $form_state['values']['guest_phone'];
	$guest_zipcode = $form_state['values']['guest_zipcode'];
	$guest_email = $form_state['values']['guest_email'];


	 
	$order=  uc_order_load($order_id);
	// load user object
	$existingUser = user_load($order->uid);

	// update some user property
	$existingUser->mail = $guest_email;
	// save existing user
	user_save((object) array('uid' => $existingUser->uid), (array) $existingUser);


	$limited_mobility_flag = $form_state['values']['limitedmobility'];
	$food_allergy_flag = $form_state['values']['foodallergy'];
	$another_party_flag = $form_state['values']['anotherparty'];
	$another_party_comments = $form_state['values']['specialinstructionscomments'];
	$celebration_flag = $form_state['values']['ocassiontypeenabled'];
	$celebration_comments = $form_state['values']['celebrationscomments'];
	$receive_email_notification_flag = $form_state['values']['emailpromotions'];
	$referal = $form_state['values']['referal'];


	//var_dump($referal); exit();



	$userChoicesObject = new stdClass();

	$userChoicesObject->limited_mobility_flag = $limited_mobility_flag;
	$userChoicesObject->food_allergy_flag = $food_allergy_flag;
	$userChoicesObject->another_party_flag = $another_party_flag;
	$userChoicesObject->another_party_comments = $another_party_comments;
	$userChoicesObject->celebration_flag = $celebration_flag;
	$userChoicesObject->celebration_comments = $celebration_comments;
	$userChoicesObject->receive_email_notification_flag = $receive_email_notification_flag;
	$userChoicesObject->order_id = $order_id;
	$userChoicesObject->customer_email = $guest_email;
	$userChoicesObject->referal = $referal;
        
	$numuserchoiceUpdate = BookingLog::updateUserChoices($userChoicesObject);



	try {
		$num_updated = db_update('uc_orders')
		->fields(array(
				'delivery_first_name' => $guest_firstname,
				'delivery_last_name' => $guest_lastname,
				'delivery_phone' => $guest_phone,
				'delivery_street1' => $guest_address,
				'delivery_city' => $guest_city,
				'delivery_zone' => $guest_state,
				'delivery_postal_code' => $guest_zipcode,
				//'primary_email' => $guest_email,
		))
		->condition('order_id', $order_id, '=')
		->execute();
	} catch (Exception $e) {
		watchdog("orderedit", $e);
	}


	//Updating Guest Email


	if (is_null($num_updated) || is_null($numuserchoiceUpdate)) {
		$msg = "Operation failed";
	} else if ($num_updated > 0 || $numuserchoiceUpdate > 0) {
		$msg = "Guest information updated successfully";
	} else if ($num_updated == 0 && $numuserchoiceUpdate == 0) {
		$msg = "No changes detected";
	}


	// var_dump($num_updated); exit();

	$commands[] = ajax_command_alert($msg);

	return array(
			'#type' => 'ajax',
			'#commands' => $commands
	);
}


function resendOrderInvoiceEmailToGuest($form, $form_state) {
	$commands = array();
	try {
		$order_id = $form_state ['values'] ['hiddenorderid'];
		$order = uc_order_load($order_id);
		$params = array(
				'subject' => t('Your Order at WATERWAYS'),
				'body' => t("Data"),
				'orderId' => $order->order_id
		);
		$to = $order->primary_email;
                $from=SiteConfiguration::getConfirmationEmail();
                
		$res = drupal_mail("email_system", "wwc_order_invoice_template", $to, language_default(), $params, $from);
		$commands [] = ajax_command_alert("Message sent successfully");
	} catch (Exception $message) {
		watchdog("Order Edit", $message);
	}

	return array(
			'#type' => 'ajax',
			'#commands' => $commands
	);
}

