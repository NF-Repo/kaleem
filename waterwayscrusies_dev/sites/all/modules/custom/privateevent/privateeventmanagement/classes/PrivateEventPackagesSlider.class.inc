<?php
class PrivateEventPackagesSlider{
	
	public static $tableName='private_events_packages_sliders';
	public $sliderId='';
	
	public $sliderType='';
	public $imagePath='';
	public $position='';
	
	public $packageId;
	
	
	
	
	
	//Add New Image
public function createSlider(){
		$id = db_insert(self::$tableName)
		->fields(array(
				'sliderType'=>$this->sliderType,
				'imagePath' => $this->imagePath,
				'position'=>$this->position,
				'packageId'=>$this->packageId,
		))
		->execute();

		return $id;
	}
	
	
	//Delete Image
	public static function deleteSlider($sliderId){
		$SliderObject=PrivateEventPackagesSlider::getSliderById($sliderId);
		if(!empty($SliderObject->imagePath)){
			drupal_unlink($SliderObject->imagePath);
		}
		
		$result = db_delete(self::$tableName)
		->condition('sliderId', $sliderId)
		->execute();
		return $result;
		
		
		
		
	}
	
	
	//Get Slider
	public static function getSliders($packageId,$sliderType){
		$result = db_select(self::$tableName, 'p')
		->fields('p')
		->condition('packageId', $packageId,'=')
		->condition('sliderType', $sliderType,'=')
		->execute()->fetchAll();
		return $result;
	}
	
	//get Image Path
	public static function getSliderById($sliderId){
		$result = db_select(self::$tableName, 'p')
		->fields('p')
		->condition('sliderId', $sliderId,'=')
		->execute()->fetchObject();
		return $result;
	}
}