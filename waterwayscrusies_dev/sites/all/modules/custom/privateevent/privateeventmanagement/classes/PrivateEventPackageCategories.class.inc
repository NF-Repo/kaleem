<?php
class PrivateEventPackageCategories{
	public $packageCategoryId;
	public $packageCategoryTitle;
	public $packageCategorySpecialText;
	public $packageHoverDescription;
	public $packageWidgetImage;
	public $status;
	
	public static $tableName="private_event_package_categories";
	
	
	//Insert
	public function createPrivateEventPackageCategory(){
		$id = db_insert(self::$tableName)
		->fields(array(
				'packageCategoryTitle'=>$this->packageCategoryTitle,
				'packageCategorySpecialText'=>$this->packageCategorySpecialText,
				'packageHoverDescription' => $this->packageHoverDescription,
				'packageWidgetImage'=>$this->packageWidgetImage,
		))
		->execute();
		return $id;
	
	}
	
	//Select
	public static function getPrivateEventPackageCategoryDetailsById($packageCategoryId){
		$result = db_select(self::$tableName, 'p')
		->fields('p')
		->condition('packageCategoryId', $packageCategoryId,'=')
		->execute()->fetchObject();
		
		
		return $result;
	
	}
	
	public static function getPrivateEventPackageCategories(){
		$result = db_select(self::$tableName, 'p')
		->fields('p')
		->execute()->fetchAll();
		return $result;
	
	}
	
	
	//Update
	public function updatePrivateEventPackageCategory(){
		$num_updated = db_update(self::$tableName)
		->fields(array(
				'packageCategoryTitle'=>$this->packageCategoryTitle,
				'packageCategorySpecialText'=>$this->packageCategorySpecialText,
				'packageHoverDescription' => $this->packageHoverDescription,
				'packageWidgetImage'=>$this->packageWidgetImage,
	
		))
		->condition('packageCategoryId', $this->packageCategoryId, '=')
		->execute();
		
		return $num_updated;

	}
	
	
}