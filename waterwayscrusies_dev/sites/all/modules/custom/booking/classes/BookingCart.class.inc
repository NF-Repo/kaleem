<?php
class BookingCart {
	
	public $cartId;
	public $ubercartOrderId;
	
	public function __construct() {
		$cartId=session_id();
		if(empty($cartId)){
			session_start();
		}
		$this->cartId=$cartId;
		return $cartId;
	}
	
	public function cartId(){
		return $this->cartId;
	}
	
	public function setUberCartOrderId($order_id){
		$_SESSION['cart'][$this->cartId]['ubercartOrderId']=$order_id;
	}
	
	
	public function getCartData(){
		return $_SESSION['cart'][$this->cartId];
	}
	
	
	public function getUberCartOrderId(){
		return isset($_SESSION['cart'][$this->cartId]['ubercartOrderId']) ? $_SESSION['cart'][$this->cartId]['ubercartOrderId'] : '';
	}
	
	public function getCardId(){
		return $_SESSION['cart'][$this->cartId];
	}
	
	public function emptyCart(){
		$_SESSION['cart'][$this->cartId]=array();
	}
	
	
	public function getCartCruiseTemplateId(){
		
		$inputbookingdata =$_SESSION['cart'][$this->cartId]['bookingInput'];
		$cruise_id = $inputbookingdata->templateid;
		return $cruise_id;
	}
	
	public function getCartEventId(){
		$inputbookingdata =$_SESSION['cart'][$this->cartId]['bookingInput'];
		$eventid = $inputbookingdata->eventid;
		return $eventid;
	}
	
	
	public function getCartEventDate(){
		$inputbookingdata =$_SESSION['cart'][$this->cartId]['bookingInput'];
		$eventDate = $inputbookingdata->date;
		return $eventDate;
	}
	
	
	public function storeBookingInput($bookingInput){
		$_SESSION['cart'][$this->cartId]['bookingInput']=$bookingInput;
	}
	
	
	
	//1 Adding Tickets To Cart
	public function addTickets($formValues){
		
		
		//drupal_json_output($formValues); exit();
		$totalticketsprice=0;
		$ticketsResult=array();
		$ticketsListArray = array();
		foreach ($formValues as $key => $value) {
			//Key -> ticketentrees2
			$reg1 = "/^(ticketentrees)?\d+$/";
			if (preg_match($reg1, $key)) {
				if ($formValues[$key] > 0) {
					$ticketenteeeid = explode('ticketentrees', $key);
					//$result = array();
					$ticketID = $ticketenteeeid[1];
					$result['ticketid'] = $ticketenteeeid[1];
					$result['qty'] = $formValues[$key];
					$result['ticketname'] = $formValues[$key . "ticketname"];
					$result['price'] = $formValues[$key . "price"];
					
					
					for ($i = 0; $i < $result['qty']; $i++) {
						$ticketsList=array();
						$ticketsList['ticketid'] = $ticketenteeeid[1];
						$ticketsList['qty'] = (int) 1;
						$ticketsList['ticketname'] = $formValues[$key . "ticketname"];
						$ticketsList['price'] = (double) $formValues[$key . "price"];
						$ticketsList['dealprice'] = (double) $formValues[$key . "price"];
						$ticketsList['grouponcode'] = "";
						$ticketsList['entree'] = $formValues['select' . $result['ticketid'] . $i];
						$ticketsList['comments'] = $formValues['dietarycomments' . $result['ticketid'] . $i];
						$ticketsListArray[]=$ticketsList;
						$totalticketsprice+=(double) $formValues[$key . "price"];;
					}
				}
			}
		}//foreach
		
		
		$bookingCart= new BookingCart();
		
		//Ticket Page
		$specialinstructions=$formValues['specialinstructions'];
		$additionalcomments=$formValues['additionalcomments'];
		$_SESSION['cart'][$this->cartId]['user']['specialinstructions']=$specialinstructions;
		$_SESSION['cart'][$this->cartId]['user']['additionalcomments']=$additionalcomments;
		
		
		$_SESSION['cart'][$this->cartId]['ticketsList']=$ticketsListArray;
		$_SESSION['cart'][$this->cartId]['totalTicketsAmount']=$totalticketsprice;
		
		
		
		
		
		$feesResult=array();
		$feeitems_tax = '';
		$cruiseFeeItemPrice = 0;
		$feeitems = Fees::load_fee_items();
		$ticketfeeids = array();
		$cruisefeeitems = '';
	
		foreach ($feeitems as $feeitem) {
			$feeId = $feeitem->id;
			$feeTitle = $feeitem->title;
			$a_feeValue = $feeitem->value;
			$feeType = $feeitem->type;
			$chargeamt = 0;
	
			$feeValue = 0;
	
	
			//Getting the Fee Value
			//$inputbookingdata = $_SESSION['bookingInput'];
			//$cruise_id = $inputbookingdata->templateid;
		    $cruise_id=$this->getCartCruiseTemplateId();
	
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
				foreach ($ticketsListArray as $ticket) {
					if ($ticket ['qty'] > 0) {
						$ticket_id = $ticket ['ticketid'];
						$ticket_qty = $ticket ['qty'];
						$ticket_price = $ticket ['price'];
						$ticket_feeids = Fees::get_ticket_feeids($ticket_id);
						if (in_array($feeId, $ticket_feeids)) {
							$ticketfeeids[] = $feeId;
							if ($feeType == "flatfee") {
								$totalcharge = $ticket_qty * $feeValue;
								$chargeamt += $totalcharge;
							}
	
							if ($feeType == "percentage") {
								$totalticketpriceamt = $ticket_qty * $ticket_price;
								$chargeamt += ($totalticketpriceamt * $feeValue) / 100;
							}
						}
					}
				}//tickets loop ends here
				// $feeitems_tax=$feeitems_tax;
				$cruiseFeeItemPrice+=$chargeamt;
				$cruisefeeitems[$feeId] = array('feeid' => $feeId, 'title' => $feeTitle, 'type' => $feeType, 'actualrate' => $feeValue, 'updatedrate' => $feeValue, 'totalamt' => $chargeamt);
			}
		}//cruise fee items loop ends here
		
		
		$_SESSION['cart'][$this->cartId]['ticketsFeeItemsList']=$cruisefeeitems;
		$_SESSION['cart'][$this->cartId]['ticketsFeeItemsTotalAmount']=$cruiseFeeItemPrice;
		$_SESSION['cart'][$this->cartId]['ticketfeeids']=$ticketfeeids;
		
		
		
		
		
	}
	
	
	
	
	
	
	//3 Getting Tax Amount
	public function getTaxAmount(){
		
		
		//Tickets Calculations
		$ticketsTaxAmount=0;
		$ticketsList=$this->getTicketsList();
		foreach($ticketsList as $ticket){
			//Check is Taxable Ticket
			$taxableStatus = CruiseTickets::isTaxableTicket($ticket['ticketid']);
			if ($taxableStatus) {
				$tPrice=0;
				if(!empty($ticket['grouponcode'])){
					$tPrice=(double) $ticket['dealprice'];
				}else{
					$tPrice=(double) $ticket['price'];
				}
				$ticketsTaxAmount+= (double) $tPrice;
			}
		
		}
		$_SESSION['cart'][$this->cartId]['ticketsTaxAmount']= (double) $ticketsTaxAmount;
		
		
		//Tickets Fee Calculation
		$feeItemsList=$this->getTicketsFeeItemsList();
		$ticketfeeids=$this->getTicketFeeIds();
		
		$feeitems_tax = 0;
		foreach ($feeItemsList as $key => $values) {
			$feeid = $values['feeid'];
			if (in_array($feeid, $ticketfeeids)) {
				$feeTaxableStatus = Fees::is_taxable_cruisefee_item($feeid);
				if ($feeTaxableStatus) {
					$feeitems_tax+=$values['totalamt'];
				}
			}
		}
		$_SESSION['cart'][$this->cartId]['ticketsFeesTaxAmount']= (double) $feeitems_tax;
		
		
		
		$taxamt=0;
		$taxvalue = variable_get('WWCTAXVALUE', '');
		$enhancementsTotalAmount=0;
		$enhancementsTotalAmount=$this->getEnhancementsTotalAmount();
		
		$taxAmount=$ticketsTaxAmount+$feeitems_tax+$enhancementsTotalAmount;
		if ($taxAmount > 0) {
			$taxamt=($taxAmount * $taxvalue) / 100;
			// $taxamt = sprintf("%.2f", ($taxAmount) * $taxvalue / 100);
		}
		$_SESSION['cart'][$this->cartId]['taxAmount']= (double) $taxamt;
		return $taxamt;
	}
	
	
	
	//2 Tickets List
	public function getTicketsList(){
		return isset($_SESSION['cart'][$this->cartId]['ticketsList']) ? $_SESSION['cart'][$this->cartId]['ticketsList'] : array();
	}
	
	public function getTicketsTotalAmount(){
		
		$ticketsList=$this->getTicketsList();
		$amount=0;
		foreach($ticketsList as $ticket){
				if(!empty($ticket['grouponcode'])){
					$tPrice=0;
					//$tPrice=(double) $ticket['dealprice'];
				}else{
					$tPrice=(double) $ticket['price'];
				}
				
				$amount+=$tPrice;
		
		}
		
		$_SESSION['cart'][$this->cartId]['totalTicketsAmount']=$amount;
		return $amount;
		//return isset($_SESSION['cart'][$this->cartId]['totalTicketsAmount']) ? $_SESSION['cart'][$this->cartId]['totalTicketsAmount'] : 0;
	}
	
	
	//Fees
	public function getTicketsFeeItemsList(){
		return isset($_SESSION['cart'][$this->cartId]['ticketsFeeItemsList']) ? $_SESSION['cart'][$this->cartId]['ticketsFeeItemsList'] : array();
	}
	
	public function getTicketsFeesTotalAmount(){
		return isset($_SESSION['cart'][$this->cartId]['ticketsFeeItemsTotalAmount']) ? $_SESSION['cart'][$this->cartId]['ticketsFeeItemsTotalAmount'] : 0;
	}
	
	
	public function getTicketFeeIds(){
		return isset($_SESSION['cart'][$this->cartId]['ticketfeeids']) ? $_SESSION['cart'][$this->cartId]['ticketfeeids'] : array();
	}
	
	
	//Order Sub Total
	public function getOrderSubTotal(){
		$orderSubTotal=0;
		$totalTicketsAmount=$this->getTicketsTotalAmount();
		$ticketsFeeItemsTotalAmount=$this->getTicketsFeesTotalAmount();
		$enhancementsTotalAmount=$this->getEnhancementsTotalAmount();
		$orderSubTotal =$totalTicketsAmount +$ticketsFeeItemsTotalAmount+$enhancementsTotalAmount;
		$_SESSION['cart'][$this->cartId]['orderSubTotal']=$orderSubTotal;
		return $orderSubTotal;
	}
	
	
	public function getOrderTotal(){
	
	
		$orderSubTotal=$this->getOrderSubTotal();
		$taxamt=$this->getTaxAmount();
		$promocodediscount=$this->getPromoCodeAmount();
		$giftCardAmount=$this->getGiftCardAmount();

	
		// 		//Voucher Codes
		// 		$vouchers=$this->getVouchers();
		// 		$voucherAmt=0;
		// 		for($i=1;$i<=count($vouchers);$i++){
		// 			if(isset($vouchers[$i]) && count($vouchers[$i]) >0  ){
		// 				$voucherAmt+=$vouchers[$i]['amount'];
		// 			}
		// 		}
	
	
	
		$orderToalAmount = ($orderSubTotal + $taxamt-$promocodediscount-$giftCardAmount);
		$_SESSION['cart'][$this->cartId]['orderToalAmount']=$orderToalAmount;
		return $orderToalAmount;
	}
	
		
	

	
	public function updateTicketsListGroupon($ticketsList){
		$_SESSION['cart'][$this->cartId]['ticketsList']=array();
		$_SESSION['cart'][$this->cartId]['ticketsList']=$ticketsList;
	}
	
	
	
	

	
	
	
		
	
	
	

	
	
	
// 	public function updateOrderTotal($orderToalAmount){
// 		$_SESSION['cart'][$this->cartId]['orderToalAmount']=$orderToalAmount;
// 	}
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	//Enhancements
	public function addEnhancements($formValues){
		$productresult = array();
		$enhancementproducts = array();
		foreach ($formValues as $enhancmentsvalues) {
			$return = array();
			/**
			 * Using the Recursive array function instead of using
			 * complex nested for loop.
			 * Making a single key value pair array of input comples nested
			 * array using array_walk_recursive,array_chunk,array_combine
			 * return Enhancment Products Array
			 *
			 *
			*/
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
		
		$_SESSION['cart'][$this->cartId]['enhancementsList']=$enhancementproducts;
		
		
		//Calculating the Enhancments Total
		$enhancementtotalprice = 0;
		foreach ($enhancementproducts as $productoption) {
			if ($productoption['qty'] > 0) {
				$productprice = $productoption['qty'] * (float) $productoption['price'];
				$enhancementtotalprice+=$productprice;
			}
		}
		
		$_SESSION['cart'][$this->cartId]['enhancementsTotalAmount']=$enhancementtotalprice;
		
		
		//Enhancements Page
		$occasiontype=$formValues['occasiontype'];
		$occasionadditionalcomments=$formValues['occasionadditionalcomments'];
		$_SESSION['cart'][$this->cartId]['user']['occasiontype']=$occasiontype;
		$_SESSION['cart'][$this->cartId]['user']['occasionadditionalcomments']=$occasionadditionalcomments;
		
		

		
	}
	
	public function getEnhancementsList(){
		return isset($_SESSION['cart'][$this->cartId]['enhancementsList']) ? $_SESSION['cart'][$this->cartId]['enhancementsList'] : array();
	}
	
	public function getEnhancementsTotalAmount(){
		return isset($_SESSION['cart'][$this->cartId]['enhancementsTotalAmount']) ? $_SESSION['cart'][$this->cartId]['enhancementsTotalAmount'] : 0;
	}
	
	
	
	
	
	
	
	public function addUserInfo($formValues){
		
		//drupal_json_output($formValues); exit();
		$_SESSION['cart'][$this->cartId]['user']['info']=array();
		//creditFirstName
		//creditLastName
		//creditCardType
		//creditCardNumber
		//creditExpMonth
		//creditExpYear
		//creditcid
		$restrictVars=array('creditFirstName','creditLastName','creditCardType','creditCardNumber','creditExpMonth','creditExpYear','creditcid','back','backtoticketorder','next','form_build_id','form_id','op');
		$userinfo=new stdClass();
		foreach($formValues as $key=>$val){
			if(!in_array($key,$restrictVars)){
				$_SESSION['cart'][$this->cartId]['user']['info'][$key]=$val;
			}
			
		}
	}
	
	
	public function getUserInfo(){
		return $_SESSION['cart'][$this->cartId]['user'];
	}
	
	
	public function addSpecialInstructions($specialinstructions,$additionalcomments){
		$_SESSION['cart'][$this->cartId]['user']['specialinstructions']=$specialinstructions;
		$_SESSION['cart'][$this->cartId]['user']['additionalcomments']=$additionalcomments;
	}
	
	
	public function addOccassionComments($occasiontype,$occasionadditionalcomments){
		$_SESSION['cart'][$this->cartId]['user']['occasiontype']=$occasiontype;
		$_SESSION['cart'][$this->cartId]['user']['occasionadditionalcomments']=$$occasionadditionalcomments;
	}
	
	public function receiveEmailUpdates($promotionaldiscountsenable){
		$_SESSION['cart'][$this->cartId]['user']['receiveEmailUpdates']=$promotionaldiscountsenable;
	}
	
	
	
	//Promocodes
	public function addPromoCode($promocode){
		$_SESSION['cart'][$this->cartId]['promocode']=$promocode;
	}
	
	public function removePromoCode(){
		$promocodeArr=$this->getPromoCode();
		$promocode='';
		$promocodediscount=0;
		$_SESSION['cart'][$this->cartId]['promocode']='';
	}
	
	public function getPromoCode(){
		return isset($_SESSION['cart'][$this->cartId]['promocode']) ? $_SESSION['cart'][$this->cartId]['promocode'] : '';
	}
	
	
	public function getPromoCodeAmount(){
		$promocodeArr=$this->getPromoCode();
		$promocode='';
		$promocodediscount=0;
		if(is_array($promocodeArr)){
			foreach($promocodeArr as $key=>$value){
				$promocode=$key;
				$promocodediscount=$value;
			}
		}
		return $promocodediscount;
	}
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	//vouchers
	public function addVoucher($voucher,$cnt){
		//$_SESSION['cart'][$this->cartId]['vouchers']='';
		//exit();
		//$_SESSION['cart'][$this->cartId]['vouchers']=array();
		if(!isset($_SESSION['cart'][$this->cartId]['vouchers'])){
			$_SESSION['cart'][$this->cartId]['vouchers']=array();
		}
		
		//var_dump($cnt); exit();
		$_SESSION['cart'][$this->cartId]['vouchers'][$cnt]=$voucher;
		
		//var_dump($_SESSION['cart'][$this->cartId]['vouchers']); exit();
	}
	
	public function getVouchers(){
		return isset($_SESSION['cart'][$this->cartId]['vouchers']) ? $_SESSION['cart'][$this->cartId]['vouchers'] : array();
	}
	
	public function removeVouchers(){ 
		$_SESSION['cart'][$this->cartId]['vouchers']=array();
	}
	
	public function removeVoucher($cnt){
		if(isset($_SESSION['cart'][$this->cartId]['vouchers'])){
			//Getting the Promocode
			$voucherData=$_SESSION['cart'][$this->cartId]['vouchers'][$cnt];
			
			if($voucherData){
				$grouponCode=$voucherData['code'];
				$this->removeTicketGroupon($grouponCode);
				$_SESSION['cart'][$this->cartId]['vouchers'][$cnt]=array();
				//var_dump($grouponCode); exit();
			}
			//var_dump($voucherData); exit();
			
			
			//$_SESSION['cart'][$this->cartId]['vouchers'][$cnt]=array();
		}
		
	}
	
	public function removeTicketGroupon($grouponCode){
		$updateTicketsList=array();
		$ticketsList=$this->getTicketsList();
		
		$action="pending";
		foreach($ticketsList as $ticket){
			
			
			if($ticket['grouponcode']==$grouponCode && $action=="pending"){
				$ticket['dealprice']=$ticket['price'];
				$ticket['grouponcode']='';
				$action="completed";
			}
			$updateTicketsList[]=$ticket;
		}
		$this->updateTicketsListGroupon($updateTicketsList);
		//var_dump($ticketsList); exit();
	}
	
	public function checkGroupon($voucherCode){
		$result=false;
		$vouchersList=$this->getVouchers();
		
		foreach($vouchersList as $voucher){
			if(isset($voucher['code'])){
				if($voucher['code']==$voucherCode){
					$result=true;
					break;
				}
			}
			
		}
		return $result;
	}
	
	
	public function resetCouponNGrouponCodes(){
		$this->removePromoCode();
		
		$ticketsList=$this->getTicketsList();
		$updateTicketsList=array();
		foreach($ticketsList as $ticket){
			if(!empty($ticket['grouponcode'])){
				$ticket['dealprice']=$ticket['price'];
				$ticket['grouponcode']='';
			}
			$updateTicketsList[]=$ticket;
		}
		$this->updateTicketsListGroupon($updateTicketsList);
		
		
		$this->removeVouchers();
		
	}
	
	
	
	//Giftcard
	public function addGiftCard($giftcard){
		$_SESSION['cart'][$this->cartId]['giftcard']=$giftcard;
	}
	
	
	public function getGiftCard(){
		return isset($_SESSION['cart'][$this->cartId]['giftcard']) ? $_SESSION['cart'][$this->cartId]['giftcard'] : array();
	}
	
	public function getGiftCardAmount(){
		$amount=0;
		$giftCardArray=$this->getGiftCard();
		$giftCardCode='';
		$giftcardhtml='';
		
		if(is_array($giftCardArray)){
			foreach($giftCardArray as $key=>$values){
				$giftCardCode=$key;
				$amount=$giftCardArray[$key]['deductedamount'];
			}
		}
	
		
		return $amount;
	}
	
	
	public function removeGiftCard(){
		$_SESSION['cart'][$this->cartId]['giftcard']=array();
	}
	
	
	
	public function storeCart(){
		try{
			$res = db_insert('booking_cart')
			->fields(array(
					'orderId' =>$this->getUberCartOrderId(),
					'cartId'=>$this->cartId(),
					'cartData'=>serialize($this->getCartData()),
					'created'=>date('m/d/Y h:i:a')
			))
			->execute();
		}catch(Exception $e){
			
		}
		
	}
	
	
	
	public static function getCartDBData($cartId,$orderId){
		try{
			
			$res=db_select('booking_cart','bc')
			->fields('bc',array('cartData'))
			->condition('cartId',$cartId,'=')
			->condition('orderId',$orderId,'=')
			->execute()->fetchField();
			
			return $res;
		}catch(Exception $e){
			
		}
		
	}
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
}