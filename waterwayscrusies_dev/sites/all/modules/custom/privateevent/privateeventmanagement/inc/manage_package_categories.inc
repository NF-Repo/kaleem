<?php

function manage_package_categories(){
	
	
	//var_dump($_GET['pageAction']); exit();
	
	$packageCategoryId = '';
	if(isset($_GET['pageAction'])){
		$pageAction=$_GET['pageAction'];
		
		//Add
		if($pageAction=="add"){
			return drupal_get_form("manage_package_categories_form",$packageCategoryId);
		}
		
		//Delete
		if ($pageAction == "edit") {
			
			if (isset ( $_GET ['id'] )) {
				$packageCategoryId = $_GET ['id'];
			}
			return drupal_get_form ( "manage_package_categories_form", $packageCategoryId );
		}
		
		//View
		if($pageAction=="view"){
			
			//echo "vieww"; exit();
			return drupal_get_form("packagecategorieslistform");
		}
	}
}


function packagecategorieslistform($form, &$form_state) {
	 
drupal_set_title('Package Categories');

global $base_url;

	$form['addcategory']=array(
			'#type'=>'markup',
			'#markup'=>'<a class="btn" href="' . $base_url . '/private-events/packagescategories?pageAction=add">Add New Category</a>'
	);
	
	$output = '';
	$header = array(
			array(
				'data' => 'Category Name',
			),
			array(
				'data' => 'Action',
			),
		
			 
	);

	
	$query = db_select("private_event_package_categories", "c");
	$query->fields('c');
	$categories = $query->execute()->fetchAll();
	$rows=array();
	foreach($categories as $category){
		
		$rows[]=array(
				$category->packageCategoryTitle,
				'<a href="' . $base_url . '/private-events/packagescategories?pageAction=edit&id=' . $category->packageCategoryId . '"><span class="icon-edit"></span> </a>'
		);
		
	}
	 
	$output .= theme_table(array(
			'header' => $header,
			'rows' => $rows,
			'attributes' => array(
					'class' => array('table table-bordered')
			),
			'sticky' => true,
			'caption' => '',
			'colgroups' => array(),
			'empty' => t("No Records!")
	)) . theme('pager');
			 
			 
			$form['outputmarkup'] = array(
					'#type' => 'markup',
					'#markup' => $output
			);
			return $form;
}





function manage_package_categories_form($form, &$form_state,$packageCategoryId) {
	drupal_add_css(drupal_get_path('module', 'privateeventmanagement') . '/css/privateeventmanagement.css');
	$form = array ();

	$packageCategoryTitle='';
	$packageCategorySpecialText='';
	$packageHoverDescription='';
	$packageWidgetImage='';
	if(!empty($packageCategoryId)){
		$existingRecord=PrivateEventPackageCategories::getPrivateEventPackageCategoryDetailsById($packageCategoryId);
		if($existingRecord){
			$packageCategoryId=$existingRecord->packageCategoryId;
			$packageCategoryTitle=$existingRecord->packageCategoryTitle;
			$packageCategorySpecialText=$existingRecord->packageCategorySpecialText;
			$packageHoverDescription=$existingRecord->packageHoverDescription;
			$packageWidgetImage=$existingRecord->packageWidgetImage;
		}
	}
	
	$menuHeaderHtml='';
	$form['headerstart']=array(
			'#type'=>'markup',
			'#markup'=>$menuHeaderHtml,
	);

	$form['packageCategoryId']=array(
			'#type'=>'hidden',
			'#default_value'=>$packageCategoryId
	);

	$form['packagedivstart']=array(
			'#type'=>'markup',
			'#markup'=>'<div class="overviewformbackground" style="margin-top:10px;">',
	);


	$form['pagetitle']=array(
			'#type'=>'markup',
			'#markup'=>'<h2 style="margin-bottom:30px;">Package Category Information&nbsp;&nbsp;</h2>'
	);



	$form['packageCategoryTitle']=array(
			'#type'=>'textfield',
			'#title'=>"Package Category Title",
			'#default_value'=>$packageCategoryTitle,
			'#required'=>TRUE,
			'#maxlength'=> 30,
	);
	
	$form['packageCategorySpecialText']=array(
			'#type'=>'textfield',
			'#title'=>"Widget Special Text",
			'#default_value'=>$packageCategorySpecialText,
			'#maxlength'=> 30,
			//'#required'=>TRUE
	);

	$form['packageHoverDescription']=array(
			'#type'=>'textfield',
			'#title'=>"Widget Hover Text",
			'#default_value'=>$packageHoverDescription,
			'#required'=>TRUE,
			'#maxlength'=> 75,
	);

	


	if(!empty($packageWidgetImage)){
		$form['background_preview']=array(
				'#type'=>'markup',
				'#markup'=>'<img style="width:200px;" class="img-polaroid" src="'.file_create_url($packageWidgetImage).'">'
		);

		$form['old_image_path']=array(
				'#type'=>'hidden',
				'#default_value'=>$packageWidgetImage
		);
	}

	$form['packageWidgetImage']=array(
			'#title' => t('Widget Background Image'),
			'#type'=>'file',
			'#description'=>'<p><span class="icon-hand-right" style="margin-top:2px;"></span>&nbsp;Uploaded Image Size should be 250 x 250 pixels. </p>'
			

	);




	



	$form['submt']=array(
			'#type'=>'submit',
			'#value'=>'Save'
	);
	
	global $base_url;
	$form['cancel']=array(
			'#type'=>'markup',
			'#markup'=>'<a class="btn" href="' . $base_url . '/private-events/packagescategories?pageAction=view">Back</a>'
	);




	$form['packagedivend']=array(
			'#type'=>'markup',
			'#markup'=>'</div>',
	);

	return $form;
}

function manage_package_categories_form_submit($form, &$form_state) {
	
	
	$dest_dir = file_default_scheme() . '://privateevents_media/private_events_widget_background/'; // Note: file_directory_path() was removed in Drupal 7.x. // $dest_dir contains the destination directory for the file.
	$validators = array (
			'file_validate_extensions' => array ('jpg png gif')
	);
	
	
	$filename = '';
	$fileURI = '';
	$old_image_path=isset($form_state['values']['old_image_path']) ? $form_state['values']['old_image_path'] : '';
	if ($_FILES ['files'] ['name'] ['packageWidgetImage'] != '') {
		if(!empty($old_image_path)){
			drupal_unlink($old_image_path);
		}
	
		if ($file = file_save_upload ( 'packageWidgetImage', $validators, $dest_dir )) {
			$fileURI = $file->uri;
			db_delete ( 'file_managed' )->condition ( 'fid', $file->fid, '=' )->execute ();
			$filename = isset ( $file->filename ) ? $file->filename : "";
		}
	}
	
	if(empty($fileURI) || is_null($fileURI)){
		$fileURI=$old_image_path;
	}
	
	
	//$privateEventPackageCategories=PrivateEventPackagCategories();
	
	$privateEventPackagCategories=new PrivateEventPackageCategories();
	$privateEventPackagCategories->packageCategoryTitle=trim($form_state['values']['packageCategoryTitle']);
	$privateEventPackagCategories->packageCategorySpecialText=trim($form_state['values']['packageCategorySpecialText']);
	$privateEventPackagCategories->packageHoverDescription=trim($form_state['values']['packageHoverDescription']);
	$privateEventPackagCategories->packageWidgetImage=$fileURI;
	
	
	if (isset ( $form_state ['values'] ['packageCategoryId'] ) && ! empty ( $form_state ['values'] ['packageCategoryId'] )) {
		$privateEventPackagCategories->packageCategoryId = $form_state ['values'] ['packageCategoryId'];
		$res = $privateEventPackagCategories->updatePrivateEventPackageCategory ();
		
		if ($res > 0) {
			drupal_set_message ( "Success" );
		} else if ($res == 0) {
		} else {
			drupal_set_message ( "Failed" );
		}
	} else {
		$res = $privateEventPackagCategories->createPrivateEventPackageCategory ();
		if ($res > 0) {
			drupal_set_message ( "Success" );
		} else {
			drupal_set_message ( "Failed" );
		}
	}


}