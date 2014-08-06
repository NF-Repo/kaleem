<?php


function home_page_content_list(){
	drupal_set_title('Front Page Configuration');
  return drupal_get_form ("home_page_content_list_form");
}


function home_page_content_list_form($from, &$form_state){
	
    drupal_add_css(drupal_get_path('module', 'homepagesliders') . '/css/homepagesliders.css');
    
	$form = array ();
    
         $form['tabmenuhtml'] = array(
                '#type' => 'item',
                 '#markup' =>  Homepagenav::getHomeStaticMenu(),

      );
      
       $gethomestaticdata=unserialize(homepagecontent::gethomepagecontent());
       $form['divslidstart']=array(
			'#type'=>'item',
			'#markup'=>'<div class="sliderdivborder">',
	);
	
        $form['homecontent']=array(
		'#type'=>'markup',
		'#markup'=>'<h2>Front Page Content &nbsp;&nbsp;<a href="#Homecontent" role="button" class="" data-toggle="modal"><span class="icon-question-sign"></span></a></h2>'
	);
        
      $form['connectcontent']=array(
       '#type'=>'text_format',
       '#title'=>t("Connect Description"),
        '#default_value'=> isset($gethomestaticdata['connectcontent']['value'])?$gethomestaticdata['connectcontent']['value']:''
        
      );
         $form['facebookcontent']=array(
       '#type'=>'text_format',
       '#title'=>t("Facebook Description"),
       '#prefix' => '<div class = "row-fluid"><div class="span6">',
       '#suffix' => '</div>',
        '#default_value'=> isset($gethomestaticdata['facebookcontent']['value'])?$gethomestaticdata['facebookcontent']['value']:''
       ); 
         
         $form['twittercontent']=array(
       '#type'=>'text_format',
       '#title'=>t("Twitter Description"),
       '#prefix' => '<div class="span6">',
       '#suffix' => '</div> </div>',
       '#default_value'=> isset($gethomestaticdata['twittercontent']['value'])?$gethomestaticdata['twittercontent']['value']:''
      );
         
       $form['pinterestcontent']=array(
        '#type'=>'text_format',
       '#title'=>t("Pinterest Description"),
        '#prefix' => '<div class = "row-fluid"><div class="span6">',
       '#suffix' => '</div>',
       '#default_value'=> isset($gethomestaticdata['pinterestcontent']['value'])?$gethomestaticdata['pinterestcontent']['value']:''
      );
         
        $form['youtubecontent']=array(
        '#type'=>'text_format',
       '#title'=>t("Youtube Description"),
       '#prefix' => '<div class="span6">',
       '#suffix' => '</div> </div>',
             
       '#default_value'=> isset($gethomestaticdata['youtubecontent']['value'])?$gethomestaticdata['youtubecontent']['value']:''
      );
     
      
     
       
       
     
       //first box content starts here
       
       $form['boxfirst'] = array(
        '#type' => 'fieldset',
        '#title' => t('First box Content'),
        '#collapsible' => TRUE,
        '#collapsed' => TRUE,
        '#tree' => TRUE,
      );
       
       $form['boxfirst']['firstboxtitle']=array(
       '#type'=>'textfield',
       '#title'=>t("First Box title"),
       '#maxlength' => 15, 
       '#prefix' => '<div class = "row-fluid"><div class="span4">',
       '#suffix' => '</div>',
        '#description' => t('Tittle cannot be longer than 15 characters'), 
       '#default_value'=> isset($gethomestaticdata['boxfirst']['firstboxtitle'])?$gethomestaticdata['boxfirst']['firstboxtitle']:''
      );
        
      $form['boxfirst']['firstboxspecial']=array(
       '#type'=>'textfield',
       '#title'=>t("First Box Special"),
        '#maxlength' => 15, 
         '#prefix' => '<div class="span4">',
         '#suffix' => '</div>',
        '#description' => t('Specialtext cannot be longer than 15 characters'), 
    
       '#default_value'=> isset($gethomestaticdata['boxfirst']['firstboxspecial'])?$gethomestaticdata['boxfirst']['firstboxspecial']:''
      );
         
       $form['boxfirst']['firstboxlink']=array(
       '#type'=>'textfield',
       '#title'=>t("Firstboxlink"),
        '#prefix' => '<div><div class="span4">',
       '#suffix' => '</div></div>',
       '#description'=> t('Please enter a valid URL. Protocol is required (http://, https://) Example : http://wwc.lsnsoft.com'),
       '#default_value'=> isset($gethomestaticdata['boxfirst']['firstboxlink'])?$gethomestaticdata['boxfirst']['firstboxlink']:''
    
      );
       
       $form['boxfirst']['firstcontent']=array(
       '#type'=>'textarea',
       '#title'=>t("First Hover Description"),
        '#maxlength' => 80, 
        '#prefix' => '<div><div class="span12">',
        '#suffix' => '</div></div>',
        '#description' => t('Hover Description cannot be longer than 80 characters'),  
        '#default_value'=> isset($gethomestaticdata['boxfirst']['firstcontent'])?$gethomestaticdata['boxfirst']['firstcontent']:''
           
      );
       //logo of first box
         $form['boxfirst']['firstcontentlogo']=array(
         '#type' => 'managed_file',
         '#prefix' => '<div class = "row-fluid">',
         '#suffix' => '</div>',
         '#description' => '<span class="icon-hand-down"><p style="font-size:12px">Uploaded Inner logo Image Size should be 165 x 61 pixels. </p></span>',
         '#upload_location' => 'public://home-page-sliders/',
         '#default_value'=> isset($gethomestaticdata['boxfirst']['firstcontentlogo'])?$gethomestaticdata['boxfirst']['firstcontentlogo']:''
           
      );
     
       ////end of first box content starts here
      
       //second box content starts here
       
         $form['boxsecond'] = array(
        '#type' => 'fieldset',
        '#title' => t('Second Box Content'),
        '#collapsible' => TRUE,
        '#collapsed' => TRUE,
        '#tree' => TRUE,
      );
       
       $form['boxsecond']['secondboxtitle']=array(
       '#type'=>'textfield',
       '#title'=>t("Second Box Title"),
       '#maxlength' => 15, 
        '#prefix' => '<div class = "row-fluid"><div class="span4">',
        '#suffix' => '</div>',
        '#description' => t('Tittle cannot be longer than 15 characters'),  
       '#default_value'=> isset($gethomestaticdata['boxsecond']['secondboxtitle'])?$gethomestaticdata['boxsecond']['secondboxtitle']:''
      );
        
      $form['boxsecond']['secondboxspecial']=array(
       '#type'=>'textfield',
       '#title'=>t("Second Box Special"),
       '#maxlength' => 15, 
       '#prefix' => '<div class="span4">',
        '#suffix' => '</div>',
        '#description' => t('Specialtext cannot be longer than 15 characters'),  
       '#default_value'=> isset($gethomestaticdata['boxsecond']['secondboxspecial'])?$gethomestaticdata['boxsecond']['secondboxspecial']:''
      );
         
       $form['boxsecond']['secondboxlink']=array(
       '#type'=>'textfield',
       '#title'=>t("Secondboxlink"),
       '#prefix' => '<div><div class="span4">',
       '#suffix' => '</div></div>',
       '#description'=> t('Please enter a valid URL. Protocol is required (http://, https://) Example : http://wwc.lsnsoft.com'),
        '#default_value'=> isset($gethomestaticdata['boxsecond']['secondboxlink'])?$gethomestaticdata['boxsecond']['secondboxlink']:''
    
      );
       
       $form['boxsecond']['secondcontent']=array(
       '#type'=>'textarea',
       '#title'=>t("Second Hover Description"),
       '#maxlength' => 80,
        '#prefix' => '<div><div class="span12">',
        '#suffix' => '</div></div>',
       '#description' => t('Hover Description cannot be longer than 80 characters'), 
       '#default_value'=> isset($gethomestaticdata['boxsecond']['secondcontent'])?$gethomestaticdata['boxsecond']['secondcontent']:''
        
      );
       
       //logo second box
       
        $form['boxsecond']['secondcontentlogo']=array(
        '#type' => 'managed_file',
         '#prefix' => '<div class = "row-fluid">',
         '#suffix' => '</div>',
         '#description' => '<span class="icon-hand-down"><p style="font-size:12px">Uploaded Inner logo Image Size should be 165 x 61 pixels. </p></span>',
         '#upload_location' => 'public://home-page-sliders/',
         '#default_value'=> isset($gethomestaticdata['boxsecond']['secondcontentlogo'])?$gethomestaticdata['boxsecond']['secondcontentlogo']:''
        
      );
       //end of second box content
       
       //third box content start here
         $form['boxthird'] = array(
        '#type' => 'fieldset',
        '#title' => t('Third Box Content'),
        '#collapsible' => TRUE,
        '#collapsed' => TRUE,
        '#tree' => TRUE,
      );
       
       $form['boxthird']['thirdboxtitle']=array(
       '#type'=>'textfield',
       '#title'=>t("Third Box Title"),
        '#maxlength' => 15,
        '#prefix' => '<div class = "row-fluid"><div class="span4">',
       '#suffix' => '</div>',
        '#description' => t('Tittle cannot be longer than 15 characters'),  
        '#default_value'=> isset($gethomestaticdata['boxthird']['thirdboxtitle'])?$gethomestaticdata['boxthird']['thirdboxtitle']:''
      );
        
      $form['boxthird']['thirdboxspecial']=array(
       '#type'=>'textfield',
       '#title'=>t("Third Box Special"),
       '#maxlength' => 15, 
        '#prefix' => '<div class="span4">',
        '#suffix' => '</div>',
        '#description' => t('Specialtext cannot be longer than 15 characters'),  
       '#default_value'=> isset($gethomestaticdata['boxthird']['thirdboxspecial'])?$gethomestaticdata['boxthird']['thirdboxspecial']:''
      );
         
       $form['boxthird']['thirdboxlink']=array(
       '#type'=>'textfield',
       '#title'=>t("Thirdboxlink"),
       '#prefix' => '<div><div class="span4">',
       '#suffix' => '</div></div>',
       '#description'=> t('Please enter a valid URL. Protocol is required (http://, https://) Example : http://wwc.lsnsoft.com'),
        '#default_value'=> isset($gethomestaticdata['boxthird']['thirdboxlink'])?$gethomestaticdata['boxthird']['thirdboxlink']:''
    
      );
       
       $form['boxthird']['thirdcontent']=array(
       '#type'=>'textarea',
       '#title'=>t("Third Hover Description"),
       '#maxlength' => 80,
        '#prefix' => '<div><div class="span12">',
        '#suffix' => '</div></div>',
       '#description' => t('Hover Description cannot be longer than 80 characters'), 
       '#default_value'=> isset($gethomestaticdata['boxthird']['thirdcontent'])?$gethomestaticdata['boxthird']['thirdcontent']:''
        
      );
       
       //logo of third box
       
       $form['boxthird']['thirdcontentlogo']=array(
        '#type' => 'managed_file',
         '#prefix' => '<div class = "row-fluid">',
        '#suffix' => '</div>',
         '#description' => '<span class="icon-hand-down"><p style="font-size:12px">Uploaded Inner logo Image Size should be 165 x 61 pixels. </p></span>',
         '#upload_location' => 'public://home-page-sliders/',
         '#default_value'=> isset($gethomestaticdata['boxthird']['thirdcontentlogo'])?$gethomestaticdata['boxthird']['thirdcontentlogo']:''
        
      );
       //end of third box content
       
      $form['submit']=array(
			'#type'=>'submit',
			'#value'=>'Save',
	);
   
      //help model pop starts here
      $homecontenthelpimage = $GLOBALS['base_url'] . "/sites/all/modules/custom/homepage/homepagesliders/img/homecontenthelp.jpeg";
     $form['sliderhomemodal']=array(
			'#type'=>'markup',
			'#markup'=>'
				<div id="Homecontent" class="modal hide fade" tabindex="-1" role="dialog" aria-labelledby="priceTableModalLabel" aria-hidden="true">
				<div class="modal-header">
				<button type="button" class="close" data-dismiss="modal" aria-hidden="true">x</button>
				<h3>Front Page Content</h3>
				</div>
				<div class="modal-body">
				<img src="' . $homecontenthelpimage . '" alt="" />
				</div>
				<div class="modal-footer">
				<button class="btn" data-dismiss="modal" aria-hidden="true">Close</button>
				</div>
				</div>'
	);
     
      
      //end of help modal pop up box
	
        return $form;
}


function home_page_content_list_form_submit($from, &$form_state){
    $homecontentdata=  serialize($form_state['values']);
    
    
    $homecontentobject=new homepagecontent();
    $homecontentobject->sethomepagecontent($homecontentdata);
     $gethomestaticdata=unserialize(homepagecontent::gethomepagecontent());
//     var_dump($gethomestaticdata['connectcontent']['value']);
//    exit();
   
}