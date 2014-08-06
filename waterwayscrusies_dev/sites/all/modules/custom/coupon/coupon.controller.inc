
<?php

/**
 * EntityCouponnterface definition.
 *
 * We create an interface here because anyone could come along and
 * use hook_entity_info_alter() to change our controller class.
 * We want to let them know what methods our class needs in order
 * to function with the rest of the module, so here's a handy list.
 *
 * @see hook_entity_info_alter()
 */
interface EntityCouponControllerInterface extends DrupalEntityControllerInterface {

    public function create();

    public function save($coupon);

    public function delete($coupon);
}

/**
 * EntityExampleBasicController extends DrupalDefaultEntityController.
 *
 * Our subclass of DrupalDefaultEntityController lets us add a few
 * important create, update, and delete methods.
 */
class EntityCouponController extends DrupalDefaultEntityController implements EntityCouponControllerInterface {

    /**
     * Create and return a new coupon entity.
     */
    public function create() {
        $coupon = new stdClass();
        $coupon->type = 'coupon';
        $coupon->coupon_code = '';
        $coupon->promo_startdate = '';
        $coupon->promo_enddate = '';
        $coupon->crusie_startdate = '';
        $coupon->crusie_enddate = '';
        $coupon->rate = '';
        $coupon->coupontype = '';
        $coupon->couponactive = '';


        return $coupon;
    }

    /**
     * Saves the custom fields using drupal_write_record()
     */
    public function save($coupon) {

        // If our entity has no id, then we need to give it a
        // time of creation.
        if (isset($coupon->id) && ($coupon->id != null)) {
            try {

                drupal_write_record('coupon', $coupon, array('id'));
                field_attach_update('coupon', $coupon);
            } catch (Exception $e) {
                watchdog("coupon", "Error while Updating coupon");
            }
        } else {
            try {
                drupal_write_record('coupon', $coupon);
                field_attach_insert('coupon', $coupon);
            } catch (Exception $e) {
                watchdog("coupon", "Error while Inserting the coupon");
            }
        }
        module_invoke_all('entity_insert', $cruise, 'coupon');
        return $coupon;
    }

    /**
     * Delete a single entity.
     *
     * Really a convenience function for delete_multiple().
     */
    public function delete($coupon) {
        db_delete('coupon')
                ->condition('id', $coupon, '=')
                ->execute();
    }

    /**
     * Delete one or more coupon entities.
     *
     * Deletion is unfortunately not supported in the base
     * DrupalDefaultEntityController class.
     *
     * @param $ids
     *   An array of entity IDs or a single numeric ID.
     */
    public function delete_multiple($entities) {
        $ids = array();
        if (!empty($entities)) {
            $transaction = db_transaction();
            try {
                foreach ($entities as $coupon) {
                    // Invoke hook_entity_delete().
                    module_invoke_all('entity_delete', $coupon, 'coupon');
                    field_attach_delete('coupon', $coupon);
                    $ids[] = $coupon->id;
                }
                db_delete('coupon')
                        ->condition('id', $ids, 'IN')
                        ->execute();
            } catch (Exception $e) {
                $transaction->rollback();
                watchdog_exception('coupon', $e);
                throw $e;
            }
        }
    }

    public function applycoupon($couponcode, $orderobj) {
    	
    	
    	
        $ordertype = $orderobj->ordertype;
        switch ($ordertype) {
            case 'booking':
                $result = $this->applycoupon_booking($couponcode, $orderobj);
                break;

            case 'giftcrd':
                $result = $this->applycoupon_giftcard($couponcode, $orderobj);
                break;

            default:
                $result = $this->applycoupon_booking($couponcode, $orderobj);
        }
        return $result;
    }

    /*    apply the coupon for an order-Booking    */

    public function applycoupon_booking($couponcode, $orderobj) {

        /* get the coupon info by coupon code   
         * check whether the coupon is active or not
         * check the coupon date-validate type ( sequential date range/multiple dates )
         * check the event is available in this coupon or not
         * check the event date range if available.
         * find out the coupon amaount based on the event quantity and coupon discoun type   
         */
//        $eventid = $orderobj->events[0]->eventid;
        $event_id = isset($orderobj->events[0]->eventid) ? $orderobj->events[0]->eventid : '';
        $ordercruisetemplateid = Scheduledevents:: getCruiseTemplateIdByEventId($event_id);

        $coupondata = $this->validatecoupon($couponcode, $orderobj->coupontype);

        
       
        $result = '';
        
        

        if ($orderobj->coupontype != 'groupon') {
            $couponevents = array();
            if (isset($coupondata[0]->eventids) && $coupondata[0]->eventids != '') {
                $couponevents = explode(',', $coupondata[0]->eventids);
            }

            if (!in_array($orderobj->events[0]->eventid, $couponevents)) {
                return FALSE;
            }
        }
        
      


        if (!$coupondata) {
            return FALSE;
        } else {

            if ($orderobj->coupontype == 'groupon') {
                if ($ordercruisetemplateid != $coupondata[0]->deal_cruise_template) {
                    return FALSE;
                }
            }

            $eventdata = array();
            $eventdata = geteventscheduledate($orderobj->events[0]->eventid);

            if (count($eventdata) > 0) {
                $eventdatetime = $eventdata[0]->scheduleddate;
                $couponcruise_startdate = $coupondata[0]->crusie_startdate;
                $couponcruise_enddate = $coupondata[0]->crusie_enddate;
                $couponcruise_start_time = strtotime($couponcruise_startdate . ' 00:00:00');
                $couponcruise_end_time = strtotime($couponcruise_enddate . ' 23:59:59');

                if ($eventdatetime >= $couponcruise_start_time && $eventdatetime <= $couponcruise_end_time) {
                    if ($orderobj->coupontype == 'groupon') {
                    	
                    	
                        $discountamount = $this->calculategroupondiscount($couponcode, $orderobj);
                        
                        
                        
                    } else {
                        $discountamount = $this->calculatediscountamount($couponcode, $orderobj);
                    }

                    return $discountamount;
                } else {
                    return FALSE;
                }
            } else {
                return FALSE;
            }
        }
    }

    /*   apply the Groupon code for an order based on tickets   */

    public function calculategroupondiscount($couponcode, $orderobj) {
    	
    	
        /*
         * Get the Ordered tickets data by unserialise
         * set the array by sorting based on price
         */
        $tickets = array();
        $unserialisedtickets = $orderobj->events[0]->tickets;
        $ordertickets = array();
        $orderticketsdata = array();
        
        
        //$unserialisedtickets = unserialize($tickets);
        
       // var_dump($unserialisedtickets); exit();
       // var_dump($unserialisedtickets); exit();
        for ($i = 0; $i < count($unserialisedtickets); $i++) {
            $ticketID = $unserialisedtickets[$i]['ticketid'];
//            $isChildTicket = CruiseTickets::isChildTicket($ticketID);
//            if (!$isChildTicket) {
            $tickeobj = new stdClass();
            $tickeobj->ticketid = $unserialisedtickets[$i]['ticketid'];
            $tickeobj->qty = $unserialisedtickets[$i]['qty'];
            $tickeobj->ticketname = $unserialisedtickets[$i]['ticketname'];
            $tickeobj->price = $unserialisedtickets[$i]['price'];
            $tickeobj->grouponqty = 0;
            $ordertickets[$unserialisedtickets[$i]['ticketid']] = $unserialisedtickets[$i]['price'];
            $orderticketsdata[$unserialisedtickets[$i]['ticketid']] = $tickeobj;
//            }
        }
        
       
        
        
        krsort($ordertickets);
        $orderticketskeys = array_keys($ordertickets);
        
        
        
        
       /*
        * Get the all groupon code applied by the order from session
        * Recalculate the ticketdiscount of existed promocodes
        * and find out the balanced tickets to the current applicable groupon code
        */
        $ordergroupons = array();
        $ordergroupons = $_SESSION['order_groupons'];
        
        var_dump($ordergroupons); exit();
        
        $applicableordergroupons = array();
        foreach ($ordergroupons as $ordergroupon) {
            $groupontickets = array();
            $groupondata = getgroupondatabycode($ordergroupon);
            $grouponticketdata = isset($groupondata[0]->deal_cruise_tickets) ? $groupondata[0]->deal_cruise_tickets : '';
            if ($grouponticketdata != '') {
                $groupontickets = explode(',', $grouponticketdata);
            }
            $grouponapplicable = FALSE;
            foreach ($orderticketskeys as $orderticketskey) {
                if (in_array($orderticketskey, $groupontickets) && !$grouponapplicable) {
                    $obj = new stdClass();
                    $obj = $orderticketsdata[$orderticketskey];
                    if ($obj->grouponqty < $obj->qty) {
                        $obj->grouponqty = $obj->grouponqty + 1;
                        $grouponapplicable = TRUE;
                        $ordergrouponobj = new stdClass();
                        $ordergrouponobj->code = $ordergroupon;
                        $ordergrouponobj->amount = $obj->price;
                        $order_groupons[] = $ordergrouponobj;
                    }
                    $orderticketsdata[$orderticketskey] = $obj;
                }
            }
        }
        $grouponapplicable = FALSE;
        
        
        $currentgroupondata = getgroupondatabycode($couponcode);
        
        
        $currentgrouponticketdata = isset($currentgroupondata[0]->deal_cruise_tickets) ? $currentgroupondata[0]->deal_cruise_tickets : '';

        
        if ($currentgrouponticketdata != '') {
            $currentgroupontickets = explode(',', $currentgrouponticketdata);
        }
        $currentdiscount=0;
        foreach ($orderticketskeys as $orderticketskey) {          
            if (in_array($orderticketskey, $currentgroupontickets) && !$grouponapplicable && $currentdiscount==0) {                
                $obj = new stdClass();
                $obj = $orderticketsdata[$orderticketskey];                
                if ($obj->grouponqty < intval($obj->qty)) {                
                    $obj->grouponqty = $obj->grouponqty + 1;
                    $grouponapplicable = TRUE;
                    $ordergrouponobj = new stdClass();
                    $ordergrouponobj->code = $couponcode;
                    $ordergrouponobj->amount = $obj->price;
                    $order_groupons[] = $ordergrouponobj;
                    
                    
                    if (isset($_SESSION['order_groupons'])) {                
                        $currentdiscount = $obj->price;
                        
                        
                        if (is_array($_SESSION['order_groupons'])) {
                            if (!in_array($couponcode, $_SESSION['order_groupons'])) {
                                $_SESSION['order_groupons'][] = $couponcode;
                            }else{
                                $currentdiscount='exist';
                            }
                        } else {
                            $_SESSION['order_groupons'][] = $couponcode;
                            $currentdiscount = $obj->price;
                        }
                    }else{
                         $_SESSION['order_groupons'][] = $couponcode;
                         $currentdiscount = $obj->price;
                    }
                }
                $orderticketsdata[$orderticketskey] = $obj;
            }           
        }
        
       
        return $currentdiscount;
    }

    
    
    
    
    public function applycoupon_giftcard($couponcode, $eventid) {
        
    }

    /*   here coupon type is coupon or vouchure    */

    public function validatecoupon($couponcode, $coupontype) {
        $coupondata = array();
        $couponcodedata = array();
        if ($coupontype == 'groupon') {
            $couponcodedata = getgroupondatabycode($couponcode);
            if (!isset($couponcodedata[0]->type)) {
                return FALSE;
            }
        } else {
            $coupondata = entity_load('coupon', $ids = FALSE, $conditions = array('coupon_code' => $couponcode), $reset = FALSE);
            foreach ($coupondata as $row) {
                $res = array();
                $res = $this->couponappliedeventlist($row->id);
                $row->eventids = $res;
                $couponcodedata[] = $row;
            }
            /*  check whether the coupon type is coupon or vouchure   */
            if (isset($couponcodedata[0]->type) && $couponcodedata[0]->type != $coupontype) {
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

    /*   calculate the discounted price for an orderobject   */

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

    public function updatecouponstatus($coupon) {

        $updatestatusresults = db_update('coupon') // Table name no longer needs {}
                ->fields(array(
                    'used' => 1,
                ))
                ->condition('id', $coupon->id, '=')
                ->execute();
    }

    public function searchcouponfilter() {
        
    }

    public function couponappliedeventlist($couponid) {
        $query = db_select("coupon_event", "c");
        $query->condition('c.coupon_id', $couponid, '=');
        $query->fields('c', array(
            'Event_id'
        ));
        $result = array();
        $result = $query->execute()->fetchAll();
        return $result[0]->Event_id;
    }

    public function couponBulkupload($input) {
        
    }

    public function AvaliablepromocodesByCategory($categoryid) {
        
    }

    public function RedeemedcouponsCategory($categoryid) {
        
    }

}

/**
 * @} End of "defgroup coupon".
 */
