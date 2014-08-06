<?php

function manage_private_event_packag_status_page($packageId){

	//var_dump($packageId); exit();
	if(isset($_GET['action'])){
		
		$eventid=$_GET['eventid'];

		if($_GET['action']=="show"){
			PrivateEventPackages::updatePrivateEventPackageDisplayStatus($packageId, 1);
		}

		if($_GET['action']=="hide"){
			PrivateEventPackages::updatePrivateEventPackageDisplayStatus($packageId, 0);

		}
	}

	$path="private-events/packages/".$eventid."/list";
	drupal_goto($path); exit();
	//echo $eventId; exit();

}


function create_new_package($eventId){
	
	
	$privateEventPackage= new PrivateEventPackages();
	$privateEventPackage->eventId=$eventId;
	$privateEventPackage->status=0;
	$packageId = $privateEventPackage->createPrivateEventPackage();
	
	$path='private-event/package/'.$packageId.'/manage';
	drupal_goto($path);
	//var_dump($path); exit();
	
}


function package_page($package_id) {
	
// 	var_dump($package_id);
// 	exit();
	
	if(isset($_GET['action']) && !empty($_GET['action'])){
		//Clean up Operations
		//Getting The Record
		$buffet_id=$_GET['buffetid'];
		$buffetData=PrivateEventBuffet::getPrivateEventBuffetsByBuffetId($buffet_id);
		$fid=$buffetData->buffet_image_fid;
		if($fid>0){
			$file=file_load($fid);
			if($file){
				if(!empty($file->uri)){
					drupal_unlink($file->uri);
				}
				//exit();
				db_delete('file_managed')->condition('fid', $fid)->execute();
				db_delete('file_usage')->condition('fid', $fid)->execute();
			}
		}
		PrivateEventBuffet::deletePrivateEventBuffet($buffet_id);
		
		unset($_GET['action']);
		unset($_GET['buffetid']);
		
		$path="private-events/packages/".$category_id;
		drupal_goto($path); exit();
		
		
		//var_dump($buffetData); exit();
		//
		
		//exit();
		
		//PrivateEventBuffet::deletePrivateEventBuffet($buffet_id);
		
	}
	
	//var_dump($_GET); exit();
	//var_dump($id); exit();
	return drupal_get_form ( "manage_package_form",$package_id);
}





function manage_package_form($form, &$form_state,$packageId) {
	drupal_add_css(drupal_get_path('module', 'privateeventmanagement') . '/css/privateeventmanagement.css');
	$form = array ();
	
	
	$menuHeaderHtml=PrivateEventPackagesHeaderMenu::getPackageMenuList($packageId,'packageinformation');
	$form['headerstart']=array(
		'#type'=>'markup',
		'#markup'=>$menuHeaderHtml,
	);
	
	$imgurl = $GLOBALS['base_url'] . "/sites/all/modules/custom/privateevent/privateeventmanagement/img/pkgdemo.png";
	$form['packagemodal']=array(
			'#type'=>'markup',
			'#markup'=>'
				<div id="packagemodal" class="modal hide fade" tabindex="-1" role="dialog" aria-labelledby="packagemodalLabel" aria-hidden="true">
				<div class="modal-header">
				<button type="button" class="close" data-dismiss="modal" aria-hidden="true">x</button>
				<h3 id="packagemodalLabel">Overview</h3>
				</div>
				<div class="modal-body">
				<img src="' . $imgurl . '" alt="" />
				</div>
				<div class="modal-footer">
				<button class="btn" data-dismiss="modal" aria-hidden="true">Close</button>
				</div>
				</div>'
	);

		
	
	//$form_state['category_id']=$categoryid;
	
	$form['packageId']=array(
		'#type'=>'hidden',
		'#default_value'=>$packageId
	);
	
	$form['packagedivstart']=array(
			'#type'=>'markup',
			'#markup'=>'<div class="overviewformbackground">',
	);
	
	
	$form['pagetitle']=array(
			'#type'=>'markup',
			'#markup'=>'<h2 style="margin-bottom:30px;">Package Information&nbsp;&nbsp;<a href="#packagemodal" role="button" class="" data-toggle="modal"><span class="icon-question-sign"></span></a></h2>'
	);
	

	
	$packageTitle='';
	$packageDescription='';
	$downloadPdfFid='';
	$packageInclusions='';
	$packageOptions='';
	$widgetSpecialText='';
	$widgetHoverText='';
	
	$widgetImagePath='';
	$cateringPdfFid='';
	$packageCategoryId='';
	
	
	
	
	
	
	$existingPackagedata=PrivateEventPackages::getPrivateEventPackageDataByPackageId($packageId);
	
	//var_dump($existingPackagedata); exit();
	if($existingPackagedata){
		
		$packageTitle=$existingPackagedata->packageTitle;
		$packageDescription=$existingPackagedata->packageDescription;
		$downloadPdfFid=$existingPackagedata->downloadPdfFid;
		$packageInclusions=$existingPackagedata->packageInclusions;
		//$packageInclusions=$existingPackagedata->packageTitle;
		
		$packageOptions=$existingPackagedata->packageOptions;
		
		$widgetSpecialText=$existingPackagedata->widgetSpecialText;
		$widgetHoverText=$existingPackagedata->widgetHoverText;
		$widgetImagePath=$existingPackagedata->widgetImagePath;
		
		$cateringPdfFid=$existingPackagedata->cateringPdfFid;
		$packageCategoryId=$existingPackagedata->packageCategoryId;
		
	}
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	$packageCategoriesList=PrivateEventPackageCategories::getPrivateEventPackageCategories();
	//var_dump($data); exit();
	
	$packageCatsList=array();
	foreach($packageCategoriesList as $packageCategory){
		$packageCatsList[$packageCategory->packageCategoryId]=$packageCategory->packageCategoryTitle.' '.$packageCategory->packageCategorySpecialText;
		
	}
	
	$form['packageCategoryId']=array(
		'#type'=>'select',
		'#options'=>$packageCatsList,
		'#default_value' => $packageCategoryId,
		'#title'=>'Package Category',
		'#required'=>TRUE,
	);
	
	
	$form['packageTitle']=array(
		'#type'=>'textfield',
		'#title'=>"Package Title",
		'#default_value'=>$packageTitle,
		'#required'=>TRUE
	);
	
// 	$form['widgetHoverText']=array(
// 			'#type'=>'textfield',
// 			'#title'=>"Widget Hover Text",
// 			'#default_value'=>$widgetHoverText,
// 			'#required'=>TRUE
// 	);
	
// 	$form['widgetSpecialText']=array(
// 			'#type'=>'textfield',
// 			'#title'=>"Widget Special Text",
// 			'#default_value'=>$widgetSpecialText,
// 			'#required'=>TRUE
// 	);
	
	
// 	if(!empty($widgetImagePath)){
// 		$form['background_preview']=array(
// 				'#type'=>'markup',
// 				'#markup'=>'<img style="width:200px;" class="img-polaroid" src="'.file_create_url($widgetImagePath).'">'
// 		);
	
// 		$form['old_image_path']=array(
// 				'#type'=>'hidden',
// 				'#default_value'=>$widgetImagePath
// 		);
// 	}
	
// 	$form['widgetImagePath']=array(
// 			'#title' => t('Widget Background Image'),
// 			'#type'=>'file',
	
// 	);
	
	
	$form['packageDescription']=array(
			'#type'=>'text_format',
			'#title'=>"Description",
			'#default_value'=>$packageDescription,
			'#required'=>TRUE
	);
	
	
	$form['downloadPdf']=array(
			'#name' => 'files[downloadPdf]',
			'#type' => 'managed_file',
			'#title' => t('Download Package PDF'),
			'#upload_location' => 'public://privateevents_media/packages/pdf/',
			'#default_value' => isset($downloadPdfFid) ? $downloadPdfFid: '',
			'#upload_validators' => array(
    		'file_validate_extensions' => array('pdf'),
    		// Pass the maximum file size in bytes
    		'file_validate_size' => '',
 			),
	);
	
	
	$form['cateringSelectionPdf']=array(
			'#name' => 'files[cateringSelectionPdf]',
			'#type' => 'managed_file',
			'#title' => t('Catering Selection PDF'),
			'#upload_location' => 'public://privateevents_media/packages/pdf/',
			'#default_value' => isset($cateringPdfFid) ? $cateringPdfFid: '',
			'#upload_validators' => array(
    		'file_validate_extensions' => array('pdf'),
    		// Pass the maximum file size in bytes
    		'file_validate_size' => '',
 			),
	);
	
	$form['markup']=array(
		'#type'=>'markup',
		'#markup'=>"<span class='icon-info-sign'></span> Please don't upload same file in Download Package PDF and Catering Selection PDF.",
	);
	
	
	$form['packageDescription']=array(
			'#type'=>'text_format',
			'#title'=>"Description",
			'#default_value'=>$packageDescription,
			'#required'=>TRUE
	);
	
	
	
	$form['packageInclusions']=array(
			'#type'=>'text_format',
			'#title'=>"Package Inclusions",
			'#default_value'=>$packageInclusions,
			'#required'=>TRUE,
			'#prefix'=>'<div class="row-fluid" style="margin-top:5%;"><div class="span6">',
			'#suffix'=>'</div>'
	);
	
	
	$form['packageOptions']=array(
			'#type'=>'text_format',
			'#title'=>"Package Options",
			'#default_value'=>$packageOptions,
			'#required'=>TRUE,
			'#prefix'=>'<div class="span6">',
			'#suffix'=>'</div></div>'
	);
	
	
	
	$form['submt']=array(
			'#type'=>'submit',
			'#value'=>'Save'
	);
	
	
	
	
	$form['packagedivend']=array(
			'#type'=>'markup',
			'#markup'=>'</div>',
	);
	
	return $form;
}
// function manage_package_form_validate($form, &$form_state){
// 	var_dump($form_state['values']['downloadPdf']); exit();	
// }

function manage_package_form_submit($form, &$form_state){
	
	
	
	
	try{
		$downloadPdfFid = 0;
		if ($form_state['values']['downloadPdf'] != "") {
			$validators = array('');
			$dest = file_default_scheme() . '://privateevents_media/packages/pdf/';
			$file = file_save_upload('downloadPdf', $validators, $dest);
			if (is_null($file)) {
				$file = file_load($form_state['values']['downloadPdf']);
				$file->status = "1";
				$fileobject = file_save($file);
				$downloadPdfFid = $fileobject->fid;
		
				// file usuage
				$fileusuage = new stdClass();
				$fileusuage->fid = $fileobject->fid;
				file_usage_add($fileusuage, 'file', 'cruise', '111');
			} else {
				$file->status = "1";
				$fileobject = file_save($file);
				$downloadPdfFid = $fileobject->fid;
		
				// file usuage
				$fileusuage = new stdClass();
				$fileusuage->fid = $fileobject->fid;
				file_usage_add($fileusuage, 'file', 'cruise', '111');
			}
		}
		
		//CateringSelection PDF
		$cateringPdfFid = 0;
		if ($form_state['values']['cateringSelectionPdf'] != "") {
			$validators = array();
			$dest = file_default_scheme() . '://privateevents_media/packages/pdf/';
			$file = file_save_upload('cateringSelectionPdf', $validators, $dest);
			if (is_null($file)) {
				$file = file_load($form_state['values']['cateringSelectionPdf']);
				$file->status = "1";
				$fileobject = file_save($file);
				$cateringPdfFid = $fileobject->fid;
		
				// file usuage
				$fileusuage = new stdClass();
				$fileusuage->fid = $fileobject->fid;
				file_usage_add($fileusuage, 'file', 'cruise', '111');
			} else {
				$file->status = "1";
				$fileobject = file_save($file);
				$cateringPdfFid = $fileobject->fid;
		
				// file usuage
				$fileusuage = new stdClass();
				$fileusuage->fid = $fileobject->fid;
				file_usage_add($fileusuage, 'file', 'cruise', '111');
			}
		}
		
		
		
		// 	$dest_dir = file_default_scheme() . '://privateevents_media/private_events_widget_background/'; // Note: file_directory_path() was removed in Drupal 7.x. // $dest_dir contains the destination directory for the file.
		// 	$validators = array (
		// 			'file_validate_extensions' => array ('jpg png gif')
		// 	);
		
		// 	$fileURI = '';
		// 	if ($_FILES ['files'] ['name'] ['widgetImagePath'] != '') {
		// 		$filename = '';
		
		
		// 		if ($file = file_save_upload ( 'widgetImagePath', $validators, $dest_dir )) {
		// 			$fileURI = $file->uri;
		// 			db_delete ( 'file_managed' )->condition ( 'fid', $file->fid, '=' )->execute ();
		// 			$filename = isset ( $file->filename ) ? $file->filename : "";
		// 		} else {
		// 			// form_set_error('', 'Could not upload file.');
		// 		}
		// 	}
		
		
		// 	if(empty($fileURI) || is_null($fileURI)){
		// 		$fileURI=$form_state['values']['old_image_path'];
		// 	}
		
		
		$privateEventPackages=new PrivateEventPackages();
		$privateEventPackages->packageId=$form_state['values']['packageId'];
		$privateEventPackages->packageTitle=$form_state['values']['packageTitle'];
		$privateEventPackages->packageDescription=$form_state['values']['packageDescription']["value"];
		$privateEventPackages->downloadPdfFid=$downloadPdfFid;
		
		$privateEventPackages->packageInclusions=$form_state['values']['packageInclusions']["value"];
		
		$privateEventPackages->packageOptions=$form_state['values']['packageOptions']["value"];
		
		//$privateEventPackages->widgetHoverText=$form_state['values']['widgetHoverText'];
		//$privateEventPackages->widgetSpecialText=$form_state['values']['widgetSpecialText'];
		//$privateEventPackages->widgetImagePath=$fileURI;
		$privateEventPackages->cateringPdfFid=$cateringPdfFid;
		$privateEventPackages->packageCategoryId=$form_state['values']['packageCategoryId'];
		//var_dump($privateEventPackages); exit();
		$updPackageResult=$privateEventPackages->updatePrivateEventPackageInfo();
		
		if($updPackageResult>0){
			drupal_set_message('Saved Successfully');
		}
	}catch(Exception $e){
		
	}
	
	
	//var_dump($privateEventPackages); exit();
	
	
	
	
	
	
	
	
	//var_dump($downloadPdfFid); exit();
}







