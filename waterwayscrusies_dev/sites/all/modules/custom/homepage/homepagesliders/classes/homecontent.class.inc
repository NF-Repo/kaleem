<?php

class homepagecontent{
    public $homecontent='';
    public function sethomepagecontent($homecontent){
		return variable_set("HOMPAGESTATICDATA", $homecontent);
	}
	
     public static function gethomepagecontent(){
		return variable_get("HOMPAGESTATICDATA");
	}
	
}