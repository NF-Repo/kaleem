<?php
function cruise_templates_page() {
	$build['cruisetemplatesfilterform'] = drupal_get_form('cruisetemplates_filteration_form');
	$build['cruisetemplateslistform'] = drupal_get_form('view_cruise_templates_form');
	return $build;
}


function cruise_list_form_validate($form, &$form_state) {
	if ($form_state['triggering_element']['#value'] == 'Go') {
		$form_state['rebuild'] = TRUE;
		return;
	}
}





function cruisetemplates_filteration_form() {
	drupal_add_css(drupal_get_path('module', 'cruise') . '/cruise.css');
	
	$form['addcruisetemplate']=array(
			'#type'=>'markup',
			'#markup'=>'<a href="' . base_path() . 'cruise/add">Add Cruise Template</a>',
	);
	
	$form['option'] = array(
			'#type' => 'select',
			'#options' => drupal_map_assoc(array('All', 'Active', 'Inactive')),
			'#default_value' => isset($_SESSION['option']) ? $_SESSION['option'] : '',
			'#prefix' => '<div class="row-fluid"><div class="span2">',
			'#suffix' => '</div>',
			'#attributes' => array('class' => array('input-medium')),
	);
	
	$form['btn'] = array(
			'#type' => 'submit',
			'#value' => 'Go',
			'#prefix' => '<div class="span10" style="margin-left: 0px;margin-top:13px;">',
			'#suffix' => '</div></div>',
	);

	return $form;
}

function cruisetemplates_filteration_form_submit($form, &$form_state) {
	if (isset($form_state['values']['option'])) {
		$_SESSION ['option'] = $form_state['values']['option'];
	}
	
	$form_state['redirect'] = 'admin/structure/cruise/manage';
	return;
}



function view_cruise_templates_form($form, &$form_state) {

	drupal_add_css(drupal_get_path('module', 'cruise') . '/cruise.css');

	

	

	$output = '';
	$header = array(
			array(
					'data' => 'Id',
					'field' => 'id'
			),
			array(
					'data' => 'Cruise Title',
					'field' => 'cruise_title'
			),
			array(
					'data' => 'Cruise Type',
					'field' => 'cruise_type'
			),
			array(
					'data' => 'Last Updated',
					'field' => 'created'
			),
			array(
					'data' => 'Actions'
			),
			array(
					'data' => 'Active',
			)
	);
	if (isset($_GET ['sort']) && isset($_GET ['order'])) {
		if ($_GET ['sort'] == 'asc')
			$sort = 'ASC';
		else
			$sort = 'DESC';

		switch ($_GET ['order']) {
			case 'Id' :
				$order = 'id';
				break;
			case 'Cruise Title' :
				$order = 'cruise_title';
				break;
			case 'Cruise Type' :
				$order = 'cruise_type';
				break;
			case 'Last Updated' :
				$order = 'created';
				break;
		}
	} else {
		$sort = 'DESC';
		$order = 'id';
	}

	$query = db_select("cruise", "n");
	$query->fields('n', array(
			'id',
			'cruise_title',
			'cruise_type',
			'cruise_status',
			'created'
	));
	if (@$_SESSION['option'] == 'Active') {
		$query->condition('cruise_status', '1', '=');
	} else if (@$_SESSION['option'] == 'Inactive') {
		$query->condition('cruise_status', '0', '=');
	}
	$query->orderBy($order, $sort);


	$query = $query->extend('TableSort')->extend('PagerDefault')->limit(10);
	$result = $query->execute()->fetchAll();

	global $base_url;
	$rows = array();
	if (count($result) > 0) {
		for ($i = 0; $i < count($result); $i ++) {
			$cruisetypeObject = cruisetypes_load($result [$i]->cruise_type);
			$cruisetype = $cruisetypeObject->name;
			$status = ($result [$i]->cruise_status == 1) ? "checked" : "unchecked";
			$rows [] = array(
					$result [$i]->id,
					$result [$i]->cruise_title,
					$cruisetype,
					date('Y-m-d H:i a', $result [$i]->created),
					'<a title="Edit" href="' . $base_url . '/cruise/' . $result [$i]->id . '/edit/cruise"><span class="icon-edit"></span></a> |
               	<a title="Schedule" href="' . $base_url . '/cruise/' . $result [$i]->id . '/schedule"> <span class="icon-calendar"></span></a>',
					'<input type="checkbox"' . $status . ' disabled="disabled">',
			);
		}
	}
	//<a href="' . $base_url . '/cruise/' . $result [$i]->id . '/delete">delete  </a>

	$output .= '<div class="">';
	$output .= '<div class="">';

	$output .= '</div>';
	$output .= theme_table(array(
			'header' => $header,
			'rows' => $rows,
			'attributes' => array(
					'class' => array(
							''
					)
			),
			'sticky' => true,
			'caption' => '',
			'colgroups' => array(),
			'empty' => t("No Records!")
	)) . theme('pager');

	$output .= '</div>';


	$form['outputmarkup'] = array(
			'#type' => 'markup',
			'#markup' => $output
	);
	return $form;
}


