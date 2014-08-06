<?php


function ajax_check_urlalias_existence_callback($form, &$form_state) {
	$commands = array ();
	
	$cruisealiassurl = $form_state ['values'] ['cruise_alias_url'];
	$cruisehiddenaliasurl = $form_state ['values'] ['cruise_alias_url_hidden'];
	$name=trim(str_replace(" ","",$cruisealiassurl));
	
	$data='';
	if($name!=''){
		if($cruisealiassurl!=$cruisehiddenaliasurl){
			$result=UrlAlias::checkUrlAlias($name);
			if ($result) {
				$data = 'The url alias ' . $name . ' is already taken!.';
				$commands [] = ajax_command_invoke('#cruise_alias_url', 'attr', array('value', ''));
				$commands [] = ajax_command_replace ( "#urlaliastatusdiv", "<div id='urlaliastatusdiv'><span class='error'>" . $data . "</span></div>" );
			
			}else{
				$commands [] = ajax_command_replace ( "#urlaliastatusdiv", "<div id='urlaliastatusdiv'><span class='error'>" . $data . "</span></div>" );
			}
		}else{
				$commands [] = ajax_command_replace ( "#urlaliastatusdiv", "<div id='urlaliastatusdiv'><span class='error'>" . $data . "</span></div>" );
		}
	}
	
	return array (
			'#type' => 'ajax',
			'#commands' => $commands
	);
}



function cruise_add() {
    $cruise = (object) array(
                'id' => '',
                'cruise_title' => '',
                'calendar_tag' => '',
                'cruise_type' => ''
    );
    $currentwindowid = time();
    $cruise->currentwindowid = $currentwindowid;

    return drupal_get_form('cruise_add_form', $cruise);
}

function cruise_add_form($form, &$form_state, $cruise) {

//    drupal_json_output($cruise);exit();
    $currentwindowid = $cruise->currentwindowid;
    drupal_add_js(drupal_get_path('module', 'cruise') . '/cruise.js');
    $formstatus = $cruise->id == '' ? "add" : "edit";

    $tabmenuhtml = '<ul class="nav nav-pills">
	    			<li class=""><a href="fees">Fees</a></li>
	    			<li class=""><a href="tickets">Tickets</a></li>
    				<li class=""><a href="enhancements">Enhancements</a></li>
    				<li class="active"><a href="#" class="active">Details<span class="element-invisible">(active tab)</span></a></li>
    			</ul>';

    /*
      $form['cruisetemplatetabs'] = array(
      '#type' => 'item',
      '#markup' =>$tabmenuhtml,
      );
     */


    $form['operation'] = array(
        '#type' => 'hidden',
        '#value' => $formstatus
    );

    $auto_id = $cruise->id != '' ? $cruise->id : "";
    $form['auto_id'] = array(
        '#type' => 'hidden',
        '#value' => $auto_id
    );

    $form['cruise_title'] = array(
        '#type' => 'textfield',
        '#title' => t('Cruise Title'),
        '#default_value' => isset($cruise->cruise_title) ? $cruise->cruise_title : '',
        '#required'=>TRUE,
    	'#prefix'=>'<div class="row-fluid"><div class="span4">',
    	'#suffix'=>'</div>',
    );
    
    $categoryOptions = '';
    $categoryOptions = CruiseCategory::getAllCategoriesList();
    $form['template_category'] = array(
    		'#type' => 'select',
    		'#title' => t('Template Category'),
    		'#default_value' => isset($cruise->template_category) ? $cruise->template_category : '',
    		'#options' => $categoryOptions,
    		'#prefix'=>'<div class="span4">',
    		'#suffix'=>'</div>',
    );
    
    $cruiseTypesObject = cruisetypes_load_multiple($ids = false);
    $cruisetypesarraylist = array();
    if (isset($cruiseTypesObject)) {
    	foreach ($cruiseTypesObject as $cruiseType) {
    		$cruisetypesarraylist[$cruiseType->id] = $cruiseType->name;
    	}
    }
    
    $form['cruise_type'] = array(
    		'#type' => 'select',
    		'#title' => t('Cruise Type'),
    		'#default_value' => isset($cruise->cruise_type) ? $cruise->cruise_type : '',
    		'#options' => $cruisetypesarraylist,
    		'#prefix'=>'<div class="span4">',
    		'#suffix'=>'</div></div>',
    );
    
    $form['cruise_status'] = array(
    		'#type' => 'checkbox',
    		'#title' => t('Active'),
    		'#default_value' => isset($cruise->cruise_status) ? $cruise->cruise_status : 1
    );
    
    
		
	// Getting the URL alias pid
	$urlaliaspid = '';
	$cruise_alias_url = '';
	if (! empty ( $auto_id )) {
		$urlsource = "cruisesdetailview/category/" . $auto_id;
		$urlAliasObject = UrlAlias::getUrlAliasDetailsBySource ( $urlsource );
		
		if ($urlAliasObject) {
			$urlaliaspid = $urlAliasObject->pid;
			$cruise_alias_url = $urlAliasObject->alias;
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
    
    
    $form['cruise_alias_url'] = array(
    		'#id'=>'cruise_alias_url',
    		'#type' => 'textfield',
    		'#title' => t('URL alias'),
    		'#ajax' => array (
    				'callback' => 'ajax_check_urlalias_existence_callback',
    		),
    		'#default_value' => $cruise_alias_url,
    		'#required'=>TRUE,
    		'#maxlength' => 255,
    		'#description'=>'<span class="icon-info-sign"></span>&nbsp;Specify a name by which this cruise template data can be accessed by human readable name instead of number.'
    		
    );
    
    
    $form['cruise_alias_url_hidden'] = array(
    		'#id'=>'cruise_alias_url_hidden',
    		'#type' => 'textfield',
    		'#default_value' => $cruise_alias_url,
    		'#prefix'=>'<div style="display:none;">',
    		'#suffix'=>'</div>',
    );
    
    
    
    
    // This entire form element will be replaced with an updated value.
    $form ['urlaliastatusdiv'] = array (
    		'#type' => 'markup',
    		'#markup' => '<div id="urlaliastatusdiv"></div>'
    );
    
    


   

   
    

    
    $form['calendarbackgroundimagandtag'] = array(
    		'#type' => 'fieldset',
    		'#title' => '<span class="icon-th-large"></span>&nbsp;'.t('Calendar Tag & Popup Background Image'),
    		
    );

    $form['calendarbackgroundimagandtag']['calendar_tag'] = array(
        '#type' => 'textfield',
        '#title' => t('Calendar Tag'),
    		'#required'=>TRUE,
        '#default_value' => isset($cruise->calendar_tag) ? $cruise->calendar_tag : '',
    	'#prefix'=>'<div class="row-fluid"><div class="span6">',
    	'#suffix'=>'</div>'
    );
 
    
  
    
    
    $tileimg = '';
    // If there is already an uploaded image display the image here.
    if ($cruise->calendar_popup_img_fid != "") {
    	try {
    		$image = file_load($cruise->calendar_popup_img_fid);
    		$url = file_create_url($image->uri);
    		$tileimg = '<img src="' . $url . '" width="150">';
    	} catch (Exception $e) {
    		watchdog('Cruise', "Error in loading Tile Image");
    	}
    }
    
    
    
    $form['calendarbackgroundimagandtag']['tile_img_preview'] = array(
    		'#type' => 'markup',
    		'#markup' => '<div class="span6">'.$tileimg,
    );
    
    $form['calendarbackgroundimagandtag']['calendar_popupimage'] = array(
    		'#name' => 'files[calendar_popupimage]',
    		'#type' => 'managed_file',
    		'#description' => '<span class="icon-hand-right"></span>&nbsp;'. t('Uploaded Image Size should be 470 x 250 pixels.'),
    		'#title' => t('Choose a picture for Calendar Popup Image'),
    		'#upload_location' => 'public://cruisetemplates/',
    		'#default_value' => isset($cruise->calendar_popup_img_fid) ? $cruise->calendar_popup_img_fid : '',
    		'#suffix'=>'</div></div><div style="margin-bottom:40px;"></div>',
    		
    );
    
  
    
    
    // //********************* Calendar Popup IMAGE **********************************//
    
    

    $form['tile'] = array(
    		'#type' => 'fieldset',
    		'#title' => '<span class="icon-th-large"></span>&nbsp;'.t('Tile'),
    
    );

    $form['tile']['tile_title'] = array(
        '#type' => 'textfield',
        '#title' => t('Tile Title'),
        '#default_value' => isset($cruise->tile_title) ? $cruise->tile_title : '',
    		'#prefix'=>'<div class="row-fluid"><div class="span3">',
    		'#suffix'=>'</div>'
    );






    // ********************* TILE IMAGE **********************************//
 
    $tileimg = '';
    // If there is already an uploaded image display the image here.
    if ($cruise->tile_img_fid != "") {
        try {
            $image = file_load($cruise->tile_img_fid);
            $url = file_create_url($image->uri);
            $tileimg = '<img src="' . $url . '" width="150">';
        } catch (Exception $e) {
            watchdog('Cruise', "Error in loading Tile Image");
        }
    }



    $form['tile']['tile_img_preview'] = array(
        '#type' => 'markup',
        '#markup' => '<div class="span3">'.$tileimg,
    );

    $form['tile']['tile_image'] = array(
        '#name' => 'files[tile_image]',
        '#type' => 'managed_file',
        '#description' => '<span class="icon-hand-right"></span>&nbsp;'.t('Uploaded Image Size should be 250 x 250 pixels.'),
        '#title' => t('Choose a picture for Tile Image'),
        '#upload_location' => 'public://cruisetemplates/',
        '#default_value' => isset($cruise->tile_img_fid) ? $cruise->tile_img_fid : '',
  
    );


   
//
//    $form['title_hover_description'] = array(
//        '#type' => 'textarea',
//        '#title' => t('Tile Hover Description'),
//        '#default_value' => isset($cruise->title_hover_description) ? $cruise->title_hover_description : '',
//        '#prefix' => '<br/>',
//        '#suffix' => '<br/>',
//    );
    //Title Hover description


    $form['tile']['title_hover_description'] = array(
        '#type' => 'text_format',
        '#title' => t('Tile Hover Description'),
        //'#format' => 'templateeditor',
        '#default_value' => isset($cruise->title_hover_description) ? $cruise->title_hover_description : '',
        '#prefix'=>'</div><div class="span6">',
    		'#suffix'=>'</div></div>'
    );












  



//    $form['menu_content'] = array(
//            '#type' => 'textarea',
//            '#title' => t('Menu Content'),
//            '#default_value' => isset($cruise->menu_content) ? $cruise->menu_content : ''
//      );





    /*
     * Right Slider Image Action 
     * The Images are stored in file_managed table , the fid values are stored as comma separated in our entity
     * Step 1 : if  $form_state['num_rightslider_imgs'] is empty we assigne value 1 , in Edit we assign number of images uploaded in the add action
     * Step 2:  Loading the Images Previously uploaded 
     * Step 3 : Reloading the images
     * 
     */
    
    $form['slidersstart']=array(
    	'#type'=>'markup',
    	'#markup'=>'<div style="margin-top:40px;"></div><div class="row-fluid">'
    );
    
    
    $form['sliderright']=array(
    		'#type'=>'markup',
    		'#markup'=>'<div class="span6">',
    );
    
    $form['right_slider'] = array(
        '#prefix' => '<div id="rightslider-fieldset-wrapper">',
        '#suffix' => '</div>',
        '#type' => 'fieldset',
        '#title' => '<span class="icon-picture"></span>&nbsp;'.t('Right Slider'),
//         '#collapsible' => TRUE,
//         '#collapsed' => FALSE,
    );

    //step 1
    if (empty($form_state['num_rightslider_imgs'])) {
        $rightSliderImgArray = explode(',', $cruise->rightslider_img_fids);
        $numofrightsliderimgs = count($rightSliderImgArray);
        $form_state['num_rightslider_imgs'] = ($numofrightsliderimgs > 0) ? $numofrightsliderimgs : 1;
    }

    for ($i = 0; $i < $form_state['num_rightslider_imgs']; $i ++) {
        try {
            if ($numofrightsliderimgs == $form_state ['num_rightslider_imgs']) {
                //Initial Preview
                //step 2
                if ($rightSliderImgArray [$i] != "") {
                    $image = file_load($rightSliderImgArray [$i]);
                    $url = file_create_url($image->uri);
                    $img = '<br/><img src="' . $url . '" width="14%" height="14%">';
                    $form ['right_slider'] ['rightsliderimgpreview' . $i] = array(
                        '#type' => 'markup',
                        '#markup' => $img
                    );
                }
            } else {
                //step 3
                $url = '';
                $fileid = $form_state['values']['rightsliderimg' . $i];
                if ($fileid != 0) {
                    $image = file_load($fileid);
                    $url = file_create_url($image->uri);
                    $img = '<br/><img src="' . $url . '" width="14%" height="14%">';
                    $form['right_slider']['rightsliderimgpreview' . $i] = array(
                        '#type' => 'markup',
                        '#markup' => $img
                    );
                }
            }
        } catch (Exception $e) {
            watchdog('Cruise', "Error in loading Right Slider Image" . $e);
        }


        $form['right_slider']['rightsliderimg' . $i] = array(
            '#name' => 'files[rightimg_' . $i . ']',
            '#type' => 'managed_file',
            //'#title' => t('Choose a picture for Right slider'),
            '#description' => '<span class="icon-hand-right"></span>&nbsp;'.t('Uploaded Image Size should be 799 x 765 pixels.'),
            '#upload_location' => 'public://cruisetemplates/',
            '#default_value' => $rightSliderImgArray[$i],
            "#upload_validators" => array(
                "file_validate_extensions" => "png gif jpg jpeg"
            ),
        );
    }

    $form['right_slider']['rightslider_addbutton'] = array(
        '#name' => 'right_slider',
        '#type' => 'submit',
        '#value' => t('Add one more file'),
        '#submit' => array('ajax_add_right_slider_submit'),
        '#ajax' => array(
            'callback' => 'ajax_add_right_slider_callback',
            'wrapper' => 'rightslider-fieldset-wrapper'
        )
    );
    //Right Slider Action Ends here

    $form['sliderrightend']=array(
    		'#type'=>'markup',
    		'#markup'=>'</div><div class="span6">',
    );

   
    /*
     * Middle Slider Image Action
     * The Images are stored in file_managed table , the fid values are stored as comma separated in our entity
     * Step 1 : if  $form_state['num_rightslider_imgs'] is empty we assigne value 1 , in Edit we assign number of images uploaded in the add action
     * Step 2:  Loading the Images Previously uploaded
     * Step 3 : Reloading the images
     *
     */
    $form['middle_slider'] = array(
        '#prefix' => '<div id="middleslider-fieldset-wrapper">',
        '#suffix' => '</div>',
        '#type' => 'fieldset',
        '#title' =>  '<span class="icon-picture"></span>&nbsp;'.t('Middle Slider'),
//         '#collapsible' => TRUE,
//         '#collapsed' => FALSE
    );

    if (empty($form_state['num_middleslider_imgs'])) {
        $middleSliderImgArray = explode(',', $cruise->middleslider_img_fids);
        $numofmiddlesliderimgs = count($middleSliderImgArray);
        $form_state['num_middleslider_imgs'] = ($numofmiddlesliderimgs > 0) ? $numofmiddlesliderimgs : 1;
    }

    for ($i = 0; $i < $form_state['num_middleslider_imgs']; $i ++) {

        try {
            if ($numofmiddlesliderimgs == $form_state ['num_middleslider_imgs']) {
                //Initial Preview
                //step 2
                if ($middleSliderImgArray [$i] != "") {
                    $image = file_load($middleSliderImgArray [$i]);
                    $url = file_create_url($image->uri);
                    $img = '<br/><img src="' . $url . '" width="14%" height="14%">';
                    $form ['middle_slider'] ['middlesliderimgpreview' . $i] = array(
                        '#type' => 'markup',
                        '#markup' => $img
                    );
                }
            } else {
                //step 3
                $url = '';
                $fileid = $form_state['values']['middlesliderimg' . $i];
                if ($fileid != 0) {
                    $image = file_load($fileid);
                    $url = file_create_url($image->uri);
                    $img = '<br/><img src="' . $url . '" width="14%" height="14%">';
                    $form['middle_slider']['middlesliderimgpreview' . $i] = array(
                        '#type' => 'markup',
                        '#markup' => $img
                    );
                }
            }
        } catch (Exception $e) {
            watchdog('Cruise', "Error in loading Middle Slider Image" . $e);
        }


        $form['middle_slider']['middlesliderimg' . $i] = array(
            '#name' => 'files[middleimg_' . $i . ']',
            '#type' => 'managed_file',
            //'#title' => t('Choose a picture for Middle Slider'),
            '#description' => '<span class="icon-hand-right"></span>&nbsp;'.t('Uploaded Image Size should be 1600 x 771 pixels.'),
            '#upload_location' => 'public://cruisetemplates/',
            '#default_value' => $middleSliderImgArray[$i]
        );
    }

    $form['middle_slider']['middleslider_addbutton'] = array(
        '#name' => 'middle_slider',
        '#type' => 'submit',
        '#value' => t('Add one more file'),
        '#submit' => array('ajax_add_middle_slider_submit'),
        '#ajax' => array(
            'callback' => 'ajax_add_middle_slider_callback',
            'wrapper' => 'middleslider-fieldset-wrapper'
        )
    );
    
    
    $form['middlesliderend']=array(
    		'#type'=>'markup',
    		'#markup'=>'</div></div><div style="margin-bottom:40px;"></div>',
    );
    
    
    $form['group_reservation_content'] = array(
    		'#type' => 'text_format',
    		'#title' => t('Group Reservation Content'),
    		//'#format' => 'templateeditor',
    		'#default_value' => isset($cruise->group_reservation_content) ? $cruise->group_reservation_content : '',
    		'#prefix' => '<div class="row-fluid"><div class="span6">',
    		'#suffix' => '</div>',
    );

    $form['middle_content'] = array(
        '#type' => 'text_format',
        '#title' => t('Middle Content'),
        //'#format' => 'templateeditor',
        '#default_value' => isset($cruise->middle_content) ? $cruise->middle_content : '',
        '#prefix' => '<div class="span6">',
    		'#suffix' => '</div></div><div style="margin-bottom:40px;"></div>',
    );




    $form['menulabels'] = array(
        '#type' => 'fieldset',
        '#title' => '<span class="icon-th"></span>&nbsp;'.t('Menu Label Settings'),
//         '#collapsible' => TRUE,
//         '#collapsed' => FALSE,
    );

    if (isset($cruise->id)) {
        $cruiseMenuLabelObject = CruiseCourse::getCourseMenuLabel($cruise->id);
        $toplevelmenulabel = isset($cruiseMenuLabelObject->menu_lable) ? $cruiseMenuLabelObject->menu_lable : '';
        $toplevelmenucaption = isset($cruiseMenuLabelObject->menu_caption) ? $cruiseMenuLabelObject->menu_caption : '';
        ;
    } else {
        $toplevelmenulabel = '';
        $toplevelmenucaption = '';
    }

    //Menu Label and Caption Details
    $form['menulabels']["toplevelmenulabel"] = array(
        '#id' => 'menulabel',
        "#type" => "textfield",
        "#title" => "Menu Label",
        '#default_value' => $toplevelmenulabel,
    );
    $form['menulabels']["toplevelmenucaption"] = array(
        '#id' => 'menulabel',
        "#type" => "textfield",
        "#title" => "Menu Label Caption",
        '#default_value' => $toplevelmenucaption,
    	'#suffix'=>'<div style="margin-bottom:40px;"></div>',
    );



//    //Menu Label and Caption Details
//     $form['menulabels']["toplevelmenulabel"] = array(
//        '#id' => 'menulabel',
//        "#type" => "textfield",
//        "#title" => "Menu Label"
//    );
//    $form['menulabels']["toplevelmenucaption"] = array(
//        '#id' => 'menulabel',
//        "#type" => "textfield",
//        "#title" => "Menu Label Caption"
//    );
    //End Menu Label and Caption Details





    /*
     * Menu (Add Course)
     */



//    if (!isset($_SESSION['temp_id'])) {
//        if ($cruise->id != '') {
//            $_SESSION['temp_id'] = $cruise->id;
//        } else {
//            $_SESSION['temp_id'] = time();
//        }
//    }




      $form['menucoursedivstart'] = array(
        '#type' => 'markup',
        '#markup' => '<div class="row-fluid"><div class="span6">',
    );
    $form['menuitem'] = array(
        '#type' => 'fieldset',
        '#title' => '<span class="icon-th"></span>&nbsp;'.t('Courses'),
//         '#collapsible' => TRUE,
//         '#collapsed' => FALSE
    );





//     $form['menuitem']['layout']=array(
//     		'#type'=>'select',
//     		'#options'=>array(''=>'Select Layout','1'=>'1','2'=>'2','3'=>'3'),
//     		'#title'=>'Choose Layout',
//     		'#default_value' => isset($cruise->menu_layout) ? $cruise->menu_layout : ''
//     );

//    var_dump($cruise->menu_layout);
//    exit();
    $layoutoptions = array('' => 'Select', '1' => '1 Column', '2' => '2 Column', '3' => '3 Column');
    //$layoutoptions = array('' => 'Select', '1' => '1 Column', '2' => '2 Column', '3' => '3 Column');
     
    $selectedOptioin = isset($form_state['values']['layout']) ? $form_state['values']['layout'] : $cruise->menu_layout;
    $form['menuitem']['layout'] = array(
        '#type' => 'select',
        '#title' => 'Choose Layout',
        '#options' => $layoutoptions,
        '#ajax' => array(
            'event' => 'change',
            'callback' => 'layoutajxactioncallback',
            'wrapper' => 'choose_column_replace',
        ),
        '#required'=>TRUE,
        '#default_value' => isset($cruise->menu_layout) ? $cruise->menu_layout : '1'
    );



    $form['menuitem']["coursename"] = array(
        '#id' => 'coursename',
        "#type" => "textfield",
        "#title" => "Course Name"
    );
    $courselabelshow = array("show" => t('Show Course Name'), "hide" => t('Hide Course Name'));
    $form['menuitem']['courselabelshow'] = array(
        '#type' => 'radios',
        '#title' => t(''),
        '#options' => $courselabelshow,
        '#default_value' => isset($coupon->courselabel_show) ? $coupon->courselabel_show : 'show',
        '#title_display' => 'invisible',
        '#attributes' => array('class' => array('radio_buttons optional')),
    );


//     $form['cruisehiddenvalue']=array(
//     		'#id'=>'cruisehiddenvalue',
//     		'#type'=>'textfield',
//     		'#value'=>'',
//     		'#prefix'=>'<div style="display:none;">',
//     		'#suffix'=>'</div>'
//     );


    $form['coursehiddenvalue'] = array(
        '#id' => 'coursehiddenvalue',
        '#type' => 'textfield',
        //'#value'=>'',
        '#prefix' => '<div style="display:none;">',
        '#suffix' => '</div>'
    );
    $form['menuitem']['currentwindowid'] = array(
        '#id' => 'currentwindowid',
        '#type' => 'textfield',
        '#value' => $currentwindowid,
        '#prefix' => '<div style="display:none;">',
        '#suffix' => '</div>'
    );






//     $form['menuitem']['columnname']=array(
//     		'#id'=>'columnname',
//     		'#type'=>'select',
//     		'#options'=>array(''=>'Select Column','1_Column'=>'1 Column','2_Column'=>'2 Column','3_Column'=>'3 Column'),
//     		'#title'=>'Select Column'
//     );

    $form['menuitem']['columnname'] = array(
        '#id' => 'columnname',
        '#type' => 'select',
        '#title' => 'Select Column',
        '#prefix' => '<div id="choose_column_replace">',
        '#suffix' => '</div>',
        '#options' => column_options($selectedOptioin),
    );




    $form['menuitem']['add_course'] = array(
        '#id' => 'add_course',
        '#type' => 'submit',
        '#value' => t('Add Course'),
        '#limit_validation_errors' => array(),
        '#submit' => array('ajax_add_course_submit'),
        '#ajax' => array(
            'callback' => 'ajax_add_course_callback',
        //'wrapper' => 'vocuhers-fieldset-wrapper',
        ),
    );

    $form['menuitem']['update_course'] = array(
        '#id' => 'update_course',
        '#type' => 'submit',
        '#value' => t('Update Course'),
        '#limit_validation_errors' => array(),
        '#submit' => array('ajax_update_course_submit'),
        '#ajax' => array(
            'callback' => 'ajax_update_course_callback',
        //'wrapper' => 'vocuhers-fieldset-wrapper',
        ),
    	'#limit_validation_errors' => array(),
            //'#attributes' => array('disabled' => 'disabled')
    );




    $form ["menuitem"]['resetcourse'] = array(
        '#type' => 'button',
        '#id' => 'resetcourse',
        '#value' => 'Reset',
            //'#attributes'=>array('onclick'=>'resetCourseForm()')
    );






    $form ['menuitem']['courselistdiv'] = array(
        '#type' => 'markup',
        '#markup' => '<div id="courselistdiv">' . getCourseTableData($currentwindowid) . '</div>'
    );





  $form['menucourseend'] = array(
    		'#type' => 'markup',
    		'#markup' => '</div><div class="span6">',
    );


 
    $form['menus'] = array(
        '#type' => 'fieldset',
        '#title' => '<span class="icon-th-list"></span>&nbsp;&nbsp;'.t('Menus'),
//         '#collapsible' => TRUE,
//         '#collapsed' => FALSE
    );
    ;




    $form["menus"]["menu_title"] = array(
        "#type" => "textfield",
        "#title" => "Menu Title",
        '#id' => 'menu_title',
    );

    $menulabelshow = array("show" => t('Show Menu Title'), "hide" => t('Hide Menu Title'));
    $form['menus']['menulabelshow'] = array(
        '#type' => 'radios',
        '#title' => t(''),
        '#options' => $menulabelshow,
        '#default_value' => isset($coupon->menulabel_show) ? $coupon->menulabel_show : 'show',
        '#title_display' => 'invisible',
        '#attributes' => array('class' => array('radio_buttons optional')),
            //'#prefix'=>'<div style="display:none;>',
            //'#suffix'=>'</div>',	
    );
    $form['coursemenuhiddenvalue'] = array(
        '#id' => 'coursemenuhiddenvalue',
        '#type' => 'textfield',
        //'#value'=>'',
        '#prefix' => '<div style="display:none;">',
        '#suffix' => '</div>'
    );



   

    $form["menus"]['selected_course'] = array(
        '#id' => 'selected_course',
        '#type' => 'select',
        '#title' => 'Select Course',
        '#prefix' => '<div id="dropdown-second-replace">',
        '#suffix' => '</div>',
        '#options' => get_dropdown_options($currentwindowid),
    	'#description'=>'<span class="icon-hand-right"></span>&nbsp;You dont have any course list in Drop down please click the Fetch Course List button'
    );
    
    $form["menus"]['add_to_dropdown'] = array(
    		'#type' => 'button',
    		'#value' => 'Fetch Course List',
    		'#ajax' => array(
    				'event' => 'click',
    				'callback' => 'dropdown_callback',
    				'wrapper' => 'dropdown-second-replace',
    		),
    		'#limit_validation_errors' => array(),
    );



    $form["menus"]['menu_image'] = array(
        '#id' => 'menu_image',
        '#type' => 'file',
        '#size' => 12,
        '#title' => t('Add Menu Image'),
    );



    $form["menus"]['description'] = array(
        '#id' => 'menudescription',
//        '#type' => 'textarea',
        '#type' => 'text_format',
        //'#format' => 'templateeditor',
        '#title' => 'Description',
        '#prefix' => '<br/>',
        '#suffix' => '<br/>',
    );
//    $form['title_hover_description'] = array(
//        '#type' => 'text_format',
//        '#title' => t('Tile Hover Description'),
//        '#format' => 'templateeditor',
//        '#default_value' => isset($cruise->title_hover_description) ? $cruise->title_hover_description : '',
//        '#prefix' => '<br/>',
//        '#suffix' => '<br/>',
//    );

    $form["menus"]['add_menu'] = array(
        '#type' => 'submit',
        '#value' => t('Add Menu'),
        '#limit_validation_errors' => array(),
        '#submit' => array('ajax_add_menu_submit'),
        '#ajax' => array(
            'callback' => 'ajax_add_menu_callback',
        //'wrapper' => 'vocuhers-fieldset-wrappers',
        ),
    	
    );

    $form['menus']['update_course_menu'] = array(
        '#id' => 'update_course_menu',
        '#type' => 'submit',
        '#value' => t('Update Menu'),
        '#limit_validation_errors' => array(),
        '#submit' => array('ajax_update_course_menu_submit'),
        '#ajax' => array(
            'callback' => 'ajax_update_course_menu_callback',
        //'wrapper' => 'vocuhers-fieldset-wrapper',
        ),
            //'#attributes' => array('disabled' => 'disabled')
    );


//     $form ["menus"]['resetcoursemenus'] = array (
//     		'#type' => 'button',
//     		'#id'=>'resetcoursemenus',
//     		'#value'=>'Reset',
//     		//'#attributes'=>array('onclick'=>'resetCourseForm()')
//     );


    $form ["menus"]['resetmenus'] = array(
        '#type' => 'button',
        '#id' => 'resetmenus',
        '#value' => 'Reset',
            //'#attributes'=>array('onclick'=>'resetmenuform()')
    );


    $form ["menus"]['menudiv'] = array(
        '#type' => 'markup',
        '#markup' => '<div id="menudiv">' . getMenuData($currentwindowid) . '</div>'
    );




    $form['menueend'] = array(
    		'#type' => 'markup',
    		'#markup' => '</div></div><div style="margin-bottom:40px;"></div>',
    );
    













    //Commented
//     $form['menu_content'] = array(
//         '#type' => 'textarea',
//         '#title' => t('Menu Content'),
//         '#default_value' => isset($cruise->menu_content) ? $cruise->menu_content : ''
//     );
//     $form['cruise_pdf'] = array(
//         '#name' => 'files[cruise_pdf]',
//         '#type' => 'managed_file',
//         '#title' => t('Cruise PDF'),
//         '#upload_location' => 'public://cruisetemplates/',
//         '#default_value' => isset($cruise->cruise_pdf_fid) ? $cruise->cruise_pdf_fid : ''
//     );

    $form['cruisemedia']=array(
    		'#type'=>'markup',
    		'#markup'=>'<div class="row-fluid"><div class="span4">'
    );


    $form['cruise_pdfs'] = array(
        '#prefix' => '<div id="cruisepdf-fieldset-wrapper">',
        '#suffix' => '</div>',
        '#type' => 'fieldset',
        '#title' => '<span class="icon-file"></span>&nbsp;'.t('Cruise PDF'),
//         '#collapsible' => TRUE,
//         '#collapsed' => FALSE
    );

    if (empty($form_state['num_cruise_pdfs'])) {
        $cruisePdfArray = explode(',', $cruise->cruise_pdf_fid);
        $numofcruisepdfs = count($cruisePdfArray);
        $form_state['num_cruise_pdfs'] = ($numofcruisepdfs > 0) ? $numofcruisepdfs : 1;
    }

    for ($i = 0; $i < $form_state['num_cruise_pdfs']; $i ++) {
        $form['cruise_pdfs']['cruisepdf' . $i] = array(
            '#name' => 'files[cruisepdf' . $i . ']',
            '#type' => 'managed_file',
            '#title' => t(''),
            '#upload_location' => 'public://cruisetemplates/',
            '#default_value' => $cruisePdfArray[$i]
        );
    }

    $form['cruise_pdfs']['cruisepdf_addbutton'] = array(
        '#name' => 'cruise_pdf',
        '#type' => 'submit',
        '#value' => t('Add one more file'),
        '#submit' => array('ajax_add_cruise_pdf_submit'),
        '#ajax' => array(
            'callback' => 'ajax_add_cruise_pdfs_callback',
            'wrapper' => 'cruisepdf-fieldset-wrapper'
        )
    );

    $form['cruisemediapdfend']=array(
    		'#type'=>'markup',
    		'#markup'=>'</div><div class="span4">'
    );


    /*
     * Image Gallery Action
     * The Images are stored in file_managed table , the fid values are stored as comma separated in our entity
     * Step 1 : if  $form_state['num_gallery_imgs'] is empty we assigne value 1 , in Edit we assign number of images uploaded in the add action
     * Step 2:  Loading the Images Previously uploaded
     * Step 3 : Reloading the images
     *
     */
    $form['cruise_gallery'] = array(
        '#prefix' => '<div id="gallery-fieldset-wrapper">',
        '#suffix' => '</div>',
        '#type' => 'fieldset',
        '#title' => '<span class="icon-picture"></span>&nbsp;'.t('Cruise Gallery'),
//         '#collapsible' => TRUE,
//         '#collapsed' => FALSE
    );

    if (empty($form_state['num_gallery_imgs'])) {
        $imgGalleryArray = explode(',', $cruise->cruise_gallery_img_fids);
        $numofgalleryimgs = count($imgGalleryArray);
        $form_state['num_gallery_imgs'] = ($numofgalleryimgs > 0) ? $numofgalleryimgs : 1;
    }

    for ($i = 0; $i < $form_state['num_gallery_imgs']; $i ++) {
        try {
            if ($numofgalleryimgs == $form_state ['num_gallery_imgs']) {
                //Initial Preview
                //step 2
                if ($imgGalleryArray [$i] != "") {
                    $image = file_load($imgGalleryArray [$i]);
                    $url = file_create_url($image->uri);
                    $img = '<br/><img src="' . $url . '" width="14%" height="14%">';
                    $form ['cruise_gallery'] ['imggallerypreview' . $i] = array(
                        '#type' => 'markup',
                        '#markup' => $img
                    );
                }
            } else {
                //step 3
                $url = '';
                $fileid = $form_state['values']['galleryimg' . $i];
                if ($fileid != 0) {
                    $image = file_load($fileid);
                    $url = file_create_url($image->uri);
                    $img = '<br/><img src="' . $url . '" width="14%" height="14%">';
                    $form['cruise_gallery']['imggallerypreview' . $i] = array(
                        '#type' => 'markup',
                        '#markup' => $img
                    );
                }
            }
        } catch (Exception $e) {
            watchdog('Cruise', "Error in loading Cruise Gallery Image" . $e);
        }



        $form['cruise_gallery']['galleryimg' . $i] = array(
            '#name' => 'files[galleryimg_' . $i . ']',
            '#type' => 'managed_file',
            //'#title' => t('Choose a picture for Cruise Gallery'),
            '#title' => t(''),
            '#upload_location' => 'public://cruisetemplates/',
            '#default_value' => $imgGalleryArray[$i]
        );
    }

    $form['cruise_gallery']['galleryimg_addbutton'] = array(
        '#name' => 'cruise_gallery',
        '#type' => 'submit',
        '#value' => t('Add one more file'),
        '#submit' => array('ajax_add_gallery_img_submit'),
        '#ajax' => array(
            'callback' => 'ajax_add_gallery_img_callback',
            'wrapper' => 'gallery-fieldset-wrapper'
        )
    );

    $form['cruisemediagalleryend']=array(
    		'#type'=>'markup',
    		'#markup'=>'</div><div class="span4">'
    );
    $form['cruise_video_url'] = array(
        '#prefix' => '<div id="cruisevideourl-fieldset-wrapper">',
        '#suffix' => '</div>',
        '#type' => 'fieldset',
        '#title' => '<span class="icon-facetime-video"></span>&nbsp;'.t('Cruise Video URL'),
//         '#collapsible' => TRUE,
//         '#collapsed' => FALSE
    );



    $cruiseVideoUrlsArray = unserialize($cruise->cruise_video_urls);
    if (empty($form_state['num_cruise_video_url'])) {
        //$imgGalleryArray = explode(',', $cruise->cruise_gallery_img_fids);
        $numofvideourls = count($cruiseVideoUrlsArray);
        $form_state['num_cruise_video_url'] = ($numofvideourls > 0) ? $numofvideourls : 1;
    }

    for ($i = 0; $i < $form_state['num_cruise_video_url']; $i ++) {
        $form['cruise_video_url']['cruise_video_url' . $i] = array(
            //'#name' => 'files[cruise_video_url_' . $i . ']',
            '#type' => 'textfield',
            //'#title' => t('Choose a picture for Cruise Gallery'),
            '#title' => t(''),
            //'#upload_location' => 'public://cruisetemplates/',
            '#default_value' => isset($cruiseVideoUrlsArray[$i]) ? $cruiseVideoUrlsArray[$i] : ''
        );


//     	$form['cruise_video_url'] = array(
//     			'#type' => 'textfield',
//     			'#title' => t('Cruise Video URL'),
//     			'#default_value' => isset($cruise->cruise_video_url) ? $cruise->cruise_video_url : ''
//     	);
    }

    $form['cruise_video_url']['cruisevideourl_addbutton'] = array(
        '#name' => 'cruise_video_url',
        '#type' => 'submit',
        '#value' => t('Add one more url'),
        '#submit' => array('ajax_add_video_url_submit'),
        '#ajax' => array(
            'callback' => 'ajax_add_video_url_callback',
            'wrapper' => 'cruisevideourl-fieldset-wrapper'
        )
    );
    $form['cruisemediaurlend']=array(
    		'#type'=>'markup',
    		'#markup'=>'</div></div><div style="margin-bottom:40px;"></div>'
    );
    /*
     * Loading Keywords and Description
     */

    //$auto_id
    $keywords = '';
    $description = '';
    $m_title = '';

    if ($auto_id != '') {
        $metaobj = MetaTag::getMetaTagsByID($auto_id, 'cruise');
        $keywords = $metaobj->keywords;
        $description = $metaobj->description;
        $m_title = $metaobj->title;
    }
    
    
    
    
    $form['metainfo'] = array(
    		'#type' => 'fieldset',
    		'#title' => '<span class="icon-globe"></span>&nbsp;'.t('Metadata'),

    );


    $form['metainfo']['meta_title'] = array(
        '#type' => 'textfield',
        '#title' => t('Meta Title'),
        '#default_value' => $m_title,
    	'#prefix'=>'<div class="row-fluid"><div class="span12">',
    	'#suffix'=>'</div></div>',
    );


    $form['metainfo']['meta_keywords'] = array(
        '#type' => 'textarea',
        '#title' => t('Meta Keywords'),
        //'#default_value' => isset($cruise->search_keywords) ? $cruise->search_keywords : ''
        '#default_value' => $keywords,
    		'#prefix'=>'<div class="row-fluid"><div class="span6">',
    		'#suffix'=>'</div>',
    );

    $form['metainfo']['meta_description'] = array(
        '#type' => 'textarea',
        '#title' => t('Meta Description'),
        //'#default_value' => isset($cruise->meta_description) ? $cruise->meta_description : '',
        '#default_value' => $description,
    		'#prefix'=>'<div class="span6">',
    		'#suffix'=>'</div></div>',
    );

    field_attach_form('cruise', $cruise, $form, $form_state);
    $form['submit'] = array(
        '#type' => 'submit',
        '#value' => t('Save')
    );



    $form ['cancel'] = array(
        '#type' => 'submit',
        '#value' => t('Cancel'),
        '#submit' => array('cruise_frm_cancel'),
        '#limit_validation_errors' => array()
    );


    return $form;
}

function cruise_frm_cancel($form, &$form_state) {
    unset($_SESSION['temp_id']);
    $url = 'admin/structure/cruise/manage';
    drupal_goto($url);
}

function layoutajxactioncallback($form, $form_state) {
    return $form['menuitem']['columnname'];
}

function column_options($key = 'colors') {
    $options = array(
        '0' => array(
            '' => '-Select-',
        ),
        '1' => array(
            '1_Column' => 'Column 1',
        ),
        '2' => array(
            '1_Column' => 'Column 1',
            '2_Column' => 'Column 2',
        ),
        '3' => array(
            '1_Column' => 'Column 1',
            '2_Column' => 'Column 2',
            '3_Column' => 'Column 3',
        ),
    );

    return $options[$key];
}

function dropdown_callback($form, $form_state) {
    return $form["menus"]['selected_course'];
}

//function get_dropdown_options() {
function get_dropdown_options($currentwindowid) {

    $data = '';
    try {
        $data = db_select('cruise_course_list', 'c')
                ->fields('c')
                ->condition('cruise_id', $currentwindowid, '=')
                ->execute()
                ->fetchAll();
    } catch (Exception $e) {
        
    }


    $arr = array(''=>'-Select-');
    for ($i = 0; $i < count($data); $i++) {
        $arr[$data[$i]->id] = $data[$i]->course_name;
    }

    return $arr;
}

/*
 * Course 
 */

function ajax_add_course_submit($form, &$form_state) {
    // rebuild the form after submitted successfully
    $form_state['rebuild'] = TRUE;
}

function ajax_update_course_submit($form, &$form_state) {
    // rebuild the form after submitted successfully
    $form_state['rebuild'] = TRUE;
}

function ajax_update_course_menu_submit($form, &$form_state) {
    // rebuild the form after submitted successfully
    $form_state['rebuild'] = TRUE;
}

function ajax_add_menu_submit($form, &$form_state) {

    // rebuild the form after submitted successfully
    $form_state['rebuild'] = TRUE;
}

function ajax_add_course_callback($form, $form_state) {
//
//
//    var_dump($form_state['values']['currentwindowid']);
//    exit();
    $currentwindowid = $form_state['values']['currentwindowid'];
    $coursename = trim($form_state['values']['coursename']);
    $columnname = $form_state['values']['columnname'];
    $courselabelshow = $form_state['values']['courselabelshow'];


  //  if (empty($coursename) || empty($columnname)) {
    //    $commands[] = ajax_command_alert("Please fill all the fields ");
   // } else {
        $courseObject = new CruiseCourse();
        $courseObject->course_name = $coursename;
        $courseObject->column_name = $columnname;
        $courseObject->courselabel_show = $courselabelshow;
//        $courseObject->cruise_id = $_SESSION['temp_id'];
        $courseObject->cruise_id = $currentwindowid;
        $courseObject->status = 0;

        $courseObject->createCruiseCourses();



        //End of Creating the Cruise Menu Label option.
   // }
// 	$query=db_insert('cruise_course_list');
// 	$query->fields(array(
// 			'course_name'=>$form_state['values']['coursename'],
// 			'column_name'=>$form_state['values']['columnname'],
// 			'cruise_id'=>$_SESSION['temp_id'] ,
// 			'status'=>0,
// 	));
    //$id=$query->execute();
//    $retdata = CruiseCourse::getTableFormattedDatabyCruiseID($_SESSION['temp_id']);
    $retdata = CruiseCourse::getTableFormattedDatabyCruiseID($currentwindowid);
    $commands[] = ajax_command_replace("#courselistdiv", "<div id='courselistdiv'>" . $retdata . "</div>");
//    $commands[] = ajax_command_invoke('#edit-layout', 'attr', array('value', '0'));
//    $commands[] = ajax_command_invoke('#coursename', 'attr', array('value', ''));
//    $commands[] = ajax_command_invoke('#columnname', 'attr', array('value', ''));
    return array(
        '#type' => 'ajax',
        '#commands' => $commands
    );
}

function ajax_update_course_callback($form, &$form_state) {


    $currentwindowid = $form_state['values']['currentwindowid'];
    $coursename = trim($form_state['values']['coursename']);
    $columnname = $form_state['values']['columnname'];
    $courselabelshow = $form_state['values']['courselabelshow'];



    if (empty($coursename) || empty($columnname)) {
        $commands[] = ajax_command_alert("Please fill all the fields ");
    } else {
        $courseObject = new CruiseCourse();
        $courseObject->course_id = $form_state['values']['coursehiddenvalue'];
        $courseObject->course_name = $form_state['values']['coursename'];
        $courseObject->column_name = $form_state['values']['columnname'];
        $courseObject->courselabel_show = $form_state['values']['courselabelshow'];
        //$courseObject->status=1;
        $courseObject->updateCruiseCourses();
    }



// 	$num_updated=db_update('cruise_course_list')
// 	->fields(array(
// 			'course_name'=>$form_state['values']['coursename'],
// 			'column_name'=>$form_state['values']['columnname'],
// 			'status'=>1,
// 	))
// 	->condition('id',$form_state['values']['coursehiddenvalue'],'=')
// 	->execute();
    $retdata = getCourseTableData($currentwindowid);

    $commands [] = ajax_command_replace("#courselistdiv", "<div id='courselistdiv'>" . $retdata . "</div>");
    return array(
        '#type' => 'ajax',
        '#commands' => $commands
    );
}

function ajax_update_course_menu_callback($form, &$form_state) {
//    var_dump($form_state['values']['description']['value']);
//    exit();
    
    
    $currentwindowid = $form_state['values']['currentwindowid'];
    $fileURI = '';
    if (isset($form_state['values']['menu_title'])) {

        $dest_dir = file_default_scheme() . '://crusemenuimages/'; // Note: file_directory_path() was removed in Drupal 7.x. // $dest_dir contains the destination directory for the file.
        $validators = array('file_validate_extensions' => array('jpg png gif'));
        if ($_FILES['files']['name']['menu_image'] != '') {
            $filename = '';
            if ($file = file_save_upload('menu_image', $validators, $dest_dir)) {
                $fileURI = $file->uri;

                db_delete('file_managed')->condition('fid', $file->fid, '=')->execute();
                $filename = isset($file->filename) ? $file->filename : "";
            } else {
                //form_set_error('', 'Could not upload file.');
            }
        }
    }

//    if (empty($form_state['values']['menu_title']) || empty($form_state['values']['selected_course']) || empty($form_state['values']['description'])) {
//        $commands[] = ajax_command_alert("Please fill all the fields ");
//    } else {
    $menuid = $form_state['values']['coursemenuhiddenvalue'];
    $menuObject = new CruiseCourseMenus();
    $menuObject->menu_title = isset($form_state['values']['menu_title']) ? $form_state['values']['menu_title'] : '';
    $menuObject->course_name = isset($form_state['values']['selected_course']) ? $form_state['values']['selected_course'] : '';
//    $menuObject->description = isset($form_state['values']['description']) ? $form_state['values']['description'] : '';
    $menuObject->description = isset($form_state['values']['description']['value']) ? $form_state['values']['description']['value'] : '';
    $menuObject->menulabel_show = isset($form_state['values']['menulabelshow']) ? $form_state['values']['menulabelshow'] : '';
//        $menuObject->cruise_id = $_SESSION['temp_id'];
    $menuObject->cruise_id = $currentwindowid;
    $menuObject->image_path = $fileURI;
    //$menuObject->status=0;
    $menuObject->id = $menuid;
    $menuObject->updateCruiseCourseMenu();
//    }
// 	$menuid=$form_state['values']['coursemenuhiddenvalue'];
// 	$num_updated=db_update('cruise_menus_list')
// 	->fields(array(
// 			'menu_title'=>$form_state['values']['menu_title'],
// 			'course_name'=>$form_state['values']['selected_course'],
// 			'description'=>$form_state['values']['description'],
// 			'image_path'=>$fileURI,
// 			'cruise_id'=>$_SESSION['temp_id'] ,
// 			'status'=>0,
// 	))
// 	->condition('id',$menuid,'=')
// 	->execute();
    $retdata = getMenuData($currentwindowid);


    $commands [] = ajax_command_replace("#menudiv", "<div id='menudiv'>" . $retdata . "</div>");
    return array(
        '#type' => 'ajax',
        '#commands' => $commands
    );
}

//function getCourseTableData() {
function getCourseTableData($currentwindowid) {
//    $data = CruiseCourse::getTableFormattedDatabyCruiseID($_SESSION['temp_id']);
    $data = CruiseCourse::getTableFormattedDatabyCruiseID($currentwindowid);
    return $data;
}

function ajax_add_menu_callback($form, $form_state) {
    $currentwindowid = $form_state['values']['currentwindowid'];
    $selected_course=$form_state['values']['selected_course'];
    
    $CruiseCourseStatus=CruiseCourse::getAllCourseDetailsbyCurrentCruiseID($currentwindowid);
    
    if($CruiseCourseStatus && $selected_course!=""){
    	//    $courseid=isset($form_state['input']['selected_course']) ? $form_state['values']['selected_course'] : '';
    	//    $data = CruiseCourse::getAllCourseDetailsbyCurrentCruiseID($courseid);
    	
    	if ($currentwindowid == '') {
    		$currentwindowid = $form['menuitem']['currentwindowid']['#value'];
    	}
    	
    	//    var_dump($form_state['values']);
    	//    exit();
    	$dest_dir = file_default_scheme() . '://crusemenuimages/'; // Note: file_directory_path() was removed in Drupal 7.x. // $dest_dir contains the destination directory for the file.
    	$validators = array('file_validate_extensions' => array('jpg png gif'));
    	if ($_FILES['files']['name']['menu_image'] != '') {
    		$filename = '';
    		$fileURI = '';
    	
    		if ($file = file_save_upload('menu_image', $validators, $dest_dir)) {
    			$fileURI = $file->uri;
    			db_delete('file_managed')->condition('fid', $file->fid, '=')->execute();
    			$filename = isset($file->filename) ? $file->filename : "";
    		} else {
    			//form_set_error('', 'Could not upload file.');
    		}
    	}
    	
    	$menuObject = new CruiseCourseMenus();
    	//    $menuObject->menu_title = isset($form_state['values']['menu_title']) ? $form_state['values']['menu_title'] : '';
    	//    $menuObject->course_name = isset($form_state['values']['selected_course']) ? $form_state['values']['selected_course'] : '';
    	//    $menuObject->description = isset($form_state['values']['description']) ? $form_state['values']['description'] : '';
    	$menuObject->menu_title = isset($form["menus"]['menu_title']['#value']) ? $form["menus"]['menu_title']['#value'] : '';
    	$menuObject->course_name = isset($form["menus"]['selected_course']['#value']) ? $form["menus"]['selected_course']['#value'] : '';
    	//    $menuObject->description = isset($form["menus"]['description']['#value']) ? $form["menus"]['description']['#value'] : '';
    	$menuObject->description = isset($form_state['values']['description']['value']) ? $form_state['values']['description']['value'] : '';
    	$menuObject->menulabel_show = isset($form["menus"]['menulabelshow']['#value']) ? $form["menus"]['menulabelshow']['#value'] : '';
    	//    $menuObject->cruise_id = $_SESSION['temp_id'];
    	$menuObject->cruise_id = $currentwindowid;
    	$menuObject->image_path = $fileURI;
    	$menuObject->status = 0;
    	$menuObject->createCruiseCourseMenu();
    	$retdata = getMenuData($currentwindowid);
    	$commands [] = ajax_command_replace("#menudiv", "<div id='menudiv'>" . $retdata . "</div>");
    	//    $commands [] = ajax_command_invoke('#menu_title', 'attr', array('value', ''));
    	//    $commands [] = ajax_command_invoke('#selected_course', 'attr', array('value', ''));
    	//    $commands [] = ajax_command_invoke('#menu_image', 'attr', array('value', ''));
    	//    $commands [] = ajax_command_invoke('#menudescription', 'attr', array('value', ''));
    	
    }else{
    	$commands[] = ajax_command_alert("Please click the Update Course button once");
    }


    return array(
        '#type' => 'ajax',
        '#commands' => $commands
    );
}

function getMenuData($currentwindowid) {
//    $data = CruiseCourseMenus::getMenuTableFormattedDatabyCruiseID($_SESSION['temp_id']);
    $data = CruiseCourseMenus::getMenuTableFormattedDatabyCruiseID($currentwindowid);
    return $data;
}

function cruise_add_form_validate($form, &$form_state) {

   
    $cruise = (object) $form_state['values'];
    field_attach_form_validate('cruise', $cruise, $form, $form_state);
}

function cruise_add_form_submit($form, &$form_state) {
	
	
	
	
	

//    var_dump($form_state['values']['currentwindowid']);
//    exit();
    $currentwindowid = $form_state['values']['currentwindowid'];
    $cruise = new stdClass();
    $auto_id = $form_state['values']['auto_id'];
    $cruise->id = $auto_id;

    $cruise->cruise_title = $form_state['values']['cruise_title'];
    $cruise->calendar_tag = $form_state['values']['calendar_tag'];
    $cruise->cruise_type = $form_state['values']['cruise_type'];
    $cruise->tile_title = $form_state['values']['tile_title'];
    $cruise->cruise_status = $form_state['values']['cruise_status'];
    $cruise->template_category = $form_state['values']['template_category'];
//    $cruise->group_reservation_content = $form_state['values']['group_reservation_content'];
    $cruise->group_reservation_content = $form_state['values']['group_reservation_content']["value"];







    //cruise_status
    // Tile Image Action
    $tileimgfid = '';
    if ($form_state['values']['tile_image'] != "") {
        $validators = array();
        $dest = file_default_scheme() . '://cruisetemplates/';
        
        
       

        
        
        
        
        
        $file = file_save_upload('tile_image', $validators, $dest);
        if (is_null($file)) {
            $file = file_load($form_state['values']['tile_image']);
            $file->status = "1";
            $fileobject = file_save($file);
            $tileimgfid = $fileobject->fid;

            // file usuage
            $fileusuage = new stdClass();
            $fileusuage->fid = $fileobject->fid;
            file_usage_add($fileusuage, 'file', 'cruise', '111');
        } else {
            $file->status = "1";
            $fileobject = file_save($file);
            $tileimgfid = $fileobject->fid;

            // file usuage
            $fileusuage = new stdClass();
            $fileusuage->fid = $fileobject->fid;
            file_usage_add($fileusuage, 'file', 'cruise', '111');
        }
    }

    $cruise->tile_img_fid = $tileimgfid;


    //calendar popup image
    //cruise_status
    // Tile Image Action
    $calendar_popup_img_fid = '';
    if ($form_state['values']['calendar_popupimage'] != "") {
        $validators = array();
        $dest = file_default_scheme() . '://cruisetemplates/';
        $file = file_save_upload('calendar_popupimage', $validators, $dest);
        if (is_null($file)) {
            $file = file_load($form_state['values']['calendar_popupimage']);
            $file->status = "1";
            $fileobject = file_save($file);
            $calendar_popup_img_fid = $fileobject->fid;

            // file usuage
            $fileusuage = new stdClass();
            $fileusuage->fid = $fileobject->fid;
            file_usage_add($fileusuage, 'file', 'cruise', '111');
        } else {
            $file->status = "1";
            $fileobject = file_save($file);
            $calendar_popup_img_fid = $fileobject->fid;

            // file usuage
            $fileusuage = new stdClass();
            $fileusuage->fid = $fileobject->fid;
            file_usage_add($fileusuage, 'file', 'cruise', '111');
        }
    }

    $cruise->calendar_popup_img_fid = $calendar_popup_img_fid;







//    $cruise->title_hover_description = $form_state['values']['title_hover_description'];
    $cruise->title_hover_description = $form_state['values']['title_hover_description']["value"];

    // Right Slider Image Action
    $right_slider_file_ids = array();
    $rightimgcount = $form_state['num_rightslider_imgs'];
    for ($i = 0; $i < $rightimgcount; $i ++) {
        $validators = array();
        $dest = file_default_scheme() . '://cruisetemplates/';
        if ($form_state['values']['rightsliderimg' . $i] != "") {
            $file = file_save_upload('rightimg_' . $i, $validators, $dest);
            if (is_null($file)) {
                $file = file_load($form_state['values']['rightsliderimg' . $i]);
                $file->status = "1";
                $fileobject = file_save($file);

                // file usuage
                $fileusuage = new stdClass();
                $fileusuage->fid = $fileobject->fid;
                file_usage_add($fileusuage, 'file', 'cruise', '111');

                $right_slider_file_ids[] = $fileobject->fid;
            } else {
                $file->status = "1";
                $fileobject = file_save($file);

                // file usuage
                $fileusuage = new stdClass();
                $fileusuage->fid = $fileobject->fid;
                file_usage_add($fileusuage, 'file', 'cruise', '111');
                $right_slider_file_ids[] = $fileobject->fid;
            }
        }
    }
    $cruise->rightslider_img_fids = implode(',', $right_slider_file_ids);

    // Middle Slider Image Action
    $middle_slider_file_ids = array();
    $middleimgcount = $form_state['num_middleslider_imgs'];
    for ($i = 0; $i < $middleimgcount; $i ++) {
        $validators = array();
        $dest = file_default_scheme() . '://cruisetemplates/';
        if ($form_state['values']['middlesliderimg' . $i] != "") {
            $file = file_save_upload('middleimg_' . $i, $validators, $dest);
            if (is_null($file)) {
                $file = file_load($form_state['values']['middlesliderimg' . $i]);
                $file->status = "1";
                $fileobject = file_save($file);
                // file usuage
                $fileusuage = new stdClass();
                $fileusuage->fid = $fileobject->fid;
                file_usage_add($fileusuage, 'file', 'cruise', '111');

                $middle_slider_file_ids[] = $fileobject->fid;
            } else {
                $file->status = "1";
                $fileobject = file_save($file);

                // file usuage
                $fileusuage = new stdClass();
                $fileusuage->fid = $fileobject->fid;
                file_usage_add($fileusuage, 'file', 'cruise', '111');
                $middle_slider_file_ids[] = $fileobject->fid;
            }
        }
    }
    $cruise->middleslider_img_fids = implode(',', $middle_slider_file_ids);

//    $cruise->middle_content = $form_state['values']['middle_content'];

    $cruise->middle_content = $form_state['values']['middle_content']['value'];
    $cruise->menu_content = $form_state['values']['menu_content'];

    // PDF Action
//     $cruise_pdf_fid = '';
//     if ($form_state['values']['cruise_pdf'] != "") {
//         $validators = array();
//         $dest = file_default_scheme() . '://cruisetemplates/';
//         $file = file_save_upload('cruise_pdf', $validators, $dest);
//         if (is_null($file)) {
//             $file = file_load($form_state['values']['cruise_pdf']);
//             $file->status = "1";
//             $fileobject = file_save($file);
//             // file usuage
//             $fileusuage = new stdClass();
//             $fileusuage->fid = $fileobject->fid;
//             file_usage_add($fileusuage, 'file', 'cruise', '111');
//             $cruise_pdf_fid = $fileobject->fid;
//         } else {
//             $file->status = "1";
//             $fileobject = file_save($file);
//             // file usuage
//             $fileusuage = new stdClass();
//             $fileusuage->fid = $fileobject->fid;
//             file_usage_add($fileusuage, 'file', 'cruise', '111');
//             $cruise_pdf_fid = $fileobject->fid;
//         }
//     }
//     $cruise->cruise_pdf_fid = $cruise_pdf_fid;
    // Cruise PDF Action
    $cruisepdf_file_ids = array();
    $cruisepdfcount = $form_state['num_cruise_pdfs'];
    for ($i = 0; $i < $cruisepdfcount; $i ++) {
        $validators = array();
        $dest = file_default_scheme() . '://cruisetemplates/';
        if ($form_state['values']['cruisepdf' . $i] != "") {
            $file = file_save_upload('cruisepdf_' . $i, $validators, $dest);
            if (is_null($file)) {
                $file = file_load($form_state['values']['cruisepdf' . $i]);
                $file->status = "1";
                $fileobject = file_save($file);
                // file usuage
                $fileusuage = new stdClass();
                $fileusuage->fid = $fileobject->fid;
                file_usage_add($fileusuage, 'file', 'cruise', '111');

                $cruisepdf_file_ids[] = $fileobject->fid;
            } else {
                $file->status = "1";
                $fileobject = file_save($file);

                // file usuage
                $fileusuage = new stdClass();
                $fileusuage->fid = $fileobject->fid;
                file_usage_add($fileusuage, 'file', 'cruisepdf', '111');

                $cruisepdf_file_ids[] = $fileobject->fid;
            }
        }
    }
    $cruise->cruise_pdf_fid = implode(',', $cruisepdf_file_ids);































    // Cruise Image Gallery Action
    $gallerimage_file_ids = array();
    $imggallerycount = $form_state['num_gallery_imgs'];
    for ($i = 0; $i < $imggallerycount; $i ++) {
        $validators = array();
        $dest = file_default_scheme() . '://cruisetemplates/';
        if ($form_state['values']['galleryimg' . $i] != "") {
            $file = file_save_upload('galleryimg_' . $i, $validators, $dest);
            if (is_null($file)) {
                $file = file_load($form_state['values']['galleryimg' . $i]);
                $file->status = "1";
                $fileobject = file_save($file);
                // file usuage
                $fileusuage = new stdClass();
                $fileusuage->fid = $fileobject->fid;
                file_usage_add($fileusuage, 'file', 'cruise', '111');

                $gallerimage_file_ids[] = $fileobject->fid;
            } else {
                $file->status = "1";
                $fileobject = file_save($file);

                // file usuage
                $fileusuage = new stdClass();
                $fileusuage->fid = $fileobject->fid;
                file_usage_add($fileusuage, 'file', 'cruise', '111');

                $gallerimage_file_ids[] = $fileobject->fid;
            }
        }
    }
    $cruise->cruise_gallery_img_fids = implode(',', $gallerimage_file_ids);

    //num_cruise_video_url
    $cruiseurlcount = $form_state['num_cruise_video_url'];
    $urlarray = array();
    for ($i = 0; $i < $cruiseurlcount; $i ++) {
        if ($form_state['values']['cruise_video_url' . $i] != '') {
            $urlarray[$i] = $form_state['values']['cruise_video_url' . $i];
        }
    }
    $cruise->cruise_video_urls = serialize($urlarray);
    $cruise->search_keywords = $form_state['values']['meta_keywords'];
    $cruise->menu_layout = $form_state['values']['layout'];






    $cruise->created = REQUEST_TIME;

    $cruiseObject = cruise_save($cruise);

//cruise menu label saving

    $coursemenulabel = $form_state['values']["toplevelmenulabel"];
    $coursemenucaption = $form_state['values']["toplevelmenucaption"];



    //Creating the Cruise Menu Label Option
    $menulabel = new CruiseCourse();
    $menulabel->coursemenulabel = $coursemenulabel;
    $menulabel->coursemenucaption = $coursemenucaption;
    $menulabel->cruise_id = $cruiseObject->id;

    $menulabel->createCruiseCoursesMenuLabel();

//end of cruise label saving









    /*
     * update course $_SESSION['temp_id']
     * 
     */
    //$tempid=$form_state['tempid'];


    try {
        $query = db_update('cruise_course_list');
        $query->fields(array(
            'cruise_id' => $cruise->id,
            'status' => 1,
        ));
//        $query->condition('cruise_id', $_SESSION['temp_id'], '=');
        $query->condition('cruise_id', $currentwindowid, '=');
        $num_updated = $query->execute();



        $query = db_update('cruise_menus_list');
        $query->fields(array(
            'cruise_id' => $cruise->id,
            'status' => 1,
        ));
//        $query->condition('cruise_id', $_SESSION['temp_id'], '=');
        $query->condition('cruise_id', $currentwindowid, '=');
        $num_updated = $query->execute();

        unset($_SESSION['temp_id']);
    } catch (Exception $e) {
        
    }







    //URL alias
    $cruise_alias_url =strtolower(trim(str_replace(" ","",$form_state['values']["cruise_alias_url"])));
    $urlaliaspid = $form_state['values']["urlaliaspid"];
    
    if($urlaliaspid==""){
    	//insert
    	if($cruise_alias_url!=""){
    		
    		
    		$urlAliasObject=new UrlAlias();
    		$urlAliasObject->source='cruisesdetailview/category/'.$cruise->id;
    		$urlAliasObject->alias=$cruise_alias_url;
    		$id=$urlAliasObject->createUrlAlias();
    		
    		
    		
    		
    		
    	}
    }else{
    	//update
    	$urlAliasObject=new UrlAlias();
    	$urlAliasObject->alias=$cruise_alias_url;
    	$urlAliasObject->pid=$urlaliaspid;
    	$urlAliasObject->updateUrlAlias();
    	
    }
    
  



    /*
     * Save the Meta information (Keywords) in Metatags Table
     */
    $aid = MetaTag::checkMetainfoExistence($cruiseObject->id, 'cruise');
    $metaObj = new MetaTag();
    $metaObj->id = $aid;
    $metaObj->reference_id = $cruise->id;
    $metaObj->keywords = $form_state['values']['meta_keywords'];
    $metaObj->description = $form_state['values']['meta_description'];
    $metaObj->category = "cruise";
    $metaObj->title = $form_state['values']['meta_title'];
    $metaObj->storeMetaTags($metaObj);

    $operation = $form_state['values']['operation'];
    $msg = '';
    if ($operation == "add") {
        $msg = 'Cruise Created Successfully.';
    } else {
        $msg = 'Cruise Updated Successfully.';
    }

    drupal_set_message($msg);

//    $form_state['redirect'] = 'admin/structure/cruise/manage';
    $form_state['redirect'] = 'cruise/' . $cruiseObject->id . '/edit';
}

/*
 * Right Slider Image action starts here
 */

function ajax_add_right_slider_callback($form, $form_state) {
    return $form['right_slider'];
}

function ajax_add_right_slider_submit($form, &$form_state) {
    $form_state['num_rightslider_imgs'] ++;
    $form_state['rebuild'] = TRUE;
}

/*
 * Middle Slider Image action starts here
 */

function ajax_add_middle_slider_callback($form, $form_state) {
    return $form['middle_slider'];
}

function ajax_add_middle_slider_submit($form, &$form_state) {
    $form_state['num_middleslider_imgs'] ++;
    $form_state['rebuild'] = TRUE;
}

function ajax_add_gallery_img_callback($form, $form_state) {
    return $form['cruise_gallery'];
}

function ajax_add_gallery_img_submit($form, &$form_state) {
    $form_state['num_gallery_imgs'] ++;
    $form_state['rebuild'] = TRUE;
}

function ajax_add_video_url_callback($form, $form_state) {
    return $form['cruise_video_url'];
}

function ajax_add_video_url_submit($form, &$form_state) {
    $form_state['num_cruise_video_url'] ++;
    $form_state['rebuild'] = TRUE;
}

function ajax_add_cruise_pdfs_callback($form, $form_state) {
    return $form['cruise_pdfs'];
}

function ajax_add_cruise_pdf_submit($form, &$form_state) {
    $form_state['num_cruise_pdfs'] ++;
    $form_state['rebuild'] = TRUE;
}

//Ajax addmore field action ends here


