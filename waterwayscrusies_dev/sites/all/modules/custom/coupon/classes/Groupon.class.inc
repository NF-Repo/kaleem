<?php
class Groupon{
	
	
	public function applyGroupon($grouponcode,$orderobj){
		
		
		//unset($_SESSION['order_groupons']); exit();
		//drupal_json_output($_SESSION['order_groupons']); exit();
		//var_dump($grouponcode); exit();
		$coupondata=$this->validateGroupon($grouponcode, $orderobj->coupontype);
		
		
		//var_dump($coupondata); exit();
		//check groupon usuage
		
		if(isset($coupondata)){
			if($coupondata[0]->is_used=="0"){

				$event_id = isset($orderobj->events[0]->eventid) ? $orderobj->events[0]->eventid : '';
				$ordercruisetemplateid = Scheduledevents:: getCruiseTemplateIdByEventId($event_id);
				if ($orderobj->coupontype == 'groupon') {
					if ($ordercruisetemplateid != $coupondata[0]->deal_cruise_template) {
						return FALSE;
					}
				}
					
				//var_dump('asdf'); exit();
				
				$eventdata = array();
				$eventdata = geteventscheduledate($orderobj->events[0]->eventid);
					
				if (count($eventdata) > 0) {
					$eventdatetime = $eventdata[0]->scheduleddate;
					$couponcruise_startdate = $coupondata[0]->crusie_startdate;
					$couponcruise_enddate = $coupondata[0]->crusie_enddate;
					$couponcruise_start_time = strtotime($couponcruise_startdate . ' 00:00:00');
					$couponcruise_end_time = strtotime($couponcruise_enddate . ' 23:59:59');
				
					if ($eventdatetime >= $couponcruise_start_time && $eventdatetime <= $couponcruise_end_time) {
							
							
						$discountamount = $this->calculategroupondiscount($grouponcode, $orderobj);
				
						//var_dump($discountamount); exit();
						return $discountamount;
					} else {
						return FALSE;
					}
				} else {
					return FALSE;
				}
			}else{
				return FALSE;
			}
		}
		
		
		
		
// 		if (!$coupondata) {
// 			return FALSE;
// 		} else {
			
			
		
// 		}
		
		//var_dump($coupondata); exit();
		
	}
	
	public function getgroupondatabycode($grouponcode) {
		
		//var_dump($grouponcode); exit();
	
		$query = db_select('deal_grouponcodes', 's');
		$query->join('coupon', 'c', 'c.id = s.deal_coupon_id');
		$query->fields('s');
		$query->fields('c');
		$query->condition('s.groupon_code', $grouponcode, '=');
		$result = $query->execute();
		$groupondata = array();
		foreach ($result as $grouponrow) {
			$groupondata[] = $grouponrow;
		}
		return $groupondata;
	}
	
	
	
	
	public function validateGroupon($couponcode, $coupontype) {
		
		//var_dump($coupontype); exit();
		
		$coupondata = array();
		$couponcodedata = array();
		if ($coupontype == 'groupon') {
			$couponcodedata = $this->getgroupondatabycode($couponcode);
		
			if (!isset($couponcodedata[0]->type)) {
				return FALSE;
			}
		}
	
	
		 
	
		/*  check whether the coupon is active or not   */
		if (isset($couponcodedata[0]->couponactive) && $couponcodedata[0]->couponactive == 1) {
			/*  check the coupon valid date type    */
			if (isset($couponcodedata[0]->coupon_validatetype) && $couponcodedata[0]->coupon_validatetype == 'sequential') {
				$startdate = $couponcodedata[0]->promo_startdate;
				$enddate = $couponcodedata[0]->promo_enddate;
				$currenttime = time();
				$start_time = strtotime($startdate);
				$end_time = strtotime($enddate);
				if ($currenttime >= $start_time && $currenttime <= $end_time) {
					 
					 
					return $couponcodedata;
				} else {
					return FALSE;
				}
			} else {
				if (isset($couponcodedata[0]->coupon_validatetype) && $couponcodedata[0]->coupon_validatetype == 'multiple') {
					/*  check the coupon valid multiple date type    */
					$sel1ecteddates = isset($couponcodedata[0]->coupon_discounteddates) ? $couponcodedata[0]->coupon_discounteddates : '';
					$datesarray = array();
					if ($sel1ecteddates != '') {
						$datesarray = explode(',', $sel1ecteddates);
					}
					$currenttime = time();
					$validcoupondate = FALSE;
					for ($i = 0; $i < count($datesarray); $i++) {
						$datetime = '';
						$datetime = strtotime($datesarray[$i]);
						$extendtime = '';
						$extendtime = $datetime + (60 * 60 * 24);
						if ($currenttime >= $datetime && $currenttime <= $extendtime) {
							$validcoupondate = TRUE;
						}
					}
					if ($validcoupondate) {
						return $couponcodedata;
					} else {
						return FALSE;
					}
				} else {
					return FALSE;
				}
			}
		} else {
			return FALSE;
		}
	}
	
	
	public function calculategroupondiscount($couponcode, $orderobj) {
		 
		$grouponSellPrice=false;
		$currentgroupondata = getgroupondatabycode($couponcode);
		$currentgrouponticketdata = isset($currentgroupondata[0]->deal_cruise_tickets) ? $currentgroupondata[0]->deal_cruise_tickets : '';
	
		$deal_sell_price=0;
		if ($currentgrouponticketdata != '') {
			$currentgroupontickets = explode(',', $currentgrouponticketdata);
			$deal_sell_price=$currentgroupondata[0]->deal_sell_price;
		}
		
		$bookingCart=new BookingCart();
		$bookingCartTickets=$bookingCart->getTicketsList();
		
		$updateTicketsList=array();
		$grouponApplied="pending";
		foreach($bookingCartTickets as $ticket){
			if($grouponApplied=="pending"){
				if(in_array($ticket['ticketid'], $currentgroupontickets) && empty($ticket['grouponcode'])){
					$ticket['dealprice']=$deal_sell_price;
					$ticket['grouponcode']=$couponcode;
					$grouponSellPrice=$deal_sell_price;
					$grouponApplied="completed";
				}
			}
			$updateTicketsList[]=$ticket;
	
		}
		
		$bookingCart->updateTicketsListGroupon($updateTicketsList);
		
		return $grouponSellPrice;
		//drupal_json_output($updateTicketsList);
		//exit();
		

	}
	
}