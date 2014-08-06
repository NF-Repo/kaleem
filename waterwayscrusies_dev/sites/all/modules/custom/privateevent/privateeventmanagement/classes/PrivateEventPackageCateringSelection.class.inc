<?php
class PrivateEventPackageCateringSelection{
	
	public static $tableName="private_events_packages_catering_selections";
	public $cateringId;
	public $buffeTitle='';
	public $buffetImageFid;
	public $buffetContent='';

	
	public $packageId='';


	public function createPrivateEventBuffet(){
		$id = db_insert(self::$tableName)
		->fields(array(
				'buffeTitle' => $this->buffeTitle,
				'buffetImageFid' => $this->buffetImageFid,
				'buffetContent' => $this->buffetContent,
				'packageId'=>$this->packageId,

		))
		->execute();
	}


	public function updatePrivateEventBuffet(){
		$num_updated = db_update(self::$tableName)
		->fields(array(
				'buffeTitle' => $this->buffeTitle,
				'buffetImageFid' => $this->buffetImageFid,
				'buffetContent' => $this->buffetContent

		))
		->condition('cateringId', $this->cateringId, '=')
		->execute();
	}

	public static function deletePrivateEventBuffet($cateringId){

		//echo $id; exit();
		$num_updated = db_delete(self::$tableName)
		->condition('cateringId', $cateringId, '=')
		->execute();
	}


	public static function getPrivateEventBuffets($packageId){
		$result = db_select(self::$tableName, 'p')
		->fields('p')
		->condition('packageId', $packageId,'=')
		->execute()->fetchAll();
		return $result;

	}

	public static function getPrivateEventBuffetsByCateringId($cateringId){

		//var_dump($buffet_id); exit();
		$result = db_select(self::$tableName, 'p')
		->fields('p')
		->condition('cateringId', $cateringId,'=')
		->execute()->fetchObject();
		return $result;

	}

}