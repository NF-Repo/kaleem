<?php
class _GiftCard {
	public function updateGiftCardStatus($giftCardArray, $order) {
		/*
		 * Gift Card
		 */
		$giftCardCode = '';
		$giftCardAmount = '';
		
		foreach ( $giftCardArray as $key => $values ) {
			$giftCardCode = $key;
			$giftCardTotalAmt = $giftCardArray [$key] ['totalgiftcardprice'];
			$giftCardAmount = $giftCardArray [$key] ['deductedamount'];
		}
		try {
			// Preparing GiftCard Data
			$giftCardData = array ();
			$giftCardData ['accountId'] = $giftCardCode;
			$giftCardData ['entryType'] = 'K';
			$giftCardData ['pin'] = null;
			$giftCardData ['valueCode'] = 'USD';
			$giftCardData ['enteredAmount'] = $giftCardAmount;
			$giftCardData ['customerType'] = '1'; // primary::1,Alternate::2,Giver of Card::3
			$giftCardData ['firstName'] = $order->billing_first_name;
			$giftCardData ['middleName'] = "";
			$giftCardData ['lastName'] = $order->billing_last_name;
			$giftCardData ['address1'] = $order->billing_street1;
			$giftCardData ['address2'] = "";
			$giftCardData ['city'] = $order->billing_city;
			$giftCardData ['state'] = uc_zone_get_by_id ( $order->billing_zone );
			$giftCardData ['postal'] = $order->billing_postal_code;
			$giftCardData ['country'] = '';
			$giftCardData ['mailPref'] = 'I';
			$giftCardData ['phone'] = $userphone; // $customerPhone;
			$giftCardData ['isMobile'] = '';
			$giftCardData ['phonePref'] = 'I'; // depends upon phone number availability if not available then it is o
			$giftCardData ['email'] = $order->primary_email;
			$giftCardData ['emailPref'] = 'I';
			$giftCardData ['birthday'] = '';
			$giftCardData ['anniversary'] = '';
			$giftCardData ['gender'] = ''; // not available
			                              // Pre Log
			$giftCardObject = new GiftCardBookingOrder ();
			$giftCardObject->giftcard_account = $giftCardCode;
			$giftCardObject->giftcard_totalamt = $giftCardTotalAmt;
			$giftCardObject->giftcard_deductedamt = $giftCardAmount;
			$giftCardObject->order_id = $order->order_id;
			$giftCardObject->uid = $order->uid;
			$giftCardObject->status = "Created";
			$giftCardObject->created = REQUEST_TIME;
			$giftCardObject->data = serialize ( $giftCardData );
			BookingLog::logGiftCard ( $giftCardObject );
			giftcard_profitpointapi_giftredemption ( "GiftRedemption", $giftCardData, $giftCardObject );
		} catch ( Exception $e ) {
			watchdog ( "Error occured in giftcard updation in booking orders", $e );
		}
	}

}