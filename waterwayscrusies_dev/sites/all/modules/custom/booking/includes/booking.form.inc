<?php

/*
 * ------------------Guest User Booking ----------------------
 * Booking Form Stages
 * 1)TICKETS
 * 2)ENHANCEMENTS
 * 3)PAYMENT
 * 4)REVIEWORDER
 * 5)CONFIRMATION
 * 
 * ->Check Valid Session
 */

function booking_form($form, &$form_state) {
	
	$bookingCart=new BookingCart();
	$bookingCart->storeBookingInput($_SESSION['bookingInput']);
	
    drupal_add_css(drupal_get_path('module', 'booking') . '/css/booking.css');
    drupal_add_css(drupal_get_path('module', 'booking') . '/css/booking-mediaqueries.css');
    drupal_add_js(drupal_get_path('module', 'booking') . '/js/booking.js');
    drupal_add_js(drupal_get_path('module', 'booking') . '/js/mask.js');

    //Tickets - 1 : Check Valid Session
    if (!isset($_SESSION['bookingInput'])) {
        drupal_goto("wwccalendar");
        exit();
    }

    //Tickets - 2 : Check Form Stage
    if (!isset($form_state['stage'])) {
        $form_state['stage'] = 'TICKETS';
        $bookingCart= new BookingCart();
        $bookingCart->emptyCart();
        $bookingCart->storeBookingInput($_SESSION['bookingInput']);
    }
    $_SESSION['formstage']=$form_state['stage'];

    $form = array();
    $form = booking_get_header($form, $form_state);
    switch ($form_state['stage']) {
        case 'TICKETS' :
            return booking_tickets_form($form, $form_state);
        
        case 'ENHANCEMENTS' :
            return booking_enhancements_form($form, $form_state);

        case 'PAYMENT' :
            return booking_payments_form($form, $form_state);
        
        case 'REVIEW_ORDER' :
            return booking_review_form($form, $form_state);
     
        default :
            return booking_tickets_form($form, $form_state);
    }
    return $form;
}



//Tickets - 3.1 Tickets Form
function booking_tickets_form($form, &$form_state) {
	
	$bookingCart= new BookingCart();
	$eventId = $bookingCart->getCartEventId();
	$cruiseTemplateId=$bookingCart->getCartCruiseTemplateId();
	$cartId=$bookingCart->cartId();
	
    $values = isset($form_state['multistep_values']['TICKETS']) ? $form_state['multistep_values']['TICKETS'] : array();
   
    $cruiseObject = CruiseEventObject::getCruiseEventDetails($eventId);
    $portid = isset($cruiseObject['portid']) ? $cruiseObject['portid'] : '';
    $eventdate = isset($cruiseObject['eventdate']) ? $cruiseObject['eventdate'] : '';

    
    $eventdatetime='';
    if ($eventdate != '') {
        $tt = strtotime($eventdate);
        $eventdatetime = strtotime(date('m/d/Y H:i:s', $tt));
    }

    $form['stepone'] = array(
        '#type' => 'fieldset',
        '#prefix' => '<div class="row-fluid">',
        '#suffix' => '</div>',
        '#title' => '',
    );

    $form['stepone']['TICKETS'] = array(
        '#type' => 'fieldset',
        '#prefix' => '<div class="span7">',
        '#suffix' => '</div>',
         '#description' => '<div class="bookingSubTitle">Advanced purchase required on all dining cruises</div><div class="purchasecontent">Select the number of tickets you would like to purchase. To purchase tickets for groups of eight or more guests, to reserve an entire deck for a private event, or to book a private yacht charter, go to</div><div class="groupresButton"><a href="#myModalrequest" role="button" data-toggle="modal"><b>GROUP RESERVATIONS</b></a> </div><div class="cruisedetailviewBtn"><b><a target="_blank" href="cruisesdetailview/category/'.$cruiseTemplateId. '?portid=' . $portid . '&eventdate=' . $eventdatetime . '&eventid='.$eventId.'">CRUISE DESCRIPTION</a></b></div>'
    );

    
    //Order Summary Table
    $ticketsoutput = getPrintOrderWidget('TICKETS');
    $form['stepone']['TICKETSORDERWIDGET'] = array(
        '#type' => 'fieldset',
        '#prefix' => '<div class="span5">',
        '#suffix' => '</div>',
        '#value' => $ticketsoutput,
    );



    //Tickets - 3.2 Getting the Tickets by Cruise
    $availableTickets = '';
    $availableTickets = getCruiseTicketByWeight($cruiseTemplateId);
    
    

    //Tickets - 3.3 Getting Ticketentrees by Ticket and Cruise Template ID
    $ticketEntrees = array();
    foreach ($availableTickets as $ticket) {
        $data = Cruise::getTicketEntrees($ticket->id, $cruiseTemplateId);
        $arr = array('' => '--Select--');
        if(count($data)>0){
        	foreach ($data as $key => $val) {
        		$arr[$key] = $val;
        		$ticketEntrees[$ticket->id] = $arr;
        	}
        	
        }else{
        	$ticketEntrees[$ticket->id] = $arr;
        }
        
        
    }
    
    
    foreach ($availableTickets as $ticket) {
        $defaultvalue = isset($values['ticketentrees' . $ticket->id]) ? $values['ticketentrees' . $ticket->id] : '';
        $ticketlabel = $ticket->title . " $" . wwcFormatPrice($ticket->price);
        $form ['stepone'] ['TICKETS'] ['ticketentrees' . $ticket->id] = array(
            '#type' => 'select',
            '#prefix' => '<div class="bookfield">',
            '#suffix' => '</div>',
            '#title' => t($ticketlabel),
            '#options' => array(
                0 => 0,
                1 => 1,
                2 => 2,
                3 => 3,
                4 => 4,
                5 => 5,
                6 => 6,
                7 => 7,
                8 => 8,
                9 => 9,
            ),
            '#default_value' => $defaultvalue,
            '#attributes' => array(
                'class' => array(
                    'ticketpriceqty'
                )
            ),
            '#ajax' => array(
                'callback' => 'ticketentrees_callback',
                'wrapper' => 'ticketentreesDiv',
                'method' => 'html',
                'effect' => 'fade',
            	//'progress' => array('type' => 'none'),
            ),
        );

        $form ['stepone'] ['TICKETS'] ['ticketentrees' . $ticket->id . "ticketname"] = array(
            '#type' => 'textfield',
            '#default_value' => $ticket->title,
            '#prefix' => '<div style="display:none;">',
            '#suffix' => '</div>'
        );

        $form ['stepone'] ['TICKETS'] ['ticketentrees' . $ticket->id . "price"] = array(
            '#type' => 'hidden',
            '#value' => $ticket->price
        );

        //attribute options
        $form['stepone']['TICKETS']['ticketentreesoption' . $ticket->id] = array(
            '#type' => 'hidden',
            '#value' => $ticket->id,
        );
    }//foreach


    $form['stepone']['TICKETS']['ticketentrees_fieldset'] = array(
        '#type' => 'markup',
        '#prefix' => '<div id="ticketentreesDiv">',
        '#suffix' => '</div>',
        '#weight' => 2,
    );
    
    

    $displayoption = "none;";
    foreach ($availableTickets as $ticket) {
        $numoftickets = 0;
        if (isset($form_state['values']['ticketentrees' . $ticket->id])) {
            $numoftickets = $form_state['values']['ticketentrees' . $ticket->id];
        } else
        if (isset($values['ticketentrees' . $ticket->id])) {
            $numoftickets = $values['ticketentrees' . $ticket->id];
        }
        // Showing the div if number of tickets > 0
        if ($numoftickets > 0) {
            $displayoption = "block;";
            break;
        }
    }

    $form['stepone']['TICKETS']['ticketentrees_fieldset']['entrystart'] = array(
        '#type' => 'markup',
        '#markup' => '<div class="row-fluid" id="entreestartdiv" style="float:left;display:' . $displayoption . '"><div class="span12 purchaseTicketDetails">'
    );

    $entrhtml = '';
    $entrhtml .= '<div class="row-fluid">';
    $entrhtml .= '<div class="span2 quantityHeader"><b>Quantity</b></div>';
    $entrhtml .= '<div class="span3 ticketHeader"><b>Ticket</b></div>';
    $entrhtml .= '<div class="span3 entreeHeader"><b>Entree</b></div>';
    $entrhtml .= '<div class="span2 entreeCheckBoxHeader"></div>';
    $entrhtml .= '<div class="span2 priceHeader"><b>Price</b></div>';
    $entrhtml .= '</div>';


    $form['stepone']['TICKETS']['ticketentrees_fieldset']['header'] = array(
        '#type' => 'markup',
        '#markup' => $entrhtml,
    );


    $form['stepone']['TICKETS']['entry'] = array(
        '#type' => 'fieldset',
        '#prefix' => '<div class="entrywidget" id="entrywidget" style="display:none;"><div id="entryContent" class=" divTable">',
        '#suffix' => '</div></div>',
    );



    foreach ($availableTickets as $ticket) {

        $numoftickets = '';
        if (isset($form_state['values']['ticketentrees' . $ticket->id])) {
            $numoftickets = $form_state['values']['ticketentrees' . $ticket->id];
        } else if (isset($values['ticketentrees' . $ticket->id])) {
            $numoftickets = $values['ticketentrees' . $ticket->id];
        }

        for ($i = 0; $i < $numoftickets; $i++) {
            $selectdefaultvalue = isset($values["select" . $ticket->id . $i]) ? $values["select" . $ticket->id . $i] : '';
            $form['stepone']['TICKETS']['ticketentrees_fieldset']['entry' . $ticket->id]["qty" . $ticket->id . $i] = array(
                '#type' => 'markup',
//                 '#markup' => '<tr><td>1</td>',
            		'#markup' => '<div class="row-fluid"><div class="span2 quantityClass">1</div>',
            );

            $form['stepone']['TICKETS']['ticketentrees_fieldset']['entry' . $ticket->id]["name" . $ticket->id . $i] = array(
                '#type' => 'markup',
//                 '#markup' => "<td>" . $ticket->title . "</td>"
            		 '#markup' => "<div class='span2 ticketClass'>" . $ticket->title . "</div>"
            );

            $form ['stepone'] ['TICKETS'] ['ticketentrees_fieldset'] ['entry' . $ticket->id] ["select" . $ticket->id . $i] = array(
                '#id' => 'option-entree-' . $ticket->id . '-' . $i,
                '#type' => 'select',
                '#options' => isset($ticketEntrees[$ticket->id]) ? $ticketEntrees[$ticket->id] : '',
                '#default_value' => $selectdefaultvalue,
                '#prefix' => "<div class='span3 entreeClass'>",
                '#suffix' => "</div>",
                '#attributes' => array(
                    "onchange" => "saveTicketEntree('option-entree-" . $ticket->id . "-" . $i . "','" . $ticket->id . "','" . $ticket->id . "-" . $i . "','" . $eventId . "')"
                )
            );

            $chkdefaultvalue = isset($values["checkbox" . $ticket->id . $i]) ? $values["checkbox" . $ticket->id . $i] : '';
            $form ['stepone'] ['TICKETS'] ['ticketentrees_fieldset'] ['entry' . $ticket->id] ["checkbox" . $ticket->id . $i] = array(
                '#id' => 'dietary-' . $ticket->id . '-' . $i,
                '#type' => 'checkbox',
                '#title' => "<i>Dietary restrictions</i>",
                '#default_value' => $chkdefaultvalue,
'#prefix' => "<div class='span3 entreeCheckBoxClass'>",
                '#suffix' => "</div>",
                '#attributes' => array(
                    "onclick" => "dietarycheckfieldEvent('" . $ticket->id . "-" . $i . "')"
                ),
                '#states' => array(
                    'disabled' => array(
                        ':input[name="select' . $ticket->id . $i . '"]' => array(
                            'value' => ''
                        )
                    )
                )
            );


            $price = '$' . wwcFormatPrice($ticket->price);
            $form ['stepone'] ['TICKETS'] ['ticketentrees_fieldset'] ['entry' . $ticket->id] ["price" . $ticket->id . $i] = array(
                '#type' => 'markup',
//                 '#markup' => "<td>" . $price . "</td><tr>"
            		'#markup' => "<div class='span2 priceClass'>" . $price . "</div></div>"
            );

            $commentvalue = ($chkdefaultvalue == 1) ? $values["dietarycomments" . $ticket->id . $i] : "";
            $form['stepone']['TICKETS']['ticketentrees_fieldset']['entry' . $ticket->id]["dietarycomments" . $ticket->id . $i] = array(
                '#id' => 'dietarycomments-' . $ticket->id . '-' . $i,
                '#type' => 'textarea',
                '#prefix' => '<div style="display:none;">',
                '#suffix' => '</div>',
                '#default_value' => $commentvalue,
            );
        }
    }


    $form['stepone']['TICKETS']['ticketentrees_fieldset']['entryend'] = array(
        '#type' => 'markup',
        '#markup' => '</div></div>',
    );

    $specialinstructionOptions = array(
        'limitedmobility' => t('Limited Mobility (wheelchair, cane, etc.)'),
        'foodallergy' => t('Food Allergy (seafood, peanuts, etc)'),
        'anotherparty' => t('Would like to be seated with another party (specify below)')
    );

    $specialinstructions = isset($values['specialinstructions']) ? $values['specialinstructions'] : array();
    $form['stepone']['TICKETS']['specialinstructions'] = array(
        '#type' => 'checkboxes',
        '#options' => $specialinstructionOptions,
        '#title' => t('<br/><b>Let us know if you have any special instructions or concerns:</b>'),
        '#default_value' => $specialinstructions,
        '#weight' => 6,
    );


    $additionalcomments = isset($values['additionalcomments']) ? $values['additionalcomments'] : "";
    $form['stepone']['TICKETS']['additionalcomments'] = array(
        '#title' => t(''),
        '#type' => 'textarea',
        '#attributes' => array('placeholder' => t('Please explain, or provide additional comments:'), 'class' => array('textareaStyleMedium')),
        '#default_value' => $additionalcomments,
        '#weight' => 7,
        '#states' => array(
            'required' => array(
                ':input[name=specialinstructions[limitedmobility]]' => array('value' => 'limitedmobility'),
            ),
        ),
    );

    $form ['modalpopup'] = array(
        '#type' => 'markup',
        '#markup' => '<div id="reservationCheckboxPopup" class="modal hide" role="dialog" aria-labelledby="myModalLabel" aria-hidden="true">
            <div class="modal-header">
                <h4>Dietary Restrictions <button type="button" id="reservationCheckboxClose"  class="close">x</button></h4>
                <!--<h4>Dietary Restrictions</h4>   -->        
    		
    		</div>
            <div class="modal-body">
                <textarea rows="4" id="dietarytext" name="dietarytext" class="span6"></textarea>
            </div>
            <div class="modal-footer">
                <span id="loaderText" style="display: none;">Loading...</span>
                <div class="booknow resCheckboxshare span4" id="shareBtn">
                        <input type="button" name="dietarysubmitbtn" id="dietarysubmitbtn"
                        value="Submit" class="resCheckbox-booknow-inner"/>
                </div>
                <input type="hidden" name="hiddenTicketId" id="hiddenTicketId">
                <input type="hidden" name="hiddenTicketEntreeId" id="hiddenTicketEntreeId">
                <input type="hidden" name="hiddenSelectedTicket" id="hiddenSelectedTicket">
			<input type="hidden" name="hiddenEventId" id="hiddenEventId">
            </div>
        </div><div class="modal-backdrop in reservationCheckBackdrop"></div>'
    );

    $form ['next'] = array(
        '#type' => 'submit',
        '#value' => t('Save & Continue'),
        '#prefix' => '<div class="paymentSubmitButton groupmarginTop"><div class="row-fluid"><div class="span4"><div class="line"></div></div><div class="booknow share span4" id="shareBtn"><span class="booknow-inner">',
        '#suffix' => '</span></div><div class="span4"><div class="line"></div></div></div></div>'
    );

    return $form;
}




function ticketentrees_callback($form, $form_state) {
    return $form['stepone']['TICKETS']['ticketentrees_fieldset'];
}





/**
 * ENHANCEMENTS
 */
function booking_enhancements_form($form, &$form_state) {
    //Enhancements - 2 Getting Values for Back Operation
    $values = isset($form_state['multistep_values']['ENHANCEMENTS']) ? $form_state['multistep_values']['ENHANCEMENTS'] : array();

    if (!empty($values)) {
        $bookingagentinputenarray = array();
        array_walk_recursive($values, function ($value) use(& $bookingagentinputenarray) {
            $bookingagentinputenarray[] = $value;
        });

        $enchunkresults = array_chunk($bookingagentinputenarray, 6);
        $enhancementkeyorder = array(
            'qty',
            'price',
            'name',
            'comments',
            'parentproduct',
            'enhancedproductnodeid'
        );

        foreach ($enchunkresults as $enchunkarray) {
            list ($qty) = $enchunkarray;
            if ($qty > 0) {
                list (,, $nodeenid) = $enchunkarray;
                $enproductenhancments[$nodeenid] = array_combine($enhancementkeyorder, $enchunkarray);
            }
        }
    }//if


    //Enhancements - 3
    if (isset($enproductenhancments) && !empty($enproductenhancments)) {
    	//Enhancements - 3.1
        $form['multistep_values'] = buildformbyCategoryId($enproductenhancments);
    } else {
    	//Enhancements - 3.2
        $form['multistep_values'] = buildformbyCategoryId();
    }
    



    $form['steptwo']['ENHANCEMENTS']['occasiontype'] = array(
    		'#type' => 'checkbox',
    		'#weight' => 6,
    		'#title' => t('<span class="paymentLable">Celebration (BirthDay, Anniversary, Special Occasions, etc.)</span>'),
    		'#default_value' => isset($values['occasiontype']) ? $values['occasiontype'] : "",
    );
    
    $form['steptwo']['ENHANCEMENTS']['occasionadditionalcomments'] = array(
    		'#title' => t(''),
    		'#type' => 'textarea',
    		'#weight' => 7,
    		'#attributes' => array('placeholder' => t('Please explain, or provide additional comments:')),
    		'#default_value' => isset($values['occasionadditionalcomments']) ? $values['occasionadditionalcomments'] : "",
    );


    $form['btnwrapper'] = array(
        '#type' => 'markup',
        '#prefix' => '<div class="bookingPaymentBtnWrapper groupmarginTop"><div class="row-fluid">',
        '#suffix' => '</div></div>'
    );

    $form['btnwrapper']['back'] = array(
        '#id' => 'enhancementsbackbutton',
        '#type' => 'submit',
        '#value' => t('Go Back'),
        '#prefix' => '<div class="span3"><div class="line"></div></div><div class="booknowBack share span3" id="shareBtn"><span class="booknowBack-inner">',
        '#suffix' => '</span></div>',
        '#attributes' => array('class' => array('goBackbtn')),
    );

    $form['btnwrapper']['next'] = array(
        '#type' => 'submit',
        '#value' => t('Save & Continue'),
        '#prefix' => '<div class="booknow share span3" id="shareBtn"><span class="booknow-inner">',
        '#suffix' => '</span></div><div class="span3"><div class="line"></div></div>',
    );

    return $form;
}

//********************* BUILD Enhancements Form Elements ***************************************

//Enhancements - 3.4
function buildformbyCategoryId($enhancedinputs = null) {
	
	$bookingCart=new BookingCart();
	$eventId = $bookingCart->getCartEventId();
	$templateid = $bookingCart->getCartCruiseTemplateId();
	
	
    $form['steptwo'] = array(
        '#type' => 'fieldset',
        '#prefix' => '<div class="row-fluid enhancementWrapper">',
        '#suffix' => '</div>',
        '#title' => '',
    );

    $form['steptwo']['ENHANCEMENTS'] = array(
        '#type' => 'fieldset',
        '#prefix' => '<div class="span7">',
        '#suffix' => '</div>',
        '#description' => '<div class="bookingSubTitle">To enrich your cruising experience,include enhancements to your cruise</div>',
    );

    $enhancementsoutput = getPrintOrderWidget('ENHANCEMENTS');
    $form['steptwo']['ENHANCEMENTSORDERWIDGET'] = array(
        '#type' => 'fieldset',
        '#prefix' => '<div class="span5">',
        '#suffix' => '</div>',
        '#description' => $enhancementsoutput,
    );

    //Enhancements - 3.5
    $result = loadCatalogProducts();
  

    /**
     * Loading the Product Related Enhancements using
     * uc_product_enhancments section using for comparision and '
     * only showing Product admin selected enhacnements only
     *
     */
    
    //Enhancements - 3.5
    $selectedproductEnhancments = CruiseEnhancements::getSelectedEnhancements($templateid, $eventId);
    
  
    $i = 0;
    foreach ($result as $productvalue) {

        foreach ($productvalue as $key => $productdetails) {
            if ($i == 0) {
                $collapsed = FALSE; // By Default First one is in Opened Mode
            } else {
                $collapsed = TRUE; // Closed Mode
            }
            $ehancementsTitle = $key;
            $key = 'fieldset'.@$productdetails[0]['categoryid'];

            $form['steptwo']['ENHANCEMENTS'][$key] = array(
                '#type' => 'fieldset',
                '#id' => $key,
                '#title' => t($ehancementsTitle),
                '#weight' => $i + 1,
                '#collapsible' => TRUE,
                '#collapsed' => $collapsed,
                '#tree' => TRUE,
                '#prefix' => '<div class="row-fluid groupmarginTop optionItemBorder"><div class="span12">',
                '#suffix' => '</div></div>'
            );


            $rowfluidcount = 1;

            foreach (array_chunk($productdetails, 2) as $optionsform) {

                // $titleoption=$productform['title'];
                $form['steptwo']['ENHANCEMENTS'][$key][$rowfluidcount] = array(
                    '#type' => 'markup',
                    '#prefix' => '<div class="row-fluid">',
                    '#suffix' => '</div>',
                    '#title' => ''
                );

                foreach ($optionsform as $productform) {
                    //checking the Enhancment productid are in selected arrays
                    //Enhancments products are getting filtered based product selected enhancements
                    $titleoption = str_replace(' ', '-', $productform['title']);

                    if (in_array($productform['nid'], $selectedproductEnhancments)) {

                        $thumbhtml = '<div class="blogInner">
                    <div class="blogTitle">
                    <span>' . $productform['title'] . '</span>
                    		<br/><span class="blogtitleCaption">' . $productform['caption'] . '</span>
                    		</div><img src="' . file_create_url($productform['enhancements_img_fid']) . '"></div>';

                        //
                        $form['steptwo']['ENHANCEMENTS'][$key][$rowfluidcount]['options'][$productform['nid']] = array(
                            '#type' => 'fieldset',
                            '#prefix' => '<div class="optionItemWrapper span6">',
                            '#suffix' => '</div>',
                            '#title' => ''
                        );
                        $form['steptwo']['ENHANCEMENTS'][$key][$rowfluidcount]['options'][$productform['nid']][$titleoption][$productform['nid']] = array(
                            '#type' => 'fieldset',
                            '#title' => '',
                            '#description' => $thumbhtml,
                            '#attributes' => array(
                                'class' => array(
                                    'optionItemthumb'
                                )
                            )
                        );
                        $selqty = isset($enhancedinputs[$productform['title']]['qty']) ? $enhancedinputs[$productform['title']]['qty'] : '';
                        $form['steptwo']['ENHANCEMENTS'][$key][$rowfluidcount]['options'][$productform['nid']][$titleoption]['item-' . $productform['nid']]['qty'] = array(
                            '#type' => 'select',
                            '#title' => uc_currency_format($productform['price']), // later we need to change this one as dynamic
                            '#options' => array(
                                1 => 1,
                                2 => 2,
                                3 => 3,
                                4 => 4,
                                5 => 5,
                                6 => 6,
                                7 => 7
                            ),
                            //'#default_value' => (count($enhancedinputs) > 0) ? $enhancedinputs[$productform['nid']]['qty'] : '',
                            '#default_value' => $selqty,
                            "#empty_option" => t('- Select -'),
                            '#prefix' => '<div class="optionQuantityWrapper">',
                            '#suffix' => '</div>',
                            '#attributes' => array(
                                'class' => array(
                                    'selectFieldStyle selectFieldSmall'
                                )
                            )
                        );

                        // Hidden field for price section
                        $form['steptwo']['ENHANCEMENTS'][$key][$rowfluidcount]['options'][$productform['nid']][$titleoption]['item-' . $productform['nid']]['price'] = array(
                            '#type' => 'hidden',
                            '#value' => $productform['price']
                        );
                        // Name of Enhancement
                        $form['steptwo']['ENHANCEMENTS'][$key][$rowfluidcount]['options'][$productform['nid']][$titleoption]['item-' . $productform['nid']]['enhancementname'] = array(
                            '#type' => 'hidden',
                            '#value' => $productform['title']
                        );

                        $comments = isset($enhancedinputs[$productform['title']]['comments']) ? $enhancedinputs[$productform['title']]['comments'] : '';
                        $form['steptwo']['ENHANCEMENTS'][$key][$rowfluidcount]['options'][$productform['nid']][$titleoption]['item-' . $productform['nid']]['comments'] = array(
                            '#type' => 'textarea',
                            '#id' => $key.$productform['nid'],
                            // '#default_value'=>$productform['price'],
                            '#default_value' => $comments,
                            '#attributes' => array('placeholder' => array('Comments')),
                                //'#element_validate' => array('_test'),
                        );


                        // Enhancement Parent ProductId
                        $form['steptwo']['ENHANCEMENTS'][$key][$rowfluidcount]['options'][$productform['nid']][$titleoption]['item-' . $productform['nid']]['selectednode'] = array(
                            '#type' => 'hidden',
                            '#value' => 80
                        );

                        // Enhancement nodeId
                        $form['steptwo']['ENHANCEMENTS'][$key][$rowfluidcount]['options'][$productform['nid']][$titleoption]['item-' . $productform['nid']]['enhacmentselectednode'] = array(
                            '#type' => 'hidden',
                            '#value' => $productform['nid']
                        );
                    }
                }
                $rowfluidcount++;
            }//laxmi chunk for lop
            $i++;
        }
    }
    
    
  
    return $form;
}

/**
 * 
 * PAYMENT
 * 
 */
function booking_payments_form($form, &$form_state) {
	
	$bookingCart=new BookingCart();
    $values = isset($form_state['multistep_values']['PAYMENT']) ? $form_state['multistep_values']['PAYMENT'] : array();

    $form['stepthree'] = array(
        '#type' => 'markup',
        '#prefix' => '<div class="row-fluid">',
        '#suffix' => '</div>',
        '#title' => ''
    );

    $form['stepthree']['PAYMENT'] = array(
        '#type' => 'fieldset',
        '#prefix' => '<div class="span7 bookingPaymentDetails">',
        '#suffix' => '</div>',
        '#title' => '',
        '#description' => '<div class="bookingSubTitle">
        Promotional Code, Gift Card, Voucher
        </div>'
    );

    $enhancementsoutput = getPrintOrderWidget('PAYMENTS');

    $form['stepthree']['paymentwidget'] = array(
        '#type' => 'fieldset',
        '#prefix' => '<div class="span5">',
        '#suffix' => '</div>',
        '#title' => '',
        '#description' => $enhancementsoutput
    );


    $form['stepthree']['PAYMENT']['codes'] = array(
        '#type' => 'fieldset',
        '#prefix' => '<div class="headercontent textmarginTop">',
        '#suffix' => '</div>',
        '#title' => '',
        '#weight' => 1,
    );

    $form['stepthree']['PAYMENT']['codes']['promocodeWrapper'] = array(
        '#type' => 'markup',
        '#prefix' => '<div class="promocodeVoucherWrapper">',
        '#suffix' => '</div>'
    );

    $form['stepthree']['PAYMENT']['codes']['promocodeWrapper']['messageWrapper'] = array(
        '#type' => 'markup',
        '#prefix' => '<div id="promocode_status_message">',
        '#suffix' => '</div>',
        '#weight' => 2,
    );

    $promocoderesult=$bookingCart->getPromoCode();
    if ($promocoderesult && $promocoderesult != "") {
    	
    	$form['stepthree']['PAYMENT']['codes']['promocodeWrapper']['prmocode'] = array(
    	 	'#id' => 'prmocode',
    	 	'#type' => 'textfield',
    	 	'#title' => '',
    	 	'#weight' => 3,
    	 	'#attributes' => array('placeholder' => t('Promo Code'), 'class' => array('inputfiledStyle inputfieldLarge'),'readonly' => true),
    	 	'#prefix' => '<div class="paymentPromoCode">',
    	 	'#suffix' => '</div>',
    	 	'#default_value' => "345",
    	   );
    	
        
    
    	$form['stepthree']['PAYMENT']['codes']['promocodeWrapper']['prmodeletebtn'] = array(
    	  		'#limit_validation_errors' => array(),
    	   		'#name' => 'removepromocode',
    	   		'#type' => 'submit',
    	   		'#value' => t('Remove'),
    	   		'#weight' => 4,
    	   		'#prefix' => '<div class="booknow share span2" id="promoapplybtn"><span class="booknow-inner">',
    	     	'#suffix' => '</span></div>',
    		 );
    }else{
    	
    	$form['stepthree']['PAYMENT']['codes']['promocodeWrapper']['prmocode'] = array(
    	 	'#id' => 'prmocode',
    	 	'#type' => 'textfield',
    	 	'#title' => '',
    	 	'#weight' => 3,
    	 	'#attributes' => array('placeholder' => t('Promo Code'), 'class' => array('inputfiledStyle inputfieldLarge')),
    	 	'#prefix' => '<div class="paymentPromoCode">',
    	 	'#suffix' => '</div>',
    	 	'#default_value' => isset($values['prmocode']) ? $values['prmocode'] : "",
    	   );

    	   $form['stepthree']['PAYMENT']['codes']['promocodeWrapper']['prmobtn'] = array(
    	  		'#limit_validation_errors' => array(),
    	   		'#name' => 'applypromocode',
    	   		'#type' => 'submit',
    	   		'#value' => t('Apply'),
    	   		'#weight' => 4,
    	   		'#prefix' => '<div class="booknow share span2" id="promoapplybtn"><span class="booknow-inner">',
    	     	'#suffix' => '</span></div>',
    		 );
    	
    }
    
    
    
    
    



 

    $form['promostatus'] = array(
        '#type' => 'hidden',
        '#value' => '',
    );


    $form['stepthree']['PAYMENT']['codes']['giftcardWrapper'] = array(
        '#type' => 'markup',
        '#prefix' => '<div class="promocodeVoucherWrapper giftcardVoucherWrapper">',
        '#suffix' => '</div>'
    );

    $form['stepthree']['PAYMENT']['codes']['giftcardWrapper']['promocodemessageWrapper'] = array(
        '#type' => 'markup',
        '#prefix' => '<div id="giftcard_status_message">',
        '#suffix' => '</div>',
        '#weight' => 1,
    );

    if($bookingCart->getGiftCardAmount()>0){
    	$form['stepthree']['PAYMENT']['codes']['giftcardWrapper']['giftcard'] = array(
    			'#type' => 'textfield',
    			'#title' => '',
    			'#weight' => 2,
    			'#attributes' => array('placeholder' => t('Gift Card Number'), 'class' => array('inputfiledStyle inputfieldLarge'),'readonly' => true),
    			'#prefix' => '<div class="paymentPromoCode">',
    			'#suffix' => '</div>',
    			'#default_value' => isset($values['giftcard']) ? $values['giftcard'] : "",
    	);
    	
    	
    	
    	$form['stepthree']['PAYMENT']['codes']['giftcardWrapper']['giftcardbtn'] = array(
    			'#type' => 'submit',
    			'#name' => 'removegiftcard',
    			'#value' => t('Remove'),
    			'#weight' => 3,
    			'#prefix' => '<div class="booknow share span2" id="shareBtn"><span class="booknow-inner">',
    			'#suffix' => '</span></div>',
    			'#limit_validation_errors' => array(),
  
    	);
    }else{
    	$form['stepthree']['PAYMENT']['codes']['giftcardWrapper']['giftcard'] = array(
    			'#type' => 'textfield',
    			'#title' => '',
    			'#weight' => 2,
    			'#attributes' => array('placeholder' => t('Gift Card Number'), 'class' => array('inputfiledStyle inputfieldLarge')),
    			'#prefix' => '<div class="paymentPromoCode">',
    			'#suffix' => '</div>',
    			'#default_value' => isset($values['giftcard']) ? $values['giftcard'] : "",
    	);
    	
    	
    	
    	$form['stepthree']['PAYMENT']['codes']['giftcardWrapper']['giftcardbtn'] = array(
    			'#type' => 'submit',
    			'#name' => 'applygiftcard',
    			'#value' => t('Apply'),
    			'#weight' => 3,
    			'#prefix' => '<div class="booknow share span2" id="shareBtn"><span class="booknow-inner">',
    			'#suffix' => '</span></div>',
    			'#limit_validation_errors' => array(),

    	);
    }
    
    
    


    $form ['stepthree'] ['PAYMENT'] ['codes'] ['giftcardWrapper'] ['giftcardbalance'] = array(
        '#type' => 'markup',
        '#markup' => '<div style="width: 100%; height: 23px; float: left;">
              <a href="#balanceCheckerPopup" role="button" data-toggle="modal" style="font-size: 13px;">CHECK GIFT CARD BALANCE:</a></div>',
        '#weight' => 4
    );

    $form['stepthree']['PAYMENT']['codes']['giftcardWrapper']['balancecheckpopup'] = array(
        '#type' => 'markup',
        '#markup' => '<div id="balanceCheckerPopup" class="modal hide" role="dialog" aria-labelledby="myModalLabel" aria-hidden="true" style="display: none;">
                <div class="modal-header">
                    <h4>Gift Card Balance Checker<button type="button" id="balanceCheckerPopupClose"  class="close">x</button></h4>
                </div>
                <div class="modal-body">
                    <div class="row-fluid">
                        <div class="span3"><label>Balance Checker: </label></div>
                        <div class="span9"><input type="text" name="giftcardpop_number" id="giftcardpop_number" /></div>
                </div>
                    <div class="row-fluid">
                        <div class="span3"><label>Balance Amount: </label></div>
                        <div class="span9"><span id="giftcardbalance_amount"></span></div>
                    </div>
                </div>
                <div class="modal-footer">
                    <span id="loaderText" style="display: none;">Please wait...</span>
                    <div class="booknow resCheckboxshare" id="shareBtn">
                        <input type="button" name="submit" value="Submit" class="resCheckbox-booknow-inner" onclick="checkbalance();"/>
                    </div>
                </div>
            </div>'
    );


    $form['stepthree']['PAYMENT']['codes']['giftcardvoucherWrapper'] = array(
        '#type' => 'markup',
        '#prefix' => '<div class="promocodeVoucherWrapper giftcardVoucherWrapper" style="margin-bottom: -6px;" id="vocuhers-fieldset-wrapper">',
        '#suffix' => '</div>',
    );


    if (empty($form_state['num_vouchers'])) {
        $form_state['num_vouchers'] = 1;
    }
   $vouchersData=array();
   $vouchersData=$bookingCart->getVouchers();
    
    for ($i = 1; $i <= $form_state['num_vouchers']; $i++) {
    	if ($i < 5) {
    		
    		if(isset($vouchersData[$i]) && count($vouchersData[$i])>0){
    			$form['stepthree']['PAYMENT']['codes']['giftcardvoucherWrapper']['voucher' . $i] = array(
    					'#type' => 'textfield',
    					'#attributes' => array('placeholder' => t('Voucher Code'), 'class' => array('inputfiledStyle inputfieldLarge'),'readonly' => true),
    					'#prefix' => '<div class="paymentPromoCode voucherWrap">',
    					'#suffix' => '</div>',
    					'#default_value' => isset($values['voucher' . $i]) ? $values['voucher' . $i] : "",
    			);
    			 
    			$form['stepthree']['PAYMENT']['codes']['giftcardvoucherWrapper']['vocuher_applybutton' . $i] = array(
    					'#type' => 'submit',
    					'#name' => 'voucher_'.$i,
    					'#value' => t('Remove'),
    					'#prefix' => '<div class="booknow share span2" id="shareBtn"><span class="booknow-inner">',
    					'#suffix' => '</span></div>',
    			);
    			
    		
    		}else{
    			
    			$form['stepthree']['PAYMENT']['codes']['giftcardvoucherWrapper']['voucher' . $i] = array(
    					'#type' => 'textfield',
    					'#attributes' => array('placeholder' => t('Voucher Code'), 'class' => array('inputfiledStyle inputfieldLarge')),
    					'#prefix' => '<div class="paymentPromoCode voucherWrap">',
    					'#suffix' => '</div>',
    					'#default_value' => isset($values['voucher' . $i]) ? $values['voucher' . $i] : "",
    			);
    			 
    			$form['stepthree']['PAYMENT']['codes']['giftcardvoucherWrapper']['vocuher_applybutton' . $i] = array(
    					'#type' => 'submit',
    					'#name' => 'voucher_'.$i,
    					'#value' => t('Apply'),
    					'#prefix' => '<div class="booknow share span2" id="shareBtn"><span class="booknow-inner">',
    					'#suffix' => '</span></div>',
    			);
    			
    		}
    		
    		
    	}
    		
    	
    	
    		
    	

       
    }//for

    $form['stepthree']['PAYMENT']['codes']['vocuher_addbutton'] = array(
        '#type' => 'submit',
        '#value' => t('+ ADD ANOTHER VOUCHER CODE'),
        '#weight' => 7,
        '#limit_validation_errors' => array(),
        '#prefix' => '<div class="voucherSubmit">',
        '#suffix' => '</div>',
        '#submit' => array('ajax_add_voucher_action'),
        '#ajax' => array(
            'callback' => 'ajax_voucher_add_callback',
            'wrapper' => 'vocuhers-fieldset-wrapper',
        ),
    );




    //Guest Details
    $form['stepthree']['PAYMENT']['guest'] = array(
        '#type' => 'fieldset',
        '#prefix' => '<div class="guestDetailscontent legendTitle groupmarginTop">',
        '#suffix' => '</div>',
        '#title' => 'Guest Details',
        '#weight' => 8,
    );

    $form['stepthree']['PAYMENT']['guest']['guest_firstname'] = array(
        '#type' => 'textfield',
        '#title' => '',
        '#weight' => 9,
        '#attributes' => array('placeholder' => t('*First Name'), 'class' => array('inputfiledStyle inputfieldMedium')),
        '#default_value' => isset($values['guest_firstname']) ? $values['guest_firstname'] : "",
    );

    $form['stepthree']['PAYMENT']['guest']['guest_lastname'] = array(
        '#type' => 'textfield',
        '#title' => '',
        '#weight' => 10,
        '#attributes' => array('placeholder' => t('*Last Name'), 'class' => array('inputfiledStyle inputfieldMedium')),
        '#default_value' => isset($values['guest_lastname']) ? $values['guest_lastname'] : "",
    );

    $form['stepthree']['PAYMENT']['guest']['guest_address'] = array(
        '#type' => 'textfield',
        '#title' => '',
        '#weight' => 11,
        '#attributes' => array('placeholder' => t('*Address'), 'class' => array('inputfiledStyle inputfieldMedium')),
        '#default_value' => isset($values['guest_address']) ? $values['guest_address'] : "",
    );

    $form['stepthree']['PAYMENT']['guest']['guest_otheraddress'] = array(
        '#type' => 'textfield',
        '#title' => '',
        '#weight' => 12,
        '#attributes' => array('class' => array('inputfiledStyle inputfieldMedium')),
        '#default_value' => isset($values['guest_otheraddress']) ? $values['guest_otheraddress'] : "",
    );


    $form['stepthree']['PAYMENT']['guest']['guest_city'] = array(
        '#type' => 'textfield',
        '#title' => '',
        '#weight' => 13,
        '#attributes' => array('placeholder' => t('*City'), 'class' => array('inputfiledStyle inputfieldMedium')),
        '#default_value' => isset($values['guest_city']) ? $values['guest_city'] : "",
    );


    $usStatesObject = uc_zone_select();
    $usStatesList = $usStatesObject['#options'];

    $form['stepthree']['PAYMENT']['guest']['guest_state'] = array(
        '#type' => 'select',
        '#title' => t(''), //later we need to change this one as dynamic
        '#weight' => 14,
        '#options' => $usStatesList,
        '#default_value' => 1,
        '#attributes' => array('placeholder' => t('*State'), 'class' => array('selectFieldStyle selectFieldVerySmall pull-left')),
        '#default_value' => isset($values['guest_state']) ? $values['guest_state'] : "",
        '#empty_option' => t('*State'),
    );


    //zip code
    $form['stepthree']['PAYMENT']['guest']['guest_zipcode'] = array(
        '#type' => 'textfield',
        '#title' => '',
        '#weight' => 15,
        '#attributes' => array('placeholder' => t('*Zipcode'), 'class' => array('inputfiledStyle inputfieldSmallxl pull-left')),
        '#default_value' => isset($values['guest_zipcode']) ? $values['guest_zipcode'] : "",
    );

    $form['stepthree']['PAYMENT']['guest']['guestphoneWrapper'] = array(
        '#type' => 'markup',
        '#prefix' => '<div>',
        '#suffix' => '</div>',
        '#weight' => 16,
    );

    $form['stepthree']['PAYMENT']['guest']['guestphoneWrapper']['guest_areacode'] = array(
		'#id'=>'guestAreaCode',
    	'#type' => 'textfield',
        //'#title_display' => 'invisible',
        '#title' => t('*Phone Number'),
        '#attributes' => array('placeholder' => t(''), 'class' => array('inputfiledStyle inputfieldVerySmall pull-left')),
        '#weight' => 17,
        //'#default_value' => isset($values['billing_phone']) ? $values['billing_phone'] : "",
        '#default_value' => isset($values['guest_areacode']) ? $values['guest_areacode'] : "",
        '#maxlength' => 5,
    );
    $form['stepthree']['PAYMENT']['guest']['guestphoneWrapper']['guest_prefix'] = array(
    	'#id'=>'guestPrefix',
    	'#type' => 'textfield',
        '#title_display' => 'invisible',
        '#title' => t('Phone Number'),
        '#attributes' => array('placeholder' => t(''), 'class' => array('inputfiledStyle inputfieldVerySmall pull-left')),
        '#default_value' => isset($values['guest_prefix']) ? $values['guest_prefix'] : "",
        '#weight' => 18,
        '#maxlength' => 3,
    );
    $form['stepthree']['PAYMENT']['guest']['guestphoneWrapper']['guest_linenumber'] = array(
    	'#id'=>'guestLineNumber',
    	'#type' => 'textfield',
        '#title_display' => 'invisible',
        '#title' => t('Phone Number'),
        '#attributes' => array('placeholder' => t(''), 'class' => array('inputfiledStyle inputfieldVerySmall')),
        '#weight' => 19,
        '#default_value' => isset($values['guest_linenumber']) ? $values['guest_linenumber'] : "",
        '#maxlength' => 4,
    );



    //Email Address
    $form['stepthree']['PAYMENT']['guest']['guest_email'] = array(
        '#type' => 'textfield',
        '#title' => '',
        '#weight' => 20,
        '#attributes' => array('placeholder' => t('*Email Address'), 'class' => array('inputfiledStyle inputfieldMedium')),
        '#default_value' => isset($values['guest_email']) ? $values['guest_email'] : "",
    );


    $form['stepthree']['PAYMENT']['guest']['guest_confirmemail'] = array(
        '#type' => 'textfield',
        '#title' => '',
        '#weight' => 21,
        '#attributes' => array('placeholder' => t('*Confirm Email Address'), 'class' => array('inputfiledStyle inputfieldMedium')),
        '#default_value' => isset($values['guest_confirmemail']) ? $values['guest_confirmemail'] : "",
    );

    $form['stepthree']['PAYMENT']['guest']['desc'] = array(
        '#type' => 'markup',
        '#markup' => '<div style="display:block;"><span class="paymentLable">Let us know your special occasion dates to receive special offers for your celebrations</span></div>',
        '#weight' => 22,
    );
   $format = 'M-d';

     $form['stepthree']['PAYMENT']['guest']['guest_dob_month'] = array(
    		'#type' => 'select',
    		'#title' => t('Date of Birth'),
    		'#weight' => 23,
    		'#options' => array(1 => t('January'), 2 => t('February'),
    				3 => t('March'), 4 => t('April'),
    				5 => t('May'), 6 => t('June'),
    				7 => t('July'), 8 => t('August'),
    				9 => t('September'), 10 => t('October'),
    				11 => t('November'), 12 => t('December')
    		),
    		'#attributes' => array('placeholder' => t('Month'), 'class' => array('selectFieldStyle selectFieldVerySmall pull-left')),
    		'#default_value' => isset($values['guest_dob_month']) ? $values['guest_dob_month'] : '',
    		'#empty_option' => t('Month'),
    );
     
     
     
     
     $form['stepthree']['PAYMENT']['guest']['guest_dob_day'] = array(
     		'#type' => 'select',
     		'#title' => t(''), //later we need to change this one as dynamic
     		'#weight' => 24,
     		'#options' => drupal_map_assoc(range(1, 31)),
     		'#attributes' => array('placeholder' => t('Day'), 'class' => array('selectFieldStyle selectFieldVerySmall')),
     		'#default_value' => isset($values['guest_dob_day']) ? $values['guest_dob_day'] : '',
     		'#empty_option' => t('Day'),
     );
     
     
     $form['stepthree']['PAYMENT']['guest']['guest_aniverssary_month'] = array(
     		'#type' => 'select',
     		'#title' => t('Anniversary Date'),
     		'#weight' => 25,
     		'#options' => array(1 => t('January'), 2 => t('February'),
     				3 => t('March'), 4 => t('April'),
     				5 => t('May'), 6 => t('June'),
     				7 => t('July'), 8 => t('August'),
     				9 => t('September'), 10 => t('October'),
     				11 => t('November'), 12 => t('December')
     		),
     		'#attributes' => array('placeholder' => t('Month'), 'class' => array('selectFieldStyle selectFieldVerySmall pull-left')),
     		'#default_value' => isset($values['guest_aniverssary_month']) ? $values['guest_aniverssary_month'] : '',
     		'#empty_option' => t('Month'),
     );
      
      
      
      
     $form['stepthree']['PAYMENT']['guest']['guest_aniverssary_day'] = array(
     		'#type' => 'select',
     		'#title' => t(''), //later we need to change this one as dynamic
     		'#weight' => 26,
     		'#options' => drupal_map_assoc(range(1, 31)),
     		'#attributes' => array('placeholder' => t('Day'), 'class' => array('selectFieldStyle selectFieldVerySmall')),
     		'#default_value' => isset($values['guest_aniverssary_day']) ? $values['guest_aniverssary_day'] : '',
     		'#empty_option' => t('Day'),
     );
     
     
    //Billing Address
    $form['stepthree']['PAYMENT']['billing'] = array(
        '#type' => 'fieldset',
        '#prefix' => '<div class="billingDetailscontent legendTitle groupmarginTop">',
        '#suffix' => '</div>',
        '#title' => 'Billing Details',
        '#weight' => 27,
    );

    $form['stepthree']['PAYMENT']['billing']['sameascheckbox'] = array(
        '#type' => 'checkbox',
        '#weight' => 28,
        '#title' => t('<span class="paymentLable">My billing address is the same as my guest details</span>'),
        '#attributes' => array('class' => array('ticketsameas')),
        '#default_value' => isset($values['sameascheckbox']) ? $values['sameascheckbox'] : "",
    );

    $form['stepthree']['PAYMENT']['billing']['billing_firstname'] = array(
        '#type' => 'textfield',
        '#title' => '',
        '#weight' => 29,
        '#attributes' => array('placeholder' => t('*First Name'), 'class' => array('inputfiledStyle inputfieldMedium')),
        '#default_value' => isset($values['billing_firstname']) ? $values['billing_firstname'] : "",
    );

    $form['stepthree']['PAYMENT']['billing']['billing_lastname'] = array(
        '#type' => 'textfield',
        '#title' => '',
        '#weight' => 30,
        '#attributes' => array('placeholder' => t('*Last Name'), 'class' => array('inputfiledStyle inputfieldMedium')),
        '#default_value' => isset($values['billing_lastname']) ? $values['billing_lastname'] : "",
    );

    $form['stepthree']['PAYMENT']['billing']['billing_address'] = array(
        '#type' => 'textfield',
        '#title' => '',
        '#weight' => 31,
        '#attributes' => array('placeholder' => t('*Address'), 'class' => array('inputfiledStyle inputfieldMedium')),
        '#default_value' => isset($values['billing_address']) ? $values['billing_address'] : "",
    );

    $form['stepthree']['PAYMENT']['billing']['billing_otheraddress'] = array(
        '#type' => 'textfield',
        '#title' => '',
        '#weight' => 32,
        '#attributes' => array('class' => array('inputfiledStyle inputfieldMedium')),
        '#default_value' => isset($values['billing_otheraddress']) ? $values['billing_otheraddress'] : "",
    );


    $form['stepthree']['PAYMENT']['billing']['billing_city'] = array(
        '#type' => 'textfield',
        '#title' => 'City',
        '#title_display' => 'invisible',
        '#weight' => 33,
        '#attributes' => array('placeholder' => t('*City'), 'class' => array('inputfiledStyle inputfieldMedium')),
        '#default_value' => isset($values['billing_city']) ? $values['billing_city'] : "",
    );




    $form['stepthree']['PAYMENT']['billing']['billing_state'] = array(
        '#type' => 'select',
        '#title' => t(''), //later we need to change this one as dynamic
        '#weight' => 34,
        '#options' => $usStatesList,
        '#default_value' => 1,
        '#attributes' => array('placeholder' => t('*State'), 'class' => array('selectFieldStyle selectFieldVerySmall pull-left')),
        '#default_value' => isset($values['billing_state']) ? $values['billing_state'] : "",
        '#empty_option' => t('*State'),
    );


    $form['stepthree']['PAYMENT']['billing']['billing_zipcode'] = array(
        '#type' => 'textfield',
        '#title' => '',
        '#weight' => 35,
        '#attributes' => array('placeholder' => t('*Zipcode'), 'class' => array('inputfiledStyle inputfieldSmallxl pull-left')),
        '#default_value' => isset($values['billing_zipcode']) ? $values['billing_zipcode'] : "",
    );


    $form['stepthree']['PAYMENT']['billing']['billingphoneWrapper'] = array(
        '#type' => 'markup',
        '#prefix' => '<div>',
        '#suffix' => '</div>',
        '#weight' => 36,
    );

    $form['stepthree']['PAYMENT']['billing']['billingphoneWrapper']['billing_areacode'] = array(
		'#id'=>'billingAreaCode',
    	'#type' => 'textfield',
        '#title' => t('*Phone Number'),
        '#attributes' => array('placeholder' => t(''), 'class' => array('inputfiledStyle inputfieldVerySmall pull-left')),
        '#weight' => 37,
        '#default_value' => isset($values['billing_areacode']) ? $values['billing_areacode'] : "",
        '#maxlength' => 5,
    );
    $form['stepthree']['PAYMENT']['billing']['billingphoneWrapper']['billing_prefix'] = array(
    	'#id'=>'billingPrefix',
    	'#type' => 'textfield',
        '#title_display' => 'invisible',
        '#title' => t('Phone Number'),
        '#attributes' => array('placeholder' => t(''), 'class' => array('inputfiledStyle inputfieldVerySmall pull-left')),
        '#default_value' => isset($values['billing_prefix']) ? $values['billing_prefix'] : "",
        '#weight' => 38,
        '#maxlength' => 3,
    );
    $form['stepthree']['PAYMENT']['billing']['billingphoneWrapper']['billing_linenumber'] = array(
    	'#id'=>'billingLineNumber',
    	'#type' => 'textfield',
        '#title_display' => 'invisible',
        '#title' => t('Phone Number'),
        '#attributes' => array('placeholder' => t(''), 'class' => array('inputfiledStyle inputfieldVerySmall')),
        '#weight' => 39,
        '#default_value' => isset($values['billing_linenumber']) ? $values['billing_linenumber'] : "",
        '#maxlength' => 4,
    );


    $form['stepthree']['PAYMENT']['billing']['billing_email'] = array(
        '#type' => 'textfield',
        '#title' => '',
        '#weight' => 40,
        '#attributes' => array('placeholder' => t('*Email Address'), 'class' => array('inputfiledStyle inputfieldMedium')),
        '#default_value' => isset($values['billing_email']) ? $values['billing_email'] : "",
    );


    $form['stepthree']['PAYMENT']['billing']['billing_confirmemail'] = array(
        '#type' => 'textfield',
        '#title' => '',
        '#weight' => 41,
        '#attributes' => array('placeholder' => t('*Confirm Email Address'), 'class' => array('inputfiledStyle inputfieldMedium')),
        '#default_value' => isset($values['billing_confirmemail']) ? $values['billing_confirmemail'] : "",
    );


    $form['stepthree']['PAYMENT']['credit'] = array(
        '#type' => 'fieldset',
        '#prefix' => '<div class="creditDetailscontent legendTitle groupmarginTop">',
        '#suffix' => '</div>',
        '#title' => 'Credit Card Details',
        '#weight' => 42,
    );


    $form['stepthree']['PAYMENT']['credit']['creditFirstName'] = array(
        '#type' => 'textfield',
        '#title' => '',
        '#weight' => 43,
        '#attributes' => array('placeholder' => t('*First Name as it appears on card'), 'class' => array('inputfiledStyle inputfieldMedium')),
        '#default_value' => isset($values['creditFirstName']) ? $values['creditFirstName'] : "",
    );

    $form['stepthree']['PAYMENT']['credit']['creditLastName'] = array(
        '#type' => 'textfield',
        '#title' => '',
        '#weight' => 44,
        '#attributes' => array('placeholder' => t('*Last Name as it appears on card'), 'class' => array('inputfiledStyle inputfieldMedium')),
        '#default_value' => isset($values['creditLastName']) ? $values['creditLastName'] : "",
    );

    $form['stepthree']['PAYMENT']['credit']['creditCardType'] = array(
        '#type' => 'select',
        '#title' => t(''), //later we need to change this one as dynamic
        '#weight' => 45,
        '#options' => array(
            'visa' => 'Visa',
            'mastercard' => 'Master Card',
            'discover' => 'Discover',
            'americanexpress' => 'American Express',
        ),
        '#attributes' => array('placeholder' => t('*Card Type'), 'class' => array('selectFieldStyle selectFieldMedium')),
        '#default_value' => isset($values['creditCardType']) ? $values['creditCardType'] : 'visa',
        '#empty_option' => t('*Card Type'),
    );


    $form['stepthree']['PAYMENT']['credit']['creditCardNumber'] = array(
        '#type' => 'textfield',
        '#title' => '',
        '#weight' => 46,
        '#attributes' => array('placeholder' => t('*Card Number'), 'class' => array('inputfiledStyle inputfieldMedium pull-left')),
        '#default_value' => isset($values['creditCardNumber']) ? $values['creditCardNumber'] : "",
    );

    $form['stepthree']['PAYMENT']['credit']['creditdatesWrapper'] = array(
        '#type' => 'markup',
        '#prefix' => '<div class="creditdatesWrapholder">',
        '#suffix' => '</div>',
        '#weight' => 47,
    );

    $form['stepthree']['PAYMENT']['credit']['creditdatesWrapper']['creditExpMonth'] = array(
        '#type' => 'select',
        '#title' => t(''),
        '#weight' => 48,
        '#options' => array(1 => t('01 - January'), 2 => t('02 - February'),
            3 => t('03 - March'), 4 => t('04 - April'),
            5 => t('05 - May'), 6 => t('06 - June'),
            7 => t('07 - July'), 8 => t('08 - August'),
            9 => t('09 - September'), 10 => t('10 - October'),
            11 => t('11 - November'), 12 => t('12 - December')
        ),
        '#attributes' => array('placeholder' => t('*ExpMonth'), 'class' => array('selectFieldStyle selectFieldVerySmall pull-left')),
        '#default_value' => isset($values['creditExpMonth']) ? $values['creditExpMonth'] : '',
        '#empty_option' => t('*Month'),
    );




    $min = intval(date('Y'));
    $max = intval(date('Y')) + 20;

    $form['stepthree']['PAYMENT']['credit']['creditdatesWrapper']['creditExpYear'] = array(
        '#type' => 'select',
        '#title' => t(''), //later we need to change this one as dynamic
        '#weight' => 49,
        '#options' => drupal_map_assoc(range($min, $max)),
        '#attributes' => array('placeholder' => t('ExpYear'), 'class' => array('selectFieldStyle selectFieldVerySmall')),
        '#default_value' => isset($values['creditExpYear']) ? $values['creditExpYear'] : '',
        '#empty_option' => t('*Year'),
    );
    
    
    

    $form['stepthree']['PAYMENT']['credit']['creditcid'] = array(
        '#type' => 'textfield',
        '#title' => '',
        '#weight' => 50,
        '#attributes' => array('placeholder' => t('*CID'), 'class' => array('inputfiledStyle inputfieldMedium')),
        '#default_value' => isset($values['creditcid']) ? $values['creditcid'] : '',
        //'#description' => '<span><abbr title="Customer Identification Number" class="initialism">?</abbr></span>',
    	'#description'=>'<a onclick="window.open(this.href, \'CVV_Info\', \'toolbar=0,scrollbars=1,location=0,statusbar=0,menubar=0,resizable=1,width=480,height=460\'); return false;" href="'.base_path().'cart/checkout/credit/cvv_info"><span><abbr title="Customer Identification Number" class="initialism">?</abbr></span></a>',
        '#maxlength' => 4,
    );

    $form['stepthree']['PAYMENT']['promotions'] = array(
        '#type' => 'fieldset',
        '#prefix' => '<div class="headercontent">',
        '#suffix' => '</div>',
        '#title' => '',
        '#weight' => 51,
    );

    $form['stepthree']['PAYMENT']['promotions']['findingusby'] = array(
        '#type' => 'select',
        '#title' => t(''), //later we need to change this one as dynamic
        '#options' => array(
            'How did you hear about us?' => 'How did you hear about us?',
            'Internet Search' => 'Internet Search',
            'Newspaper' => 'Newspaper',
            'Magazine' => 'Magazine',
            'Email' => 'Email',
            'Brochure' => 'Brochure',
            'Radio or TV' => 'Radio or TV',
            'Concierge' => 'Concierge',
            'Word of Mouth' => 'Word of Mouth',
            'Return Customer' => 'Return Customer',
            'Online Ads' => 'Online Ads',
            'Daily Deal' => 'Daily Deal',
            'Seattle Times' => 'Seattle Times',
            'Pandora' => 'Pandora',
            'Gift Certificate' => 'Gift Certificate'
        ),
        '#default_value' => 'How did you hear about us?',
        '#attributes' => array('class' => array('selectFieldStyle selectFieldMedium groupmarginTop')),
        '#default_value' => isset($values['findingusby']) ? $values['findingusby'] : '',
    );

    $form['stepthree']['PAYMENT']['promotions']['promotiondiscounts'] = array(
        '#type' => 'checkbox',
        '#weight' => 52,
        '#title' => t('<span class="paymentLable">I would like to recieve Waterways special promotions and discounts by email</span>'),
        '#default_value' => isset($values['promotiondiscounts']) ? $values['promotiondiscounts'] : '',
        //'#attributes' => array('checked' => TRUE),
    );

    $form['stepthree']['PAYMENT']['promotions']['notemessage'] = array(
        '#type' => 'markup',
        '#weight' => 53,
        '#prefix' => '<div class="textmarginTop"><div class="row-fluid"><div class="span12"><label style="color: #818181; margin-bottom: 30px;">You will have the opportunity to review your order on the next page before proceeding with purchase.</label>',
        '#suffix' => '</div></div></div>'
    );



    //Not Inserting any card details that would be handled at booking Page section
    $form['btnwrapper'] = array(
        '#type' => 'markup',
        '#prefix' => '<div class="bookingPaymentBtnWrapper textmarginTop"><div class="row-fluid">',
        '#suffix' => '</div></div>'
    );

    $form['btnwrapper']['back'] = array(
        '#id' => 'paymentbackbutton',
        '#type' => 'submit',
        '#value' => t('Go Back'),
        '#prefix' => '<div class="span3"><div class="line"></div></div><div class="booknowBack share span3" id="shareBtn"><span class="booknowBack-inner">',
        '#suffix' => '</span></div>',
        '#attributes' => array('class' => array('goBackbtn')),
    );


    $form['btnwrapper']['backtoticketorder'] = array(
        '#id' => 'backtoticketorderbutton',
        '#type' => 'submit',
        '#value' => t('Go Back To Tickets'),
        '#prefix' => '<div style="display:none;">',
        '#suffix' => '</div>',
    );

    $form['btnwrapper']['next'] = array(
        '#type' => 'submit',
        '#value' => t('Save & Continue'),
        '#prefix' => '<div class="booknow share span3" id="shareBtn"><span class="booknow-inner">',
        '#suffix' => '</span></div><div class="span3"><div class="line"></div></div>',
    );
    return $form;
}

/**
 * REVIEW_ORDER
 */
function booking_review_form($form, &$form_state) {
	
	$bookingCart= new BookingCart();
	$userdata=$bookingCart->getUserInfo();
	$userInfo=$userdata['info'];
	$paymentStepvalues = isset($form_state['multistep_values']['PAYMENT']) ? $form_state['multistep_values']['PAYMENT'] : array();
	$receiveEmailNotifications=isset($paymentStepvalues['promotiondiscounts']) ? $paymentStepvalues['promotiondiscounts'] : '';
	
	
	
    $values = isset($form_state['multistep_values']['REVIEW_ORDER']) ? $form_state['multistep_values']['REVIEW_ORDER'] : array();
    $guestDetails = '';
    $guestDetails .= '<h3>Guest Details</h3>';
    $guestDetails .= '<div class="row-fluid"><div class="span12">' . $userInfo['guest_firstname'] . ' ' . $userInfo['guest_lastname'] . ' </div></div>';
    $guestDetails .= '<div class="row-fluid"><div class="span12">' . $userInfo['guest_address'] .'</div></div>';
    
    if(!empty($userInfo['guest_otheraddress'])){
    	$guestDetails .= '<div class="row-fluid"><div class="span12">' .$userInfo['guest_otheraddress'].'</div></div>';
    }
    $satecityzip = $userInfo['guest_city'] . ' , ' . uc_zone_get_by_id($userInfo['guest_state']) . ', ' . $userInfo['guest_zipcode'];
    $guestDetails .= '<div class="row-fluid"><div class="span12">' . $satecityzip . '</div></div>';
    
    $guestPhone=$userInfo['guest_areacode']." ".$userInfo['guest_prefix']."  ".$userInfo['guest_linenumber'];
    
    $guestDetails .= '<div class="row-fluid"><div class="span12">' . $guestPhone . '</div></div>';
    $guestDetails .= '<div class="row-fluid"><div class="span12">' . $userInfo['guest_email'] . '</div></div>';
    $billingaddress = '';
    $billingaddress .= '<h3>Billing Details</h3>';
    $billingaddress .= '<div class="row-fluid"><div class="span12">' . $userInfo['billing_firstname'] . ' ' . $userInfo['billing_lastname'] . '</div></div>';
    $billingaddress .= '<div class="row-fluid"><div class="span12">' . $userInfo['billing_address'] .'</div></div>';

    if(!empty($userInfo['billing_otheraddress'])){
    	$billingaddress .= '<div class="row-fluid"><div class="span12">' . $userInfo['billing_otheraddress']. '</div></div>';
    }
    
    
    $satecityzipbilling = $userInfo['billing_city'] . ',  ' . uc_zone_get_by_id($userInfo['billing_state']) . ', ' . $userInfo['billing_zipcode'];
    $billingaddress .= '<div class="row-fluid"><div class="span12">' . $satecityzipbilling . '</div></div>';
    
    $billingPhone=$userInfo['billing_areacode']." ".$userInfo['billing_prefix']."  ".$userInfo['billing_linenumber'];
    $billingaddress .= '<div class="row-fluid"><div class="span12">' . $billingPhone . '</div></div>';
    $billingaddress .= '<div class="row-fluid"><div class="span12">' . $userInfo['billing_email'] . '</div></div>';
    

    $creaditDetails = '';
    $creaditDetails .= '<h3>Credit Card Details</h3>';
    $creaditDetails .= '<div class="row-fluid"><div class="span12">' . ucfirst($form_state['values']['creditCardType']) . '</div></div>';
    $cc_number = $form_state['values']['creditCardNumber'];
    $creaditDetails .= '<div class="row-fluid"><div class="span12">Credit Card: <span class="creditcardvalue">' . maskCreditCard($cc_number) . '</span></div></div>';
    $cardexpdate = $form_state['values']['creditExpMonth'] . '/' . $form_state['values']['creditExpYear'];
    $creaditDetails .= '<div class="row-fluid"><div class="span12">Exp date: <span class="creditcardvalue">' . $cardexpdate . '</span></div></div>';
    $policyconditions = '';
    $policyconditions .= '<h3>Cancellation Policy</h3>';
    $policyconditions .= '<p>Actually cliche authentic, raw denim keytar marfa flexitarian farm-to-table church-key VHS fap selfies biodiesel next level.</p>';


    $ordersidewidget = getPrintOrderWidget('REVIEW');

    $form['REVIEW_ORDER'] = array(
        '#type' => 'fieldset',
        '#title' => '',
        '#weight' => 1,
        '#prefix' => '<div class="row-fluid">',
        '#suffix' => '</div>',
    );
    $form['REVIEW_ORDER']['reviewdetails'] = array(
        '#type' => 'fieldset',
        '#title' => '',
        '#weight' => 2,
        '#prefix' => '<div class="span7">',
        '#suffix' => '</div>',
    );
    $form['REVIEW_ORDER']['reviewdetails']['guest'] = array(
        '#type' => 'fieldset',
        '#title' => '',
        '#weight' => 3,
        '#prefix' => '<div class="row-fluid">',
        '#suffix' => '</div>',
    );
    $form['REVIEW_ORDER']['reviewdetails']['guest']['address'] = array(
        '#type' => 'fieldset',
        '#title' => '',
        '#value' => $guestDetails,
        '#weight' => 4,
        '#prefix' => '<div class="span6">',
        '#suffix' => '</div>',
    );

    $form['REVIEW_ORDER']['reviewdetails']['guest']['billingaddress'] = array(
    		'#type' => 'fieldset',
    		'#title' => '',
    		'#value' => $billingaddress,
    		'#weight' => 5,
    		'#prefix' => '<div class="span6">',
    		'#suffix' => '</div>',
    );
    
    
    $form['REVIEW_ORDER']['reviewdetails']['billing'] = array(
        '#type' => 'fieldset',
        '#title' => '',
        '#weight' => 7,
        '#prefix' => '<div class="row-fluid">',
        '#suffix' => '</div>',
    );
    
    $form['REVIEW_ORDER']['reviewdetails']['billing']['creditcard'] = array(
    		'#type' => 'fieldset',
    		'#title' => '',
    		'#value' => $creaditDetails,
    		'#weight' => 8,
    		'#prefix' => '<div class="span6">',
    		'#suffix' => '</div><div class="span6"></div>',
    );
    
    
    
    $form['REVIEW_ORDER']['reviewdetails']['conditional'] = array(
        '#type' => 'fieldset',
        '#title' => '',
        '#weight' => 9,
        '#prefix' => '<div class="row-fluid">',
        '#suffix' => '</div>',
    );
    $form['REVIEW_ORDER']['reviewdetails']['conditional']['ploicydescription'] = array(
        '#type' => 'fieldset',
        '#title' => '',
        '#value' => $policyconditions,
        '#weight' => 10,
        '#prefix' => '<div class="span12 cancellationPolicy">',
        '#suffix' => '</div>',
    );
    $form['REVIEW_ORDER']['reviewdetails']['conditional']['promotionaldiscountsenable'] = array(
        '#type' => 'checkbox',
        '#title' => t('I would like to recieve Waterways special promotions and discounts by email.'),
        '#prefix' => '<div class="span12" style="display:none;">',
        '#suffix' => '</div>',
        '#weight' => 11,
        '#default_value' =>$receiveEmailNotifications
    );
    $form['REVIEW_ORDER']['reviewdetails']['conditional']['agreement'] = array(
        '#type' => 'checkbox',
        '#title' => t('I have  read and agree to the terms and conditions.'),
        '#prefix' => '<div class="span12">',
        '#suffix' => '</div>',
        '#weight' => 12,
        //'#attributes' => array('checked' => 'checked'),
    );
    $form['REVIEW_ORDER']['orderwidget'] = array(
        '#type' => 'fieldset',
        '#title' => '',
        '#description' => $ordersidewidget,
        '#weight' => 13,
        '#prefix' => '<div class="span5">',
        '#suffix' => '</div>',
    );


    //Now Dividing the Above row-fluid
    $form['btnwrapper'] = array(
        '#type' => 'markup',
        '#prefix' => '<div class="bookingPaymentBtnWrapper groupmarginTop"><div class="row-fluid">',
        '#suffix' => '</div></div>',
        '#weight' => 15
    );

    $form['btnwrapper']['back'] = array(
        '#type' => 'submit',
        '#value' => t('Go Back'),
        '#prefix' => '<div class="span3"><div class="line"></div></div><div class="booknowBack share span3" id="shareBtn"><span class="booknowBack-inner">',
        '#suffix' => '</span></div>',
        '#weight' => 16,
        '#attributes' => array('class' => array('goBackbtn')),
    );

    $form['btnwrapper']['next'] = array(
        '#type' => 'submit',
        '#value' => t('Purchase'),
        '#prefix' => '<div class="booknow share span3" id="shareBtn"><span class="booknow-inner">',
        '#suffix' => '</span></div><div class="span3"><div class="line"></div></div>',
        '#weight' => 17,
        '#states' => array(
            'enabled' => array(
                ':input[name = "agreement"]' => array('checked' => TRUE),
            ),
        ),
    );
    return $form;
}



/*
 * Voucher
 */

function ajax_voucher_add_callback($form, $form_state) {
    return $form['stepthree']['PAYMENT']['codes']['giftcardvoucherWrapper'];
}

function ajax_add_voucher_action($form, &$form_state) {
    $form_state['num_vouchers'] ++;
    $form_state['rebuild'] = TRUE;
}






