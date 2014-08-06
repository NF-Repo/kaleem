<?php
function booking_get_header($form, &$form_state) {
  
    
    $bookingCart= new BookingCart();
    $eventId = $bookingCart->getCartEventId();
    $cruiseitemId=$bookingCart->getCartCruiseTemplateId();
    $bookingdate=$bookingCart->getCartEventDate();
    
    //echo $bookingdate; exit();
    
    
    $bookingdate = strtotime($bookingdate);
    $bookingdate = date("F d, Y", $bookingdate);
    
   // echo $bookingdate; exit();
	
	//using cruise Item Id loading the Complete Item Details
    $cruisedetails = cruisedetailsWithportdetails($cruiseitemId);
    $form_state['stage'] = isset($form_state['stage']) ? $form_state['stage'] : 1;
	$form_stages = array (
			'TICKETS' => 1,
			'ENHANCEMENTS' => 2,
			'PAYMENT' => 3,
			'REVIEW_ORDER' => 4 
	);

    if (isset ( $form_stages [$form_state ['stage']] )) {
		$current_step = $form_stages [$form_state ['stage']];
	} else {
		$current_step = 1;
	}

    $stages = array(
        1 => array('data' => 'TICKETS'),
        2 => array('data' => 'ENHANCEMENTS'),
        3 => array('data' => 'PAYMENT'),
        4 => array('data' => 'REVIEW_ORDER'),
    );

    $stages[$current_step]['class'] = array('active');


    $attributes = array(
        'id' => 'my-custom-listing',
        'class' => 'custom-class another-custom-class', // a string or indexed (string) array with the classes for the list tag
    );

    $form['header'] = array(
        '#type' => 'fieldset',
        '#prefix' => '<div class="contentbox-inner mobilebookingContainer"><div class="row-fluid"><div class="span12">',
        '#suffix' => '</div></div>',
        '#title' => '',
    );

    $stepmenuarray = array(
        1 => 'TICKETS',
        2 => 'ENHANCEMENTS',
        3 => 'PAYMENT',
        4 => 'REVIEW ORDER',
        5 => 'CONFIRMATION'
    );
    
    $ticketsmenu = '<div class="menuRelative"><div class="subMenu responsiveWeb"><ul>';
    foreach ($stepmenuarray as $menukey => $menuvalue) {
        if ($menukey == $current_step) {
            $ticketsmenu.='<li class="active">' . $menuvalue . '</li>';
        } else {
            $ticketsmenu.='<li>' . $menuvalue . '</li>';
        }
    }
    $ticketsmenu .= '</ul></div><div class="responsiveMobile groupmarginTop" align="center"><select class="ticketpriceqty" disabled>';
    foreach ($stepmenuarray as $menukey => $menuvalue) {
        if ($menukey == $current_step) {
            $ticketsmenu.='<option selected="select">' . $menuvalue . '</option>';
        } else {
            $ticketsmenu.='<option>' . $menuvalue . '</option>';
        }
    }
    $ticketsmenu .= '</select></div></div>';


    $form['header']['stepmenu'] = array(
        '#type' => 'fieldset',
        '#prefix' => '<div class="row-fluid"><div class="span12">',
        '#suffix' => '</div></div>',
        '#title' => '',
        '#value' => $ticketsmenu
    );
    
    
	if ($current_step == 1) {
		$stages_content = '<div class="row-fluid shreBtnmargin responsiveWeb">
    					   <div class="span12"></div>
    					   </div>
    					   <div class="row-fluid responsiveWeb">
    					   <div class="span12">
    					   <div class="ticketTitle">PURCHASE TICKETS</div>
     					   </div>
     					   </div>';
	}



    if ($current_step == 2) {
        $stages_content = '<div class="row-fluid shreBtnmargin responsiveWeb">
    					   <div class="span12"></div>
    					   </div>
						   <div class="row-fluid responsiveWeb">
						   <div class="span12">
						   <div class="ticketTitle">ENHANCEMENTS</div>
						   </div>
						   </div>';
    }

    
    if ($current_step == 3) {
        $stages_content = '<div class="row-fluid shreBtnmargin responsiveWeb">
						   <div class="span12"></div>
						   </div>
						   <div class="row-fluid responsiveWeb">
						   <div class="span12">
						   <div class="ticketTitle">PAYMENT</div>
						   </div>
						   </div>';
    }
    
    
    if ($current_step == 4) {
        $stages_content = '<div class="row-fluid shreBtnmargin responsiveWeb">
						    <div class="span12"></div>
						    </div>
						    <div class="row-fluid responsiveWeb">
						    <div class="span12">
						    <div class="ticketTitle">Review Order</div>
						    </div>
						    </div>
						     
							<div class="row-fluid">
							<div class="span7">
							<div class="purchaseSubTitle">
						        		
							<a href="#" id="addvoucherlink">Add Promo code or Gift Card </a>
							</div>
							</div>
							<div class="span5"></div>
							</div>';
    }


    $form['header']['content'] = array(
        '#type' => 'fieldset',
        '#prefix' => '<div class="headercontent">',
        '#suffix' => '</div>',
        '#title' => '',
        '#value' => $stages_content,
    );

    return $form;
}


//Given the current stage the user is on, calculate what the next step would be
function booking_move_to_next_stage($form, &$form_state) {

    switch ($form_state['stage']) {
        case 'TICKETS':
            $_SESSION['formstage']="ENHANCEMENTS";
            return 'ENHANCEMENTS';
            break;

        case 'ENHANCEMENTS':
            $_SESSION['formstage']="PAYMENT";
            return 'PAYMENT';
            break;

        case 'PAYMENT':
            $_SESSION['formstage']="REVIEW_ORDER";
            return 'REVIEW_ORDER';
            break;
    }
}



//Given the current stage the user is on, calculate what the previous step would be
function booking_move_to_previous_stage($form, &$form_state) {
	
    switch ($form_state['stage']) {
        case 'ENHANCEMENTS':
            $_SESSION['formstage']="TICKETS";
            return 'TICKETS';
            break;

        case 'PAYMENT':
            $_SESSION['formstage']="ENHANCEMENTS";
            return 'ENHANCEMENTS';
            break;

        case 'REVIEW_ORDER':
            $_SESSION['formstage']="PAYMENT";
            return 'PAYMENT';
            break;
    }
}




function loadProductattributeoptions($productid) {
    $productnode = node_load($productid);
    $produtdetails = array();
    $results = array();
    $produtdetails['title'] = $productnode->title;
    //getting the Product Attributtes and options
    //make sure that accroding to the requirement we have one attirbute with one option
    foreach ($productnode->attributes as $options) {
        foreach ($options as $optionkey => $optionvalue) {
            $produtdetails[$optionkey] = $optionvalue;
        }

        $results[] = $produtdetails;
    }
    return $results;
}



//load catalog Products
function loadCatalogProducts() {

    $resultsarray = array();
    $query = new EntityFieldQuery();
    $entities = $query->entityCondition('entity_type', 'taxonomy_term');
    $query->entityCondition('bundle', 'catalog');
    $query->propertyCondition('name', 'Enhancements'); //change 2 to any vocabulary ID
    $query->execute();
    if (count($entities->ordered_results) > 0) {
        foreach ($entities->ordered_results as $termresults) {
            $children = taxonomy_get_children($termresults->entity_id);
            foreach ($children as $childvalue) {
                //loading the Products
                $produtclist[$childvalue->name] = catalogEnhancementCategoryProducts($childvalue->tid);
                $resultsarray[] = $produtclist;
                unset($produtclist);
            }
        }
    }
    return $resultsarray;
}




function catalogEnhancementCategoryProducts($catalogid) {
	$resultsetnodes=array();
	$totalresults=array();
	$enhancemnetsnodes=getenhancementlistby($catalogid);
	foreach ($enhancemnetsnodes as $ennodes)
	{
		$resultsetnodes['id']=$ennodes->id;
		$resultsetnodes['title']=$ennodes->title;
		$resultsetnodes['price']=$ennodes->price;
		$resultsetnodes['description']=$ennodes->description;
		$resultsetnodes['caption']=$ennodes->caption;
		$resultsetnodes['hoursoffsale']=$ennodes->hoursoffsale;
		$resultsetnodes['nid']=$ennodes->nid;
		$resultsetnodes['status']=$ennodes->status;
		$resultsetnodes['categoryid']=$ennodes->categoryid;
		$resultsetnodes['created']=$ennodes->created;
		$resultsetnodes['enhancements_img_fid']=$ennodes->enhancements_img_fid;
		$totalresults[]=$resultsetnodes;
		unset($resultsetnodes);
	}
	
return $totalresults;
	
}


function catalogEnhancementCategoryProductsold($catalogid) {
    $resultsarray = array();
    $enhancementproducts = array();
    $query = new EntityFieldQuery();
    $entities = $query->entityCondition('entity_type', 'node');
    $query->entityCondition('bundle', 'product');
    $query->propertyCondition('status', 1); // Don't include unpublished products.
    $query->fieldCondition('taxonomy_catalog', 'tid', $catalogid);
    $query->execute();

    if (count($entities->ordered_results) > 0) {
        foreach ($entities->ordered_results as $productnodes) {
          //loading Products using entity id
		  //getenhancementlist
        	
            $productnodes = node_load($productnodes->entity_id);
            $enhancementproducts['nid'] = $productnodes->nid;
            $enhancementproducts['title'] = $productnodes->title;
            $enhancementproducts['status'] = $productnodes->status;
            $enhancementproducts['model'] = $productnodes->model;
            $enhancementproducts['price'] = $productnodes->price;
            $enhancementproducts['imageurl'] = file_create_url(getArrayloop($productnodes->uc_product_image));

            $resultsarray[] = $enhancementproducts;
            unset($enhancementproducts);
        }
    }

    return $resultsarray;
}





function buildform() {
    //Booking Flow Implementation
    $form = array();
    $form['ENHANCEMENTS'] = array(
        '#type' => 'fieldset',
        '#prefix' => '<div class="row-fluid"><div class="span6">',
        '#suffix' => '</div></div>',
        '#title' => '',
    );
    $result = loadCatalogProducts();

    foreach ($result as $productvalue) {
        foreach ($productvalue as $key => $productdetails) {
            $form['ENHANCEMENTS'][$key] = array(
                '#type' => 'fieldset',
                '#title' => t($key),
                '#weight' => 1,
                '#collapsible' => TRUE,
                '#collapsed' => FALSE,
                '#tree' => TRUE,
            );
            foreach ($productdetails as $productform) {
                $thumbhtml = '<div class="span6 blogInner">
    						  <div class="blogTitle">
   							  <span>' . $productform['name'] . '</span></div><img src="' . $productform['imageurl'] . '"></div>';
                
                $form['ENHANCEMENTS'][$key][$productform['name']] = array(
                    '#type' => 'fieldset',
                    '#prefix' => '<div class="headercontent">',
                    '#suffix' => '</div>',
                    '#title' => '',
                    '#value' => $thumbhtml,
                );
                
                //option set one field
                $form['ENHANCEMENTS'][$key][$productform['name']]['item-' . $productform['name']['nid']] = array(
                    '#type' => 'select',
                    '#title' => $productform['price'], //later we need to change this one as dynamic
                    '#options' => array(
                        0 => 0, 1 => 1, 2 => 2, 3 => 3, 4 => 4, 5 => 5, 6 => 6, 7 => 7
                    ),
                    '#default_value' => 0,
                );
            }
        }
    }
    return $form;
}




//format:array(productenhancements=>array(enhancmentid,enhancementid,enhancmentid));
function getProductEnchancements($porudtctId) {
	$query = db_select('cruise_template_enhancements','cen');
	$query->fields('cen');
	$query->condition('cen.cruisetemplateid',2,'=');
	$result = $query->execute();
	$enhancmentresults=array();
	foreach ($result as $val)
	{
		$enhancmentresults=unserialize($val->templateenhancements);
	}
	
	return $enhancmentresults;//array(id1,id2,id3)
}




class CruiseEventObject{
	
	
	public static $eventName='';
	public static $departuretime='';
	public static $returntime;
	public static $portid;
	
	public static $data=array();
	public static function getCruiseEventDetails($eventid,$eventDate){
		
		//$bookingCart= new BookingCart();
		
		//Getting Eventname
		//$eventid=$_SESSION['bookingInput']->eventid;
		$scheduledEventObject=scheduledevents_load($eventid);
		if(!is_null($scheduledEventObject) && isset($scheduledEventObject)){
			$data['eventname']=$scheduledEventObject->name;
			$data['portid']=$scheduledEventObject->port_id;
			$data['departuretime']=$scheduledEventObject->departuretime;
			$data['returntime']=$scheduledEventObject->returntime;
			$data['boardingtime']=$scheduledEventObject->boardingtime;
			$eventTimeUnix = strtotime($eventDate);
			$eventdate = date("F d, Y", $eventTimeUnix);
			$data['eventdate']=$eventdate;
			
			//Port Details
			$portData = loadportById($data['portid']);
			$data['portname']=$portData['portname'];
		}
		return $data;
	}
	
	public static function getCruiseEventWidget(){
		
	}
}



function getPrintOrderWidget($type) {
	
	$bookingCart= new BookingCart();
	$eventId = $bookingCart->getCartEventId();
	
	$cruiseTemplatId=$bookingCart->getCartCruiseTemplateId();
	$cruiseObject=cruise_load($cruiseTemplatId);
	$cruiseTitle=$cruiseObject->cruise_title;
	$eventDate=$bookingCart->getCartEventDate();
	
	$cruisedetails=CruiseEventObject::getCruiseEventDetails($eventId,$eventDate);

	//var_dump($cruisedetails); exit();
	$prefixwidget = '<div class="ordersummaryBox">
			<div class="sliderBox">
			<div class="sliderBoxout">
			<div class="orderBoxInner">
			<!--<div class="Ordertitle"><span>Order Summary</span><br><span>' . $cruisedetails ['eventname'] . '<br><br>
			<span>Departure Port : ' . $cruisedetails ['portname'] . '</span><br><span>Departure Time ' . $cruisedetails ['departuretime'] . '</span>&nbsp;&nbsp;<span>Return ' . $cruisedetails ['returntime'] . '</span></div>
			<div>-->
			
			<div class="widgettitleHeader">
			<div class="span2">
			<div class="line"></div>
			</div>
			<div class="span8" align="center">
			<h2>Order Summary</h2>
			</div>
			<div class="span2">
			<div class="line"></div>
			</div>
			</div>
			
			<div class="widgetBody">
			<h2>' .$cruiseTitle . '</h2>
			<h3>' . $cruisedetails ['eventdate'] . '</h3>
			<h4>Departure Port: ' . $cruisedetails ['portname'] . '</h4>
			
			<div class="timingDetails">
			<h3>Depart ' . $cruisedetails ['departuretime'] . '</h3>
			<h4><img src="' . base_path () . 'sites/all/themes/waterways/assets/img/dot.png"></h4>
			<h3>Return ' . $cruisedetails ['returntime'] . '</h3></div>';
	
	$suffixwidget='<div align="center"><img src="' . base_path () . 'sites/all/themes/waterways/assets/img/innerbox-widget-img.png"></div>
			</div>
			</div>
			</div>
			</div>';
	
	$vouchers='';
	$tickets='';
	
	
	
	$tickets=BookingWidget::tickets();
	$orderSubTotal=BookingWidget::orderSubTotal();
	$feeItems=BookingWidget::feeItems();
	$tax=BookingWidget::tax();
	$orderTotal=BookingWidget::orderTotal();
	$enhancement=BookingWidget::enhancements();
	$promocode=BookingWidget::promocode();
	$giftcard=BookingWidget::giftcard();
	//$vouchers=BookingWidget::getVouchers();
	
	
	return "{$prefixwidget} {$tickets} {$enhancement} {$feeItems} {$orderSubTotal} {$tax} {$promocode} {$vouchers} {$giftcard} {$orderTotal} {$suffixwidget}";
	
	
}







