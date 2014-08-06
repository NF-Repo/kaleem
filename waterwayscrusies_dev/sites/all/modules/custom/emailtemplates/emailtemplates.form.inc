
<?php

/**
 * Provides a wrapper on the edit form to add a new entity.
 */
function emailtemplates_add() {
    // Create a basic entity structure to be used and passed to the validation
    // and submission functions.
//    $emailtemplates = entity_get_controller('emailtemplates')->create();
    return drupal_get_form('emailtemplates_form', $emailtemplates);
}

/**
 * Form function to create an emailtemplates entity.
 *
 * The pattern is:
 * - Set up the form for the data that is specific to your
 *   entity: the columns of your base table.
 * - Call on the Field API to pull in the form elements
 *   for fields attached to the entity.
 */
function emailtemplates_form($form, &$form_state, $emailtemplates) {

    drupal_add_css(drupal_get_path('module', 'emailtemplates') . '/emailtemplates.css');
    $formstatus = $emailtemplates->id == '' ? "add" : "edit";
    if ($formstatus == 'add') {
        $title = 'Create New Email Template';
    }
    if ($formstatus == 'edit') {
        $title = 'Edit Email Template';
    }
    $title = ucwords($title);
    drupal_set_title($title, $output = CHECK_PLAIN);
    
    $form['emailtemplatesdivstart'] = array(
        '#type' => 'markup',
        '#markup' => '<div id="emailtemplatesForm">',
    );

    $form['operation'] = array(
        '#type' => 'hidden',
        '#value' => $formstatus
    );

    $auto_id = $emailtemplates->id != '' ? $emailtemplates->id : "";
    $form['auto_id'] = array(
        '#type' => 'hidden',
        '#value' => $auto_id
    );


    $form['name'] = array(
        '#type' => 'textfield',
        '#title' => t('Name'),
        '#required' => TRUE,
        '#default_value' => isset($emailtemplates->name) ? $emailtemplates->name : '',
    );


    $form['subject'] = array(
        '#type' => 'textfield',
        '#title' => t('subject'),
        '#required' => TRUE,
        '#default_value' => isset($emailtemplates->subject) ? $emailtemplates->subject : '',
    );


//    $form['mail_body'] = array(
//        '#type' => 'textarea',
//        '#title' => t('Body'),
//        '#rows' => 15,
//        '#default_value' => isset($emailtemplates->mail_body) ? $emailtemplates->mail_body : '',
//    );
    $form['mail_body'] = array(
        '#type' => 'text_format',
        '#format' => 'emailtemplate_editor',
        '#title' => t('Body'),
        '#prefix' => '<br/>',
        '#suffix' => '<br/>',
//        '#rows' => 15,
        '#default_value' => isset($emailtemplates->mail_body) ? $emailtemplates->mail_body : '',
    );

    field_attach_form('emailtemplates', $emailtemplates, $form, $form_state);

    $form['actions'] = array('#type' => 'actions');
    $form['actions']['submit'] = array(
        '#type' => 'submit',
        '#value' => t('Save'),
        '#prefix' => '<div class="row-fluid><div class="span12"><div class="pull-left">',
        '#suffix' => '</div>',
    );
//    $form['cancel'] = array(
//        '#type' => 'submit',
//        '#value' => t('Cancel'),
//         '#prefix' => '<div class="pull-left">',
//        '#suffix' => '</div></div>',
//        '#submit' => array('emailtemaplates_cancel'),
//    );
    $form['actions']['cancel'] = array(
        '#type' => 'link',
        '#title' => t('Cancel'),
        '#href' => 'emailtemplates',
        '#prefix' => '<div class="pull-left">',
        '#suffix' => '</div></div>',
        '#attributes' => array('class' => array('btn inputfieldMedium')),
    );

    $form['emailtemplatesdivend'] = array(
        '#type' => 'markup',
        '#markup' => '</div>',
    );
    return $form;
}

/**
 * Validation handler for emailtemplates_add_form form.
 * We pass things straight through to the Field API to handle validation
 * of the attached fields.
 */

/**
 * Form submit handler: submits basic_add_form information
 */
function emailtemplates_form_submit($form, &$form_state) {

    $emailtemplates = new stdClass();
    $emailtemplates->id = $form_state['values']['auto_id'];
    $emailtemplates->name = $form_state['values']['name'];
    $emailtemplates->subject = $form_state['values']['subject'];
    $emailtemplates->mail_body = $form_state['values']['mail_body']['value'];


    field_attach_submit('emailtemplates', $emailtemplates, $form, $form_state);
    $emailtemplates = emailtemplates_save($emailtemplates);
    $operation = $form_state['values']['operation'];
    $msg = '';
    if ($operation == "add") {
        $msg = 'Email template created successfully.';
    } else {
        $msg = 'Email template updated successfully.';
    }

    $form_state['redirect'] = 'emailtemplates';
}

function emailtemaplates_cancel($form, &$form_state) {
    global $base_url;
    $url = $base_url . '/emailtemplates';
    drupal_goto($url);
}

/**
 * Multi-step form for previewing and sending a Email Temaplate.
 */
function emailtemplates_user_form($form, &$form_state, $emailtemplates) {

    if (emailtemplates_isset_or($form_state['storage']['step']) == 3) {
        drupal_set_message(t('Email(s) sent'));
        unset($form_state['storage']);
    }

    $step = empty($form_state['storage']['step']) ? 1 : $form_state['storage']['step'];
    $form_state['storage']['step'] = $step;
    $form_state['storage']['emailtemplates'] = $emailtemplates;

// Get any query args
    $nid = $form_state['storage']['nid'] = emailtemplates_is_natural(emailtemplates_isset_or($_REQUEST['nid'])) ? $_REQUEST['nid'] : NULL;
    $uid = $form_state['storage']['uid'] = emailtemplates_is_natural(emailtemplates_isset_or($_REQUEST['uid'])) ? $_REQUEST['uid'] : NULL;
    $recipient_callback = $form_state['storage']['recipient_callback'] = (
            emailtemplates_isset_or($_REQUEST['recipient_callback']) === 'true' ||
            emailtemplates_isset_or($_REQUEST['uid']) === '0' // backward compatibility
            );


    if ($recipient_callback) {
        $default_mail = t('Recipient list will be generated for preview.');
    } elseif (emailtemplates_isset_or($form_state['storage']['recipients_raw'])) {
        $default_mail = $form_state['storage']['recipients_raw'];
    } else {
        $default_mail = '';
        if ($uid) {
            if ($account = user_load($uid)) {
                $default_mail = $account->mail;
            } else {
                drupal_set_message(t('Cannot load a user with uid @uid.', array('@uid' => $uid)), 'error');
            }
        }
    }

    switch ($step) {
        case 1:
            $form['recipients'] = array(
                '#title' => t('To'),
                '#type' => 'textarea',
                '#required' => TRUE,
                '#rows' => 3,
                '#default_value' => $default_mail,
                '#description' => t('Enter the recipient(s) separated by lines or commas. A separate email will be sent to each, with token substitution if the email corresponds to a site user.'),
                '#disabled' => $recipient_callback,
            );

            $form['subject'] = array(
                '#type' => 'textfield',
                '#title' => t('Subject'),
                '#maxlength' => 255,
                '#default_value' => $emailtemplates->subject,
                '#required' => TRUE,
            );
            if (!(emailtemplates_has_mimemail())) {
                $form['mail_body'] = array(
                    '#type' => 'text_format',
                    '#format' => 'emailtemplate_editor',
                    '#title' => t('Body'),
                    '#default_value' => $emailtemplates->mail_body,
//                    '#rows' => 15,
                    '#description' => t('Review and edit standard template before previewing. This will not change the template for future emailings, just for this one. To change the template permanently, go to the <a href="@settings">template page</a>. You may use the tokens below.', array('@settings' => url('admin/structure/pets/manage/' . $emailtemplates->name))),
                );
//                $form['mail_body'] = array(
//                    '#type' => 'textarea',
//                    '#title' => t('Body'),
//                    '#default_value' => $emailtemplates->mail_body,
//                    '#rows' => 15,
//                    '#description' => t('Review and edit standard template before previewing. This will not change the template for future emailings, just for this one. To change the template permanently, go to the <a href="@settings">template page</a>. You may use the tokens below.', array('@settings' => url('admin/structure/pets/manage/' . $emailtemplates->name))),
//                );
            }


            $form['preview'] = array(
                '#type' => 'submit',
                '#value' => t('Preview'),
            );


            break;

        case 2:

            $form['info'] = array(
                '#value' => t('A preview of the email is shown below. If you\'re satisfied, click Send. If not, click Back to edit the email.'),
            );

            //receipents User Emails with Their User Ids
            $form['recipients'] = array(
                '#type' => 'textarea',
                '#title' => t('To'),
                '#rows' => 4,
                '#value' => emailtemplates_recipients_formatted($form_state['storage']['recipients']),
                '#disabled' => TRUE,
            );

            $form['subject'] = array(
                '#type' => 'textfield',
                '#title' => t('Subject'),
                '#size' => 80,
                '#value' => $form_state['values']['subject'],
                '#disabled' => TRUE,
            );

            $form['mail_body'] = array(
                '#type' => 'textarea',
                '#title' => t('Body'),
                '#rows' => 15,
                '#value' => $form_state['values']['mail_body'],
                '#disabled' => TRUE,
            );


            $form['back'] = array(
                '#type' => 'submit',
                '#value' => t('Back'),
                '#submit' => array('emailtemplates_user_form_back'),
            );
            $form['submit'] = array(
                '#type' => 'submit',
                '#value' => t('Send email(s)'),
            );
            break;
    }
    return $form;
}

/**
 * Form submission.  Take action on step 2 (confirmation of the populated templates).
 */
function emailtemplates_user_form_submit($form, &$form_state) {


    $step = empty($form_state['storage']['step']) ? 1 : $form_state['storage']['step'];
    $form_state['storage']['step'] = $step;

    switch ($step) {
        case 1:

            $form_state['rebuild'] = TRUE;
            $form_state['storage']['recipients_raw'] = $form_state['values']['recipients'];
            $form_state['storage']['subject'] = $form_state['values']['subject'];
            $form_state['storage']['mail_body'] = $form_state['values']['mail_body'];
            break;

        case 2:

            /* validations to be done before send a email */
//Getting Reciepients from  emailtemplates_user_form_validate//  

            $form_state['rebuild'] = TRUE;
            $name = $form_state['storage']["emailtemplates"]->name;
            $recipients = $form_state['storage']['recipients'];
            $options = array(
                'subject' => $form_state['values']['subject'],
                'body' => $form_state['values']['mail_body'],
            );
            emailtemplates_send_mail($name, $recipients, $options);
            break;
    }

    $form_state['storage']['step'] ++;
}

function emailtemplates_send_mail($name, $recipients, $options) {


    $emailtemp = emailtemplates_load_by_name($name);
    foreach ($emailtemp as $emailtmplates) {
        $emailtempl = $emailtmplates;
    }


    // Override subject and body if necessary
    $emailtempl->subject = isset($options['subject']) ? $options['subject'] : $emailtempl->subject;
    $emailtempl->mail_body = isset($options['body']) ? $options['body'] : $emailtempl->mail_body;


    // Resolve from address
    if (isset($options['from'])) {
        $from = $options['from'];
    } else {
        $from = variable_get('site_mail', ini_get('sendmail_from'));
    }

    // Store data in params in case a module wants to act on them somehow
    $params = array(
        'emailtemplates_from' => $from,
        'emailtemplates_recipients' => $recipients,
        'emailtemplates_nid' => isset($options['nid']),
    );

    foreach ($recipients as $recipient) {
        if (is_array($recipient)) {
            $params['emailtemplates_to'] = $recipient['mail'];
            $params['emailtemplates_uid'] = $recipient['uid'];
        } else {
            // Strip leading uid for backward compatability
            $mail = preg_replace('/^[0-9]*\|/', '', $recipient);
            $params['emailtemplates_to'] = $mail;
            $params['emailtemplates_uid'] = emailtemplates_lookup_uid($mail);
        }
        emailtemplates_send_one_mail($emailtempl, $params);
    }
}

function emailtemplates_send_mail_bytempid($tempid, $recipients, $options) {


    $emailtemp = emailtemplates_load_by_id($tempid);
    foreach ($emailtemp as $emailtmplates) {
        $emailtempl = $emailtmplates;
    }


    // Override subject and body if necessary
    $emailtempl->subject = isset($options['subject']) ? $options['subject'] : $emailtempl->subject;
    $emailtempl->mail_body = isset($options['body']) ? $options['body'] : $emailtempl->mail_body;


    // Resolve from address
    if (isset($options['from'])) {
        $from = $options['from'];
    } else {
        $from = variable_get('site_mail', ini_get('sendmail_from'));
    }

    // Store data in params in case a module wants to act on them somehow
    $params = array(
        'emailtemplates_from' => $from,
        'emailtemplates_recipients' => $recipients,
        'emailtemplates_nid' => isset($options['nid']),
    );

    foreach ($recipients as $recipient) {
        if (is_array($recipient)) {
            $params['emailtemplates_to'] = $recipient['mail'];
            $params['emailtemplates_uid'] = $recipient['uid'];
        } else {
            // Strip leading uid for backward compatability
            $mail = preg_replace('/^[0-9]*\|/', '', $recipient);
            $params['emailtemplates_to'] = $mail;
            $params['emailtemplates_uid'] = emailtemplates_lookup_uid($mail);
        }
        emailtemplates_send_one_mail($emailtempl, $params);
    }
}

function emailtemplates_send_one_mail($emailtemplates, $params) {


    if (!emailtemplates_is_valid($emailtemplates)) {
        watchdog('emailtemplates', 'Invalid emailtemplates object in pet_send_one_mail().', array(), WATCHDOG_ERROR);
        return;
    }

    if (empty($params['emailtemplates_from'])) {
        watchdog('emailtemplates', 'Missing sender email address in emailtemplates_send_one_mail() for emailtemplates \'%name\'.', array('%name' => $emailtemplates->name), WATCHDOG_ERROR);
        return;
    }

    if (empty($params['emailtemplates_to'])) {
        watchdog('emailtemplates', 'Missing recipient email address in emailtemplates_send_one_mail() for emailtemplates \'%name\'.', array('%name' => $emailtemplates->name), WATCHDOG_ERROR);
        return;
    }

    $params['emailtemplates'] = $emailtemplates;

    $substitutions = emailtemplates_substitutions($emailtemplates, $params);
    $params['subject'] = token_replace($emailtemplates->subject, $substitutions);
    $params['body'] = token_replace($emailtemplates->mail_body, $substitutions);


    $message = drupal_mail('emailtemplates', $emailtemplates->name, $params['emailtemplates_to'], language_default(), $params, $params['emailtemplates_from']);
    if ($message['result']) {
        watchdog('emailtemplates', 'Successfully sent email to %recipient', array('%recipient' => $params['emailtemplates_to']));
    }
}

function emailtemplates_substitutions($emailtemplates, $params) {
    $uid = $params['emailtemplates_uid'];
    $nid = $params['emailtemplates_nid'];

    // Standard substitutions
    $substitutions['global'] = NULL;
    if (!empty($uid)) {
        $user = user_load($uid);
        $substitutions['user'] = $user;
    }
    if (!empty($nid)) {
        $node = node_load($nid);
        $substitutions['node'] = $node;
    }

    // Give modules the opportunity to add their own token types/objects
    drupal_alter('emailtemplates_substitutions', $substitutions, $params);
    return $substitutions;
}

function emailtemplates_lookup_uid($mail) {
    $uid = db_query_range('SELECT uid FROM {users} WHERE mail = :mail', 0, 1, array(':mail' => $mail))->fetchField();
    return $uid;
}

function emailtemplates_is_valid($emailtemplates) {
    return is_object($emailtemplates) && !empty($emailtemplates->name) && is_numeric($emailtemplates->id);
}

function emailtemplates_load_by_name($name) {
    $emailtemp = emailtemplates_load_multiple(array(), array('name' => $name));
    return $emailtemp;
}

function emailtemplates_load_by_id($id) {
    $emailtemp = emailtemplates_load_multiple(array(), array('id' => $id));
    return $emailtemp;
}

function emailtemplates_mail($key, &$message, $params) {
    $message['subject'] = $params['subject'];
    $message['body'][] = $params['body'];
    $message['headers']['Content-Type'] = 'text/html; charset=UTF-8; format=flowed';
}

/**
 * Return user to starting point on template multi-form.
 */
function emailtemplates_user_form_back($form, &$form_state) {
    $form_state['rebuild'] = TRUE;
    $form_state['storage']['step'] = 1;
}

/**
 * Return an array of email recipients provided by a callback function.
 */
function emailtemplates_callback_recipients($form_state) {
    $nid = $form_state['storage']['nid'];
    $emailtemplates = $form_state['storage']['emailtemplates'];
    $callback = $emailtemplates->recipient_callback;
    $node = empty($nid) ? NULL : node_load($nid);

    if (!empty($callback)) {
        if (function_exists($callback)) {
            $recipients = $callback($node);

            // Remove uid for backward compatibility
            if (isset($recipients) && is_array($recipients)) {
                $new_recipients = array();
                foreach ($recipients as $recipient) {
                    $recipient = preg_replace('/^[0-9]*\|/', '', $recipient);
                    $new_recipients[] = $recipient;
                }
                return $new_recipients;
            }
        }
    }

    return NULL;
}

/**
 * Check if a variable is set and return it if so, otherwise the alternative.
 */
function emailtemplates_isset_or(&$val, $alternate = NULL) {
    return isset($val) ? $val : $alternate;
}

/**
 * Return TRUE if a $val is a natural number (integer 1, 2, 3, ...).  Base can
 * be changed to zero if desired.
 */
function emailtemplates_is_natural($val, $base = 1) {
    if (!isset($val)) {
        return FALSE;
    }
    $return = ((string) $val === (string) (int) $val);
    if ($return && intval($val) < $base) {
        $return = FALSE;
    }
    return $return;
}

/**
 * Return formatted list of EmailTemplates recipients for preview display.
 */
function emailtemplates_recipients_formatted($recipients) {

    $output = '';
    if (is_array($recipients)) {
        foreach ($recipients as $recipient) {
            $output .= $recipient['mail'] . ' ';
            $output .= $recipient['uid'] ? t('(user @uid)', array('@uid' => $recipient['uid'])) : t('(no user id)');
            $output .= "\n";
        }
        return $output;
    }
}

/**
 * Check presence of mimemail.
 */
function emailtemplates_has_mimemail() {
    return module_exists('mimemail');
}

/**
 * Form deletion handler.
 *
 * @todo: 'Are you sure?' message.
 */
function emailtemplates_item_delete($id) {
    $msg = '';
    try {
        emailtemplates_delete($id);
        $msg = 'Email template deleted successfully.';
    } catch (Exception $e) {
        $msg = 'Operation Failed.';
    }
    drupal_set_message($msg);
    drupal_goto('admin/structure/emailtemplates/manage');
}
