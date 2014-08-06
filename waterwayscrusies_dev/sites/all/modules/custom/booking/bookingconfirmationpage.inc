<?php
function bookingconfirmation($orderid, $cartId) {
	drupal_add_css(drupal_get_path('module', 'booking') . '/css/booking-mediaqueries.css');
	drupal_add_css(drupal_get_path('module', 'booking') . '/css/booking.css');
	
	$cartData='';
	if($cartId){
		//$bookingCart= new BookingCart();
		//$bookingCart->emptyCart();
	}
	
	$cartData=unserialize(BookingCart::getCartDBData($cartId, $orderid));

	$eventId=$cartData['bookingInput']->eventid;
	$eventDate=$cartData['bookingInput']->date;
	$templateid=$cartData['bookingInput']->templateid;

	//Redirecting the user if the User Refresh Page, Otherwise somedivs content is missing,
// 	if (!isset($_SESSION['formstage'])) {
// 		global $base_url;
// 		drupal_goto($base_url);
// 		exit();
// 	}


	$orderdetails = @uc_order_load($orderid);
	//cruisetemplate details

	$templateid=Scheduledevents::getCruiseTemplateIdByEventId($eventId);
	$templatedetails=cruise_load($templateid);



	//Billing and Payments information
	$data = array();
	$data['eventname'] = '';
	$data['portid'] = '';
	$data['departuretime'] = '';
	$data['returntime'] = '';
	$data['boardingtime'] = '';
	$data['eventdate'] = '';
	$data['portname'] = '';
	$data['portdescription'] = '';
	$data['portColor'] = '';
	$data['portAddress'] = '';
	$data['portState'] = '';
	$data['portCity'] = '';
	$data['portPostalCode'] = '';
	$data['parkingdownloadpdf'] = '';
	$data['cruisetemplatename']='';


	$scheduledEventObject = scheduledevents_load($eventId);
	if (!is_null($scheduledEventObject) && isset($scheduledEventObject)) {
		$data['eventname'] = $scheduledEventObject->name;
		$data['cruisetemplatename']=$templatedetails->cruise_title;
		$data['portid'] = $scheduledEventObject->port_id;
		$data['departuretime'] = $scheduledEventObject->departuretime;
		$data['returntime'] = $scheduledEventObject->returntime;
		$data['boardingtime'] = $scheduledEventObject->boardingtime;
		$eventTimeUnix = strtotime($eventDate);
		$eventdate = date("F d, Y", $eventTimeUnix);
		$data['eventdate'] = $eventdate;

		//Port Details
		$portData = loadportById($data['portid']);

		$data['portname'] = $portData['portname'];
		$data['portdescription'] = $portData['portdescription'];
		$data['portColor'] = $portData['portColor'];
		$data['portAddress'] = $portData['portAddress'];
		$data['portState'] = $portData['portState'];
		$data['portCity'] = $portData['portCity'];
		$data['portPostalCode'] = $portData['portPostalCode'];
		$data['parkingdownloadpdf'] = $portData['parkingdownloadpdf'];
	}

	$parkingpdf = $data['parkingdownloadpdf'];
	//$parkingImg = $cruisedetails['parkingImg'];
	//$parkingDirections = $cruisedetails['directions'];


	$cruisehtml = '<div class="cruiseDetailsInnerbox">';
	$cruisehtml.='<div class="row-fluid package-heading">';
	$cruisehtml.='<div class="span2 sunsetLinePadding"><div class="line"></div></div>';
	$cruisehtml.='<div class="span8"><div class="sliderCaption">' . $data['cruisetemplatename'] . '</div></div>';
	$cruisehtml.='<div class="span2 sunsetLinePadding"><div class="line"></div></div>';
	$cruisehtml.='</div>';
	$cruisehtml.='<div class="row-fluid">';
	$cruisehtml.='<div class="span6 customspan6">';
	$cruisehtml.='<h3 class="bookingheader2">' . $data['eventdate'] . '</h3>';
	$cruisehtml.='<p class="bookingheader2">Departure Port: <br>' . $data['portname'] . '</p>';
	$cruisehtml.='</div>';
	$cruisehtml.='<div class="span3 customdepart">';
	$cruisehtml.= '<h3 class="bookingheader2" style="background: url(' . base_path() . path_to_theme() . '/assets/img/dot.png) no-repeat right 12px; padding-right: 5px;">Depart ' . $data['departuretime'] . '</h3>';
	$cruisehtml.='</div>';
	$cruisehtml.='<div class="span3 customreturn">';
	$cruisehtml.='<h3 class="bookingheader2">Return ' . $data['returntime'] . '</h3>';
	$cruisehtml.='</div>';
	$cruisehtml.='</div>';
	$cruisehtml.='<div class="row-fluid textmarginTop">';
	$cruisehtml.='<div class="span12">';
	$cruisehtml.='<p>Your cruise departs from <b>' . $data['portname'] . '</b> <br/> <b>' . $data['portAddress'] . '</b> <b>' . $data['portCity'] . '' . $data['portState'] . ' ' . $data['portPostalCode'] . '</b><br/>';
	$cruisehtml.='<i>Located on the south end of Lake Union in downtown Seattle</i>';
	$cruisehtml.='</p>';
	$cruisehtml.='<p class="textmarginTop"><i>*Please see Map & Directions for address, directions and parking information. Please arrive at least 30 minutes prior to your cruises departure time, to allow time for parking and boarding. Boarding starts 30 minutes prior to departure.</i></p>';
	$cruisehtml.='</div>';
	$cruisehtml.='</div>';
	$cruisehtml.='<div class="row-fluid" id="routeMaplinesholder">';
	$cruisehtml.='<div class="span5 "><div class="line"></div></div>';
	$cruisehtml.='<div class="span2" align="center"><img src="' . base_path() . path_to_theme() . '/assets/img/banner-icon-img.png"></div>';
	$cruisehtml.='<div class="span5 "><div class="line"></div></div>  </div> </div>';





	$creditcard = '<div class="span6">';
	$creditcard .= '<div class="bookingheader3">Credit Card Details</div>';
	$creditcard .= '<div class="paymentUserDetails textmarginTop">';
	$creditcard .= '<h5>*Card Type:</h5>';
	$creditcard .= '<h5 class="paymentUsernames">' . ucfirst($orderdetails->payment_details['cc_type']) . '</h5>';
	$creditcard.='</div>';
	$creditcard.='<div class="paymentUserDetails">';
	$creditcard.='<h5>*Card Number:</h5>';
	$creditcard.='<h5 class="paymentUsernames">**** *** *** ' . $orderdetails->payment_details['cc_number'] . '</h5>';
	$creditcard.='</div>';



	$creditcard.='<div class="paymentUserDetails">';
	$creditcard.='<h5>*Expiration Date:</h5>';
	$creditcard.='<h5 class="paymentUsernames">' . $orderdetails->payment_details['cc_exp_month'] . '/' . $orderdetails->payment_details['cc_exp_year'] . '</h5>';
	$creditcard.='</div>';
	$creditcard.='</div>';




	//Ticket Widget gets here
	//     $ticketshtml = '<table cellpadding="0" cellspacing="0" border="0">';
	//     $ticketshtml.='<tr>';
	//     $ticketshtml.='<td class="bookingheader2 quantityHeader">Quantity</td>';
	//     $ticketshtml.='<td class="bookingheader2 ticketHeader">Ticket</td>';
	//     $ticketshtml.='<td class="bookingheader2 entreeHeader">Entree</td>';
	//     $ticketshtml.='<td class="bookingheader2 entreeCheckBoxHeader"></td>';
	//     $ticketshtml.='<td class="bookingheader2 priceHeader">Price</td>';
	//     $ticketshtml.='</tr>';


	$ticketshtml = '';
	$ticketshtml .= '<div class="row-fluid">';
	$ticketshtml .= '<div class="span2 quantityHeader"><b>Quantity</b></div>';
	$ticketshtml .= '<div class="span3 ticketHeader"><b>Ticket</b></div>';
	$ticketshtml .= '<div class="span3 entreeHeader"><b>Entree</b></div>';
	$ticketshtml .= '<div class="span2 entreeCheckBoxHeader"></div>';
	$ticketshtml .= '<div class="span2 priceHeader"><b>Price</b></div>';
	$ticketshtml .= '</div>';


	$ticketsresult = array();


	//********************** TICKETS ******************************
	
	//$cartData
	
	$ticketsdata=$cartData['ticketsList'];
	for($i=0;$i<count($ticketsdata);$i++){
		$ticketId=$ticketsdata[$i]['ticketid'];
		$noOfTickets=(int) $ticketsdata[$i]['qty'];
		if($noOfTickets>0){
			$price = $ticketsdata[$i]['price'];
			
			if(!empty($ticketsdata[$i]['grouponcode'])){
				$price=0;
			}
			
			$ticketName=cruisetickets_load($ticketsdata[$i]['ticketid'])->title;
			
			$id = $ticketsdata[$i]['entree'];
			$cruiseEntreeObject = cruiseentrees_load($id);
			$entreeName = @$cruiseEntreeObject->title;
			$comment='';
			if(!empty($ticketsdata[$i]['comments'])){
				$comment = '<p class="ticketdesc"><b>Dietary restrictions:</b> <br/>' . $ticketsdata[$i]['comments'] . '</p>';
				
			}
			$ticketshtml.='<div class="row-fluid">';
			$ticketshtml .= '<div class="span2 quantityClass">1</div>';
			$ticketshtml .= '<div class="span2 ticketClass">' . $ticketName . '</div>';
			$ticketshtml .= '<div class="span3 entreeClass">' . $entreeName . '</div>';
			$ticketshtml .= '<div class="span3 entreeCheckBoxClass">' . $comment . '</div>';
			$ticketshtml .= '<div class="span2 priceClass">$' . wwcFormatPrice($price) .'</div>';
			$ticketshtml .= '</div>';

		}

	}



	$specialcomment=isset($cartData['user']['additionalcomments']) ? $cartData['user']['additionalcomments'] : '';
	$occasionalcomments=isset($cartData['user']['occasionadditionalcomments']) ? $cartData['user']['occasionadditionalcomments'] : '';
	$ticketshtml.='<table><tr>';
	$ticketshtml.='<td colspan="5">';
	$ticketshtml.='<div class="textmarginTop">';
	$ticketshtml.='<textarea placeholder="Special Concerns..." rows="5" class="forminputCL" readonly>' . $specialcomment . '</textarea>';
	$ticketshtml.='<textarea placeholder="Comments..." rows="5" class="forminputCL forminputCLRight" readonly>' . $occasionalcomments . '</textarea>';
	$ticketshtml.='</div>';
	$ticketshtml.='</td>';
	$ticketshtml.='</tr>';
	$ticketshtml.='</table>';


	//voucher codes
	$vouchersHtml='';
	$vouchercodes=$cartData['vouchers'];
	$v_codes=array();
	for($i=1;$i<=count($vouchercodes);$i++){
		if(isset($vouchercodes[$i])){
			if(!empty($vouchercodes[$i]['code'])){
				$v_codes[]=$vouchercodes[$i]['code'];
			}
		}
	
	}
	if(count($v_codes)>0){
		$codes=implode(',', $v_codes);
		$vouchersHtml.='<div class="chargesWrapper">';
		$vouchersHtml.='<h3 class="chargesWrapperLeft bookingheader3">Voucher Codes :'.$codes.'</h3>';
		$vouchersHtml.='</div>';
	}



	//********************** Enhancements ******************************
	//$ticketsdata=unserialize(CartSession::getTickets($sessionid));
	$enhancementData=$cartData['enhancementsList'];



	$enhancementhtml='';
	if(count($enhancementData)>0){
		$enhancementhtml.= '<div class="row-fluid">';
		$enhancementhtml.='<div class="span2 quantityHeader"><b>Quantity</b></div>';
		$enhancementhtml.='<div class="span3 ticketHeader"><b>Enhancement Name</b></div>';
		$enhancementhtml.='<div class="span3 entreeHeader commentsClass"><b>Comments</b></div>';
		$enhancementhtml.='<div class="span3 entreeCheckBoxHeader unitPriceClass"><b>Unit price</b></div>';
		$enhancementhtml.='<div class="span1 priceHeader"><b>Price</b></div>';
		$enhancementhtml.='</div>';
		$ticketsresult = array();
		 
		for($i=0;$i<count($enhancementData);$i++){
			$totalPrice=$enhancementData[$i]['qty']*$enhancementData[$i]['price'];
			$enhancementhtml.='<div class="row-fluid">';
			$enhancementhtml .= '<div class="span2 quantityClass">'.$enhancementData[$i]['qty'].'</div>';
			$enhancementhtml .= '<div class="span3 ticketClass">' . $enhancementData[$i]['name'] . '</div>';
			$enhancementhtml .= '<div class="span3 entreeClass commentsClass">'  . $enhancementData[$i]['comments'] . '</div>';
			$enhancementhtml .= '<div class="span3 entreeCheckBoxClass unitPriceClass"> $'  .wwcFormatPrice($enhancementData[$i]['price']).'</div>';
			$enhancementhtml .='<div class="span1 priceClass">$' . wwcFormatPrice($totalPrice) . '</div>';
			$enhancementhtml .= '</div>';
			$ticketId=$ticketsdata[$i]['ticketid'];
			$noOfTickets=(int) $ticketsdata[$i]['qty'];
		}
	}

	 
	//*******************   CRUISE FEE  ***********************
	$chargeswidget= $vouchersHtml.''.'<div class="span7 printcssfee">';
	$cruiseFeeItems=$cartData['ticketsFeeItemsList'];
	$totalCruisefeeAmt=0;
	foreach($cruiseFeeItems as $item){
		$feeName=$item['title'];
		$price = $item['totalamt'];
		if($price!="" || $price!=0){
			$calamt =wwcFormatPrice($price);
			$chargeswidget.='<div class="chargesWrapper">';
			$chargeswidget.='<h3 class="chargesWrapperLeft bookingheader3">' . $feeName . '</h3><span></span><h3 class="chargesWrapperRight bookingheader3">$'. $calamt . '</h3>';
			$chargeswidget .= '</div>';
		}
	}



	//*************************** COUPON *********************************
	$cpn=$cartData['promocode'];
	$cpncode=false;
	$cpndiscount=false;
	if(@is_array($cpn)){
		foreach($cpn as $key=>$value){
			$cpncode=$key;
			$cpndiscount=$value;
		}
	}
	 
	if ($cpndiscount!="" || $cpndiscount!=0 ) {
		$calamt =wwcFormatPrice($cpndiscount);
		$chargeswidget .= '<div class="chargesWrapper">';
		$chargeswidget .= '<h3 class="chargesWrapperLeft bookingheader3">Promocode</h3><span></span><h3 class="chargesWrapperRight bookingheader3">-$' . $calamt . '</h3>';
		$chargeswidget.='</div>';
	}


	//*************************** GIFT CARD *********************************
	$giftCardArray=$cartData['giftcard'];
	$giftCardCode='';
	$giftCardAmount='';
		 
	foreach($giftCardArray as $key=>$values){
		if ($giftCardArray[$key]['deductedamount']!="") {
		$giftCardCode=$key;
		$calamt =wwcFormatPrice($giftCardArray[$key]['deductedamount']);
		$chargeswidget.='<div class="chargesWrapper">';
		$chargeswidget.='<h3 class="chargesWrapperLeft bookingheader3">Gift Card</h3><span></span><h3 class="chargesWrapperRight bookingheader3">-$' . $calamt . '</h3>';
		$chargeswidget.='</div>';
	}
  }
 



	
	
		

	//*************************** SUB TOTAL *********************************
	$orderSubTotal=$cartData['orderSubTotal'];
	$calamt =wwcFormatPrice($orderSubTotal);
	$chargeswidget.='<div class="chargesWrapper">';
	$chargeswidget.='<h3 class="chargesWrapperLeft bookingheader3">Subtotal</h3><span></span><h3 class="chargesWrapperRight bookingheader3">$' . $calamt . '</h3>';
	$chargeswidget.='</div>';


	//Tax
	$totalTaxamt=$cartData['taxAmount'];
	$calamt =wwcFormatPrice($totalTaxamt);
	$chargeswidget.='<div class="chargesWrapper">';
	$chargeswidget.='<h3 class="chargesWrapperLeft bookingheader3">Tax</h3><span></span><h3 class="chargesWrapperRight bookingheader3">$' . $calamt . '</h3>';
	$chargeswidget.='</div>';



	//total amount
	$orderTotal=$cartData['orderToalAmount'];
	$calamt =wwcFormatPrice($orderTotal);

	$chargeswidget.='<div class="row-fluid textmarginTop">';
	$chargeswidget.='<div class="span12" align="right">';
	$chargeswidget.='<span class="bookingheader1">Total</span>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="bookingheader1">$' . $calamt . '</span>';
	$chargeswidget.='</div>';
	$chargeswidget.='</div>';
	$chargeswidget.='</div>';



	//$guestdob=Reservation :: reservation_get_user_personal_information($orderid)['dob'];
	//$guestdobArray=Reservation :: reservation_get_user_personal_information($orderid);
	
	$guestdob='';
	$guest_dob_month=$cartData['user']['info']['guest_dob_month'];
	$guest_dob_day=$cartData['user']['info']['guest_dob_day'];
	
	if(!empty($guest_dob_month) && !empty($guest_dob_day) ){
		$guestdob=$guest_dob_month."-".$guest_dob_day;
	}
	

	

	$templateresouces = array(
			'orderid' => $orderid,
			'cruiseboxwidget' => $cruisehtml,
			'billingwidget' => addressfilteration($orderdetails, 'billing', $guestdob),
			'guestwidget' => addressfilteration($orderdetails, 'guest', $guestdob),
			'creditwidget' => $creditcard,
			'ticketswidget' => $ticketshtml,
			'enhancementwidget'=>$enhancementhtml,
			'subcharges' => $chargeswidget,
			'parkingpdf' => $parkingpdf,
			'directions' => @$parkingDirections,
			'parkingImg' => @$parkingImg,
			'cruiseportid' => $data['portid'],
			'cruiseportname' => @$cruisePortname,
			'cruiseportaddrs' =>@ $cruisePortAddress,
			'cruiseportpdfdownload' => @$cruisePortDownloadpdf
	);

	unset($_SESSION['formstage']);
	return theme('bookingconfirmation', $templateresouces);

}



/*
 * Input Paramts Orderid and Cartsession Id
*/


function addressfilteration($order, $type, $guestdob) {


	switch ($type) {
		case 'billing':
			//Guest Details
			$addressthtml = '<div class="span6 cpaddContainer billingmargintop">';
			$addressthtml.='<div class="bookingheader3">Billing Address</div>';
			$addressthtml.='<div class="paymentUserDetails textmarginTop">';
			$addressthtml.='<h5>*First Name:</h5>';
			$addressthtml.='<h5 class="paymentUsernames">' . $order->billing_first_name . '</h5>';
			$addressthtml.='</div>';
			$addressthtml.='<div class="paymentUserDetails">';
			$addressthtml.='<h5>*Last Name:</h5>';
			$addressthtml.='<h5 class="paymentUsernames">' . $order->billing_last_name . '</h5>';
			$addressthtml.='</div>';


			$addressthtml.='<div class="paymentUserDetails">';
			$addressthtml.='<h5>*Address 1:</h5>';
			$addressthtml.='<h5 class="paymentUsernames">' . $order->billing_street1 . '</h5>';
			$addressthtml.='</div>';


			if(!empty($order->billing_street2)){
				$addressthtml.='<div class="paymentUserDetails">';
				$addressthtml.='<h5>&nbsp; Address 2:</h5>';
				$addressthtml.='<h5 class="paymentUsernames">' . $order->billing_street2 . '</h5>';
				$addressthtml.='</div>';
			}




			$addressthtml.='<div class="paymentUserDetails">';
			$addressthtml.='<h5>*City:</h5>';
			$addressthtml.='<h5 class="paymentUsernames">' . $order->billing_city . '</h5>';
			$addressthtml.='</div>';
			$addressthtml.='<div class="paymentUserDetails">';
			$addressthtml.='<h5>*State/province:</h5>';
			$addressthtml.='<h5 class="paymentUsernames">' . uc_zone_get_by_id($order->billing_zone) . '</h5>';
			$addressthtml.='</div>';
			$addressthtml.='<div class="paymentUserDetails">';
			$addressthtml.='<h5>*Zip:</h5>';
			$addressthtml.='<h5 class="paymentUsernames">' . $order->billing_postal_code . '</h5>';
			$addressthtml.='</div>';


			$addressthtml.='<div class="paymentUserDetails">';
			$addressthtml.='<h5>*Phone Number:</h5>';
			$addressthtml.='<h5 class="paymentUsernames">' . $order->billing_phone . '</h5>';
			$addressthtml.='</div>';
			$addressthtml.='<div class="paymentUserDetails">';
			$addressthtml.='<h5>*Email Address:</h5>';
			$addressthtml.='<h5 class="paymentUsernames">' . $order->primary_email . '</h5>';
			$addressthtml.='</div>';
			//$addressthtml.='<div class="paymentUserDetails">';
			//$addressthtml.='<h5>*Birth Date:</h5>';
			//$addressthtml.='</div>';
			$addressthtml.='</div>';

			return $addressthtml;
			break;
		case 'guest':

			$guestmail='';
			$account=user_load($order->uid);
			$guestmail=isset($account->mail) ? $account->mail : '';

			$addressthtml = '<div class="span6 cpaddContainer">';
			$addressthtml.='<div class="bookingheader3">Guest Details</div>';
			$addressthtml.='<div class="paymentUserDetails textmarginTop">';
			$addressthtml.='<h5>*First Name:</h5>';
			$addressthtml.='<h5 class="paymentUsernames">' . $order->delivery_first_name . '</h5>';
			$addressthtml.='</div>';
			$addressthtml.='<div class="paymentUserDetails">';
			$addressthtml.='<h5>*Last Name:</h5>';
			$addressthtml.='<h5 class="paymentUsernames">' . $order->delivery_last_name . '</h5>';
			$addressthtml.='</div>';
			$addressthtml.='<div class="paymentUserDetails">';
			$addressthtml.='<h5>*Address 1:</h5>';
			$addressthtml.='<h5 class="paymentUsernames">' . $order->delivery_street1 . '</h5>';
			$addressthtml.='</div>';


			if(!empty($order->delivery_street2)){
				$addressthtml.='<div class="paymentUserDetails">';
				$addressthtml.='<h5>&nbsp; Address 2:</h5>';
				$addressthtml.='<h5 class="paymentUsernames">' . $order->delivery_street2 . '</h5>';
				$addressthtml.='</div>';
			}


			$addressthtml.='<div class="paymentUserDetails">';
			$addressthtml.='<h5>*City:</h5>';
			$addressthtml.='<h5 class="paymentUsernames">' . $order->delivery_city . '</h5>';
			$addressthtml.='</div>';
			$addressthtml.='<div class="paymentUserDetails">';
			$addressthtml.='<h5>*State/province:</h5>';
			$addressthtml.='<h5 class="paymentUsernames">' . uc_zone_get_by_id($order->delivery_zone) . '</h5>';
			$addressthtml.='</div>';
			$addressthtml.='<div class="paymentUserDetails">';
			$addressthtml.='<h5>*Zip:</h5>';
			$addressthtml.='<h5 class="paymentUsernames">' . $order->delivery_postal_code . '</h5>';
			$addressthtml.='</div>';
			$addressthtml.='<div class="paymentUserDetails">';
			$addressthtml.='<h5>*Phone Number:</h5>';
			$addressthtml.='<h5 class="paymentUsernames">' . $order->delivery_phone . '</h5>';
			$addressthtml.='</div>';
			$addressthtml.='<div class="paymentUserDetails">';
			$addressthtml.='<h5>*Email Address:</h5>';
			$addressthtml.='<h5 class="paymentUsernames">' . $guestmail . '</h5>';
			$addressthtml.='</div>';


			//$addressthtml.='<h5 class="paymentUsernames">02/02/1986</h5>';

			if($guestdob!=""){
				$addressthtml.='<div class="paymentUserDetails">';
				$addressthtml.='<h5>Birth Date:</h5>';
				$dayandMonth=explode('-',$guestdob);
				$month=$dayandMonth[0];
				$day=$dayandMonth[1];
					
				$result["month"] = $month;
				$monthNum = sprintf("%02s", $result["month"]);
				$timestamp = mktime(0, 0, 0, $monthNum, 10);
				$monthName = date("F", $timestamp);
				$dob=$monthName."-".$day;
					
				$addressthtml.='<h5 class="paymentUsernames">' . $dob. '</h5>';
				$addressthtml.='</div>';
					
			}


			$addressthtml.='</div>';




			return $addressthtml;
			break;

		default:
			break;
	}
}