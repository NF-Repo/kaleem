<?php

function giftcard_confirmation_page($val){
	$id=base64_decode($val);
	return drupal_get_form("giftcard_confirmation_details_form",$id);
	//echo base64_encode('satya');
	//echo base64_decode($val);
	//var_dump($val); exit();
	
}


function giftcard_confirmation_details_form($form, &$form_state,$id) {
	
	
	drupal_add_js(drupal_get_path('module', 'giftcard') . '/js/giftcard.js');
	$stages = array(
			1 => array('data' => '1'),
			2 => array('data' => '2'),
			3 => array('data' => '3'),
			4 => array('data' => '4'),
			5 => array('data' => '5'),
	);
	
	$current_step=5;
	
	$stages[$current_step]['class'] = array('active');
	
	$stages_list = theme('item_list', array('items' => $stages));
	
	/*
	 * content-inner div closed in subnav from element.
	*/
	$form['header']['subNav'] = array(
			'#type' => 'fieldset',
			'#title' => '',
			'#value' => $stages_list,
			'#attributes' => array('class' => array('subNav')),
			'#prefix' => '<div class="row-fluid"><div class="span12"><div class="menuRelative">',
			'#suffix' => '</div></div></div>'
	);
	
	
	
	$giftCardOrderdata=GiftCard::getGiftCardOrderData($id);
	
	
	//var_dump($giftCardOrderdata); exit();
	$message='';
	$deliverydate='';
	$firstName='';
	$email='';
	$totalamount='';
	
	if ($giftCardOrderdata) {
		$data = unserialize ( $giftCardOrderdata->data );
		$message = $data['customermessage'];
		$deliverydate = $data['deliverydate'];
		$firstName =  $data['yourname'];
		$email =  $data['recipientemail'];
		$totalamount = sprintf ( '%.2f', $giftCardOrderdata->giftcard_totalprice );
	}
       
	//var_dump($data); exit();
	drupal_add_css(drupal_get_path('module', 'giftcard') . '/css/giftcard.css');
			$form['giftcardreviewdetailsstartdiv'] = array(
				'#type' => 'markup',
				'#markup' => '<div id="purchaseForm">',
		);

		$form['confirmation'] = array(
				'#type' => 'markup',
				'#markup' => '<div class="row-fluid"><div class="span12"><h3>
          Thanks for Purchasing the Gift Card(s)</h3><h4>Your order ' . $id. '</h4></div></div>'
		);

		$form['hrwrapper'] = array(
				'#type' => 'markup',
				'#markup' => '<hr style="border: 1px solid #999;" />'
		);

		$form['ordersummary'] = array(
				'#type' => 'markup',
				'#markup' => '<div class="row-fluid"><div class="span12"><h3>Order Summary</h3></div></div>'
		);

		$form['ordersummaryto'] = array(
				'#type' => 'markup',
				'#markup' => '<div class="row-fluid"><div class="span12"><span>
          <strong>Gift Card: </strong></span><span>$' . $totalamount . '</span><br/><span><strong>To: </strong></span><span>' . $email . '</span><br/><span><strong>Delivery Date: </strong></span><span>'.$deliverydate.'</span></div></div>'
		);


		$form['ordersummaryfrom'] = array(
				'#type' => 'markup',
				'#markup' => '<div class="row-fluid"><div class="span12"><span><strong>From: </strong></span><span>' . $firstName . '</span><br/><span>' . $message . '!</span></div></div>'
		);


		$form['giftcardreviewdetailsenddiv'] = array(
				'#type' => 'markup',
				'#markup' => '</div">',
		);

		//$form_state['stage']='gift_card_details';
	
	return $form;
}