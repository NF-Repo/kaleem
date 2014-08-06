<?php
class PrivateEventPackagesHeaderMenu{
	
	
	
	public static function getPackageMenuList($packageId,$currentMenu){
		
		
		$menuTiles=PrivateEventPackages::getPrivateEventPackageNEventTitles($packageId);
		if($menuTiles){
			drupal_set_title($menuTiles->eventName.' > '.$menuTiles->packageTitle);
		}
		
		
		$packagClass="btn";
		if($currentMenu=="packageinformation"){
			$packagClass="btn btn-primary";
		}
		
		
		$sliderClass="btn";
		if($currentMenu=="slider"){
			$sliderClass="btn btn-primary";
		}
		
		$priceTableClass="btn";
		if($currentMenu=="pricetable"){
			$priceTableClass="btn btn-primary";
		}
		
		
		$cateringSelectionClass="btn";
		if($currentMenu=="catering"){
			$cateringSelectionClass="btn btn-primary";
		}
		
		$packageStatus=PrivateEventPackages::getPrivateEventPackageStatus($packageId);
		//var_dump($t); exit();
		
		global $base_url;
		
		$packagesMenuHtml='
		<div class="row-fluid"><nav class="span12"><ul style="display:block;" class="breadcrumb">
		<li class="active">
		<a href="'.$base_url.'/private-events/packages/packages/list/'.$packageId.'" class="btn"><span class="icon-home" style="margin-top:4px;"></span>&nbsp;Packages</a>&nbsp;</li>
		<a href="'.$base_url.'/private-event/package/'.$packageId.'/manage" class="'.$packagClass.'"><span class="icon-info-sign" style="margin-top:4px;"></span>&nbsp;Package Information</a>&nbsp;</li>';
		
		if($packageStatus){
		$packagesMenuHtml.='<a href="'.$base_url.'/private-event/package/'.$packageId.'/sliders" class="'.$sliderClass.'"><span class="icon-picture" style="margin-top:4px;"></span>&nbsp;Sliders</a>&nbsp;</li>
		<a href="'.$base_url.'/private-event/package/'.$packageId.'/pricetable" class="'.$priceTableClass.'"><span class="icon-list-alt" style="margin-top:4px;"></span>&nbsp;Price Table</a>&nbsp;</li>
		<a href="'.$base_url.'/private-event/package/'.$packageId.'/cateringselections" class="'.$cateringSelectionClass.'"><span class="icon-glass" style="margin-top:4px;"></span>&nbsp;Catering Selections</a>&nbsp;</li>';
		}
		
		
		$packagesMenuHtml.='</ul></nav></div>';
		
		
		
		return $packagesMenuHtml;
	
		
		
	}
}