<?php

function booking_form_validate($form, &$form_state) {
    drupal_add_css(drupal_get_path('module', 'booking') . '/css/booking-mediaqueries.css');
    drupal_add_css(drupal_get_path('module', 'booking') . '/css/booking.css');
    drupal_add_js(drupal_get_path('module', 'booking') . '/js/booking.js');
    drupal_add_js(drupal_get_path('module', 'booking') . '/js/mask.js');

    if ($form_state['triggering_element']['#value'] == 'Go Back To Tickets') {
        return;
    }

    if ($form_state['triggering_element']['#value'] == 'Go Back') {
        return;
    }

    if ($form_state['triggering_element']['#value'] == 'Apply') {
        return;
    }
    
    if ($form_state['triggering_element']['#value'] == 'Remove') {
    	return;
    }
    
    
    
    switch ($form_state['stage']) {
        case 'TICKETS':
            return booking_tickets_validate($form, $form_state);
            break;

        case 'PAYMENT':
            return booking_payments_validate($form, $form_state);
            break;

        case 'ENHANCEMENTS':
            return booking_enhancements_form_validate($form, $form_state);
            break;
            
            
         case 'REVIEW_ORDER' :
           	return booking_review_form_validate($form, $form_state);
    }
}


function booking_review_form_validate($form, &$form_state){
	
	$bookingCart=new BookingCart();
	$cruiseTempateID = $bookingCart->getCartCruiseTemplateId();
	$eventID = $bookingCart->getCartEventId();
	$eventDate =$bookingCart->getCartEventDate();
	$errorflag=false;

	$availabletickets = Scheduledevents::getAvailableTicketsCount($cruiseTempateID, $eventID);
	$availablewindows = Scheduledevents::getAvailableWindowsCount($cruiseTempateID, $eventID);
	
	


	$selectedTicketsQty=0;
	$carttickets=$bookingCart->getTicketsList();
	
	foreach($carttickets as $ticket){
		$selectedTicketsQty+=$ticket['qty'];
	}
	
	$selectedEnhancements =$bookingCart->getEnhancementsList();
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




function booking_tickets_validate($form, &$form_state) {
	
	
	$bookingCart=new BookingCart();
    $cruiseTempateID =$bookingCart->getCartCruiseTemplateId();
    $eventID = $bookingCart->getCartEventId();
    $eventDate =$bookingCart->getCartEventDate();
    
    //Tickets- 3.4 Getting Assigned Tickets Quantiy based on Schedule Events
    $availabletickets = 0;
    $availabletickets = Scheduledevents::getAvailableTicketsCount($cruiseTempateID, $eventID);
    
    $val=0;
    $cnt = 0;
    if ($form_state['triggering_element']['#value'] == 'Save & Continue') {
        if ($availabletickets > 0) {
            $status = false;
            foreach ($form_state['values'] as $key => $value) {
                $reg1 = "/^(ticketentrees)?\d+$/";
                if (preg_match($reg1, $key)) {
                    //Getting the Ticket ID
                    $entry = $key;
                    $ticketArr = explode('ticketentrees', $entry);
                    $ticketID = $ticketArr[1];
                    
                    if ($form_state['values']['ticketentrees' . $ticketID] > 0) {
                    	
                    	//Tickets- 3.5 is Child Ticket
                        $isChildTicket = CruiseTickets::isChildTicket($ticketID);
                        if ($isChildTicket) {
                            //skip the validation	
                        } else {
                            $val = (int) $form_state['values'][$key];
                        }
                       // var_dump($form_state['values'][$key]);
                        $cnt += (int) $form_state['values'][$key];
                        if ($val > 0) {
                            $status = true;
                            //break;
                        }
                    }
                }
            }//for 
            
        
            //Tickets- 3.6
            if ($cnt > $availabletickets) {
                form_set_error('', 'There are not enough Tickets available for your request. Please call (206) 223-2060 to speak with a sales representative.');
            }
            if ($status == false) {
                form_set_error('', 'You must select at least one ticket');
            } else {
                
            }

            $limitedmobility = $form_state['values']['specialinstructions']['limitedmobility'];
            $foodallergy = $form_state['values']['specialinstructions']['foodallergy'];
            $anotherparty = $form_state['values']['specialinstructions']['anotherparty'];
            $additionalcomments = $form_state['values']['additionalcomments'];

            if (($anotherparty != "0" || $limitedmobility != "0" || $foodallergy != "0") && empty($additionalcomments)) {
                form_set_error('additionalcomments', 'Please enter comments! ');
            } else {
               
            }
        }//if
        else {
            form_set_error('', 'There are not enough Tickets available for your request. Please call (206) 223-2060 to speak with a sales representative.');
        }
    }
}



//Enhancements - 3.6
function booking_enhancements_form_validate($form, &$form_state) {
	
	$bookingCart=new BookingCart();
	
	$cruiseTempateID =$bookingCart->getCartCruiseTemplateId();
	$eventID = $bookingCart->getCartEventId();
	$eventDate =$bookingCart->getCartEventDate();

    $availablewindows = 0;
    $enhancementproducts=array();
    
    //Enhancements - 3.7
    $availablewindows = Scheduledevents::getAvailableWindowsCount($cruiseTempateID, $eventID);
    $bookinginputs = $form_state['values'];
    foreach ($form_state['values'] as $enhancmentsvalues) {
        $return = array();
        @array_walk_recursive($enhancmentsvalues, function($a) use (&$return) {
            $return[] = $a;
        });
        $chunkresults = array_chunk($return, 6);

        $enhancementkeyorder = array('qty', 'price', 'name', 'comments', 'parentproduct', 'enhancedproductnodeid');
        foreach ($chunkresults as $chunkarray) {
            list($qty) = $chunkarray;
            if ($qty > 0) {
                $enhancementproducts[] = array_combine($enhancementkeyorder, $chunkarray);
            }
        }
    }

    $cnt = 0;


    //drupal_json_output($enhancementproducts); exit();
    $msg = '';
    $commenetError = false;
    foreach ($enhancementproducts as $product) {
    	//Enhancements - 3.8
        if (CruiseEnhancements::is_window_inventory_item($product['enhancedproductnodeid'])) {
            $cnt+=$product['qty'];
        }


        //Enhancements - 3.9
        if (CruiseEnhancements::is_comment_required($product['enhancedproductnodeid'])) {
              if ($product['comments'] != '') {
                //nothing to validate   
            } else {
                $msg.=$product['name'] . "<br/>";
                $categoryid = CruiseEnhancements::get_product_category($product['enhancedproductnodeid']);

                $key = 'fieldset' . $categoryid;
                $errorkey = 'fieldset' .$categoryid. $product['enhancedproductnodeid'];
                
                /*    loads in scope = footer because of  loading jquery first   */
                drupal_add_js('jQuery(document).ready(function () { '
                        . 'collapsefieldset("' . $key . '","set"); '
                        . 'errormsg("' . $errorkey . '","set")'
                        . '});', array('type' => 'inline', 'scope' => 'footer', 'weight' => 5));

                $commenetError = true;
            }
        }
    }


    //do the validation if the quantity > 0
    if ($cnt > 0) {
        if ($cnt > $availablewindows) {
            form_set_error('', 'There are not enough Enhancements available for your request. Please call (206) 223-2060 to speak with a sales representative.');
        } else {
        }
    }

    if ($commenetError) {
        form_set_error('', 'Please Enter Comments for <br/>' . $msg);
    }
    //Comment Box Validation
    //enhancment form validation gets here
}



function booking_payments_validate($form, &$form_state) {

	$bookingCart=new BookingCart();
	
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
        //if (_validate_credit_card_type($cardnumber)) {
        $statusKey = false;
        form_set_error('creditCardNumber', '');
        form_set_error('creditCardType', '');
    }
    
    
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

    $orderTotalAmount=$bookingCart->getOrderTotal();
    if($orderTotalAmount>0){
    	
    }else{
    	$statusKey = false;
    	form_set_error('','The transaction cannot be processed at this time.');
    }
    

    if ($statusKey == false) {
        form_set_error('', 'Please fill the required fields');
    }
}

