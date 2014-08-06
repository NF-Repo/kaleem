<?php
function emailtemplates_notificaton_status_page(){
return drupal_get_form('emailtemplates_notificaton_status_form');
}


function emailtemplates_notificaton_status_form($form, &$form_state) {
	$output = '';
	$header = array(
			array('data' => 'Cruise Title', 'field' => 'cruise_title'),
			array('data' => 'Scheduled Date', 'field' => 'schedule_event_date'),
			array('data' => 'Order ID', 'field' => 'order_id'),
			array('data' => 'Customer ID', 'field' => 'user_id'),
			array('data' => 'Customer Email', 'field' => 'mail'),
			array('data' => 'Sent', 'field' => 'sent_date'),
			array('data' => 'Status', 'field' => 'status'),
	);


	//var_dump($_GET['order']);
	if (isset($_GET['sort']) && isset($_GET['order'])) {
		if ($_GET['sort'] == 'asc')
			$sort = 'ASC';
		else
			$sort = 'DESC';

		switch ($_GET['order']) {
			case 'Cruise Title':
				$order = 'cruise_title';
				break;
			case 'Scheduled Date':
				$order = 'schedule_event_date';
				break;
			case 'Order ID' :
				$order = 'order_id';
				break;
			case 'Customer ID' :
				$order = 'user_id';
				break;

			case 'Customer Email' :
				$order = 'mail';
				break;

			case 'Sent' :
				$order = 'sent_date';
				break;

			case 'Status' :
				$order = 'status';
				break;

		}
	} else {
		$sort = 'ASC';
		$order = 'uid';
	}


	
	$query = db_select('emailtemplate_notifications_sent', '');
	$query->fields('emailtemplate_notification', array('user_id', 'cruise_template_id', 'schedule_event_id', 'schedule_event_date', 'order_id'));
	$query->fields('emailtemplate_notifications_sent', array('status', 'sent_date'));
	$query->fields('cruise', array('cruise_title'));
	$query->fields('users', array('mail'));
	$query->innerJoin('emailtemplate_notification', '', 'emailtemplate_notifications_sent.en_id = emailtemplate_notification.id');
	$query->innerJoin('cruise', '', 'emailtemplate_notification.cruise_template_id = cruise.id');
	$query->innerJoin('users', '', 'emailtemplate_notification.user_id = users.uid');
	//$result = $query->execute()->fetchAll();
	
	$query->orderBy($order, $sort);
	$query = $query->extend('TableSort')->extend('PagerDefault')->limit(20);
	$result = $query->execute()->fetchAll();
	
	//var_dump($result); exit();

	$rows = array();
	if (count($result) > 0) {
		for ($i = 0; $i < count($result); $i++) {
			$rows[] = array(
					$result[$i]->cruise_title,
					date('d/m/Y h:i a',$result[$i]->schedule_event_date),
					$result[$i]->order_id,
					$result[$i]->user_id,
					$result[$i]->mail,
					date('d/m/Y h:i a',$result[$i]->sent_date),
					ucwords($result[$i]->status),
				
			);
		}
	}

	$output .= theme_table(
			array(
					'header' => $header,
					'rows' => $rows,
					'attributes' => array('class' => array('')),
					'sticky' => true,
					'caption' => '',
					'colgroups' => array(),
					'empty' => t("No Records!"),
			)
	) . theme('pager');

	

	$form['mkp'] = array(
			'#type' => 'markup',
			'#markup' => $output,
	);

	return $form;
}