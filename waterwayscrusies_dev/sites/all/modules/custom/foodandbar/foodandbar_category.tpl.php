<?php
/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
//print base_path();
//print drupal_get_path('module', 'cruisesdetailview');
//var_dump($nodeid);

?>




<script  src="<?php echo base_path() . path_to_theme() ?>/js/jquery.bxslider.js"></script>
<link rel="stylesheet" href="<?php echo base_path() . path_to_theme() ?>/css/jquery.bxslider.css" type="text/css" />
<script>

    var dirbasepath = Drupal.settings.basePath;
    
    function foodmenu()
    {
        
  var height = $(window).height();
  var width = $(window).width();

  if(width>height) {
    // Landscape - PUT THE CODE YOU WANT TO RUN HERE
   if($(".bx-has-controls-direction").hasClass("bx-controls")){
       
   }
  }
  else {
    // Portrait - PUT THE CODE YOU WANT TO RUN HERE
     if(!$(".bx-has-controls-direction").hasClass("bx-controls")){
    $('#foodSelectionMenu').bxSlider({
                infiniteLoop: false,
                hideControlOnEnd: true,
                minSlides: 1,
                maxSlides: 3,
                slideWidth: 300,
                slideHeight: 300,
                slideMargin: -30,
                touchEnabled: false,
                pager: false,
                responsive: true,
            });
  }
  }
    }
    $(function() {
        // To do active Tab
        $(".responsiveWeb li").removeClass("active");
        $('.foodandbar').addClass("active");
        $("#rightsliderImages div:first-child").addClass("active");
        
         var isiPad = navigator.userAgent.match(/iPad/i) != null;
        var ua = navigator.userAgent;
        var isiPad = /iPad/i.test(ua) || /iPhone OS 3_1_2/i.test(ua) || /iPhone OS 3_2_2/i.test(ua);
        window.addEventListener("resize", function() {
        
        });
        
        
        foodmenu();
        //laxmi
        $(window).resize( function(){
            foodmenu();
});
        
        
        
        //laxmi ends
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
//        if(isiPad)
//        {
//             if((window.orientation == 0)||(window.orientation==180)) 
//             {// Portrait
//  $('#foodSelectionMenu').bxSlider({
//                infiniteLoop: false,
//                hideControlOnEnd: true,
//                minSlides: 1,
//                maxSlides: 3,
//                slideWidth: 300,
//                slideHeight: 300,
//                slideMargin: -30,
//                touchEnabled: false,
//                pager: false,
//                responsive: true,
//            });
//            
//        }}
            
        var windowsize=$(window).width();
        var landingSlider;
//        if(windowsize<=783 && !isiPad){
//            
//            landingSlider=$('#foodSelectionMenu').bxSlider({
//                infiniteLoop: false,
//                hideControlOnEnd: true,
//                minSlides: 1,
//                maxSlides: 3,
//                slideWidth: 300,
//                slideHeight: 300,
//                slideMargin: -30,
//                touchEnabled: false,
//                pager: false
//            });
//
//        }

        /* Arrow scroll top script */
        $("#upArrow").click(function() {
            $("html, body").animate({scrollTop: $("#header").offset().top}, 800);
        });


//        window.addEventListener("resize", function() {
//            var windowsize = $(window).width();
//            if (windowsize <= 768) {
//            if(!$(".bx-has-controls-direction").hasClass("bx-controls")){
//            	landingSlider=$('#foodSelectionMenu').bxSlider({
//                    infiniteLoop: false,
//                    hideControlOnEnd: true,
//                    minSlides: 1,
//                    maxSlides: 3,
//                    slideWidth: 300,
//                    slideHeight: 300,
//                    slideMargin: -30,
//                    touchEnabled: false,
//                    pager: false
//                });
//                }
//            if(landingSlider){
//		landingSlider.reloadSlider();
//			}
//            } else {
//				$("#foodSelectionMenu").removeAttr("style");
//                var li_count = $('#foodSelectionMenu li').length;
//
//                for (var n = 0; n < li_count; n++) {
//                    $('#foodSelectionMenu li').eq((3 * n) + 3).css('clear', 'both', 'position', 'relative', 'width', '300px', 'min-height', '100px');
//                    $('.bx-wrapper .bx-viewport').css('height', '100% !important');
//
//                }
//            }
//
//            setTimeout(function(){
//                $('#foodSelectionMenu').css('-webkit-transform', 'translate3d(0px, 0px, 0px)');
//                },50);
//        }, false);
//
//        var isiPad = navigator.userAgent.match(/iPad/i) != null;
//        var ua = navigator.userAgent;
//        var isiPad = /iPad/i.test(ua) || /iPhone OS 3_1_2/i.test(ua) || /iPhone OS 3_2_2/i.test(ua);

// window.addEventListener("resize", function() {
//          var windowsize=$(window).width();
//       if(windowsize<=768 && !isiPad){
////            alert("hi");
//        $('#foodSelectionMenu').bxSlider({
//          infiniteLoop: false,
//           hideControlOnEnd: true,
//           minSlides: 1,
//           maxSlides: 3,
//           slideWidth: 300,
//           slideHeight: 300,
//           slideMargin: -30,
//           touchEnabled: false,
//           pager: false
//       });
//
//        }  else{
//           
//           var li_count=$('#foodSelectionMenu li').length;
//         
//         for ( var n = 0; n < li_count; n++ ) {
//           $('#foodSelectionMenu li').eq((3*n)+3).css('clear', 'both','position','relative','width', '300px','min-height','100px');
//           $('.bx-wrapper .bx-viewport').css('height','100% !important');
//            
//        }
//    }
// }, false);
        
         var isiPad = navigator.userAgent.match(/iPad/i) != null;
         var ua = navigator.userAgent;
         var isiPad = /iPad/i.test(ua) || /iPhone OS 3_1_2/i.test(ua) || /iPhone OS 3_2_2/i.test(ua);
         
         
        
//        window.addEventListener("orientationchange", function() {
//  // Announce the new orientation number
////  alert(window.orientation);
//  if((isiPad)) {
//   if((window.orientation == 0)||(window.orientation==180)) // Portrait
//       {
////          alert("portrait");
//$('.bx-next').css('display','none');
//          $('#foodSelectionMenu').bxSlider({
//          infiniteLoop: false,
//          autoControls: false,
//           hideControlOnEnd: true,
//           minSlides: 1,
//           maxSlides: 3,
//           slideWidth: 300,
//           slideHeight: 300,
//           slideMargin: -30,
//           touchEnabled: false,
//           pager: false
//       });
//       }
//      
//       else if((window.orientation==90)||(window.orientation==-90))
//      {
////            alert("noportrait");
//
//      var li_count=$('#foodSelectionMenu li').length;
//       for ( var n = 0; n < li_count; n++ ) {
//     $('#foodSelectionMenu li').eq((3*n)+3).css('clear', 'both','position','relative','width', '300px','min-height','100px');
//     $('.bx-viewport').css('height','100%');
//          
//      }
//          }
//      }
//}, false);


    });
    
</script>



<div id="content">
    <div class="contentDetail">
        <!-- sunset dining start here -->



        <div class="row-fluid cruiseDetailinfo foodandbardetailview">
            <div class="span6">
                <div class="detailBackLink responsiveWeb"><a href="<?php echo base_path() ?>foodandbar"> <img src="<?php echo base_path() . path_to_theme(); ?>/images/back_arrow.png" /> BACK TO ALL MENUS</a></div>

                <div class="row-fluid">
                    <div class="span2">

                    </div>
                    <div class="span10 foodandbar-details">
                        <?php 
                                    drupal_set_title($loadfoodandbartitle, $output=CHECK_PLAIN);
                                ?>
                        <div class="menuTitle"> <?php print $loadfoodandbartitle; ?></div>
                        <div class="menuDesc"><?php print $loadfoodandbardesc; ?></div>


                        <div class="row-fluid request-quote" align="center">

                            <div class="span3 sunsetLinePadding left-line-margin">
                                <div class="line"></div>
                            </div>
                            <div class="span6">
                                <!-- booknow start here -->
                                <div class="booknow ">
                                    <a href="#myModalrequest" role="button" data-toggle="modal">
                                        <div class="booknow-inner">Request a Proposal</div>
                                    </a>
                                </div>
                                <!-- booknow end here -->
                            </div>
                            <div class="span3 sunsetLinePadding right-line-margin">
                                <div class="line"></div>
                            </div>
                        </div>


                    </div>
                </div><!-- row-fluid end -->

            </div>
            <div class="span6">

                <div class="fleet-slider-buttons">
                    <!-- removed downarrow from here -->
                    <div id="myCarousel" class="carousel slide">
                        <div class="carousel-inner" id="rightsliderImages">
                            <?php print $loadslider; ?>
                        </div>
                    </div>
                </div>

            </div>
        </div>

    </div>


    <div class="foodSelectionMenu">
            
            <div class="title">
                Menu Selections
                <div class="pull-right responsiveWeb">
                   <img src="<?php echo base_path() ?>sites/default/files/cruisemenu/download-icon.png"><a href="<?php print $loadmenupdfurl; ?>" target="_blank"> DOWNLOAD MENU PDF</a>
                </div>
            </div>
            
            
            <div class="foodSelection">
            <ul id="foodSelectionMenu">
                <?php print $buffetdetails; ?>
            </ul>
            </div>

       
 <div id="upArrow" class="privatecruiseUpArrow">
        <img src="<?php print base_path() . drupal_get_path('module', 'privateevents_cruise') . '/images/sunset-banner-uparrow.png'; ?>" />
    </div>
    </div><!-- row-fluid end -->

  
   
    
    
    
</div>
