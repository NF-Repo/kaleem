<?php

class CruisetypesController extends DrupalDefaultEntityController {

    public function save($cruisetypes) {
        if (isset($cruisetypes->id) && ($cruisetypes->id != null)) {
            try {
                drupal_write_record('cruisetypes', $cruisetypes, array('id'));
                field_attach_update('cruisetypes', $cruisetypes);
            } catch (Exception $e) {
                watchdog("cruisetypes", "Error while Updating the Cruise Type");
            }
        } else {
            try {
                drupal_write_record('cruisetypes', $cruisetypes);
                field_attach_insert('cruisetypes', $cruisetypes);
            } catch (Exception $e) {
                watchdog("cruisetypes", "Error while Inserting the Cruise Type");
            }
        }
        module_invoke_all('entity_insert', $cruisetypes, 'cruisetypes');
        return $cruisetypes;
    }

    public function delete($id) {
        db_delete('cruisetypes')
                ->condition('id', $id, '=')
                ->execute();
    }

}