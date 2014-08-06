<?php
class _ReservationLog {
	
	
	public function reservationLog($cartUser,$order,$eventId,$eventDate,$selectedtickets,$selectedenhancements,$cartid,$occasiontype,$occasionadditionalcomments,$specialinstructions,$additionalcomments) {
		
		
		
		
		$guestDOB='';
		if(!empty($cartUser['guest_dob_month'])){
			$guest_dob_month=$cartUser['guest_dob_month'];
			$guest_dob_day=$cartUser['guest_dob_day'];
			$guestDOB=$guest_dob_month."-".$guest_dob_day;
			 
		}
		$newUserObject->guest_dob = $guestDOB;
		 
		 
		$guestAniverssary='';
		if(!empty($cartUser['guest_aniverssary_month'])){
			$guest_aniverssary_month=$cartUser['guest_aniverssary_month'];
			$guest_aniverssary_day=$cartUser['guest_aniverssary_day'];
			$guestAniverssary=$guest_aniverssary_month."-".$guest_aniverssary_day;
		}
		
		
		
		
		
		$userChoicesObj = new UserChoices ();
		$userChoicesObj->celebration_comments = $occasionadditionalcomments;
		$userChoicesObj->celebration_flag = $occasiontype;
		$userChoicesObj->receive_email_notification_flag = (isset($cartUser ['promotiondiscounts']) && !empty($cartUser ['promotiondiscounts'])) ? 1 : 0;
		$userChoicesObj->customer_email = $cartUser ['guest_email'];
		$userChoicesObj->event_id = $eventId;
		$userChoicesObj->food_allergy_flag = (isset($specialinstructions ['foodallergy']) && $specialinstructions ['foodallergy'] != '') ? 1 : 0;
		$userChoicesObj->limited_mobility_flag = (isset($specialinstructions ['limitedmobility']) && $specialinstructions ['limitedmobility'] != '') ? 1 : 0;
		$userChoicesObj->another_party_comments = $additionalcomments;
		$userChoicesObj->another_party_flag = (isset($specialinstructions ['anotherparty']) && $specialinstructions ['anotherparty']) ? 1 : 0;
		$userChoicesObj->referal = isset($cartUser ['findingusby']) ? $cartUser ['findingusby'] : '';
		$userChoicesObj->what_they_brought = "";
		$userChoicesObj->order_id = $order->order_id;
		$userChoicesObj->order_total = $order->order_total;
		$userChoicesObj->uid = $order->uid;
		
		
		BookingLog::logUserChoices ( $userChoicesObj );
		
		$usercruiseinputs = array (
				'cartsessionid' => array (
						$cartid 
				),
				'cruiseproduct' => array (
						'producttickets' => $selectedtickets,
						'cruiseitem' => $eventId 
				),
				'enhacementproducts' => $selectedenhancements,
				'specialinstructions' => array (
						'limitedmobility' => (isset($specialinstructions ['limitedmobility']) && $specialinstructions ['limitedmobility'] != '') ? 1 : 0,
						'foodallergy' => (isset($specialinstructions ['foodallergy']) && $specialinstructions ['foodallergy'] != '') ? 1 : 0,
						'anotherparty' => (isset($specialinstructions ['anotherparty']) && $specialinstructions ['anotherparty']) ? 1 : 0,
						'specialcomments' => $additionalcomments, 
				),
				'celebrationtext' => array (
						'ocassiontypeenabled' => $occasiontype,
						'ocassionaldescription' => $occasionadditionalcomments, 
				),
				'usersdata' => array (
						'email' => $cartUser ['guest_email'], // treating the default email
						'uid' => $order->uid,
						'dob' => $guestDOB,
						'aniversary' => $guestAniverssary,
						'findingusby' => isset($cartUser ['findingusby']) ? $cartUser ['findingusby'] : '',
						'receiveemailnotifications' => (isset($cartUser ['promotiondiscounts']) && $cartUser ['promotiondiscounts'] != '') ? 1 : 0 
				) 
		);
		
		$userchioce = ( object ) array (
				'userchoice' => $usercruiseinputs 
		);
		
		// Note : Loggin User Role
		$loggedUserRole = 'anonymous user';
		$reservationObj = new stdClass ();
		$reservationObj->bookingdate = strtotime ($eventDate);
		$reservationObj->orderid = $order->order_id;
		$reservationObj->uid = $order->uid;
		$reservationObj->bookedby = $loggedUserRole; // Agent or Admin,Web Information
		$reservationObj->data = $userchioce; // using searilazed Object for maintaing the exact array structure default serialized structure maintaing db
		$reservationObj->created = REQUEST_TIME;
		$reservationObj->eventid =$eventId;
		$savereservation = reservation_save ( $reservationObj );
	}
}