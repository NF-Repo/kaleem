<?php

function useraccounts_page() {
    $build['userfilterform'] = drupal_get_form('account_filteration_form');
    $build['userslistform'] = drupal_get_form('view_useraccount_form');
    return $build;
}

//filter form
function account_filteration_form() {

    drupal_add_css(drupal_get_path('module', 'user_accounts') . '/user_accounts.css');
// 	$form['account_status']=array(
// 			'#type'=>'select',
// 			'#options'=>array('1'=>'active','0'=>'inactive'),
// 			'#default_value'=>$_SESSION['account_status']
// 	);


    $form['search_by'] = array(
        '#id' => 'search_by',
        '#type' => 'select',
        '#options' => array('all' => 'ALL', 'name' => 'Name', 'mail' => 'Email', 'company_name' => 'Companyname'),
        '#default_value' => $_SESSION['search_by'],
        '#prefix' => '<div class="row-fluid"><div class="span8">',
        '#attributes' => array('class' => array('pull-left input-medium searchfielsmargin')),
            //'#suffix' => '</div>',
    );

    $form['searchfield'] = array(
        '#type' => 'textfield',
        //'#title'=>'Search',
        '#states' => array(
            'invisible' => array('#search_by' => array('value' => 'all'),
            ),
        ),
        '#default_value' => $_SESSION['searchfield'],
        '#attributes' => array('class' => array('pull-left searchfielsmargin')),
            //'#attributes' => array('class' => array('plbtn'), 'tabindex' => '28'),
            //'#prefix' => '&nbsp;',
            //'#suffix' => '</div>',
    );
    $form ['go'] = array(
        '#type' => 'submit',
        '#value' => 'Find',
        //'#prefix' => '&nbsp;&nbsp;',
        '#suffix' => '</div>',
        '#attributes' => array('class' => array('pull-left searchfielsmargin')),
// 			'#states' => array(
// 					'disabled' => array('#search_by' => array('value' => 'select'),
// 					),
// 			),
    );


    $form['adduserbtn'] = array(
        '#id' => 'adduserbtn',
        '#type' => 'submit',
        '#value' => 'Add User',
        '#limit_validation_errors' => array(),
        '#prefix' => '<div class="span4" align="right">',
        '#suffix' => '</div></div>',
            //'#attributes'=>array('class'=>array('primary')),
    );


    return $form;
}

function account_filteration_form_validate($form, &$form_state) {


    if ($form_state['triggering_element']['#value'] == 'Add User') {
        drupal_goto('register-new-user');
    }
}

function account_filteration_form_submit($form, &$form_state) {
    //$_SESSION['account_status']=$form_state['values']['account_status'];
    $_SESSION['search_by'] = $form_state['values']['search_by'];
    $_SESSION['searchfield'] = $form_state['values']['searchfield'];
    $form_state['redirect'] = 'useraccounts';
    return;
}

function view_useraccount_form($form, &$form_state) {
    $output = '';
    $header = array(
        array('data' => 'Id', 'field' => 'uid'),
        array('data' => 'Username', 'field' => 'name'),
        array('data' => 'Email', 'field' => 'mail'),
        array('data' => 'Role', 'field' => 'role_name'),
        array('data' => 'Company', 'field' => 'company_name'),
        array('data' => 'Phone', 'field' => 'phone'),
        array('data' => 'Last Access', 'field' => 'access'),
        array('data' => 'Status', 'field' => 'status'),
        array('data' => 'Actions'),
    );


    //var_dump($_GET['order']);
    if (isset($_GET['sort']) && isset($_GET['order'])) {
        if ($_GET['sort'] == 'asc')
            $sort = 'ASC';
        else
            $sort = 'DESC';

        switch ($_GET['order']) {
            case 'Id':
                $order = 'uid';
                break;
            case 'Username':
                $order = 'name';
                break;
            case 'Email' :
                $order = 'mail';
                break;
            case 'Role' :
                $order = 'role_name';
                break;

            case 'Company' :
                $order = 'company_name';
                break;

            case 'Phone' :
                $order = 'phone';
                break;

            case 'Status' :
                $order = 'status';
                break;

            case 'Last Access' :
                $order = 'access';
                break;
        }
    } else {
        $sort = 'ASC';
        $order = 'uid';
    }


// 	$query = db_select('users', '');
// 	$query->fields('users', array('name', 'pass', 'mail', 'status', 'uid'));
// 	$query->fields('user_accounts', array('first_name', 'last_name', 'company_name', 'phone', 'address'));
// 	$query->innerJoin('user_accounts', '', 'users.uid = user_accounts.uid');
// 	$result = $query->execute()->fetchAll();

    $query = db_select('users', 'u');
    $query->join('user_accounts', 'ua', 'u.uid = ua.uid');
    $query->join('users_roles', 'ur', 'ur.uid = u.uid');
    $query->leftjoin('role', 'r', 'ur.rid = r.rid');
    $query->fields('u', array('name', 'pass', 'mail', 'status', 'uid', 'access'));
    $query->fields('ua', array('first_name', 'last_name', 'company_name', 'phone', 'address'));
    $query->fields('ur', array('rid'));
    $query->addField('r', 'name', 'role_name');



// 	if($_SESSION['account_status']=='1'){
// 		$query->condition('status','1','=');
// 	}else if($_SESSION['account_status']=='0'){
// 		$query->condition('status','0','=');
// 	}


    if ($_SESSION['search_by'] == 'name') {
        $query->condition('u.name', '%' . $_SESSION['searchfield'] . '%', 'LIKE');
    } else if ($_SESSION['search_by'] == 'mail') {
        $query->condition('u.mail', '%' . $_SESSION['searchfield'] . '%', 'LIKE');
    } else if ($_SESSION['search_by'] == 'company_name') {
        $query->condition('ua.company_name', '%' . $_SESSION['searchfield'] . '%', 'LIKE');
    }
// 	}else if($_SESSION['account_status']=='0'){
// 		$query->condition('status','0','=');
// 	}
    $query->orderBy($order, $sort);
    $query = $query->extend('TableSort')->extend('PagerDefault')->limit(20);

    $result = $query->execute()->fetchAll();


// 	drupal_json_output($result);
// 	exit();
    global $base_url;
    $rows = array();
    if (count($result) > 0) {
        for ($i = 0; $i < count($result); $i++) {
            $access = $result[$i]->access ? t('@time ago', array('@time' => format_interval(REQUEST_TIME - $result[$i]->access))) : t('never');
            $status = array(t('blocked'), t('active'));
            $rows[] = array(
                $result[$i]->uid,
                ucwords($result[$i]->name),
                $result[$i]->mail,
                ucwords($result[$i]->role_name),
                ucwords($result[$i]->company_name),
                $result[$i]->phone,
                $access,
                $status[$result[$i]->status],
                //$result[$i]->mail,
                '<a href="' . $base_url . '/useraccount/' . $result[$i]->uid . '/edit">Edit |</a>
                <a href="' . $base_url . '/useraccount/' . $result[$i]->uid . '/delete">Delete</a>'
            );
        }
    }

    $output .= '<div class="">';
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
}
