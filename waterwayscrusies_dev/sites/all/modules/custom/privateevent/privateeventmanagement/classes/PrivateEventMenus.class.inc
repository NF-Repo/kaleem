<?php

/*
 * class Private Event Menus (Overview, Yachts, Gallery , Packages)
 */
class PrivateEventMenus{
	public $menuId;
	public $menuName;
	public static $tableName="private_event_menu";

	public static function getPrivateEventMenuList(){
		try{
			$result = db_select(self::$tableName, 'p')
			->fields('p')
			->execute()->fetchAll();
		}catch(Exception $e){
			drupal_set_message(t('db_select failed. Message = %message, query= %query',array('%message' => $e->getMessage(), '%query' => $e->query_string)), 'error');
		}
		return $result;
	}


	public static function getPrivateEventMenuNameByMenuId($menuId){
		try{
			$result = db_select(self::$tableName, 'p')
			->fields('p',array('menuName'))
			->condition('menuId', $menuId,'=')
			->execute()->fetchField();
			
			if($result){
				return $result;
				
			}else{
				throw new Exception('No Data Found');
			}
			
			//var_dump($result); exit();
		}catch(Exception $e){
			drupal_set_message( $e->getMessage());
			watchdog('Private Event Menus', $e);
		}
		
	}
        
        public static function getPrivateEventMenuIdByMenuName($menuname){
            try{
			$result = db_select(self::$tableName, 'p')
			->fields('p',array('menuId'))
			->condition('menuname', $menuname,'=')
			->execute()->fetchField();
			
			if($result){
				return $result;
				
			}else{
				throw new Exception('No Data Found');
			}
			
			//var_dump($result); exit();
		}catch(Exception $e){
			drupal_set_message( $e->getMessage());
			watchdog('Private Event Menus', $e);
		}
        }
}