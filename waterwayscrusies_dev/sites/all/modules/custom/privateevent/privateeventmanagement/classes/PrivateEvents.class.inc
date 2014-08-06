<?php
class PrivateEvents{
	public $eventId;
	public $eventName;
	public $eventNameSpecialText;
	public $menusIds;
	public static $tableName="private_events";
	
	public $status;
	public $widgetImagePath;
	public $widgetHoverText;
	
	
	public function createPrivateEvent(){
		$id = db_insert(self::$tableName)
		->fields(array(
				'eventName'=>$this->eventName,
				'eventNameSpecialText'=>$this->eventNameSpecialText,
				'menusIds' => $this->menusIds,
				'widgetImagePath'=>$this->widgetImagePath,
				'widgetHoverText'=>$this->widgetHoverText,
				'status'=>1
		))
		->execute();
		
		return $id;
		
	}
	
	
	public function updatePrivateEvent(){
			$num_updated = db_update(self::$tableName)
			->fields(array(
					'eventName'=>$this->eventName,
					'eventNameSpecialText'=>$this->eventNameSpecialText,
					'menusIds' => $this->menusIds,
					'widgetImagePath'=>$this->widgetImagePath,
					'widgetHoverText'=>$this->widgetHoverText,
						
			))
			->condition('eventId', $this->eventId, '=')
			->execute();
			
			return $num_updated;
		
	}
	
	public static function getPrivateEventMenus($eventId){
		$result = db_select(self::$tableName, 'p')
		->fields('p')
		->condition('eventId', $eventId,'=')
		->execute()->fetchObject();
		return $result;
		
	}
	
	
	//Getting Event Title
	public static function getPrivateEventTitle($eventId){
		$result = db_select(self::$tableName, 'p')
		->fields('p',array('eventName'))
		->condition('eventId', $eventId,'=')
		->execute()->fetchField();
		return $result;
	
	}
	
	
	
	//Hide and Show
	public static function updatePrivateEventDisplayStatus($eventId,$status){
		$num_updated = db_update(self::$tableName)
		->fields(array(
				'status'=>$status,
	
		))
		->condition('eventId', $eventId, '=')
		->execute();
	
	}
	
	public static function getActivePrivateEvents(){
		$result = db_select(self::$tableName, 'p')
		->fields('p')
		->condition('status',1,'=')
		->execute()->fetchAll();
		return $result;
		
	}
	
	
}