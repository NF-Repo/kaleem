<?php

function coupon_form_validate($form, &$form_state) {
    $typeofdiscount = $form_state['values']['type'];
    field_attach_form_validate('coupon', $form_state['values']['coupon'], $form, $form_state);
    $couponid = isset($form_state['values']['couponid']) ? $form_state['values']['couponid'] : '';
    $formstatus = $couponid == '' ? "add" : "edit";
    $createtype = $form_state['values']['createtype'];
    if ($formstatus == 'add') {
        $title = 'Create New ' . $createtype;
        if ($createtype == 'coupon') {
            $title = 'Create New Discount';
        }
    } else {
        $title = 'Edit ' . $createtype;
        if ($createtype == 'coupon') {
            $title = 'Edit Discount';
        }
    }
    $title = ucwords($title);
    drupal_set_title($title, $output = CHECK_PLAIN);
    drupal_add_css(drupal_get_path('module', 'coupon') . '/coupon.css');

    $statusKey = true;
//    return $statusKey;
//    //couponcode


    if ($form_state['values']["coupon_code"] == '') {
        $statusKey = false;
        form_set_error('coupon_code', "Code field is required.");
    }

    if ($form_state['values']["operation"] != 'edit') {
        $couponcode = $form_state['values']['coupon_code'];
        $duplicateecoupons = couponexists($couponcode);

        if ($duplicateecoupons) {



            if ($typeofdiscount == 'coupon') {

                form_set_error('coupon_code', "This coupon exists please try with another coupon.");
            } else if ($typeofdiscount == 'voucher') {

                form_set_error('coupon_code', "This Voucher exists please try with another Voucher.");
            }
            $statusKey = false;
        }
    }

    //coupon Type
    $coupontype = $form_state['values']['coupontype'];

    if ($coupontype == 'percentage') {
        $couponrate = $form_state['values']['rate'];
        if (!is_numeric($couponrate) && $couponrate != '') {
            $statusKey = false;
            form_set_error('rate', 'Coupon rate will accept only numbers');
        }

        if (!is_couponrate($coupontype, $couponrate)) {
            $statusKey = false;

            if ($typeofdiscount == 'coupon') {
                form_set_error('rate', 'Coupon rate should not be empty');
            } else if ($typeofdiscount == 'voucher') {
                form_set_error('rate', 'Voucher rate should not be empty');
            }
        }
    }

    if ($coupontype == 'flatfee') {
        $couponrate = $form_state['values']['flatfeerate'];

        if (!is_numeric($couponrate) && $couponrate != '') {
            $statusKey = false;
            form_set_error('rate', 'Coupon rate will accept only numbers');
        }
        if (!is_couponrate($coupontype, $couponrate)) {
            $statusKey = false;
            if ($typeofdiscount == 'coupon') {
                form_set_error('rate', 'Coupon rate should not be empty');
            } else if ($typeofdiscount == 'voucher') {
                form_set_error('flatfeerate', 'Voucher rate should not be empty');
            }
        }
    }


    if ($coupontype == '2 for 1') {
        $couponrate = $form_state['values']['twoforonerate'];
        if (!is_numeric($couponrate) && $couponrate != '') {
            $statusKey = false;
            form_set_error('rate', 'Coupon rate will accept only numbers');
        }
//        if (!is_couponrate($coupontype, $couponrate)) {
//            $statusKey = false;
//            form_set_error('twoforonerate', 'Coupon rate should not be empty');
//        }
    }


    //
////promo start Date & End date
    $promostartdate = $form_state['values']['promo_startdate'];
    $promoenddate = $form_state['values']['promo_enddate'];
    if ($form_state['values'] ["coupon_validatetype"] != 'multiple') {
//        if ($form_state['values']['promo_startdate']['date'] == '') {
        if ($form_state['values']['promo_startdate'] == '') {
            $statusKey = false;
            form_set_error('promo_startdate', 'Please enter promotion start date');
        }
    } else {

        if ($form_state['input']['multidate'] == '') {
            $statusKey = false;
            form_set_error('multidate', 'Please enter promotion start and end dates');
        }
    }

//    if ($form_state['values']['promo_startdate']['date'] != '') {
    if ($form_state['values']['promo_startdate'] != '') {
        if (!is_valid_promodates($promoenddate, $promostartdate)) {
            $statusKey = false;
            form_set_error('promo_enddate', 'Promotion end date should be greater than or equal to promotion start Date');
        }
    }


    ////Cruise start date
//    if ($form_state['values']['crusie_startdate']['date'] == '') {
    if ($form_state['values']['crusie_startdate'] == '') {
        $statusKey = false;
        form_set_error('crusie_startdate', 'Please enter cruise start date');
    }

////Cruise start date&End date
    $crusiestartdate = $form_state['values']['crusie_startdate'];
    $crusieenddate = $form_state['values']['crusie_enddate'];


//    if ($form_state['values']['crusie_startdate']["date"] != '') {
    if ($form_state['values']['crusie_startdate'] != '') {
        if (!is_valid_crusiedates($crusieenddate, $crusiestartdate)) {
            $statusKey = false;
            form_set_error('crusie_enddate', 'Cruise end date should be greater than or equal to cruise start date');
        }
    }



//coupon Events
    $events = $form_state['values']['events'];
    $eventchecked = FALSE;
    foreach ($events as $event) {
        if ($event != 0) {
            $eventchecked = TRUE;
        }
    }
    if (!$eventchecked) {
        $statusKey = false;
        form_set_error('events', 'Atleast one Event required');
    }
}

function groupon_form_validate($form, &$form_state) {
//    $typeofdiscount = $form_state['values']['type'];
//    field_attach_form_validate('coupon', $form_state['values']['coupon'], $form, $form_state);
//    $couponid = isset($form_state['values']['couponid']) ? $form_state['values']['couponid'] : '';
//    $formstatus = $couponid == '' ? "add" : "edit";
//    $createtype = $form_state['values']['createtype'];
//    if ($formstatus == 'add') {
//        $title = 'Create New ' . $createtype;
//        if ($createtype == 'coupon') {
//            $title = 'Create New Discount';
//        }
//    } else {
//        $title = 'Edit ' . $createtype;
//        if ($createtype == 'coupon') {
//            $title = 'Edit Discount';
//        }
//    }
//    $title = ucwords($title);
//    drupal_set_title($title, $output = CHECK_PLAIN);
//    drupal_add_css(drupal_get_path('module', 'coupon') . '/coupon.css');
//
//    $statusKey = true;
////    return $statusKey;
////    //couponcode
//
//
//    if ($form_state['values']["coupon_code"] == '') {
//        $statusKey = false;
//        form_set_error('coupon_code', "Code field is required.");
//    }
//
//    if ($form_state['values']["operation"] != 'edit') {
//        $couponcode = $form_state['values']['coupon_code'];
//        $duplicateecoupons = couponexists($couponcode);
//
//        if ($duplicateecoupons) {
//
//
//
//            if ($typeofdiscount == 'coupon') {
//
//                form_set_error('coupon_code', "This coupon exists please try with another coupon.");
//            } else if ($typeofdiscount == 'voucher') {
//
//                form_set_error('coupon_code', "This Voucher exists please try with another Voucher.");
//            }
//            $statusKey = false;
//        }
//    }
//
//    //coupon Type
//    $coupontype = $form_state['values']['coupontype'];
//
//    if ($coupontype == 'percentage') {
//        $couponrate = $form_state['values']['rate'];
//        if (!is_numeric($couponrate) && $couponrate != '') {
//            $statusKey = false;
//            form_set_error('rate', 'Coupon rate will accept only numbers');
//        }
//
//        if (!is_couponrate($coupontype, $couponrate)) {
//            $statusKey = false;
//
//            if ($typeofdiscount == 'coupon') {
//                form_set_error('rate', 'Coupon rate should not be empty');
//            } else if ($typeofdiscount == 'voucher') {
//                form_set_error('rate', 'Voucher rate should not be empty');
//            }
//        }
//    }
//
//    if ($coupontype == 'flatfee') {
//        $couponrate = $form_state['values']['flatfeerate'];
//
//        if (!is_numeric($couponrate) && $couponrate != '') {
//            $statusKey = false;
//            form_set_error('rate', 'Coupon rate will accept only numbers');
//        }
//        if (!is_couponrate($coupontype, $couponrate)) {
//            $statusKey = false;
//            if ($typeofdiscount == 'coupon') {
//                form_set_error('rate', 'Coupon rate should not be empty');
//            } else if ($typeofdiscount == 'voucher') {
//                form_set_error('flatfeerate', 'Voucher rate should not be empty');
//            }
//        }
//    }
//
//
//    if ($coupontype == '2 for 1') {
//        $couponrate = $form_state['values']['twoforonerate'];
//        if (!is_numeric($couponrate) && $couponrate != '') {
//            $statusKey = false;
//            form_set_error('rate', 'Coupon rate will accept only numbers');
//        }
////        if (!is_couponrate($coupontype, $couponrate)) {
////            $statusKey = false;
////            form_set_error('twoforonerate', 'Coupon rate should not be empty');
////        }
//    }
//
//
//    //
//////promo start Date & End date
//    $promostartdate = $form_state['values']['promo_startdate'];
//    $promoenddate = $form_state['values']['promo_enddate'];
//    if ($form_state['values'] ["coupon_validatetype"] != 'multiple') {
////        if ($form_state['values']['promo_startdate']['date'] == '') {
//        if ($form_state['values']['promo_startdate'] == '') {
//            $statusKey = false;
//            form_set_error('promo_startdate', 'Please enter promotion start date');
//        }
//    } else {
//
//        if ($form_state['input']['multidate'] == '') {
//            $statusKey = false;
//            form_set_error('multidate', 'Please enter promotion start and end dates');
//        }
//    }
//
////    if ($form_state['values']['promo_startdate']['date'] != '') {
//    if ($form_state['values']['promo_startdate'] != '') {
//        if (!is_valid_promodates($promoenddate, $promostartdate)) {
//            $statusKey = false;
//            form_set_error('promo_enddate', 'Promotion end date should be greater than or equal to promotion start Date');
//        }
//    }
//
//
//    ////Cruise start date
////    if ($form_state['values']['crusie_startdate']['date'] == '') {
//    if ($form_state['values']['crusie_startdate'] == '') {
//        $statusKey = false;
//        form_set_error('crusie_startdate', 'Please enter cruise start date');
//    }
//
//////Cruise start date&End date
//    $crusiestartdate = $form_state['values']['crusie_startdate'];
//    $crusieenddate = $form_state['values']['crusie_enddate'];
//
//
////    if ($form_state['values']['crusie_startdate']["date"] != '') {
//    if ($form_state['values']['crusie_startdate'] != '') {
//        if (!is_valid_crusiedates($crusieenddate, $crusiestartdate)) {
//            $statusKey = false;
//            form_set_error('crusie_enddate', 'Cruise end date should be greater than or equal to cruise start date');
//        }
//    }
//
//
//
////coupon Events
//    $events = $form_state['values']['events'];
//    $eventchecked = FALSE;
//    if (is_array($events)) {
//        foreach ($events as $event) {
//            if ($event != 0) {
//                $eventchecked = TRUE;
//            }
//        }
//    }
//
//    if (!$eventchecked) {
//        $statusKey = false;
//        form_set_error('events', 'Atleast one Event required');
//    }
}
