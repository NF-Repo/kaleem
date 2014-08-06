<?php

/**
 * Validate EmailTemplate form.
 */
function emailtemplates_user_form_validate($form, &$form_state) {

    $step = empty($form_state['storage']['step']) ? 1 : $form_state['storage']['step'];
    $recipients=$form_state['values']['recipients'];
    
    if ($step == 1) {
        $errors = emailtemplates_validate_recipients($form_state, $recipients);
        if (!empty($errors)) {
            form_set_error('recipients', '<ul><li>' . implode('</li><li>', $errors) . '</li></ul>');
        } else {
            // Save recipients to avoid redundant processing on form submit
            $form_state['storage']['recipients'] = $recipients;
        }

        
    }
}

/**
 * Validate existence of a non-empty recipient list free of email errors.
 */
function emailtemplates_validate_recipients($form_state, &$recipients) {

    $errors = array();
    $recipients = array();

    if ($form_state['storage']['recipient_callback']) {
        // Get recipients from callback
        $mails = emailtemplates_callback_recipients($form_state);
        if (!is_array($mails)) {
            $errors[] = t('There is no recipient callback defined for this template or it is not returning an array.');
            return $errors;
        }
    } else {
        // Get recipients from form field
        $mails = emailtemplates_parse_mails($form_state['values']['recipients']);
    }

    // Validate and build recipient array with uid on the fly
    foreach ($mails as $mail) {
        if (!valid_email_address($mail)) {
            $errors[] = t('Invalid email address found: %mail.', array('%mail' => $mail));
        } else {
            $recipients[] = array('mail' => $mail, 'uid' => emailtemplates_lookup_uid($mail));
        }
    }

    // Check for no recipients
    if (empty($errors) && count($recipients) < 1) {
        $errors[] = t('There are no recipients for this email.');
    }

    return $errors;
}

function emailtemplates_parse_mails($mail_text) {
    return preg_split('/[\n\r, ]/', $mail_text, -1, PREG_SPLIT_NO_EMPTY);
}

/**
 * Parse a list of emails and return errors if any.
 */
function emailtemplates_validate_emails($mail_text) {
    $errors = array();

    foreach (emailtemplates_parse_mails($mail_text) as $mail) {
        if (!valid_email_address($mail)) {
            $errors[] = t('Invalid email address found: %mail.', array('%mail' => $mail));
        }
    }

    return $errors;
}
