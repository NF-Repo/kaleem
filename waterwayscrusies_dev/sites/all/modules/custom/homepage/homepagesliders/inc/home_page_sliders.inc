<?php

/* 
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

function home_page_add_sliders(){
    return drupal_get_form ("home_page_add_sliders_form");
}


function home_page_add_sliders_form($from, &$form_state){
    drupal_add_css(drupal_get_path('module', 'homepagesliders') . '/css/homepagesliders.css');
	global $base_url;
    
    $form = array ();
    
         
         
      $form['divslidstart']=array(
			'#type'=>'item',
			'#markup'=>'<div class="sliderdivborder">',
	);
	
      $form['slidertitles']=array(
		'#type'=>'markup',
		'#markup'=>'<h2>Home Sliders &nbsp;&nbsp;<a href="#HomeModal" role="button" class="" data-toggle="modal"><span class="icon-question-sign"></span></a></h2>'
	);
      
        $form['addhomesliders']=array(
	'#type' => 'select',
         '#prefix' => '<div class="homesliderborder">',
         '#suffix'=>'</div>',
        '#options' => array(
                   'Left'   => t('Left Slider'),
                   'Right' => t('Right Slider'),
                   ),
        '#title' => t('Slider Type')
      );
        
      $form['sliderimage'] = array(
         '#type' => 'managed_file',
         '#description' => '<span class="icon-hand-down"><p style="font-size:12px">Uploaded Image Size should be 1327 x 624 pixels. </p></span>',
         '#upload_location' => 'public://home-page-sliders/',
         '#title' =>t('Slider Image'),
         '#prefix' => '<div class="homesliderborder">',
         '#suffix'=>'</div>',
         '#required'=>true,
      
      );
      
        $form['slidecontent'] = array(
          '#type' => 'fieldset',
          '#prefix' => '<div class="row-fluid">',
       );
      
       $form['slidecontent']['Slidertitle']=array(
       '#type'=>'textfield',
       '#title'=>t("Slider Title"),
        '#prefix' => '<div class="span6">',
        '#suffix' => '</div>',
     );
       
      $form['slidecontent']['Sliderspecialtext']=array(
       '#type'=>'textfield',
       '#title'=>t("Slider Special Text"),
        '#prefix' => '<div class="span6">',
        '#suffix' => '</div>',
      );
        
       $form['Slidercontent']=array(
       '#type'=>'text_format',
       '#title'=>t("Slider Content"),
        
      );
       
        $form['buttoncontent'] = array(
          '#type' => 'fieldset',
          '#prefix' => '<div class="row-fluid">',
       );
       
       $form['buttoncontent']['Buttontext']=array(
       '#type'=>'textfield',
       '#title'=>t("Button Text"),
        '#prefix' => '<div class="span6">',
        '#suffix' => '</div>',
      );
        
       $form['buttoncontent']['Buttonurl']=array(
       '#type'=>'textfield',
       '#title'=>t("Button Url Link"),
        '#prefix' => '<div class="span6">',
        '#suffix' => '</div>',
         '#description'=> '<span class="icon-hand-down"></span><div style="font-size:12px">Please enter a valid URL. Protocol is required (http://, https://) Example : '.$base_url.'</div>'
      );
       
          $form['links'] = array(
          '#type' => 'fieldset',
           '#title' => t('URL\'s'),
           '#collapsible' => TRUE,
            '#collapsed' => TRUE,
           '#tree' => TRUE,
       );
       
       $form['links']['firsttext']=array(
            '#type'=>'textfield',
            '#title'=>t("URL # 1 Button Name"),
             '#maxlength' => 30, 
             '#description' => '<span class="icon-hand-down"></span><div style="font-size:12px">Text cannot be longer than 30 characters</div>', 
             '#prefix' => '<div class = "row-fluid" style="margin-top:20px"><div class="span6">',
              '#suffix' => '</div>',
          
      );
        
       $form['links']['firsturl']=array(
            '#type'=>'textfield',
            '#title'=>t("URL # 1 Button Link"),
             '#prefix' => '<div class="span6">',
             '#suffix' => '</div> </div>',
             '#description'=> '<span class="icon-hand-down"></span><div style="font-size:12px">Please enter a valid URL. Protocol is required (http://, https://) Example :'.$base_url.'</div>',
      );
       
        $form['links']['secondtext']=array(
            '#type'=>'textfield',
            '#title'=>t("URL # 2 Button Name"),
             '#maxlength' => 30, 
             '#description' => '<span class="icon-hand-down"></span><div style="font-size:12px">Text cannot be longer than 30 characters</div>', 
             '#prefix' => '<div class = "row-fluid"><div class="span6">',
             '#suffix' => '</div>',
      );
       
           
       $form['links']['secondurl']=array(
            '#type'=>'textfield',
             '#title'=>t("URL # 2 Button Link"),
             '#prefix' => '<div class="span6">',
             '#suffix' => '</div> </div>',
            '#description'=>'<span class="icon-hand-down"></span><div style="font-size:12px"> Please enter a valid URL. Protocol is required (http://, https://) Example : '.$base_url.'</div>',
           
      );
        
       $form ['submit'] = array(
            '#type' => 'submit',
            '#value' => t('Submit'),
            //'#weight' => 1,
          );
       
       $form['backbtn']=array(
       		'#type'=>'markup',
       		'#markup'=>'<a class="btn" href="'.$base_url.'/homepagedrag"> Back </a>'
       );
//        $form['tabmenuhtml'] = array(
//                 '#type' => 'item',
//                // '#prefix' => '<ul class="nav nav-pills">',
//                  '#markup' =>  homepagenav::getHomeSlider(),
//       );

      
     
      $form['divslidend']=array(
			'#type'=>'item',
			'#markup'=>'</div>',
	);
	
     $sliderhelpimage = $GLOBALS['base_url'] . "/sites/all/modules/custom/homepage/homepagesliders/img/sliderdemo.png";
     $form['slidermodal']=array(
			'#type'=>'markup',
			'#markup'=>'
				<div id="HomeModal" class="modal hide fade" tabindex="-1" role="dialog" aria-labelledby="priceTableModalLabel" aria-hidden="true">
				<div class="modal-header">
				<button type="button" class="close" data-dismiss="modal" aria-hidden="true">x</button>
				<h3>Home Slider</h3>
				</div>
				<div class="modal-body">
				<img src="' . $sliderhelpimage . '" alt="" />
				</div>
				<div class="modal-footer">
				<button class="btn" data-dismiss="modal" aria-hidden="true">Close</button>
				</div>
				</div>'
	);
     
     
        return $form;
}

function home_page_add_sliders_form_submit($from, &$form_state){
    
    $layout=$form_state['values']['addhomesliders'];
    $getimage=file_load($form_state['values']['sliderimage']);
    
    $imagepath=$getimage->uri;

    $sliderdata=Serialize($form_state['values']);
    
    $homepagesliderobject=new homepagesliders();
    
    $homepagesliderobject->sliderlayout=$layout;
    
    $homepagesliderobject->imagepath=$imagepath;
    
    $homepagesliderobject->data=$sliderdata;
    
    $homepagesliderobject->insert_homepage_slider();
     $path="homepagedrag";
     drupal_goto($path);
    
}

//moduel popups

