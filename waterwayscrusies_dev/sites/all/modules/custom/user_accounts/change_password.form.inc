<?php

function useraccount_changepassword_page(){
	drupal_set_title('Reset Your Password');
	return drupal_get_form('useraccount_changepassword_form');
}






function useraccount_changepassword_form($form, &$form_state) {
	
	$form['pass'] = array(
		'#type' => 'password_confirm',
		'#size' => 25,
		'#description' => t('Provide a password for the new account in both fields.'),
		'#required' => TRUE,
	);
	

	$form ['submit'] = array (
			'#value' => t ( 'Change Password' ),
			'#type' => 'submit',
		 
	);
	
	$form ['cancel'] = array (
			'#type' => 'submit',
			'#value' => t ( 'Cancel' ),
			'#limit_validation_errors' => array () 
	);
	
	return $form;
}

function useraccount_changepassword_form_submit($form, &$form_state) {
	
	global $user;
	$accountObject=user_load($user->uid);
	if($accountObject){
		$account=array();
		foreach($accountObject as $key=>$values){
			if($key=="pass"){
				$account['pass']=$form_state ['values'] ['pass'];
			}else {
				$account[$key]=$values;
			}
		
		}
		$updatedUserAccount = user_save((object) array('uid' => $user->uid), $account);
		
		if($updatedUserAccount){
			drupal_set_message('Password updated successfully.');
		}else{
			drupal_set_message('Password updation failed.');
		}
	}else{
		drupal_set_message('Invalid Operation.');
	}
	
	
}

