<?php
function private_events_list_page() {
	
	$privateEventCategories=PrivateEvents::getActivePrivateEvents();
	//$packageCategoriesList=PrivateEventPackages::getPackagesCategoriesList();
	
	global $base_url;
	$html='';
	
	
	foreach($privateEventCategories as $privateEventCategory){
		$path='';
		$urlsource = "private_events/category/" . $privateEventCategory->eventId;
		$urlAliasObject = UrlAlias::getUrlAliasDetailsBySource ( $urlsource );
		if($urlAliasObject){
			$path=$base_url.'/'.$urlAliasObject->alias;
		}else{
			$path=$base_url.'/private_events/category/'.$privateEventCategory->eventId;
		}
		
			
		$html.='<div class="row-fluid privateEventTitle">';
		$html.='<div class="span12">';
		$html.='<div class="eventTitle" id="WEDDINGS">
        <a href="' . $path.'">' . $privateEventCategory->eventName . '</a></div>';
		$html.='</div>';
		$html.='</div>';
		
		
		$packageCategoriesList=PrivateEventPackages::getPackagesCategoriesListByEventId($privateEventCategory->eventId);
		if($packageCategoriesList){
			$html.='<div class="row-fluid tiles">';
			$html.='<ul class="weddingcategory">';
			foreach($packageCategoriesList as $packageCategory){
				if($packageCategory->eventId==$privateEventCategory->eventId){
					$html .= '<li>';
					$html .= '<div class="sliderBox">';
					$imagesrc = file_create_url ($packageCategory->packageWidgetImage);
					$html .= '<div class="sliderBoxout" style="background:url(' . $imagesrc . ') no-repeat;background-size:cover;">';
					$html .= '<div class="event-bgimage">';
					$html .= '</div>';
					$html .= '<div class="sliderBoxInner"> '.$packageCategory->packageCategoryTitle.'<br/><span>'.$packageCategory->packageCategorySpecialText.'</span></div>';
					$html .= '</div>';
						
					$html .= '<div class="sliderBoxover" onclick="packageDetailsView('.$privateEventCategory->eventId.','.$packageCategory->packageCategoryId.')" style="display: none;">';
					$html .= '<div class="sliderBox-first-inner">';
					$html .= '<img src="' . base_path () . 'sites/all/themes/waterways/assets/img/innerbox-top-img.png"><br>'.$packageCategory->packageHoverDescription.'<br>
			                                  <a>Click to Learn More</a><div class="line"></div>';
					$html .= '</div>';
						
					$html .= '</div></div></li>';
				}
			}
			
			$html.='</ul>';
			$html.='</div>';
		}
	}
	
	return theme('private_events', array('loadprivatecontent' => $html));
}



