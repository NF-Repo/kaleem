<?php

class TicketOrderCommentsController extends DrupalDefaultEntityController {
    public function save($ticketordercomments) {
        if (isset($ticketordercomments->id) && ($ticketordercomments->id != null)) {
            try {
                drupal_write_record('ticketordercomments', $ticketordercomments, array('id'));
                field_attach_update('ticketordercomments', $ticketordercomments);
            } catch (Exception $e) {
                watchdog("ticketordercomments", "Error while Updating the Ticket Order Comments");
            }
        } else {
            try {
                drupal_write_record('ticketordercomments', $ticketordercomments);
                field_attach_insert('ticketordercomments', $ticketordercomments);
            } catch (Exception $e) {
                watchdog("ticketordercomments", "Error while Inserting the Ticket Order Comments");
            }
        }
        module_invoke_all('entity_insert', $ticketordercomments, 'ticketordercomments');
        return $ticketordercomments;
    }

    public function delete($id) {
        db_delete('ticketordercomments')
                ->condition('id', $id, '=')
                ->execute();
    }

}