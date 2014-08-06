<?php

function giftcard_form($form, &$form_state) {
	
	
	drupal_add_js(drupal_get_path('module', 'giftcard') . '/js/giftcard.js');
	drupal_add_js(drupal_get_path('module', 'giftcard') . '/js/mask.js');
	
    /*
     * Giftcard Form Stages
     * 1. Giftcard Details
     * 2. Mailing Address
     * 3. Card Details
     * 4. Review Details
     * 5. Confirmation
     */

	
    drupal_add_css(drupal_get_path('module', 'giftcard') . '/css/giftcard.css');
    if (!isset($form_state['stage'])){
    	$form_state['stage'] = 'gift_card_details';
    }

    

    $form = array();
    $form = giftcard_get_header($form, $form_state);

    switch ($form_state['stage']) {
        case 'gift_card_details':
            return giftcard_gift_card_details_form($form, $form_state);
            break;

        case 'mailing_address_details':
            return giftcard_mailing_address_details_form($form, $form_state);
            break;

        case 'credit_card_details':
            return giftcard_credit_card_details_form($form, $form_state);
            break;

        case 'review_details':
            return giftcard_review_details_form($form, $form_state);
            break;

        case 'giftcard_confirmation':
            return giftcard_confirmation_details_form($form, $form_state);

        default:
            return giftcard_gift_card_details_form($form, $form_state);
            break;
    }

    return $form;
}




// 1 Form for the gift_card_details step.
function giftcard_gift_card_details_form($form, &$form_state) {
	
	$form['#attributes']['autocomplete'] = 'off';
    $form['giftcarddetailsdivstart'] = array(
        '#type' => 'markup',
        '#markup' => '<div id="purchaseForm">',
    );

    $form['giftcarddetailsheader'] = array(
        '#type' => 'markup',
        '#markup' => '<h3>1. Enter gift card details</h3>',
        '#prefix' => '<div class="row-fluid"><div class="span12">',
        '#suffix' => '</div></div>',
    );

    $values = isset($form_state['multistep_values']['gift_card_details']) ? $form_state['multistep_values']['gift_card_details'] : array();
    $pregiftcardAmount = array(
        'setmyown' => 'Set My Own',
        5 => '$5',
        30 => '$30',
        40 => '$40');
    $pregiftcardamountdefaultvalue = isset($values['pregiftcardamount']) ? $values['pregiftcardamount'] : '';

    $form['gift_card_details']['pregiftcardamount'] = array(
        '#id' => 'pregiftcardamount',
        '#type' => 'select',
        '#title' => t('Amount'),
        '#options' => $pregiftcardAmount,
        '#default_value' => $pregiftcardamountdefaultvalue,
        '#description' => t(''),
        '#prefix' => '<div class="row-fluid dateFieldMedium "><div class="span12">',
        '#suffix' => '</div></div>',
        '#attributes' => array('class' => array('selectFieldStyle selectFieldMedium')),
    );


    $customgiftcardamountdefaultvalue = isset($values['customgiftcardamount']) ? $values['customgiftcardamount'] : '';
    $form['gift_card_details']['customgiftcardamount'] = array(
        '#id' => 'customgiftcardamount',
        '#type' => 'textfield',
        '#title' => t(''),
        '#default_value' => $customgiftcardamountdefaultvalue,
        '#size' => 60,
        '#maxlength' => 128,
        '#attributes' => array('placeholder' => t('$'), 'class' => array('inputfiledStyle inputfieldMedium')),
        '#prefix' => '<div class="row-fluid"><div class="span12">',
        '#suffix' => '</div></div>',
        '#states' => array(
            'visible' => array(
                ':input[name="pregiftcardamount"]' => array('#type' => 'hidden', 'value' => 'setmyown'),
            ),
        ),
    );

    $recipientemaildefaultvalue = isset($values['recipientemail']) ? $values['recipientemail'] : '';
    $form['gift_card_details']['recipientemail'] = array(
        '#id' => 'recipientemail',
        '#type' => 'textfield',
        '#title' => t(''),
        '#default_value' => $recipientemaildefaultvalue,
        '#size' => 60,
        '#maxlength' => 128,
        '#attributes' => array('placeholder' => t('Recipient Email'), 'class' => array('inputfiledStyle inputfieldMedium')),
        '#prefix' => '<div class="row-fluid"> <div class="span6">',
        '#suffix' => '</div>',
    );

    $yournamedefaultvalue = isset($values['yourname']) ? $values['yourname'] : '';
    $form['gift_card_details']['yourname'] = array(
        '#id' => 'yourname',
        '#type' => 'textfield',
        '#title' => t(''),
        '#default_value' => $yournamedefaultvalue,
        '#size' => 60,
        '#maxlength' => 128,
        '#attributes' => array('placeholder' => t('Your Name'), 'class' => array('inputfiledStyle inputfieldMedium')),
        '#prefix' => '<div class="span6">',
        '#suffix' => '</div></div>',
    );

    $customermessagedefaultvalue = isset($values['customermessage']) ? $values['customermessage'] : '';
    $form['gift_card_details']['customermessage'] = array(
        '#id' => 'customermessage',
        '#title' => t(''),
        '#type' => 'textarea',
        '#description' => t(''),
        '#default_value' => $customermessagedefaultvalue,
        '#attributes' => array('placeholder' => 'Message', 'class' => array('textareaStyle textarealarge')),
        '#prefix' => '<div class="row-fluid"> <div class="span12">',
        '#suffix' => '</div></div>',
    );


    $giftcardqty = array(1 => '1', 2 => '2', 3 => '3', 4 => '4');
    $giftcardqtydefaultvalue = isset($values['giftcardqty']) ? $values['giftcardqty'] : '';
    $form['gift_card_details']['giftcardqty'] = array(
        '#id' => 'giftcardqty',
        '#type' => 'select',
        '#title' => t('Quantity'),
        '#options' => $giftcardqty,
        '#default_value' => $giftcardqtydefaultvalue,
        '#description' => t(''),
        '#attributes' => array('class' => array('selectFieldStyle selectFieldMedium')),
        '#prefix' => '<div class="row-fluid"><div class="span6">',
        '#suffix' => '</div>',
    );


    $deliverydatedefaultvalue = isset($values['deliverydate']) ? $values['deliverydate'] : '';
    $form['gift_card_details']['deliverydate'] = array(
        '#id' => 'deliverydate',
        '#type' => 'date_popup',
        '#date_format' => 'Y-m-d',
        '#title' => t('Delivery Date'),
        '#default_value' => $deliverydatedefaultvalue,
        '#description' => '',
        '#attributes' => array('class' => array('dateFieldStyle dateFieldMedium')),
        '#prefix' => '<div class="span6">',
        '#suffix' => '</div></div>',
    );

    $form['gift_card_details']['promotioncode'] = array(
        '#type' => 'textfield',
        '#title' => '',
        '#attributes' => array('placeholder' => t('Promotion code if applicable'), 'class' => array('inputfiledStyle inputfieldMedium')),
        '#prefix' => '<div class="row-fluid"><div class="span12">',
        '#suffix' => '</div></div>',
    );

    $form['next'] = array(
        '#type' => 'submit',
        '#value' => t('Submit'),
        '#attributes' => array('class' => array('wcbooknowbtn')),
        '#prefix' => '<div class="wcbooknowbtnWrap marginTop">',
        '#suffix' => '</div>',
    );
    $form['giftcarddetailsdivend'] = array(
        '#type' => 'markup',
        '#markup' => '</div>',
    );
    return $form;
}





// 2 Form for the mailing_address_details step.
function giftcard_mailing_address_details_form($form, &$form_state) {
	$form['#attributes']['autocomplete'] = 'off';
    $form['giftcardmailingaddresstart'] = array(
        '#type' => 'markup',
        '#markup' => '<div id="purchaseForm">',
    );

    $form['giftcardmailingaddressheader'] = array(
        '#type' => 'markup',
        '#markup' => '<h3>2. Enter Mailing Address Details </h3>',
        '#prefix' => '<div class="row-fluid"><div class="span12">',
        '#suffix' => '</div></div>',
    );


    $values = isset($form_state['multistep_values']['mailing_address_details']) ? $form_state['multistep_values']['mailing_address_details'] : array();

    $customerfirstnamedefaultvalue = isset($values['customerfirstname']) ? $values['customerfirstname'] : '';
    $form['mailing_address_details']['customerfirstname'] = array(
        '#id' => 'customerfirstname',
        '#type' => 'textfield',
        '#title' => t(''),
        '#default_value' => $customerfirstnamedefaultvalue,
        '#size' => 60,
        '#maxlength' => 128,
        '#attributes' => array('placeholder' => t('*First Name'), 'class' => array('inputfiledStyle inputfieldMedium')),
        '#prefix' => '<div class="row-fluid"><div class="span6">',
        '#suffix' => '</div>',
    );


    $customerlastnamedefaultvalue = isset($values['customerlastname']) ? $values['customerlastname'] : '';
    $form['mailing_address_details']['customerlastname'] = array(
        '#id' => 'customerlastname',
        '#type' => 'textfield',
        '#title' => t(''),
        '#default_value' => $customerlastnamedefaultvalue,
        '#size' => 60,
        '#maxlength' => 128,
        '#attributes' => array('placeholder' => t('Last Name'), 'class' => array('inputfiledStyle inputfieldMedium')),
        '#prefix' => '<div class="span6">',
        '#suffix' => '</div></div>',
    );

    $customeraddressdefaultvalue = isset($values['customeraddress']) ? $values['customeraddress'] : '';
    $form['mailing_address_details']['customeraddress'] = array(
        '#id' => 'customeraddress',
        '#type' => 'textfield',
        '#title' => t(''),
        '#default_value' => $customeraddressdefaultvalue,
        '#size' => 60,
        '#maxlength' => 256,
        '#attributes' => array('placeholder' => t('*Address'), 'class' => array('inputfiledStyle inputfieldMedium')),
        '#prefix' => '<div class="row-fluid"><div class="span6">',
        '#suffix' => '</div>',
    );

    $customersecondaryaddressdefaultvalue = isset($values['customersecondaryaddress']) ? $values['customersecondaryaddress'] : '';
    $form['mailing_address_details']['customersecondaryaddress'] = array(
        '#id' => 'customersecondaryaddress',
        '#type' => 'textfield',
        '#title' => t(''),
        '#default_value' => $customersecondaryaddressdefaultvalue,
        '#size' => 60,
        '#maxlength' => 256,
        '#attributes' => array('placeholder' => t(''), 'class' => array('inputfiledStyle inputfieldMedium')),
        '#prefix' => '<div class="span6">',
        '#suffix' => '</div></div>',
    );


    $customercitydefaultvalue = isset($values['customercity']) ? $values['customercity'] : '';
    $form['mailing_address_details']['customercity'] = array(
        '#id' => 'customercity',
        '#type' => 'textfield',
        '#title' => t(''),
        '#default_value' => $customercitydefaultvalue,
        '#size' => 60,
        '#maxlength' => 256,
        '#attributes' => array('placeholder' => t('*City'), 'class' => array('inputfiledStyle inputfieldMedium')),
        '#prefix' => '<div class="row-fluid"><div class="span6">',
        '#suffix' => '</div>',
    );

    $usStatesObject = uc_zone_select();
    $usStatesList = $usStatesObject['#options'];

    $customerstatedefaultvalue = isset($values['customerstate']) ? $values['customerstate'] : '1';
    $form['mailing_address_details']['customerstate'] = array(
        '#id' => 'customerstate',
        '#type' => 'select',
        '#title' => t(''), //later we need to change this one as dynamic
        '#options' => $usStatesList,
        '#attributes' => array('placeholder' => t('*State'), 'class' => array('selectFieldStyle selectFieldMedium')),
        '#default_value' => $customerstatedefaultvalue,
        '#empty_option' => t('*State'),
        '#prefix' => '<div class="span6">',
        '#suffix' => '</div></div>',
    );

    $customerzipcodedefaultvalue = isset($values['customerzipcode']) ? $values['customerzipcode'] : '';
    $form['mailing_address_details']['customerzipcode'] = array(
        '#id' => 'customerzipcode',
        '#type' => 'textfield',
        '#title' => t(''),
        '#default_value' => $customerzipcodedefaultvalue,
        '#size' => 60,
        '#maxlength' => 8,
        '#attributes' => array('placeholder' => t('*Zip Code'), 'class' => array('inputfiledStyle inputfieldMedium')),
        '#prefix' => '<div class="row-fluid"><div class="span6">',
        '#suffix' => '</div>',
    );

    $customerphonenumberdefaultvalue = isset($values['customerphonenumber']) ? $values['customerphonenumber'] : '';
    $form['mailing_address_details']['customerphonenumber'] = array(
        '#id' => 'customerphonenumber',
        '#type' => 'textfield',
        '#title' => t(''),
        '#default_value' => $customerphonenumberdefaultvalue,
        '#size' => 60,
        '#maxlength' => 15,
        '#attributes' => array('placeholder' => t('*Phone Number'), 'class' => array('inputfiledStyle inputfieldMedium')),
        '#prefix' => '<div class="span6">',
        '#suffix' => '</div></div>',
    );

    $customeremaildefaultvalue = isset($values['customeremail']) ? $values['customeremail'] : '';
    $form['mailing_address_details']['customeremail'] = array(
        '#id' => 'customeremail',
        '#type' => 'textfield',
        '#title' => t(''),
        '#default_value' => $customeremaildefaultvalue,
        '#size' => 60,
        '#maxlength' => 50,
        '#attributes' => array('placeholder' => t('*Email'), 'class' => array('inputfiledStyle inputfieldMedium')),
        '#prefix' => '<div class="row-fluid"><div class="span6">',
        '#suffix' => '</div>',
    );

    $customerconfirmemaildefaultvalue = isset($values['customerconfirmemail']) ? $values['customerconfirmemail'] : '';
    $form['mailing_address_details']['customerconfirmemail'] = array(
        '#id' => 'customerconfirmemail',
        '#type' => 'textfield',
        '#title' => t(''),
        '#default_value' => $customerconfirmemaildefaultvalue,
        '#size' => 60,
        '#maxlength' => 50,
        '#attributes' => array('placeholder' => t('*Confirm Email'), 'class' => array('inputfiledStyle inputfieldMedium')),
        '#prefix' => '<div class="span6">',
        '#suffix' => '</div></div>',
    );

    $form['back'] = array(
        '#type' => 'submit',
        '#value' => t('Back'),
        '#attributes' => array('class' => array('wcbooknowbtn')),
        '#prefix' => '<div class="row-fluid marginTop"><div class="span12"><div class="wcbooknowbtnWrap">',
        '#suffix' => '</div>',
    );

    $form['next'] = array(
        '#type' => 'submit',
        '#value' => t('Save & Continue'),
        '#attributes' => array('class' => array('wcbooknowbtn')),
        '#prefix' => '<div class="wcbooknowbtnWrap btnleftmargin">',
        '#suffix' => '</div></div></div>',
    );

    $form['giftcardmailingaddresstartend'] = array(
        '#type' => 'markup',
        '#markup' => '</div>',
    );

    return $form;
}




// 3 Form for the credit_card_details step.
function giftcard_credit_card_details_form($form, &$form_state) {

	$form['#attributes']['autocomplete'] = 'off';
    $form['giftcardcreditcarddetailsstartdiv'] = array(
        '#type' => 'markup',
        '#markup' => '<div id="purchaseForm">',
    );


    $form['giftcardcreditcardheader'] = array(
        '#type' => 'markup',
        '#markup' => '<h3>3. Card Details </h3>',
        '#prefix' => '<div class="row-fluid"><div class="span12">',
        '#suffix' => '</div></div>',
    );



    $values = isset($form_state['multistep_values']['credit_card_details']) ? $form_state['multistep_values']['credit_card_details'] : array();

    $creditcardtype = array('chooseone' => 'Card Type', 'visa' => 'Visa', 'mastercard' => 'Master Card', 'discover' => 'Discover', 'americanexpress' => 'American Express');
    $creditcardtypedefaultvalue = isset($values['creditcardtype']) ? $values['creditcardtype'] : '';
    $form['credit_card_details']['creditcardtype'] = array(
        '#id' => 'creditcardtype',
        '#type' => 'select',
        '#title' => t(''),
        '#options' => $creditcardtype,
        //'#default_value' => $creditcardtypedefaultvalue,
        '#description' => t(''),
        '#attributes' => array('placeholder' => t('*Card Type'), 'class' => array('selectFieldStyle selectFieldMedium')),
        '#prefix' => '<div class="row-fluid"> <div class="span12">',
        '#suffix' => '</div></div>',
    );

    $creditcardnumberdefaultvalue = isset($values['creditcardnumber']) ? $values['creditcardnumber'] : '';
    $form['credit_card_details']['creditcardnumber'] = array(
        '#id' => 'creditcardnumber',
        '#type' => 'textfield',
        '#title' => t(''),
        //'#default_value' => $creditcardnumberdefaultvalue,
        '#size' => 60,
        '#maxlength' => 20,
        '#attributes' => array('placeholder' => t('*Card Number'), 'class' => array('inputfiledStyle inputfieldMedium'),'autocomplete' => 'off'),
        '#prefix' => '<div class="row-fluid"> <div class="span6">',
        '#suffix' => '</div>',
    );

    $creditcardcvvdefaultvalue = isset($values['creditcardcvv']) ? $values['creditcardcvv'] : '';
    $form['credit_card_details']['creditcardcvv'] = array(
        '#id' => 'creditcardcvv',
        '#type' => 'textfield',
        '#title' => t(''),
        //'#default_value' => $creditcardcvvdefaultvalue,
        '#size' => 60,
        '#maxlength' => 4,
        '#attributes' => array('placeholder' => t('*CID'), 'class' => array('inputfiledStyle inputfieldMedium'),'autocomplete' => 'off'),
        '#prefix' => '<div class="span6">',
        '#suffix' => '</div></div>',
    );

    $cardexpmonthdefaultvalue = isset($values['cardexpmonth']) ? $values['cardexpmonth'] : '';
    $form['credit_card_details']['cardexpmonth'] = array(
        '#type' => 'select',
        '#title' => t(''),
        '#options' => array(1 => t('01 - January'), 2 => t('02 - February'),
            3 => t('03 - March'), 4 => t('04 - April'),
            5 => t('05 - May'), 6 => t('06 - June'),
            7 => t('07 - July'), 8 => t('08 - August'),
            9 => t('09 - September'), 10 => t('10 - October'),
            11 => t('11 - November'), 12 => t('12 - December')
        ),
        //'#default_value' => $cardexpmonthdefaultvalue,
        '#attributes' => array('class' => array('selectFieldStyle selectFieldMedium')),
        '#empty_option' => t('*Month'),
        '#prefix' => '<div class="row-fluid"> <div class="span6">',
        '#suffix' => '</div>',
    );


    $min =intval(date('Y'));
    $max = intval(date('Y')) + 20;


    $cardexpyeardefaultvalue = isset($values['cardexpyear']) ? $values['cardexpyear'] : '';
    $form['credit_card_details']['cardexpyear'] = array(
        '#type' => 'select',
        '#title' => t(''),
        '#options' => drupal_map_assoc(range($min, $max)),
        '#attributes' => array('placeholder' => t('ExpYear'), 'class' => array('selectFieldStyle selectFieldMedium')),
        //'#default_value' => $cardexpyeardefaultvalue,
        '#empty_option' => t('*Year'),
        '#prefix' => '<div class="span6">',
        '#suffix' => '</div></div>',
    );

    $knownbyvalues = array(
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
    );
    $knownbydefaultvalue = isset($values['knownby']) ? $values['knownby'] : '';
    $form['credit_card_details']['knownby'] = array(
        '#id' => 'giftcardqty',
        '#type' => 'select',
        '#title' => t(''),
        '#options' => $knownbyvalues,
        '#default_value' => $knownbydefaultvalue,
        '#description' => t(''),
        '#attributes' => array('class' => array('selectFieldStyle selectFieldMedium groupmarginTop')),
        '#prefix' => '<div class="row-fluid"> <div class="span12">',
        '#suffix' => '</div></div>',
    );

    $receiveemailoptiondefaultvalue = isset($values['receiveemailoption']) ? $values['receiveemailoption'] : '';
    $form['credit_card_details']['receiveemailoption'] = array(
        '#id' => 'receiveemailoption',
        '#type' => 'checkbox',
        '#title' => t('<span class="paymentLable">I would like to recieve Waterways special promotions and discounts by email </span>'),
        '#default_value' => isset($receiveemailoptiondefaultvalue)?$receiveemailoptiondefaultvalue:"",
        '#attributes' => array('checked' => TRUE),
        '#prefix' => '<div class="row-fluid"> <div class="span12">',
        '#suffix' => '</div></div>',
    );

    $form['back'] = array(
        '#type' => 'submit',
        '#value' => t('Back'),
        '#attributes' => array('class' => array('wcbooknowbtn')),
        '#prefix' => '<div class="row-fluid marginTop"><div class="span12"><div class="wcbooknowbtnWrap">',
        '#suffix' => '</div>',
    );

    $form['next'] = array(
        '#type' => 'submit',
        '#value' => t('Submit'),
        '#attributes' => array('class' => array('wcbooknowbtn')),
        '#prefix' => '<div class="wcbooknowbtnWrap btnleftmargin">',
        '#suffix' => '</div></div>',
    );


    $form['giftcardcreditcarddetailsenddiv'] = array(
        '#type' => 'markup',
        '#markup' => '</div>',
    );

    return $form;
}





// 4 Form for the credit_card_details step.
function giftcard_review_details_form($form, &$form_state) {

	
	
	
	$giftcardData=$form_state['multistep_values']['gift_card_details'];
	$creditCardData=$form_state['multistep_values']['credit_card_details'];
	
	$giftcardAmt=$giftcardData['pregiftcardamount'];
      
	if(strcasecmp($giftcardData['pregiftcardamount'],"setmyown")==0){
		$giftcardAmt=$giftcardData['customgiftcardamount'];
              
	}
	
	
    $form['giftcardreviewdetailsstartdiv'] = array(
        '#type' => 'markup',
        '#markup' => '<div id="purchaseForm">',
    );

    $form['giftcardcreviewheader'] = array(
        '#type' => 'markup',
        '#markup' => '<h3>4. Review Details</h3>',
        '#prefix' => '<div class="row-fluid"><div class="span12">',
        '#suffix' => '</div></div>',
    );
    
    $form['gift_card_details']['pregiftcardamount'] = array(
        '#type' => 'markup',
        '#markup' => t('<b>Amount:</b>'),
        '#prefix' => '<div class="row-fluid"> <div class="span6">',
        '#suffix' => '</div>'
    );
    $form['gift_card_details']['pregiftcardamountvalue'] = array(
        '#type' => 'markup',
        '#markup' => '$' . ($giftcardAmt),
        '#prefix' => '<div class="span6">',
        '#suffix' => '</div></div>'
    );

    $form['gift_card_details']['recipientemail'] = array(
        '#type' => 'markup',
        '#markup' => t('<b>Recipient Email:</b>'),
        '#prefix' => '<div class="row-fluid"> <div class="span6">',
        '#suffix' => '</div>'
    );

    $form['gift_card_details']['recipientemailvalue'] = array(
        '#type' => 'markup',
        '#markup' => $giftcardData['recipientemail'],
        '#prefix' => '<div class="span6">',
        '#suffix' => '</div></div>',
    );

    $form['gift_card_details']['yourname'] = array(
        '#type' => 'markup',
        '#markup' => t('<b>Your Name:</b>'),
        '#prefix' => '<div class="row-fluid"> <div class="span6">',
        '#suffix' => '</div>',
    );

    $form['gift_card_details']['yournamevalue'] = array(
        '#type' => 'markup',
        '#markup' => $giftcardData['yourname'],
        '#prefix' => '<div class="span6">',
        '#suffix' => '</div></div>',
    );

    $form['gift_card_details']['customermessage'] = array(
        '#type' => 'markup',
        '#markup' => t('<b>Message:</b>'),
        '#prefix' => '<div class="row-fluid"> <div class="span6">',
        '#suffix' => '</div>',
    );

    $form['gift_card_details']['customermessagevalue'] = array(
        '#type' => 'markup',
        '#markup' => $giftcardData['customermessage'],
        '#prefix' => '<div class="span6">',
        '#suffix' => '</div></div>',
    );

    $form['gift_card_details']['giftcardqty'] = array(
        '#type' => 'markup',
        '#markup' => t('<b>Quantity:</b>'),
        '#prefix' => '<div class="row-fluid"> <div class="span6">',
        '#suffix' => '</div>',
    );

    $giftcardqty = array(1 => '1', 2 => '2', 3 => '3', 4 => '4');
    $form['gift_card_details']['giftcardqtyvalue'] = array(
        '#type' => 'markup',
        '#markup' => $giftcardData['giftcardqty'],
        '#prefix' => '<div class="span6">',
        '#suffix' => '</div></div>',
    );

    $form['gift_card_details'][''] = array(
        '#type' => 'markup',
        '#markup' => '<hr style="border: 1px solid #999;" />'
    );


    $form['gift_card_details']['deliverydate'] = array(
        '#type' => 'markup',
        '#markup' => t('<b>Delivery Date:</b>'),
        '#prefix' => '<div class="row-fluid"> <div class="span6">',
        '#suffix' => '</div>',
    );

    $form['gift_card_details']['deliverydatevalue'] = array(
        '#type' => 'markup',
        '#markup' => $giftcardData['deliverydate'],
        '#prefix' => '<div class="span6">',
        '#suffix' => '</div></div>',
    );

    $form['review_details']['creditcardtype'] = array(
        '#type' => 'markup',
        '#markup' => t('<b>Card Type:</b>'),
        '#prefix' => '<div class="row-fluid"> <div class="span6">',
        '#suffix' => '</div>',
    );
    $creditcardtypes = array('chooseone' => 'Card Type', 'visa' => 'Visa', 'mastercard' => 'Master Card', 'discover' => 'Discover', 'americanexpress' => 'American Express');
    $creditcardtypedefaultvalue = $creditCardData['creditcardtype'];
    $form['review_details']['creditcardtypevalue'] = array(
        '#id' => 'creditcardtype',
        '#type' => 'markup',
        '#markup' => $creditcardtypes[$creditcardtypedefaultvalue],
        '#prefix' => '<div class="span6">',
        '#suffix' => '</div></div>',
    );

    $form['review_details']['creditcardnumber'] = array(
        '#type' => 'markup',
        '#markup' => t('<b>Card Number:</b>'),
        '#prefix' => '<div class="row-fluid"> <div class="span6">',
        '#suffix' => '</div>',
    );

    $creditcardnumberdefaultvalue = $creditCardData['creditcardnumber'];
    $cardnumbermasked='';
    $cardvalue = substr($creditcardnumberdefaultvalue, -3);
    for ($i = 0; $i < strlen($creditcardnumberdefaultvalue) - 3; $i++) {
        $cardnumbermasked.='x';
    }
    $cardnumbermasked.=$cardvalue;
    $form['credit_card_details']['creditcardnumbervalue'] = array(
        '#type' => 'markup',
        '#markup' => $cardnumbermasked,
        '#prefix' => '<div class="span6">',
        '#suffix' => '</div></div>',
    );

    $form['credit_card_details']['creditcardcvv'] = array(
        '#type' => 'markup',
        '#markup' => t('<b>CID:</b>'),
        '#prefix' => '<div class="row-fluid"> <div class="span6">',
        '#suffix' => '</div>',
    );
    $cidmask='';
    $creditcardcvvdefaultvalue =$creditCardData['creditcardcvv'];
    for ($i = 0; $i < strlen($creditcardcvvdefaultvalue); $i++) {
        $cidmask.='x';
    }
    $form['credit_card_details']['creditcardcvvvalue'] = array(
        '#type' => 'markup',
        '#markup' => $cidmask,
        '#prefix' => '<div class="span6">',
        '#suffix' => '</div></div>',
    );

    $form['credit_card_details']['cardexpmonth'] = array(
        '#type' => 'markup',
        '#markup' => t('<b>Exp Month:</b>'),
        '#prefix' => '<div class="row-fluid"> <div class="span6">',
        '#suffix' => '</div>',
    );

    $form['credit_card_details']['cardexpmonthvalue'] = array(
        '#type' => 'markup',
        '#markup' => $creditCardData['cardexpmonth'],
        '#prefix' => '<div class="span6">',
        '#suffix' => '</div></div>',
    );


    $form['credit_card_details']['cardexpyear'] = array(
        '#type' => 'markup',
        '#markup' => t('<b>Exp Year:</b>'),
        '#prefix' => '<div class="row-fluid"> <div class="span6">',
        '#suffix' => '</div>',
    );

    $form['credit_card_details']['cardexpyearvalue'] = array(
        '#type' => 'markup',
        '#markup' => $creditCardData['cardexpyear'],
        '#prefix' => '<div class="span6">',
        '#suffix' => '</div></div>',
    );

    $form['back'] = array(
        '#type' => 'submit',
        '#value' => t('Back'),
        '#attributes' => array('class' => array('wcbooknowbtn')),
        '#prefix' => '<div class="row-fluid marginTop"><div class="span12"><div class="wcbooknowbtnWrap">',
        '#suffix' => '</div>',
    );

    $form['next'] = array(
    	'#id'=>'finishorder',
        '#type' => 'submit',
        '#value' => t('Finish Order'),
        '#attributes' => array('class' => array('wcbooknowbtn')),
        '#prefix' => '<div id="finishorderdiv" class="wcbooknowbtnWrap btnleftmargin">',
        '#suffix' => '</div>',
    );
    
    
    $form['finishorderprocessing'] = array(
    		'#id'=>'finishorderprocessing',
    		'#type' => 'submit',
    		'#value' => t('Processing...'),
    		'#attributes' => array('class' => array('wcbooknowbtn'),'disabled'=>'disabled'),
    		'#prefix' => '<div id="finishorderprocessingdiv" style="display:none;" class="wcbooknowbtnWrap btnleftmargin">',
    		'#suffix' => '</div></div>',
    );


    $form['giftcardreviewdetailsenddiv'] = array(
        '#type' => 'markup',
        '#markup' => '</div">',
    );

    return $form;
}




//final step

