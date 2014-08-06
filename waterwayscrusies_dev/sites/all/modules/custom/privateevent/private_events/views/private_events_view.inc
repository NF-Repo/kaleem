<?php


function private_events_category($eventId){
	
	//var_dump($eventId); exit();
	$eventTitle=PrivateEvents::getPrivateEventTitle($eventId);
	$tabedmenu = getcategWeddingMenu($eventId);
	$categoryresources = array(
// 			'weddingtabmenu' => $tabedmenu['eventMenuHeaderTab'],
// 			'mobilemenu' => $tabedmenu['mobilemenuhtml'],
// 			'eventscategorytitle' => $tabedmenu['eventtitle'],
// 			'gallery' => $tabedmenu['gallery'],
// 			'shortdescription' => $tabedmenu['eventdescription'],
// 			'overviewlink' => $tabedmenu['overviewlink'],
// 			'categoryslider' => $tabedmenu['bottomslider'],
// 			'masterpagetitle' => $tabedmenu['mastertitle'],
			
			'eventWebMenuHeaderTab' => $tabedmenu ['eventWebMenuHeaderTab'],
			'mobilemenu' => $tabedmenu ['mobilemenuhtml'],
			'widgetTitle' => $tabedmenu ['widgetTitle'],
			'widgetDescription' => $tabedmenu ['widgetDescription'],
			//'overviewlink' => $tabedmenu ['overviewlink'],
			'eventTitle' => $eventTitle,
			'backgroundimage' => $tabedmenu ['backgroundimage'],
			'widget_button_name' => $tabedmenu ['widget_button_name'],
			'widget_button_url' => $tabedmenu ['widget_button_url'],
			'gallery' => $tabedmenu ['gallery'],
			'categoryslider' => $tabedmenu['bottomslider'],
	);
	
	//var_dump($categoryresources); exit();
	return theme('private_events_Category', $categoryresources);
}

//private events categories
function  getcategWeddingMenu($privateEventId){
	
	//var_dump($privateEventId); exit();

	 
	
	
	
	$webmenuhtml = '';
	$mobilemenuhtml = '';
	$gallery = '';
	
	
	

	global $base_url;
	$privateEventsList = PrivateEvents::getPrivateEventMenus ( $privateEventId );
	
	
	
	$defaultMenuId = '';
	if(isset($_GET['menuid'])){
		$defaultMenuId= $_GET['menuid'];
		//unset($_GET['menuid']);
	}
	
	
	//Header Menu
	$menusIds = explode ( ',', $privateEventsList->menusIds );
	$webmenuhtml .= '<ul>';
	foreach ( $menusIds as $menuId ) {
		//Hide Packages
		if($menuId!=4){
			if (empty ( $defaultMenuId )) {
				$defaultMenuId = $menuId;
			}
			if($defaultMenuId==$menuId){
				$customeMenuName=PrivateEventMenuList::getPrivateEventMenuNameByMenuId($privateEventId, $menuId);
				$mnuName=!empty($customeMenuName) ? $customeMenuName: PrivateEventMenus::getPrivateEventMenuNameByMenuId ( $menuId );
				$webmenuhtml .= '<li><a class="active" href="#">'.  $mnuName. '</a></li>';
				$mobilemenuhtml.="<option value='' selected='selected'>" .$mnuName. "</option>";
			}else{
				$path=$base_url.'/private_events/category/'.$privateEventId.'?menuid='.$menuId;
				$customeMenuName=PrivateEventMenuList::getPrivateEventMenuNameByMenuId($privateEventId, $menuId);
				$menuName=!empty($customeMenuName) ? $customeMenuName: PrivateEventMenus::getPrivateEventMenuNameByMenuId ( $menuId );
				$webmenuhtml .= '<li><a class="" href="'.$path.'">' .$menuName . '</a></li>';
				$mobilemenuhtml.='<option value="' . $path. '" >' . $menuName. '</option>';
			}
		}
		
		
	}
	$webmenuhtml.='</ul>';
	
	//Widget Data
	$widgetTitle ="";
	$widgetDescription = "";
	$backgroundimage = "";
	$widget_button_name="";
	$widget_button_url="";
	
	if($defaultMenuId==1 || $defaultMenuId==2|| $defaultMenuId==3 || $defaultMenuId==5 || $defaultMenuId==6 ) {
		$defaultMenudata = PrivateEventMenuList::getPrivateEventMenuList ( $privateEventId, $defaultMenuId );
		if ($defaultMenudata) {
			$menuData = unserialize ( $defaultMenudata->data );
			$widgetTitle = $menuData ['widget_title'];
			$widgetDescription = $menuData ['widget_description'];
			$backgroundimage = !empty($menuData ['background_img_path']) ? $menuData ['background_img_path'] :$backgroundimage;
			$widget_button_name=$menuData ['widget_button_name'];
			$widget_button_url=$menuData ['widget_button_url'];
		}
	}
	
	//Gallery Data
	$gallery='';
	$videoGallery='';
	if($defaultMenuId==7){
		$backgroundimage="public://privateevents_media/overview/defaultlightbackground.png";
		$galleryData=PrivateEventGallery::getGalleryListByEventId($privateEventId);
		foreach($galleryData as $galleryObject){
			if($galleryObject->galleryType=="image"){
				if(!empty($galleryObject->galleryData)){
					$imgsrc = file_create_url($galleryObject->galleryData);
					$gallery.='<li><div class="sliderBox"><div class="sliderBoxout">';
					$gallery.='<a  class="mfp-image image-link" href="' . $imgsrc . '" rel="Title photoset">
                  <img src="' . $imgsrc . '" alt="placeholder">
                 </a>';
					$gallery.='</div></div></li>';
				}
	
			}
				
			if($galleryObject->galleryType=="video"){
				if(!empty($galleryObject->galleryData)){
					$videourl = file_create_url($galleryObject->galleryData);
					$icon = 'public://videoicon.jpg';
					$videoGallery.='<li><div class="sliderBox"><div class="sliderBoxout">';
					$videoGallery.='<a  class="mfp-iframe image-link" href="' . $videourl . '" rel="Title photoset">
                  <img src="' . file_create_url($icon) . '" alt="placeholder">
                 </a>';
					$videoGallery.='</div></div></li>';
				}
	
			}
		}
		//var_dump($galleryData); exit();
	}
	
	if(empty($backgroundimage)){
		$backgroundimage="public://privateevents_media/overview/defaultbackground.png";
	}
	
	

	 
	 
	 
	
	return array(
			'eventWebMenuHeaderTab' => $webmenuhtml,
			'mobilemenuhtml' => $mobilemenuhtml,
			'widgetTitle' => $widgetTitle,
			'widgetDescription' => $widgetDescription,
			'backgroundimage' => $backgroundimage,
			'widget_button_name'=>$widget_button_name,
			'widget_button_url'=>$widget_button_url,
			'gallery' => $gallery.''.$videoGallery,
			'bottomslider' => getEventPackageSliderList($privateEventId),
	);
}
//end of private events categorie
















function getEventPackageSliderList($privateEventId) {

	
	$packageCategoriesList=PrivateEventPackages::getPackagesCategoriesList();
	
	$html='';
	
	foreach($packageCategoriesList as $packageCategory){
		
		//var_dump($privateEventId); exit();
		if($packageCategory->eventId==$privateEventId){
			$backgroundimage="";
			$html .= '<li>';
			$html .= '<div class="sliderBox">';
			$imagesrc = file_create_url ($packageCategory->packageWidgetImage);
			$html .= '<div class="sliderBoxout" style="background:url(' . $imagesrc . ') no-repeat;background-size:cover;">';
			
			$html .= '<div class="event-bgimage">';
			$html .= '</div>';
			$html .= '<div class="sliderBoxInner">'.$packageCategory->packageCategoryTitle.'<br/><span>'.$packageCategory->packageCategorySpecialText.'</span></div>';
			$html .= '</div>';
			
			$html .= '<div class="sliderBoxover" onclick="packageDetailsView('.$privateEventId.','.$packageCategory->packageCategoryId.')" style="display: none;">';
			$html .= '<div class="sliderBox-first-inner">';
			$html .= '<img src="' . base_path () . 'sites/all/themes/waterways/assets/img/innerbox-top-img.png"><br>'.$packageCategory->packageHoverDescription.'<br>
                                  <a>Click to Learn More</a><div class="line"></div>';
			$html .= '</div>';
			
			$html .= '</div></div></li>';
		}
		
	}
	
	
	
	//var_dump($data); exit();
	//$data = getPrivateevents ();

	return $html;
}
