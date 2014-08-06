<?php

include_once 'emailtemplates.form.inc';

/**
 * Helper function to make a link.
 */
function _sendemailtemplates_make_link() {
    return drupal_render(drupal_get_form('sendemailtemplates_select_form'));
}

function sendemailtemplates_select_form($form, &$form_state) {
    $_SESSION['templateusers'] = '';
    drupal_set_title('Send Email Template', CHECK_PLAIN);
    $templateoptions = array();
    drupal_add_js(drupal_get_path('module', 'emailtemplates') . '/js/emailtemplates.js');
    $templates = entity_load('emailtemplates');
    foreach ($templates as $template) {
        $templateoptions[$template->id] = $template->name;
    }

    $form['template'] = array(
        '#type' => 'select',
        '#options' => $templateoptions,
        '#attributes' => array('class' => array('input-medium')),
    );
    $form['template_users'] = array(
        '#type' => 'textfield',
        '#title' => t(''),
        '#prefix' => '<div style="display:none;">',
        '#suffix' => '</div>',
    );


    $form['template_usermails'] = array(
        '#title' => t('Emails'),
        '#type' => 'textarea',
//        '#description' => t('The comment will be unpublished if it contains any of the phrases above. Use a case-sensitive, comma-separated list of phrases. Example: funny, bungee jumping, "Company, Inc."'),
//        '#default_value' => isset($context['keywords']) ? drupal_implode_tags($context['keywords']) : '',
    );
    $form['markup'] = array(
        '#type' => 'markup',
        '#prefix' => '<div class="row-fluid><div class="span12"><div class="pull-left">',
        '#suffix' => '</div>',
        '#markup' => '<div id="magical-modal-link">' . l('Preview', 'sendemailtemplates/nojs', array('attributes' => array('class' => 'ctools-use-modal btn'))) . '</div>'
    );
    $form["submit"] = array("#type" => "submit", "#value" => "Emails you wish to",
        "#ajax" => array(
            "callback" => "dr_search_test_form_callback",
            "wrapper" => "test-ajax", "effect" => "fade"),
        '#prefix' => '<div class="pull-left" style="margin-left:6px;">',
        '#suffix' => '</div>',
    );
    $form['actions']['cancel'] = array(
        '#type' => 'link',
        '#title' => t('Cancel'),
        '#href' => 'emailtemplates',
        '#prefix' => '<div class="pull-left">',
        '#suffix' => '</div></div>',
        '#attributes' => array('class' => array('btn inputfieldMedium')),
    );
//    $form['submit'] = array(
//        '#type' => 'submit',
//        '#value' => 'Preview',
//    );
//    if(isset($_GET['templateusers'])){
//        var_dump($_GET);
//        exit();
//    }

    $output = '';
    $header = array(
        array(
            'data' => '',
        ),
        array(
            'data' => 'Name',
            'field' => 'name'
        ),
        array(
            'data' => 'Email',
            'field' => 'mail'
        ),
    );
    if (isset($_GET ['sort']) && isset($_GET ['order'])) {
        if ($_GET ['sort'] == 'asc')
            $sort = 'ASC';
        else
            $sort = 'DESC';

        switch ($_GET ['order']) {
            case 'Name' :
                $order = 'name';
            case 'Email' :
                $order = 'mail';
                break;
        }
    } else {
        $sort = 'ASC';
        $order = 'mail';
    }
    $query = db_select("users", "u");
    $query->condition('uid', 0, '!=');
    $query->condition('uid', 1, '!=');
    $query->fields('u', array(
        'uid',
        'name',
        'mail'
    ));

//    $query->orderBy($order, $sort);
    $query = $query->extend('TableSort')->extend('PagerDefault')->limit(10);
    $result = $query->execute()->fetchAll();
//    
    if (count($result) > 0) {
        for ($i = 0; $i < count($result); $i ++) {
//            $detailsview = '<a href="' . base_path() . 'customer/' . $result [$i]->uid . '/orders' . $getparams . '"><input type="button" class="button" value="Detail View"></a>';
            $rows [] = array(
                '<input type="checkbox" name="groupcheck" class="checkboxgroup" value="' . $result [$i]->mail . '" >',
                $result [$i]->name,
                $result [$i]->mail,
            );
        }
    }
    $output .= '<div class="row-fluid"><div class="span12">';
//    $output .= '<div class="">';
//
//    $output .= '</div>';
    $output .= theme_table(array(
                'header' => $header,
                'rows' => $rows,
                'attributes' => array(
                    'class' => array()
                ),
                'sticky' => true,
                'caption' => '',
                'colgroups' => array(),
                'empty' => t("No Records!")
            )) . theme('pager');

    $output .= '</div>';
    $output .= '</div>';


    $form['outputmarkup'] = array(
        '#type' => 'markup',
        '#markup' => $output
    );
    return $form;
}

//
//function sendemailtemplates_select_form_submit($form, &$form_state) {
//    $users = $form_state['input'] ["template_users"];
//    $template = $form_state['input'] ["template"];
//    return '<div id="magical-modal-link">' . l('Preview', 'sendemailtemplates/nojs/' . $users . '/' . $template . '/modal', array('attributes' => array('class' => 'ctools-use-modal'))) . '</div>';
//}
function dr_search_test_form_callback($form, &$form_state) {
    $templateusers = $form_state['input']['template_users'];
    $template = $form_state['input']['template'];
    $_SESSION['templateusers'] = $templateusers;
    $_SESSION['template'] = $template;
    $form['template_usermails']['#default_value'] = $templateusers;

    $commands = array();
    $commands[] = ajax_command_invoke('#edit-template-usermails', 'attr', array('value', $templateusers));

    return array(
        '#type' => 'ajax',
        '#commands' => $commands
    );
    //return $form;
}

//function sendemailtemplates_select_form_validate($form, &$form_state) {
//
////    if ($form_state['triggering_element']['#value'] == 'Preview') {
////        $form_state['rebuild'] = TRUE;
////        return;
////    }t
//}

/**
 * An example page.
 */
function sendemailtemplates_page() {

    // Load the modal library and add the modal javascript.
    ctools_include('modal');
    ctools_modal_add_js();
    return _sendemailtemplates_make_link();
}

/**
 * Ajax menu callback.
 */
function sendemailtemplates_callback($ajax) {

//    if ($ajax) {
//        ctools_include('ajax');
//        ctools_include('modal');
//
//        $form_state = array(
//            'ajax' => TRUE,
//            'title' => t('MyModule Modal Form'),
//        );
//
//        // Use ctools to generate ajax instructions for the browser to create
//        // a form in a modal popup.
//        $output = ctools_modal_form_wrapper('sendemailtemplates_form', $form_state);
//
//        // If the form has been submitted, there may be additional instructions
//        // such as dismissing the modal popup.
//        if (!empty($form_state['ajax_commands'])) {
//            $output = $form_state['ajax_commands'];
//        }
//
//        // Return the ajax instructions to the browser via ajax_render().
//        print ajax_render($output);
//        drupal_exit();
//    } else {
//        return drupal_get_form('sendemailtemplates_form');
//    }
    // Fall back if $ajax is not set.
    if (!$ajax) {
        return drupal_get_form('sendemailtemplates_form');
    }

    ctools_include('modal');
    ctools_include('ajax');
    $form_state = array(
        'title' => t('Send Email Templates'),
        'id' => 'sendemailtemplates_form',
        'ajax' => TRUE,
    );

    $output = ctools_modal_form_wrapper('sendemailtemplates_form', $form_state);
    if (!empty($form_state['executed'])) {
        // We'll just overwrite the form output if it was successful.
        $output = array();
        ctools_add_js('ajax-responder');
        $output[] = ctools_modal_command_dismiss(t('Popup Added Successfully'));

        $output[] = ctools_ajax_command_reload();
    }
    print ajax_render($output);
}

/**
 * Drupal form to be put in a modal.
 */
function sendemailtemplates_form($form, $form_state) {
    $form = array();
//    $userid = $_GET['userid'];
//    $templateid = $_GET['templateid'];
    /*   get the all  selecteduseremails   */
//    $userids = $_SESSION['templateusers'];
    $useremailids = $_SESSION['templateusers'];
    $templateid = $_SESSION['template'];
    $userids_arr = explode(',', $userids);
    $useremails = array();
    $useremailsdata = '';
    $useremails = explode(',', $useremailids);
    foreach ($useremails as $useremail) {
        $userdata = user_load($userid);
        if ($useremailsdata != '') {
            $useremailsdata.=',' . $useremailids;
        } else {
            $useremailsdata.=$useremailids;
        }
    }
//    foreach ($userids_arr as $userid) {
//        $userdata = user_load($userid);
//        if ($useremails != '') {
//            $useremails.=',' . $userdata->mail;
//        } else {
//            $useremails.=$userdata->mail;
//        }
//    }

    /*   get the email template data   */
    $templatedata = emailtemplates_load($templateid);
    $subject = '';
    $body = '';
    $subject = $templatedata->subject;
    $body = $templatedata->mail_body;
//        
//    $form['selectedusers'] = array(
//        '#type' => 'textfield',
//        '#title' => t(''),
//        '#prefix'=>'<div style="display:none">',
//        '#default_value' => $userids,
//    );
//    $form['selectedtemplate'] = array(
//        '#type' => 'textfield',
//        '#title' => t(''),
//        '#suffix'=>'</div>',
//        '#default_value' => $templateid,
//    );

    $output .= '<div class="row-fluid" style="border-bottom:1px solid #CFCFCF;"><div class="span12"><div class="pull-left">To : </div><div class="pull-left">' . $useremailids . '</div></div></div>';
    $output .= '<div class="row-fluid" style="border-bottom:1px solid #CFCFCF;"><div class="span12"><div class="pull-left">Subject : </div><div class="pull-left">' . $subject . ' </div></div></div>';
    $output .= '<div class="row-fluid" style="border-bottom:1px solid #CFCFCF;"><div class="span12" style="min-height:250px;">' . $body . '<div class="pull-left"></div></div></div>';


    $form['outputmarkup'] = array(
        '#type' => 'markup',
        '#markup' => $output
    );
//    $form["submit"] = array("#type" => "submit", "#value" => "Send Email Template",
//        "#ajax" => array(
//            "callback" => "sendemailtemplates_to_users",
//            "wrapper" => "test-ajax", "effect" => "fade")
//    );
    $form['create'] = array(
        '#type' => 'submit',
        '#value' => 'SEND',
        '#executes_submit_callback' => TRUE,
        '#prefix' => '<div class="row-fluid"><div class="span12 pull-left" style="margin-top:10px;">',
        '#suffix' => '</div></div>',
        '#submit' => array('sendemailtemplates_to_users'),
        '#weight' => 5,
    );
    return $form;
}

/**
 * Drupal form submit handler.
 */
function sendemailtemplates_to_users(&$form, &$form_state) {
    // Generate the new link using the submitted text value.
    ctools_include('modal');
    ctools_modal_add_js();
    /*   get the all  selecteduseremails   */
//    $userids = $_SESSION['templateusers'];
    $useremailids = $_SESSION['templateusers'];
    $templateid = $_SESSION['template'];
    $userids_arr = explode(',', $userids);
    $useremails = array();
    $useremailsdata = '';
    $useremails = explode(',', $useremailids);

    $recipients = array();
    $recipients = $useremails;
//    foreach ($userids_arr as $userid) {
//        $userdata = user_load($userid);
//        $recipients[] = $userdata->mail;
//    }
    emailtemplates_send_mail_bytempid($templateid, $recipients, $options = array());
//    var_dump($form_state['input']['selectedusers']);
//    var_dump($form_state['input']['selectedtemplate']);
//    exit();
    // Generate the new link using the submitted text value.
    $link = _sendemailtemplates_make_link();

    // Tell the browser to close the modal.
    $form_state['ajax_commands'][] = ctools_modal_command_dismiss();

    // Tell the browser to replace the old link with the new one.
    $form_state['ajax_commands'][] = ajax_command_replace('#magical-modal-link', $link);
//    return $form;
    drupal_set_message('Email sent Successfully.');
    $_SESSION['templateusers'] = '';
}

/* menu callback function for the assigning email templates to the Cruise event */

function cruise_event_emailtemplates() {

    return drupal_render(drupal_get_form('cruise_event_emailtemplates_form'));
}

function cruise_event_emailtemplates_form($form, &$form_state) {
    $_SESSION['templateusers'] = '';
    $templateoptions = array();
    $eventslist = array();
    $eventoptions = array();
    $eventslist = getcruise_events();
    foreach ($eventslist as $event) {
        $eventoptions[$event->scheduledevent_id] = $event->name;
    }
    $templates = entity_load('emailtemplates');
    foreach ($templates as $template) {
        $templateoptions[$template->id] = $template->name;
    }
//    $form['eventid'] = array(
//        '#type' => 'select',
//        '#prefix' => '<div class="row-fluid"><div class="span12"><div class="pull-left">',
//        '#suffix' => '</div>',
//        '#title' => t('Cruise Event'),
//        '#options' => $eventoptions,
//        '#attributes' => array('class' => array('input-medium')),
//    );



    $cruises = getallcruisedetails();
    $cruiseoptions = array();
    $cruiseoptions[''] = 'Select';
    foreach ($cruises as $cruise) {
        $cruiseoptions[$cruise->id] = $cruise->cruise_title;
    }

    $form["cruisetype"] = array(
        '#type' => 'select',
        '#title' => t('Cruise'),
        '#options' => $cruiseoptions,
//        '#default_value' => 'defalut_value',
        '#ajax' => array(
            'event' => 'change',
            'callback' => 'cruise_events',
            'wrapper' => 'crusievent',
            'method' => 'replace',
        ),
//        '#attributes' => array('onchange' => "form.eventsubmit('cruisetype')"),
    );
    $form['templateid'] = array(
        '#type' => 'select',
        '#prefix' => '<div class="pull-left">',
        '#suffix' => '</div></div></div>',
        '#title' => t('Choose Template'),
        '#options' => $templateoptions,
        '#attributes' => array('class' => array('input-medium')),
    );
    $form['submit'] = array(
        '#type' => 'submit',
        '#prefix' => '<div class="row-fluid"><div class="span12"><div class="pull-left">',
        '#suffix' => '</div></div></div>',
        '#value' => 'Submit',
    );
    return $form;
}

function cruise_event_emailtemplates_form_submit($form, &$form_state) {

    $cruisetype = isset($form_state['input']['cruisetype']) ? $form_state['input']['cruisetype'] : '';

//    $eventid = isset($form_state['input']['eventid']) ? $form_state['input']['eventid'] : '';
    $templateid = isset($form_state['input']['templateid']) ? $form_state['input']['templateid'] : '';
    $emailtemplates = new stdClass();
//    $emailtemplates->eventid = $eventid;
    $emailtemplates->cruise_templateid = $cruisetype;
    $emailtemplates->templateid = $templateid;
    $event_templates = getcruise_event_templates($cruisetype);
    if (count($event_templates) > 0) {
        $emailtemplates->id = $event_templates[0]->id;
    }
    if (isset($emailtemplates->id) && ($emailtemplates->id != null)) {
        try {
            drupal_write_record('cruise_event_emailtemplates', $emailtemplates, array('id'));
            drupal_set_message('Email template Update to Cruise Successfully');
        } catch (Exception $e) {
            watchdog("cruise_event_emailtemplates", "Error while Updating cruise_event_emailtemplates");
        }
    } else {
        try {
            drupal_write_record('cruise_event_emailtemplates', $emailtemplates);
            drupal_set_message('Email template Added to Cruise Successfully');
        } catch (Exception $e) {
            watchdog("cruise_event_emailtemplates", "Error while Inserting the cruise_event_emailtemplates");
        }
    }
}

function cruise_template_emailtemplates() {
    return drupal_render(drupal_get_form('cruise_template_emailtemplateslist_form'));
}

function cruise_template_emailtemplateslist_form_validate($form, &$form_state) {
    if ($form_state['triggering_element']['#value'] == 'Go') {
        $form_state['rebuild'] = TRUE;
        return;
    }
}

function cruise_template_emailtemplateslist_form($form, &$form_state) {
    global $base_url;
    $_SESSION['templateusers'] = '';
    $title = ucwords('Cruise Email templates');
    drupal_set_title($title, $output = CHECK_PLAIN);
    drupal_add_css(drupal_get_path('module', 'emailtemplates') . '/emailtemplates.css');
    $output = '';

    $header = array(
        array(
            'data' => 'Id',
            'field' => 'id'
        ),
        array(
            'data' => 'Cruise Template',
            'field' => 'cruise_title'
        ),
        array(
            'data' => 'Emailtemplate',
            'field' => 'name'
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
            case 'Cruise Template' :
                $order = 'cruise_title';
                break;
            case 'Emailtemplate' :
                $order = 'name';
                break;
        }
    } else {
        $sort = 'ASC';
        $order = 'id';
    }

    $query = db_select("cruise_event_emailtemplates", "e");
    $query->Join('cruise', 'c', 'c.id = e.cruise_templateid');
    $query->Join('emailtemplates', 't', 't.id = e.templateid');
    $query->fields('e', array('id'));
    $query->fields('c', array('cruise_title'));
    $query->fields('t', array('name'));

    $query->orderBy($order, $sort);


    $query = $query->extend('TableSort')->extend('PagerDefault')->limit(10);
    $result = $query->execute()->fetchAll();

    global $base_url;
    $rows = array();
    if (count($result) > 0) {
        for ($i = 0; $i < count($result); $i ++) {

            $rows [] = array(
                $result [$i]->id,
                $result [$i]->cruise_title,
                $result [$i]->name
            );
        }
    }

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
        '#markup' => $html . $output
    );
    return $form;
}

function add_emailtemplates_tocruise() {
    return drupal_get_form('add_emailtemplates_tocruise_form');
}

function add_emailtemplates_tocruise_form_validate($form, &$form_state) {
    if ($form_state['triggering_element']['#value'] == 'Go') {
        $form_state['rebuild'] = TRUE;
        return;
    }
}

function add_emailtemplates_tocruise_form($form, &$form_state) {
    global $base_url;
    $_SESSION['templateusers'] = '';
    $cruiseid = arg(1);
    $cruise=cruise_load($cruiseid);
    $cruisetitle=isset($cruise->cruise_title)?$cruise->cruise_title:'';
    $title = ucwords($cruisetitle.' Email templates');
    drupal_set_title($title, $output = CHECK_PLAIN);
    drupal_add_css(drupal_get_path('module', 'emailtemplates') . '/emailtemplates.css');

    $templateoptions = array();
    $eventslist = array();
    $eventoptions = array();
    $eventslist = getcruise_events();
    foreach ($eventslist as $event) {
        $eventoptions[$event->scheduledevent_id] = $event->name;
    }
    $templates = entity_load('emailtemplates');
    $templateoptions[''] = 'Select';
    foreach ($templates as $template) {
        $templateoptions[$template->id] = $template->name;
    }
   
    $cruisetemplates=getcruise_event_templates($cruiseid);
  
    foreach ($cruisetemplates as $cruisetemplate){
        if($cruisetemplate->period == 72 && $cruisetemplate->type == 'prior'){
            $templateid_prior72=$cruisetemplate->templateid;
            $templateid_prior72_id=$cruisetemplate->id;
        }
        if($cruisetemplate->period == 24 && $cruisetemplate->type == 'prior'){
            $templateid_prior24=$cruisetemplate->templateid;
            $templateid_prior24_id=$cruisetemplate->id;
        }
        if($cruisetemplate->period == 24 && $cruisetemplate->type == 'post'){
            $templateid_post24=$cruisetemplate->templateid;
            $templateid_post24_id=$cruisetemplate->id;
        }
    }
    
    $form['templateid_prior72'] = array(
        '#type' => 'select',
        '#prefix' => '<div class="row-fluid"><div class="span12"><div class="pull-left" style="margin:1em;">72 Hrs Prior Email Template :</div><div class="pull-left">',
        '#suffix' => '</div></div>',
        '#title' => t(''),
        '#options' => $templateoptions,
         '#default_value' => isset($templateid_prior72)?$templateid_prior72:'',
        '#attributes' => array('class' => array('input-xxlarge')),
    );
    $form['templateid_prior72_id'] = array(
        '#type' => 'textfield',
        '#prefix' => '<div style="display:none">',
        '#suffix' => '</div>',
        '#title' => t(''),
        '#default_value' => isset($templateid_prior72_id)?$templateid_prior72_id:'',
    );
    $form['templateid_prior24'] = array(
        '#type' => 'select',
        '#prefix' => '<div class="row-fluid"><div class="span12"><div class="pull-left" style="margin:1em;">24 Hrs Prior Email Template :</div><div class="pull-left">',
        '#suffix' => '</div></div>',
        '#title' => t(''),
        '#options' => $templateoptions,
        '#default_value' => isset($templateid_prior24)?$templateid_prior24:'',
        '#attributes' => array('class' => array('input-xxlarge')),
    );
    $form['templateid_prior24_id'] = array(
        '#type' => 'textfield',
        '#prefix' => '<div style="display:none">',
        '#suffix' => '</div>',
        '#title' => t(''),
        '#default_value' => isset($templateid_prior24_id)?$templateid_prior24_id:'',
    );
    $form['templateid_after24'] = array(
        '#type' => 'select',
        '#prefix' => '<div class="row-fluid"><div class="span12"><div class="pull-left" style="margin:1em;">24 Hrs Post Email Template :</div><div class="pull-left">',
        '#suffix' => '</div></div>',
        '#title' => t(''),
        '#options' => $templateoptions,
        '#default_value' => isset($templateid_post24)?$templateid_post24:'',
        '#attributes' => array('class' => array('input-xxlarge')),
    );
    $form['templateid_after24_id'] = array(
        '#type' => 'textfield',
        '#prefix' => '<div style="display:none">',
        '#suffix' => '</div>',
        '#default_value' => isset($templateid_post24_id)?$templateid_post24_id:'',
        '#title' => t(''),
    );
    $form['submit'] = array(
        '#type' => 'submit',
        '#prefix' => '<div class="row-fluid"><div class="span12"><div class="pull-left">',
        '#suffix' => '</div></div></div>',
        '#value' => 'Submit',
    );
    return $form;
}

function add_emailtemplates_tocruise_form_submit($form, &$form_state) {
    $cruiseid = arg(1);
    $cruiseemailtemplates = array();
    $cruise72hrprior = new stdClass();
    $cruise72hrprior->id = $form_state['values']['templateid_prior72_id'];
    $cruise72hrprior->templateid = $form_state['values']['templateid_prior72'];
    $cruise72hrprior->cruise_templateid = $cruiseid;
    $cruise72hrprior->type = 'prior';
    $cruise72hrprior->period = 72;
    $cruiseemailtemplates[] = $cruise72hrprior;

    $cruise24hrprior = new stdClass();
    $cruise24hrprior->id = $form_state['values']['templateid_prior24_id'];
    $cruise24hrprior->templateid = $form_state['values']['templateid_prior24'];
    $cruise24hrprior->cruise_templateid = $cruiseid;
    $cruise24hrprior->type = 'prior';
    $cruise24hrprior->period = 24;
    $cruiseemailtemplates[] = $cruise24hrprior;

    $cruise24hrpost = new stdClass();
    $cruise24hrpost->id = $form_state['values']['templateid_after24_id'];
    $cruise24hrpost->templateid = $form_state['values']['templateid_after24'];
    $cruise24hrpost->cruise_templateid = $cruiseid;
    $cruise24hrpost->type = 'post';
    $cruise24hrpost->period = 24;
    $cruiseemailtemplates[] = $cruise24hrpost;

    foreach ($cruiseemailtemplates as $cruiseemailtemplate) {
        if (isset($cruiseemailtemplate->id) && ($cruiseemailtemplate->id != null)) {
            try {
                drupal_write_record('cruise_event_emailtemplates', $cruiseemailtemplate, array('id'));                
            } catch (Exception $e) {
                watchdog("cruise_event_emailtemplates", "Error while Updating cruise_event_emailtemplates");
            }
        } else {
            try {
                drupal_write_record('cruise_event_emailtemplates', $cruiseemailtemplate);
            } catch (Exception $e) {
                watchdog("cruise_event_emailtemplates", "Error while Inserting the cruise_event_emailtemplates");
            }
        }
    }
    drupal_set_message('Email templates Added to Cruise Successfully');    

}

?>