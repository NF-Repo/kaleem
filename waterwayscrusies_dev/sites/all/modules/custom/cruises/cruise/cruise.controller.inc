<?php

class CruiseController extends DrupalDefaultEntityController {

    public function save($cruise) {
        if (isset($cruise->id) && ($cruise->id != null)) {
            try {
                drupal_write_record('cruise', $cruise, array('id'));
                field_attach_update('cruise', $cruise);
            } catch (Exception $e) {
                watchdog("cruise", "Error while Updating the Cruise");
            }
        } else {
            try {
                drupal_write_record('cruise', $cruise);
                field_attach_insert('cruise', $cruise);
            } catch (Exception $e) {
                watchdog("cruise", "Error while Inserting the Cruise");
            }
        }
        module_invoke_all('entity_insert', $cruise, 'cruise');
        return $cruise;
    }

    public function delete($id) {
        db_delete('cruise')
                ->condition('id', $id, '=')
                ->execute();
    }

}