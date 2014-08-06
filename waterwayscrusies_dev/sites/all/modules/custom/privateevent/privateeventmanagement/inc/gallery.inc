<?php
function private_events_gallery_page($eventId){
	
	if(isset($_GET['action'])){
		//echo "asdf"; exit();
		$galleryId=$_GET['galleryId'];
		PrivateEventGallery::deleteGallery($galleryId);
		
		$path="private-event/".$eventId."/gallery";
		unset($_GET['galleryId']);
		unset($_GET['action']);
		
		drupal_goto($path); exit();
	}
	
	return drupal_get_form("private_event_gallery_form",$eventId);
	
}


function private_event_gallery_form($form, &$form_state,$eventId) {
	

	
	
	
	drupal_add_css(drupal_get_path('module', 'privateeventmanagement') . '/css/privateeventmanagement.css');
	
	$form=array();
	

	$form['eventId']=array(
		'#type'=>'hidden',
		'#default_value'=>$eventId,
	);
	
	
	
	$privateEventMenus = PrivateEvents::getPrivateEventMenus($eventId );
	$default_private_event_fields=explode(',', $privateEventMenus->menusIds);
	$menuHtml=PrivateEventHeaderMenu::getHeaderMenu($default_private_event_fields,$eventId,"7");
	$form['menuHtml']=array(
			'#type'=>'markup',
			'#markup'=>$menuHtml,
	);
	
	
	
	$form['fileuploadhtmlstart']=array(
			'#type'=>'markup',
			'#markup'=>'<div class="overviewformbackground">',
	);
	
	$form['galleryType']=array(
		'#type'=>'select',
		'#options'=>array('image'=>'Image','video'=>'Video'),
		'#title'=>'Type'
	);
	
	
	$form['videoUrl']=array(
		'#type'=>'textfield',
		'#title'=>'Video URL',
		//'#required'=>TRUE,
			'#states' => array (
					'visible' => array (
							':input[name="galleryType"]' => array (
									'value' => 'video'
							)
					)
			),
			'#description'=>'Example : https://www.youtube.com/watch?feature=player_embedded&v=cMNNN266ryM',
	);
	
	
	$form['imagePath']=array(
			'#type'=>'file',
			'#title'=>'Image',
			//'#required'=>TRUE,
			'#states' => array (
					'visible' => array (
							':input[name="galleryType"]' => array (
									'value' => 'image'
							)
					)
			),
			'#description'=>'<p><span class="icon-hand-right" style="margin-top:2px;"></span>&nbsp;Uploaded Image Size should be 260 x 190 pixels. </p>'
		
			
	);
	
	
	$form['submit']=array(
		'#type'=>'submit',
		'#value'=>'Save',
	);
	

	$form['fileuploadhtmlstartend']=array(
			'#type'=>'markup',
			'#markup'=>'</div>',
	);
	
	$form['gallerydivstart']=array(
			'#type'=>'markup',
			'#markup'=>'<div class="overviewformbackground" style="margin-top:20px;">',
	);
	global $base_url;
	
	$galleryHtml='';
	$galleryData=PrivateEventGallery::getGalleryListByEventId($eventId);
	
	if($galleryData){
		$galleryHtml='<table class="table table-bordered">';
		foreach($galleryData as $gallery){
		
			$galleryHtml.="<tr>";
			$galleryHtml.='<td><a href="' . $base_url . '/private-event/'.$eventId.'/gallery?action=deleteGallery&galleryId=' . $gallery->galleryId . '"><span class="icon-trash" style="margin-top:30px;"></span></a></td>';
			if($gallery->galleryType=="video"){
				$galleryHtml.='<td>'.$gallery->galleryData.'</td>';
			}
		
			if($gallery->galleryType=="image"){
				if(file_exists($gallery->galleryData)){
					$galleryHtml.='<td><img class="img-polaroid" style="width:100px;" src="'.file_create_url($gallery->galleryData).'"'.$gallery->galleryData.'</td>';
		
		
				}
			}
		
			$galleryHtml.="</tr>";
		
		}
		$galleryHtml.="</table>";
	}
	
	
	//var_dump($data); exit();
	
	$form['galleryHtml']=array(
			'#type'=>'markup',
			'#markup'=>$galleryHtml,
	);
	
	
	
	$form['gallerydivend']=array(
			'#type'=>'markup',
			'#markup'=>'</div">',
	);
	
	
	
	return $form;
}


function private_event_gallery_form_validate($form, &$form_state) {
	$galleryType=$form_state['values']['galleryType'];
	
	if($galleryType=="video"){
		if(empty($form_state['values']['videoUrl'])){
			form_set_error('','URL Is Required');
		}
		
	}
	
	if($galleryType=="image"){
		if ($_FILES ['files'] ['name'] ['imagePath'] == '') {
			form_set_error('','Image is Required');
		}
	}
	
	
}

function private_event_gallery_form_submit($form, &$form_state){
	
	$eventId=$form_state['values']['eventId'];
	$galleryType=$form_state['values']['galleryType'];
	
	$privateEventGallery= new PrivateEventGallery();
	
	$privateEventGallery->eventId=$eventId;
	$privateEventGallery->galleryType=$galleryType;
	
	
	if($galleryType=="video"){
		$videoUrl=$form_state['values']['videoUrl'];
		$privateEventGallery->galleryData=$videoUrl;
	}
	
	if($galleryType=="image"){
		
		
		$dest_dir = file_default_scheme() . '://privateevents_media/gallery/'; // Note: file_directory_path() was removed in Drupal 7.x. // $dest_dir contains the destination directory for the file.
		$validators = array (
				'file_validate_extensions' => array ('jpg png gif')
		);
		if ($_FILES ['files'] ['name'] ['imagePath'] != '') {
			$filename = '';
			$fileURI = '';
				
			if ($file = file_save_upload ( 'imagePath', $validators, $dest_dir )) {
				$fileURI = $file->uri;
				db_delete ( 'file_managed' )->condition ( 'fid', $file->fid, '=' )->execute ();
				$filename = isset ( $file->filename ) ? $file->filename : "";
			} else {
				// form_set_error('', 'Could not upload file.');
			}
		}
		
		
		//$imagePath=$form_state['values']['imagePath'];
		$imagePath=$fileURI;
		$privateEventGallery->galleryData=$imagePath;
	}
	
	
	$privateEventGallery->createGallery();
	
	
	
	//var_dump($privateEventGallery); exit();
	
}