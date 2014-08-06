<?php


function private_events_overview_page($eventId,$type){
	
	$menuId='';
	switch ($type) {
		case 'overview' :
			$menuId = 1;
			break;
		
		case 'yachts' :
			$menuId = 2;
			break;
		case 'eventplanning' :
			$menuId = 3;
			break;
		case 'catering' :
			$menuId = 5;
			break;
		case 'vendors' :
			$menuId = 6;
			break;
	}
	return drupal_get_form("private_event_overview_form",$eventId,$menuId);
}



/*
 * Overview form 
 */

function private_event_overview_form($form, &$form_state,$eventId,$menuId) {
	
	drupal_add_css(drupal_get_path('module', 'privateeventmanagement') . '/css/privateeventmanagement.css');
	
	$oldRecorddata=PrivateEventMenuList::getPrivateEventMenuList($eventId, $menuId);
	$pageTitle=PrivateEventMenus::getPrivateEventMenuNameByMenuId($menuId);
	$menu_name=$pageTitle;
	$widget_title='';
	$widget_description='';
	$widget_button_name='REQUEST FOR PROPOSAL';
	$widget_button_url='';
	$privateEventMenuId='';
	$background_img_path='';
	
	

	if($oldRecorddata){
		$privateEventMenuId=$oldRecorddata->privateEventMenuId;
		$privateeventOverviewlist=unserialize($oldRecorddata->data);
		$menu_name=$privateeventOverviewlist['menu_name'];
		//$background_img_fid=$privateeventOverviewlist['background_img_fid'];
		$background_img_path=$privateeventOverviewlist['background_img_path'];
		
		$widget_title=$privateeventOverviewlist['widget_title'];
		$widget_description=$privateeventOverviewlist['widget_description'];
		$widget_button_name=$privateeventOverviewlist['widget_button_name'];
		$widget_button_url=$privateeventOverviewlist['widget_button_url'];
	}
	
	
	$form=array();
	$privateEventMenus = PrivateEvents::getPrivateEventMenus($eventId );
	$default_private_event_fields=explode(',', $privateEventMenus->menusIds);
	
	$menuHtml=PrivateEventHeaderMenu::getHeaderMenu($default_private_event_fields,$eventId,$menuId);
	
	$form['menuHtml']=array(
			'#type'=>'markup',
			'#markup'=>$menuHtml,
	);
	
	$imgurl = $GLOBALS['base_url'] . "/sites/all/modules/custom/privateevent/privateeventmanagement/img/overview.jpg";
	$form['overviewmodal']=array(
			'#type'=>'markup',
			'#markup'=>'
				<div id="overviewmodal" class="modal hide fade" tabindex="-1" role="dialog" aria-labelledby="overviewmodalLabel" aria-hidden="true">
				<div class="modal-header">
				<button type="button" class="close" data-dismiss="modal" aria-hidden="true">x</button>
				<h3 id="overviewmodalLabel">Overview</h3>
				</div>
				<div class="modal-body">
				<img src="' . $imgurl . '" alt="" />
				</div>
				<div class="modal-footer">
				<button class="btn" data-dismiss="modal" aria-hidden="true">Close</button>
				</div>
				</div>'
	);
	
	
	$form['privateEventMenuId']=array(
		'#type'=>'hidden',
		'#default_value'=>$privateEventMenuId,
	);
	
	$form['eventId']=array(
		'#type'=>'hidden',
		'#default_value'=>$eventId,
	);
	
	$form['menuId']=array(
			'#type'=>'hidden',
			'#default_value'=>$menuId,
	);
	
	
	$form['divstart']=array(
			'#type'=>'markup',
			'#markup'=>'<div class="overviewformbackground">',
	);
	
	
	$form['pagetitle']=array(
			'#type'=>'markup',
			'#markup'=>'<h2 style="margin-bottom:30px;">'.$pageTitle.'&nbsp;&nbsp;<a href="#overviewmodal" role="button" class="" data-toggle="modal"><span class="icon-question-sign"></span></a></h2>'
	);
	
	
	$form['menu_name']=array(
		'#type'=>'textfield',
		'#title'=>'Menu Name',
		'#default_value'=>$menu_name,
		'#required' => TRUE,
	
			
	);
	
	if(!empty($background_img_path)){
		$form['background_preview']=array(
			'#type'=>'markup',
			'#markup'=>'<img style="width:200px;" class="img-polaroid" src="'.file_create_url($background_img_path).'">'
		);
		
		$form['old_image_path']=array(
			'#type'=>'hidden',
			'#default_value'=>$background_img_path
		);
	}
	
	$form['backgroundimage']=array(
    	'#title' => t('Choose a picture for Background'),
		'#type'=>'file',
		'#description'=>'<p><span class="icon-hand-right" style="margin-top:2px;"></span>&nbsp;Uploaded Image Size should be 1600 x 771 pixels. </p>'
			
	);
	

	

	
	
	$form['widget']=array(
		'#type'=>'markup',
		'#markup'=>'<div style="margin-top:50px;"><h3>Widget Properties</h3></div>',
	);
	
	
	
	
	$form['widget_title'] = array(
		'#type' => 'textfield',
		'#title' => t('Title'),
		'#default_value'=>$widget_title,
	);
	
	$form['widget_description'] = array(
		'#type' => 'text_format',
		'#title' => t('Description'),
		'#default_value'=>$widget_description,
	);
	
	
	$form['widgetbuttonhelp']=array(
		'#type'=>'markup',
		'#markup'=>'<br/><br/><span class="icon-hand-right"></span>&nbsp;&nbsp;Button URL is optional, for empty case it is showing the Request Proposal Form'
	);
	
	$form['widget_button_name'] = array(
		'#type' => 'textfield',
		'#title' => t('Button Name'),
		'#default_value'=>$widget_button_name,
			
	);
	
	global $base_url;
	$form['widget_button_url'] = array(
		'#type' => 'textfield',
		'#title' => t('Button Url'),
		'#default_value'=>$widget_button_url,
		'#description'=>'<span class="icon-bell"></span>&nbsp;Please enter a valid URL. Protocol is required (http://, https://) Example : '.$base_url,
				
	);



	$form['submit']=array(
			'#type'=>'submit',
			'#value'=>'Save',
	);

	$form['cancel']=array(
			'#id'=>'privateeventbtncancel',
			'#type'=>'submit',
			'#value'=>'Cancel',
	);
	
	
	$form['divend']=array(
			'#type'=>'markup',
			'#markup'=>'</div>',
	);

	return $form;

}


function private_event_overview_form_validate($form, &$form_state){
	//var_dump('asdf'); exit();
}

function private_event_overview_form_submit($form, &$form_state){
	
	
	$dest_dir = file_default_scheme() . '://privateevents_media/overview/'; 
	$validators = array (
		'file_validate_extensions' => array ('jpg png gif')
	);


	$old_image_path=isset($form_state['values']['old_image_path']) ? $form_state['values']['old_image_path'] :'';
	
	$filename = '';
	$fileURI = '';
	if ($_FILES ['files'] ['name'] ['backgroundimage'] != '') {
		if(!empty($old_image_path)){
			drupal_unlink($old_image_path);
		}
	
	
		if ($file = file_save_upload ( 'backgroundimage', $validators, $dest_dir )) {
			$fileURI = $file->uri;
			db_delete ( 'file_managed' )->condition ( 'fid', $file->fid, '=' )->execute ();
			$filename = isset ( $file->filename ) ? $file->filename : "";
		} else {
			// form_set_error('', 'Could not upload file.');
		}
	}
	
	if(empty($fileURI)){
		//$background_img_path=$old_image_path;
		$fileURI=$old_image_path;
	}
	//$imagePath=$form_state['values']['imagePath'];
	//$imagePath=$fileURI;
	
	
	
	//var_dump($form_state['values']); exit();
	
	$data=array();
	$data['menu_name']=$form_state['values']['menu_name'];
	$data['background_img_path']=$fileURI;
	$data['widget_title']=$form_state['values']['widget_title'];
	$data['widget_description']=$form_state['values']['widget_description']["value"];
	$data['widget_button_name']=$form_state['values']['widget_button_name'];
	$data['widget_button_url']=$form_state['values']['widget_button_url'];
	
	
	$privateEventMenuList= new PrivateEventMenuList();
	$privateEventMenuList->eventId=$form_state['values'] ['eventId'] ;
	$privateEventMenuList->menuId=$form_state['values'] ['menuId'];
	$privateEventMenuList->data=serialize($data);
	
	
	
	if(!empty($form_state['values'] ['privateEventMenuId'])){
		$privateEventMenuList->privateEventMenuId=$form_state['values'] ['privateEventMenuId'];
		$privateEventMenuList->updatePrivateEventMenuList();
	}else{
		$privateEventMenuList->createPrivateEventMenuList();
	}
	
}

