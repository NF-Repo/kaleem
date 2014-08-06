<?php
function private_events_packages($eventId,$packageCategoryId){
	
	
	//echo $eventId;
	//echo $packageCategoryId; exit();
	
	
	
	

	try {
		global $user;


		$pagedetails = getPackagesMenu($eventId, $packageCategoryId);

		$cruiseresouces = array(
				'deskTopMenu' => $pagedetails['deskTopMenu'],
				'mobileMenu' => $pagedetails['mobileMenu'],
				'packageParentName' => $pagedetails['packageParentName'],
				'packageTitle' => $pagedetails['packageTitle'],
				'packageDecription' => $pagedetails['packageDecription'],
				'rightslider' => $pagedetails['rightslider'],
				'middleslider' => $pagedetails['middleslider'],
				'cateringselectionmenu' => $pagedetails['cateringselectionmenu'],
				'downloadpackagelink' => $pagedetails['downloadpackagelink'],
				'downloadmenulink' => $pagedetails['downloadmenulink'],
				'packageinclusion' => $pagedetails["packageinclusionscontent"],
				'packageoption' => $pagedetails["packageoptionscontent"],
				'deskTopPriceTable' => $pagedetails['deskTopPriceTable'],
				'mobilePriceTable' => $pagedetails['mobilePriceTable'],
				'priceTableDescription'=>$pagedetails["priceTableDescription"],
				'socialMediasharePath'=>$pagedetails["socialMediasharePath"],
		);

		return theme('private_events_packages', $cruiseresouces);
	} catch (Exception $ex) {

	}
}


function getPackagesMenu($eventId, $packageCategoryId) {
	
	
	$defaultPackageId='';
	if(isset($_GET['defaultPackageId'])){
		$defaultPackageId=$_GET['defaultPackageId'];
	}
	
	global $base_url;
	
	$path=$base_url.'/private_events/packages/'.$eventId.'/'.$packageCategoryId;
	
// 	$deskTopMenu = '';
// 	$deskTopMenu.='<ul>';
// 	$mobileMenu='';
// 	$deskTopMenu.='';
// 	$deskTopMenu.='<li><a href="#" class="active">CELEBRATE ROMANCE</a></li>';
// 	$deskTopMenu.='<li><a href="#" >FOREVER MEMORIES</a></li>';
// 	$deskTopMenu.='<li><a href="#"> ENCHANTED EVENING</a></li>';
// 	$deskTopMenu.='<li><a href="#">ENCORE EXTRAORDINAIRE</a></li>';
// 	$deskTopMenu.='<li><a href="#"> BON VOYAGE</li>';
// 	$mobileMenu.="<option value='#' selected='selected'></option>";
// 	$deskTopMenu.='</ul>';
// 	$packageTitle='SUB TITLE ';
	
	$packagesListdata=PrivateEventPackages::getPackagesListByEventNPkgCatID($eventId, $packageCategoryId);
	if($packagesListdata){
		$deskTopMenu = '';
		$deskTopMenu.='<ul>';
		$mobileMenu='';
		foreach($packagesListdata as $package){
			if(empty($defaultPackageId)){
				$defaultPackageId=$package->packageId;
			}
			
			if($defaultPackageId==$package->packageId){
				$deskTopMenu.='<li><a href="#" class="active">'.$package->packageTitle.'</a></li>';
				$mobileMenu.="<option value='' selected='selected'>".$package->packageTitle."</option>";
			}else{
				$deskTopMenu.='<li><a href="'.$path.'?defaultPackageId='.$package->packageId.'" class="">'.$package->packageTitle.'</a></li>';
				$mobileMenu.='<option value="'.$path.'?defaultPackageId='.$package->packageId.'">'.$package->packageTitle.'</option>';
			}
		}
		$deskTopMenu.='</ul>';
		
	}

	$packageinclusions='';
	$packageoptions='';
	$deskTopPriceTable='';
	$cateringselectionmenu='';

	$rightslider='';
	$middleslider='';
	$packageTitle='';
	$packageDecription='';
	$downloadPdfUrl='';
	$cateringPdfUrl='';
	//Based on $defaultPackageId Get The Package Data
	if(!empty($defaultPackageId)){
		$privateEventPackagesData=PrivateEventPackages::getPrivateEventPackageDataByPackageId($defaultPackageId);
		
		if($privateEventPackagesData){
			
			//var_dump($privateEventPackagesData);exit();
			$packageTitle=$privateEventPackagesData->packageTitle;
			$cateringLayoutStyle=$privateEventPackagesData->cateringLayoutStyle;
			$cateringEditorData=$privateEventPackagesData->cateringEditorData;
			
			//Catering PDF
			$cateringPdfFid=file_load($privateEventPackagesData->cateringPdfFid);
                       
			if(!empty($cateringPdfFid)){
				$displaydownloadpdf=$cateringPdfFid->uri;
				$downloadPdfUrl=file_create_url($cateringPdfFid->uri);
			}
			
			
			//Packages PDF
			$downloadPdfFid=file_load($privateEventPackagesData->downloadPdfFid);
			if(!empty($downloadPdfFid)){
				$displaydownloadcatpdf=$downloadPdfFid->uri;
				$cateringPdfUrl=file_create_url($downloadPdfFid->uri);
			}
			
			
			$packageinclusions = $privateEventPackagesData->packageInclusions;
			$packageoptions = $privateEventPackagesData->packageOptions;
			
			
			
			//PRICE TABLE
			$privateEventPackagePriceTableObject=PrivateEventPackagesPriceTable::getPrivateEventPriceTableDataByPackageId($defaultPackageId);
			$priceTableDesciption=$privateEventPackagePriceTableObject->priceTableDesciption;
			if($privateEventPackagePriceTableObject){
				
				if($privateEventPackagePriceTableObject->layoutStyle=="editor"){
					$deskTopPriceTable=$privateEventPackagePriceTableObject->editorData;
					$mobilePriceTable=$privateEventPackagePriceTableObject->editorData;
					//echo $deskTopPriceTable; exit();
				}
					
				else if($privateEventPackagePriceTableObject->layoutStyle=="fixed_layout"){
					$priceTableList=unserialize($privateEventPackagePriceTableObject->fixedLayoutData);
					//Web
					$deskTopPriceTable = ' ';
					$deskTopPriceTable.='<table class="sticky-enabled">';
					$deskTopPriceTable.='<thead>';
					$deskTopPriceTable.='<tr>';
					$deskTopPriceTable.='<th>TIME</th>';
					$deskTopPriceTable.='<th>40-60 GUESTS</th>';
					$deskTopPriceTable.='<th>61-90 GUESTS</th>';
					$deskTopPriceTable.='<th>91-130 GUESTS</th>';
					$deskTopPriceTable.='</tr>';
					$deskTopPriceTable.='</thead>';
					$deskTopPriceTable.='<tbody>';
					foreach ($priceTableList as $pricetableresults) {
						
						if(!empty($pricetableresults->time)){
							$deskTopPriceTable.='<tr class="odd">';
							$deskTopPriceTable.='<td><b>' . $pricetableresults->time . '</b><br>' . $pricetableresults->boarding . '</td>';
							$deskTopPriceTable.='<td>$' . $pricetableresults->guest4060 . '</td>';
							$deskTopPriceTable.='<td>$' . $pricetableresults->guest6190 . '</td>';
							$deskTopPriceTable.='<td>$' . $pricetableresults->guest91130 . '</td>';
							$deskTopPriceTable.='</tr>';
						}
						
					}
					$deskTopPriceTable.='</tbody>';
					$deskTopPriceTable.='</table>';
					
					
					
					//Mobile
					$mobilePriceTable = '';
					$mobilePriceTable.='<table>';
					$html4060 = '<tr><td class="pricelistheader"><b>TIME</b></td><td class="pricelistheader2"><b>40-60 GUESTS</b></td></tr>';
					$html6090 = '<tr><td class="pricelistheader"></td><td class="pricelistheader2"><b>61-90 GUESTS</b></td></tr>';
					$html90130 = '<tr><td class="pricelistheader"></td><td class="pricelistheader2"><b>91-130 GUESTS</b></td></tr>';
					foreach ($priceTableList as $pricetableresults) {
						
						if(!empty($pricetableresults->time)){
							$html4060.='<tr><td width="50%"><b>' . $pricetableresults->time . '</b><br>' . $pricetableresults->boarding . '</td><td class="pricelist">$' . $pricetableresults->guest4060 . '</td></tr>';
							$html6090.='<tr><td width="50%"><b>' . $pricetableresults->time . '</b><br>' . $pricetableresults->boarding . '</td><td  class="pricelist">$' . $pricetableresults->guest6190  . '</td></tr>';
							$html90130.='<tr><td width="50%"><b>' . $pricetableresults->time. '</b><br>' . $pricetableresults->boarding . '</td><td  class="pricelist">$' . $pricetableresults->guest91130 . '</td></tr>';
						}
					}
					$mobilePriceTable.=$html4060 . $html6090 . $html90130;
					$mobilePriceTable.='</tbody>';
					$mobilePriceTable.='</table>';
					
					
				}
			}//Price Table 
			
			
			//CATERING SELECTIONS
			if($cateringLayoutStyle=="fixed_layout"){
				$privateEventCateringData=PrivateEventPackageCateringSelection::getPrivateEventBuffets($defaultPackageId);
				foreach($privateEventCateringData as $cateringData){
					$file=file_load($cateringData->buffetImageFid);
					$cateringImage='';
					if($file){
						if(file_exists($file->uri)){
							$filePath=file_create_url($file->uri);
							$cateringImage='<img src="'.$filePath.'">';
						}
					}
					
					
					
					
					$cateringselectionmenu.='
                           <div id="cateringSelectionMenu">
                            <ul id="privatecatering" class="cateringSelectionMenu">
                            <li>
                            <div class="weddingmenuItem">
                            <div class="menuItemTitle">'.$cateringData->buffeTitle.'<br></div>
                            <div class="menuItemthirdinner"><div style="max-width:95px;">
                            '.$cateringImage.'</div>
                            <div class="itemImgText"><div class="menuItmeSubTitle">'.$cateringData->buffetContent.'<br></div></div></div></div></li>
                            </ul>
                            </div> ';
				}
			}else{
				$cateringselectionmenu=$cateringEditorData;
			}

			
			
		}
		
		
		
		//Sliders

		$defaultrightslider="public://privateevents_media/packages/sliders/defaultrightslider.jpg";
		$privateEventPackageRightSliders=PrivateEventPackagesSlider::getSliders($defaultPackageId, 'right_slider');
		if(count($privateEventPackageRightSliders)<1){
			$rightslider.= '<div class="item"><img src="'.file_create_url($defaultrightslider).'"/></div>';
		}
		
		foreach($privateEventPackageRightSliders as $rightSliderObject){
			$backgroundimage=$rightSliderObject->imagePath;
			$rightslider.= '<div class="item"><img src="'.file_create_url($backgroundimage).'"/></div>';
		}
		 
		 
		//Middle Slider
		$defaultmidleslider="public://privateevents_media/packages/sliders/defaultmiddleslider.jpg";
		
		$privateEventPackageMiddleSliders=PrivateEventPackagesSlider::getSliders($defaultPackageId, 'middle_slider');
		if(count($privateEventPackageMiddleSliders)<1){
			$middleslider.= '<div class="item"><img src="'.file_create_url($defaultmidleslider).'"/></div>';
		}
		
		foreach($privateEventPackageMiddleSliders as $middleSliderObject){
			$backgroundimage=$middleSliderObject->imagePath;
		
			$middleslider.= '<div class="item"><img src="'.file_create_url($backgroundimage).'"/></div>';
		}
		
		$packageDecription= $privateEventPackagesData->packageDescription;

		
	}
	
	
	
	

	
	
	$packageParentName="";
	$packageCategoryData=PrivateEventPackageCategories::getPrivateEventPackageCategoryDetailsById($packageCategoryId);
	if($packageCategoryData){
		$packageParentName=$packageCategoryData->packageCategoryTitle." ".$packageCategoryData->packageCategorySpecialText;
	}
	
	//var_dump($packageTitle); exit();
	$socialMediasharePath=$base_url.'/private_events/packages/'.$eventId.'/'.$packageCategoryId.'?defaultPackageId='.$defaultPackageId;
	
        
        
	return array(
			'deskTopMenu' => $deskTopMenu,
			'mobileMenu' => $mobileMenu,
			'packageDecription' => $packageDecription,
			'packageTitle' => $packageTitle,//subtitle
			'packageParentName' => $packageParentName,
			'rightslider' => $rightslider,
			'middleslider' => $middleslider,
			'cateringselectionmenu' => $cateringselectionmenu,
			'downloadpackagelink' => $cateringPdfUrl,
			'downloadmenulink' => $downloadPdfUrl,
			'packageinclusionscontent' => $packageinclusions,
			'packageoptionscontent' => $packageoptions,
			'deskTopPriceTable'=>$deskTopPriceTable,
			'mobilePriceTable' => $mobilePriceTable,
			'priceTableDescription'=>$priceTableDesciption,
			'socialMediasharePath'=>$socialMediasharePath,
	);
}