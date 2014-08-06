<?php
/* Handle the form submission of the Booking form 
 * 
 * ->Tickets
 * ->Cruise Fee Calculation
 * ->Cruise Fee UPdated Price -  initially the actual price and modified price are same, but in the 
 *   enhancements page updated cruise fee stored in the modified price field
 * ->Total Amount
 * ->Create Cart Session Object
 * ->Move to Next Stage
 * 
 * 1 : Getting Tax value
 * 2: session id
 * 
 * 
 * Calculations -> Tickets + Enhancements + Cruise Fee Items - Promocode - Gift Card - Voucher codes
 */


function bookingagent_form_submit($form, &$form_state) {
	$sessionid=$_SESSION['cruisecardsession'];
	/*
	 * --------Check the Tax Value-------
	*/
	
	$taxvalue='';
	$taxitemslistserialized=CartSession::getTaxAmtItems($sessionid);
	//$taxvalue=variable_get('WWCTAXVALUE','');
	
	if($taxitemslistserialized){
		$taxitemslist=unserialize($taxitemslistserialized);
		$tval=$taxitemslist['taxvalue'];
		if($tval!='' && $tval>0){
			$taxvalue=$tval;
		}else{
			$taxvalue=variable_get('WWCTAXVALUE','');;
		}
	}else{
		$taxvalue=variable_get('WWCTAXVALUE','');
	}

    switch ($form_state['stage']) {
        case 'TICKETS':
            $result = array();
            $totalresults = array();
            $totalticketsprice = 0;
            $tickets_tax=0;
            foreach ($form_state['values'] as $key => $value) {
                $reg1 = "/^(ticketentrees)?\d+$/";
                if (preg_match($reg1, $key)) {
                    if ($form_state['values'][$key] > 0) {
                        $ticketenteeeid = explode('ticketentrees', $key);
                        $result = array();
                        //------------Ticket--------------
                        $result['ticketid'] = $ticketenteeeid[1];
                        $ticketID = $ticketenteeeid[1];
                        $result['qty'] = $form_state['values'][$key];
                        $result['price'] = $form_state['values'][$key . "price"];
                        $result['ticketname'] = $form_state['values'][$key . "ticketname"];

                        //--------Tickets Entrees and Comments-----------
                        $entrees = array();
                        $entreesComments = array();
                        for ($i = 0; $i < $result['qty']; $i++) {
                            $entrees[] = $form_state['values']['select' . $result['ticketid'] . $i];
                            $entreesComments[] = $form_state['values']['select' . $result['ticketid'] . $i] . '-' . $i . '-' . $form_state['values']['dietarycomments' . $result['ticketid'] . $i];
                        }
                        $result['ticketentreeids'] = $entrees;
                        $result['ticketentreescomments'] = $entreesComments;
                        $totalticketsprice+= $form_state['values'][$key] * $form_state['values'][$key . "price"];
                        $totalresults[] = $result;

                        $istaxableTicket = CruiseTickets::isTaxableTicket($ticketID);
                        if ($istaxableTicket) {
                            $tickets_tax+=$form_state['values'][$key] * $form_state['values'][$key . "price"];
                        }
                    }
                }
            }//foreach








            /*------------Cruise Fee Calculation--------------------
             * 
             */
            
        
            $cruiseFeeItemPrice=0;
            $ticket_tax_feeids=array();
            $feeitems=Fees::load_fee_items();
            	foreach ($feeitems as $feeitem) {
                $feeId = $feeitem->id;
                $feeTitle = $feeitem->title;
                //$feeValue = $feeitem->value;
                $feeType = $feeitem->type;
                $chargeamt = 0;

                $a_feeValue = $feeitem->value;
                $feeValue = 0;



                //Getting the Fee Value
                $inputbookingdata = $_SESSION['bookingInput'];
                $cruise_id = $inputbookingdata->templateid;
                //Check is Fee activated or not
                $feeStatus = CruiseFees::getCruiesFeeStatus($cruise_id, $feeId);

                if ($feeStatus == "1") {
                    $u_feeValue = CruiseFees::getCruiesFeePrice($cruise_id, $feeId);
                    if ($u_feeValue > 0 && $u_feeValue != "") {
                        $feeValue = $u_feeValue;
                    } else {
                        $feeValue = $a_feeValue;
                    }


                    //$totalresults nothing but total tickets
                    foreach ($totalresults as $ticket) {
                        if ($ticket ['qty'] > 0) {
                            $ticket_id = $ticket ['ticketid'];
                            $ticket_qty = $ticket ['qty'];
                            //$ticket_price = $ticket ['price'];
                            $ticket_price = $ticket ['price'];


                            $ticket_feeids = Fees::get_ticket_feeids($ticket_id);
                            if (in_array($feeId, $ticket_feeids)) {
                                $ticket_tax_feeids[] = $feeId;
                                if ($feeType == "flatfee") {
                                    //Cruise Fee Updated Price
                                    $m_feevalue = CartSession::getCruieFeeItemModifiedPrice($sessionid, $feeId);
                                    $u_feeValue = ($m_feevalue != '') ? $m_feevalue : $feeValue;
                                    // var_dump($m_feevalue);
                                    $totalcharge = $ticket_qty * $u_feeValue;
                                    $chargeamt += $totalcharge;
                                }
                                if ($feeType == "percentage") {
                                    $totalticketpriceamt = $ticket_qty * $ticket_price;
                                    //Cruise Fee Updated Price
                                    $m_feevalue = CartSession::getCruieFeeItemModifiedPrice($sessionid, $feeId);
                                    $u_feeValue = ($m_feevalue != 0) ? $m_feevalue : $feeValue;
                                    $chargeamt += ($totalticketpriceamt * $u_feeValue) / 100;
                                }
                            }
                        }
                    }//tickets loop ends here
                }

                if ($chargeamt != "" && $chargeamt != 0) {
                    $cruiseFeeItemPrice+=$chargeamt;
                    $cruisefeeitems[$feeId] = array('feeid' => $feeId, 'title' => $feeTitle, 'type' => $feeType, 'actualrate' => $feeValue, 'updatedrate' => $u_feeValue, 'totalamt' => $chargeamt);
                }
            }//cruise fee items loop ends here


            /*------------Tax Calculations--------------------
             * 
             */
            $feeitems_tax = 0;
            foreach ($cruisefeeitems as $key => $values) {
                $feeid = $values['feeid'];
                if (in_array($feeid, $ticket_tax_feeids)) {
                    $feeTaxableStatus = Fees::is_taxable_cruisefee_item($feeid);
                    if ($feeTaxableStatus) {
                        $feeitems_tax+=$values['totalamt'];
                    }
                }
            }





            //Check promocode
            $cpn = CartSession::getPromocode($sessionid);
            $cpndiscountamt = 0;
            if ($cpn != "" && !empty($cpn)) {
                $cpncode = false;
                $cpnarr = unserialize($cpn);
                foreach ($cpnarr as $key => $val) {
                    $cpncode = $key;
                }

                if ($cpncode) {
                    $inputbookingdata = $_SESSION ['bookingInput'];
                    $ticketobject = $totalresults;
                    //$ordersubtotal= $subTotalPrice; here order subtotal total tickets price
                    try {
                        $orderobj = new stdClass ();
                        $orderobj->ordertype = "booking";
                        $eventdata = new stdclass ();
                        $eventdata->eventid = $inputbookingdata->eventid;
                        $eventdata->tickets = $ticketobject;
                        $orderobj->events [] = $eventdata;
                        $orderobj->ordersubtotal = $totalticketsprice;
                        $orderobj->coupontype = "coupon";
                        $couponObj = entity_get_controller('coupon')->applycoupon($cpncode, $orderobj);
                        $cpndiscountamt = $couponObj [0]->discountamount;
                        $promocode = array($cpncode => $cpndiscountamt);
                        CartSession::insertPromoCode($sessionid, $promocode);
                    } catch (Exception $e) {
                        // watchdog
                    }
                }
            }










            /*------------Enhancements Calculation--------------------
             * 
             */
            $enhancements_total_price = 0;
            $enhancementTotal = CartSession::getTotalEnhancementsPrice($sessionid);
            if ($enhancementTotal != "") {
                $enhancements_total_price = $enhancementTotal;
            }

            
            
            /*------------subtotal Calculation--------------------
             * 
             * Order Subtotal = Tickets+Enhancements+FeeItems
             */
            $orderSubTotal=$totalticketsprice+$enhancements_total_price+$cruiseFeeItemPrice;
            
             
            
            /*
             * ------------ Tax Calculations -------------
             */
            
            $taxitemsserialized = CartSession::getTaxAmtItems($sessionid);
            
            
        
            $taxamt = 0;
            if ($taxitemsserialized) {
                $taxitems = unserialize($taxitemsserialized);
                $tickets_tax = $tickets_tax;
                //$feeitems_tax = $taxitems['feeitems_tax'];
                $enhancements_tax = $taxitems['enhancements_tax'];


                $taxarr = array();
                $taxarr['taxvalue']=$taxvalue;
                $taxarr['tickets_tax'] = $tickets_tax;
                $taxarr['feeitems_tax'] = $feeitems_tax;
                $taxarr['enhancements_tax'] = $enhancements_tax;
                if ($taxvalue > 0) {
                    $taxamt = sprintf("%.2f", ($tickets_tax + $feeitems_tax + $enhancements_tax) * $taxvalue / 100);
                }

                $taxarr['total_tax'] = $taxamt;


                CartSession::insertTaxItems($sessionid, serialize($taxarr));
            } else {
                $taxarr = array();
                $taxarr['taxvalue']=$taxvalue;
                $taxarr['tickets_tax'] = $tickets_tax;
                $taxarr['feeitems_tax'] = $feeitems_tax;
                if ($taxvalue > 0) {
                    $taxamt = sprintf("%.2f", ($tickets_tax + $feeitems_tax) * $taxvalue / 100);
                }

                $taxarr['total_tax'] = $taxamt;
            }


            /*
             * ------------- ORDER TOTAL------------
             * Order Total = Subtotal + TAX
             * 
             */
            $orderTotalPrice=($orderSubTotal+$taxamt)-$cpndiscountamt;
            
            
            //Create Cart Session Object
            $cartsesison = new stdClass();
            $cartsesison->cartid = 'cart' . user_password(14);
            $cartsesison->sessionid = $sessionid;
            $cartsesison->bookitems = (object) array('ticketoptions' => $totalresults);
            $cartsesison->createdtime = REQUEST_TIME;
            $cartsesison->subtotal=($orderSubTotal);
            $cartsesison->ticketstotalprice=$totalticketsprice;
            $cartsesison->total=$orderTotalPrice;
            $cartsesison->tickets=serialize($totalresults);
            $cartsesison->feeitems=serialize($cruisefeeitems);
            $cartsesison->tax=serialize($taxarr);
            CartSession::createCartSession($cartsesison);
          
            //Move to Next Stage
            $form_state['multistep_values'][$form_state['stage']] = $form_state['values'];
            $form_state['new_stage'] = bookingagent_move_to_next_stage($form, $form_state);
            break;



        case 'ENHANCEMENTS':
        	
        	$updTaxValue=$form_state['values']['taxvalue'];
        	
        	//drupal_json_output($form_state ['values']); exit();
            $productresult = array();
            $enhancementproducts = array();
            foreach ($form_state ['values'] as $enhancmentsvalues) {
                $return = array();
                array_walk_recursive($enhancmentsvalues, function ($a) use(&$return) {
                    $return [] = $a;
                });

                $chunkresults = array_chunk($return, 7);
                $enhancementkeyorder = array(
                    'qty',
                    'price',
                    'comments',
                    'actualprice',
                    'name',
                    'parentproduct',
                    'enhancedproductnodeid'
                );
                foreach ($chunkresults as $chunkarray) {
                    list ( $qty ) = $chunkarray;
                    if ($qty > 0) {
                        $enhancementproducts [] = array_combine($enhancementkeyorder, $chunkarray);
                    }
                }
            }
            
            
          

            //enhancements insertion
            CartSession::insertEnhancements($sessionid, $enhancementproducts);

            //Calculating the Enhancments Total
            $enhancementtotalprice = 0;
            foreach ($enhancementproducts as $productoption) {
                if ($productoption['qty'] > 0) {
                    $productprice = $productoption['qty'] * (float) $productoption['price'];
                    $enhancementtotalprice+=$productprice;
                }
            }
            
            //Total Enhancment insertion
            CartSession::insertTotalEnhancementsPrice($sessionid, $enhancementtotalprice);
            
          
            //Check promocode
            $totalTicketPrice = CartSession::getTotalTicketsPrice($sessionid);
            $cpn = CartSession::getPromocode($sessionid);
            $cpndiscountamt = 0;
            if ($cpn != "" && !empty($cpn)) {
                $cpncode = false;
                $cpnarr = unserialize($cpn);
                foreach ($cpnarr as $key => $val) {
                    $cpncode = $key;
                }

                if ($cpncode) {
                    $inputbookingdata = $_SESSION ['bookingInput'];
                    $ticketobject = CartSession::getTickets($sessionid);
                    try {
                        $orderobj = new stdClass ();
                        $orderobj->ordertype = "booking";
                        $eventdata = new stdclass ();
                        $eventdata->eventid = $inputbookingdata->eventid;
                        $eventdata->tickets = $ticketobject;
                        $orderobj->events [] = $eventdata;
                        $orderobj->ordersubtotal = $totalTicketPrice;
                        $orderobj->coupontype = "coupon";
                        $couponObj = entity_get_controller('coupon')->applycoupon($cpncode, $orderobj);
                        $cpndiscountamt = $couponObj [0]->discountamount;
                        $promocode = array($cpncode => $cpndiscountamt);
                        CartSession::insertPromoCode($sessionid, $promocode);
                    } catch (Exception $e) {
                        // watchdog
                    }
                }
            }
            
            
            //Remove Gift Card
            CartSession::removeGiftCard($sessionid);
            CartSession::removeVoucher($sessionid);
            //remove groupons
            unset($_SESSION['order_groupons']);


            

            //Cruise Fees
            $cruisefeeitems = array();
            $ticket_tax_feeids=array();
            $cruiseFeeItemPrice = 0;
            foreach ($form_state['values'] as $key => $value) {
                $feereg = "/^(cruisefeeid_)?\d+$/";
                if (preg_match($feereg, $key)==1) {
                    $arr = explode('cruisefeeid_', $key);
                    $feeid = $arr[1];
                    $title = $form_state['values']['cruisefeetitle_' . $feeid];
                    $updatedRate = $form_state['values']['cruisefeesmodifiedrate_' . $feeid];
                    $actualRate = $form_state['values']['cruisefeesactualrate_' . $feeid];
                    $type = $form_state['values']['cruisefeestype_' . $feeid];

                    //calculating total tickets price
                    $tickets = unserialize(CartSession::getTickets($sessionid));
                    $tickets_tax=0;
                    $chargeamt = 0;
                    foreach ($tickets as $ticket) {
                        if ($ticket ['qty'] > 0) {
                            $ticket_id = $ticket ['ticketid'];
                            $ticket_qty = $ticket ['qty'];
                            //$ticket_price = $ticket ['price'];
                            $ticket_price = $ticket ['price'];
                            $ticket_feeids = Fees::get_ticket_feeids($ticket_id);
                            if (in_array($feeid, $ticket_feeids)) {
                                $ticket_tax_feeids[]=$feeid;

                                if ($type == "flatfee") {
                                    $m_feevalue = $updatedRate;
                                    $u_feeValue = ($m_feevalue != '') ? $m_feevalue : $actualRate;
                                    $totalcharge = $ticket_qty * $u_feeValue;
                                    $chargeamt += $totalcharge;
                                }

                                if ($type == "percentage") {
                                    $totalticketpriceamt=$ticket_qty*$ticket_price;
                                    //Cruise Fee Updated Price
                                    $m_feevalue = $updatedRate;
                                    $u_feeValue = ($m_feevalue != '') ? $m_feevalue : $actualRate;
                                    $chargeamt += ($totalticketpriceamt * $u_feeValue) / 100;
                                }
                            }
                            
                        $istaxableTicket = CruiseTickets::isTaxableTicket($ticket_id);
                        if ($istaxableTicket) {
                            $tickets_tax+=$ticket_qty * $ticket_price;
                        }
                        }
                    }//tickets loop ends here

                    $cruiseFeeItemPrice+=$chargeamt;
                    $cruisefeeitems[$feeid] = array('feeid' => $feeid, 'title' => $title, 'type' => $type, 'actualrate' => $actualRate, 'updatedrate' => $updatedRate, 'totalamt' => $chargeamt);
                }
            }
            
            
            //if No Fees
            if(count($cruisefeeitems)==0){
                 $taxitemslist=  unserialize(CartSession::getTaxAmtItems($sessionid));
                 $tickets_tax=$taxitemslist['tickets_tax'];
                    
            }

            
            CartSession::insertCruiseFeeItems($sessionid, $cruisefeeitems);
            
            
            
          
             
             //---------Cruise Fee Tax Calculation--------------
            $feeitems_tax = 0;
            foreach ($cruisefeeitems as $key => $values) {
                $feeid = $values['feeid'];
                if (in_array($feeid, $ticket_tax_feeids)) {
                    $feeTaxableStatus = Fees::is_taxable_cruisefee_item($feeid);
                    if ($feeTaxableStatus) {
                        $feeitems_tax+=$values['totalamt'];
                    }
                }
            }
           
            
            
            
            
            
            
            $totalTicketsamt=  CartSession::getTotalTicketsPrice($sessionid);
             

            $chargeTax=0;
            $chargeAmt=0;
            $chargetype=$form_state ['values']['chargetype'];
            $charge=$form_state ['values']['charge'];
            $chargedescription=$form_state ['values']['chargedescription'];
            $chargevalue=  is_numeric($form_state ['values']['chargevalue']) ?$form_state ['values']['chargevalue'] : 0;
            //$_SESSION['chargeTaxName']=$chargedescription;
            
            $chargeTaxName='';
            $chargeTaxName=$chargedescription;
            
            //Type is Dollar Amount
            
            /*
             * Case 1: (Charge) Checkbox checked and Type is Dollar Amount, Add Charge Tax to Tax calculations
             *         
             */
            if($chargetype=="dollaramount"){
                if($charge==1){
                    //done
                    $chargeTax=$chargevalue;
                    $chargeTaxValue=$chargeTax;
                }
                else{
                    $chargeAmt=$chargevalue;
                    $chargeTaxValue=$chargeAmt;
                }
                
            }else{
                //PERCENTAGE
                if($charge==1){
                    
                    $percentagefee=($totalTicketsamt+$enhancementtotalprice)*$chargevalue/100;
                    $chargeTax=$percentagefee;
                    $chargeTaxValue=$chargeTax;
                } 
                else{
                    $percentagefee=($totalTicketsamt+$enhancementtotalprice)*$chargevalue/100;
                    $chargeAmt=$percentagefee;
                    $chargeTaxValue=$chargeAmt;
                }
                
            }
            
            
            //insertion
            $tax_lineitem=array();
            $tax_lineitem[$chargeTaxName]=array('taxname'=>$chargeTaxName,'chargetype'=>$chargetype,'checkbox'=>$charge,'amt'=>$chargeTaxValue);
            CartSession::insertTaxLineItem($sessionid, $tax_lineitem);
            
            
            //Discounts
            $discounttype=$form_state ['values']['discounttype'];
            $discount=$form_state ['values']['discount'];
            $discountdescription=$form_state ['values']['discountdescription'];
            $discountvalue=  is_numeric($form_state ['values']['discountvalue']) ? $form_state ['values']['discountvalue'] : 0;
            
            $descriptionDiscountName=$discountdescription;
            
            $discountTax=0;
            $discountAmt=0;
      if($discounttype=="dollaramount"){
                if($discount==1){
                    $discountTax=$discountvalue;
                    $descriptionDiscountValue=$discountTax;
                }
                
                else{
                    $discountAmt=$discountvalue;
                    $descriptionDiscountValue=$discountAmt;
                }
                
            }else{
                //percentage
                if($discount==1){
                    $percentagefee=($totalTicketsamt+$enhancementtotalprice)*$discountvalue/100;
                    $discountTax=$percentagefee;
                    $descriptionDiscountValue=$discountTax;
                    
                }else{
                    $percentagefee=($totalTicketsamt+$enhancementtotalprice)*$discountvalue/100;
                    $discountAmt=$percentagefee;
                    $descriptionDiscountValue=$discountAmt;
                    
                }
                
            }
            
            //insertion
            $discount_lineitem=array();
            $discount_lineitem[$descriptionDiscountName]=array('discountname'=>$descriptionDiscountName,'discounttype'=>$discounttype,'checkbox'=>$discount,'amt'=>$descriptionDiscountValue);
            CartSession::insertDiscountLineItem($sessionid, $discount_lineitem);
            
            $taxamt=0;
               if($updTaxValue>0){
               	$tax_amt = (($tickets_tax + $feeitems_tax + $enhancementtotalprice+$chargeTax)-$discountTax) * $updTaxValue / 100;
               	$taxamt=sprintf("%.2f", $tax_amt);
               }
            $taxarr = array();
            $taxarr['taxvalue']=$updTaxValue;
            $taxarr['tickets_tax'] = $tickets_tax;
            $taxarr['feeitems_tax'] = $feeitems_tax;
            $taxarr['enhancements_tax'] = $enhancementtotalprice;
            $taxarr['total_tax'] = $taxamt;
            CartSession::insertTaxItems($sessionid, serialize($taxarr));
            
            /*------------Subtotal Calculation--------------------
             * Order Subtotal = Tickets+Enhancements+FeeItems
             */
            $orderSubTotal=($totalTicketPrice+$enhancementtotalprice+$cruiseFeeItemPrice+$chargeTax+$chargeAmt-$discountTax-$discountAmt);
            CartSession::insertOrderSubTotal($sessionid, $orderSubTotal);
            
            
            /*
             * Total Order Calculations
             */
            /*
             * ------------- ORDER TOTAL------------
             * Order Total = Subtotal + TAX
             * 
             */
            $orderTotalPrice=($orderSubTotal+$taxamt)-$cpndiscountamt;
            CartSession::insertOrderTotal($sessionid, $orderTotalPrice);
            //unset($_SESSION['order_groupons']);
            
            
            $form_state['multistep_values'][$form_state['stage']] = $form_state['values'];
            $form_state['new_stage'] = bookingagent_move_to_next_stage($form, $form_state);
            break;


        case 'PAYMENT':
            //Moving Page to Next Step
            $form_state['multistep_values'][$form_state['stage']] = $form_state['values'];
            $form_state['new_stage'] = bookingagent_move_to_next_stage($form, $form_state);
            break;

        case 'REVIEW_ORDER':
            
            if ($form_state['triggering_element']['#value'] != 'Go Back') {
            
            $form_state['paymentstatus'] = "";
            $form_state['booking_orderid'] = "";
            bookingagent_review_form_submit($form, $form_state);
            
            
            //var_dump($form_state['booking_orderid']); exit();
            if ($form_state['paymentstatus'] == "success") {
            	$orderpath = 'confirmation/orderid/' . $form_state['booking_orderid'] . '/' . $_SESSION['cruisecardsession'];
            	drupal_goto($orderpath);
            	$form_state['complete'] = TRUE;
            } else {
//            	drupal_set_message('Invalid Card Details');
            	$form_state['new_stage'] = "PAYMENT";
            }
            }
            
            
            
            
            
            break;

        default:
            $form_state['multistep_values'][$form_state['stage']] = $form_state['values'];
            $form_state['new_stage'] = bookingagent_move_to_next_stage($form, $form_state);
            break;
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

        $form_state['new_stage'] = bookingagent_move_to_previous_stage($form, $form_state);
    }

    if (isset($form_state['multistep_values']['form_build_id'])) {
        $form_state['values']['form_build_id'] = $form_state['multistep_values']['form_build_id'];
    }
    $form_state['multistep_values']['form_build_id'] = $form_state['values']['form_build_id'];
    $form_state['stage'] = $form_state['new_stage'];
    $form_state['rebuild'] = TRUE;
}

/**
 * Handles the submission of the final stage
 * Sends an email to the user confirming their entry
 *  
 */
function bookingagent_review_form_submit($form, &$form_state) {
	
    $bookingagentinputs = $form_state['multistep_values'];
    $bookingagentresults = bookingagent_procees($bookingagentinputs);
    
    
    if ($bookingagentresults['bookingagentresult']) {
    	$path = 'adminconfirmation/orderid/' . $bookingagentresults['currentorder'] . '/' . $_SESSION['cruisecardsession'];
    	drupal_goto($path);
    	$form_state['complete'] = TRUE;
     }
    else {
    	$form_state['paymentstatus'] = "failed";
    	//watchdog('bookingstatus', 'Invalid Card Details');
    }
}



function bookingagent_procees($bookingagentinputs) {
	
	$sessionid=$_SESSION['cruisecardsession'];
	
	//Calculated Prices
	$total_tickets_price=CartSession::getTotalTicketsPrice($sessionid);
	$total_enhancements_price=CartSession::getTotalEnhancementsPrice($sessionid);

    //making complex nested array into single array using array_walk_recursive function
    $bookingagentinputsinglearray = array();
    array_walk_recursive($bookingagentinputs, function ($value, $key) use (& $bookingagentinputsinglearray) {
                $bookingagentinputsinglearray[$key] = $value;
	});
	
	
     //Getting Tickets
    //$total_tickets_price = 0;
   	$total_ticketQty=0;
    $selectedtickets = array ();
    $tickets=unserialize(CartSession::getTickets($sessionid));
    foreach($tickets as $ticket){
    	if($ticket['qty']>0){
    		$selectedtickets[] = $ticket;
    		$total_ticketQty+=$ticket['qty'];
    		//$total_tickets_price+=$ticket['qty'] * $ticket['price'];
    	}
    }
    
 
    
    
    //Getting the Product ID of Event ID ( Event ID treated as a Product node)
    $sessionObject = $_SESSION['bookingInput'];
    $eventId=$sessionObject->eventid;
    $scheduleEventObject=scheduledevents_load($eventId);
    $event_product_node=$scheduleEventObject->nid;
    $eventproductnodedata = node_load($event_product_node);

    $eventProduct=new stdClass();
    $eventProduct->nid = $event_product_node;
    $eventProduct->qty = 1; //By Default Cruise Item qty is treating as single
    $eventProduct->price = $total_tickets_price;
    $eventProduct->title = $eventproductnodedata->title;
    $eventProduct->model = $eventproductnodedata->model;
    
    $newUserObject = new stdClass();
    $guestDOB='';
    if(!empty($bookingagentinputs['PAYMENT']['guest_dob_month'])){
    	$guest_dob_month=$bookingagentinputs['PAYMENT']['guest_dob_month'];
    	$guest_dob_day=$bookingagentinputs['PAYMENT']['guest_dob_day'];
    	$guestDOB=$guest_dob_month."-".$guest_dob_day;
    	 
    }
    
    
    $guestAniverssary='';
    if(!empty($bookingagentinputs['PAYMENT']['guest_aniverssary_month'])){
    	$guest_aniverssary_month=$bookingagentinputs['PAYMENT']['guest_aniverssary_month'];
    	$guest_aniverssary_day=$bookingagentinputs['PAYMENT']['guest_aniverssary_day'];
    	$guestAniverssary=$guest_aniverssary_month."-".$guest_aniverssary_day;
    }
    $newUserObject->mail = $bookingagentinputs['PAYMENT']['guest_email'];
    $userObject = user_load_by_mail($newUserObject->mail);
    
    //check user existence if it is true returns the user id otherwise new account created
    if ($userObject) {
    	$userId = $userObject->uid;
    } else {
    	$userObject = new UserAccount ();
    	$userObject->user_name = $bookingagentinputs['PAYMENT']['guest_firstname'] . '' . $bookingagentinputs['PAYMENT']['guest_lastname'] . time();
    	$userObject->password = "password";
    	$userObject->firstname = $bookingagentinputs['PAYMENT']['guest_firstname'];
    	$userObject->last_name = $bookingagentinputs['PAYMENT']['guest_lastname'];
    	$userObject->mail = $bookingagentinputs['PAYMENT']['guest_email'];
    	$userObject->role_id = 7;
    	$guestPhone=$bookingagentinputs['PAYMENT']['guest_areacode']." ".$bookingagentinputs['PAYMENT']['guest_prefix']."  ".$bookingagentinputs['PAYMENT']['guest_linenumber'];
    	$userObject->phone = $guestPhone;
    	$userObject->address = $bookingagentinputs['PAYMENT']['billing_address'] . " " . $bookingagentinputs['PAYMENT']['billing_city'] . " " . $bookingagentinputs['PAYMENT']['billing_zipcode'];
    	$userId = $userObject->createNewUser();
    }
    //creating order
    $order = uc_order_new($userId, 'in_checkout'); //Creating the new Order
    $order_id = $order->order_id;
    $order->products[] = $eventProduct;
   
    $selectedenhancements = array ();
    $enhancements=unserialize(CartSession::getEnhancements($sessionid));
    $totalenhancementQty=0;
    foreach ($enhancements as $enhancement) {
    	$totalenhancementQty+=$enhancement['qty'];
    	$selectedenhancements[] = $enhancement;
    	$cartenhancednode = node_load($enhancement['enhancedproductnodeid']);
    	$cartenhancedproduct = uc_product_load($cartenhancednode);
    	$cartenhancedproduct->nid = $cartenhancednode->nid;
    	$cartenhancedproduct->qty = $enhancement['qty']; //By Default Cruise Item qty is treating as single
    	//$cartenhancedproduct->price=$enhancement['qty']* $enhancement['customprice'];
    	$cartenhancedproduct->price=$enhancement['price'];
    	$cartenhancedproduct->title = $cartenhancednode->title;
    	$cartenhancedproduct->model = $cartenhancednode->model;
    	$order->products[] = $cartenhancedproduct;
    }
    
    $feeItems=CartSession::getCruiseFeeItems($sessionid);
    foreach($feeItems as $feeitem){
    	$feeitemname=$feeitem['title'];
    	$totalAmount=$feeitem['totalamt'];
    	$order->line_items[] = uc_order_line_item_add(
    			$order->order_id, 'generic', $feeitemname, $totalAmount, _uc_line_item_data('cruise_fee_item', 'weight'), array('name' => 'FEE_ITEMS')
    	);
    }
    
    
   
    //giftcard
    $giftCardserialized=CartSession::getGiftCard($sessionid);
    
    if($giftCardserialized){
    	$giftCardArray=unserialize($giftCardserialized);
    	$giftCardCode='';
    	$giftCardAmount=0;
    	$totalgiftcardprice=0;
    	$deductedamount=0;
    	foreach($giftCardArray as $key=>$values){
    		$giftCardCode=$key;
    		$deductedamount=$giftCardArray[$key]['deductedamount'];
    		$totalgiftcardprice=$giftCardArray[$key]['totalgiftcardprice'];
    	}
    	$order->line_items[] = uc_order_line_item_add($order->order_id, 'generic', 'Gift Card ' . $giftCardCode, -($deductedamount), _uc_line_item_data('giftcard', 'weight'), array('name' => 'GIFT_CARD')
    	);
    }
    
    
    
    
    
    //Tax
    $taxamt = CartSession::getTaxAmt($sessionid);
    if ($taxamt != "") {
         $order->line_items[] = uc_order_line_item_add($order->order_id, 'generic', 'Tax ', ($taxamt), _uc_line_item_data('Tax', 'weight'), array('name' => 'TAX')
         );
    }
    
    
    
    
    
    
    //Coupon Code
    $couponLineItem=unserialize(CartSession::getPromocode($sessionid));
    
    foreach($couponLineItem as $key=>$val){
    	$cpncode=$key;
    	$cpndiscount=(float)$val;
  	$order->line_items[] = uc_order_line_item_add($order->order_id, 'generic', 'Promo Code ' . $cpncode, -($cpndiscount), _uc_line_item_data('coupon', 'weight'), array('name' => 'PROMO_CODE')
    	);
    }
    
    
    
    
    //Voucher codes
    $vouchercodesserializedobject=CartSession::getVoucherCodes($sessionid);
    if($vouchercodesserializedobject && $vouchercodesserializedobject!=""){
    	$vouchercodes=unserialize($vouchercodesserializedobject);
    	foreach($vouchercodes as $code=>$amt){


    		$order->line_items[] = uc_order_line_item_add($order->order_id, 'generic', 'Voucher' . $code, -($amt), _uc_line_item_data('coupon', 'weight'), array('name' => 'VOUCHER_CODE')
    		);
    	}
    }
    
  
    
    
    //Line Items
    //1 Charges
    $taxLineItemSerialized = CartSession::getTaxLineItem($sessionid);
    if ($taxLineItemSerialized) {
    	$taxLineItemArray = reset(unserialize($taxLineItemSerialized));
    		if ($taxLineItemArray['amt'] != "") {
    			$order->line_items[] = uc_order_line_item_add($order->order_id, 'generic', 'Additional Tax Line Item - ' . $taxLineItemArray['taxname'], ($taxLineItemArray['amt']), _uc_line_item_data('coupon', 'weight'), array('name' => 'ADDITIONAL_CHARGE_TAX')
    			);
    		}
    }
    
    
    //Line Item Discount
    $discountLineItemSerialized = CartSession::getDiscountLineItem($sessionid);
    if ($discountLineItemSerialized) {
    	$discountLineItemArray = reset(unserialize($discountLineItemSerialized));
    	if ($taxLineItemArray['amt'] != "") {
    		$order->line_items[] = uc_order_line_item_add($order->order_id, 'generic', 'Additional Discount Line Item - ' . $discountLineItemArray['discountname'], -($discountLineItemArray['amt']), _uc_line_item_data('coupon', 'weight'), array('name' => 'ADDITIONAL_DISCOUNT')
    		);
    	}
    }
    
    	
  
    //$order->line_items = uc_order_load_line_items($order);
   
    //Loading Billing Information(billing Details)
    $order->billing_first_name = $bookingagentinputs['PAYMENT']['billing_firstname'];
    $order->billing_last_name = $bookingagentinputs['PAYMENT']['billing_lastname'];
    $order->billing_company = null;
    $order->billing_street1 = $bookingagentinputs['PAYMENT']['billing_address'];
    $order->billing_street2 = $bookingagentinputs['PAYMENT']['billing_otheraddress'];
    
    
    $order->billing_postal_code = $bookingagentinputs['PAYMENT']['billing_zipcode'];
    $order->billing_city = $bookingagentinputs['PAYMENT']['billing_city'];
    $order->billing_zone = $bookingagentinputs['PAYMENT']['billing_state'];
    $order->billing_country = 840;
    $bareaCode = $bookingagentinputs['PAYMENT']['billing_areacode'];
    
    //for giftcard
    $barcode=preg_replace('/[()]/s', '', $bareaCode);
    
    
    $bprefix = $bookingagentinputs['PAYMENT']['billing_prefix'];
    $blineNumber = $bookingagentinputs['PAYMENT']['billing_linenumber'];
    $order->billing_phone = $bareaCode ." ". $bprefix . "-" . $blineNumber;
    $userphone=$barcode ."-". $bprefix . "-" . $blineNumber;
    $order->primary_email = $bookingagentinputs['PAYMENT']['billing_email'];
    $order->delivery_first_name = $bookingagentinputs['PAYMENT']['guest_firstname'];
    $order->delivery_last_name = $bookingagentinputs['PAYMENT']['guest_lastname'];
    $order->delivery_company = null;
    $order->delivery_street1 = $bookingagentinputs['PAYMENT']['guest_address'];
    $order->delivery_street2 = $bookingagentinputs['PAYMENT']['guest_otheraddress'];
    
    $order->delivery_postal_code = $bookingagentinputs['PAYMENT']['guest_zipcode'];
    $order->delivery_city = $bookingagentinputs['PAYMENT']['guest_city'];
    $order->delivery_zone = $bookingagentinputs['PAYMENT']['guest_state'];
    $order->delivery_country = 840;
    $areaCode = $bookingagentinputs['PAYMENT']['guest_areacode'];
    $prefix = $bookingagentinputs['PAYMENT']['guest_prefix'];
    $lineNumber = $bookingagentinputs['PAYMENT']['guest_linenumber'];
    $order->delivery_phone = $areaCode ." ".$prefix . "-" . $lineNumber;
    
     
    $totalAmt=CartSession::getOrderTotal($sessionid);
    $order->uid = $userId;

    //Case :if order total is 0 then
    if($transactionamt==0){
    	$result=true;
    }
    
    
    //do Payment
   
    $paymentMethodType=$bookingagentinputs['PAYMENT']['paymentMethodType'];
    $transactionamt=$totalAmt;
    $activitylogid='';
    global $user;
    if($paymentMethodType=="1"){
    	
    	//Loading the Payment Creditcard details information
    	$order->payment_method = 'credit';
    	$order->payment_details['cc_number'] = $bookingagentinputs['PAYMENT']['creditCardNumber'];
    	$order->payment_details['cc_exp_month'] = $bookingagentinputs['PAYMENT']['creditExpMonth'];
    	$order->payment_details['cc_exp_year'] = $bookingagentinputs['PAYMENT']['creditExpYear'];
    	$order->payment_details['cc_cvv'] = $bookingagentinputs['PAYMENT']['creditcid'];
    	$order->payment_details['cc_type'] = $bookingagentinputs['PAYMENT']['creditCardType'];
    	
    	db_update('uc_orders')
    	->fields(array('payment_method' => $order->payment_method))
    	->condition('order_id', $order->order_id)
    	->execute();

    	
    	$logdata = array ();
    	$logdata [] = date ( 'm/d/Y h:i:s a', time () ) . " " . $user->name . "Started Booking";
    	$logdata [] = "User ID " . $user->uid;
    	$logdata [] = "Order ID " . $order->order_id;
    	$logdata [] = "Chosen " . $total_ticketQty . " Tickets ";
    	$logdata [] = "Chosen " . $totalenhancementQty . " Enhancements";
    	$logdata [] = "Completed Personal Details and Credit Card Details";
    	$logdata [] = "Completed Review Order Operation";
    	$logdata [] = "Payment Processing Initiated";
    	
    	$userObject = new UserLog ();
    	$userObject->id = "";
    	$userObject->uid = $user->uid;
    	$userObject->status = "0";
    	$userObject->message = "Agent Booking";
    	$userObject->location = $_SERVER ["REQUEST_URI"];
    	$userObject->referer = "Agent Booking";
    	$userObject->data = serialize ( $logdata );
    	$userObject->type = "agentbooking";
    	$userObject->timestamp = time ();
    	$userObject->ipaddress = $_SERVER ['REMOTE_ADDR'];
    	
    	$activitylogid = UserLog::logUserActivity ( $userObject );
    	
    	//set the Payment Type
    	$data = array();
    	
    	watchdog('current amt entred', $transactionamt);
    	//$data['txn_type'] = UC_CREDIT_AUTH_CAPTURE;

    	$data['txn_type'] = UC_CREDIT_AUTH_CAPTURE;
    	$cc_data['cc_number'] = $order->payment_details['cc_number'];
    	$cc_data['cc_cvv'] = $order->payment_details['cc_cvv'];
    	$cc_data['cc_exp_month'] = $order->payment_details['cc_exp_month'];
    	$cc_data['cc_exp_year'] = $order->payment_details['cc_exp_year'];
    	
    	// Cache the values for use during processing.
    	uc_credit_cache('save', $cc_data, FALSE);
    	uc_order_save($order);
    	
    	
    	//Executing the Payment Process
    	$result = uc_payment_process_payment('credit', $order->order_id, $transactionamt, $data, TRUE, NULL, FALSE);
    	_uc_credit_save_cc_data_to_order($order->payment_details, $order->order_id);
    }else{
    	
    	
    	$order->payment_method = 'free_order';
    	$order->payment_details['cc_number'] = "";
    	$order->payment_details['cc_exp_month'] = "";
    	$order->payment_details['cc_exp_year'] = "";
    	$order->payment_details['cc_cvv'] = "";
    	$order->payment_details['cc_type'] = "";
    	uc_order_save($order);
    	
    	
    	$logdata = array ();
    	$logdata [] = date ( 'm/d/Y h:i:s a', time () ) . " " . $user->name . "Started Booking";
    	$logdata [] = "User ID " . $user->uid;
    	$logdata [] = "Order ID " . $order->order_id;
    	$logdata [] = "Chosen " . $total_ticketQty . " Tickets ";
    	$logdata [] = "Chosen " . $totalenhancementQty . " Enhancements";
    	$logdata [] = "Selected Free Order Payment method";
    	$logdata [] = "Completed Review Order Operation";
    	 
    	$userObject = new UserLog ();
    	$userObject->id = "";
    	$userObject->uid = $user->uid;
    	$userObject->status = "0";
    	$userObject->message = "Agent Booking";
    	$userObject->location = $_SERVER ["REQUEST_URI"];
    	$userObject->referer = "Agent Booking";
    	$userObject->data = serialize ( $logdata );
    	$userObject->type = "agentbooking";
    	$userObject->timestamp = time ();
    	$userObject->ipaddress = $_SERVER ['REMOTE_ADDR'];
    	 
    	$activitylogid = UserLog::logUserActivity ( $userObject );
    	
    	
    	
    	$comments="Free Order Created";
    	$received = REQUEST_TIME;
    	uc_payment_enter($order->order_id, "free_order", $transactionamt, $user->uid, '', $comments, $received);
    	$result=true;
    	uc_order_save($order);
    }

    
    
    if ($result) {
        uc_cart_complete_sale($order, $login = FALSE);
        
        db_update('uc_orders')
        ->fields(
        array('order_total' => $transactionamt,
        'order_status' => 'payment_received'))
        ->condition('order_id', $order->order_id)
        ->execute();
        
        //User Activity Log
        $userObject=new UserLog();
        $userObject->id=$activitylogid;
        $userObject->status="1";
        $logdata[]="Payment Success";
        $logdata[]=date('m/d/Y h:i:s a', time()). " Order Completed Successfully";
        $userObject->data=serialize($logdata);
        $userObject->data=serialize($logdata);
        $activitylogid=UserLog::logUserActivity($userObject);
        //drupal_json_output($userObject);
        
        
        
        /*
         * Gift Card
        */
        $giftCardSerialized = CartSession::getGiftCard($sessionid);
        
     
        $giftCardCode = '';
        $giftCardAmount = '';
        
        if (!empty($giftCardSerialized)) {
        	$giftCardArray = unserialize($giftCardSerialized);
        
        	foreach ($giftCardArray as $key => $values) {
        		$giftCardCode = $key;
        		$giftCardTotalAmt = $giftCardArray[$key]['totalgiftcardprice'];
        		$giftCardAmount = $giftCardArray[$key]['deductedamount'];
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
        
        
        //Groupons Update
        $vouchercodesserialized = CartSession::getVoucherCodes($sessionid);
        if ($vouchercodesserialized) {
        	$vouchercodes = unserialize($vouchercodesserialized);
        	foreach ($vouchercodes as $code => $amt) {
        		try{
        			$num_updated = db_update('deal_grouponcodes')
        			->fields(array(
        					'is_used' => '1',
        			))
        			->condition('groupon_code', $code, '=')
        			->execute();
        		}catch(Exception $e){
        
        		}
        
        	}
        }
        unset($_SESSION['order_groupons']);
        
        
        //Booking Log
    	foreach($selectedtickets as $ticket){
    	$ticketObj=new Tickets();
    	$ticketObj-> ticket_id=$ticket['ticketid'];
    	$ticketObj-> order_id=$order->order_id;
    	$ticketObj-> ticket_qty=$ticket['qty'];
    	$ticketObj-> ticket_price=$ticket['price'];
    	$ticketObj-> event_id=$_SESSION['bookingInput']->eventid;
    	$ticketObj-> bookingtype="agent";
    	$id=BookingLog::logTickets($ticketObj);
    	for($i=0;$i<count($ticket['ticketentreeids']);$i++){
    		$ticketEntreesObj=new TicketEntrees();
    		$ticketEntreesObj-> entree_id=(int)$ticket['ticketentreeids'][$i];
    		$ticketEntreesObj->ref_id=$id;
    		$commentArray=explode('-', $ticket['ticketentreescomments'][$i]);
    		$ticketEntreesObj->comments=isset($commentArray[2]) ? $commentArray[2] : '';
    		
    		//var_dump($ticketEntreesObj);
    		BookingLog::logTicketEntrees($ticketEntreesObj);
    	}
    }
    
   
     
    
    //Enhancements Log
    foreach($selectedenhancements as $enhancement){
    $enhancementObj=new Enhancements();
	$enhancementObj-> comments=$enhancement['comments'];
	$enhancementObj-> enhancement_id=$enhancement['enhancedproductnodeid'];
	$enhancementObj-> enhancement_price=$enhancement['price'];
	$enhancementObj-> enhancement_qty = $enhancement['qty'];
	$enhancementObj-> event_id=$_SESSION['bookingInput']->eventid;
	$enhancementObj->order_id=$order->order_id;
	BookingLog::logEnhancements($enhancementObj);
    }
    
      
    
    //Userchoices Log
    
    $userChoicesObj=new UserChoices();
   
	$userChoicesObj->celebration_comments=$bookingagentinputsinglearray ['occasionadditionalcomments'];
	$userChoicesObj->celebration_flag=($bookingagentinputsinglearray ['occasiontype']!='') ? 1 : 0;
	$userChoicesObj->receive_email_notification_flag=($bookingagentinputsinglearray['promotionaldiscountsenable']!='') ? 1 : 0;
	$userChoicesObj->customer_email=$bookingagentinputsinglearray ['guest_email'];
	$userChoicesObj-> event_id=$_SESSION['bookingInput']->eventid;
	$userChoicesObj->food_allergy_flag=($bookingagentinputsinglearray ['foodallergy']!='') ? 1 : 0;
	$userChoicesObj->limited_mobility_flag=($bookingagentinputsinglearray ['limitedmobility']!='') ? 1 :0;
	$userChoicesObj-> another_party_comments=$bookingagentinputsinglearray ['additionalcomments'];
	$userChoicesObj->another_party_flag=($bookingagentinputsinglearray ['anotherparty']) ? 1 : 0;
	$userChoicesObj->what_they_brought="";
	$userChoicesObj->order_id=$order->order_id;
	$userChoicesObj->order_total=$transactionamt;
	$userChoicesObj->referal = $bookingagentinputsinglearray['findingusby'];
	$userChoicesObj->uid=$userId;
	
	
	
	BookingLog::logUserChoices($userChoicesObj);
	

        //After sucessfule completion for bookingagent sale 
        /**
         * Creating the bookingagent Entity System
         * 
         * 
         * 
         */
        //bookingagent Entity information gets loads here




        $usercruiseinputs = array(
        	'cartsessionid'=>array($sessionid),
            'entrees' => array(), //$cartenhancedproduct,//Storing the complete selected enhancement product object
            'Dietary' => array(), //To DO
            'cruiseproduct' => array('producttickets' => $selectedtickets,
                'cruiseitem' => $_SESSION['bookingInput']->eventid), //format:: -->"attrid": "2","qty": "1","optionid": "3","3-price": "69.00"
            'enhacementproducts' => $selectedenhancements,
            'specialinstructions' => array(
                'limitedmobility' => ($bookingagentinputsinglearray ['limitedmobility']!='') ? 1 :0,
                'foodallergy' => ($bookingagentinputsinglearray ['foodallergy']!='') ? 1 : 0,
                'anotherparty' => ($bookingagentinputsinglearray ['anotherparty']) ? 1 : 0,
                'specialcomments' => $bookingagentinputsinglearray ['additionalcomments']
            ),
            'celebrationtext' => array(
            
                'ocassiontypeenabled' => ($bookingagentinputsinglearray ['occasiontype']!='') ? 1 : 0,
                'ocassionaldescription' => $bookingagentinputsinglearray['occasionadditionalcomments']),
            'usersdata' => array(
                'email' => $bookingagentinputsinglearray['guest_email'], //treating the default email
                //'uid' => 0, //To DO replace with accountId
                'uid' => $userId,
                'dob' => $guestDOB,
                'aniversary' => $guestAniverssary,
                'findingusby' => $bookingagentinputsinglearray['findingusby'],
                'waterwayspromotionsAnddiscounts' => ($bookingagentinputsinglearray['promotionaldiscountsenable']!='') ? 1 : 0,
                'receiveemailnotifications'=>($bookingagentinputsinglearray['promotionaldiscountsenable']!='') ? 1 : 0,
            ),
            
        );
        
        $userchioce = (object) array('userchoice' => $usercruiseinputs);

        
        global $user;
        if (in_array('administrator', $user->roles)) {
            $loggedUserRole = 'administrator';
        } else if (in_array('agent', $user->roles)) {
            $loggedUserRole = 'agent';
        } else if (in_array('staff admin', $user->roles)) {
            $loggedUserRole = 'staff admin';
        }
        
        $createdUserid = '';
        if ($user->uid) {
            $createdUserid = $user->uid;
        } else {
            $createdUserid = $userId;
        }
        $reservationObj = new stdClass();

//        $reservationObj->bookingagentdate = REQUEST_TIME;
        $reservationObj->bookingdate = strtotime($_SESSION['bookingInput']->date);
        $reservationObj->orderid = $order->order_id;
        $reservationObj->uid = $createdUserid;
        $reservationObj->bookedby = $loggedUserRole; //Agent or Admin,Web Information
        $reservationObj->data = $userchioce; //using searilazed Object for maintaing the exact array structure default serialized structure maintaing db
        $reservationObj->created = REQUEST_TIME;
        $reservationObj->eventid=$_SESSION['bookingInput']->eventid;
        
        $savereservation = reservation_save($reservationObj);
        
        
        //Email Template Notification
        $datestamp=Scheduledevents::getEventDateNTimeTimeStamp($_SESSION['bookingInput']->eventid);
        $emailNotificationObject = new EmailTemplateNotification ();
        $emailNotificationObject->user_id = $userId;
        $emailNotificationObject->cruise_template_id =  $_SESSION['bookingInput']->templateid;
        $emailNotificationObject->schedule_event_id = $_SESSION['bookingInput']->eventid;
        $emailNotificationObject->schedule_event_date = $datestamp;
        $emailNotificationObject->order_id=  $order->order_id;
        $emailNotificationObject->emailNotification ();
        
        
        //Sending Email to Guest User
        $params = array(
        		'subject' => t('Your Order at WATERWAYS'),
        		'body' => t("Data"),
        		'orderId' => $order->order_id
        );
        $to = $order->primary_email;
        $from=SiteConfiguration::getConfirmationEmail();
        
        
        drupal_mail("email_system", "wwc_order_invoice_template", $to, language_default(), $params, $from);
        
        
       
        if ($savereservation) {
        	
        	$totalTickets=0;
        	$selectedTickets=unserialize(CartSession::getTickets($sessionid));
        	foreach($selectedTickets as $ticket){
        		if($ticket['qty']){
        			$totalTickets+=$ticket['qty'];
        		}
        	}
        	 
        	$totalWindows=0;
        	$enhancementserialized=CartSession::getEnhancements($sessionid);
        	 
        	if($enhancementserialized){
        		$enhancementproducts=unserialize($enhancementserialized);
        	}
        	$cnt=0;
        	foreach($enhancementproducts as $product){
        		if(CruiseEnhancements::is_window_inventory_item($product['enhancedproductnodeid'])){
        			$totalWindows+=$product['qty'];
        		}
        	}
        	 
        	//
        	 
        	//Note : updating the inventory
        	 
        	$inputbookingdata = $_SESSION['bookingInput'];
        	$cruiseTempateID=$inputbookingdata->templateid;
        	$eventID=$inputbookingdata->eventid;
        	$eventDate=$inputbookingdata->date;
        	
        	Scheduledevents::updateInventory($cruiseTempateID,$eventID,$totalTickets, $totalWindows);
        	
        	
            //update the stock information
//             $ordersesult = getStockUpdateByOrder($order);
//             if ($ordersesult) {
//                 watchdog('bookingagentstock', 'Updated sucessfully for ' . $order->order_id);
//             } else {
//                 watchdog('bookingagentstock', 'Updation failed for ' . $order->order_id);
//             }
            //end of stock updation
            return array('currentorder' => $order->order_id, 'bookingagentresult' => $result);
        }
		//End of bookingagent information
    }else{
    	//User Activity Log
    	$userObject=new UserLog();
    	$userObject->id=$activitylogid;
    	$userObject->status="0";
    	$logdata[]="Payment Failed";
    	$logdata[]=date('m/d/Y h:i:s a', time()). " Order incompleted";
    	$userObject->data=serialize($logdata);
    	$activitylogid=UserLog::logUserActivity($userObject);
    }
}


