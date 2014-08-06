<?php

class PrivateEventGallery{
	
	
	public static $tableName="private_events_gallery";
	public $galleryId;
	public $galleryType;
	public $galleryData;
	public $position;
	public $eventId;
	
	public function createGallery(){
		$id = db_insert(self::$tableName)
		->fields(array(
				'galleryId'=>$this->galleryId,
				'galleryType' => $this->galleryType,
				'galleryData'=>$this->galleryData,
				'position'=>$this->position,
				'eventId'=>$this->eventId,
		))
		->execute();
		
	}
	
	
	public static function deleteGallery($galleryId){
		$galleryObject=PrivateEventGallery::getGalleryById($galleryId);
		
		if($galleryObject){
			if($galleryObject->galleryType=="image"){
				if(file_exists($galleryObject->galleryData)){
					drupal_unlink($galleryObject->galleryData);
				}
				
			}
		}
		
		$result = db_delete(self::$tableName)
		->condition('galleryId', $galleryId)
		->execute();
		return $result;
		
		
	}
	
	public static function getGalleryListByEventId($eventId){
		$result = db_select(self::$tableName, 'p')
		->fields('p')
		->condition('eventId', $eventId,'=')
		->execute()->fetchAll();
		return $result;
		
	}
	
	
	public static function getGalleryById($galleryId){
		
		$result = db_select(self::$tableName, 'p')
		->fields('p')
		->condition('galleryId', $galleryId,'=')
		->execute()->fetchObject();
		return $result;
		
	}
}