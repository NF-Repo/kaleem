<?php

function giftcard_get_header($form, &$form_state) {

    $form_state['stage'] = isset($form_state['stage']) ? $form_state['stage'] : 1;
    
    $form_stages = array(
      'gift_card_details' => 1,
      'mailing_address_details' => 2,
      'credit_card_details' => 3,
      'review_details' => 4,
      'giftcard_confirmation' => 5,
    ); 

    if (isset($form_stages[$form_state['stage']])) {
     $current_step = $form_stages[$form_state['stage']]; 
    }
    else {
     $current_step = 1;
    }

  $menulist = getConnectMenu('GIFT CARDS');
    $menu = $menulist['connectmenu'];
    $mobilemenu = $menulist['mobilemenu'];
    $form['header'] = array(
       '#type' => 'markup',
       '#prefix' => '<div class="container content-inner"><div class="row-fluid"><div class="span12">',
       '#suffix' => '</div></div>'
    );

    $form['header']['submenu'] = array(
        '#type' => 'markup',
        '#markup' => '<div class="row-fluid subMenuWrapper">
              <div class="span12">
                  <div class="connectTitle"> CONNECT </div>

                  <div class="subMenu responsiveWeb">
                      <ul>
                          '.$menu.'
                      </ul>
                  </div>

                  <div class="subMenu responsiveMobile">

                      <select>
                          '.$mobilemenu.'
                      </select>

                  </div>
              </div>
          </div>'
    );

    
    $form['header']['giftcardtitle'] = array(
        '#type' => 'markup',
        '#markup' => '<div class="maincontent-inner"><h3 class="field-title">Check Balance</h3><div>If you already have a Gift Card you can check its current balance below.</div>'
    );
    
    $form['header']['giftcardcheckbalance'] = array(
        '#type' => 'textfield',
        '#title' => t('Check Balance'),
        '#title_display' => 'invisible',
        '#attributes' => array('class' => array('inputfiledStyle pull-left span12')),
        '#prefix' => '<div class="row-fluid" style="margin-top: 2%;"><div class="span6">',
        '#suffix' => '</div>'
    );
    
    $form['header']['giftcardcheckbalancebtn'] = array(
        '#type' => 'button',
        '#value' => t('Apply'),
        '#limit_validation_errors' => array(),
        '#ajax' => array(
            'callback' => 'checkgiftcardbalance',
        ),
        '#attributes' => array('class' => array('wcbooknowbtn')),
        '#prefix' => '<div class="span6" style="margin-left: 0px;"><div class="wcbooknowbtnWrap btnleftmargin">',
        '#suffix' => '</div></div></div>',
            //'#weight'=>1,
    );
    
    $form['header']['giftcardmessageWrapper'] = array(
        '#type' => 'markup',
        '#prefix' => '<div id="giftcard_status_message">',
        '#suffix' => '</div>',
    );
    
    $form['header']['hrwrapper'] = array(
        '#type' => 'markup',
        '#markup' => '<div style="margin: 26px 0px;"><hr style="border: 1px solid #999;" /></div></div>'
    );
    
    
    $form['header']['purchasegiftcardtitle'] = array(
        '#type' => 'markup',
        '#markup' => '<div class="row-fluid">
                          <div class="span12"> 
                              <h3 class="field-title">Purchase a Gift Card</h3>
                          </div>
                      </div>'
    );
    
    
    $stages = array(
    1 => array('data' => '1'),
    2 => array('data' => '2'),
    3 => array('data' => '3'),
    4 => array('data' => '4'),
    5 => array('data' => '5'),
  );
  
  $stages[$current_step]['class'] = array('active');
  
  $stages_list = theme('item_list', array('items' => $stages));
  
  /*
   * content-inner div closed in subnav from element.
   */
  $form['header']['subNav'] = array(
    '#type' => 'fieldset',
    '#title' => '',
    '#value' => $stages_list,
    '#attributes' => array('class' => array('subNav')),
    '#prefix' => '<div class="row-fluid"><div class="span12"><div class="menuRelative">',
    '#suffix' => '</div></div></div>'
  );
    
    return $form;
}

/**
 * Given the current stage the user is on, calculate what the next step would be
 * 
 * @param type $form_state
 * @return string 
 */
function giftcard_move_to_next_stage($form, &$form_state) {
drupal_add_css(drupal_get_path('module', 'giftcard') . '/css/giftcard.css');
    switch ($form_state['stage']) {
        case 'gift_card_details':
            return 'mailing_address_details';
            break;

        case 'mailing_address_details':
            return 'credit_card_details';
            break;

        case 'credit_card_details';
            return 'review_details';
            break;

        case 'review_details';
            return 'giftcard_confirmation';
            break;
    }
}

/**
 * Given the current stage the user is on, calculate what the previous step
 * would be
 * 
 * @param type $form_state
 * @return string 
 */
function giftcard_move_to_previous_stage($form, &$form_state) {
	drupal_add_css(drupal_get_path('module', 'giftcard') . '/css/giftcard.css');
    switch ($form_state['stage']) {
        case 'mailing_address_details':
            return 'gift_card_details';
            break;

        case 'credit_card_details':
            return 'mailing_address_details';
            break;
        case 'review_details':
            return 'credit_card_details';
            break;
    }
}



function checkgiftcardbalance($form, $form_state){
	drupal_add_css(drupal_get_path('module', 'giftcard') . '/css/giftcard.css');
	$commands = array();
	$cardNumber = $form_state['values']['giftcardcheckbalance'];
	$requestType = 'Inquiry';
	$entryType = 'K';
	$giftcardbalancecheckers =giftcard_profitpointapi_balancechecker($requestType, $entryType, $cardNumber);
	$giftcardstatus = 'This Gift Card code is invalid or has expired.';
	if ($giftcardbalancecheckers['message'] == 'sucess') {
		$giftcardamount = $giftcardbalancecheckers['results']['amount'];
		$giftcardstatus = "Card Balance : $".$giftcardamount;
	} else {
	}
	$commands[] = ajax_command_replace("#giftcard_status_message", "<div id='giftcard_status_message'>" . $giftcardstatus . "</div>");
	return array('#type' => 'ajax', '#commands' => $commands);
}

