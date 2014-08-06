<?php

class homepagesliderdelete{
    
    
   public  static function removetheImageComplete($sliderid){
        $result = db_select('homepage_sliders', 'g')
		->fields('g',array('image'))
		->condition('Id', $sliderid)
		->execute()->fetchAssoc();
		return $result;
    }
    
    
    
    public static function deleteSliderFontpage($sliderid){
        $gettheimagedirecto=homepagesliderdelete::removetheImageComplete($sliderid);
        drupal_unlink($gettheimagedirecto['image']);
          db_delete('homepage_sliders')
              ->condition('Id', $sliderid)
              ->execute();
        
 }
}
