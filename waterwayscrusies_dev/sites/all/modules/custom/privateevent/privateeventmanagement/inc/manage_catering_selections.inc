<?php

function catering_selections_page($packageId){
	//var_dump($packageId); exit();
	
	if(isset($_GET['action']) && !empty($_GET['action'])){
		//Clean up Operations
		//Getting The Record
		$cateringId=$_GET['cateringId'];
		$buffetData=PrivateEventPackageCateringSelection::getPrivateEventBuffetsByCateringId($cateringId);
		
		//var_dump($buffetData); exit();
		
		$fid=$buffetData->buffetImageFid;
		if($fid>0){
			$file=file_load($fid);
			//var_dump($file); exit();
			if($file){
				if(!empty($file->uri)){
					drupal_unlink($file->uri);
				}
				//exit();
				db_delete('file_managed')->condition('fid', $fid)->execute();
				db_delete('file_usage')->condition('fid', $fid)->execute();
			}
		}
		PrivateEventPackageCateringSelection::deletePrivateEventBuffet($cateringId);
	
		unset($_GET['action']);
		unset($_GET['cateringId']);
	
		$path="/private-event/package/".$packageId.'/cateringselections';
		drupal_goto($path); exit();

	
	}
	
	
	return drupal_get_form ( "buffet_form",$packageId);
}



function layoutsettingscallback(&$form, &$form_state) {
	
	$cateringLayoutStyle=$form_state['values']['cateringLayoutStyle'];
	$packageId=$form_state['values']['packageId'];
	
	PrivateEventPackages::updatePrivateEventBuffetLayout($packageId, $cateringLayoutStyle);
	$commands[] = ajax_command_alert("Layout Style Updated Successfully!");
	return array (
			'#type' => 'ajax',
			'#commands' => $commands
	);
	
	//var_dump($cateringLayoutStyle); exit();
	// The element is already build correctly at this point so just return it.
	return $form;
}
/*
 * Form
 */
function buffet_form($form, &$form_state,$packageId) {
	drupal_add_css(drupal_get_path('module', 'privateeventmanagement') . '/css/privateeventmanagement.css');
	$form = array ();
	
	/*
	 * Old Record Data
	 */
	//$existingData=PrivateEventPackageCateringSelection::getPrivateEventPackageDataByPackageId($packageId);
	
	$existingData=PrivateEventPackages::getPrivateEventPackageDataByPackageId($packageId);
	//var_dump($existingData); exit();
	
	//$existingData=Pri
	$cateringEditorData='';
	$cateringLayoutStyle='';
	if($existingData){
		$cateringLayoutStyle=$existingData->cateringLayoutStyle;
		$cateringEditorData=$existingData->cateringEditorData;
		
	}
		
	
	$menuHeaderHtml=PrivateEventPackagesHeaderMenu::getPackageMenuList($packageId,'catering');
	$form['headerstart']=array(
		'#type'=>'markup',
		'#markup'=>$menuHeaderHtml,
	);
	
	$form['cateringdivstart']=array(
		'#type'=>'markup',
		'#markup'=>'<div class="overviewformbackground">',
	);
	
	

	$form ['packageId'] = array (
		'#type' => 'hidden',
		'#default_value' => $packageId 
	);
	
	
	$form ['catering'] = array (
		'#type' => 'markup',
		'#markup' => '<h2>Catering Selections</h2>' 
	);
	
	
	$form['layoutsettingsdiv']=array(
		'#type'=>'markup',
		'#markup'=>'<div id="layoutsettingsdiv"></div>'
	);
	
	$form ['cateringLayoutStyle'] = array (
			'#type' => 'select',
			'#options' => array (
				'fixed_layout' => 'Fixed Column',
				'editor' => 'Editor' 
			),
			'#default_value'=>$cateringLayoutStyle,
			"#limit_validation_errors" => array(),
			'#ajax' => array(
					'callback' => 'layoutsettingscallback',
					'wrapper' => 'layoutsettingsdiv'
			)
	);
	$form ['buffet'] = array (
			'#type' => 'fieldset',
			'#title' => 'Buffet',
			'#states' => array (
					'visible' => array (
							':input[name="cateringLayoutStyle"]' => array (
									'value' => 'fixed_layout' 
							) 
					) 
			) 
	);
	
	
	$form['buffet']['tablestart']=array(
		'#type'=>'markup',
		'#markup'=>'<table>',
	);
	
	
	$buffetObject=false;
	
	
	$cateringId='';
	$buffeTitle='';
	$buffetImageFid='';
	$buffetContent='';
	
	if(isset($_GET['cateringId'])){
		$cateringId=$_GET['cateringId'];
		$buffetObject=PrivateEventPackageCateringSelection::getPrivateEventBuffetsByCateringId($cateringId);
		
		$cateringId=$buffetObject->cateringId;
		$buffeTitle=$buffetObject->buffeTitle;
		$buffetImageFid=$buffetObject->buffetImageFid;
		$buffetContent=$buffetObject->buffetContent;
		
		
	}
	
	$form['buffet']['buffeTitle']=array(
			'#type'=>'textfield',
			'#prefix'=>'<tr><td><b>Title :</b></td><td>',
			'#suffix'=>'</td></tr><tr><td><b>Image :</b></td><td>',
			'#default_value'=>$buffeTitle,
	
	);
	
	$form ['buffet'] ['buffet_image'] = array (
			'#name' => 'files[buffet_image]',
			'#type' => 'managed_file',
			'#upload_location' => 'public://privateevents_media/packages/buffetcategories/',
			'#default_value' =>$buffetImageFid,
			'#description'=>'<p><span class="icon-hand-right" style="margin-top:2px;"></span>&nbsp;Uploaded Image Size should be 250 x 250 pixels. </p>'
	);
	
	
	$form['buffet']['buffetContent']=array(
			'#type'=>'text_format',
			'#prefix'=>'</td></tr><tr><td><b>Content :</b></td><td>',
			'#suffix'=>'</td></tr>',
			'#default_value'=>$buffetContent,
	);
	
	
	
	$form['buffet']['tableend']=array(
		'#type'=>'markup',
		'#markup'=>'</table>',
	);
	
	
	
	$form['cateringEditorData']=array(
			'#type'=>'text_format',
			'#title'=>'Content',
			'#states' => array(
					'visible' => array(
							':input[name="cateringLayoutStyle"]' => array('value' => 'editor'),
			  ),
			),
			'#default_value'=>$cateringEditorData,
				
	);
	

	
	if (!empty($cateringId)) {
		$form ['cateringId'] = array (
			'#type' => 'hidden',
			'#value' => $cateringId,
		);
		
		
		$form ['update_buffet'] = array (
			'#type' => 'submit',
			'#value' => t ( 'Update Buffet' ),
			'#submit' => array (
				'update_buffet_submit' 
				) 
		);
		
		$form ['cancel_buffet'] = array (
				'#type' => 'submit',
				'#value' => t ( 'Cancel Buffet' ),
				// '#limit_validation_errors' => array(),
				'#submit' => array (
						'cancel_buffet_submit' 
				) 
		)
		;
	} else {
		
		$form ['add_buffet'] = array (
				'#type' => 'submit',
				'#value' => t ( 'Save' ),
				// '#limit_validation_errors' => array(),
				'#submit' => array (
						'add_buffet_submit' 
				) 
		);
	}
	
	

	
	
	
	$privateEventBuffetTableData='';
	global $base_url;
	
	$privateEventBuffetsdata=PrivateEventPackageCateringSelection::getPrivateEventBuffets($packageId);
	if(count($privateEventBuffetsdata)>0){
		$privateEventBuffetTableData='<table class="privateeventbuffet"><tr><th>Buffet Title</th><th>Image</th><th>Content</th><th>Actions</th></tr>';
	
		foreach($privateEventBuffetsdata as $privateEventBuffet){
			
			if($cateringId!=$privateEventBuffet->cateringId){
				$imagePath="";
				$file=file_load($privateEventBuffet->buffetImageFid);
				if($file->uri){
					$imagePath='<img class="img-polaroid" style="width:100px;" src="'.file_create_url($file->uri).'">';
				}
				
				$privateEventBuffetTableData.="<tr>";
				$privateEventBuffetTableData.="<td>".$privateEventBuffet->buffeTitle."</td>";
				$privateEventBuffetTableData.='<td>'.$imagePath.'</td>';
				$privateEventBuffetTableData.="<td>".$privateEventBuffet->buffetContent."</td>";
				$privateEventBuffetTableData.='<td>
					<a href="' . $base_url . '/private-event/package/'.$packageId.'/cateringselections?cateringId='.$privateEventBuffet->cateringId.'"><span class="icon-edit"></span>  </a>
				
				|
							<a href="' . $base_url . '/private-event/package/'.$packageId.'/cateringselections?action=delete&cateringId='.$privateEventBuffet->cateringId.'"><span class="icon-trash"></span>  </a>
				
				
					';
				$privateEventBuffetTableData.="</tr>";
			}
		
		}
		$privateEventBuffetTableData.="</table>";
	}
	
	
	$form['privateeventdata']=array(
		'#id'=>'privateeventdata',
		'#type'=>'fieldset',
		'#title'=>'',
			'#states' => array (
					'visible' => array (
							':input[name="cateringLayoutStyle"]' => array (
									'value' => 'fixed_layout'
							)
					)
			)
	);
	
	$form['privateeventdata']['privateEventTableData']=array(
			'#type'=>'markup',
			'#markup'=>$privateEventBuffetTableData,
			
	);
	
	
	$form['cateringdivend']=array(
			'#type'=>'markup',
			'#markup'=>'</div>',
	);

	return $form;
}


function add_buffet_submit($form, &$form_state){
	
	
	$cateringLayoutStyle=$form_state['values']['cateringLayoutStyle'];
	$packageId=$form_state['values']['packageId'];
	
	
	if($cateringLayoutStyle=="editor"){
		$cateringEditorData=$form_state['values']['cateringEditorData']['value'];
		PrivateEventPackages::updatePrivateEventBuffetEditorData($packageId, $cateringLayoutStyle, $cateringEditorData);
		
	}
	if($cateringLayoutStyle=="fixed_layout"){
		PrivateEventPackages::updatePrivateEventBuffetLayout($packageId, $cateringLayoutStyle);
		
		$buffet_img_fid = 0;
		if ($form_state['values']['buffet_image'] != "") {
			$validators = array();
			$dest = file_default_scheme() . '://privateevents_media/packages/buffetcategories/';
			$file = file_save_upload('buffet_image', $validators, $dest);
			if (is_null($file)) {
				$file = file_load($form_state['values']['buffet_image']);
				$file->status = "1";
				$fileobject = file_save($file);
				$buffet_img_fid = $fileobject->fid;
		
				// file usuage
				$fileusuage = new stdClass();
				$fileusuage->fid = $fileobject->fid;
				file_usage_add($fileusuage, 'file', 'cruise', '111');
			} else {
				$file->status = "1";
				$fileobject = file_save($file);
				$buffet_img_fid = $fileobject->fid;
		
				// file usuage
				$fileusuage = new stdClass();
				$fileusuage->fid = $fileobject->fid;
				file_usage_add($fileusuage, 'file', 'cruise', '111');
			}
		}

		$privateEventPackageCateringSelection= new PrivateEventPackageCateringSelection();
		$privateEventPackageCateringSelection->packageId=$packageId;
		$privateEventPackageCateringSelection->buffetImageFid=$buffet_img_fid;
		$privateEventPackageCateringSelection->buffeTitle=$form_state['values']['buffeTitle'];
		$privateEventPackageCateringSelection->buffetContent=$form_state['values']['buffetContent']['value'];
		$privateEventPackageCateringSelection->createPrivateEventBuffet();
	}
}







function update_buffet_submit($form, &$form_state){
	
	$cateringLayoutStyle=$form_state['values']['cateringLayoutStyle'];
	$packageId=$form_state['values']['packageId'];
	
	//var_dump('hello'); exit();
	
	if($cateringLayoutStyle=="fixed_layout"){
	
		$buffet_img_fid = 0;
		if ($form_state['values']['buffet_image'] != "") {
			$validators = array();
			$dest = file_default_scheme() . '://PrivateEventsBuffetImages/';
			$file = file_save_upload('buffet_image', $validators, $dest);
			if (is_null($file)) {
				$file = file_load($form_state['values']['buffet_image']);
				$file->status = "1";
				$fileobject = file_save($file);
				$buffet_img_fid = $fileobject->fid;
		
				// file usuage
				$fileusuage = new stdClass();
				$fileusuage->fid = $fileobject->fid;
				file_usage_add($fileusuage, 'file', 'cruise', '111');
			} else {
				$file->status = "1";
				$fileobject = file_save($file);
				$buffet_img_fid = $fileobject->fid;
		
				// file usuage
				$fileusuage = new stdClass();
				$fileusuage->fid = $fileobject->fid;
				file_usage_add($fileusuage, 'file', 'cruise', '111');
			}
		}
		
		$privateEventPackageCateringSelection= new PrivateEventPackageCateringSelection();
		$privateEventPackageCateringSelection->cateringId=$form_state['values']['cateringId'];
		//$privateEventPackageCateringSelection->packageId=$packageId;
		$privateEventPackageCateringSelection->buffetImageFid=$buffet_img_fid;
		$privateEventPackageCateringSelection->buffeTitle=$form_state['values']['buffeTitle'];
		$privateEventPackageCateringSelection->buffetContent=$form_state['values']['buffetContent']['value'];
		
		//var_dump($privateEventPackageCateringSelection); exit();
		$privateEventPackageCateringSelection->updatePrivateEventBuffet();
		
		
		$path='private-event/package/'.$packageId.'/cateringselections';
		$form_state['redirect']=$path;
		//drupal_goto($path); exit();
		
	}


}

function cancel_buffet_submit($form, &$form_state){
	
	$packageId=$form_state['values']['packageId'];
	$path='private-event/package/'.$packageId.'/cateringselections';
		drupal_goto($path); exit();

}