<?php
function payment($order_id){
	return drupal_get_form("booking_order_payment", $order_id);
}



function booking_order_payment($form, &$form_state,$order_id){
	
	drupal_add_css(drupal_get_path('module', 'booking_orders') . '/css/booking_orders.css', array('group' => CSS_DEFAULT, 'type' => 'file'));
	
	$form['orderheader']=array(
			'#type'=>'markup',
			'#markup'=>'<div class="row-fluid"><nav class="span12"><ul style="display:block;" class="breadcrumb">
			<li><a href="'.base_path().'staffadmin/order/'.$order_id.'/info">Order Info</a></li> <span class="icon-chevron-right"></span>
			<li ><a href="'.base_path().'staffadmin/order/'.$order_id.'/updateorder">Cart</a></li> <span class="icon-chevron-right"></span>
			<li><h4>Payment</h4></li> <span class="icon-chevron-right"></span>
			<li><a href="'.base_path().'staffadmin/order/'.$order_id.'/history">History</a></li>
			</nav></div>',
	);
	
	
	
    

	
	
	$form['hiddenorderid'] = array(
			'#id' => 'hiddenorderid',
			'#type' => 'textfield',
			'#prefix' => '<div style="display:none;">',
			'#suffix' => '</div>',
			'#default_value' => $order_id,
	);
	
	
	
	

	
	$order = uc_order_load($order_id);
	$newOrderTotalAmt = $order->order_total;
	$balance = round(uc_payment_balance($order), 2);
	if ($balance == -0) {
		$balance = 0;
	}
	$originalOrderAmt = $newOrderTotalAmt - $balance;
	
	
	$form ['balance'] = array(
			'#type' => 'textfield',
			'#prefix' => '<div style="display:none;">',
			'#suffix' => '</div>',
			'#default_value' => $balance,
			'#attributes' => array('readonly' => array('readonly')),
	);
	
	
	
	$form['customerbillinginformation'] = array(
			'#type' => 'fieldset',
			'#title' => '<span class="icon-info-sign"></span>&nbsp;'.t('Payment Information'),
			'#prefix' => '<div class="row-fluid"><div class="span1"></div><div class="span6">',
			'#suffix' => '</div>',
	);
	
	
	$bhtml='<table>';
	$bhtml.='<tr><td class="tdalignright"><b>First Name :</b></td><td>'.ucwords($order->billing_first_name).'</td></tr>';
	$bhtml.='<tr><td class="tdalignright"><b>Last Name :</b></td><td>'.ucwords($order->billing_last_name).'</td></tr>';
	$bhtml.='<tr><td class="tdalignright"><b>Address 1 :</b></td><td>'.ucwords($order->billing_street1).'</td></tr>';
	
	if(!empty($order->billing_street1)){
		$bhtml.='<tr><td class="tdalignright"><b>Address 2 :</b></td><td>'.ucwords($order->billing_street2).'</td></tr>';
	}
	
	$bhtml.='<tr><td class="tdalignright"><b>City :</b></td><td>'.ucwords($order->billing_city).'</td></tr>';
	$bhtml.='<tr><td class="tdalignright"><b>State :</b></td><td>'.ucwords(uc_zone_get_by_id($order->billing_zone)).'</td></tr>';
	$bhtml.='<tr><td class="tdalignright"><b>Zip Code :</b></td><td>'.ucwords($order->billing_postal_code).'</td></tr>';
	$bhtml.='<tr><td class="tdalignright"><b>Email :</b></td><td>'.$order->primary_email.'</td></tr>';
	$bhtml.='</table>';
	
	
	$form['customerbillinginformation']['info'] = array(
			'#type' => 'markup',
			'#markup' => $bhtml,
	);
	
	
	
	
	
	
	
	
	
	
	
	
	
	$form['ordersummary'] = array(
			'#type' => 'fieldset',
			'#title' => '<span class="icon-info-sign"></span>&nbsp;'.t('Order Information'),
			'#prefix' => '<div class="span4">',
			'#suffix' => '</div><div class="span1"></div></div>',
	);
	
	$orhtml='<table class=""><tr><td class=""><b>Invoice (Order ID) :</b></td><td style="width:50%">'.$order_id.'</td></tr>';
	$orhtml.='<tr><td class=""><b>Original Order Total:</b></td><td>$ '.$originalOrderAmt.'</td></tr>';
	$orhtml.='<tr><td class=""><b>New Order Total:</b></td><td>$ '.$newOrderTotalAmt.'</td></tr>';
	$orhtml.='<tr><td class=""></td><td>-------</td></tr>';
	$orhtml.='<tr><td class=""><b>Balance:</b></td><td> $'.$balance.'</td></tr></table>';
	
	
	
	$form['ordersummary']['info'] = array(
			'#type' => 'markup',
			'#markup'=>$orhtml,
	);

	$form['creditcardpaymentandrefund'] = array(
			'#type' => 'markup',
			'#markup' => '<div class="row-fluid">',
	);
	
	
	
	$form['creditcardpayment'] = array(
			'#type' => 'fieldset',
			'#title' => '<span class="icon-ok-sign"></span>&nbsp;'.t('Credit card terminal'),
			'#prefix' => '<div class="span1"></div><div class="span5">',
			'#suffix' => '</div>',
	);
	
	
	$form ['creditcardpayment'] ['span6col2contentdstart'] = array(
			'#type' => 'markup',
			'#markup' => '<table>',
	
	);
	
	
	$creditbalance='<tr><td class="tdalignright">  <b>Amount :</b></td><td>$ '.$balance.'</td></tr>';
	$form['creditcardpayment']['amount']=array(
			'#type'=>'markup',
			'#markup'=>$creditbalance,
	);
	
	
	$form['creditcardpayment']['creditCardType'] = array(
			'#type' => 'select',
			'#prefix' => '<tr><td class="tdalignright"><b>Card Type  :</b></td><td>',
			'#suffix' => '</td></tr>',
			'#options' => array(
					'visa' => 'Visa',
					'mastercard' => 'Master Card',
					'discover' => 'Discover',
					'americanexpress' => 'American Express',
			),
			
	);
	
	
	
	
	
	
	
	$form['creditcardpayment']['creditCardNumber'] = array(
			'#type' => 'textfield',
			'#prefix' => '<tr><td class="tdalignright"><b>Card Number :</b></td> <td>',
			'#suffix' => '</td></tr>',
	);
	
	
	
	$form['creditcardpayment']['creditExpMonth'] = array(
			'#type' => 'select',
			'#prefix' => '<tr><td class="tdalignright"><b>Exp. Month :</b></td><td>',
			'#suffix' => '</td></tr>',
			'#options' => array(1 => t('01 - January'), 2 => t('02 - February'),
					3 => t('03 - March'), 4 => t('04 - April'),
					5 => t('05 - May'), 6 => t('06 - June'),
					7 => t('07 - July'), 8 => t('08 - August'),
					9 => t('09 - September'), 10 => t('10 - October'),
					11 => t('11 - November'), 12 => t('12 - December')
			),
			
	);
	
	
	
	
	$min = intval(date('Y'));
	$max = intval(date('Y')) + 20;
	$form['creditcardpayment']['creditExpYear'] = array(
			'#type' => 'select',
			'#prefix' => '<tr><td class="tdalignright"><b>Exp. Year : </b></td><td>',
			'#suffix' => '</td></tr>',
			'#options' => drupal_map_assoc(range($min, $max)),
			
	);
	
	$form['creditcardpayment']['creditcid'] = array(
			'#type' => 'textfield',
			'#prefix' => '<tr><td class="tdalignright"><b>CVV Code :</b></td><td>',
			'#suffix' => '</td></tr>',
			'#maxlength' => 4,
	
	);
	
	$creditbtn=($balance > 0) ? array(''=>'') : array('disabled'=>'true');
	$form ['creditcardpayment'] ['processorder'] = array(
			'#type' => 'submit',
			'#value' => 'Process Payment',
			'#prefix' => '<tr><td></td><td>',
			'#suffix' => '</td></tr>',
			'#submit' => array('processpaymentprocess'),
			'#validate' => array('processpaymentprocess_validate'),
			'#attributes'=>$creditbtn,
			// 			'#ajax' => array(
			// 					'callback' => 'processorder',
			// 					'effect' => 'fade'
			// 			)
	);
	
	$form ['creditcardpayment'] ['span6col2contentdend'] = array(
			'#type' => 'markup',
			'#markup' => '</table>',
	
	);
	
	
	$form['paymentrefund'] = array(
			'#type' => 'fieldset',
			'#title' => '<span class="icon-ok-sign"></span>&nbsp;'.t('Refund Transaction'),
			'#prefix' => '<div class="span5">',
			'#suffix' => '</div><div class="span1"></div>',
	);
	
	
	$paymentReceipts=uc_payment_load_payments($order_id);
	$paymentReceiptData=unserialize($paymentReceipts[0]->data);
	$transactionid=$paymentReceiptData['txn_id'];
	
	$form['hiddentransactionid']=array(
			'#type'=>'textfield',
			'#default_value'=>$transactionid,
			'#prefix'=>'<div style="display:none;">',
			'#suffix'=>'</div>',
	);
	
	
	
	$transactioncontent='<table><tr><td class="tdalignright"><b>Transaction ID : </b></td><td>'.$transactionid.'</td></tr>';
		$form['paymentrefund']['transactionid']=array(
				'#type'=>'markup',
				'#markup'=>$transactioncontent,
	
		);
		
		$orderTotalAmt = $order->order_total;
		$balance = round(uc_payment_balance($order), 2);
		$refundbalance=0;
		if($balance<0){
		  if ($balance < $orderTotalAmt) {
				$amt = $orderTotalAmt - $balance;
				$refundbalance = abs($balance);
			}
		}
		
		
		$form['hiddenrefundbalance']=array(
				'#type'=>'textfield',
				'#default_value'=>$refundbalance,
				'#prefix'=>'<div style="display:none;">',
				'#suffix'=>'</div>',
		);
		
		$refundContent='<tr><td class="tdalignright">  <b>Refund Amount :</b></td><td>$ '.$refundbalance.'</td></tr>';
			$form['paymentrefund']['refundamount']=array(
					'#type'=>'markup',
					'#markup'=>$refundContent,
			);
		
		
		
		$form['paymentrefund']['refundcreditcardnumber']=array(
				'#type'=>'textfield',
				'#prefix' => '<tr><td class="tdalignright"><b>Credit Card Number:</b></td><td>',
			    '#suffix' => '</td></tr>',
		
		);
		
		
		
		
		$form['paymentrefund']['refundcreditExpMonth'] = array(
				'#type' => 'select',
				'#prefix' => '<tr><td class="tdalignright"><b>Exp. Month : </b></td><td>',
				'#suffix' => '</td></tr>',
				'#options' => array(1 => t('01 - January'), 2 => t('02 - February'),
						3 => t('03 - March'), 4 => t('04 - April'),
						5 => t('05 - May'), 6 => t('06 - June'),
						7 => t('07 - July'), 8 => t('08 - August'),
						9 => t('09 - September'), 10 => t('10 - October'),
						11 => t('11 - November'), 12 => t('12 - December')
				),
				'#attributes' => array('class'=>array('input-medium')),
		);
		
		
		
		
		$min = intval(date('Y'));
		$max = intval(date('Y')) + 20;
		$form['paymentrefund']['refundcreditExpYear'] = array(
				'#type' => 'select',
				'#prefix' => '<tr><td class="tdalignright"><b>Exp. Year: </b></td><td>',
				'#suffix' => '</td></tr>',
				'#options' => drupal_map_assoc(range($min, $max)),
				'#attributes' => array('class'=>array('input-small'))
		);
		
		$refundbtn=($refundbalance > 0) ? array(''=>'') : array('disabled'=>'true');
		$form ['paymentrefund'] ['refundpayment'] = array(
				'#type' => 'submit',
				'#value' => 'Refund Payment',
				'#prefix' => '<tr><td></td><td>',
				'#suffix' => '</td></tr>',
				'#submit' => array('refundpaymentprocess'),
				'#validate' => array('refundpaymentprocess_validate'),
				'#attributes'=>$refundbtn,
				
		);
		
		
		
		$form['paymentrefundend']=array(
				'#type'=>'markup',
				'#markup'=>'</table>',
		
		);
	return $form;
}




function refundpaymentprocess_validate($form, $form_state) {
	
	$creditCardNumber=trim($form_state ['values']['refundcreditcardnumber']);
	
	$creditExpMonth=trim($form_state ['values']['refundcreditExpMonth']);
	$creditExpYear=trim($form_state ['values']['refundcreditExpYear']);
	
	$statusKey = true;
	if(empty($creditCardNumber)){
		$statusKey = false;
		form_set_error('refundcreditcardnumber', '');
	}
	
	$currentMonth = (int) date("m");
	$currentYear = date("Y");
	
	
	if ($creditExpMonth < $currentMonth && $currentYear >= $creditExpYear) {
		$statusKey = false;
		form_set_error('refundcreditExpMonth', '');
		form_set_error('refundcreditExpYear', '');
	}
	
	
	if ($statusKey == false) {
		form_set_error('', 'Please fill the required fields');
	}

	
	
}

function processpaymentprocess_validate($form, $form_state) {
	
	$creditCardNumber=trim($form_state ['values']['creditCardNumber']);
	$creditExpMonth=trim($form_state ['values']['creditExpMonth']); 
	$creditExpYear=trim($form_state ['values']['creditExpYear']); 

	$creditcid=trim($form_state ['values']['creditcid']); 
	$creditCardType=trim($form_state ['values']['creditCardType']); 
	$statusKey = true;
	
	if (!is_valid_credit_card($creditCardNumber, $creditCardType)) {
		$statusKey = false;
		form_set_error('creditCardNumber', '');
		form_set_error('creditCardType', '');
	}
	
	
	
	$currentMonth = (int) date("m");
	$currentYear = date("Y");

	
	if ($creditExpMonth < $currentMonth && $currentYear >= $creditExpYear) {
	$statusKey = false;
	 form_set_error('creditExpMonth', '');
	 form_set_error('creditExpYear', '');
	}
	
	if (strlen($creditcid) < 3) {
		$statusKey = false;
		form_set_error('creditcid', '');
	}
	
	if ($statusKey == false) {
		form_set_error('', 'Please fill the required fields');
	}
	
	
	
}


function processpaymentprocess($form, $form_state){
	
	
		$order_id = $form_state ['values'] ['hiddenorderid'];
		$order = uc_order_load($order_id);
		$transactionamt = $form_state ['values'] ['balance'];
	
		$order->payment_method = 'credit';
		$order->payment_details['cc_number'] = $form_state ['values']['creditCardNumber']; 
		$order->payment_details['cc_exp_month'] = $form_state ['values']['creditExpMonth']; 
		$order->payment_details['cc_exp_year'] = $form_state ['values']['creditExpYear']; 
		$order->payment_details['cc_cvv'] = $form_state ['values']['creditcid']; 
		$order->payment_details['cc_type'] = $form_state ['values']['creditCardType']; 
	
		$data['txn_type'] = UC_CREDIT_AUTH_CAPTURE;
		$cc_data['cc_number'] = $order->payment_details['cc_number'];
		$cc_data['cc_cvv'] = $order->payment_details['cc_cvv'];
		$cc_data['cc_exp_month'] = $order->payment_details['cc_exp_month'];
		$cc_data['cc_exp_year'] = $order->payment_details['cc_exp_year'];
	
		// Cache the values for use during processing.
		uc_credit_cache('save', $cc_data, FALSE);
		uc_order_save($order);
	
		$result = uc_payment_process_payment('credit', $order->order_id, $transactionamt, $data, TRUE, NULL, FALSE);
		//$result = uc_payment_process_payment('credit', $form_state['values']['order_id'], $form_state['values']['amount'], $data, TRUE, NULL, FALSE);
	
		//_uc_credit_save_cc_data_to_order($order->payment_details, $order->order_id);
		_uc_credit_save_cc_data_to_order(uc_credit_cache('load'), $order->order_id);
	
		if ($result) {
			uc_cart_complete_sale($order, $login = FALSE);
			try {
				db_update('uc_orders')
				->fields(
				array('order_status' => 'payment_received'))
				->condition('order_id', $order->order_id)
				->execute();
			} catch (Exception $e) {
	
			}
	
	
			$msg="Payment Successful";
		} else {
			$msg="Payment failed";
		}
	
	drupal_set_message($msg);
	
}


function refundpaymentprocess($form, $form_state){
	
	$authCode=$form_state['values']['hiddentransactionid'];
	$orderid=$form_state['values']['hiddenorderid'];
	$refundAmount=$form_state['values']['hiddenrefundbalance'];
	
	$cardNumber=trim($form_state['values']['refundcreditcardnumber']);
	$expirationDate=trim($form_state['values']['refundcreditExpMonth']).''.trim($form_state['values']['refundcreditExpYear']);

	//API Login ID : 4rLuU885Hx6
	$login=trim(variable_get('uc_authnet_api_login_id', ''));
	
	
	//Transaction Key : 3H8MxX5hAP5365pP
	$transkey=trim(variable_get('uc_authnet_api_transaction_key', ''));
	
	/*
	 uc_authnet_aim_txn_mode : developer_test
	'live' => t('Live transactions in a live account'),
	'live_test' => t('Test transactions in a live account'),
	'developer_test' => t('Developer test account transactions'),
	*/
	$authnet_aim_txn_mode=variable_get('uc_authnet_aim_txn_mode', 'live_test');
	$xml=new AuthnetXML($login,$transkey,$authnet_aim_txn_mode);
	
	
	
	$xml->createTransactionRequest(array(
			'refId' => rand(1000000, 100000000),
			'transactionRequest' => array(
					'transactionType' => 'refundTransaction',
					'amount' => $refundAmount,
					'payment' => array(
							'creditCard' => array(
									'cardNumber' => $cardNumber,
									'expirationDate' => $expirationDate
							)
					),
					'authCode' => $authCode,
			),
	));
	
	
	
	$message='';
	if(isset($xml->messages->message->text)){
		$message.=$xml->messages->message->text;
	}
	
	//success
	if(isset($xml->transactionResponse->messages->message->description)){
		$message.="<br/>".$xml->transactionResponse->messages->message->description;
		//transaction id
		if(isset($xml->transactionResponse->transId)){
			$message.= "<br/><b>Transaction ID : </b> ".$xml->transactionResponse->transId;
		}
	}
	
	//error
	if(isset($xml->transactionResponse->errors->error->errorText)){
		$message.="<br/>".$xml->transactionResponse->errors->error->errorText;
	}
	
	
	$success=($xml->isSuccessful()) ? true : false;
	
	if($success){
		$transactionid= (int)$xml->transactionResponse->transId;

		global $user;
		$methodofpayment = 'credit';
		$_message = "Payment through Credit Card, Transaction ID: " . $transactionid;
		$_message.="<br/> Paid Amount:" . uc_currency_format($refundAmount);
		uc_order_comment_save($orderid, $user->uid, $_message);
		
		$comment = "Transaction ID: " . $transactionid;
		$data=array('module' => 'uc_authorizenet', 'txn_type' => 'auth_capture', 'txn_id' => $transactionid, 'txn_authcode' => 'YZXNQ6');
		uc_payment_enter($orderid, 'AuthroizeNet', -$refundAmount, $user->uid, $data, $comment, $received = REQUEST_TIME);
	}
	
	drupal_set_message($message);
}




function booking_order_payment_submit($form, $form_state) {
	
	var_dump('submit'); exit();
	
	$msg='';
	//$result=false;
	if ($form_state ['values']['creditCardNumber'] != '' && $form_state ['values']['creditExpMonth'] != '' && $form_state ['values']['creditCardType'] != '' && $form_state ['values']['creditcid'] != '') {
		$order_id = $form_state ['values'] ['hiddenorderid'];
		$order = uc_order_load($order_id);
		$transactionamt = $form_state ['values'] ['balance'];

		$order->payment_method = 'credit';
		$order->payment_details['cc_number'] = $form_state ['values']['creditCardNumber']; // '4007000000027';
		$order->payment_details['cc_exp_month'] = $form_state ['values']['creditExpMonth']; //'02';
		$order->payment_details['cc_exp_year'] = $form_state ['values']['creditExpYear']; //'15';
		$order->payment_details['cc_cvv'] = $form_state ['values']['creditcid']; //'521';
		$order->payment_details['cc_type'] = $form_state ['values']['creditCardType']; //'visa';//$_SESSION['payment

		
		$data['txn_type'] = UC_CREDIT_AUTH_CAPTURE;
		$cc_data['cc_number'] = $order->payment_details['cc_number'];
		$cc_data['cc_cvv'] = $order->payment_details['cc_cvv'];
		$cc_data['cc_exp_month'] = $order->payment_details['cc_exp_month'];
		$cc_data['cc_exp_year'] = $order->payment_details['cc_exp_year'];
		
		// Cache the values for use during processing.
		uc_credit_cache('save', $cc_data, FALSE);
		uc_order_save($order);

		$result = uc_payment_process_payment('credit', $order->order_id, $transactionamt, $data, TRUE, NULL, FALSE);
		//$result = uc_payment_process_payment('credit', $form_state['values']['order_id'], $form_state['values']['amount'], $data, TRUE, NULL, FALSE);
		
		//_uc_credit_save_cc_data_to_order($order->payment_details, $order->order_id);
	    _uc_credit_save_cc_data_to_order(uc_credit_cache('load'), $order->order_id);

		if ($result) {
			uc_cart_complete_sale($order, $login = FALSE);
			//$commands[] = ajax_command_alert("Payment Successful");
			try {
				db_update('uc_orders')
				->fields(
				array('order_status' => 'payment_received'))
				->condition('order_id', $order->order_id)
				->execute();
			} catch (Exception $e) {

			}


			$msg="Payment Successful";
			//drupal_set_message("Payment Successful");
			//$commands[] = array('command' => 'reloadafterpayment');
		} else {
			$commands[] = ajax_command_alert("Payment failed");
			$msg="Payment failed";
			//$commands[] = array('command' => 'reloadafterdeleteevent');
		}
	} else {
		$msg="Please Enter Valid Credit Card Details";
	}



	drupal_set_message($msg);
}


function proceedtorefundpage($form, &$form_state){
	$order_id = $form_state ['values'] ['hiddenorderid'];
	$path="refund/".$order_id;
	drupal_goto($path);
}



