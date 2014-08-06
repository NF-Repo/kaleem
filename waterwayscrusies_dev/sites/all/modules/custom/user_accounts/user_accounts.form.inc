<?php

function useraccount_edit_page($uid){
	
	return drupal_get_form('useraccount_form',$uid);
	
}


function useraccount_delete_page($uid){
	
	$path='useraccounts';
	$msg='';
	
	
	
	$result=UserAccount::deleteUser($uid);
	
	if($result){
	$msg="Record Deleted Successfully";	
	}else{
		$msg='Operation Failed';
	}
	
	drupal_set_message ( $msg);
	drupal_goto($path);
	
	
}

function useraccount_form($form, &$form_state,$uid) {

	$userObject=UserAccount::getUserDetails($uid);
	$userid='';
	$currentOperation='add';
	if($userObject->uid!=''){
		$userid=$userObject->uid;
		$currentOperation='edit';
	}
	
	
	$user_name='';
	$first_name='';
	$last_name='';
	$user_role='';
	$mail='';
	$company_name='';
	$phone='';
	$address='';
	$rid='';
	$status=0;
	
	if($userObject){
		
		$user_name=$userObject->name;
		$first_name=$userObject->first_name;
		$last_name=$userObject->last_name;
		$user_role=$userObject->name;
		$mail=$userObject->mail;
		$company_name=$userObject->company_name;
		$phone=$userObject->phone;
		$address=$userObject->address;
		$rid=$userObject->rid;
		$status=$userObject->status;
		
		
		
	}
	
	
	
	
	
	$roles = user_roles ();
	$selectedRoles = array ();
	foreach ( $roles as $key => $value ) {
		// echo $value;
		if ($value != 'anonymous user' && $value != 'authenticated user' && $value != 'administrator') {
			$selectedRoles [$key] = ucwords ( $value );
		}
	}
	
	$form = array ();
	
	// User ID
	$form ['userid'] = array (
			'#type' => 'hidden',
			'#value' =>  $userid
	);
	
	
	$form ['currentoperation'] = array (
			'#type' => 'hidden',
			'#value' => $currentOperation
	);
	
	// User Name
	$form ['user_name'] = array (
			'#type' => 'textfield',
			'#title' => t ( 'User Name' ),
			'#default_value' => $user_name,
			'#size' => 60,
			'#maxlength' => 60,
			'#required' => TRUE,
			'#description' => t ( 'Spaces are allowed; punctuation is not allowed except for periods, hyphens, apostrophes, and underscores.' ),
// 			'#ajax' => array (
// 					'callback' => 'ajax_check_user_existence_callback',
// 					//'event'=>'change' 
// 			),
			
			'#attributes' => ($currentOperation=="edit") ? array('disabled' => 'disabled') : '',
	);
	
	if($currentOperation=="edit"){
		$form['user_status'] = array(
				'#type' => 'checkbox',
				'#title' => t('Active'),
				'#default_value' => $status
		);
		
		$form['pass'] = array(
				'#type' => 'password_confirm',
				'#size' => 25,
				'#description' => t('Provide a password for the account in both fields.'),
				'#required' => FALSE,
		);
		
	}
	
	
	
	
	
	
	
	// This entire form element will be replaced with an updated value.
	$form ['username_status'] = array (
			'#type' => 'markup',
			'#markup' => '<div id="usernamestatusdiv"></div>' 
	);
	
	// First Name
	$form ['first_name'] = array (
			'#type' => 'textfield',
			'#title' => t ( 'First Name' ),
			'#default_value' => $first_name,
			'#size' => 60,
			'#maxlength' => 60 
	// '#required' => TRUE,
		);
	
	// Last Name
	$form ['last_name'] = array (
			'#type' => 'textfield',
			'#title' => t ( 'Last Name' ),
			'#default_value' => $last_name,
			'#size' => 60,
			'#maxlength' => 60 
	// '#required' => TRUE,
		);
	
	// Role
	$form ['user_role'] = array (
			
			'#type' => 'select',
			'#title' => t ( 'Role' ),
			'#options' => $selectedRoles,
			//'#value' => $rid,
			// '#description' => t(''),
			'#required' => TRUE ,
			'#default_value' =>$rid
	);
	
	// Email Address
	$form ['mail'] = array (
			'#type' => 'textfield',
			'#title' => t ( 'Email Address' ),
			'#maxlength' => 254,
			'#size' => 60,
			'#default_value'=>$mail,
			'#required' => TRUE,
// 			'#ajax' => array (
// 					'callback' => 'ajax_check_user_email_existence_callback',
// 			),
			'#attributes' => ($currentOperation=="edit") ? array('disabled' => 'disabled') : '',
			 
			
	);
	
	// This entire form element will be replaced with an updated value.
	$form ['useremail_status'] = array (
			'#type' => 'markup',
			'#markup' => '<div id="useremailstatusdiv"></div>'
	);
	
	// Company Name
	$form ['company_name'] = array (
			'#type' => 'textfield',
			'#title' => t ( 'Company Name' ),
			'#default_value' => $company_name,
			'#size' => 60,
			'#maxlength' => 60 
	// '#required' => TRUE,
		);
	
	// Phone
	$form ['phone'] = array (
			'#type' => 'textfield',
			'#title' => t ( 'Phone' ),
			'#default_value' => $phone,
			'#size' => 60,
			'#maxlength' => 20 
	// '#required' => TRUE,
		);
	
	// Address
	$form ['address'] = array (
			'#title' => t ( 'Address' ),
			'#type' => 'textarea',
			'#cols' => 60,
			'#rows' => 5, 
	// '#description' => t (''),
			'#default_value' => $address,
		);
	
	
	
	
	
	
	$actionBtnName=($currentOperation=="edit") ? 'Update User Details' : 'Add New User';
	$form ['submit'] = array (
			'#type' => 'submit',
			'#value' => $actionBtnName,
			'#submit' => array (
					'user_accounts_frm_submit' 
			) 
	);
	
	$form ['cancel'] = array (
			'#type' => 'submit',
			'#value' => t ( 'Cancel' ),
			'#submit' => array ('user_accounts_frm_cancel' 
			),
			'#limit_validation_errors' => array () 
	);
	
	return $form;
}


function user_accounts_frm_cancel($form, &$form_state) {
	$url = 'useraccounts';
	drupal_goto($url);
}


function useraccount_form_validate($form, &$form_state) {
	
	

	//Validate the Fields iff operation is Add (Create New User)
	
	//if($form_state['values']['user_name']=='add'){
		if ($form_state['triggering_element']['#value'] == 'Add New User') {
			//Username
			if (isset($form_state['values']['user_name'])) {
				if ($error = user_validate_name($form_state['values']['user_name'])) {
					form_set_error('user_name', $error);
				}
				elseif ((bool) db_select('users')->fields('users', array('uid'))->condition('name', db_like($form_state['values']['user_name']), 'LIKE')->range(0, 1)->execute()->fetchField()) {
					form_set_error('user_name', t('The name %name is already taken.', array('%name' => $form_state['values']['user_name'])));
				}
			}
		
		
			//Mail Validation
			$mail = trim($form_state['values']['mail']);
			// Validate the e-mail address, and check if it is taken by an existing user.
			if ($error = user_validate_mail($form_state['values']['mail'])) {
				form_set_error('mail', $error);
			}
			elseif ((bool) db_select('users')->fields('users', array('uid'))->condition('mail', db_like($form_state['values']['mail']), 'LIKE')->range(0, 1)->execute()->fetchField()) {
				form_set_error('mail', t('The e-mail address %email is already taken.', array('%email' => $form_state['values']['mail'])));
			}
		}
//	}//if
	
	
	
	
}


function user_accounts_frm_submit($form, &$form_state) {
	
	
	
	
	$userstatus=isset($form_state ['values'] ['user_status']) ? $form_state ['values'] ['user_status'] : 1; 
	
	$userObject = new UserAccount ();
	$userObject->uid = $form_state ['values'] ['userid'];
	$userObject->user_name = $form_state ['values'] ['user_name'];
	$userObject->status=$userstatus;
	
	//pass
	if(!empty( $form_state ['values'] ['pass']) && isset( $form_state ['values'] ['pass'])){
		$userObject->password=$form_state ['values'] ['pass'];
	}
	//$userObject->password = "password";
	$userObject->firstname = $form_state ['values'] ['first_name'];
	$userObject->last_name = $form_state ['values'] ['last_name'];
	$userObject->mail = $form_state ['values'] ['mail'];
	$userObject->role_id = $form_state ['values'] ['user_role'];
	$userObject->company_name = $form_state ['values'] ['company_name'];
	$userObject->phone = $form_state ['values'] ['phone'];
	$userObject->address = $form_state ['values'] ['address'];
	
	
	if($form_state ['values'] ['currentoperation']=="edit"){
		$result = $userObject->updateUser();
	}else if($form_state ['values'] ['currentoperation']=="add"){
		$userObject->password = user_password ( 6 );
		$result = $userObject->createNewUser ();
		$params = array(
				'subject' => t('Waterways Login Information'),
				'body' => t("Data"),
				'user' => $userObject->user_name,
				'pass' => $userObject->password
		);
		$to = $userObject->mail;
                $from=SiteConfiguration::getConfirmationEmail();
		drupal_mail("email_system", "wwc_useraccountemail", $to, language_default(), $params, $from);
		
		
	}
	
	
	
	$path='useraccounts';
	$msg='';
	// Result : True or False
	if ($result) {
		if($form_state ['values'] ['currentoperation']=="add"){
			$msg='User Record Added Successfully';
		}
		if($form_state ['values'] ['currentoperation']=="edit"){
			$msg='User Record Updated Successfully';
		}
	} else {
		//$msg='Operation Failed';
		//drupal_set_message ( 'Failed' );
	}
	
	drupal_set_message ( $msg);
	drupal_goto($path);
	
	
}







//User Name Validation
function ajax_check_user_existence_callback($form, &$form_state) {
	$commands = array ();
	$name = $form_state ['values'] ['user_name'];

	// Validation
	if (isset ( $name )) {
		if ($error = user_validate_name ( $name )) {
				
			$data = $error;
		} elseif (( bool ) db_select ( 'users' )->fields ( 'users', array (	'uid'	) )->condition ( 'name', db_like ( $name ), 'LIKE' )->range ( 0, 1 )->execute ()->fetchField ()) {
			$data = 'The name ' . $name . ' is already taken!.';
			$commands [] = ajax_command_invoke('#edit-user-name', 'attr', array('value', ''));
		}
	}
	$commands [] = ajax_command_replace ( "#usernamestatusdiv", "<div id='usernamestatusdiv'><span class='error'>" . $data . "</span></div>" );
	return array (
			'#type' => 'ajax',
			'#commands' => $commands
	);
}



//Email
function ajax_check_user_email_existence_callback($form, &$form_state) {
	$commands = array ();
	$mail = $form_state ['values'] ['mail'];

	// Validate the e-mail address, and check if it is taken by an existing user.
	if ($error = user_validate_mail($mail)) {
		$data = $error;
	}
	elseif ((bool) db_select('users')->fields('users', array('uid'))->condition('mail', db_like($form_state['values']['mail']), 'LIKE')->range(0, 1)->execute()->fetchField()) {
		$data = 'The e-mail address ' . $mail . ' is already taken!.';
		$commands [] = ajax_command_invoke('#edit-mail', 'attr', array('value', ''));

	}
	$commands [] = ajax_command_replace ( "#useremailstatusdiv", "<div id='useremailstatusdiv'><span class='error'>" . $data . "</span></div>" );
	return array (
			'#type' => 'ajax',
			'#commands' => $commands
	);
}






