<?php

class TicketOrdersEntreesController extends DrupalDefaultEntityController {

    public function save($ticketorderentrees) {
        if (isset($ticketorderentrees->id) && ($ticketorderentrees->id != null)) {
            try {
                drupal_write_record('ticketorderentrees', $ticketorderentrees, array('id'));
                field_attach_update('ticketorderentrees', $ticketorderentrees);
            } catch (Exception $e) {
                watchdog("ticketorderentrees", "Error while Updating the Ticket Order Entrees");
            }
        } else {
            try {
                drupal_write_record('ticketorderentrees', $ticketorderentrees);
                field_attach_insert('ticketorderentrees', $ticketorderentrees);
            } catch (Exception $e) {
                watchdog("ticketorderentrees", "Error while Inserting the Ticket Order Entrees");
            }
        }
        module_invoke_all('entity_insert', $ticketorderentrees, 'ticketorderentrees');
        return $ticketorderentrees;
    }

    public function delete($id) {
        db_delete('ticketorderentrees')
                ->condition('id', $id, '=')
                ->execute();
    }

}