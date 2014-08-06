<?php
class PrivateEventPackagesPriceTable{
	public static $tableName="private_events_packages_pricetable";

	public $priceTableId;
	public $category_id='';


	public $layoutStyle;
	public $fixedLayoutData;
	public $editorData;
	public $priceTableDesciption;

	//Create Price Table
	public function createPrivateEventPriceTable(){
		$id = db_insert(self::$tableName)
		->fields(array(
				'packageId'=>$this->packageId,
				'layoutStyle' => $this->layoutStyle,
				'editorData' => $this->editorData,
				'fixedLayoutData' => $this->fixedLayoutData,
				'priceTableDesciption'=>$this->priceTableDesciption,
		))
		->execute();
	}

	public function updatePrivateEventPriceTableEditor(){
		//update
		$num_updated = db_update(self::$tableName)
		->fields(array(
				'layoutStyle' => $this->layoutStyle,
				'editorData' => $this->editorData,
				'priceTableDesciption'=>$this->priceTableDesciption,
				//'fixedLayoutData' => $this->fixedLayoutData,
		))
		->condition('priceTableId', $this->priceTableId, '=')
		->execute();
	}

	public function updatePrivateEventPriceTableFixedLayout(){
		//update
		$num_updated = db_update(self::$tableName)
		->fields(array(
				'layoutStyle' => $this->layoutStyle,
				//'editorData' => $this->editorData,
				'fixedLayoutData' => $this->fixedLayoutData,
				'priceTableDesciption'=>$this->priceTableDesciption,
		))
		->condition('priceTableId', $this->priceTableId, '=')
		->execute();
	}




	//Get Price Table Data
	public static function getPrivateEventPriceTableDataByPackageId($packageId){
		$result = db_select(self::$tableName, 'p')
		->fields('p')
		->condition('packageId', $packageId,'=')
		->execute()->fetchObject();
		return $result;

	}


}