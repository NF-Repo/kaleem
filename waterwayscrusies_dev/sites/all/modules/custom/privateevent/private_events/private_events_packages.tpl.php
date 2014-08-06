
<script type="text/javascript" src="<?php echo base_path() . path_to_theme() ?>/js/buttons.js"></script>
<script  src="<?php echo base_path() . path_to_theme() ?>/js/jquery.bxslider.js"></script>
<link rel="stylesheet" href="<?php echo base_path() . path_to_theme() ?>/css/bxslider.pack.css" type="text/css" />

<script>


    var switchTo5x=true;
    stLight.options({
        publisher: 'dr-52e8796a-b9e2-6a17-d82d-251b74277418',
        doNotHash: false, 
        doNotCopy: false,
        hashAddressBar: false
    });           

    $(function() {

        $(".responsiveWeb li").removeClass("active");
        $('.privateevents').addClass("active");

        $("#rightsliderImages div:first-child").addClass("active");
        $("#routmapSlider div:first-child").addClass("active");
 

        /* Arrow scroll bottom script */
        $("#downArrow").click(function() {
            $("html, body").animate({scrollTop: $(".weddingDownslider").offset().top}, 800);
        });
        
        /* Arrow scroll top script */
        $("#upArrow").click(function() {
            $("html, body").animate({scrollTop: $("#header").offset().top}, 800);
        });



       var windowsize=$(window).width();
       var landingSlider;
        if(windowsize<=783){
        	landingSlider=$('.cateringSelectionMenu').bxSlider({
            infiniteLoop: false,
            hideControlOnEnd: true,
            minSlides: 1,
            maxSlides: 3,
            slideWidth: 300,
            slideHeight: 300,
            slideMargin: -30,
            touchEnabled: true,
            pager: false
        });
    }
    window.addEventListener("resize", function() {
        var windowsize = $(window).width();
      
        if (windowsize <= 768) {
        if(!$(".bx-has-controls-direction").hasClass("bx-controls")){
            landingSlider=$('.cateringSelectionMenu').bxSlider({
                infiniteLoop: false,
                hideControlOnEnd: true,
                minSlides: 1,
                maxSlides: 3,
                slideWidth: 300,
                slideHeight: 300,
                slideMargin: -30,
                touchEnabled: false,
                pager: false
            });
           
            }
			if(landingSlider){
				 landingSlider.reloadSlider();
			}
        } else {
            var li_count = $('.cateringSelectionMenu li').length;
           
            for (var n = 0; n < li_count; n++) {
                $('.cateringSelectionMenu li').eq((3 * n) + 3).css('clear', 'both', 'position', 'relative', 'width', '300px', 'min-height', '100px');
                $('.bx-wrapper .bx-viewport').css('height', '100% !important');

            }
        }

        setTimeout(function(){
            $('.cateringSelectionMenu').css('-webkit-transform', 'translate3d(0px, 0px, 0px)');
            },50);
    }, false);

    var isiPad = navigator.userAgent.match(/iPad/i) != null;
    var ua = navigator.userAgent;
    var isiPad = /iPad/i.test(ua) || /iPhone OS 3_1_2/i.test(ua) || /iPhone OS 3_2_2/i.test(ua);

    window.addEventListener("orientationchange", function() {
        // Announce the new orientation number
//alert(window.orientation);
        if ((isiPad)) {
            if ((window.orientation == 0) || (window.orientation == 180)) // Portrait
            {
//      alert("portrait");
if(!$(".bx-has-controls-direction").hasClass("bx-controls")){
	landingSlider=$('.cateringSelectionMenu').bxSlider({
                    infiniteLoop: false,
                    hideControlOnEnd: true,
                    minSlides: 1,
                    maxSlides: 3,
                    slideWidth: 300,
                    slideHeight: 300,
                    slideMargin: -30,
                    touchEnabled: false,
                    pager: false
                });
}
if(landingSlider){
	 landingSlider.reloadSlider();
}
            }

            else if ((window.orientation == 90) || (window.orientation == -90))
            {
            	$('.cateringSelectionMenu').css('-webkit-transform', 'translate3d(0px, 0px, 0px)');
//        alert("noportrait");
                var li_count = $('.cateringSelectionMenu li').length;
                for (var n = 0; n < li_count; n++) {
                    $('.cateringSelectionMenu li').eq((3 * n) + 3).css('clear', 'both', 'position', 'relative', 'width', '300px', 'min-height', '100px');
                    $('.bx-viewport').css('height', '100%');

                }
            }
        }
    }, false);

    

        $('.responsiveMobile select').change(function() {
            window.location.href=$(this).val();
        });

       
        //slider1 image indicator
        //slider one api
        $('#eventsmyCarousel').carousel({
            interval:5000
        }).on('slide', function (e) {
            var slideFrom = $('#eventsmyCarousel').find('.active').index();
            var slideTo = $(e.relatedTarget).index();
            var nextindicatorname='#eventind'+slideTo;
            $(".eventIndicators li div").removeClass("active");
            $(nextindicatorname).addClass("active");
        });
  
        $(document).on('click','.eventindicator',function(){
            var indicatorid=parseInt(this.id.slice(8));
    
            $('#eventsmyCarousel').carousel('pause');
            $('#eventsmyCarousel').carousel(indicatorid);
        });
    
    
        $("#eventcaroselnext").click(function(e){
            $('#eventsmyCarousel').carousel('next'); 
            $("#eventsmyCarousel").carousel("cycle");
            e.preventDefault(); 
        });
        $("#eventcaroselprev").click(function(e){
            $('#eventsmyCarousel').carousel('prev'); 
            $("#eventsmyCarousel").carousel("cycle");
            e.preventDefault(); 
        });
        //slider one api ends
        $("#rightsliderImages div:first-child").addClass("active");
        if($("#rightsliderImages").children().length>1){
            $("#rightsliderImages .item").each(function(index){
                if(index==0){
                    var indicators='<li><div class="eventindicator active" id="eventind'+index+'"></div> </li>';
               }else{
                    var indicators='<li><div class="eventindicator" id="eventind'+index+'"></div></li>';
                }
                $(".eventIndicators").append(indicators);
            });
        }
        //slider1 image indicators gets ends here

        //        slider two gets loads here
        $('#eventmydownCarousel').carousel({
            interval:5000
        }).on('slide', function (e) {
            var slideFrom = $('#eventmydownCarousel').find('.active').index();
            var slideTo = $(e.relatedTarget).index();
            var nextindicatorname='#eventdownind'+slideTo;
            $(".eventmydownIndicators li div").removeClass("active");
            $(nextindicatorname).addClass("active");
        });
  
        $(document).on('click','.eventmydownindicator',function(){
            var indicatorid=parseInt(this.id.slice(12));
    
            $('#eventmydownCarousel').carousel('pause');
            $('#eventmydownCarousel').carousel(indicatorid);
        });
    
    
        $("#eventdowncaroselnext").click(function(e){
            $('#eventmydownCarousel').carousel('next'); 
            $("#eventmydownCarousel").carousel("cycle");
            e.preventDefault(); 
        });
        $("#eventdowncaroselprev").click(function(e){
            $('#eventmydownCarousel').carousel('prev'); 
            $("#eventmydownCarousel").carousel("cycle");
            e.preventDefault(); 
        });
       
       
        $("#middlesliderscarousel div:first-child").addClass("active");
        if($("#middlesliderscarousel").children().length>1){
            $("#middlesliderscarousel .item").each(function(index){
                if(index==0){
                    
                    var indicators='<li><div class="eventmydownindicator active" id="eventdownind'+index+'"></div> </li>';
                }else{
                    var indicators='<li><div class="eventmydownindicator" id="eventdownind'+index+'"></div></li>';
                }
                $(".eventmydownIndicators").append(indicators);
            });
        }
        //        slider two ends loads here
        
    });





            function sharethisfn(shareval){
                if(shareval == 'facebook'){
                    $( "#fbsharebn").children('span').trigger( "click" );
                }
                if(shareval == 'twitter'){
                    $( "#twitsharebn").children('span').trigger( "click" );
                }
                if(shareval == 'share'){
                   $( "#sharethisbtn" ).trigger( "click" );
                }
            }

</script>
<?php
$actual_link = 'http://'.$_SERVER['HTTP_HOST'].request_uri();
?>

<?php global $base_url; ?>
<div id="content">
    <div class="contentDetail">
        <!-- sunset dining start here -->

        <div class="row-fluid cruiseDetailinfo privateeventsdetailview">
            <div class="span6">
                  <div class="detailBackLink"><a href="<?php echo base_path()?>private_events"><img src="<?php echo base_path() . path_to_theme(); ?>/images/back_arrow.png" /> BACK TO PRIVATE EVENTS</a></div>
                
                <div class="row-fluid">
                    <div class="span2"></div>
                    <div class="span10">
                        <!-- here rendering cruise details here -->
                        <div id="cruiseDetailsHolder">

                            <div id="cruiseDetailsHolder">
                                <div class="weddingPackagesTitle"><?php print $packageParentName; ?></div>

<!-- kaleem subtabmenu of packages-->
                                <div class="responsiveWeb subMenu">
                                    <?php print $deskTopMenu; ?>

                                </div>
                                <!--submenu in mobile view-->
                                <div class="responsiveMobile subMenu">

                                    <select>
                                        <?php print $mobileMenu ?>
                                    </select>

                                </div>
                                <?php 
                                    drupal_set_title($packageTitle, $output=CHECK_PLAIN);
                                ?>
                                
                        <!--kaleem packages title and description-->
                         <?php if(!empty($packageTitle)) { ?>
                                <div class="weddingPackagesSubTitle"><?php print $packageTitle; ?></div>
                         <?php } ?>
                                <?php if(!empty($packageDecription)) { ?>
                                <div class="weddingPackagesDescription"><?php print $packageDecription; ?></div>
                                <?php } ?>
                            </div>

                        </div>

                    <!--kaleem DOWNLOAD WEDDING PACKAGES-->
                    
                  <?php if(!empty($downloadpackagelink)) {?>
                        <div class="weddingdownloadlink">
                        
                           
                            
                            <div class="clickMapLink"> <img src="<?php print base_path() . drupal_get_path('module', 'private_events') . '/images/download-icon.png'; ?>" />  <div> <a href="<?php print $downloadpackagelink; ?>" target="_blank">DOWNLOAD WEDDING PACKAGES</a></div></div>

                        </div>

                  <?php }?>
                        <div class="row-fluid sunsetBooknowMobile">
                            <div class="span4 weddingLinePadding">
                                <div class="line"></div>
                            </div>
                            <div class="span4 requestmobile">
                                <!-- booknow start here -->
                                <div class="booknow ">
                                    
<!--   kaleem Request a proposal link below the download wedding packages -->
                                    <a  href="#myModalrequest" role="button" data-toggle="modal">
                                        <div class="booknow-inner">Request a Proposal</div>
                                    </a>
                                </div>
                                <!-- booknow end here -->
                            </div>
                            <div class="span4 weddingLinePadding" >
                                <div class="line"></div>
                            </div>
                        </div>

                        <!-- here rendering cruise description here -->
                        <div class="groupResNote" id="descriptionHolder"></div>

                    </div>
                </div><!-- row-fluid end -->

            </div>
            <div class="span6">
                <div class="sunset-slider-buttons">
                    <div id="downArrow" class="privatecruiseDownArrow">
                        <img src="<?php print base_path() . drupal_get_path('module', 'private_events') . '/images/sunset-banner-downarrow.png'; ?>" />
                    </div>

                    <!--slider one gets loads-->

                    <div id="eventsmyCarousel" class="carousel slide">
                        <!--                        <ol class="carousel-indicators">
                                                    <li data-target="#myCarousel" data-slide-to="0" class="active"></li>
                                                    <li data-target="#myCarousel" data-slide-to="1"></li>
                                                </ol>-->
                        <ul class="eventIndicators"></ul>  
                        <!-- Carousel FOR First Slider items -->
                        
<!--                        kaleem first slider of packages-->

                        <div class="carousel-inner" id="rightsliderImages">

                         
                            
                            <?php print $rightslider; ?>

                        </div>
                        <!-- Carousel nav -->


                        <a class="carousel-control left" href="javascript:void(0);" id="eventcaroselprev"><img src="<?php echo base_path() . path_to_theme(); ?>/images/carousol-leftarrow.png" /></a>
                        <a class="carousel-control right" href="javascript:void(0);" id="eventcaroselnext"><img src="<?php echo base_path() . path_to_theme(); ?>/images/carousol-rightarrow.png" /></a>
                    </div>

                    <!--end of slider 1-->

                    <div class="booknow share-mobile responsiveMobile">
                       


                                <div class="buttonsdiv">
                                    <!--<span class='pull-left facebook fbsharebtnimg' onclick="sharethisfn('facebook');" style="width:30%" displayText='Facebook'></span>-->
                                    
                                    <div class="pull-left twitter">
                                        <a href="https://twitter.com/share" class="twitter-share-button" data-count="none">Tweet</a>
                                    </div>
                                    
                                    <script>!function(d,s,id){var js,fjs=d.getElementsByTagName(s)[0],p=/^http:/.test(d.location)?'http':'https';if(!d.getElementById(id)){js=d.createElement(s);js.id=id;js.src=p+'://platform.twitter.com/widgets.js';fjs.parentNode.insertBefore(js,fjs);}}(document, 'script', 'twitter-wjs');</script>

                                    <div class='pull-left pinterest'>
                                        <a href="http://www.pinterest.com/pin/create/button/
                                           ?url=http%3A%2F%2Fwww.flickr.com%2Fphotos%2Fkentbrew%2F6851755809%2F
                                           &media=http%3A%2F%2Ffarm8.staticflickr.com%2F7027%2F6851755809_df5b2051c9_z.jpg
                                           &description=Next%20stop%3A%20Pinterest"
                                           data-pin-do="buttonPin"
                                           data-pin-config="above">
                                            <img src="//assets.pinterest.com/images/pidgets/pin_it_button.png" />
                                        </a>
                                    </div>
                                    <script type="text/javascript" src="//assets.pinterest.com/js/pinit.js"></script>
                                    <script type="text/javascript">
                                    (function(d){
                                        var f = d.getElementsByTagName('SCRIPT')[0], p = d.createElement('SCRIPT');
                                        p.type = 'text/javascript';
                                        p.async = true;
                                        p.src = '//assets.pinterest.com/js/pinit.js';
                                        f.parentNode.insertBefore(p, f);
                                    }(document));
                                    </script>
                                    <div id="fb-root"></div>
                                    <script>(function(d, s, id) {
                                      var js, fjs = d.getElementsByTagName(s)[0];
                                      if (d.getElementById(id)) return;
                                      js = d.createElement(s); js.id = id;
                                      js.src = "//connect.facebook.net/en_US/all.js#xfbml=1&appId=384985111641504";
                                      fjs.parentNode.insertBefore(js, fjs);
                                    }(document, 'script', 'facebook-jssdk'));
                                    </script>
                                    <div class="fb-like pull-left facebook" data-href="https://developers.facebook.com/docs/plugins/" data-layout="button" data-action="like" data-show-faces="true" data-share="false"></div>
                                    <!--<span class='pull-left twitter twitsharebtnimg' onclick="sharethisfn('twitter');" style="width:30%" displayText='Tweet'></span>-->
                                    <div class='sharetext pull-left' onclick='sharethisfn('share');'> Share</div>
                                    
                                     <span class='st_sharethis_large' displayText='ShareThis' st_title='<?php print $packageTitle; ?>' st_summary='<?php print $packageDecription; ?>' id='sharethisbtn' style='display:none;'></span>
                                </div>


                                

                        <!--</a>-->
                    </div>
                    <div class="booknow share responsiveWeb" id="shareBtn">
                        <span class="booknow-inner st_sharethis_custom" st_url="<?php $server_url = $GLOBALS['base_url'];
                
                        echo $socialMediasharePath; ?>" displayText="Share" st_title="<?php print $packageTitle; ?>" st_summary="<?php print $packageDecription; ?>">Share</span>
                        
                    </div>

                </div>


                <div class="booknow Reviews">
                    <a href="#">
                        <div class="booknow-inner">See Reviews!</div>
                    </a>
                </div>
            </div>


        </div>
    </div>

    <!-- sunset dining end here -->
</div>
<!-- sunset down slider start here -->
<div class="row-fluid">
    <div class="span12">
        <!-- sunsetDownslider start here -->
        <div class="weddingDownslider">

            <!--slide 2 gets starts here-->

            <div id="eventmydownCarousel" class="carousel slide">
                <ul class="eventmydownIndicators"></ul>  
                <!--                    <ol class="carousel-indicators">
                                        <li data-target="#mydownCarousel" data-slide-to="0" class="active"></li>
                                        <li data-target="#mydownCarousel" data-slide-to="1"></li>
                                    </ol>-->
                <!-- Midddle Carousel items -->
                
<!--                kaleem middle slider packages-->

                <div class="carousel-inner" id="middlesliderscarousel">
                    <?php print $middleslider; ?>
                </div>

                <a class="carousel-control left" href="javascript:void(0);" id="eventdowncaroselprev"><img src="<?php echo base_path() . path_to_theme(); ?>/images/carousol-leftarrow.png" /></a>
                <a class="carousel-control right" href="javascript:void(0);" id="eventdowncaroselnext"><img src="<?php echo base_path() . path_to_theme(); ?>/images/carousol-rightarrow.png" /></a>


                <!--                     Carousel nav 
                                    <a class="carousel-control left" href="#mydownCarousel" data-slide="prev"><img src="<?php echo base_path() . path_to_theme(); ?>/images/carousol-leftarrow.png" /></a>
                                    <a class="carousel-control right" href="#mydownCarousel" data-slide="next"><img src="<?php echo base_path() . path_to_theme(); ?>/images/carousol-rightarrow.png" /></a>
                -->
            </div>
            <!--slider two gets ends here-->
            
<!--            kaleem packages Inclusiong -->
            <div class="weddingDownsliderboxContainer">
            <div class="weddingDownsliderbox leftweddingbox">
                <div class="weddingDownsliderInner">
                    <div class="row-fluid package-heading">
                        <div class="span2 sunsetLinePadding"><div class="line"></div></div>
                        <div class="span8"><div class="sliderCaption">Package Inclusions</div></div>
                        <div class="span2 sunsetLinePadding"><div class="line"></div></div>
                    </div>

                    <div class="sliderInnercontent">
                        <?php if(!empty($packageinclusion)){
                        print $packageinclusion; }?> 
                    </div>

                    <div class="row-fluid" id="routeMaplinesholder">
                        <div class="span5 "><div class="line"></div></div>
                        <div class="span2" align="center"><img src="<?php print base_path() . drupal_get_path('module', 'private_events') . '/images/banner-icon-img.png'; ?>" /></div>
                        <div class="span5 "><div class="line"></div></div>
                    </div>
                </div><!-- sunsetDownsliderInner end -->
            </div>

                
<!--               kaleem package options -->
            <div class="weddingDownsliderbox rightslider">
                <div class="weddingDownsliderInner">
                    <div class="row-fluid package-heading">
                        <div class="span2 sunsetLinePadding"><div class="line"></div></div>
                        <div class="span8"><div class="sliderCaption">Package Options</div></div>
                        <div class="span2 sunsetLinePadding"><div class="line"></div></div>
                    </div>

                    <div class="sliderInnercontent">
                         
                       <?php print $packageoption; ?>
                    </div>

                    <div class="row-fluid" id="routeMaplinesholder">
                        <div class="span5 "><div class="line"></div></div>
                        <div class="span2" align="center"><img src="<?php print base_path() . drupal_get_path('module', 'private_events') . '/images/banner-icon-img.png'; ?>" /></div>
                        <div class="span5 "><div class="line"></div></div>
                    </div>
                </div><!-- sunsetDownsliderInner end -->
            </div>
        </div>
        </div>
        <!-- sunsetDownslider end here -->
    </div>
</div>
<!-- sunset down slider end here -->


<!--Pricing Table gets Loads here-->
<!-- price table kaleem -->
<div class="container">
    <?php if((!empty($deskTopPriceTable))||(!empty($mobilePriceTable))||(!empty($priceTableDescription))) {?>
    <div id="pricingTable">

        <div class="pricetitle"> Pricing<br>
            <span>*Per person cost</span>
        </div>

        <div class="visible-desktop visible-tablet hidden-phone">
            <?php //echo drupal_render($deskTopPriceTable); ?>
            <?php echo $deskTopPriceTable; ?>
        </div>

        <div class="visible-phone hidden-tablet hidden-desktop priceMobiletbl">
            <?php print $mobilePriceTable; ?>
        </div>

        <div class="pricetableNote">
               <?php print $priceTableDescription; ?>
          
        </div>
        
    
            <?php } ?>
        <!--Request a Proposal start here--> 
<!--kaleem Request a proposal link -->
        <div class="row-fluid responsiveWeb">
            <div class="span3 hidden-tablet">
            </div>
            <div class="span6 requesttablet">


                <div class="row-fluid sunsetBooknowMobile">
                    <div class="span4 weddingLinePadding">
                        <div class="line"></div>
                    </div>
                    <div class="span4 requestmobile">
                        <!-- booknow start here -->
                        <div class="booknow ">
                            <a  href="#myModalrequest" role="button" data-toggle="modal">
                                <div class="booknow-inner">Request a Proposal</div>
                            </a>
                        </div>
                        <!-- booknow end here -->
                    </div>
                    <div class="span4 weddingLinePadding" >
                        <div class="line"></div>
                    </div>
                </div>


            </div>
            <div class="span3 hidden-tablet">
            </div>
        </div>

        <!--Request a Proposal end here--> 



    </div> <!-- end pricingTable -->
</div>
<!--End of Pricing Table here-->




<!-- kaleem Catering Selections and  DOWNLOAD MENU PDF   -->

<?php if(!empty($cateringselectionmenu)||!empty($downloadmenulink)) {?>
<div class="container" >

    <!-- here rendering cruise menu content -->
<?php if(!empty($cateringselectionmenu)){ ?>
    <div class="weddingmenu" id="cruiseMenuRender" style=<?php //echo $catselection ?> >
        
        <div class="title">
            Catering Selections
        <?php }?>
            
           
            <?php if(!empty($downloadmenulink)){ ?> 
       
            <div class="pull-right responsiveWeb">
                <!--<img src="<?php //echo base_path(); ?>sites/default/files/cruisemenu/download-icon.png"><a href="<?php// print $downloadmenulink; ?>" target="_blank"> DOWNLOAD MENU PDF</a>-->
            <?php 
            if($downloadmenulink!="javascript:viod(0)"){
   ?>
                
      
          
          <img src="<?php echo base_path(); ?>sites/default/files/cruisemenu/download-icon.png" ><a href="<?php print $downloadmenulink; ?>" target="_blank"> DOWNLOAD MENU PDF</a>
            
   <?php 

   }
            
            ?>
            </div>
                  
        <?php  } ?>
        </div>
        <!-- testing -->
        <!--            <div class="subtitle">Freshly baked artisan bread served with herbed butter.</div>-->
    <?php if(!empty($cateringselectionmenu)) {?>
        <div id="cateringSelectionMenu">
            <ul class="cateringSelectionMenu" id="privatecatering">
                
                <?php print $cateringselectionmenu; ?>
            </ul>
        </div>
    <?php }?>
    </div>
    <!-- dinnermenu end here -->
    <div id="upArrow" class="privatecruiseUpArrow">
        <img src="<?php print base_path() . drupal_get_path('module', 'private_events') . '/images/sunset-banner-uparrow.png'; ?>" />
    </div>

</div>
<?php } ?>




<!-- sunset map view start here -->

<!-- sunset map view start here -->




</div>






