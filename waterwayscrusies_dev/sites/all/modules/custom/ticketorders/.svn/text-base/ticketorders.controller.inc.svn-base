<?php

class TicketOrdersController extends DrupalDefaultEntityController {

    public function save($ticketorders) {
        if (isset($ticketorders->id) && ($ticketorders->id != null)) {
            try {
                drupal_write_record('ticketorders', $ticketorders, array('id'));
                field_attach_update('ticketorders', $ticketorders);
            } catch (Exception $e) {
                watchdog("ticketorders", "Error while Updating the Ticket Orders");
            }
        } else {
            try {
                drupal_write_record('ticketorders', $ticketorders);
                field_attach_insert('ticketorders', $ticketorders);
            } catch (Exception $e) {
                watchdog("ticketorders", "Error while Inserting the Ticket Orders");
            }
        }
        module_invoke_all('entity_insert', $ticketorders, 'ticketorders');
        return $ticketorders;
    }

    public function delete($id) {
        db_delete('ticketorders')
                ->condition('id', $id, '=')
                ->execute();
    }

}