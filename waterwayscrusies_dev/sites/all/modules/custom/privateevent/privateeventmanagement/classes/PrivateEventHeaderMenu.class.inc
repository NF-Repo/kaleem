<?php
class PrivateEventHeaderMenu{
	
	
	public static function getHeaderMenu($menuids,$eventId,$menuId){
		
		
		
		$eventTitle=PrivateEvents::getPrivateEventTitle($eventId);
		drupal_set_title($eventTitle);
		//echo "test".$menuId; exit();
		
		$overviewClass="btn";
		$yachtsClass="btn";
		$eventPlanningClass="btn";
		$packagesClass="btn";
		//$packagesClass="btn";
		$vendorClass="btn";
		$galleryClass="btn";
		$cateringClass="btn";
		
		$settingsClass='btn';
		if($menuId=="1"){
			$overviewClass="btn btn-primary";
		}
		else if($menuId=="2"){
			$yachtsClass="btn btn-primary";
		}
		else if($menuId=="3"){
			$eventPlanningClass="btn btn-primary";
		}
		else if($menuId=="4"){
			$packagesClass="btn btn-primary";
		}
		else if($menuId=="5"){
			$cateringClass="btn btn-primary";
		}
		else if($menuId=="6"){
			$vendorClass="btn btn-primary";
		}
		else if($menuId=="7"){
			$galleryClass="btn btn-primary";
		}else if($menuId=="0"){
			$settingsClass="btn btn-primary";
		}
		
		
		
		
		
		$html='<div class="row-fluid"><nav class="span12"><ul style="display:block;" class="breadcrumb">';
		global $base_url;
		$html.='<li class="active"><a href="'.$base_url.'/private-events/" class="btn"><span class="icon-home" style="margin-top:4px;"></span>&nbsp;Private Events</a>&nbsp;</li>';
		
		foreach($menuids as $menuId){
			if($menuId=="1"){
				$Overview=true;
				$OverviewData['id']=1;
				$OverviewData['title']=PrivateEventMenus::getPrivateEventMenuNameByMenuId($menuId);
				$html.='<li class="active"><a href="'.$base_url.'/private-event/'.$eventId.'/overview/page" class="'.$overviewClass.'"><span class="icon-edit" style="margin-top:4px;"></span>&nbsp;'.$OverviewData['title'].'</a>&nbsp;</li>';
			}
		
		
			if($menuId=="2"){
				$Yachts=true;
				$YachtsData['id']=1;
				$YachtsData['title']=PrivateEventMenus::getPrivateEventMenuNameByMenuId($menuId);
				$html.='<li class="active"><a href="'.$base_url.'/private-event/'.$eventId.'/yachts/page" class="'.$yachtsClass.'"><span class="icon-flag" style="margin-top:4px;"></span>&nbsp;'.$YachtsData['title'].'</a>&nbsp;</li>';
			}
		
			if($menuId=="3"){
				$Event_Planning=true;
				$Event_PlanningData['id']=1;
				$Event_PlanningData['title']=PrivateEventMenus::getPrivateEventMenuNameByMenuId($menuId);
				$html.='<li class="active"><a href="'.$base_url.'/private-event/'.$eventId.'/eventplanning/page" class="'.$eventPlanningClass.'"><span class="icon-calendar" style="margin-top:4px;"></span>&nbsp;'.$Event_PlanningData['title'].'</a>&nbsp;</li>';
			}
		
		
			if($menuId=="4"){
				$Packages=true;
				$PackagesData['id']=1;
				$PackagesData['title']=PrivateEventMenus::getPrivateEventMenuNameByMenuId($menuId);
				$html.='<li class="active"><a href="'.$base_url.'/private-events/packages/'.$eventId.'/list" class="'.$packagesClass.'"><span class="icon-tags" style="margin-top:4px;"></span>&nbsp;'.$PackagesData['title'].'</a>&nbsp;</li>';
			}
		
			if($menuId=="5"){
				$Catering=true;
				$CateringData['id']=1;
				$CateringData['title']=PrivateEventMenus::getPrivateEventMenuNameByMenuId($menuId);
				$html.='<li class="active"><a href="'.$base_url.'/private-event/'.$eventId.'/catering/page" class="'.$cateringClass.'"><span class="icon-glass" style="margin-top:4px;"></span>&nbsp;'.$CateringData['title'].'</a>&nbsp;</li>';
			}
		
		
			if($menuId=="6"){
				$Vendors=true;
				$VendorsData['id']=1;
				$VendorsData['title']=PrivateEventMenus::getPrivateEventMenuNameByMenuId($menuId); 
				$html.='<li class="active"><a href="'.$base_url.'/private-event/'.$eventId.'/vendors/page" class="'.$vendorClass.'"><span class="icon-list-alt" style="margin-top:4px;"></span>&nbsp;'.$VendorsData['title'].'</a>&nbsp;</li>';
			}
		
		
			if($menuId=="7"){
				$Gallery=true;
				$GalleryData['id']=1;
				$GalleryData['title']=PrivateEventMenus::getPrivateEventMenuNameByMenuId($menuId);
				$html.='<li class="active"><a href="'.$base_url.'/private-event/'.$eventId.'/gallery" class="'.$galleryClass.'"><span class="icon-picture" style="margin-top:4px;"></span>&nbsp;'.$GalleryData['title'].'</a>&nbsp;</li>';
			}
		
			
		}
		$html.='<li class="active"><a href="'.$base_url.'/private-event/'.$eventId.'/manage" class="'.$settingsClass.'"><span class="icon-wrench" style="margin-top:4px;"></span></span>&nbsp;Settings</a>&nbsp;</li></ul></nav></div>';
		
		
		
		return $html;
	}
	
}