<?php
function giftcard_order_view_page($order_id){
	
	return drupal_get_form("giftcardorderpagetemplate_form",$order_id);
	
}



function giftcardorderpagetemplate_form($form, &$form_state ,$order_id) {
	
	//drupal_add_js(drupal_get_path('module', 'giftcard') . '/js/giftcard.js');
        $form=array();
	$eachGiftCardAmt=0;
	$giftcardorderdata=GiftCard::getGiftCardOrderData($order_id);
        
	$giftCardOrderData=unserialize($giftcardorderdata->data);
	$orderstatus=db_select('uc_orders','u')
                 ->fields('u',array('order_status'))
                 ->condition('order_id', $order_id ,'=')
                 ->execute()
                ->fetchAssoc();
                

	//var_dump($giftCardOrderData); exit();
	$pregiftcardamount=$giftCardOrderData['pregiftcardamount'];
	
	$giftcardqty= isset($giftCardOrderData['giftcardqty']) ? $giftCardOrderData['giftcardqty'] : '';
	if(isset($pregiftcardamount) && !empty($pregiftcardamount)){
		$eachGiftCardAmt=$pregiftcardamount;
	}else{
		$customgiftcardamount=$giftCardOrderData['customgiftcardamount'];
		$eachGiftCardAmt=$customgiftcardamount;
	}
	
	$recipientemail=$giftCardOrderData['recipientemail'];
	$yourname=$giftCardOrderData['yourname'];
	$customermessage=!empty($giftCardOrderData['customermessage']) ? $giftCardOrderData['customermessage'] : '----';
	$deliverydate=$giftCardOrderData['deliverydate'];
	$promotioncode=$giftCardOrderData['promotioncode'];
	$customerfirstname=$giftCardOrderData['customerfirstname'];
	$customerlastname=$giftCardOrderData['customerlastname'];
	$customeraddress=$giftCardOrderData['customeraddress'];
	$customersecondaryaddress=$giftCardOrderData['customersecondaryaddress'];
	$customercity=$giftCardOrderData['customercity'];
	$customerstate=$giftCardOrderData['customerstate'];
	$customerzipcode=$giftCardOrderData['customerzipcode'];
	
	
	$customerphonenumber=$giftCardOrderData['customerphonenumber'];
	$customeremail=$giftCardOrderData['customeremail'];
	$knownby=$giftCardOrderData['knownby'];
	$receiveemailoption=$giftCardOrderData['receiveemailoption'];
	
	
	
	//var_dump($giftcarddetails); exit();
	
	drupal_set_title("Gift Card Order : ".$order_id);
	global $base_url;
	$html = '';

        $order=uc_order_load($order_id);
   
       	$logourl = $GLOBALS['base_url'] . "/sites/all/commonimages/logo.png";
		$html = '<html>
			<body style="margin=0px; padding:0px;">
			<table style="border: 1px solid #333333";>
			 
			  <tr>
		        <td style="width:30%;" valign="top">
		          <span class="icon-user"></span> <b>Purchasing Information:</b> 
		      	<br> 
		    		' . $order->billing_first_name . ' ' . $order->billing_last_name . '<br>
		    		' . $order->billing_street1 . '
                                '.$order->delivery_street2.'<br>    
		     		' . $order->billing_city . ', ' . uc_zone_get_by_id($order->billing_zone) . ' ' . $order->billing_postal_code . '<br>
		    		' . $order->billing_phone.'<br/>'.$order->primary_email.'
		  		</td>
		  
    				
    	    				
		    	
				<td style="width:70%;">
		    	<span class="icon-shopping-cart"></span> <b>Order Summary:</b> <br/>
				Order# : ' . $order->order_id . '
				<br/>
    		
  				Order Date : '.date("m/d/Y", $order->created).'
                <br/>                                                		
                                                                		
     			Order Total : '.uc_currency_format($order->order_total).' <br/>
     						
     		Quantity : '.$giftcardqty.'<br/>
		  	Amount : '.uc_currency_format($eachGiftCardAmt).'<br/>
                           
		  	To : '.$recipientemail.'<br/>
		  	Message : '.$customermessage.'<br/>
		  	Delivery Date : '.date('m/d/Y',strtotime($deliverydate)).'<br/>
  				</td>
    		</tr>
  	 		
			
  	 		
	 
  	 		
  	 
     </table>
</html>';
               
              $form['orderid'] = array(
				'#type' => 'hidden',
				'#default_value' => $order_id
		);
                
               $form['giftcardview'] = array(
				'#type' => 'markup',
				'#markup' => $html
		);
                 
               $form['giftcardpayment'] = array(
                        '#type' => 'select',
                        '#title' => t('Order status'),
                        '#options' => array(
                            'payment_received' => t('Payment Received'),
                            'completed'=> t('Completed'),
                        ),
                       '#default_value' => isset($orderstatus['order_status'])?$orderstatus['order_status']:'',

                 );
                $form['update']=array(
                    '#type'=>'submit',
                    '#value'=>'Update'
                   );
                   return $form;
	
}

function giftcardorderpagetemplate_form_submit($form, &$form_state) {
   $order_id=$form_state['values']['orderid'];
    $orderstatus=$form_state['values']['giftcardpayment'];
    if(isset($orderstatus)){
    try{
       db_update('uc_orders')
        ->fields(array('order_status' => $orderstatus))
        ->condition('order_id', $order_id )
        ->execute();
       drupal_set_message(t("Order is Updated"));
       } catch (Exception $e) {
        
       }
    }
   
}