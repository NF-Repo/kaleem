<?php

class Homepagenav{
    
      public static function getHomesliderMenu() {
         global $base_url;
               $menuhtml = '<div class="home_nav"><ul class="nav nav-pills">
                                    <li class=""><a class="btn btn-primary" href="'.$base_url.'/homepagedrag"><i class=""></i>Sliders</a></li>
                                    <li class=""><a class="btn" href="'.$base_url.'/homepage-sliders/content">&nbsp;Content</a></li>
    			</ul>
                        <a style="margin-bottom: 25px;" href="'.$base_url.'/homepage-sliders" class=""><span class="icon-plus-sign"></span> Add Slider</a> </div>';
         
        return $menuhtml;
    }
   
      public static function getHomeStaticMenu() {
         global $base_url;
               $menuhtml = '<ul class="nav nav-pills">
	    			<li class=""><a  class="btn" href="'.$base_url.'/homepagedrag"><i class=""></i>Sliders</a></li>
	    			<li class=""><a class="btn btn-primary" href="'.$base_url.'/homepage-sliders/content">&nbsp;Content</a></li>
    			</ul>';
         
        return $menuhtml;
    }
    
    public static function getHomeSlider() {
         global $base_url;
               $menuhtml = '<a href="'.$base_url.'/homepagedrag" class="btn" ><i class=""></i>Back</a>';
        return $menuhtml;
    }
}