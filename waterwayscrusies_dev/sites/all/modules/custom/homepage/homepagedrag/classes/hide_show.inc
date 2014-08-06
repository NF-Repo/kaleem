<?php

/* 
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

class hidandshow{
    
    
    public function hidandshowdata($sliderid,$sliderlayout,$sliderimage,$sliderdata){
               
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
	
}