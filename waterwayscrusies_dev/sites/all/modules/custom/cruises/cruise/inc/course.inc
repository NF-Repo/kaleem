<?php
// function delete_course(){
// 	$courseid = $_GET['courseid'];
	
// 	$msg='';
	
// 	try{
// 		$rec=db_delete('cruise_course_list')
// 		->condition('id',$courseid,'=')
// 		->execute();
		
// 		if($rec>0){
// 			$msg="success";
// 		}else{
// 			$msg="failed";
// 		}
// 	}catch(Exception $e){
		
// 	}
	
	
// 	$data=db_select('cruise_course_list','c')
// 	->fields('c')
// 	->condition('cruise_id',$_SESSION['temp_id'],'=')
// 	->execute()
// 	->fetchAll();

// 	$retdata='<table class="table table-striped">';
// 	$retdata.='<tr><th>Course Name</th><th>Column Name</th><th>Remove</th><th>Edit</th></tr>';

// 	global $base_url;
// 	$arr=array();
// 	$p='';
// 	$_SESSION['t']=array();
// 	for($i=0;$i<count($data);$i++){
		
// 		$arr[$data[$i]->id]=$data[$i]->course_name;

// 		$_SESSION['t']=$arr;

// 		//$p.='<option value="'.$data[$i]->id.'">'.$data[$i]->course_name.'</option>';
// 		$arr[$data[$i]->id]=$data[$i]->course_name;
// 		$retdata.="<tr>";
// 		$retdata.="<td>".$data[$i]->course_name."</td>";
// 		$retdata.="<td>".$data[$i]->column_name."</td>";
// 		//$retdata.='<td>'.'<a href="' . $base_url . '//' . $data[$i]->id . '">Remove </a>'.'</td>';
// 		//$retdata.='<td><a href="#" onclick="userapproval(' . $data->id.')">Remove</a></td>';
// 		//$retdata.='<td> <a href="javascript:void();" onclick="return test(' . $data[$i]->id.');">Remove</a></td>';
		
// 		$retdata.='<td><a href="javascript:deletecourse('.$data[$i]->id.');">Remove</a></td>';
// 		$retdata.="<td>Edit</td>";
// 		$retdata.="</tr>";
// 	}
// 	$retdata.='</table>';
	
	
// 	drupal_json_output($retdata);
// }




// function get_course_details(){
// 	$courseid = $_GET['courseid'];
// 	$data=db_select('cruise_course_list','c')
// 	->condition('id',$courseid,'=')
// 	->fields('c')
// 	->execute()->fetchObject();
	
// 	$arr=array();
// 	$arr['course_name']=$data->course_name;
// 	$arr['column_name']=$data->column_name;
	
	
	
// 	 drupal_json_output($arr);
// 	 exit();
// 	//return $data;
// }