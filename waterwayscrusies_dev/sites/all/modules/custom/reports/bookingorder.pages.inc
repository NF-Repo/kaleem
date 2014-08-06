<?php
function bookingorder_view_page($order_id){
	return bookingorderpagetemplate($order_id);
	
}



function bookingorderpagetemplate($orderId) {
	
	drupal_set_title("Booking Order : ".$orderId);
	global $base_url;
	$html = '';
	$order = uc_order_load($orderId);
	$ordercreated = date('m-d-Y', $order->created);
	$reservationicketdata = Reservation::getReservationTicketData($orderId);
	$reservationenhancedata = Reservation::getReservationEnhancementsData($orderId);

	$userChoiceObject = BookingLog::getUserChoices($orderId);
	$specialconcerns = isset($userChoiceObject->another_party_comments) ? $userChoiceObject->another_party_comments : '';
	$comments = isset($userChoiceObject->celebration_comments) ? $userChoiceObject->celebration_comments : '';

	$ordertotal=$order->order_total;
	//$ordertotal = isset($userChoiceObject->order_total) ? $userChoiceObject->order_total : 0;
	$ticketshtml = '';
// 	foreach ($reservationicketdata as $reservationicketrow) {

// 		$enteydata = array();

// 		for ($i = 0; $i < $reservationicketrow['qty']; $i++) {
// 			$entryname = '';
// 			if ($reservationicketrow['ticketentreeids'][$i] != '') {
// 				$entryname = CruiseEntrees::getCruiseEntreeNamebyId($reservationicketrow['ticketentreeids'][$i]);
// 			}

// 			$ticketshtml.='<tr>
//                         <td width="20%">1</td>
//                         <td width="25%">' . $reservationicketrow['ticketname'] . '</td>
//                         <td width="35%">' . $entryname . '</td>
//                         <td width="20%">' . uc_currency_format($reservationicketrow['price']) . '</td>
//                        </tr>';
// 		}
// 	}
	
	
	
	$ticketsdata=$reservationicketdata;
	for($i=0;$i<count($ticketsdata);$i++){
		$ticketId=$ticketsdata[$i]['ticketid'];
		$noOfTickets=(int) $ticketsdata[$i]['qty'];
		if($noOfTickets>0){
			$price = $ticketsdata[$i]['price'];
			$ticketName=cruisetickets_load($ticketsdata[$i]['ticketid'])->title;
			for($j=0;$j<$noOfTickets;$j++){
				$id = $ticketsdata[$i]['ticketentreeids'][$j];
				$cruiseEntreeObject = cruiseentrees_load($id);
				$entreeName = $cruiseEntreeObject->title;
				
				if($entreeName==''){
					$entreeName='No options';
				}
				$commentsString = $ticketsdata[$i]['ticketentreescomments'][$j];
				$commentArray=explode('-', $commentsString);
				$comment='';
				if(isset($commentArray[2]) && $commentArray[2]!=''){
					$comment = '<p class="ticketdesc">D.R: <i>' . $commentArray[2] . '</i></p>';
				}
	
				$ticketshtml.='<tr>';
				$ticketshtml .= '<td>1</td>';
				$ticketshtml .= '<td>' . $ticketName . '</td>';
				$ticketshtml .= '<td>' . $entreeName .$comment. '</td>';
				$ticketshtml .= '<td></td>';
				$ticketshtml .= '<td>$' . wwcFormatPrice($price) . '</td>';
				$ticketshtml .= '</tr>';
			}
		}
	
	}
	
	
$ordercommentshtml='<tr><td width="10%">Special Concerns : <i>'.$specialconcerns . '<i></td></tr>';
$ordercommentshtml.='<tr><td width="10%">Celebration Comments : <i>'.$comments . '</i></td></tr>';

	
	
	$enhancementhtml = '';

	foreach ($reservationenhancedata as $reservationenhancerow) {

		$enhancementhtml.='<tr>
                             <td>' . $reservationenhancerow['qty'] . '</td>
                             <td>' . $reservationenhancerow['name'] . '</td>
                             <td>' . $reservationenhancerow['comments'] . '</td>
                             <td>$' . wwcFormatPrice($reservationenhancerow['price']) . '</td>
                             <td>$' . wwcFormatPrice($reservationenhancerow['qty'] * $reservationenhancerow['price']) . '</td>
                          </tr>';
	}
	
	$enhancementhtmlcontent='';
	if($enhancementhtml!=''){

		$enhancementhtmlcontent='<tr>
                                                            <td colspan="2">
                                                                <b>Enhancements:&nbsp;</b>
                                                                <table width="100%">
                                                                    <tbody>
                                                                        <tr>
                                                                            <td width="10%"></td>
                                                                            <td valign="bottom">
                                                                                <table width="100%" style="font-family:verdana,arial,helvetica;font-size:small;background:none!important" border="0" cellspacing="0" cellpadding="0" bgcolor="#ccc" align="center">
                                                                                    <tbody>
                                                                                        <tr>
                                                                                            <td width="10%"><b>Quantity</b></td>
                                                                                            <td width="30%"><b>Enhancement Name</b></td>
                                                                                            <td width="30%"><b>Comments</b></td>
                                                                                            <td width="15%"><b>Unit price</b></td>
                                                                                            <td width="15%"><b>Price</b></td>
                                                                                        </tr>
                                                                                        ' . $enhancementhtml . '
                                                                                    </tbody>
                                                                                </table>
                                                                            </td>
                                                                        </tr>
                                                                    </tbody>
                                                                </table>
                                                            </td>
                                                        </tr>';
		
	}
	
	

	$orderlineitems = isset($order->line_items) ? $order->line_items : array();
	

	//drupal_json_output($orderlineitems); exit();
	
	$orderlineitemhtml='';
	
	$feeitemshtml='';
	$vouchercodeshtml='';
	$promocodehtml='';
	$giftcardhtml='';
	$taxhtml='';
	$additionaltaxhtml='';
	$additionaldiscounthtml='';
	foreach($orderlineitems as $lineitem){
		
		
		//fee items
		if($lineitem['data']['name']=="FEE_ITEMS"){
			$feeitemshtml.='<tr>
                     <td></td>
                     <td></td>
                     
                     <td colspan="2" style="text-align:right;">' . $lineitem['title'] . ':</td>
                     <td>$' . wwcFormatPrice($lineitem['amount']) . '</td>
                     </tr>';
			
			
		}
		
		//voucher codes
		if($lineitem['data']['name']=="VOUCHER_CODE"){
			$vouchercodeshtml.='<tr>
                     <td></td>
                     <td></td>
                     
                     <td colspan="2" style="text-align:right;">' . $lineitem['title'] . ':</td>
                     <td>-$' . wwcFormatPrice(abs($lineitem['amount'])) . '</td>
                     </tr>';
		}
		
		//promocode
		if($lineitem['data']['name']=="PROMO_CODE"){
			$promocodehtml.='<tr>
                     <td></td>
                     <td></td>
                    
                     <td colspan="2" style="text-align:right;">' . $lineitem['title'] . ':</td>
                     <td>-$' . wwcFormatPrice(abs($lineitem['amount'])) . '</td>
                     </tr>';
		}
		
		
		//giftcard code
		if($lineitem['data']['name']=="GIFT_CARD"){
			$giftcardhtml.='<tr>
                     <td></td>
                     <td></td>
                    
                     <td colspan="2" style="text-align:right;">' . $lineitem['title'] . ':</td>
                     <td>-$' . wwcFormatPrice(abs($lineitem['amount'])) . '</td>
                     </tr>';
		}
		
		
		//tax
		if($lineitem['data']['name']=="TAX"){
			$taxhtml.='<tr>
                     <td></td>
                     <td></td>
                     
                     <td colspan="2" style="text-align:right;"> <b>' . $lineitem['title'] . ' </b>:</td>
                     <td><b>$' . wwcFormatPrice($lineitem['amount']) . '</b></td>
                     </tr>';
		}
		
		//additional line item tax
		if($lineitem['data']['name']=="ADDITIONAL_CHARGE_TAX"){
			$additionaltaxhtml.='<tr>
                     <td></td>
                     <td></td>
           
                     <td colspan="2" style="text-align:right;"> ' . $lineitem['title'] . ' :</td>
                     <td>$' . wwcFormatPrice($lineitem['amount']) . '</td>
                     </tr>';
		}
		
		//additional discount line item
		if($lineitem['data']['name']=="ADDITIONAL_DISCOUNT"){
			$additionaldiscounthtml.='<tr>
                     <td></td>
                     <td></td>
      
                     <td colspan="2" style="text-align:right;"> ' . $lineitem['title'] . ' </b>:</td>
                     <td>$' . wwcFormatPrice($lineitem['amount']) . '</b></td>
                     </tr>';
		}
		
	}
	
	
// 	$orderSubTotal=CartSession::getOrderSubTotal($orderId);
// 	$subtotalhtml='<tr>
//                      <td></td>
//                      <td></td>
                     
//                      <td colspan="2" style="text-align:right;"><b>' . Subtotal . '</b>:</td>
//                      <td><b>$' . wwcFormatPrice($orderSubTotal) . '</b></td>
//                      </tr>';
	
	
	$ordertotalhtml='<tr>
                     <td></td>
                     <td></td>
                     <td colspan="2" style="text-align:right;"><b>' . Total . '</b>:</td>
                     <td><b>$' . wwcFormatPrice($ordertotal) . '</b></td>
                     </tr>';
	
	

	
	
	$orderlineitemhtml=$additionaltaxhtml.''.$additionaldiscounthtml.''.$feeitemshtml.''.$promocodehtml.''.$giftcardhtml.''.$vouchercodeshtml.''.$taxhtml.''.$ordertotalhtml;
	
	
	//echo $orderlineitemhtml; exit();
	

	
	
	
	


// 	$orderlineitemhtml = '';
// 	foreach ($orderlineitems as $orderlineitem) {
// 		if ($orderlineitem['type'] == 'generic') {
// 			$title = '';
// 			$title = $orderlineitem['title'];
// 			$lineitemtitle = $orderlineitem['title'];
// 			if (strpos($title, 'Promo Code') !== false) {
// 				$lineitemtitle = 'Promo Code';
// 			}
// 			$orderlineitemhtml.='<tr>
//                                  <td width="30%">
//                                     <div style="">' . $lineitemtitle . ' </div>
//                                     <div style="float:right;border-bottom: 1px dotted #000;min-width:70%;"></div>
//                                  </td>
//                                  <td width="70%"><b>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;' . uc_currency_format($orderlineitem['amount']) . '</b></td>
//                               </tr>';
// 		}
// 	}

	$guestmail='';
	$account=user_load($order->uid);
	$guestmail=isset($account->mail) ? $account->mail : '';


	$html = '';
	$html.='<table width="95%" border="0" cellspacing="0" cellpadding="1" align="center" bgcolor="#006699" style="font-family:verdana,arial,helvetica;font-size:small;border: 1px solid #333333">
    <tbody>
        <tr>
            <td>
                <table width="100%" border="0" cellspacing="0" cellpadding="5" align="center" bgcolor="#FFFFFF" style="font-family:verdana,arial,helvetica;font-size:small">
                    <tbody>
                       
                        <tr valign="top">
                            <td>
                                <table cellpadding="4" cellspacing="0" border="0" width="100%" style="font-family:verdana,arial,helvetica;font-size:small">
                                    <tbody>
                                        <tr>
                                            <td colspan="2" bgcolor="#006699" style="color:white">
                                                <b>Purchasing Information:</b>
                                            </td>
                                        </tr>
                                        <tr>
                                            <td colspan="2">
                                                <table width="100%" cellspacing="0" cellpadding="0" style="font-family:verdana,arial,helvetica;font-size:small">
                                                    <tbody>
                                                        <tr>
                                                            <td valign="top" width="50%">
                                                                <b>Billing Address:</b><br>
                                                                ' . $order->billing_first_name . ' ' . $order->billing_last_name . '<br>
                                                                ' . $order->billing_street1 . '<br>
                                                                ' . $order->billing_city . ', ' . uc_zone_get_by_id($order->billing_zone) . ' ' . $order->billing_postal_code . '<br>
                                                                ' . $order->billing_phone.'<br/>'.$order->primary_email.'
                                                            </td>
                                                            <td valign="top" width="50%">
                                                                <b>Guest Address:</b><br>
                                                                ' . $order->delivery_first_name . ' ' . $order->delivery_last_name . '<br>
                                                                ' . $order->delivery_street1 . '<br>
                                                                ' . $order->delivery_city . ', ' . uc_zone_get_by_id($order->delivery_zone) . ' ' . $order->delivery_postal_code . '<br>
                                                                ' . $order->delivery_phone . '<br/>'.$guestmail.'	
                                                            </td>
                                                        </tr>
                                                    </tbody>
                                                </table>
                                            </td>
                                        </tr>
                                        
                                        <tr>
                                            <td nowrap="">
                                                &nbsp;
                                            </td>
                                        </tr>
                                        <tr>
                                            <td colspan="2" bgcolor="#006699" style="color:white">
                                                <b>Order Summary:</b>
                                            </td>
                                        </tr>
                                        <tr>
                                            <td colspan="2">
                                                <table border="0" cellpadding="1" cellspacing="0" width="100%" style="font-family:verdana,arial,helvetica;font-size:small">
                                                    <tbody>
                                                        <tr>
                                                            <td nowrap="">
                                                                <b>Order #:</b>
                                                            </td>
                                                            <td width="98%">
                                                                ' . $orderId . '
                                                            </td>
                                                        </tr>
                                                        <tr>
                                                            <td nowrap="">
                                                                <b>Order Date: </b>
                                                            </td>
                                                            <td width="98%">
                                                                ' . $ordercreated . '
                                                            </td>
                                                        </tr>
                                                                		
                                                                		<tr>
                                            <td nowrap="">
                                                <b>Order Grand Total:</b>
                                            </td>
                                            <td width="98%">
                                                <b>' . uc_currency_format($ordertotal) . '</b>
                                            </td>
                                        </tr>
                                                        <tr>
                                                            <td colspan="2">
                                                                <br/><b>Ticket Details:&nbsp;</b>
                                                                <table width="100%">
                                                                    <tbody>
                                                                        <tr>
                                                                            <td width="10%"></td>
                                                                            <td valign="bottom">
                                                                                <table width="100%" style="font-family:verdana,arial,helvetica;font-size:small;background:none!important" border="0" cellspacing="0" cellpadding="0" bgcolor="#ccc" align="center">
                                                                                    <tbody>
                                                                                        <tr width="10%">
                                                                                            <td width="10%"><b>Quantity</b></td>
                                                                                            <td width="30%"><b>Ticket</b></td>
                                                                                            <td width="30%"><b>Entree</b></td>
                                                                                            <td width="15%"></td>
                                                                							<td width="15%" ><b>Price</b></td>
                                                                		
                                                                		
                                                                                        </tr>
                                                                                        ' . $ticketshtml .'
                                                                                        
                                                                                    </tbody>
                                                                                </table>
                                                                            </td>
                                                                        </tr>
                                                                    </tbody>
                                                                </table>
                                                            </td>
                                                        </tr>
                                                                                        		
                                                    
                                                                                        ' . $enhancementhtmlcontent . '
                                                                                   
                                                                                        		
                                                        <tr>
                                                            <td colspan="2">
                                                             
                                                                <table width="100%">
                                                                    <tbody>
                                                                        <tr>
                                                                            <td width="10%"></td>
                                                                            <td valign="bottom">
                                                                                <table width="100%" style="font-family:verdana,arial,helvetica;font-size:small;background:none!important" border="0" cellspacing="0" cellpadding="0" bgcolor="#ccc" align="center">
                                                                                    <tbody>
                                                                                        <tr>
                                                                                            <td width="10%"></td>
                                                                                            <td width="30%"></td>
                                                                                            <td width="30%"></td>
                                                                                            <td width="15%"></td>
                                                                                            <td width="15%"></td>
                                                                                        </tr>
                                                                                        ' . $orderlineitemhtml . '
                                                                                    </tbody>
                                                                                </table>
                                                                            </td>
                                                                        </tr>
                                                                    </tbody>
                                                                </table>
                                                            </td>
                                                        </tr>                                		
                                                                                        		
                      		
                                                                                        		
                                                                                        		
                                                        <tr>
                                                            <td colspan="2">
                                                                <br/><b>Order Comments:&nbsp;</b>
                                                                <table width="100%">
                                                                    <tbody>
                                                                        <tr>
                                                                            <td width="80%">
                                                                                        ' .$ordercommentshtml.'
                                                                             </td>
                                                                        </tr>
                                                                    </tbody>
                                                                </table>
                                                            </td>
                                                        </tr>                                 		
                                                       
                            
                                                    </tbody>
                                                </table>
                                            </td>
                                        </tr>
                                       
                                    </tbody>
                                </table>
                            </td>
                        </tr>
                    </tbody>
                </table>
            </td>
        </tr>
    </tbody>
</table>';
	return $html;
}