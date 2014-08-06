<?php
class _EnhancementsLog{
	public function enhancementsLog($selectedenhancements,$order_id,$eventId){
		foreach ($selectedenhancements as $enhancement) {
			$enhancementObj = new Enhancements();
			$enhancementObj->comments = $enhancement['comments'];
			$enhancementObj->enhancement_id = $enhancement['enhancedproductnodeid'];
			$enhancementObj->enhancement_price = $enhancement['price'];
			$enhancementObj->enhancement_qty = $enhancement['qty'];
			$enhancementObj->event_id = $eventId;
			$enhancementObj->order_id = $order_id;
			BookingLog::logEnhancements($enhancementObj);
		}
	}
}