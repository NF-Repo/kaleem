<?php
class _Inventory{
	public function updateInventory(){
		$totalTickets = 0;
		$selectedTickets = unserialize(CartSession::getTickets($sessionid));
		foreach ($selectedTickets as $ticket) {
			if ($ticket['qty']) {
				$totalTickets+=$ticket['qty'];
			}
		}
		
		$totalWindows = 0;
		$enhancementserialized = CartSession::getEnhancements($sessionid);
		
		if ($enhancementserialized) {
			$enhancementproducts = unserialize($enhancementserialized);
		}
		$cnt = 0;
		foreach ($enhancementproducts as $product) {
			if (CruiseEnhancements::is_window_inventory_item($product['enhancedproductnodeid'])) {
				$totalWindows+=$product['qty'];
			}
		}
		
		//Note : updating the inventory
		$cruiseTempateID = $bookingCart->getCartCruiseTemplateId();
		$eventID = $bookingCart->getCartEventId();
		$eventDate =$bookingCart->getCartEventDate();
		
		Scheduledevents::updateInventory($cruiseTempateID, $eventID, $totalTickets, $totalWindows);
		//updating the inventory ends here
		//Note : update the stock information
	}
}