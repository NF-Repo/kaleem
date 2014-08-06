<?php
function bookingcartpage(){
	$bookingCart = new BookingCart();
	
	$cartid=$bookingCart->getCardId();
	
	
	
	drupal_json_output($cartid);
	exit();
	
}