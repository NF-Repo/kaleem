<?php
function slider_page($packageId){
	
	if(isset($_GET['action'])&& $_GET['sliderId'] ){
		//var_dump($_GET['sliderId']); 
		$status=PrivateEventPackagesSlider::deleteSlider($_GET['sliderId']);
		unset($_GET['action']);
		unset($_GET['sliderId']);
		
		if($status){
			drupal_set_message("Success!");
		}
		
		$path='private-event/package/'.$packageId.'/sliders';
		drupal_goto($path);
	
	}
	
	return drupal_get_form('manage_package_slider_form',$packageId);
	//var_dump($packageId); exit();
}







function manage_package_slider_form($form, &$form_state,$packageId) {
	
	drupal_add_css(drupal_get_path('module', 'privateeventmanagement') . '/css/privateeventmanagement.css');
	$form = array ();
	
	
	global $base_url;
	$menuHeaderHtml=PrivateEventPackagesHeaderMenu::getPackageMenuList($packageId,'slider');
	$form['headerstart']=array(
		'#type'=>'markup',
		'#markup'=>$menuHeaderHtml,
	);
	
	$form['packageId']=array(
		'#type'=>'hidden',
		'#default_value'=>$packageId
	);
	
	$form['rightsliderdivstart']=array(
			'#type'=>'markup',
			'#markup'=>'<div class="overviewformbackground">',
	);
	
	
	$form['right_slider_image'] = array(
    '#id' => 'right_slider_image',
    '#type' => 'file',
	'#description'=>'<p><span class="icon-hand-right" style="margin-top:2px;"></span>&nbsp;For Right Slider Uploaded Image Size should be 799 x 765 pixels. <br/> <span class="icon-hand-right" style="margin-top:2px;"></span>&nbsp;For Middle Slider Uploaded Image Size should be  1600 X 771 pixels. </p>'
			
	//'#required' => TRUE,
    //'#title' => t('Add  Image'),
   
    );
	
	
	$form['slider_type']= array(
		'#type'=>'select',
		'#options'=>array('right_slider'=>'Right Slider','middle_slider'=>'Middle Slider'),
	);
	
	$form['upload_btn']=array(
		'#type'=>'submit',
		'#value'=>'Upload File'
	);
	
	
	$form['rightsliderdivend']=array(
			'#type'=>'markup',
			'#markup'=>'</div>',
	);
	
	$form['rightsliderimagedivstart']=array(
			'#type'=>'markup',
			'#markup'=>'<div class="overviewformbackground" style="margin-top:20px;">',
	);
	

	$html='<div class="row-fluid"><div class="span6"><table class="table table-striped"><tr><td colspan="3"><h3>Right Slider Images</h3></td></tr>';
	$rightSliderdata=PrivateEventPackagesSlider::getSliders($packageId, 'right_slider');
	if(count($rightSliderdata)>0){
		foreach($rightSliderdata as $slider){
			$rightSliderAction='<a href="' . $base_url . '/private-event/package/'.$packageId.'/sliders/?action=delete&sliderId=' . $slider->sliderId . '"><span class="icon-trash" style="margin-top:30px;"></span></a>';
			
			$html.='<tr>';
			$html.='<td><img  class="img-polaroid" style="width:100px;" src="'.file_create_url($slider->imagePath).'"></td><td>'.$rightSliderAction.'</td></tr>';
		}
	}
	
	$html.="</table></div>";
	
	$form['rightslidergallery']=array(
		'#type'=>'markup',
		'#markup'=>$html
	);
	
	
	
	$mhtml='<div class="span6"><table class="table table-striped"><tr><td colspan="3"><h3>Middle Slider Images</h3></td></tr>';
	$middleSliderdata=PrivateEventPackagesSlider::getSliders($packageId, 'middle_slider');
	if(count($middleSliderdata)>0){
		foreach($middleSliderdata as $slider){
			$middleSliderAction='<a href="' . $base_url . '/private-event/package/'.$packageId.'/sliders/?action=delete&sliderId=' . $slider->sliderId . '"><span class="icon-trash" style="margin-top:30px;"></span></a>';
				
			$mhtml.='<tr>';
			$mhtml.='<td><img  class="img-polaroid" style="width:100px;" src="'.file_create_url($slider->imagePath).'"></td><td>'.$middleSliderAction.'</td></tr>';
		}
	}
	
	$mhtml.="</table></div></div>";
	
	$form['middeleslidergallery']=array(
			'#type'=>'markup',
			'#markup'=>$mhtml
	);
	
	
	
	$form['rightsliderimagedivend']=array(
			'#type'=>'markup',
			'#markup'=>'</div>',
	);
	
	
	
	return $form;
	
	
}


function manage_package_slider_form_validate($form, &$form_state) {
	
	//var_dump($_FILES ['files'] ['name'] ['right_slider_image']); exit();
	if ($_FILES ['files'] ['name'] ['right_slider_image'] == '') {
		form_set_error('','please select an image');
	}
	
}



/*
 * Submit Action
 */
function manage_package_slider_form_submit($form, &$form_state){
	
	
	 
	$dest_dir = file_default_scheme() . '://privateevents_media/packages/sliders/'; // Note: file_directory_path() was removed in Drupal 7.x. // $dest_dir contains the destination directory for the file.
	$validators = array (
			'file_validate_extensions' => array ('jpg png gif')
	);
	if ($_FILES ['files'] ['name'] ['right_slider_image'] != '') {
		$filename = '';
		$fileURI = '';
		 
		if ($file = file_save_upload ( 'right_slider_image', $validators, $dest_dir )) {
			$fileURI = $file->uri;
			db_delete ( 'file_managed' )->condition ( 'fid', $file->fid, '=' )->execute ();
			$filename = isset ( $file->filename ) ? $file->filename : "";
		} else {
			// form_set_error('', 'Could not upload file.');
		}
	}
	
	$privateEventPackagesSlider=new PrivateEventPackagesSlider();
	$privateEventPackagesSlider->packageId=$form_state['values']['packageId'];
	$privateEventPackagesSlider->imagePath=$fileURI;
	$privateEventPackagesSlider->position=0;
	$privateEventPackagesSlider->sliderType=$form_state['values']['slider_type'];
	$privateEventPackagesSlider->createSlider();
	
	//var_dump($privateEventPackagesSlider); exit();
	
}