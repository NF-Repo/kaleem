<?php
function cruisefee_update_form($form,&$form_state,$cruise)
{
	
	drupal_set_title ($cruise->cruise_title );

	//Form Starts here
	//Form ends here
	/*feeid: "8",
	type: "flatfee",
	value: "10.00000",
	title: "Port Charge1"
	*
	*/
	
	$cruiseid=$cruise->id;

	$form ['cruisefee'] ['feeitemstart'] = array (
			'#type' => 'markup',
			'#markup' => '<table class="table table-bordered"><tr><th>Assign</th> <th>Fee Title</th> <th>Fee Type</th>  <th>Default Rate</th> <th>Applied Rate</th></tr>'
	);




	$form ['cruisefee'] ['cruisetempalteid'] = array (
			'#type' => 'hidden',
			'#default_value' => $cruiseid,
	);

	$feeItems=CruiseFees::getAllFeeItemsByTemplateId($cruiseid);

	$noOfFeeItems=count($feeItems);

	$form ['cruisefee'] ['nooffeeitems'] = array (
			'#type' => 'hidden',
			'#default_value' => $noOfFeeItems,
	);





	for($i=0;$i<count($feeItems);$i++){

		
		$activestatus=CruiseFees::getCruiesFeeStatus($cruiseid, $feeItems [$i]->feeid);
		
		$defaultrate=sprintf("%.2f",$feeItems [$i]->value);
		//$updated_price= isset($updfee) ? $updfee : '';
		$form ['cruisefee'] ['feestatus' . $i] = array (
				'#type' => 'checkbox',
				//'#markup' => $feeItems [$i]->title,
				'#prefix'=>'<tr><td>',
				'#suffix'=>'</td>',
				'#default_value' => $activestatus,
		);
		

		$form ['cruisefee'] ['feeitem' . $i] = array (
				'#type' => 'markup',
				'#markup' => $feeItems [$i]->title,
				'#prefix'=>'<td>',
				'#suffix'=>'</td>'
		);

		$updfee=CruiseFees::getCruiesFeePrice($cruiseid, $feeItems [$i]->feeid);
		//if($updfee!=""){
			$updated_price= isset($updfee) ? $updfee : $defaultrate;
		//}else{
			//$updated_price= $defaultrate;
		//}
		



		$form ['cruisefee'] ['feeid' . $i] = array (
				'#type' => 'hidden',
				'#default_value' => $feeItems [$i]->feeid,
				// 				'#prefix'=>'<td>',
		// 				'#suffix'=>'</td>'
		);

		$form ['cruisefee'] ['feeitemtype' . $i] = array (
				'#type' => 'markup',
				'#markup' => $feeItems [$i]->type,
				'#prefix'=>'<td>',
				'#suffix'=>'</td>'
		);
		
		$form ['cruisefee'] ['feedefaultrate' . $i] = array (
				'#type' => 'hidden',
				'#default_value' => $defaultrate,
				// 				'#prefix'=>'<td>',
				// 				'#suffix'=>'</td>'
		);

		$form ['cruisefee'] ['feeitemprice' . $i] = array (
				'#type' => 'markup',
				'#markup' => $defaultrate,
				'#prefix'=>'<td>',
				'#suffix'=>'</td>'
		);


	//
	
		$form ['cruisefee'] ['feeitemupdateprice' . $i] = array (
				'#type' => 'textfield',
				'#default_value' => $updated_price,
				'#prefix'=>'<td>',
				'#suffix'=>'</td></tr>'
		);
	}

	$form ['cruisefee'] ['feeitemsend'] = array (
			'#type' => 'markup',
			'#markup' => '</table>'
	);

	if($noOfFeeItems>0){
	$form ['cruisefee']['submit'] = array (
			'#type' => 'submit',
			'#value' => t ( 'Save' ),
	);
	}




	return $form;


}


function cruisefee_update_form_submit($form,&$form_state){

//drupal_json_output($form_state['values']); exit();
	
	$noOfFeeItems=(int)$form_state['values']['nooffeeitems'];
	$cruise_template_id=$form_state['values']['cruisetempalteid'];

	//var_dump($cruise_template_id); exit();
	//Clear all the existing records
	CruiseFees::deleteFeeItemsByCruiseTID($cruise_template_id);

	for($i=0;$i<$noOfFeeItems;$i++){
		//cruisetempalteid
		$fee_id=$form_state['values']['feeid'.$i];
		$cruisefee_price=$form_state['values']['feeitemupdateprice'.$i];
		$active=$form_state['values']['feestatus'.$i];
		
		if($cruisefee_price==""){
			$cruisefeePrice=$form_state['values']['feedefaultrate'.$i];
		}else{
			$cruisefeePrice=$cruisefee_price;
		}
		
		
		
		
		
		
		$cruiseFee=new CruiseFees();
		$cruiseFee->setCruiseTemplateId($cruise_template_id);
		$cruiseFee->setFeeId($fee_id);
		$cruiseFee->setPrice($cruisefeePrice);
		$cruiseFee->setStatus($active);
		
		
		//drupal_json_output($cruiseFee); exit();
		
		
		$cruiseFee->createCruiseFee();
	
	}






}