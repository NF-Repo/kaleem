<?php
function pricetable_page($packageId){
	return drupal_get_form ( "pricetable_form",$packageId);
}




//<form Starts here>
function pricetable_form($form, &$form_state,$packageId) {
		drupal_add_css(drupal_get_path('module', 'privateeventmanagement') . '/css/privateeventmanagement.css');
	
		//var_dump($packageId); exit();

	$existingrecord=PrivateEventPackagesPriceTable::getPrivateEventPriceTableDataByPackageId($packageId);
	
	//var_dump($existingrecord); exit();
	$existingNumberOfRows='';
	$layoutStyle ='fixed_layout';
	$existingEditorContent='';
	$priceTableId='';
	$priceTableDesciption='';
	//$existingPriceTableArray=array();
	if($existingrecord){
		$dbRecord=$existingrecord;
		$priceTableId=$dbRecord->priceTableId;
		$layoutStyle=$dbRecord->layoutStyle;
		$priceTableDesciption=$dbRecord->priceTableDesciption;
		$existingPriceTableArray=unserialize($dbRecord->fixedLayoutData);
		$existingNumberOfRows=count($existingPriceTableArray);
		$existingEditorContent=$dbRecord->editorData;
	}
	
//	exit();
	//var_dump($layoutStyle); exit();
	//$form = array ();

	$menuHeaderHtml=PrivateEventPackagesHeaderMenu::getPackageMenuList($packageId,'pricetable');
	$form['headerstart']=array(
		'#type'=>'markup',
		'#markup'=>$menuHeaderHtml,
	);


	//<Hidden Params>
	$form['packageId']=array(
			'#type'=>'hidden',
			'#value'=>$packageId
	);
	
	$form['priceTableId']=array(
			'#type'=>'hidden',
			'#value'=>$priceTableId,
	);

	//</Hidden Params>
	


	//<Modal Popup>
	$logourl = $GLOBALS['base_url'] . "/sites/all/modules/custom/privateevent/privateeventmanagement/img/pricetableview.png";
	$form['pricetablemodal']=array(
			'#type'=>'markup',
			'#markup'=>'
				<div id="priceTableModal" class="modal hide fade" tabindex="-1" role="dialog" aria-labelledby="priceTableModalLabel" aria-hidden="true">
				<div class="modal-header">
				<button type="button" class="close" data-dismiss="modal" aria-hidden="true">x</button>
				<h3 id="priceTableModalLabel">Price Table</h3>
				</div>
				<div class="modal-body">
				<img src="' . $logourl . '" alt="" />
				</div>
				<div class="modal-footer">
				<button class="btn" data-dismiss="modal" aria-hidden="true">Close</button>
				</div>
				</div>'
	);

	$form['pricetabledivstart']=array(
			'#type'=>'markup',
			'#markup'=>'<div class="overviewformbackground">',
	);

	$form['pagetitle']=array(
		'#type'=>'markup',
		'#markup'=>'<h2>Price Table&nbsp;&nbsp;<a href="#priceTableModal" role="button" class="" data-toggle="modal"><span class="icon-question-sign"></span></a></h2>'
	);
	
	$form['layoutStyle']=array(
		'#type'=>'select',
		'#options'=>array('editor'=>'Editor','fixed_layout'=>'Fixed Layout'),
		'#prefix'=>'<table><tr><td style="width:15%;"><b>Choose Layout :<b></td><td style="width:85%;">',
		'#suffix'=>'</td></table>',
		'#default_value'=>$layoutStyle,
	);
	
	
	
	


	$num_price_table_count = !empty($form_state['values']['num_pricetable_rows']) ? $form_state['values']['num_pricetable_rows'] : 1;
	
	if(empty($form_state['values']['num_pricetable_rows'])){
		$num_price_table_count=$existingNumberOfRows;
	}
// 	if(isset($existingNumberOfRows) && !empty($existingNumberOfRows)){
// 		$num_price_table_count=$existingNumberOfRows;
// 	}
	
	
	$pricetablerange=range(0,20);
	$form['num_pricetable_rows'] = array(
			'#title' => t('How many Rows do you want in Price Table?'),
			'#type' => 'select',
			'#options' => drupal_map_assoc($pricetablerange),
			//'#default_value' => $num_price_table_count,
			'#default_value' => $num_price_table_count,
			
			'#ajax' => array(
					'callback' => 'ajax_price_table_callback',
					'wrapper' => 'pricetable-div',
					'effect' => 'slide',
					'progress' => array('type' => 'none'),
						
			),
			'#states' => array(
					'visible' => array(
							':input[name="layoutStyle"]' => array('value' => 'fixed_layout'),
					),
			),
	);






	
	$form['editorData']=array(
			'#type'=>'text_format',
			'#prefix'=>'</td></tr><tr><td><b>Content :</b></td><td>',
			'#suffix'=>'</td></tr>',
			//'#default_value'=>isset($buffetObject->buffet_content) ? $buffetObject->buffet_content : '',
			'#states' => array(
					'visible' => array(
							':input[name="layoutStyle"]' => array('value' => 'editor'),
					),
			),
			'#default_value'=>isset($existingEditorContent) ? $existingEditorContent : '',
	);
	
	
	
	



	$form['pricetable_fieldset'] = array(
			'#title' => t(""),
			'#prefix' => '<div id="pricetable-div">',
			'#suffix' => '</div>',
			'#type' => 'fieldset',
			'#description' => t(''),
			'#attributes'=>array('class'=>array('pricetable')),
			'#states' => array(
					'visible' => array(
							':input[name="layoutStyle"]' => array('value' => 'fixed_layout'),
			  ),
			),
	);
	
	



	


	$form['pricetable_fieldset']['pricetable_fieldset_start'] = array(
			'#type' => 'markup',
			'#markup' =>'<table class="table table-bordered">
			<tr>
			<th>Time</th>
			<th>Boarding</th>
			<th>40-60 Guests</th>
			<th>61-90 Guests</th>
			<th>91-130 Guests</th>
			</tr>
			',
	);

	for ($i = 1; $i <= $num_price_table_count; $i++) {

		$form['pricetable_fieldset']["time$i"] = array(
				'#type' => 'textfield',
				//'#title' => "Checkbox $i",
				'#prefix' => '<tr><td>',
				'#suffix' => '</td>',
				'#default_value'=>isset($existingPriceTableArray[$i-1]->time) ? $existingPriceTableArray[$i-1]->time : '',
		);

		$form['pricetable_fieldset']["boarding$i"] = array(
				'#type' => 'textfield',
				//'#title' => "Checkbox $i",
				'#prefix' => '<td>',
				'#suffix' => '</td>',
				'#default_value'=>isset($existingPriceTableArray[$i-1]->boarding) ? $existingPriceTableArray[$i-1]->boarding : '',
		);

		$form['pricetable_fieldset']["guest4060$i"] = array(
				'#type' => 'textfield',
				//'#title' => "Checkbox $i",
				'#prefix' => '<td>',
				'#suffix' => '</td>',
				'#default_value'=>isset($existingPriceTableArray[$i-1]->guest4060) ? $existingPriceTableArray[$i-1]->guest4060 : '',
				'#attributes'=>array('class'=>array('input-small')),
		);


		$form['pricetable_fieldset']["guest6190$i"] = array(
				'#type' => 'textfield',
				//'#title' => "Checkbox $i",
				'#prefix' => '<td>',
				'#suffix' => '</td>',
				'#default_value'=>isset($existingPriceTableArray[$i-1]->guest6190) ? $existingPriceTableArray[$i-1]->guest6190 : '',
				'#attributes'=>array('class'=>array('input-small')),
		);

		$form['pricetable_fieldset']["guest91130$i"] = array(
				'#type' => 'textfield',
				//'#title' => "Checkbox $i",
				'#prefix' => '<td>',
				'#suffix' => '</td></tr>',
				'#default_value'=>isset($existingPriceTableArray[$i-1]->guest91130) ? $existingPriceTableArray[$i-1]->guest91130 : '',
				'#attributes'=>array('class'=>array('input-small')),
		);




	}
	
	$form['priceTableDesciption']=array(
			'#type'=>'text_format',
			'#prefix'=>'</td></tr><tr><td><b>Price Table Description :</b></td><td>',
			'#suffix'=>'</td></tr>',
			'#default_value'=>isset($priceTableDesciption) ? $priceTableDesciption : '',
	);

	$form['pricetable_fieldset']['pricetable_fieldset_end'] = array(
			'#type' => 'markup',
			'#markup' => '</table>',
	);




	$form['submit']=array(
		'#type'=>'submit',
		'#value'=>'Save'
	);
	
	
	$form['pricetabledivend']=array(
			'#type'=>'markup',
			'#markup'=>'</div>',
	);



	return $form;
}
//<form ends here>
function ajax_price_table_callback($form, $form_state) {
	return $form['pricetable_fieldset'];
}

function pricetable_form_submit($form, &$form_state) {
	
	//var_dump($form_state['values']); exit();
	
	

	$packageId=$form_state['values']['packageId'];
	
	
	if($form_state['values']['layoutStyle']=="fixed_layout"){
		$num_pricetable_rows=$form_state['values']['num_pricetable_rows'];
		$priceTable=array();
		if($num_pricetable_rows>0){
			for($i=1;$i<=$num_pricetable_rows;$i++){
				$priceTableObject=new stdClass();
				$priceTableObject->time=$form_state['values']['time'.$i];
				$priceTableObject->boarding=$form_state['values']['boarding'.$i];
				$priceTableObject->guest4060=$form_state['values']['guest4060'.$i];
				$priceTableObject->guest6190=$form_state['values']['guest6190'.$i];
				$priceTableObject->guest91130=$form_state['values']['guest91130'.$i];
				$priceTable[]=$priceTableObject;
			}
		}
		$privateEventPackagesPriceTable=new PrivateEventPackagesPriceTable();
		$privateEventPackagesPriceTable->packageId=$packageId;
		$privateEventPackagesPriceTable->layoutStyle=$form_state['values']['layoutStyle'];
		
		$privateEventPackagesPriceTable->priceTableDesciption=$form_state['values']['priceTableDesciption']['value'];
		
		
		$privateEventPackagesPriceTable->fixedLayoutData=serialize($priceTable);
		//$priceTableObject->createPrivateEventPriceTable();
		
	}
	
	if($form_state['values']['layoutStyle']=="editor"){
		$privateEventPackagesPriceTable=new PrivateEventPackagesPriceTable();
		$privateEventPackagesPriceTable->packageId=$packageId;
		$privateEventPackagesPriceTable->layoutStyle=$form_state['values']['layoutStyle'];
		$privateEventPackagesPriceTable->editorData=$form_state['values']['editorData']['value'];
		$privateEventPackagesPriceTable->priceTableDesciption=$form_state['values']['priceTableDesciption']['value'];
		
		
	}
	
	//var_dump($privateEventPackagesPriceTable); exit();
	//
	if(!empty($form_state['values']['priceTableId'])){
		
		if($form_state['values']['layoutStyle']=="fixed_layout"){
			$privateEventPackagesPriceTable->priceTableId=$form_state['values']['priceTableId'];
			$privateEventPackagesPriceTable->updatePrivateEventPriceTableFixedLayout();
		}
		
		if($form_state['values']['layoutStyle']=="editor"){
			$privateEventPackagesPriceTable->priceTableId=$form_state['values']['priceTableId'];
			$privateEventPackagesPriceTable->updatePrivateEventPriceTableEditor();
		}
		
		
		//update
	}else{
		//var_dump($privateEventPackagesPriceTable); exit();
		//echo "create"; exit();
		//create
		$privateEventPackagesPriceTable->createPrivateEventPriceTable();
	}
	
	

	
	
}