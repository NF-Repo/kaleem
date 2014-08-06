<?php
class PrivateEventMenuList{
	public static $tableName="private_event_menu_list";
	public $privateEventMenuId;
	
	public $eventId;
	public $menuId;
	public $data;
	
	public function createPrivateEventMenuList(){
		try{
			$id = db_insert(self::$tableName)
			->fields(array(
					'eventId' => $this->eventId,
					'menuId' => $this->menuId,
					'data' => $this->data
			
			))
			->execute();
			return $id;
		}catch (Exception $e){
			
		}
		
	}

	public function updatePrivateEventMenuList(){
		try{
			$num_updated = db_update(self::$tableName)
			->fields(array(
					'data' => $this->data
			))
			->condition('privateEventMenuId', $this->privateEventMenuId, '=')
			->execute();
			
		}catch(Exception $e){
			
		}
		
	}


	public static function getPrivateEventMenuList($eventId,$menuId){
		try{
			$result = db_select(self::$tableName, 'p')
			->fields('p')
			->condition('eventId', $eventId,'=')
			->condition('menuId', $menuId,'=')
			->execute()->fetchObject();
			
			return $result;
			
		}catch(Exception $e){
			
		}
	}
	
	
	public static function getPrivateEventMenuNameByMenuId($eventId,$menuId){
		$menu_name='';
		try{
			$result = db_select(self::$tableName, 'p')
			->fields('p')
			->condition('eventId', $eventId,'=')
			->condition('menuId', $menuId,'=')
			->execute()->fetchObject();
			
			if($result){
				$privateeventOverviewlist=unserialize($result->data);
				$menu_name=$privateeventOverviewlist['menu_name'];
			}

			//var_dump($result); exit();
			
			return $menu_name;
				
		}catch(Exception $e){
				
		}
	}





}
