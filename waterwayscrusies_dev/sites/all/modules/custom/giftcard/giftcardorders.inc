<?php
function giftcardorders_page(){
	
	
$header = array (
			array ('data' => 'OrderId',
			),
			array ('data' => 'Customer',
			),
			array ('data' => 'Email',
			),
			array ('data' => 'Phone' 
			),
			array ('data' => 'Purchase date' 
			),
			array ('data' => 'Total Amount' 
			),
			array ('data' => 'Status' 
			),
			array ('data' => 'Action' 
			) 
	);

	

	$sort = 'DESC';

	
	$query = db_select('giftcard_orders', '');
	$query->innerJoin('uc_orders', 'u', 'u.order_id = giftcard_orders.order_id');
	$query->fields('giftcard_orders', array('data', 'order_id'));
	$query->fields('u');
	$query->orderBy('u.order_id', 'DESC');
        $query->condition('u.order_status', array('payment_received','completed'),'IN');
	$query = $query->extend('TableSort')->extend('PagerDefault')->limit(50);
	$result = $query->execute()->fetchAll();
	
	///drupal_json_output($result); exit();
	
	
	
	global $base_url;
	$rows = array();
	if (count($result) > 0) {
		for ($i = 0; $i < count($result); $i++) {
			$orderstatus = str_replace('_', ' ', $result [$i]->order_status);
			$rows[] = array(
					$result[$i]->order_id,
					$result [$i]->delivery_last_name . ' ' . $result [$i]->delivery_first_name,
					$result [$i]->primary_email,
                $result [$i]->delivery_phone,
                date('m/d/Y', $result [$i]->created),
                uc_currency_format($result [$i]->order_total),
                ucwords($orderstatus),
				'<a href="' . $base_url . '/giftcardorder/' . $result[$i]->order_id . '/view">View </a>'
			);
		}
	}
	
	$output = '<div class="">';
	$output .= '<div class="">';
	
	$output .= '</div>';
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
	
	$output .= '</div>';
	
	$form['mkp'] = array(
			'#type' => 'markup',
			'#markup' => $output,
	);
	
	return $form;
	
	
	
	
	drupal_json_output($result);
	exit();
	return "hello";
}