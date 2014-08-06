<?php

/* 
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */



class homepagesliders{
    
    public static $tableName="homepage_sliders";
    public $sliderlayout;
    public $imagepath;
    public $data;
    
   	public function insert_homepage_slider(){
		$id = db_insert(self::$tableName)
		->fields(array(
				'sliderlayout'=>$this->sliderlayout,
                                'image'=>$this->imagepath,
				'data' => $this->data,
		))
		->execute();
		
	}
        public static function  gethomepagesliderdata(){
             $result = db_select(self::$tableName, 'g')
		->fields('g',array('image','sliderlayout','data'))
                 ->orderBy("position", "ASC")
                 ->condition('status', 1,'=')
		->execute()->fetchAll();
		return $result;
           }
        
        public function  gethomePageSliderEdit($sliderid){
             $result = db_select(self::$tableName, 'g')
		->fields('g',array('image','sliderlayout','data'))
                 ->condition('Id', $sliderid,'=')
		 ->execute()->fetchAssoc();
		return $result;
           }
        
           public function getsliderupdata($sliderid,$sliderlayout,$sliderimage,$sliderdata){
               
               $sliderupdated = db_update(self::$tableName) 
               ->fields(array(
                     'sliderlayout' => $sliderlayout,
                     'image' => $sliderimage,
                     'data'=>$sliderdata,
                 ))
                ->condition('Id', $sliderid,'=')
                  ->execute();
               return $sliderupdated;
               
           }
         
           
           public function hideandShowSliders($id,$hideshow){
                  $updatestatus = db_update(self::$tableName) 
                  ->fields(array(
                     'status' => $hideshow,
                 ))
                ->condition('Id',$id,'=')
                  ->execute();
               return $updatestatus;
           }
           
           
}