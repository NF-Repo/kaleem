<?php
function private_events_test_page(){
	//1Getting All Menus
	//$data=PrivateEventMenus::getPrivateEventMenuList();
	
	
	//2 Getting Menu Name By Id
	$data=PrivateEventMenus::getPrivateEventMenuNameByMenuId(1);
	
	
	var_dump($data); exit();
	return "hello world";
}