<?php


function giftcard_form_submit($form, &$form_state) {
	
	switch ($form_state ['stage']) {
		case 'review_details' :
			$form_state ['multistep_values'] [$form_state ['stage']] = $form_state ['values'];
			if ($form_state ['triggering_element'] ['#value'] != 'Back') {
				$submitResult=giftcard_credit_card_details_submit ( $form, $form_state );
				if($submitResult){
					$form_state ['new_stage']="giftcard_confirmation";
				}else{
					$form_state ['new_stage']="credit_card_details";
				}
				//$form_state ['new_stage'] = giftcard_move_to_next_stage ( $form, $form_state );
			}
			break;
		
		default :
			$form_state ['multistep_values'] [$form_state ['stage']] = $form_state ['values'];
			$form_state ['new_stage'] = giftcard_move_to_next_stage ( $form, $form_state );
			break;
	}
	
	if ($form_state ['triggering_element'] ['#value'] == 'Back') {
		$form_state ['new_stage'] = giftcard_move_to_previous_stage ( $form, $form_state );
	}
	
	if (isset ( $form_state ['multistep_values'] ['form_build_id'] )) {
		$form_state ['values'] ['form_build_id'] = $form_state ['multistep_values'] ['form_build_id'];
	}
	$form_state ['multistep_values'] ['form_build_id'] = $form_state ['values'] ['form_build_id'];
	$form_state ['stage'] = $form_state ['new_stage'];
	
	$form_state ['rebuild'] = TRUE;
}





/**
 * Handles the submission of the final stage
 */

function giftcard_credit_card_details_submit($form, &$form_state) {
	
	$gift_card_details_values = isset($form_state['multistep_values']['gift_card_details']) ? $form_state['multistep_values']['gift_card_details'] : array();
	$giftcardprice=0;
	$userId=false;
	$productqty=0;
	
	$mailingAddressData=$form_state['multistep_values']['mailing_address_details'];
	$giftCardData=$form_state['multistep_values']['gift_card_details'];
	$creditCardData=$form_state['multistep_values']['credit_card_details'];
	$reviewData=$form_state['multistep_values']['review_details'];
	
	
	//User Account
	$userObject = user_load_by_mail($mailingAddressData['customeremail']);
	$userId=false;
	if ($userObject) {
		$userId = $userObject->uid;
	} else {
		$userObject = new UserAccount ();
		$userObject->user_name = $mailingAddressData['customerfirstname'].''.$mailingAddressData['customerlastname']. time();
		$userObject->password = user_password ( 6 );
		$userObject->firstname = $mailingAddressData['customerfirstname'];
		$userObject->last_name = $mailingAddressData['customerlastname'];
		$userObject->mail = $mailingAddressData['customeremail'];
		$userObject->role_id = 7;
		$userObject->phone = $mailingAddressData['customerphonenumber'];
		$userObject->address = $mailingAddressData['customeraddress'] . " " . $mailingAddressData['customercity'] . " " . $mailingAddressData['customerzipcode'];
		$userId = $userObject->createNewUser();
	}
	
	
	///var_dump($userId); exit();
	
	//Product
	$giftproductdata = node_load(272); //load the node of content type product.
	$giftcardAmt=$giftCardData['pregiftcardamount'];
	if(strcasecmp($giftCardData['pregiftcardamount'],"setmyown")==0){
		$giftcardAmt=$giftCardData['customgiftcardamount'];
	}

	$productqty = trim($giftCardData["giftcardqty"]);
	
	
	//Ubercart Order
	$order = uc_order_new($userId, 'in_checkout'); //Creating the new Order
	$order_id = $order->order_id;
	

	
	//Loading Billing Information(billing Details)
	$billingvalues = isset($form_state['multistep_values']['mailing_address_details']) ? $form_state['multistep_values']['mailing_address_details'] : array();
	$order->billing_first_name = $mailingAddressData['customerfirstname'];
	$order->billing_last_name = $mailingAddressData['customerlastname'];
	$order->billing_company = null;
	$order->billing_street1 = $mailingAddressData['customeraddress'];
	$order->billing_street2 = $mailingAddressData['customersecondaryaddress'];
	$order->billing_postal_code = $mailingAddressData['customerzipcode'];
	$order->billing_city = $mailingAddressData['customercity'];
	$order->billing_zone = $mailingAddressData['customerstate'];
	$order->billing_country = 840;
	$order->billing_phone = $mailingAddressData['customerphonenumber'];
	
	$order->primary_email = $mailingAddressData['customeremail'];
	
	//Loading Shipping information
	$order->delivery_first_name = $mailingAddressData['customerfirstname'];
	$order->delivery_last_name = $mailingAddressData['customerlastname'];
	$order->delivery_company = null;
	$order->delivery_street1 = $mailingAddressData['customeraddress'];
	$order->delivery_street2 = $mailingAddressData['customersecondaryaddress'];
	$order->delivery_postal_code = $mailingAddressData['customerzipcode'];
	$order->delivery_city = $mailingAddressData['customercity'];
	$order->delivery_zone = $mailingAddressData['customerstate'];
	$order->delivery_country = 840;
	$order->delivery_phone = $mailingAddressData['customerphonenumber'];
	

	//Adding Product to Ubercart Order
	$giftProduct = new stdClass ();
	$giftProduct->nid = $giftproductdata->nid;
	$giftProduct->title = $giftproductdata->title;
	$giftProduct->model = $giftproductdata->model;
	$giftProduct->qty = $productqty;
	$giftcardprice = sprintf ( '%.4f', $giftcardAmt );
	$giftProduct->price = $giftcardAmt;
	$order->products [] = $giftProduct;
	uc_order_save($order);
	

	//->Logging Gift Card Order
                
	$giftCardArray=array();    
	$giftCardArray['pregiftcardamount']=$giftcardAmt;
	$giftCardArray['giftcardqty']=$giftCardData['giftcardqty'];
	
	
	$giftCardArray['customgiftcardamount']=$giftCardData['customgiftcardamount'];
	$giftCardArray['recipientemail']=$giftCardData['recipientemail'];
	$giftCardArray['yourname']=$giftCardData['yourname'];
	$giftCardArray['customermessage']=$giftCardData['customermessage'];
	$giftCardArray['deliverydate']=$giftCardData['deliverydate'];
	$giftCardArray['promotioncode']=$giftCardData['promotioncode'];
	
	$giftCardArray['customerfirstname']=$mailingAddressData['customerfirstname'];
	$giftCardArray['customerlastname']=$mailingAddressData['customerlastname'];
	$giftCardArray['customeraddress']=$mailingAddressData['customeraddress'];
	$giftCardArray['customersecondaryaddress']=$mailingAddressData['customersecondaryaddress'];
	$giftCardArray['customercity']=$mailingAddressData['customercity'];
	$giftCardArray['customerstate']=$mailingAddressData['customerstate'];
	$giftCardArray['customerzipcode']=$mailingAddressData['customerzipcode'];
	$giftCardArray['customerphonenumber']=$mailingAddressData['customerphonenumber'];
	$giftCardArray['customeremail']=$mailingAddressData['customeremail'];
	$giftCardArray['knownby']=isset($mailingAddressData['knownby']) ? $mailingAddressData['knownby'] : '';
	$giftCardArray['receiveemailoption']=$creditCardData['receiveemailoption'];
	
	
	$giftCardOrderObject= new GiftCard();
	$giftCardOrderObject->giftcard_price=$giftcardprice;
	$giftCardOrderObject->giftcard_totalprice=($productqty*$giftProduct->price);
	$giftCardOrderObject->giftcard_qty=$productqty;
	$giftCardOrderObject->order_id=$order->order_id;
	$giftCardOrderObject->uid=$userId;
	$giftCardOrderObject->created=REQUEST_TIME;
	$giftCardOrderObject->data=serialize($giftCardArray);
	
	$giftCardOrderObject->loggingGiftCardOrder();
    $order->uid = $userId;
    $currenttotal = uc_order_get_total($order);
  	
  	
  	//Loading the Payment Creditcard details information
    $order->payment_method = 'credit';
    $order->payment_details['cc_number'] = $creditCardData['creditcardnumber']; 
    $order->payment_details['cc_exp_month'] = $creditCardData['cardexpmonth']; 
    $order->payment_details['cc_exp_year'] = $creditCardData['cardexpyear']; 
    $order->payment_details['cc_cvv'] = $creditCardData['creditcardcvv']; 
    $order->payment_details['cc_type'] = $creditCardData['creditcardtype'];
    //save the Order Object using order_save 
    uc_order_save($order);
  
 

   	//Updating the Payment Method on After Ordercreation completed in 'in-checkout mode
    $order = uc_order_load($order->order_id);
    db_update('uc_orders')
            ->fields(array('payment_method' => $order->payment_method))
            ->condition('order_id', $order->order_id)
            ->execute();

    //set the Payment Type
    $data = array();
   
		
		// watchdog('current amt entred', $order->order_total);
	$data ['txn_type'] = UC_CREDIT_AUTH_CAPTURE;
	
	$cc_data ['cc_number'] = $creditCardData ['creditcardnumber'];
	$cc_data ['cc_cvv'] = $creditCardData ['creditcardcvv'];
	$cc_data ['cc_exp_month'] = $creditCardData ['cardexpmonth'];
	$cc_data ['cc_exp_year'] = $creditCardData ['cardexpyear'];
	// Cache the values for use during processing.
	uc_credit_cache ( 'save', $cc_data, FALSE );
	uc_order_save ( $order );
    		    		
    //Executing the Payment Process
    $paymentgiftcard = uc_payment_process_payment ( 'credit', $order->order_id, $order->order_total, $data, TRUE, NULL, FALSE );
	_uc_credit_save_cc_data_to_order ( $order->payment_details, $order->order_id );
	
	if ($paymentgiftcard) {
		
		//Complete The order
		uc_cart_complete_sale($order, $login = FALSE);
		
		//Update The Order Sattus
		try {
			db_update('uc_orders')
			->fields(
			array('order_total' => $order->order_total,
			'order_status' => 'payment_received'))
			->condition('order_id', $order->order_id)
			->execute();
		} catch (Exception $e) {
		
		}
		
                
		
		// creating Gift object.
		
		$giftinputobj = new stdClass ();
		$giftinputobj->orderid = $order->order_id;
		$giftinputobj->uid = $userId;
		$giftinputobj->giftcarddata = $giftCardData;
		$giftinputobj->mailingaddressdata = $mailingAddressData;
		$giftinputobj->creditdata = $creditCardData;
		$giftinputobj->reviewdata = $reviewData;
		
		// Sending Email
		try{
			$params = array (
					'subject' => t ( 'Your Order at WATERWAYS!' ),
					'r_name' => $giftCardData ['recipientemail'],
					's_name' => $giftCardData ['yourname'],
					'amt' => $giftcardAmt,
					'body' => t ( "Data" ),
					'order_id' => $order->order_id,
					'message' => $giftCardData ['customermessage']
			);
			$to = $order->primary_email;
			$from = SiteConfiguration::getConfirmationEmail ();
			drupal_mail ( "email_system", "wwc_giftcard_invoice_mail", $to, language_default (), $params, $from );
		}catch(Exception $e){
			
		}
		
		$path="purchasegiftcard-confirmation/".base64_encode($order->order_id);
		drupal_goto($path); exit();
	}
	
	return $paymentgiftcard;
    
}





