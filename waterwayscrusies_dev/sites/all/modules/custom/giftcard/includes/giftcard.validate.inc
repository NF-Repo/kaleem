<?php
function giftcard_form_validate($form, &$form_state) {
     
	drupal_add_js(drupal_get_path('module', 'giftcard') . '/js/giftcard.js');
	drupal_add_js(drupal_get_path('module', 'giftcard') . '/js/mask.js');
	
	if ($form_state['triggering_element']['#value'] == 'Apply') {
		return;
	}

    if ($form_state['triggering_element']['#value'] == 'Back') {
        return;
    }

    switch ($form_state['stage']) {

        case 'gift_card_details':
            return giftcard_gift_card_details_validate($form, $form_state);
            break;

        case 'mailing_address_details':
            return giftcard_mailing_address_details_validate($form, $form_state);
            break;

        case 'credit_card_details':
            return giftcard_credit_card_details_validate($form, $form_state);
            break;

        case 'review_details':
            return giftcard_review_details_form_validate($form, $form_state);
            break;
    }
}

/**
 * Validation for the gift_card_details step
 */
function giftcard_gift_card_details_validate($form, &$form_state) {
	drupal_add_css ( drupal_get_path ( 'module', 'giftcard' ) . '/css/giftcard.css' );
	$selectedQty = $form_state ['values'] ['giftcardqty'];
	
	
	// step 1 validation takes place here
	$statusKey = true;
	
	// do the validation if choice is setmyown
	$selectedchoice = $form_state ['values'] ['pregiftcardamount'];
	if ($selectedchoice == "setmyown") {
		$enteredvalue = $form_state ['values'] ['customgiftcardamount'];
		if ($enteredvalue == "" || $enteredvalue == 0) {
			form_set_error ( 'customgiftcardamount', 'Please Enter A valid Amount.' );
		}
	}
	
	$customername = $form_state ['values'] ['yourname'];
	
	if (! is_valid_name ( $customername )) {
		$statusKey = false;
		form_set_error ( 'yourname', 'Please Enter valid Name' );
	}
	$customeremail = $form_state ['values'] ['recipientemail'];
	
	if (! is_valid_email ( $customeremail )) {
		form_set_error ( 'recipientemail', 'Please Enter valid Email Id' );
		$statusKey = false;
	}
	
	$selecteddeliverydate = $form_state ['values'] ['deliverydate'];
	$selecteddatetime = strtotime ( $selecteddeliverydate );
	$currenttime = time ();
	if ($selecteddeliverydate != NULL) {
		if ($selecteddatetime < $currenttime) {
			form_set_error ( 'deliverydate', 'Delivery date should be greater than current date.' );
			$statusKey = false;
		}
	}
}

/**
 * Validation for the mailing_address_details step
 */
function giftcard_mailing_address_details_validate($form, &$form_state) {
    drupal_add_css(drupal_get_path('module', 'giftcard') . '/css/giftcard.css');
    $statusKey = true;

    $customerfirstname = $form_state['values']['customerfirstname'];
    if (!is_valid_name($customerfirstname)) {
        $statusKey = false;
        form_set_error('customerfirstname', 'Please Enter A valid First Name');
    }

    if (!$form_state['values']['customeraddress']) {
        $statusKey = false;
        form_set_error('customeraddress', 'Please Enter Address');
    }


    $customer_city = $form_state['values']['customercity'];
    if (!is_valid_name($customer_city)) {
        $istatusKey = false;
        form_set_error('customercity', 'Please Enter A city');
    }

    if (!$form_state['values']['customerstate']) {
        $statusKey = false;
        form_set_error('customerstate', 'Please Enter A State');
    }
    $customer_zipcode = $form_state['values']['customerzipcode'];
    if (!is_valid_us_zipcode($customer_zipcode)) {
        $statusKey = false;
        form_set_error('customerzipcode', 'Please Enter A valid Zipcode');
    }
    $phone = $form_state['values']['customerphonenumber'];
    if (!is_valid_us_phone($phone)) {
        $statusKey = false;
        form_set_error('customerphonenumber', 'Please Enter A valid Phone Number');
    }

    $customer_email = $form_state['values']['customeremail'];
    $customer_confirmemail = $form_state['values']['customerconfirmemail'];

    if (!is_valid_email($customer_email)) {
        form_set_error('customeremail', 'Please Enter valid Email Id');

        $statusKey = false;
    }
    if (!is_both_are_same($customer_email, $customer_confirmemail)) {

        form_set_error('customerconfirmemail', 'Email and Confirmation Email not matches');
        $statusKey = false;
    }
    }

/**
 * Validation for the credit_card_details step
 */
function giftcard_credit_card_details_validate($form, &$form_state) {


    $gift_card_details_values = isset($form_state['multistep_values']['gift_card_details']) ? $form_state['multistep_values']['gift_card_details'] : array();
 
    drupal_add_css(drupal_get_path('module', 'giftcard') . '/css/giftcard.css');

    $statusKey=true;
    
    
    $cardnumber = $form_state['values']['creditcardnumber'];
    $creditcid = $form_state['values']['creditcardcvv'];
    $cardname = $form_state['values']['creditcardtype'];
    $cardyear = $form_state['values']['cardexpyear'];
    $selectedmonth = $form_state['values']['cardexpmonth'];
    if ($cardname == 'chooseone') {
        form_set_error('creditcardtype', 'Please Select Card Type.');
    }
    if (!is_valid_credit_card($cardnumber, $cardname)) {
        $statusKey = false;
        form_set_error('creditcardnumber', 'Please Enter A valid Credit Card Number.');
    }

    
    if ($form_state['values']['creditcardtype'] == 'americanexpress' && strlen($creditcid) < 4) {
        $statusKey = false;
        form_set_error('creditcardcvv', 'Please Enter CID Atleast 4 Digits.');
    } else if (strlen($creditcid) < 3) {
        $statusKey = false;
        form_set_error('creditcardcvv', 'Please Enter CID Atleast 3 Digits.');
    }
    if (!(is_numeric($creditcid))) {
        $statuskey = false;
        form_set_error('creditcardcvv', 'Please Enter Valid CID Number');
    }
    if (!is_valid_credit_card_month($selectedmonth, $cardyear)) {
        // $statusKey=false;
        form_set_error('cardexpmonth', 'Please Enter A Valid Credit Card Expiry Month.');
}

    // if(!is_valid_)
    if (!$form_state['values']['cardexpmonth']) {
        $statusKey = false;
        form_set_error('cardexpmonth', 'Please Enter Card Expired Month.');
    }

    if (!$form_state['values']['cardexpyear']) {
        $statusKey = false;
        form_set_error('cardexpyear', 'Please Enter Card Expired year.');
    }
    
}

//giftcard_review_details_form
function giftcard_review_details_form_validate($form, &$form_state) {
    $gift_card_details_values = isset($form_state['multistep_values']['gift_card_details']) ? $form_state['multistep_values']['gift_card_details'] : array();

}
