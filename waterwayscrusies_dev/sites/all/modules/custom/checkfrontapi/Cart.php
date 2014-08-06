<?php
/**
 * Checkfront Sample Code: Browse inventory & create booking.
 *
 * This is sample code is for demonstration only and should not be used in production
 * without modifcation.  It does not adequtly secure your OAuth tokens.
 *
 * see: 
 * 
 * API Documenation:  http://www.checkfront.com/developers/api/
 * API Error Codes:  http://www.checkfront.com/developers/api-error
 * PHP SDK - https://github.com/Checkfront/PHP-SDK
 * CQL Documenation: http://www.checkfront.com/developers/api-cql/
 *
 */

/*
 * @access public
 * @package Checkfront
 */




/* 
 * You need to create a new application in your Checkfront Account under
Manage / Extend / Api and supply the details below. 

This example bybasses the oauth authorization redirect by supplying "oob" 
(Out Of Bounds) as the redirect_uri, and by generating the access and 
refresh tokens from within Checkfront. 

For more infomration on your endpoints see: 
http://www.checkfront.com/developers/api/#endpoints
*/

// a general class that wraps the api along with some custom calls
class Booking {

	public $cart_id = '';
	public $cart = array();

	function __construct() {

		// create api connection to Checkfront
		// you can generate oauth values in Add-ons / Api in your account
		$this->Checkfront = new Checkfront(
		
                        
                         array(
        'host'=>'lsnsoftinc.checkfront.com',
        'consumer_key'  => '38072278951cbd76182793',
        'consumer_secret' => '59e63581fdbf9ed061856867ad3ead1ed7f9b38c',
        'redirect_uri'=>'oob',
        'refresh_token' => 'f71d218a849f79d78bb70a1e805064a0',
        'mode' => 'public',
    )
		);
		// init shopping cart
		$this->cart();
	}

	// fetch items from inventory based on date
	public function query_inventory($data) {
         
//		$response = $this->Checkfront->get('item',array('start_date'=>$data['start_date'],'end_date'=>$data['end_date']));
          $response = $this->Checkfront->get('item',$data);
		return $response['items'];
	}
        
        
        //fetch the items by id
        public function query_inventoryByItem($data,$itemID) {
         		
          $response = $this->Checkfront->get('item/'.$itemID,$data);
		return $response['items'];
	}
        
        

	// add slips to the booking session
	public function set($slips=array()) {
 
		$response = $this->Checkfront->post('booking/session',array('slip'=>$slips,'session_id'=>$session_id));
		$this->cart_id= $response['booking']['session']['id'];
		$this->cart();
	}

	// get the booking form fields required to make a booking
	public function form() {
		$response = $this->Checkfront->get('booking/form');
		return $response['booking']['form'];
	}

	// get cart session
	public function cart() {
		if(isset($_GET['cart_id'])) $this->cart_id = $_GET['cart_id'];
		if($this->cart_id) {
			$response = $this->Checkfront->post('booking/session',array('session_id'=>$this->cart_id));
			if($response['booking']['item']) {
				foreach($response['booking']['item']  as $line_id => $data) {
					// store for later
					$this->cart[$line_id] = $data;
				}
			}
		}
	}

	// create a booking using the session and the posted form fields
	public function create($form) {
         
		$form['session_id'] = $form['cart_id'];
                
		if($response = $this->Checkfront->post('booking/create',array('session_id'=>$this->cart_id,'form'=>$form))) {
			return $response;
		}
              
	}
}
