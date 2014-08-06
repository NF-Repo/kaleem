<?php


//Generates a header which informs the user of which stage they're on.
function bookingagent_get_header($form, &$form_state) {
	drupal_set_title('');
    
    $form_state['stage'] = isset($form_state['stage']) ? $form_state['stage'] : 1;
    $form_stages = array(
        'TICKETS' => 1,
        'ENHANCEMENTS' => 2,
        'PAYMENT' => 3,
        'REVIEW_ORDER' => 4,
    );

    if (isset($form_stages[$form_state['stage']])) {
        $current_step = $form_stages[$form_state['stage']];
    } else {
        $current_step = 1;
    }

    $stages = array(
        1 => array('data' => 'TICKETS'),
        2 => array('data' => 'ENHANCEMENTS'),
        3 => array('data' => 'PAYMENT'),
        4 => array('data' => 'REVIEW_ORDER'),
    );

    $stages[$current_step]['class'] = array('active');


    $attributes = array(
        'id' => 'my-custom-listing',
        'class' => 'custom-class another-custom-class', // a string or indexed (string) array with the classes for the list tag
    );
    $form['header'] = array(
        '#type' => 'fieldset',
        '#prefix' => '<div class="contentbox-inner mobilebookingagentContainer"><div class="row-fluid"><div class="span12">',
        '#suffix' => '</div></div>',
        '#title' => '',

    );

    $stepmenuarray = array(
        1 => 'TICKETS',
        2 => 'ENHANCEMENTS',
        3 => 'PAYMENT',
        4 => 'REVIEW ORDER',
        5 => 'CONFIRMATION'
    );
    $ticketsmenu = '<div class="menuRelative"><div class="subMenu responsiveWeb"><ul>';
    foreach ($stepmenuarray as $menukey => $menuvalue) {
        if ($menukey == $current_step) {
            $ticketsmenu.='<li class="active">' . $menuvalue . '</li>';
        } else {
            $ticketsmenu.='<li>' . $menuvalue . '</li>';
        }
    }
    
    
    $ticketsmenu .= '</ul></div><div class="responsiveMobile groupmarginTop" align="center"><select class="ticketpriceqty">';
    foreach ($stepmenuarray as $menukey => $menuvalue) {
        if ($menukey == $current_step) {
            $ticketsmenu.='<option selected="select">' . $menuvalue . '</option>';
        } else {
            $ticketsmenu.='<option>' . $menuvalue . '</option>';
        }
    }
    
    $ticketsmenu .= '</select></div></div>';


    $form['header']['stepmenu'] = array(
        '#type' => 'fieldset',
        '#prefix' => '<div class="row-fluid"><div class="span12">',
        '#suffix' => '</div></div>',
        '#title' => '',
        '#value' => $ticketsmenu
    );
    
    
    if ($current_step == 1) {
        $stages_content = '<div class="row-fluid shreBtnmargin responsiveWeb">
        <div class="span12"></div>
        </div>
        <div class="row-fluid responsiveWeb">
        <div class="span12">
        <div class="ticketTitle">PURCHASE TICKETS</div>
        </div>
        </div>';
    }


    if ($current_step == 2) {
    $stages_content = '<div class="row-fluid shreBtnmargin responsiveWeb">
        <div class="span12"></div>
        </div>
        <div class="row-fluid responsiveWeb">
        <div class="span12">
        <div class="ticketTitle">ENHANCEMENTS</div>
        </div>
        </div>';
    }

    //Payments sessions

    if ($current_step == 3) {
      $stages_content = '<div class="row-fluid shreBtnmargin responsiveWeb">
        <div class="span12"></div>
        </div>
        <div class="row-fluid responsiveWeb">
        <div class="span12">
        <div class="ticketTitle">PAYMENT</div>
        </div>
        </div>';
    }
    
    
    if ($current_step == 4) {
      $stages_content = '<div class="row-fluid shreBtnmargin responsiveWeb">
      <div class="span12"></div>
      </div>
      <div class="row-fluid responsiveWeb">
      <div class="span12">
      <div class="ticketTitle">Review Order</div>
      </div>
      </div>';
    }


    $form['header']['content'] = array(
        '#type' => 'fieldset',
        '#prefix' => '<div class="headercontent">',
        '#suffix' => '</div>',
        '#title' => '',
        '#value' => $stages_content,
    );

    return $form;
}



/**
 * Given the current stage the user is on, calculate what the next step would be
 
 */
function bookingagent_move_to_next_stage($form, &$form_state) {

    switch ($form_state['stage']) {
        case 'TICKETS':
            $_SESSION['formstage']="ENHANCEMENTS";
            return 'ENHANCEMENTS';
            break;

        case 'ENHANCEMENTS':
            $_SESSION['formstage']="PAYMENT";
            return 'PAYMENT';
            break;

        case 'PAYMENT':
            $_SESSION['formstage']="REVIEW_ORDER";
            return 'REVIEW_ORDER';
            break;
    }
}




/**
 * Given the current stage the user is on, calculate what the previous step
 * would be
 */
function bookingagent_move_to_previous_stage($form, &$form_state) {

    switch ($form_state['stage']) {
        case 'ENHANCEMENTS':
            $_SESSION['formstage']="TICKETS";
            return 'TICKETS';
            break;

        case 'PAYMENT':
            $_SESSION['formstage']="ENHANCEMENTS";
            return 'ENHANCEMENTS';
            break;

        case 'REVIEW_ORDER':
            $_SESSION['formstage']="PAYMENT";
            return 'PAYMENT';
            break;
    }
}


//load catalog Products
function _loadCatalogProducts() {

    $resultsarray = array();
    $query = new EntityFieldQuery();
    $entities = $query->entityCondition('entity_type', 'taxonomy_term');
    $query->entityCondition('bundle', 'catalog');
    $query->propertyCondition('name', 'Enhancements'); //change 2 to any vocabulary ID
    //$query->fieldCondition('field_cruiseitemid', 'value', $cruiseitemId, '=');
    $query->execute();

    if (count($entities->ordered_results) > 0) {
        foreach ($entities->ordered_results as $termresults) {
            $children = taxonomy_get_children($termresults->entity_id);
            foreach ($children as $childvalue) {
                //loading the Products

                $produtclist[$childvalue->name] = _catalogEnhancementCategoryProducts($childvalue->tid);
              
                $resultsarray[] = $produtclist;
                unset($produtclist);
            }
        }
    }

    return $resultsarray;
}




function _catalogEnhancementCategoryProducts($catalogid) {
    $resultsetnodes = array();
    $totalresults = array();
    $enhancemnetsnodes = getenhancementlistby($catalogid);
    foreach ($enhancemnetsnodes as $ennodes) {
        $resultsetnodes['id'] = $ennodes->id;
        $resultsetnodes['title'] = $ennodes->title;
        $resultsetnodes['price'] = $ennodes->price;
        $resultsetnodes['description'] = $ennodes->description;
        $resultsetnodes['caption'] = $ennodes->caption;
        $resultsetnodes['hoursoffsale'] = $ennodes->hoursoffsale;
        $resultsetnodes['nid'] = $ennodes->nid;
        $resultsetnodes['status'] = $ennodes->status;
        $resultsetnodes['categoryid'] = $ennodes->categoryid;
        $resultsetnodes['created'] = $ennodes->created;
        $resultsetnodes['enhancements_img_fid'] = $ennodes->enhancements_img_fid;
        $totalresults[] = $resultsetnodes;
        unset($resultsetnodes);
    }
    return $totalresults;
}


class _CruiseEventObject{
	public static $eventName='';
	public static $departuretime='';
	public static $returntime;
	public static $portid;
	
	public static $data=array();
	public static function _getCruiseEventDetails(){
		//Getting Eventname
		$eventid=$_SESSION['bookingInput']->eventid;
		$scheduledEventObject=scheduledevents_load($eventid);
		if(!is_null($scheduledEventObject) && isset($scheduledEventObject)){
			$data['eventname']=$scheduledEventObject->name;
			$data['portid']=$scheduledEventObject->port_id;
			$data['departuretime']=$scheduledEventObject->departuretime;
			$data['returntime']=$scheduledEventObject->returntime;
			$data['boardingtime']=$scheduledEventObject->boardingtime;
			$eventTimeUnix = strtotime($_SESSION['bookingInput']->date);
			$eventdate = date("F d, Y", $eventTimeUnix);
			$data['eventdate']=$eventdate;
			
			//Port Details
			$portData = loadportById($data['portid']);
			$data['portname']=$portData['portname'];
		
		}
		
		return $data;
	}
}


function orderSummaryWidget($type) {
	//session_start();
	$cruisedetails=_CruiseEventObject::_getCruiseEventDetails();
	
	
	$inputbookingdata = $_SESSION['bookingInput'];
	$cruiseTemplatId=$inputbookingdata->templateid;
	$cruiseObject=cruise_load($cruiseTemplatId);
	$cruiseTitle=$cruiseObject->cruise_title;
	
	
	$prefixwidget = '<div class="ordersummaryBox">
			<div class="sliderBox">
			<div class="sliderBoxout">
			<div class="orderBoxInner">
			<!--<div class="Ordertitle"><span>Order Summary</span><br><span>' . $cruiseTitle . '<br><br>
			<span>Departure Port : ' . $cruisedetails ['portname'] . '</span><br><span>Departure Time ' . $cruisedetails ['departuretime'] . '</span>&nbsp;&nbsp;<span>Return ' . $cruisedetails ['returntime'] . '</span></div>
			<div>-->
			
			<div class="widgettitleHeader">
			<div class="span2">
			<div class="line"></div>
			</div>
			<div class="span8" align="center">
			<h2>Order Summary</h2>
			</div>
			<div class="span2">
			<div class="line"></div>
			</div>
			</div>
			
			<div class="widgetBody">
			<h2>' . $cruiseTitle . '</h2>
			<h3>' . $cruisedetails ['eventdate'] . '</h3>
			<h4>Departure Port: ' . $cruisedetails ['portname'] . '</h4>
			
			<div class="timingDetails">
			<h3>Depart ' . $cruisedetails ['departuretime'] . '</h3>
			<h4><img src="' . base_path () . 'sites/all/themes/waterways/assets/img/dot.png"></h4>
			<h3>Return ' . $cruisedetails ['returntime'] . '</h3></div>';
	
	$suffixwidget='<div align="center"><img src="' . base_path () . 'sites/all/themes/waterways/assets/img/innerbox-widget-img.png"></div>
			</div>
			</div>
			</div>
			</div>';
	
	
	
	switch ($type) {
        case 'TICKETS' :
        	$ticketswidget = _ticketswidget();
        	$enhancementorderwidget = _enhancementWidgets ();
        	$orderlinewidget = _orderlineitemwidgets ();
        	$printhtmlwidget = $prefixwidget . $ticketswidget .$enhancementorderwidget. $orderlinewidget . '
			</div>'.$suffixwidget;
        	return $printhtmlwidget;
        	break;
            break;

        case 'ENHANCEMENTS' :
        	$ticketswidget = _ticketswidget();
        	$enhancementorderwidget = _enhancementWidgets ();
        	$orderlinewidget = _orderlineitemwidgets ();
        	$printhtmlwidget = $prefixwidget . $ticketswidget .$enhancementorderwidget. $orderlinewidget . '
			</div>'.$suffixwidget;
			return $printhtmlwidget;
			break;
		
		case 'PAYMENT' :
			$ticketswidget = _ticketswidget ();
			$enhancementorderwidget = _enhancementWidgets ();
			$orderlinewidget = _orderlineitemwidgets ();
			$printhtmlwidget = $prefixwidget . $ticketswidget . $enhancementorderwidget . $orderlinewidget . '
			</div>
			<br/>'.$suffixwidget;
			return $printhtmlwidget;
			break;
		
		case 'REVIEW' :
			$ticketswidget = _ticketswidget ();
			$enhancementorderwidget = _enhancementWidgets ();
			$orderlinewidget = _orderlineitemwidgets ();
			$printhtmlwidget = $prefixwidget . $ticketswidget . $enhancementorderwidget . $orderlinewidget . '
			</div>
			<br/>'.$suffixwidget;
			return $printhtmlwidget;
			break;
		
		default :
			break;
	}
}


//Order Line Items
function _orderlineitemwidgets() {
	$sessionid=$_SESSION['cruisecardsession'];
	$cruisefeehtml='';
	$promocodehtml='';
	$giftcardhtml='';
	$vouchercodehtml='';
	$subtotalhtml='';
	$totalhtml='';
        
        $chargetaxhtml='';
        $discounthtml='';

        
        
    //tax Line Item    
    $taxLineItemSerialized=CartSession::getTaxLineItem($sessionid);
    
    if($taxLineItemSerialized){
    	$taxLineItemArray=unserialize($taxLineItemSerialized);
    	foreach($taxLineItemArray as $key=>$values){
    		if($values['amt']>0){
    			$chargetaxhtml.='<div id="chargesdiv" class="taxCharges">';
    			$chargetaxhtml.='<div class="chargesWrapper">';
    			$chargetaxhtml.='<h3 class="chargesWrapperLeft">'.$values['taxname'].'</h3>';
    			$chargetaxhtml.='<span></span><h3 class="chargesWrapperRight"><div>$' . wwcFormatPrice($values['amt']).'</div></h3>';
    			$chargetaxhtml.='</div></div>';
    		}
    		
    		
    		
    	}
    }
    
    //discount Line Item
    $discountLineItemSerialized=CartSession::getDiscountLineItem($sessionid);
    if($discountLineItemSerialized){
    	$discountLineItemArray=unserialize($discountLineItemSerialized);
    	foreach($discountLineItemArray as $key=>$values){
    		if($values['amt']>0){
    			//$chargetaxdivstatus=($_SESSION['chargeTaxValue']!="") ? "block" : "none";
    			$chargetaxhtml.='<div id="chargesdiv" class="taxCharges">';
    			$chargetaxhtml.='<div class="chargesWrapper">';
    			$chargetaxhtml.='<h3 class="chargesWrapperLeft">'.$values['discountname'].'</h3>';
    			$chargetaxhtml.='<span></span><h3 class="chargesWrapperRight"><div>-$' . wwcFormatPrice($values['amt']).'</div></h3>';
    			$chargetaxhtml.='</div></div>';
    		}
    		
    	}
    	
    }
    
   // var_dump($discountLineItemArray); exit();
   //exit();
    
   
    
    
    
//     $discountdivstatus=($_SESSION['descriptionDiscountValue']!="") ? "block" : "none";    
//     $discounthtml.='<div id="chargesdiv" style="display:'.$discountdivstatus.';" class="taxCharges">';
//     $discounthtml.='<div class="chargesWrapper">';
//     $discounthtml.='<h3 class="chargesWrapperLeft">'.$_SESSION['descriptionDiscountName'].'</h3>';
//     $discounthtml.='<span></span><h3 class="chargesWrapperRight"><div>-$' . wwcFormatPrice($_SESSION['descriptionDiscountValue']).'</div></h3>';
//     $discounthtml.='</div></div>';
        
        
	
	
	
	//Cruise Fee Line Items
	$cruiseFeeItems=CartSession::getCruiseFeeItems($sessionid);
	
	//drupal_json_output($cruiseFeeItems); exit();
	$totalCruisefeeAmt=0;
	foreach($cruiseFeeItems as $item){
		$totalCruisefeeAmt+=$chargeamt;
		
		if($item['totalamt']!='' && $item['totalamt']>0){
		$cruisefeehtml.='<div class="taxCharges">';
		$cruisefeehtml.='<div class="chargesWrapper">';
		$cruisefeehtml.='<h3 class="chargesWrapperLeft">' . $item['title'] . '</h3>';
		$chargeamt=$item['totalamt'];
		$amountFormattedString = sprintf("%.2f", $chargeamt);
		$amtArray = explode('.', $amountFormattedString);
		$amtWholeNumber = 0;
		$amtDecimalNumber = 0;
		
		if (isset($amtArray)) {
		$amtWholeNumber = $amtArray[0];
		$amtDecimalNumber = $amtArray[1];
		}
		$amt = '$' . $amtWholeNumber . '.<sup>' . $amtDecimalNumber . '</sup>';
		
		$cruisefeehtml.='<span></span><h3 class="chargesWrapperRight">' . $amt . '</h3>';
		$cruisefeehtml.='</div></div>';
		}
	}
	
	
	
	
    //Promocode
	$couponAmtWholeNumber = 0;
	$couponAmtDecimalNumber = 0;
    $promocodeserialized=CartSession::getPromocode($sessionid);
    if($promocodeserialized){
    	$promocode='';
    	$promocodediscount=0;
    	$promocodeArr=unserialize($promocodeserialized);
    	foreach($promocodeArr as $key=>$value){
    		$promocode=$key;
    		$promocodediscount=$value;
    	}
    	$cpnAmount = ($promocodediscount!='') ? $promocodediscount : 0;
    	$couponAmountFormattedString = number_format($cpnAmount, 2);
    	$couponArray = explode('.', $couponAmountFormattedString);
    	
    	
    	if (isset($couponArray)) {
    		$couponAmtWholeNumber = $couponArray[0];
    		$couponAmtDecimalNumber = $couponArray[1];
    	}
    	
    }
    
    $cpndivstatus=($promocodeserialized) ? "block" : "none";
    $promocodehtml.='<div id="cpnmaindiv" style="display:'.$cpndivstatus.';" class="taxCharges">';
    $promocodehtml.='<div class="chargesWrapper">';
    $promocodehtml.='<h3 class="chargesWrapperLeft">Coupon</h3>';
    $promocodehtml.='<span></span><h3 class="chargesWrapperRight"><div id="cpnamountdiv">-$' . $couponAmtWholeNumber . '.<sup>' . $couponAmtDecimalNumber . '</sup></div></h3>';
    $promocodehtml.='</div></div>';
    
 
    
    //Gift Card
    $giftCardserialized=CartSession::getGiftCard($sessionid);
    $giftcardAmtWholeNumber = 0;
    $giftcardAmtDecimalNumber = 0;
    if($giftCardserialized){
    	$giftCardArray=unserialize($giftCardserialized);
    	$giftCardCode='';
    	$giftCardAmount='';
    	foreach($giftCardArray as $key=>$values){
    		$giftCardCode=$key;
    		$giftCardAmount=$giftCardArray[$key]['deductedamount'];
    	}
    	
    	$giftcardAmount = ($giftCardAmount!='') ? $giftCardAmount : 0;
    	$giftcardAmountFormattedString = number_format($giftcardAmount, 2);
    	$giftcardArray = explode('.', $giftcardAmountFormattedString);
    	
    	if (isset($giftcardArray)) {
    		$giftcardAmtWholeNumber = $giftcardArray[0];
    		$giftcardAmtDecimalNumber = $giftcardArray[1];
    	}
    }
    $gifctcarddivstatus=($giftCardserialized) ? "block" : "none";
    $giftcardhtml.='<div id="giftcardmaindiv" style="display:'.$gifctcarddivstatus.';" class="taxCharges">';
    $giftcardhtml.='<div class="chargesWrapper">';
    $giftcardhtml.='<h3 class="chargesWrapperLeft">Gift Card</h3>';
    $giftcardhtml.='<span></span><h3 class="chargesWrapperRight"><div id="giftcardamountdiv">-$' . $giftcardAmtWholeNumber . '.<sup>' . $giftcardAmtDecimalNumber . '</sup></h3>';
    $giftcardhtml.='</div></div>';
    
    
    //Voucher codes
    $voucherAmount=CartSession::getVoucherCodesPrice($sessionid);
    $voucherAmtWholeNumber = 0;
    $voucherAmtDecimalNumber = 0;
    if($voucherAmount!="" || $voucherAmount!=0){
    	$voucherwidgethtml = '';
    	
    	$voucherAmountFormattedString = number_format($voucherAmount, 2);
    	$voucherArray = explode('.', $voucherAmountFormattedString);
    	
    	if (isset($voucherArray)) {
    		$voucherAmtWholeNumber = $voucherArray[0];
    		$voucherAmtDecimalNumber = $voucherArray[1];
    	}
    }
    $vouchermaindiv=($voucherAmount>0) ? "block" : "none";
    $vouchercodehtml.='<div id="vouchermaindiv" style="display:'.$vouchermaindiv.';" class="taxCharges">';
    $vouchercodehtml.='<div class="chargesWrapper">';
    $vouchercodehtml.='<h3 class="chargesWrapperLeft">Voucher</h3>';
    $vouchercodehtml.='<span></span><h3 class="chargesWrapperRight"><div id="voucheramtdiv">-$' . $voucherAmtWholeNumber . '.<sup>' . $voucherAmtDecimalNumber . '</sup></div></h3>';
    $vouchercodehtml.='</div></div>';
    
    
    
    
    
    
    
    //Sub Total
    $cruisesubtotalWholeNumber = 0;
    $cruisesubtotalamtDecimalNumber = 0;
    
    
    $orderSubTotal=CartSession::getOrderSubTotal($sessionid);
    if($orderSubTotal!=""){
    	$cruisesubtotalFormattedString = number_format($orderSubTotal, 2);
    	$cruisesubtotalArray = explode('.', $cruisesubtotalFormattedString);
    	
    	if (isset($cruisesubtotalArray)) {
    		$cruisesubtotalWholeNumber = $cruisesubtotalArray[0];
    		$cruisesubtotalamtDecimalNumber = $cruisesubtotalArray[1];
    	}
    	$cruisesubtotalchargeamt = '$' . $cruisesubtotalWholeNumber . '.<sup>' . $cruisesubtotalamtDecimalNumber . '</sup>';
    	$subtotalhtml = '';
    	$subtotalhtml.='<div class="taxCharges">';
    	$subtotalhtml.='<div class="chargesWrapper">';
    	$subtotalhtml.='<h3 class="chargesWrapperLeft">Subtotal</h3>';
    	$subtotalhtml.='<span></span><h3 class="chargesWrapperRight">' . $cruisesubtotalchargeamt . '</h3>';
    	$subtotalhtml.='</div>';
    }
    
     //Tax
    $taxAmt=CartSession::getTaxAmt($sessionid);
    if($taxAmt!=""){
    	$taxamthtml = '';
    	$taxamthtml.='<div class="taxCharges">';
    	$taxamthtml.='<div class="chargesWrapper">';
    	$taxamthtml.='<h3 class="chargesWrapperLeft">Tax</h3>';
    	$taxamthtml.='<span></span><h3 class="chargesWrapperRight">$' . _wwcFormatPrice($taxAmt) . '</h3>';
    	$taxamthtml.='</div>';
    }
    
    
    
    
    //************** Total Amount ****************************
    $totalcal=CartSession::getOrderTotal($sessionid);
    if($totalcal!=""){
    	$numFormattedString = number_format($totalcal, 2);
    	$totalArray = explode('.', $numFormattedString);
    	$totalAmt = $totalArray[0];
    	$decimals = $totalArray[1];
    	$totalhtml = '';
    	$totalhtml.='<div class="totalDetails">';
    	$totalhtml.='<h2 align="left">Total</h2>';
    	$totalhtml.='<h2 align="right"><div id="totalamtdiv">$' . $totalAmt . '.<sup>' . $decimals . '</sup></div></h2>';
    	$totalhtml.='</div>';
    	
    	if($_SESSION['formstage']=="PAYMENT"){
    		//$totalhtml.="<a href='#' id='editticketorderlinkPayment'>EDIT TICKET ORDER </a><br/>";
    		//$totalhtml.="<a href='#' id='backtoenhancementslink'>BACK TO ENHANCEMENTS</a><br/>";
    	}
    	
    	if($_SESSION['formstage']=="ENHANCEMENTS"){
    		//$totalhtml.="<a href='#' id='editticketorderlinkEnhancements'>EDIT TICKET ORDER </a><br/>";
    	
    	}
    }
     
   $html=$chargetaxhtml.''.$discounthtml.''.$cruisefeehtml.''.$promocodehtml.''.$giftcardhtml.''.$vouchercodehtml.''.$subtotalhtml.''.$taxamthtml.''.$totalhtml;
   //$ordertotals = $subchargeswidget . $taxsubchargeswidget . $couponwidgethtml . $voucherwidgethtml . $giftcardwidgethtml . $subtotalwidgethtml . $totalwidgethtml;
    return $html;
}

function _enhancementWidgets() {
	$enhancementhtml='';
	$sessionid=$_SESSION['cruisecardsession'];
	$enhancementsserialized=CartSession::getEnhancements($sessionid);
	if($enhancementsserialized){
		$enhancmentsitems=unserialize($enhancementsserialized);
		foreach ($enhancmentsitems as $enhancmentwidget) {
			$amount = $enhancmentwidget['qty'] * $enhancmentwidget['price'];
			$amountFormattedString = number_format($amount, 2);
			$amtArray = explode('.', $amountFormattedString);
			$amtWholeNumber = 0;
			$amtDecimalNumber = 0;
			if (isset($amtArray)) {
				$amtWholeNumber = $amtArray[0];
				$amtDecimalNumber = $amtArray[1];
			}
			$amt = '$' . $amtWholeNumber . '.<sup>' . $amtDecimalNumber . '</sup>';
			 
			$enhancementhtml.='<div class="ticketDetails">';
			$enhancementhtml.='<h3 align="left">Enhancement <br/><span >' . $enhancmentwidget['qty'] . ' ' . $enhancmentwidget['name'] . '</span></h3>';
			$enhancementhtml.='<h3 align="right">Price <br/><span>' . $amt.'</span></h3>';
			$enhancementhtml.='</div>';
			$enhancementhtml.='<div class="line"></div><div class="line"></div>';
		}
	}
	
  
    return $enhancementhtml;
}



function _ticketswidget() {
    $tickethtml = '';
    $sessionid = $_SESSION ['cruisecardsession'];
    $ticketsserialized = CartSession::getTickets($sessionid);
    if ($ticketsserialized) {
        $tickets = unserialize($ticketsserialized);
        foreach ($tickets as $ticket) {
            if ($ticket ['qty'] > 0) {
                $tickethtml .= '<div class="ticketDetails">';
                $tickethtml .= '<h3 align="left">Ticket <br/><span >' . $ticket['qty'] . ' ' . $ticket['ticketname'] . '</span></h3>';

                $amount = $ticket['qty'] * $ticket['price'];
                $amountFormattedString = number_format($amount, 2);
                $amtArray = explode('.', $amountFormattedString);
                $amtWholeNumber = 0;
                $amtDecimalNumber = 0;
                if (isset($amtArray)) {
                    $amtWholeNumber = $amtArray [0];
                    $amtDecimalNumber = $amtArray [1];
                }
                $amt = '$' . $amtWholeNumber . '.<sup>' . $amtDecimalNumber . '</sup>';
                $tickethtml .= '<h3 align="right">Price <br/><span>' . $amt . ' </span></h3>';
                $tickethtml .= '</div>';
                $tickethtml .= '<div class="line"></div><div class="line"></div>';
            }
        }
    }
    return $tickethtml;
}
