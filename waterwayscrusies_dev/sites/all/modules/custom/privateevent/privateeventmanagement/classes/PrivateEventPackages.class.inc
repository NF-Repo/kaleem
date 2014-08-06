<?php
class PrivateEventPackages{
	
	public static $tableName="private_event_packages";
	public $packageId;
	public $packageTitle;
	public $packageDescription;
	public $eventId;
	public $cateringLayoutStyle;
	public $editorData;
	public $status;
	
	public $packageOptions;
	public $packageInclusions;
	public $downloadPdfFid;
	
	public $widgetImagePath;
	public $widgetHoverText;
	public $widgetSpecialText;
	public $cateringPdfFid;
	
	public $packageCategoryId;
	public $packageStatus;
	
	
	
	public static function getPackagesCategoriesList(){
		$query = db_select('private_event_package_categories', '');
		$query->fields('private_event_package_categories', array('packageCategoryId', 'packageCategoryTitle', 'packageCategorySpecialText', 'packageHoverDescription', 'packageWidgetImage'));
		$query->fields('private_event_packages', array('packageCategoryId','eventId'));
		$query->innerJoin('private_event_packages', '', 'private_event_packages.packageCategoryId = private_event_package_categories.packageCategoryId');
		$query->condition('private_event_packages.packageStatus',1, '=');
		$result = $query->distinct()->execute()->fetchAll();
		return $result;
	}
	
	
	public static function getPackagesCategoriesListByEventId($eventId){
		$query = db_select('private_event_package_categories', '');
		$query->fields('private_event_package_categories', array('packageCategoryId', 'packageCategoryTitle', 'packageCategorySpecialText', 'packageHoverDescription', 'packageWidgetImage'));
		$query->fields('private_event_packages', array('packageCategoryId','eventId'));
		$query->innerJoin('private_event_packages', '', 'private_event_packages.packageCategoryId = private_event_package_categories.packageCategoryId');
		$query->condition('private_event_packages.eventId',$eventId, '=');
		$query->condition('private_event_packages.packageStatus',1, '=');
		$result = $query->distinct()->execute()->fetchAll();
		return $result;
	}
	
	public static function getPackagesListByEventId($eventId){
		$query = db_select("private_event_packages", "p");
		$query->fields('p');
		$query->condition('eventId',$eventId, '=');
		$query->condition('packageStatus',1, '=');
		$result = $query->execute()->fetchAll();
		return $result;
	}
	
	
	//Get Data By Event and Categroy ID
	public static function getPackagesListByEventNPkgCatID($eventId,$packageCategoryId){
		$query = db_select("private_event_packages", "p");
		$query->fields('p');
		$query->condition('eventId',$eventId, '=');
		$query->condition('packageCategoryId',$packageCategoryId, '=');
		$result = $query->execute()->fetchAll();
		return $result;
	}
	
	
	

	public function createPrivateEventPackage(){
		$id = db_insert(self::$tableName)
		->fields(array(
				'packageTitle'=>$this->packageTitle,
				'packageDescription' => $this->packageDescription,
				'eventId'=>$this->eventId,
				'status'=>$this->status,
				'packageStatus'=>1
		))
		->execute();

		return $id;
	}
	
	
	public function updatePrivateEventPackageInfo(){
		$num_updated = db_update(self::$tableName)
		->fields(array(
				'packageTitle'=>$this->packageTitle,
				'packageDescription' => $this->packageDescription,
				'packageOptions'=>$this->packageOptions,
				'packageInclusions'=>$this->packageInclusions,
				'downloadPdfFid'=>$this->downloadPdfFid,
				'widgetImagePath'=>$this->widgetImagePath,
				'widgetHoverText'=>$this->widgetHoverText,
				'widgetSpecialText'=>$this->widgetSpecialText,
				'status'=>1,
				'cateringPdfFid'=>$this->cateringPdfFid,
				'packageCategoryId'=>$this->packageCategoryId,
					
		))
		->condition('packageId', $this->packageId, '=')
		->execute();
		
		return $num_updated;
	}


	//Update Layout Style
	public static function updatePrivateEventBuffetLayout($packageId, $cateringLayoutStyle){
		$num_updated = db_update(self::$tableName)
		->fields(array(
				'cateringLayoutStyle' => $cateringLayoutStyle,
					
		))
		->condition('packageId', $packageId, '=')
		->execute();
	}


	//update Editor Data
	public static function updatePrivateEventBuffetEditorData($packageId, $cateringLayoutStyle, $cateringEditorData){
		$num_updated = db_update(self::$tableName)
		->fields(array(
				'cateringEditorData' => $cateringEditorData,
				'cateringLayoutStyle' => $cateringLayoutStyle,
					
		))
		->condition('packageId', $packageId, '=')
		->execute();
	}


	//Get Price Table Data
	public static function getPrivateEventPackageDataByPackageId($packageId){
		$result = db_select(self::$tableName, 'p')
		->fields('p')
		->condition('packageId', $packageId,'=')
		->execute()->fetchObject();
		return $result;

	}
	
	
	//Getting the Current Package Status
	public static function getPrivateEventPackageStatus($packageId){
		$result = db_select(self::$tableName, 'p')
		->fields('p',array('status'))
		->condition('packageId', $packageId,'=')
		->execute()->fetchField();
		return $result;
	}
	
	
	//Getting Package Title
	public static function getPrivateEventPackageTitle($packageId){
		$result = db_select(self::$tableName, 'p')
		->fields('p',array('packageTitle'))
		->condition('packageId', $packageId,'=')
		->execute()->fetchField();
		return $result;
	}
	
	
	public static function getPrivateEventPackageNEventTitles($packageId){
		$query = db_select('private_events', '');
		$query->fields('private_events', array('eventName'));
		$query->fields('private_event_packages', array('packageTitle'));
		$query->innerJoin('private_event_packages', '', 'private_event_packages.eventId = private_events.eventId');
		$query->condition('private_event_packages.packageId', $packageId,'=');
		$result = $query->execute()->fetchObject();
		
		return $result;
	}
	
	
	public static function deleteUnusedPackages(){
		$result = db_delete(self::$tableName)
		->condition('status', 0)
		->execute();
		return $result;
	}
	
	
	//Hide and Show
	public static function updatePrivateEventPackageDisplayStatus($packageId,$status){
		$num_updated = db_update(self::$tableName)
		->fields(array(
				'packageStatus'=>$status,
	
		))
		->condition('packageId', $packageId, '=')
		->execute();
	
	}
	
	
	


}