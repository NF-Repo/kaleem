<?php

function home_page_slider_delete($sliderid){
      
    if(!empty($sliderid)){
         $deleteslider=new homepagesliderdelete();
         $deleteslider->deleteSliderFontpage($sliderid);
         $path="homepagedrag";
         drupal_goto($path);
    }
}
