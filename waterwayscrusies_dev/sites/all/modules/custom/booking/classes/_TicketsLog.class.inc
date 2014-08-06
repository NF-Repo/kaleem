<?php
class _TicketsLog{
	
	public function logTickets($ticketsList,$order_id,$eventId){
		
		
	foreach ($ticketsList as $ticket) {
			$ticketObj = new Tickets();
			$ticketObj->ticket_id = $ticket['ticketid'];
			$ticketObj->order_id = $order_id;
			$ticketObj->ticket_qty = $ticket['qty'];
			$ticketObj->ticket_price = $ticket['price'];
			if(!empty($ticket['grouponcode'])){
				$ticketObj->ticket_price=0;
			}
			
			$ticketObj->event_id = $eventId;
			$ticketObj->bookingtype = "guestuser";
			$id = BookingLog::logTickets($ticketObj);
			
			if(isset($ticket['entree']) && !empty($ticket['entree'])){
				$ticketEntreesObj = new TicketEntrees();
				$ticketEntreesObj->entree_id = (int) $ticket['entree'];
				$ticketEntreesObj->ref_id =(int) $id;
				$ticketEntreesObj->comments = isset($ticket['comments']) ? $ticket['comments'] : '';
				BookingLog::logTicketEntrees($ticketEntreesObj);
			}
		}
	}
}