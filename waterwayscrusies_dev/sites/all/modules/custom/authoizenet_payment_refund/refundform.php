<?php


function authoizenet_payment_refund_page($orderid){
	return drupal_get_form("refund_form",$orderid);
	
}






function refund_form($form, &$form_state,$orderid) {
	
	
	drupal_add_css(drupal_get_path('module', 'authoizenet_payment_refund') . '/authoizenet_payment_refund.css');
	
	
	$cData=CreditCardFreezer::getorderccdata($orderid);
	$order=uc_order_load($orderid);
	$orderTotalAmt = $order->order_total;
    $balance = round(uc_payment_balance($order), 2);

    $refundbalance=0;
//     if ($balance < $orderTotalAmt) {
//     	$amt = $orderTotalAmt - $balance;
//         $refundbalance = abs($balance);
//     }
    
    if($balance<0){
    	if ($balance < $orderTotalAmt) {
    		$amt = $orderTotalAmt - $balance;
    		$refundbalance = abs($balance);
    	}
    }
    
   
	
	$orderTotal=$order->order_total;
	$customerEmail=$order->primary_email;
	$customerName=$order->delivery_first_name. " ".$order->delivery_last_name;
	
	$paymentReceipts=uc_payment_load_payments($orderid);
	$paymentReceiptData=unserialize($paymentReceipts[0]->data);
	$transactionid=$paymentReceiptData['txn_id'];
	
	
// 	$transactionids=array();
// 	foreach($paymentReceipts as $paymentReceipt){
// 		$paymentReceiptData=unserialize($paymentReceipt->data);
// 		$transactionids[$paymentReceiptData['txn_id']]=$paymentReceiptData['txn_id'];
// 	}

	//Hidden Elements
	
	$form['hiddenorderid']=array(
		'#type'=>'textfield',
		'#default_value'=>$orderid,
		'#prefix'=>'<div style="display:none;">',
		'#suffix'=>'</div>',
	);
	
	
	$form['hiddentransactionid']=array(
			'#type'=>'textfield',
			'#default_value'=>$transactionid,
			'#prefix'=>'<div style="display:none;">',
			'#suffix'=>'</div>',
	);
	
	$form['hiddenrefundbalance']=array(
			'#type'=>'textfield',
			'#default_value'=>$refundbalance,
			'#prefix'=>'<div style="display:none;">',
			'#suffix'=>'</div>',
	);
	
	
			
	$form['rowfluidstart']=array(
		'#type'=>'markup',
		'#markup'=>'
			<table><tr><td colspan=2>Click OK to continue this action and Refund this transaction. Click Cancel to return to the previous page.</td></tr>',
	);		
	
	//maskCreditCard();
	$transactioncontent='<tr><td></td></tr><tr><td class="tabledatalign">  <b>Transaction ID : </b></td><td>'.$transactionid.'</td></tr>';
	$form['transactionid']=array(
		'#type'=>'markup',
		'#markup'=>$transactioncontent,
		
	);
	
	$customernamecontent='<tr><td class="tabledatalign">  <b>Customer Name :</b></td><td>'.$customerName.'</td></tr>';
	$form['customername']=array(
		'#type'=>'markup',
		'#markup'=>$customernamecontent,
			
	);
	
	
	$customeremailcontent='<tr><td class="tabledatalign">  <b>Customer Email :</b></td><td>'.$customerEmail.'</td></tr>';
	$form['customeremail']=array(
		'#type'=>'markup',
		'#markup'=>$customeremailcontent,
	);
	
	
	$paymentContent='<tr><td class="tabledatalign">  <b>Payment Amount :</b></td><td>'.$orderTotal.'</td></tr>';
	$form['paymentamount']=array(
		'#type'=>'markup',
		'#markup'=>$paymentContent,
	);
	
	
	$refundContent='<tr><td class="tabledatalign">  <b>Refund Amount :</b></td><td>'.$refundbalance.'</td></tr>';
	$form['refundamount']=array(
			'#type'=>'markup',
			'#markup'=>$refundContent,
	);
	
	
	$invoiceContent='<tr><td class="tabledatalign">  <b>Invoice (Order ID) :</b></td><td>'.$orderid.'</td></tr>';
	$form['orderid']=array(
			'#type'=>'markup',
			'#markup'=>$invoiceContent,
	);
	
	
	$paymentMethodContent='<tr><td class="tabledatalign">  <b>Payment Method :</b></td><td>'.maskCreditCard(trim($cData[0]->ccnum)).'</td></tr>';
	$form['paymentmethod']=array(
			'#type'=>'markup',
			'#markup'=>$paymentMethodContent,
	);
	
	
	$expirationdateContent='<tr><td class="tabledatalign">  <b>Expiration Date :</b></td><td>'.trim($cData[0]->ccexpirationdate).'</td></tr>';
	$form['expirationdate']=array(
			'#type'=>'markup',
			'#markup'=>$expirationdateContent,
	);
	
	
		
	$form['refundsubmit']=array(
			'#type'=>'submit',
			'#value'=>'Ok',
			'#prefix'=>'<tr><td class="tabledatalign">',
			'#suffix'=>'</td>'
	);
	
	
	$form['refundcancel']=array(
			'#type'=>'submit',
			'#value'=>'Cancel',
			'#submit'=>array('refundcancelpage'),
			'#prefix'=>'<td >',
			'#suffix'=>'</td></tr>'
	);
	
	
	
	$form['rowfluidend']=array(
			'#type'=>'markup',
			'#markup'=>'</table>',
	);

	return $form;

}




function refundcancelpage($form, &$form_state){
	$orderid=$form_state['values']['hiddenorderid'];
	$path="staffadmin/order/".$orderid."/payment";
	drupal_goto($path);
}

function refund_form_submit($form, &$form_state){
	
	$authCode=$form_state['values']['hiddentransactionid'];
	$orderid=$form_state['values']['hiddenorderid'];
	$refundAmount=$form_state['values']['hiddenrefundbalance'];
	$cData=CreditCardFreezer::getorderccdata($orderid);
	$cardNumber=trim($cData[0]->ccnum);
	$expirationDate=trim($cData[0]->ccexpirationdate);
	

	
	
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