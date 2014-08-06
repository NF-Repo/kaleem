<?php

/**
 * @file
 * Handle the form validation of the customer survey form
 * All hooks are in the .module file.
 */

/**
 * Master validation function for the customer survey form - uses per-stage 
 * validation and calls functions for each one.
 * 
 * @param type $form
 * @param type $form_state
 * @return type 
 */
function bookingagent_form_validate($form, &$form_state) {
    drupal_add_css(drupal_get_path('module', 'bookingagent') . '/css/bookingagent-mediaqueries.css');
    drupal_add_css(drupal_get_path('module', 'bookingagent') . '/css/bookingagent.css');
    drupal_add_js(drupal_get_path('module', 'bookingagent') . '/js/bookingagent.js');
    drupal_add_js(drupal_get_path('module', 'booking') . '/js/mask.js');
    
    
    
    
//    Go Back To Tickets
    

    if ($form_state['triggering_element']['#value'] == 'Go Back To Tickets') {
        //$form_state['stage']="ENHANCEMENTS";
        return;
    }
    
//    var_dump('asdf');
//    exit();

    if ($form_state['triggering_element']['#value'] == 'Go Back') {
        return;
    }


    if ($form_state['triggering_element']['#value'] == 'Apply') {
        return;
    }
    //return;
    
    

    switch ($form_state['stage']) {
        
        

        case 'TICKETS':
            return bookingagent_tickets_validate($form, $form_state);
            break;

        case 'PAYMENT':
            return bookingagent_payments_validate($form, $form_state);
            break;

        case 'ENHANCEMENTS':
            return bookingagent_enhancements_form_validate($form, $form_state);
            break;
            
            
            case 'REVIEW_ORDER' :
            	return bookingagent_review_form_validate($form, $form_state);
    }
}



function bookingagent_review_form_validate($form, &$form_state){

	$inputbookingdata = $_SESSION['bookingInput'];

	$cruiseTempateID = $inputbookingdata->templateid;
	$eventID = $inputbookingdata->eventid;
	$eventDate = $inputbookingdata->date;
	$errorflag=false;

	$availabletickets = Scheduledevents::getAvailableTicketsCount($cruiseTempateID, $eventID);
	$availablewindows = Scheduledevents::getAvailableWindowsCount($cruiseTempateID, $eventID);
	
	


	$sessionid = $_SESSION['cruisecardsession'];
	$selectedTicketsQty=0;
	$carttickets=unserialize(CartSession::getTickets($sessionid));
	
	foreach($carttickets as $ticket){
		$selectedTicketsQty+=$ticket['qty'];
	}
	
	$selectedEnhancements = unserialize(CartSession::getEnhancements($sessionid));
	$ticketsmsg='';
	if($selectedTicketsQty>0){
		if ($selectedTicketsQty > $availabletickets) {
			$ticketsmsg='Tickets';
			$errorflag=true;
		}
	}



	$selectedEnhancementQty=0;
	foreach ($selectedEnhancements as $product) {
		if (CruiseEnhancements::is_window_inventory_item($product['enhancedproductnodeid'])) {
			$selectedEnhancementQty+=$product['qty'];
		}
	}


	$enhancementsmsg='';
	if ($selectedEnhancementQty > 0) {
		if ($selectedEnhancementQty > $availablewindows) {
			$enhancementsmsg='Enhancements';
			$errorflag=true;
		}
	}


	$msg='';
	if($errorflag){

		if($ticketsmsg!='' && $enhancementsmsg!=''){
			$msg='This transaction cannot be completed at this time.<br/> ';
			$msg.='There are not enough Tickets and Enhancements available for your request. Please call (206) 223-2060 to speak with a sales representative.';
		}else if($ticketsmsg!=''){
			$msg='This transaction cannot be completed at this time.<br/>';
			$msg.='There are not enough Tickets available for your request. Please call (206) 223-2060 to speak with a sales representative.';
				
		}else if($enhancementsmsg!=''){
			$msg='This transaction cannot be completed at this time.<br/> ';
			$msg.='There are not enough Enhancements available for your request. Please call (206) 223-2060 to speak with a sales representative.';
		}
		form_set_error('', $msg);
	}
}




/**
 * Validation for the rate_the_room step
 * 
 * @param type $form
 * @param type $form_state 
 */
function bookingagent_tickets_validate($form, &$form_state) {
	
	$inputbookingdata = $_SESSION['bookingInput'];
	$cruiseTempateID=$inputbookingdata->templateid;
	$eventID=$inputbookingdata->eventid;
	$eventDate=$inputbookingdata->date;
	$availabletickets=0;
	$availabletickets=Scheduledevents::getAvailableTicketsCount($cruiseTempateID, $eventID);
	
	
	
 if ($form_state['triggering_element']['#value'] == 'Save & Continue') {
    	
    	if($availabletickets>0){
        $status = false;
        $cnt=0;
        foreach ($form_state['values'] as $key => $value) {
            $reg1 = "/^(ticketentrees)?\d+$/";
            if (preg_match($reg1, $key)) {
            	//Getting the Ticket ID
            	$entry=$key;
            	$ticketArr=split('ticketentrees', $entry);
            	$ticketID=$ticketArr[1];
                $cnt += (int) $form_state['values'][$key];
            }
        }//for 
        
        if($cnt>$availabletickets){
        	form_set_error('', 'There are not enough Tickets available for your request. Please call (206) 223-2060 to speak with a sales representative.');
        }
        
        if ($cnt == 0) {
            form_set_error('', 'You must select at least one ticket');
        }
        
        $limitedmobility=$form_state['values']['specialinstructions']['limitedmobility'];
		$foodallergy=$form_state['values']['specialinstructions']['foodallergy'];
		$anotherparty=$form_state['values']['specialinstructions']['anotherparty'];
		$additionalcomments=$form_state['values']['additionalcomments'];
		
		if (($anotherparty != "0" || $limitedmobility != "0" || $foodallergy != "0") && empty($additionalcomments)) {
			//echo "if";//please enter comments 
			form_set_error('additionalcomments', 'Please enter comments! ');
		}else{
			//echo "else";
		}
		
		
    }//if
    else{
    	form_set_error('', 'There are not enough Tickets available for your request. Please call (206) 223-2060 to speak with a sales representative.');
    }
    
    
    //Check comments
    
	}
	
	
}

function bookingagent_enhancements_form_validate($form, &$form_state) {
	
	$inputbookingdata = $_SESSION['bookingInput'];
	$cruiseTempateID=$inputbookingdata->templateid;
	$eventID=$inputbookingdata->eventid;
	$eventDate=$inputbookingdata->date;
	$availablewindows=0;
	$availablewindows=Scheduledevents::getAvailableWindowsCount($cruiseTempateID, $eventID);
	
	
	
	$bookinginputs = $form_state['values'];
	foreach ($form_state['values'] as $enhancmentsvalues) {
		$return = array();
		array_walk_recursive($enhancmentsvalues, function($a) use (&$return) {
			$return[] = $a;
		});
		
		
		$chunkresults = array_chunk($return, 7);
		$enhancementkeyorder = array (
						'qty',
						'customprice',
						'comments',
						'price',
						'name',
						'parentproduct',
						'enhancedproductnodeid' 
				);
				
		//$enhancementkeyorder = array('qty','price','name','comments', 'parentproduct', 'enhancedproductnodeid');
		foreach ($chunkresults as $chunkarray) {
			list($qty) = $chunkarray;
			if ($qty > 0) {
				$enhancementproducts[] = array_combine($enhancementkeyorder, $chunkarray);
			}
		}
	}
	
	
	
	$cnt=0;
         $msg='';
    $commenetError=false;
	foreach($enhancementproducts as $product){
		if(CruiseEnhancements::is_window_inventory_item($product['enhancedproductnodeid'])){
			$cnt+=$product['qty'];
		}
                
        if(CruiseEnhancements::is_comment_required($product['enhancedproductnodeid'])){
           // $msg.=$product['name'] ."<br/>";
           // $commenetError=true;
            
            if($product['comments']!=''){
             //nothing to validate   
            }else{
            $msg.=$product['name'] ."<br/>";
            
            $categoryid = CruiseEnhancements::get_product_category($product['enhancedproductnodeid']);

                $key = 'fieldset' . $categoryid;
                $errorkey = 'fieldset' .$categoryid. $product['enhancedproductnodeid'];
                
                /*    loads in scope = footer because of  loading jquery first   */
                drupal_add_js('jQuery(document).ready(function () { '
                        . 'collapsefieldset("' . $key . '","set"); '
                        . 'errormsg("' . $errorkey . '","set")'
                        . '});', array('type' => 'inline', 'scope' => 'footer', 'weight' => 5));


            $commenetError=true;
            }
            
            
        }
	}
	//do the validation if the quantity > 0
	if($cnt>0){
		if($cnt>$availablewindows){
			form_set_error('', 'There are not enough Enhancements available for your request. Please call (206) 223-2060 to speak with a sales representative.');
		}else{
			//put the Count in session variables for inventory updation
			//$_SESSION['inventory_selected_window_enhancements']=$cnt;
		}
	}
        
        if ($commenetError) {
            form_set_error('', 'Please Enter Comments for <br/>'.$msg);
    }
    //enhancment form validation gets here
    
	
    //enhancment form validation gets here
}

/**
 * Validation for the rate_the_service step
 * 
 * @param type $form
 * @param type $form_state 
 */
function bookingagent_payments_validate($form, &$form_state) {
	
	

    $statusKey = true;
    $guest_firstname = $form_state['values']['guest_firstname'];
    if (!is_valid_name($guest_firstname)) {
        $statusKey = false;
        form_set_error('guest_firstname', '');
    }
    
   
    if (!$form_state['values']['guest_lastname']) {
        $statusKey = false;
        form_set_error('guest_lastname', '');
    }
    
    if (!$form_state['values']['guest_address']) {
        $statusKey = false;
        form_set_error('guest_address', '');
    }
   
    $guest_city = $form_state['values']['guest_city'];
    if (!is_valid_name($guest_city)) {
        $statusKey = false;
        form_set_error('guest_city', '');
    }
   
    if (!$form_state['values']['guest_state']) {
        $statusKey = false;
        form_set_error('guest_state', '');
    }
    $guest_zipcode = $form_state['values']['guest_zipcode'];
    if (!is_valid_us_zipcode($guest_zipcode)) {
        $statusKey = false;
        form_set_error('guest_zipcode', '');
    }
    

    $guest_areaCode = $form_state['values']['guest_areacode'];
    $guest_prefix = $form_state['values']['guest_prefix'];
    $guest_linenumber = $form_state['values']['guest_linenumber'];
    $guestPhone = $guest_areaCode . '-' . $guest_prefix . '-' . $guest_linenumber;
    if (!_is_valid_us_phone($guestPhone)) {
        $statusKey = false;
        form_set_error('guest_areacode', '');
        form_set_error('guest_prefix', '');
        form_set_error('guest_linenumber', '');
    }
    
  

//    if (!$form_state['values']['guest_areacode']) {
//        form_set_error('guest_phone', 'Please Enter Your Phone');
//    }
//    if (!$form_state['values']['guest_email']) {
//        form_set_error('guest_email', 'Please Enter Your Email');
//    }
//    if (!$form_state['values']['guest_confirmemail']) {
//        form_set_error('guest_confirmemail', 'Please Confirm Your Email');
//    }

    $guest_email = $form_state['values']['guest_email'];
    $guest_confirmemail = $form_state['values']['guest_confirmemail'];

    if (is_valid_email($guest_email)) {
        if (!is_both_are_same($guest_email, $guest_confirmemail)) {
           
            form_set_error('guest_email', '');
            form_set_error('guest_confirmemail', '');
            $statusKey = false;
        }
    } else {
        form_set_error('guest_email', '');
        form_set_error('guest_confirmemail', '');
    }
    
//     var_dump($statusKey);
//     exit();
  


    //Billing Fields
    $billing_firstname = $form_state['values']['billing_firstname'];
    if (!is_valid_name($billing_firstname)) {
        $statusKey = false;
        form_set_error('billing_firstname', '');
    }
    
    
    if (!$form_state['values']['billing_lastname']) {
        $statusKey = false;
        form_set_error('billing_lastname', '');
    }
    if (!$form_state['values']['billing_address']) {
        $statusKey = false;
        form_set_error('billing_address', '');
    }
    $billing_city = $form_state['values']['billing_city'];
    if (!is_valid_name($billing_city)) {
        $statusKey = false;
        form_set_error('billing_city', '');
    }
    if (!$form_state['values']['billing_state']) {
        $statusKey = false;
        form_set_error('billing_state', '');
    }
    

    $billing_zipcode = $form_state['values']['billing_zipcode'];
    if (!is_valid_us_zipcode($billing_zipcode)) {
        $statusKey = false;
        form_set_error('billing_zipcode', '');
    }
//    if (!$form_state['values']['billing_phone']) {
//        form_set_error('billing_phone', 'Please Enter Your Phone');
//    }
    $billing_areaCode = $form_state['values']['billing_areacode'];
    $billing_prefix = $form_state['values']['billing_prefix'];
    $billing_linenumber = $form_state['values']['billing_linenumber'];
    $billingPhone = $billing_areaCode . '-' . $billing_prefix . '-' . $billing_linenumber;
    if (!_is_valid_us_phone($billingPhone)) {
        $statusKey = false;
        form_set_error('billing_areacode', '');
        form_set_error('billing_prefix', '');
        form_set_error('billing_linenumber', '');
    }
    
   
//    if (!$form_state['values']['billing_email']) {
//        form_set_error('billing_email', 'Please Enter Your Email');
//    }
//    if (!$form_state['values']['billing_confirmemail']) {
//        form_set_error('billing_confirmemail', 'Please Confirm Your Email');
//    }

    $billing_email = $form_state['values']['billing_email'];
    $billing_confirmemail = $form_state['values']['billing_confirmemail'];
    if (is_valid_email($billing_email)) {
        if (!is_both_are_same($billing_email, $billing_confirmemail)) {
            $statusKey = false;
            form_set_error('billing_email', '');
            form_set_error('billing_confirmemail', '');
        }
    } else {
        form_set_error('billing_email', '');
        form_set_error('billing_confirmemail', '');
    }
    
   
    $paymentMethodType=$form_state['values']['paymentMethodType'];
    
    if($paymentMethodType=="1"){
    	//creditCardType
    	//Credit Card Validation :creditCardNumber
    	$cardnumber = $form_state['values']['creditCardNumber'];
    	$creditcid = $form_state['values']['creditcid'];
    	$cardname = $form_state['values']['creditCardType'];
    	
    	if (!$form_state['values']['creditFirstName']) {
    		$statusKey = false;
    		form_set_error('creditFirstName', '');
    	}
    	
    	if (!$form_state['values']['creditLastName']) {
    		$statusKey = false;
    		form_set_error('creditLastName', '');
    	}
    	
    	if (strlen($cardname) == 0) {
    		$statusKey = false;
    		form_set_error('creditCardType', '');
    	}
    	
    	if (!is_valid_credit_card($cardnumber, $cardname)) {
    		$statusKey = false;
    		form_set_error('creditCardNumber', '');
    	}
    	
    	//     if (!$form_state['values']['creditExpMonth']) {
    	//         $statusKey = false;
    	//         form_set_error('creditExpMonth', '');
    	//     }
    	
    	//     if (!$form_state['values']['creditExpYear']) {
    	//         $statusKey = false;
    	//         form_set_error('creditExpYear', '');
    	//     }
    	
    	
    	
    	
    	$currentMonth = (int) date("m");
    	$currentYear = date("Y");
    	
    	$ccExpMonth=$form_state['values']['creditExpMonth'];
    	$ccExpYear=$form_state['values']['creditExpYear'];
    	
    	if (!$ccExpMonth) {
    		$statusKey = false;
    		form_set_error('creditExpMonth', '');
    	}
    	
    	if (!$ccExpYear) {
    		$statusKey = false;
    		form_set_error('creditExpYear', '');
    	}
    	
    	if($ccExpMonth!="" && $ccExpYear!="" ){
    		if ($ccExpMonth < $currentMonth && $currentYear >= $ccExpYear) {
    			form_set_error('creditExpMonth', 'Invalid Month');
    			form_set_error('creditExpYear', '');
    		}
    	}
    	
    	
    	
    	if (strlen($creditcid) < 3) {
    		$statusKey = false;
    		form_set_error('creditcid', '');
    	}
    }

    

    if ($statusKey == false) {
        form_set_error('', 'Please fill the required fields');
    }
}

/**
 * Validation for the personal_details step
 * 
 * @param type $form
 * @param type $form_state 
 */
function bookingagent_personal_details_validate($form, &$form_state) {
    if (!$form_state['values']['name']) {
        form_set_error('name', 'You must enter a name before continuing');
    }

    if (!valid_email_address($form_state['values']['email'])) {
        form_set_error('email', 'You must enter a valid email address');
    }
}