<?php

function booking_form_submit($form, &$form_state) {
	
	$bookingCart=new BookingCart();
	$trigValue=$form_state['triggering_element']['#value'];
	$trigName=$form_state['triggering_element']['#name'];
	$trigNamArr=explode('_', $trigName);
	
	if(count($trigNamArr)>1){
		$trigName=$trigNamArr[0];
		$voucherTxtboxid=$trigNamArr[1];
	}
	
	//********************** Gift Card ***************************
	if($trigName=="applygiftcard" && $trigValue=="Apply"){
		$giftcard=trim($form_state['input']['giftcard']);
		$giftCardMessage="";
		if(empty($giftcard)){
			$giftCardMessage="Please Enter Valid Gift Card Number!";
		}else{
			$giftCardMessage=applyGiftCard($giftcard);
		}
		drupal_set_message($giftCardMessage);
		$form_state['new_stage'] = "PAYMENT";
	}else if($trigName=="removegiftcard" && $trigValue=="Remove"){
		$bookingCart->removeGiftCard();
		$giftCardMessage='Removed Successfully!';
		drupal_set_message($giftCardMessage);
		$form_state['new_stage'] = "PAYMENT";
	}
	
	
	
	//********************** Groupons ***************************
	else if($trigName=="voucher" && $trigValue=="Apply"){
		$voucherMessages='';
		$voucherCode=$form_state['input']['voucher'.$voucherTxtboxid];
		if($voucherCode!=""){
			$voucherexistresult=$bookingCart->checkGroupon($voucherCode);
			if($voucherexistresult){
				$voucherMessages="Groupon Code already redeemed";
			}else{
				$dealPriceAmount=applyGrouponCode($voucherCode);
				if($dealPriceAmount){
					$voucherMessages="Groupon Code successfully applied!";
					$voucher = array('code'=>$voucherCode ,'amount'=> $dealPriceAmount);
					$bookingCart->addVoucher($voucher, $voucherTxtboxid);
				}else{
					$voucherMessages="Invalid Groupon Code or already redeemed!";
				}
			}
			}
		drupal_set_message($voucherMessages);
		$form_state['new_stage'] = "PAYMENT";
	}
	else if($trigName=="voucher" && $trigValue=="Remove"){
		$bookingCart->removeVoucher($voucherTxtboxid);
		$voucherMessages='Removed Successfully!';
		drupal_set_message($voucherMessages);
		$form_state['new_stage'] = "PAYMENT";
	}
	
	//********************** Promo Code ***************************
	else if($trigName=="applypromocode" && $trigValue=="Apply"){
		$promoCode=$form_state['input']['prmocode'];
		applyPromoCode($promoCode);
		$form_state['new_stage'] = "PAYMENT";
	}else if($form_state['triggering_element']['#name']=="removepromocode" && $form_state['triggering_element']['#value']=="Remove"){
	 $bookingCart->removePromoCode();
	 drupal_set_message('Removed Successfully!');
	 $form_state['new_stage'] = "PAYMENT";
	}
	
	
	
	else{
		switch ($form_state['stage']) {
			case 'TICKETS':
				$bookingCart->getCartCruiseTemplateId();
				$bookingCart->addTickets($form_state['values']);
				$form_state['multistep_values'][$form_state['stage']] = $form_state['values'];
				$form_state['new_stage'] = booking_move_to_next_stage($form, $form_state);
				break;
		
		
			case 'ENHANCEMENTS':
				$bookingCart->addEnhancements($form_state['values']);
				$bookingCart->resetCouponNGrouponCodes();
				$form_state['multistep_values'][$form_state['stage']] = $form_state['values'];
				$form_state['new_stage'] = booking_move_to_next_stage($form, $form_state);
				break;
		
			case 'PAYMENT':
				$bookingCart->addUserInfo($form_state['values']);
				$form_state['multistep_values'][$form_state['stage']] = $form_state['values'];
				$form_state['new_stage'] = booking_move_to_next_stage($form, $form_state);
				break;
		
			case 'REVIEW_ORDER':
				$form_state['multistep_values'][$form_state['stage']] = $form_state['values'];
				if ($form_state['triggering_element']['#value'] != 'Go Back') {
					$cartid=$bookingCart->cartId();
					$form_state['paymentstatus'] = "";
					$form_state['booking_orderid'] = "";
					//$form_state['paymentstatus']="incomplete";
					booking_review_form_submit($form, $form_state);
					if ($form_state['paymentstatus'] == "success") {
						$orderpath = 'confirmation/orderid/' . $form_state['booking_orderid'] . '/' . $cartid;
						
						//var_dump($orderpath); exit();
						drupal_goto($orderpath);
						$form_state['complete'] = TRUE;
					} else {
						//                    drupal_set_message('Invalid Card Details');
						$form_state['new_stage'] = "PAYMENT";
					}
				}
				break;
		
			default:
				//$form_state['multistep_values'][$form_state['stage']] = $form_state['values'];
				//$form_state['new_stage'] = booking_move_to_next_stage($form, $form_state);
				//break;
		}
		
		if (isset($form_state['complete']))
			drupal_goto('confirmation');
		
		if ($form_state['triggering_element']['#value'] == 'Go Back To Tickets') {
			$form_state['new_stage'] = 'TICKETS';
		}
		
		if ($form_state['triggering_element']['#value'] == 'EDITORDER') {
		
			$form_state['new_stage'] = 'TICKETS';
		}
		
		if ($form_state['triggering_element']['#value'] == 'Go Back') {
		
			$form_state['new_stage'] = booking_move_to_previous_stage($form, $form_state);
		}
		
		if (isset($form_state['multistep_values']['form_build_id'])) {
			$form_state['values']['form_build_id'] = $form_state['multistep_values']['form_build_id'];
		}
		
	}
	
	
   // $form_state['multistep_values']['form_build_id'] = $form_state['values']['form_build_id'];
    $form_state['stage'] = $form_state['new_stage'];
    $form_state['rebuild'] = TRUE;
}





function booking_review_form_submit($form, &$form_state) {
    //After the Order Review Form SubmitImplementing the Payment Process
    $bookinginputs = $form_state['multistep_values'];
    $bookingresults = booking_procees($bookinginputs);
    
   // var_dump($bookingresults); exit();
    if ($bookingresults['bookingresult']) {
        $form_state['paymentstatus'] = "success";
        $form_state['booking_orderid'] = $bookingresults['currentorder'];
        //$form_state['paymentstatus']="complete";
        //Now the Update the staust of Booking
        //Redirecting to the booking complete page
       // watchdog('bookingstatus', $bookingresults['currentorder']);
       // watchdog('bookingstatus', $_SESSION['cruisecardsession']);
        //$orderpath = 'confirmation/orderid/' . $bookingresults['currentorder'] . '/' . $_SESSION['cruisecardsession'];
        //drupal_goto($orderpath);
    }
    //End of handling of payments
    else {
        $form_state['paymentstatus'] = "failed";
        //$form_state['paymentstatus']="incomplete";
        watchdog('bookingstatus', 'Invalid Card Details');
        //drupal_set_message('Invalid Card Details');
    }
}




/*
 * Final Booking Form Submission
 */

function booking_procees($bookinginputs) {
	
	//var_dump($bookinginputs); exit();
	
	
	$bookingCart= new BookingCart();
	$eventId=$bookingCart->getCartEventId();
	$eventDate=$bookingCart->getCartEventDate();
	$cruiseTemplateId=$bookingCart->getCartCruiseTemplateId();
	$cartId=$bookingCart->cartId();
	
	$cartUserData=$bookingCart->getUserInfo();
	$cartUser=$cartUserData['info'];
	
	
	$occasiontype=isset($cartUserData['occasiontype'])  ? $cartUserData['occasiontype'] : 0;
	$occasionadditionalcomments=isset($cartUserData['occasionadditionalcomments']) ? $cartUserData['occasionadditionalcomments'] : '';
	$specialinstructions=isset($cartUserData['specialinstructions']) ? $cartUserData['specialinstructions'] : array();
	$additionalcomments=isset($cartUserData['additionalcomments']) ? $cartUserData['additionalcomments'] : '';
	
	
	
	
	
	

	
	$total_tickets_price= (double) $bookingCart->getTicketsTotalAmount();
	$total_enhancements_price= (double) $bookingCart->getEnhancementsTotalAmount();
	$orderTotalAmount =(double) $bookingCart->getOrderTotal();
	
	
	
	
	
	$paymentDetails=$bookinginputs['PAYMENT'];
	$creditCardType=$paymentDetails['creditCardType'];
	$creditCardNumber=$paymentDetails['creditCardNumber'];
	$creditExpMonth=$paymentDetails['creditExpMonth'];
	$creditExpYear=$paymentDetails['creditExpYear'];
	$creditcid=$paymentDetails['creditcid'];
	

	$ticketsList=$bookingCart->getTicketsList();
	$total_ticketQty=count($ticketsList);
   
    
    
    //******************  User Acount ****************************
	//var_dump($cartUser); exit();
    //$newUserObject->mail = $cartUser['guest_email'];
    $userObject = user_load_by_mail($cartUser['guest_email']);
    
    
    if ($userObject) {
        $userId = $userObject->uid;
    } else {
        $userObject = new UserAccount ();
        $username= $cartUser['guest_firstname'] . '' . $cartUser['guest_lastname'] . time();
        $userObject->user_name = str_replace(' ', '', $username);
        $userObject->password = user_password(8);
        $userObject->firstname = $cartUser['guest_firstname'];
        $userObject->last_name = $cartUser['guest_lastname'];
        $userObject->mail = $cartUser['guest_email'];
        $userObject->role_id = 7;
        $guestPhone=$cartUser['guest_areacode']."-".$cartUser['guest_prefix']."-".$cartUser['guest_linenumber'];
        $userObject->phone = $guestPhone;
        $userObject->address = $cartUser['billing_address'] . " " . $cartUser['billing_city'] . " " . $cartUser['billing_zipcode'];
       
        $userId = $userObject->createNewUser();
    }
    //****************************************************************
    
   
   
    
    
    //1 Ubercart order
    $order_id=$bookingCart->getUberCartOrderId();
    if(empty($order_id)){
    	$order = uc_order_new($userId, 'in_checkout'); //Creating the new Order
    	$bookingCart->setUberCartOrderId($order->order_id);
    }else{
    	$order=uc_order_load($order_id);
    	unset($order->products);
    	unset($order->line_items);
    	$order->order_total=0;
    }
    
    
    
    
    //Getting the Product ID of Event ID ( Event ID treated as a Product node)
    $scheduleEventObject = scheduledevents_load($eventId);
    $event_product_node = $scheduleEventObject->nid;
    $eventproductnodedata = node_load($event_product_node);

    
    //2 Adding Products
    $eventProduct = new stdClass();
    $eventProduct->nid = $event_product_node;
    $eventProduct->qty = 1;
    $eventProduct->price = $total_tickets_price;
    $eventProduct->title = $eventproductnodedata->title;
    $eventProduct->model = $eventproductnodedata->model;
    $order->products[] = $eventProduct;
   

    
    //Enhancements
    $totalenhancementQty = 0;
    $selectedenhancements = array();
    $enhancements = $bookingCart->getEnhancementsList();
    foreach ($enhancements as $enhancement) {
        $totalenhancementQty+=$enhancement['qty'];
        $selectedenhancements[] = $enhancement;
        $cartenhancednode = node_load($enhancement['enhancedproductnodeid']);
        $cartenhancedproduct = uc_product_load($cartenhancednode);
        $cartenhancedproduct->nid = $cartenhancednode->nid;
        $cartenhancedproduct->qty = $enhancement['qty']; 
        $cartenhancedproduct->price = $enhancement['price'];
        $cartenhancedproduct->title = $cartenhancednode->title;
        $cartenhancedproduct->model = $cartenhancednode->model;
        $order->products[] = $cartenhancedproduct;
    }


    
    //***********************  Line Items ***********************  
   
    //Line Items :(1) Fee Items
    $feeItems = $bookingCart->getTicketsFeeItemsList();
    foreach ($feeItems as $feeitem) {
        $feeitemname = $feeitem['title'];
        $totalAmount = (double) $feeitem['totalamt'];
        $order->line_items[] = uc_order_line_item_add(
                $order->order_id, 'generic', $feeitemname, $totalAmount, _uc_line_item_data('cruise_fee_item', 'weight'), array('name' => 'FEE_ITEMS')
        );
    }
    
    
    
    //Line Items :(2) Fee Items
	$giftcardList=$bookingCart->getGiftCard();
	if(isset($giftcardList) && count($giftcardList)>0){
		$giftCardCode = '';
		$giftCardAmount = 0;
		$totalgiftcardprice = 0;
		$deductedamount = 0;
		foreach ($giftcardList as $key => $values) {
		     $giftCardCode = $key;
		     $deductedamount = (double) $giftcardList[$key]['deductedamount'];
		     $totalgiftcardprice =(double) $giftcardList[$key]['totalgiftcardprice'];
		 }
        $order->line_items[] = uc_order_line_item_add($order->order_id, 'generic', 'Gift Card ' . $giftCardCode, -($deductedamount), _uc_line_item_data('giftcard', 'weight'), array('name' => 'GIFT_CARD'));
	}
	
	//Line Items :(3) PromoCode
	$couponLineItem = $bookingCart->getPromoCode();
	  if(@is_array($couponLineItem)){
	  	foreach ($couponLineItem as $key => $val) {
	  		$cpncode = $key;
	  		$cpndiscount = (double) $val;
	  		$order->line_items[] = uc_order_line_item_add($order->order_id, 'generic', 'Promo Code ' . $cpncode, -($cpndiscount), _uc_line_item_data('coupon', 'weight'), array('name' => 'PROMO_CODE')
	  		);
	   	}
	 }
	 
    //Tax
    $taxamt = $bookingCart->getTaxAmount();
    if ($taxamt != "") {
        $order->line_items[] = uc_order_line_item_add($order->order_id, 'generic', 'Tax ', ($taxamt), _uc_line_item_data('Tax', 'weight'), array('name' => 'TAX')
        );
    }
    
    
    
    //Note :  Billing Information(billing Details)
    $order->billing_first_name = $cartUser['billing_firstname'];
    $order->billing_last_name = $cartUser['billing_lastname'];
    $order->billing_company = null;
    $order->billing_street1 = $cartUser['billing_address'];
    $order->billing_street2 = $bookinginputs['PAYMENT']['billing_otheraddress'];
    $order->billing_postal_code = $cartUser['billing_zipcode'];
    $order->billing_city = $cartUser['billing_city'];
    $order->billing_zone = $cartUser['billing_state'];
    $order->billing_country = 840;
    $bareaCode = $cartUser['billing_areacode'];

  
    $barcode = preg_replace('/[()]/s', '', $bareaCode);
    $bprefix = $cartUser['billing_prefix'];
    $blineNumber = $cartUser['billing_linenumber'];
    $order->billing_phone = $bareaCode . " " . $bprefix . "-" . $blineNumber;
    $userphone = $barcode . "-" . $bprefix . "-" . $blineNumber;
    $order->primary_email = $cartUser['billing_email'];
    $order->delivery_first_name = $cartUser['guest_firstname'];
    $order->delivery_last_name = $cartUser['guest_lastname'];
    $order->delivery_company = null;
    $order->delivery_street1 = $cartUser['guest_address'];
    $order->delivery_street2 = $cartUser['guest_otheraddress'];
    $order->delivery_postal_code = $cartUser['guest_zipcode'];
    $order->delivery_city = $cartUser['guest_city'];
    $order->delivery_zone = $cartUser['guest_state'];
    $order->delivery_country = 840;
    $areaCode = $cartUser['guest_areacode'];
    $prefix = $cartUser['guest_prefix'];
    $lineNumber = $cartUser['guest_linenumber'];
    $order->delivery_phone = $areaCode . " " . $prefix . "-" . $lineNumber;
    
	
    $order->payment_method = 'credit';
    $order->payment_details['cc_number'] = $creditCardNumber;
    $order->payment_details['cc_exp_month'] = $creditExpMonth;
    $order->payment_details['cc_exp_year'] = $creditExpYear;
    $order->payment_details['cc_cvv'] = $creditcid;
    $order->payment_details['cc_type'] =$creditCardType;
    
    
    
    
   
    uc_order_save($order);
    

    $logdata = array();
    $logdata[] = date('m/d/Y h:i:s a', time()) . " " . $order->delivery_first_name . " " . $order->delivery_last_name . "Started Booking";
    $logdata[] = "Order ID " . $order->order_id;
    $logdata[] = "Chosen " . $total_ticketQty . " Tickets ";
    $logdata[] = "Chosen " . $totalenhancementQty . " Enhancements";
    $logdata[] = "Completed Personal Details and Credit Card Details";
    $logdata[] = "Completed Review Order Operation";
    $logdata[] = "Payment Processing Initiated";
    $userObject = new UserLog();
    $userObject->id = "";
    $userObject->uid = $userId;
    $userObject->status = "0";
    $userObject->message = "Booking";
    $userObject->location = $_SERVER["REQUEST_URI"];
    $userObject->referer = "Booking";
    $userObject->data = serialize($logdata);
    $userObject->type = "booking";
    $userObject->timestamp = time();
    $userObject->ipaddress = $_SERVER['REMOTE_ADDR'];
    $activitylogid = UserLog::logUserActivity($userObject);
    
    
   
    
    
    
    


    $data['txn_type'] = UC_CREDIT_AUTH_CAPTURE;
    $cc_data['cc_number'] = $order->payment_details['cc_number'];
    $cc_data['cc_cvv'] = $order->payment_details['cc_cvv'];
    $cc_data['cc_exp_month'] = $order->payment_details['cc_exp_month'];
    $cc_data['cc_exp_year'] = $order->payment_details['cc_exp_year'];
    // Cache the values for use during processing.
    uc_credit_cache('save', $cc_data, FALSE);
    uc_order_save($order);
    
    
    if($orderTotalAmount>0){
    	if($orderTotalAmount==$order->order_total){
    		 
    		//Executing the Payment Process
    		//$result = uc_payment_process_payment('credit', $order->order_id, $orderTotalAmount, $data, TRUE, NULL, FALSE);
    		//_uc_credit_save_cc_data_to_order($order->payment_details, $order->order_id);
    		//echo "same";
    		
    		//Case :if order total is 0 then
    		//if ($orderTotalAmount == 0) {
    		//	$result = true;
    		//}
    		
    		
    		
    		$result=true;
    		if ($result) {
    			
    	
    			$bookingCart->storeCart();
    			uc_cart_complete_sale($order, $login = FALSE);
    			// After sucessfule completion for booking sale
    			try {
    				db_update('uc_orders')
    				->fields(array('order_status' => 'payment_received'))
    				->condition('order_id', $order->order_id)
    				->execute();
    			} catch (Exception $e) {
    				
    			}
    			
    			
    			
    		
    			//User Activity Log
    			$userObject = new UserLog();
    			$userObject->id = $activitylogid;
    			$userObject->status = "1";
    			$logdata[] = "Payment Success";
    			$logdata[] = date('m/d/Y h:i:s a', time()) . " Order Completed Successfully";
    			$userObject->data = serialize($logdata);
    		
    			$activitylogid = UserLog::logUserActivity($userObject);
    			
    		
    			//GiftCard
    			$giftCardArray=$bookingCart->getGiftCard();
    			if(count($giftCardArray)>0){
    				$giftCard= new _GiftCard();
    				$giftCard->updateGiftCardStatus($giftCardArray, $order);
    			}
    			
    			//Groupons Update
    			$vouchercodes=$bookingCart->getVouchers();
    			if(count($vouchercodes)>0){
    				$groupons= new _Groupons();
    				$groupons->updateGroupons($vouchercodes);
    			}
    			
    			
    			
    			
    			
    			
    			//Tickets Log
    			$ticketslog = new _TicketsLog();
    			$ticketslog->logTickets($ticketsList, $order->order_id,$eventId);
    			
    			//exit();
    			//Enhancements Log
    			$enhancementsLog= new _EnhancementsLog();
    			$enhancementsLog->enhancementsLog($selectedenhancements, $order->order_id,$eventId);
    			
    			//Userchoices Log
    		
    			
    			$reeservationLog= new _ReservationLog();
    			$reeservationLog->reservationLog($cartUser, $order, $eventId, $eventDate, $ticketsList, $selectedenhancements, $cartId,$occasiontype,$occasionadditionalcomments,$specialinstructions,$additionalcomments);
    			
    			//exit();
    			//$cartUser
    		
    			//Email Template Notification
    			$datestamp=Scheduledevents::getEventDateNTimeTimeStamp($eventId);
    			$emailNotificationObject = new EmailTemplateNotification ();
    			$emailNotificationObject->user_id = $order->uid;
    			$emailNotificationObject->cruise_template_id =  $cruiseTemplateId;
    			$emailNotificationObject->schedule_event_id = $eventId;
    			$emailNotificationObject->schedule_event_date = $datestamp;
    			$emailNotificationObject->order_id=  $order->order_id;
    			$emailNotificationObject->emailNotification ();
    			
    			
    			
    		
    			//Invoice Email
    			$params = array(
    					'subject' => t('Your Order at WATERWAYS'),
    					'body' => t("Data"),
    					'orderId' => $order->order_id
    			);
    		
    			$to = $order->primary_email;
    			$from=SiteConfiguration::getConfirmationEmail();
    			drupal_mail("email_system", "wwc_order_invoice_template", $to, language_default(), $params, $from);
    			return array('currentorder' => $order->order_id, 'bookingresult' => $result);
    		} else {
    			//User Activity Log
    			$userObject = new UserLog();
    			$userObject->id = $activitylogid;
    			$userObject->status = "0";
    			$logdata[] = "Payment Failed";
    			$logdata[] = date('m/d/Y h:i:s a', time()) . " Order incompleted";
    			$userObject->data = serialize($logdata);
    			$activitylogid = UserLog::logUserActivity($userObject);
    			return array('currentorder' => $order->order_id, 'bookingresult' => 'false');
  
    		}
    
    	}else{
    		echo "Invalid Operation Performed"; exit();
    	}
    }else{
    	//User Activity Log
    	$userObject = new UserLog();
    	$userObject->id = $activitylogid;
    	$userObject->status = "0";
    	$logdata[] = "Incorrect Order Total";
    	$logdata[] = date('m/d/Y h:i:s a', time()) . " Order incompleted";
    	$userObject->data = serialize($logdata);
    	$activitylogid = UserLog::logUserActivity($userObject);
    	return array('currentorder' => $order->order_id, 'bookingresult' => 'false');
    }
  }





function applyGrouponCode($grouponCode){
// 	var_dump($grouponCode);
// 	exit();
	
	$bookingCart=new BookingCart();
	$eventId=$bookingCart->getCartEventId();
	$ticketsList=$bookingCart->getTicketsList();
	$ordersubtotal=$bookingCart->getOrderSubTotal();
	$ticketobject = $bookingCart->getTicketsList();
	
	
	$orderobj = new stdClass ();
	$orderobj->ordertype = "booking";
	$eventdata = new stdclass ();
	$eventdata->eventid = $eventId;
	$eventdata->tickets = $ticketsList;
	$orderobj->events [] = $eventdata;
	$orderobj->ordersubtotal = $ordersubtotal;
	$orderobj->coupontype = "groupon";
	
	$groupon=new Groupon();
	$dealPriceAmount=$groupon->applyGroupon($grouponCode, $orderobj);
	return $dealPriceAmount;

	
}
function applyPromoCode($couponcode){
	
	$msg='';
	if(empty($couponcode)){
		$msg="Please Enter Valid Promocode!";
	}else{
		$bookingCart=new BookingCart();
		$eventId=$bookingCart->getCartEventId();
		$ticketsList=$bookingCart->getTicketsList();
		
		$tickettotalamt=$bookingCart->getTicketsTotalAmount();
		//$tickettotalamt=$bookingCart->getTicketsTotalAmount();

		
		//var_dump($tickettotalamt); exit();
			
		$eventdata = new stdclass ();
		$eventdata->eventid = $eventId;
		$eventdata->tickets = $ticketsList;
			
		$orderobj = new stdClass ();
		$orderobj->ordertype = "booking";
		$orderobj->events [] = $eventdata;
		$orderobj->ordersubtotal = $tickettotalamt;
		$orderobj->coupontype = "coupon";
		$couponObject= new Coupon();
		$discountamt=$couponObject->applyCoupon($couponcode, $orderobj);
		
		
		
		if ($discountamt > 0) {  //9
			$promocode = array($couponcode => $discountamt);
			$bookingCart->addPromoCode($promocode);
			$cpnstatus = "Promo Code successfully applied";
			
			
		} else {
			$cpnstatus = 'Invalid Promocode';
			
		}
		
		
		
		
	}


	//var_dump($form_state['values']); exit();
	//$couponcode = trim($form_state ['values'] ['prmocode']);
	///var_dump($couponcode); exit();

	drupal_set_message($cpnstatus);
}




//Gift Cart
function applyGiftCard($cardNumber){
	$bookingCart= new BookingCart();
	$totalAmount = (float) $bookingCart->getOrderTotal();
	
	if ($totalAmount > 0) {
	
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
					$bookingCart->addGiftCard($giftcardArray);
					$giftcardstatus = "Gifcard successfully applied";
			
				} else {
					$giftcardstatus = 'No balance exists for this card';
				}
			} 
	
	}
	
	return $giftcardstatus;
}





