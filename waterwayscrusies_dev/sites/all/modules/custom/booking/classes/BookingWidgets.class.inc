<?php
class BookingWidget{
	
	//Tickets
	public static function tickets(){
		$bookingCart= new BookingCart();
		$tickets=$bookingCart->getTicketsList();
		$tickethtml='';
		foreach ( $tickets as $ticket ) {
			if ($ticket ['qty'] > 0) {
				$tickethtml .= '<div class="ticketDetails">';
				$tickethtml .= '<h3 align="left">Ticket <br/><span >' . $ticket['qty'] . ' ' . $ticket['ticketname'] . '</span></h3>';
				$amount=0;
				$amount = $ticket['price'];
				if(!empty($ticket['grouponcode'])){
					//$amount=$ticket['dealprice'];
					$amount=0;
				}
				//$amount = $ticket['qty'] * $ticket['price'];
				
				$tickethtml .= '<h3 align="right">Price <br/><span>$' . wwcFormatPrice($amount) . ' </span></h3>';
				$tickethtml .= '</div>';
				$tickethtml .= '<div class="line"></div><div class="line"></div>';
			}
		}
		return $tickethtml;
	}
	
	//Fee Items
	public static function feeItems(){
		$cruisefeehtml='';
		$bookingCart= new BookingCart();
		$cruiseFeeItems=$bookingCart->getTicketsFeeItemsList();
			foreach($cruiseFeeItems as $item){
				$chargeamt=$item['totalamt'];
				if($chargeamt>0){
					$cruisefeehtml.='<div class="taxCharges">';
					$cruisefeehtml.='<div class="chargesWrapper">';
					$cruisefeehtml.='<h3 class="chargesWrapperLeft">' . $item['title'] . '</h3>';
					$cruisefeehtml.='<span></span><h3 class="chargesWrapperRight">$' . wwcFormatPrice($chargeamt) . '</h3>';
					$cruisefeehtml.='</div></div>';
				}
		
			}
			return $cruisefeehtml;
		
	}
	
	
	//Order Subtotal
	public static function orderSubTotal(){
		$subtotalhtml='';
		$bookingCart= new BookingCart();
		$orderSubTotal=$bookingCart->getOrderSubTotal();
		if($orderSubTotal!=""){
			$subtotalhtml = '';
			$subtotalhtml.='<div class="taxCharges">';
			$subtotalhtml.='<div class="chargesWrapper">';
			$subtotalhtml.='<h3 class="chargesWrapperLeft">Subtotal</h3>';
			$subtotalhtml.='<span></span><h3 class="chargesWrapperRight">$' . wwcFormatPrice($orderSubTotal) . '</h3>';
			$subtotalhtml.='</div>';
		}
		return $subtotalhtml;
	}
	
	//Tax
	public static function tax(){
		$taxamthtml = '';
		$bookingCart= new BookingCart();
		$taxAmt=$bookingCart->getTaxAmount();
		if($taxAmt!=""){
			
			$taxamthtml.='<div class="taxCharges">';
			$taxamthtml.='<div class="chargesWrapper">';
			$taxamthtml.='<h3 class="chargesWrapperLeft">Tax</h3>';
			$taxamthtml.='<span></span><h3 class="chargesWrapperRight">$' . wwcFormatPrice($taxAmt) . '</h3>';
			$taxamthtml.='</div>';
		}
		return $taxamthtml;
	}
	
	
	
	//Order Total
	public static function orderTotal(){
		$totalhtml='';
		$bookingCart= new BookingCart();
		$orderTotal=(float)$bookingCart->getOrderTotal();
		
		//var_dump($orderTotal); exit();
		if($orderTotal>0){
			$totalhtml.='<div class="totalDetails">';
			$totalhtml.='<h2 align="left">Total</h2>';
			$totalhtml.='<h2 align="right"><div id="totalamtdiv">$' .wwcFormatPrice($orderTotal).'</div></h2>';
			$totalhtml.='</div>';
			if($_SESSION['formstage']=="PAYMENT"){
				$totalhtml.="<a href='#' id='editticketorderlinkPayment'>EDIT TICKET ORDER </a><br/>";
				$totalhtml.="<a href='#' id='backtoenhancementslink'>BACK TO ENHANCEMENTS</a><br/>";
			}
			if($_SESSION['formstage']=="ENHANCEMENTS"){
				$totalhtml.="<a href='#' id='editticketorderlinkEnhancements'>EDIT TICKET ORDER </a><br/>";
		
			}
		}
		return $totalhtml;
	}
	
	
	//Enhancements
	public static function enhancements(){
			$enhancementhtml='';
			$bookingCart= new BookingCart();
			$enhancmentsitems=$bookingCart->getEnhancementsList();
				foreach ($enhancmentsitems as $enhancmentwidget) {
					$amount = $enhancmentwidget['qty'] * $enhancmentwidget['price'];
					$enhancementhtml.='<div class="ticketDetails">';
					$enhancementhtml.='<h3 align="left">Enhancement <br/><span >' . $enhancmentwidget['qty'] . ' ' . $enhancmentwidget['name'] . '</span></h3>';
					$enhancementhtml.='<h3 align="right">Price <br/><span>$' . wwcFormatPrice($amount).'</span></h3>';
					$enhancementhtml.='</div>';
					$enhancementhtml.='<div class="line"></div><div class="line"></div>';
				}
		
			return $enhancementhtml;
	}
	
	
	//promocode
	public static function promocode(){
	$promocodehtml='';
	$bookingCart= new BookingCart();
	$promocodeArr=$bookingCart->getPromoCode();
	$promocodediscount=0;
	if(is_array($promocodeArr)){
		foreach($promocodeArr as $key=>$value){
			$promocode=$key;
			$promocodediscount=$value;
		}
	}
   
    
    $cpndivstatus=($promocodediscount>0) ? "block" : "none";
    $promocodehtml.='<div id="cpnmaindiv" style="display:'.$cpndivstatus.';" class="taxCharges">';
    $promocodehtml.='<div class="chargesWrapper">';
    $promocodehtml.='<h3 class="chargesWrapperLeft">Promo Code</h3>';
    $promocodehtml.='<span></span><h3 class="chargesWrapperRight"><div id="cpnamountdiv">-$' . wwcFormatPrice($promocodediscount).'</div></h3>';
    $promocodehtml.='</div></div>';
   	return $promocodehtml;
	
		
	}
	
	
	//get vouchers
	public static function getVouchers(){
		
		$bookingCart= new BookingCart();
		$vouchers=$bookingCart->getVouchers();
		$vouchercodehtml='';
		//var_dump(count($vouchers)); exit();
		for($i=1;$i<=count($vouchers);$i++){
			
			if(isset($vouchers[$i]) && count($vouchers[$i]) >0  ){
				$vouchercodehtml.='<div id="vouchermaindiv"  class="taxCharges">';
				$vouchercodehtml.='<div class="chargesWrapper">';
				$vouchercodehtml.='<h3 class="chargesWrapperLeft">'.$vouchers[$i]['code'].'</h3>';
				$vouchercodehtml.='<span></span><h3 class="chargesWrapperRight"><div id="voucheramtdiv">-$' .wwcFormatPrice($vouchers[$i]['amount']).'</div></h3>';
				$vouchercodehtml.='</div></div>';
			}
		}
		
		return $vouchercodehtml;

	}
	
	//giftcard
	public static function giftcard(){
		$giftCardAmount=0;
		$bookingCart= new BookingCart();
		$giftCardArray=$bookingCart->getGiftCard();
		$giftCardCode='';
		$giftcardhtml='';
		foreach($giftCardArray as $key=>$values){
			$giftCardCode=$key;
			$giftCardAmount=$giftCardArray[$key]['deductedamount'];
		}
		
		if($giftCardAmount>0){
			$giftcardhtml.='<div id="giftcardmaindiv" class="taxCharges">';
			$giftcardhtml.='<div class="chargesWrapper">';
			$giftcardhtml.='<h3 class="chargesWrapperLeft">Gift Card</h3>';
			$giftcardhtml.='<span></span><h3 class="chargesWrapperRight"><div id="giftcardamountdiv">-$' . wwcFormatPrice($giftCardAmount).'</div></h3>';
			$giftcardhtml.='</div></div>';
		}
		
		return $giftcardhtml;
		
		
	}
	
	
}