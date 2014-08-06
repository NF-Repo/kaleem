
<?php

/**
 * EntityCouponnterface definition.
 *
 * We create an interface here because anyone could come along and
 * use hook_entity_info_alter() to change our controller class.
 * We want to let them know what methods our class needs in order
 * to function with the rest of the module, so here's a handy list.
 *
 * @see hook_entity_info_alter()
 */
interface EntityEmailTemplatesControllerInterface extends DrupalEntityControllerInterface {

    public function create();

    public function save($emailtemplates);

    public function delete($emailtemplates);
}

/**
 * EntityExampleBasicController extends DrupalDefaultEntityController.
 *
 * Our subclass of DrupalDefaultEntityController lets us add a few
 * important create, update, and delete methods.
 */
class EntityEmailTemplatesController extends DrupalDefaultEntityController implements EntityEmailTemplatesControllerInterface {

    /**
     * Create and return a new coupon entity.
     */
    public function create() {
        $emailtemplates = new stdClass();
        $emailtemplates->name = '';
        $emailtemplates->subject = '';
        $emailtemplates->mail_body = '';
       


        return $emailtemplates;
    }

    /**
     * Saves the custom fields using drupal_write_record()
     */
    public function save($emailtemplates) {

        // If our entity has no id, then we need to give it a
        // time of creation.
       
        if (isset($emailtemplates->id) && ($emailtemplates->id != null)) {
            try {

                drupal_write_record('emailtemplates', $emailtemplates, array('id'));
                field_attach_update('emailtemplates', $emailtemplates);
            } catch (Exception $e) {
                watchdog("emailtemplates", "Error while Updating emailtemplates");
            }
        } else {
            try {
                drupal_write_record('emailtemplates', $emailtemplates);
                field_attach_insert('emailtemplates', $emailtemplates);
            } catch (Exception $e) {
                watchdog("emailtemplates", "Error while Inserting the emailtemplates");
            }
        }
        module_invoke_all('entity_insert', $emailtemplates, 'emailtemplates');
        return $emailtemplates;
    }

    /**
     * Delete a single entity.
     *
     * Really a convenience function for delete_multiple().
     */
    public function delete($emailtemplates) {
        db_delete('emailtemplates')
                ->condition('id', $emailtemplates, '=')
                ->execute();
    }

    /**
     * Delete one or more emailtemplates entities.
     *
     * Deletion is unfortunately not supported in the base
     * DrupalDefaultEntityController class.
     *
     * @param $ids
     *   An array of entity IDs or a single numeric ID.
     */
    public function delete_multiple($entities) {
        $ids = array();
        if (!empty($entities)) {
            $transaction = db_transaction();
            try {
                foreach ($entities as $emailtemplates) {
                    // Invoke hook_entity_delete().
                    module_invoke_all('entity_delete', $emailtemplates, 'emailtemplates');
                    field_attach_delete('emailtemplates', $emailtemplates);
                    $ids[] = $emailtemplates->id;
                }
                db_delete('emailtemplates')
                        ->condition('id', $ids, 'IN')
                        ->execute();
            } catch (Exception $e) {
                $transaction->rollback();
                watchdog_exception('emailtemplates', $e);
                throw $e;
            }
        }
    }

   
   

    }

/**
 * @} End of "defgroup emailtemplates".
 */
