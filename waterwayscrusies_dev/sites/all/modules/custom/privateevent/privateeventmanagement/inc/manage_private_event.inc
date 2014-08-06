<?php
function manage_private_event_status_page($eventId){
	
	
	
	if(isset($_GET['action'])){
		
		if($_GET['action']=="show"){
			PrivateEvents::updatePrivateEventDisplayStatus($eventId, 1);
		}
		
		if($_GET['action']=="hide"){
			PrivateEvents::updatePrivateEventDisplayStatus($eventId, 0);
				
		}
		
		
	}
	
	$path="private-events";
	drupal_goto($path); exit();
	//echo $eventId; exit();
	
}

//New Private Event
function new_private_event_page(){
	
	
	$id=false;
	return drupal_get_form("private_event_form",$id);
}


function manage_private_event_page($id){
	drupal_set_title('');
	return drupal_get_form("private_event_form",$id);
}

//Delete Action
function delete_private_event_page($id){
	echo "Need to implement Delete Action";
	exit();
	var_dump($id); exit();
}



function private_event_form($form, &$form_state,$id) {
	drupal_add_css(drupal_get_path('module', 'privateeventmanagement') . '/css/privateeventmanagement.css');
	$form=array();
	
	
	$menuId=0;
	
	$default_private_event_fields=array();
	$private_event_title='';
	$widget_image='';
	$eventNameSpecialText='';
	$widgetHoverText='';
	if (! empty ( $id ) && isset ( $id )) {
		$form_state ['page_action'] = "edit";
		$form_state ['private_event_id'] = $id;
		$privateEventMenus = PrivateEvents::getPrivateEventMenus($id );
		$default_private_event_fields=explode(',', $privateEventMenus->menusIds);
		$private_event_title=$privateEventMenus->eventName;
		$eventNameSpecialText=$privateEventMenus->eventNameSpecialText;
		$widget_image=$privateEventMenus->widgetImagePath;
		$widgetHoverText=$privateEventMenus->widgetHoverText;
		
		
	} else {
		$form_state ['page_action'] = "add";
	}
	
	
	//var_dump($default_private_event_fields); exit();
	
	
	
	if($form_state ['page_action']=="edit"){
		
		$menuHtml=PrivateEventHeaderMenu::getHeaderMenu($default_private_event_fields,$id,$menuId);
		$form['menuHtml']=array(
				'#type'=>'markup',
				'#markup'=>$menuHtml,
		);
	}
	
	
	
	//var_dump($default_private_event_fields); exit();
	
	$privateEventMenus=PrivateEventMenus::getPrivateEventMenuList();
	
	
	//var_dump($privateEventMenus); exit();
	$menus=array();
	
	foreach($privateEventMenus as $menu){
		$menus[$menu->menuId]=$menu->menuName;
	}
	
	
	$form['divstart']=array(
			'#type'=>'markup',
			'#markup'=>'<div class="overviewformbackground">',
	);
	
	
	
	$imgurl = $GLOBALS['base_url'] . "/sites/all/modules/custom/privateevent/privateeventmanagement/img/privateeventview.jpg";
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
	
	

	
	$form['eventName'] = array(
			//'#title' => t('Private Event Name').'<a href="#overviewmodal" role="button" class="" data-toggle="modal"><span class="icon-question-sign"></span></a>',
			'#title' => t('Private Event Category Name'),
			'#type' => 'textfield',
			'#default_value' =>$private_event_title,
			'#required'=>TRUE,
			
	);
	
	
// 	$form['eventNameSpecialText'] = array(
// 		'#title' => t('Special Text'),
// 		'#type' => 'textfield',
// 		'#default_value' =>$eventNameSpecialText,
// 		//'#required'=>TRUE,
				
// 	);
	
// 	$form['widgetHoverText'] = array(
// 			'#title' => t('Widget Hover Text'),
// 			'#type' => 'textfield',
// 			'#default_value' =>$widgetHoverText,
// 			'#required'=>TRUE,
// 			'#size'=>20,
	
// 	);
	
	//var_dump($default_private_event_fields); exit();
	$form['menusIds'] = array(
			'#title' => t('Menus'),
			'#type' => 'checkboxes',
			'#description' => t(''),
			'#options' => $menus,
			'#default_value' =>$default_private_event_fields,
			'#required' => TRUE,
	);
	
	
// 	if(!empty($widget_image) && file_exists($widget_image)){
// 		$form['widgetImagePreview']=array(
// 			'#type'=>'markup',
// 				'#markup'=>'<img style="width:200px;" class="img-polaroid" src="'.file_create_url($widget_image).'">'
// 		);
		
// 		$form['old_image_path']=array(
// 				'#type'=>'hidden',
// 				'#default_value'=>$widget_image
// 		);
		
		
		
// 	}
// 	$form['widget_image']=array(
// 		'#title' => t('Widget Background Image'),
// 		'#type'=>'file',
				
// 	);
	
	
	$private_event_url_alias = '';
	$urlaliaspid="";
	if (! empty ( $id )) {
		$urlsource = "private_events/category/" . $id;
		$urlAliasObject = UrlAlias::getUrlAliasDetailsBySource ( $urlsource );
	
		//var_dump($urlAliasObject); exit();
		if ($urlAliasObject) {
			$urlaliaspid = $urlAliasObject->pid;
			$private_event_url_alias = $urlAliasObject->alias;
		}
	}
	
	
	
	$form['urlaliaspid'] = array(
			'#id'=>'urlaliaspid',
			'#type' => 'textfield',
			'#title' => t('URL alias PID'),
			'#default_value' => $urlaliaspid,
			'#prefix'=>'<div style="display:none;">',
			'#suffix'=>'</div>',
			//'#required'=>TRUE,
	   
	);
	
	$form['private_event_url_alias'] = array(
			'#id'=>'private_event_url_alias',
			'#type' => 'textfield',
			'#title' => t('URL alias'),
			'#default_value' => $private_event_url_alias,
			'#required'=>TRUE,
			'#maxlength' => 255,
			'#description'=>'<span class="icon-info-sign"></span>&nbsp;Specify a name by which this private event data can be accessed by human readable name instead of number.'
	
	);
	
	$form['private_event_url_alias_hidden'] = array(
			'#id'=>'private_event_url_alias_hidden',
			'#type' => 'textfield',
			'#default_value' => $private_event_url_alias,
			'#prefix'=>'<div style="display:none;">',
			'#suffix'=>'</div>',
	);
	
	
	
	$form['urlaliasresult']=array(
		'#type'=>'markup',
		'#markup'=>'<div id="urlaliasresult"></div>'
	);
	
	
	
	$form['submit']=array(
		'#type'=>'submit',
		'#value'=>'Save',
	);
	
	$form['cancel']=array(
			'#limit_validation_errors' => array(),
			'#id'=>'privateeventbtncancel',
			'#type'=>'submit',
			'#value'=>'Cancel',
			'#submit' => array('private_event_cancel_submit'),
	);
	
	
	$form['divend']=array(
			'#type'=>'markup',
			'#markup'=>'</div>',
	);
	
	return $form;
	
}

function private_event_form_validate($form, &$form_state) {
	$private_event_url_alias=trim($form_state['values']['private_event_url_alias']);
	$private_event_url_alias_hidden=trim($form_state['values']['private_event_url_alias_hidden']);
	
	if($private_event_url_alias!=''){
		if($private_event_url_alias!=$private_event_url_alias_hidden){
			$result=UrlAlias::checkUrlAlias($private_event_url_alias);
			if ($result) {
				$data = 'The url alias ' . $private_event_url_alias . ' is already taken!.';
				form_set_error('private_event_url_alias',$data);
				
			}
		}
	}
	
}


function private_event_cancel_submit($form, &$form_state) {
	$path="private-events";
	$form_state['redirect'] = $path;
}





function private_event_form_submit($form, &$form_state) {
	
	
	
	//var_dump($form_state['values']['old_image_path']); exit();
// 	$dest_dir = file_default_scheme() . '://privateevents_media/private_events_widget_background/'; // Note: file_directory_path() was removed in Drupal 7.x. // $dest_dir contains the destination directory for the file.
// 	$validators = array (
// 			'file_validate_extensions' => array ('jpg png gif')
// 	);
	
	
// 	$filename = '';
// 	$fileURI = '';
// 	if ($_FILES ['files'] ['name'] ['widget_image'] != '') {
	
// 		$old_image_path=isset($form_state['values']['old_image_path']) ? $form_state['values']['old_image_path'] :'';
// 		if(!empty($old_image_path)){
// 			drupal_unlink($old_image_path);
// 		}
	
		
// 		if ($file = file_save_upload ( 'widget_image', $validators, $dest_dir )) {
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
	
	//var_dump($fileURI); exit();
	
	$privateEventId='';
	
	//Add Action
	if($form_state ['page_action']=="add"){
		$privateEvents= new PrivateEvents();
		$privateEvents->eventName=trim($form_state['values']['eventName']);
		//$privateEvents->eventNameSpecialText=trim($form_state['values']['eventNameSpecialText']);
		//$privateEvents->widgetHoverText=trim($form_state['values']['widgetHoverText']);
		//$privateEvents->widgetImagePath=$fileURI;
		$selectedMenusIdsArray=$form_state['values']['menusIds'];
		$menuIds=array();
		foreach($selectedMenusIdsArray as $key=>$value){
			if($value!="0"){
				$menuIds[]=$value;
			}
		}
		
		if(count($menuIds)>0){
			$privateEvents->menusIds=implode(',',$menuIds);
		}else{
			$privateEvents->menusIds='';
		}
		$res=(int)$privateEvents->createPrivateEvent();
		
		if($res>0){
			$private_event_url_alias =strtolower(trim(str_replace(" ","",$form_state['values']["private_event_url_alias"])));
			$urlaliaspid = $form_state['values']["urlaliaspid"];
			$urlAliasObject=new UrlAlias();
			$urlAliasObject->source='private_events/category/'.$res;
			$urlAliasObject->alias=$private_event_url_alias;
			$id=$urlAliasObject->createUrlAlias();
			$path="private-event/".$res."/manage";
			drupal_set_message("Saved successfully!");
			drupal_goto($path); exit();
		}else{
			drupal_set_message('Failed');
		}
	}
	
	if($form_state ['page_action']=="edit"){
		$privateEvents= new PrivateEvents();
		$privateEvents->eventId=$form_state ['private_event_id'];
		$privateEvents->eventName=trim($form_state['values']['eventName']);
		//$privateEvents->eventNameSpecialText=trim($form_state['values']['eventNameSpecialText']);
		//$privateEvents->widgetHoverText=trim($form_state['values']['widgetHoverText']);
		
		//$privateEvents->widgetImagePath=$fileURI;
		$selectedMenusIdsArray=$form_state['values']['menusIds'];
		$menuIds=array();
		foreach($selectedMenusIdsArray as $key=>$value){
			if($value!="0"){
				$menuIds[]=$value;
			}
		}
		
		if(count($menuIds)>0){
			$privateEvents->menusIds=implode(',',$menuIds);
		}else{
			$privateEvents->menusIds='';
		}
		
		$updatePrivateEventResult=$privateEvents->updatePrivateEvent();

		
		$urlAliasResult=0;
		$private_event_url_alias =strtolower(trim(str_replace(" ","",$form_state['values']["private_event_url_alias"])));
		$urlaliaspid = $form_state['values']["urlaliaspid"];
		if($urlaliaspid==""){
			if($private_event_url_alias!=""){
				$urlAliasObject=new UrlAlias();
				$urlAliasObject->source='private_events/category/'.$form_state ['private_event_id'];
				$urlAliasObject->alias=$private_event_url_alias;
				$urlAliasResult=$urlAliasObject->createUrlAlias();
			}
		}else{
			//update
			$urlAliasObject=new UrlAlias();
			$urlAliasObject->alias=$private_event_url_alias;
			$urlAliasObject->pid=$urlaliaspid;
			$urlAliasResult=$urlAliasObject->updateUrlAlias();
		}
		
		if($updatePrivateEventResult>0 || $urlAliasResult>0){
			drupal_set_message("Saved successfully!");
		}
	}
}

