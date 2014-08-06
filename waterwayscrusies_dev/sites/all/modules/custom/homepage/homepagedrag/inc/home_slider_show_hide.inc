<?php

/* 
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

function homepage_status_page($id ,$hideshow){
    if(!empty($id)){
         $slidehide= new homepagesliders();
         $slidehide->hideandShowSliders($id,$hideshow);
          $path="homepagedrag";
          drupal_goto($path);
    }

}


