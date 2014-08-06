<?php



function history($order_id){
	return drupal_get_form("booking_order_history", $order_id);
}



function booking_order_history($form, &$form_state,$order_id){


	
	$form['orderheader']=array(
			'#type'=>'markup',
			'#markup'=>'<div class="row-fluid"><nav class="span12"><ul style="display:block;" class="breadcrumb">
			<li><a href="'.base_path().'staffadmin/order/'.$order_id.'/info">Order Info</a></li> <span class="icon-chevron-right"></span>
			<li ><a href="'.base_path().'staffadmin/order/'.$order_id.'/updateorder">Cart</a></li> <span class="icon-chevron-right"></span>
			<li><a href="'.base_path().'staffadmin/order/'.$order_id.'/payment">Payment</a></li> <span class="icon-chevron-right"></span>
			<li class="active"><h4>History</h4></li>
			</nav></div>',
	);
	
	

//Order History

$form['orderhistory'] = array(
		'#type' => 'fieldset',
		'#id' => 'historytab',
		'#title' => '<span class="icon-calendar"></span> &nbsp;'.t('History'),
// 		'#collapsible' => TRUE,
// 		'#group' => 'orderedit'
);


$query = db_select('uc_order_admin_comments', 'u');
$query->fields('u');
$query->condition('message', '%stock%', 'NOT LIKE');
$query->condition('order_id', $order_id);
$query->orderBy('u.created');
$query->orderBy('u.comment_id');
$comments = $query->execute()->fetchAll();


// $comments = uc_order_comments_load($order_id, TRUE);
$commenttabledata = tapir_get_table('uc_op_admin_comments_view_table', $comments);

$form['orderhistory']['logmessage'] = array(
		'#type' => 'markup',
		'#markup' => drupal_render($commenttabledata),
);
return $form;
}