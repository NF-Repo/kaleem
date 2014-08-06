<?php




class GiftCard {
	
	
// 	public $pregiftcardamount;
// 	public $customgiftcardamount;
// 	public $recipientemail;
// 	public $yourname;
// 	public $customermessage;
// 	public $deliverydate;
// 	public $promotioncode;
// 	public $customerfirstname;
// 	public $customerlastname;
// 	public $customeraddress;
// 	public $customersecondaryaddress;
// 	public $customercity;
// 	public $customerstate;
// 	public $customerzipcode;
// 	public $customerphonenumber;
// 	public $customeremail;
// 	public $knownby;
// 	public $receiveemailoption;
	
	public $data;
	
	
	
	
	public $order_id;
	public $uid;
	public $giftcard_qty;
	public $giftcard_price;
	public $giftcard_totalprice;
	public $created;
	
	public function loggingGiftCardOrder() {
		try{
			$query = db_insert ( 'giftcard_orders' );
			$query->fields ( array (
					'order_id' => $this->order_id,
					'uid' => $this->uid,
					'giftcard_qty' => $this->giftcard_qty,
					'giftcard_price' => $this->giftcard_price,
					'giftcard_totalprice' => $this->giftcard_totalprice,
					'created' => $this->created,
					'data'=>$this->data,
			) );
			$query->execute ();
		}catch(Exception $e){
			
		}
		
	}

	public static function getGiftCardOrderData($order_id) {
		$result = db_select ( 'giftcard_orders', 'g' )->fields ( 'g' )->condition ( 'order_id', $order_id, '=' )->execute ()->fetchObject ();
		
		return $result;
	}
}