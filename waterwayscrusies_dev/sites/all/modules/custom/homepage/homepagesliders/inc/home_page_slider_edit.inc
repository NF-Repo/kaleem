<?php


function home_page_slider_edit($sliderid){
     return drupal_get_form ("home_page_slider_edit_form",$sliderid);
}


function home_page_slider_edit_form($from, &$form_state, $sliderid ){
        drupal_add_css(drupal_get_path('module', 'homepagesliders') . '/css/homepagesliders.css');
	$form = array ();
       $geteditdata=new homepagesliders();
      $getslidereditfields =$geteditdata->gethomePageSliderEdit($sliderid);
    
       $getdatasliders=unserialize($getslidereditfields['data']);
        $form['sliderid']= array(
          '#type' =>'hidden',
          '#default_value'=> isset($sliderid)?$sliderid:'',
        );
        
         
         
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
        '#value'=> isset($getslidereditfields['sliderlayout'])?$getslidereditfields['sliderlayout']:'',
        '#title' => t('Slider Contain')
      );
        
      $form['sliderimage'] = array(
         '#type' => 'managed_file',
         '#description' => '<span class="icon-hand-down"><p style="font-size:12px">Uploaded Image Size should be 1327 x 624 pixels. </p></span>',
         '#upload_location' => 'public://home-page-sliders/',
         '#title' =>t('Slider Image'),
         '#prefix' => '<div class="homesliderborder">',
         '#suffix'=>'</div>',
         '#default_value'=> isset($getdatasliders['sliderimage'])?$getdatasliders['sliderimage']:'',
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
        '#default_value'=> isset($getdatasliders['Slidertitle'])?$getdatasliders['Slidertitle']:'',
     );
       
      $form['slidecontent']['Sliderspecialtext']=array(
       '#type'=>'textfield',
       '#title'=>t("Slider Special Text"),
        '#prefix' => '<div class="span6">',
        '#suffix' => '</div>',
        '#default_value'=> isset($getdatasliders['Sliderspecialtext'])?$getdatasliders['Sliderspecialtext']:'',
      );
        
       $form['Slidercontent']=array(
       '#type'=>'text_format',
       '#title'=>t("Slider Content"),
        '#default_value'=> isset($getdatasliders['Slidercontent']['value'])?$getdatasliders['Slidercontent']['value']:'',
        
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
        '#default_value'=> isset($getdatasliders['Buttontext'])?$getdatasliders['Buttontext']:'',
      );
        
       $form['buttoncontent']['Buttonurl']=array(
       '#type'=>'textfield',
       '#title'=>t("Button Url Link"),
        '#prefix' => '<div class="span6">',
        '#suffix' => '</div>',
         '#default_value'=> isset($getdatasliders['Buttonurl'])?$getdatasliders['Buttonurl']:'',
         '#description'=> '<span class="icon-hand-down"></span><div style="font-size:12px">Please enter a valid URL. Protocol is required (http://, https://) Example : http://wwc.lsnsoft.com</div>'
           
      );
       
          $form['links'] = array(
          '#type' => 'fieldset',
           '#title' => t('Links'),
           '#collapsible' => TRUE,
            '#collapsed' => TRUE,
           '#tree' => TRUE,
       );
       
       $form['links']['firsttext']=array(
            '#type'=>'textfield',
            '#title'=>t("First Link Text"),
             '#maxlength' => 30, 
            '#default_value'=> isset($getdatasliders['links']['firsttext'])?$getdatasliders['links']['firsttext']:'',
             '#description' => '<span class="icon-hand-down"></span><div style="font-size:12px">Text cannot be longer than 30 characters</div>', 
             '#prefix' => '<div class = "row-fluid" style="margin-top:20px"><div class="span6">',
              '#suffix' => '</div>',
          
      );
        
       $form['links']['firsturl']=array(
            '#type'=>'textfield',
            '#title'=>t("First  Url Link"),
             '#prefix' => '<div class="span6">',
            '#default_value'=> isset($getdatasliders['links']['firsturl'])?$getdatasliders['links']['firsturl']:'',
             '#suffix' => '</div> </div>',
             '#description'=> '<span class="icon-hand-down"></span><div style="font-size:12px">Please enter a valid URL. Protocol is required (http://, https://) Example : http://wwc.lsnsoft.com</div>',
      );
       
        $form['links']['secondtext']=array(
            '#type'=>'textfield',
            '#title'=>t("Second Link Text"),
             '#maxlength' => 30, 
             '#default_value'=> isset($getdatasliders['links']['secondtext'])?$getdatasliders['links']['secondtext']:'',
             '#description' => '<span class="icon-hand-down"></span><div style="font-size:12px">Text cannot be longer than 30 characters</div>', 
             '#prefix' => '<div class = "row-fluid"><div class="span6">',
             '#suffix' => '</div>',
      );
       
           
       $form['links']['secondurl']=array(
            '#type'=>'textfield',
             '#title'=>t("Second  Url Link"),
             '#prefix' => '<div class="span6">',
             '#default_value'=> isset($getdatasliders['links']['secondurl'])?$getdatasliders['links']['secondurl']:'',
             '#suffix' => '</div> </div>',
            '#description'=>'<span class="icon-hand-down"></span><div style="font-size:12px"> Please enter a valid URL. Protocol is required (http://, https://) Example : http://wwc.lsnsoft.com, For Request A Proposal Enter #myModalrequest" role="button" data-toggle="modal</div>',
           
      );
        
       $form ['submit'] = array(
            '#type' => 'submit',
            '#value' => t('Submit'),
            '#weight' => 1,
          );
     $form['tabmenuhtml'] = array(
                '#type' => 'item',
               // '#prefix' => '<ul class="nav nav-pills">',
                 '#markup' =>  homepagenav::getHomeSlider(),
        );
      $form['divslidend']=array(
			'#type'=>'item',
			'#markup'=>'</div>',
	);
	
     $sliderhelpimage = $GLOBALS['base_url'] . "/sites/all/modules/custom/homepagesliders/img/homesliderhelp.jpeg";
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

function home_page_slider_edit_form_submit($from, &$form_state){
    
    $sliderid=$form_state['values']['sliderid'];
    $sliderlayout=$form_state['values']['addhomesliders'];
    $sliderimage=file_load($form_state['values']['sliderimage'])->uri;
    $sliderdata=serialize($form_state['values']);
    $editsliderobj=new homepagesliders();
    $editsliderobj->getsliderupdata($sliderid,$sliderlayout,$sliderimage,$sliderdata);
     $path="homepagedrag";
     drupal_goto($path);
     
}


