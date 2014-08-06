<?php

class scheduledeventsController extends DrupalDefaultEntityController {

    public function save($scheduledevents) {
   
//     	return $scheduledevents;
        if (isset($scheduledevents->id) && ($scheduledevents->id != null)) {
            try {
                drupal_write_record('scheduledevents', $scheduledevents, array('id'));
                field_attach_update('scheduledevents', $scheduledevents);
            } catch (Exception $e) {
                watchdog("scheduledevents", "Error while Updating the Scheduled events");
            }
        } else {
            try {
                drupal_write_record('scheduledevents', $scheduledevents);
                field_attach_insert('scheduledevents', $scheduledevents);
            } catch (Exception $e) {
                watchdog("scheduledevents", "Error while Inserting the Scheduled events". $e);
            }
        }
        module_invoke_all('entity_insert', $scheduledevents, 'scheduledevents');
        return $scheduledevents;
    }

    
    
    
    public function delete($id) {
        db_delete('scheduledevents')
                ->condition('id', $id, '=')
                ->execute();
    }

    
    
    /*   get the scheduled event's inventory data by Event Id
     *   @params :  $eventId  
     */

//     public function geteventinventory($eventId) {
        
//         try {
//             $query = db_select('scheduledevents', 's');
//             $query->join('scheduledevent_dates', 'd', 's.id=d.scheduledevent_id');
//             $query->condition('s.id', $eventId, '=');
//             // Adding fields
//             $query->fields('s', array('id', 'ticketinventory', 'availabletickets',
//                 'soldtickets', 'windowseatinventory', 'availablewindowseats','windowseatssold'));
//             $query->fields('d', array('scheduleddate'));
//             $results = $query->execute()
//                     ->fetchAll();
//             $eventinventory = array();
//             foreach ($results as $eventinventorydata) {
//                 $eventinventory[] = $eventinventorydata;
//             }
//             return $eventinventory;
//         } catch (Exception $e) {
//             watchdog("Get inventory list by eventId - scheduledevents.controller.inc", $e);
//         }
//     }
    
    
// }


/*   get the scheduled event's inventory data by Event Id
 *   @params :  $eventId
*/

public function geteventinventory($eventId) {

	
	try {
		$query = db_select('scheduledevents', 's');
		$query->join('scheduledevent_dates', 'd', 's.id=d.scheduledevent_id');
		$query->condition('s.id', $eventId, '=');
		// Adding fields
		$query->fields('s', array('id', 'ticketinventory', 'availabletickets',
				'soldtickets', 'windowseatinventory', 'availablewindowseats','windowseatssold','status'));
		$query->fields('d', array('scheduleddate'));
		$results = $query->execute()
		->fetchAll();
		$eventinventory = array();
		foreach ($results as $eventinventorydata) {
			$eventinventory[] = $eventinventorydata;
		}
		return $eventinventory;
	} catch (Exception $e) {
		watchdog("Get inventory list by eventId - scheduledevents.controller.inc", $e);
	}
}

}

