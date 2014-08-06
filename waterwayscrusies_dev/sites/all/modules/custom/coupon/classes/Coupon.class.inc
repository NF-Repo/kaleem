<?php
class Coupon{
	
	public function getApplicableCouponEventsList($couponid) {
		$query = db_select("coupon_event", "c");
		$query->condition('c.coupon_id', $couponid, '=');
		$query->fields('c', array(
				'Event_id'
		));
		$result = array();
		$result = $query->execute()->fetchAll();
		return $result[0]->Event_id;
	}
	
	
	
	public function applyCoupon($couponcode, $orderobj){
		
		$coupondata=$this->validatecoupon($couponcode, $orderobj->coupontype);
		
		
		if($coupondata){
			$eventdata = array();
			$eventdata = geteventscheduledate($orderobj->events[0]->eventid);
			
			if (count($eventdata) > 0) {
				$eventdatetime = $eventdata[0]->scheduleddate;
				$couponcruise_startdate = $coupondata[0]->crusie_startdate;
				$couponcruise_enddate = $coupondata[0]->crusie_enddate;
				$couponcruise_start_time = strtotime($couponcruise_startdate . ' 00:00:00');
				$couponcruise_end_time = strtotime($couponcruise_enddate . ' 23:59:59');
			
				if ($eventdatetime >= $couponcruise_start_time && $eventdatetime <= $couponcruise_end_time) {
						$discountamount = $this->calculatediscountamount($couponcode, $orderobj);
						return  $discountamount [0]->discountamount;
						//return $discountamount;
				} else {
					return FALSE;
				}
			} else {
				return FALSE;
			}
		}else{
			return FALSE;
		}
		
		
		//var_dump($coupondata); exit();

		
	}
	
	
	public function validatecoupon($couponcode, $coupontype) {
		
		
	$coupondata = array();
		$couponcodedata = array();
		$coupondata = entity_load('coupon', $ids = FALSE, $conditions = array('coupon_code' => $couponcode), $reset = FALSE);
		
		if($coupondata){
			foreach ($coupondata as $row) {
				$res = array();
				$res = $this->getApplicableCouponEventsList($row->id);
				$row->eventids = $res;
				$couponcodedata[] = $row;
			}
			
			//var_dump($couponcodedata); exit();
			
			
			
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
					
					var_dump('asfasfd'); exit();
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
		}else{
			return FALSE;
		}
// 		}
	}
	
	
	
	
	
	
	
	
	
	
	
	
public function calculatediscountamount($couponcode, $orderobj) {

        $coupondata = array();
        $coupondata = entity_load('coupon', $ids = FALSE, $conditions = array('coupon_code' => $couponcode), $reset = FALSE);
        $couponcodedata = array();
        foreach ($coupondata as $row) {
            $couponcodedata[] = $row;
        }

        if (isset($couponcodedata[0]->coupontype) && $couponcodedata[0]->coupontype == 'flatfee') {
            $couponcodedata[0]->discountamount = $couponcodedata[0]->rate;
        }
        if (isset($couponcodedata[0]->coupontype) && $couponcodedata[0]->coupontype == 'percentage') {
            $subtotalAmount = $orderobj->ordersubtotal;
            $couponrate = $couponcodedata[0]->rate;
            //Calculate Discount amt
            $discountamt = ($subtotalAmount * $couponrate / 100);
            $couponcodedata[0]->discountamount = $discountamt;
        }
        if (isset($couponcodedata[0]->coupontype) && $couponcodedata[0]->coupontype == '2 for 1') {

            $subtotalAmount = $orderobj->ordersubtotal;
            $tickets = array();
            $tickets = $orderobj->events[0]->tickets;

            $ordertickets = array();
            $totaltickets = 0;
            for ($i = 0; $i < count($tickets); $i++) {
                $ticketID = $tickets[$i]['ticketid'];
                $isChildTicket = CruiseTickets::isChildTicket($ticketID);
                if (!$isChildTicket) {
                    $ordertickets[$tickets[$i]['price']] = $tickets[$i];
                    $totaltickets = $totaltickets + $tickets[$i]['qty'];
                }
            }
            $discountedtickets = ($totaltickets / 2);
            $discountedticketsamount = 0;
            $discountedtickets = round($discountedtickets, 0, PHP_ROUND_HALF_DOWN); //7.5 = 7
            ksort($ordertickets);
//            $ordertotal = 0;
            foreach ($ordertickets as $orderticket) {
                if ($discountedtickets > $orderticket['qty']) {
                    $discountedticketsamount = $discountedticketsamount + ($orderticket['qty'] * $orderticket['price']);
                    $discountedtickets = $discountedtickets - $orderticket['qty'];
                } else {
                    $discountedticketsamount = $discountedticketsamount + ($discountedtickets * $orderticket['price']);
                    $discountedtickets = 0;
                }
//                $ordertotal = $ordertotal + ($orderticket['qty'] * $orderticket['price']);
            }
            $couponcodedata[0]->discountamount = $discountedticketsamount;
        }
        return $couponcodedata;
    }
}