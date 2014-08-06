<?php
function sitecontactmailpage(){
	
	return drupal_get_form("sitecontactmail_form");
}


function sitecontactmail_form($form,$form_state){
	
	
	$form=array();
	$form['contactEamil']=array(
		'#type'=>'textfield',
		'#title'=>'Contact US',
		//'#description'=>"The To address used for receiving Contact Us",
		'#required'=>TRUE,
		'#default_value'=>variable_get('CONTACTUSEMAIL','contact@waterwayscruises.com'),
		'#attributes'=>array('class'=>array('input-xxlarge'))
		
		
	);
	
	$form['contactRpf']=array(
		'#type'=>'textfield',
		'#title'=>'Request A Proposal',
		//'#description'=>"The To address used for receiving Request A Proposal",
		'#required'=>TRUE,
		'#default_value'=>variable_get('REQUESTAPROPOSALEMAIL','contact@waterwayscruises.com'),
		'#attributes'=>array('class'=>array('input-xxlarge'))
		
		
	);
        $form['contactFeedback']=array(
		'#type'=>'textfield',
		'#title'=>'Feedback',
		//'#description'=>"The To address used for receiving Feedback",
		'#required'=>TRUE,
		'#default_value'=>variable_get('FEEDBACKEMAIL','contact@waterwayscruises.com'),
        '#attributes'=>array('class'=>array('input-xxlarge'))
		
		
	);
        $form['contactCorporate']=array(
		'#type'=>'textfield',
		'#title'=>'Corporate Giving',
		//'#description'=>"The To address used for receiving Corporate Giving",
		'#required'=>TRUE,
		'#default_value'=>variable_get('CORPORATEGIVINGEMAIL','contact@waterwayscruises.com'),
        '#attributes'=>array('class'=>array('input-xxlarge'))
		
		
	);
        
	$form['confirmationEmail']=array(
		'#type'=>'textfield',
		'#title'=>'Orders Confirmation Email',
		//'#description'=>"The From address in automated e-mails sent during Order Booking notifications.)",
		'#required'=>TRUE,
		'#default_value'=>variable_get('CONFIRMATIONEMAIL','websales@waterwayscruises.com'),
		'#attributes'=>array('class'=>array('input-xxlarge'))
	);
	
	
	$form['submit']=array(
			'#type'=>'submit',
			'#value'=>'Save',
	);
	
	return $form;
	
	
	
	
}


function sitecontactmail_form_submit($form,$form_state){
    
 
	$site= new SiteConfiguration();
	$site->setContactUs(trim($form_state['values']['contactEamil']));
        $site->setRfp(trim($form_state['values']['contactRpf']));
        $site->setFeedback(trim($form_state['values']['contactFeedback']));
	$site->setConfirmationEmail($form_state['values']['confirmationEmail']);
        $site->setCorporatGiving($form_state['values']['contactCorporate']);
        
	drupal_set_message("Configuration Saved Successfully");
}