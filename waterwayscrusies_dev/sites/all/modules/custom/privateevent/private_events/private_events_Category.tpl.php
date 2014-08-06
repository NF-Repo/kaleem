<script defer src="<?php echo base_path() . path_to_theme() ?>/js/jquery.bxslider.js"></script>
<link rel="stylesheet" href="<?php echo base_path() . path_to_theme() ?>/css/jquery.bxslider.css" type="text/css" />


<link rel="stylesheet" href="<?php echo base_path() . path_to_theme() ?>/magnific-popup/magnific-popup.css" type="text/css" />

<script defer src="<?php echo base_path() . path_to_theme() ?>/magnific-popup/jquery.magnific-popup.js"></script>

<script>

function packageDetailsView(eventId,packageCategoryId){
	 window.location.href = '<?php echo base_path() ?>private_events/packages/' + eventId+'/'+packageCategoryId;
  // alert('hello');
   //window.location.href = '<?php //echo base_path() ?>//eventscruise/' + nid;

}

    $(function() {
        var selectedMenuItem = '<?php //echo $selectedmenuelement; ?>';
        //
        $("#weddingsRequestProposal").click(function() {
            $("#myModalrequest").modal('show');
        });
        // menu active script
        // To do active Tab
        $(".responsiveWeb li").removeClass("active");
        $('.privateevents').addClass("active");

        $('.weddingcategory li .sliderBox').mouseover(function(e) {
            $(this).children(".sliderBoxout").hide();
            $(this).children(".sliderBoxover").show();
        });

        $('.weddingcategory li .sliderBox').mouseout(function(e) {
            $(this).children(".sliderBoxout").show();
            $(this).children(".sliderBoxover").hide();
        });
        $('.weddingcategory').bxSlider({
        	 infiniteLoop: false,
             hideControlOnEnd: true,
             minSlides: 2,
             maxSlides: 3,
             slideWidth: 300,
             slideHeight: 300,
             slideMargin: 10,
             touchEnabled: true
       });

        $('.eventsbxslider').magnificPopup({
            delegate: 'a',
            closeMarkup:'<button title="Close (Esc)" type="button" class="mfp-close"><div class="mfp-close close-text">Close</div></button>',
        
            // child items selector, by clicking on it popup will open
            gallery: {
                enabled: true
            }
            //  type:'image'
  
            // other options
        });


        $('.eventsbxslider').bxSlider({
            infiniteLoop: false,
            hideControlOnEnd: true,
            minSlides: 1,
            maxSlides: 5,
            slideWidth: 300,
            slideHeight: 250,
            slideMargin: 10,
            touchEnabled: true
        });
        
        $('.responsiveMobile select').change(function() {
            window.location.href =  $(this).val();
        });

    });
</script>



<div class="cruiseDetailinfo privateEventsWeddings">
    <div class="row-fluid">
        <div class="span12">
           
             <div class="detailBackLink"><a href="<?php echo base_path()?>private_events"><img src="<?php echo base_path() . path_to_theme(); ?>/images/back_arrow.png" /> BACK TO PRIVATE EVENTS</a></div>
            <div class="row-fluid">
                
                <div class="span12">
                    <div class="weddingTitle"><?php print $eventTitle; ?></div>




                    <!--sub Menu links Area-->

                    <div class="responsiveWeb subMenu">
   <!--kaleem getting the sub-tab-name example Overview,venodor,gallery,packages-->
   
   
   
                        <?php print $eventWebMenuHeaderTab; ?>
                    </div>

                    <!--End Of Sub Menu Links Area-->

                </div>

            </div><!-- row-fluid end -->

        </div>
    </div>
</div>





<!--submenu in mobile view-->
<div class="responsiveMobile subMenu">
    <select>
        <?php print $mobilemenu ?>
    </select>
</div>




<!--doe-->


<!-- sunset down slider start here -->
<div class="row-fluid">
    <div class="span12">
        <!-- sunsetDownslider start here -->
        <div class="sunsetDownslider">

            <!-- kaleem Background image of Overview,venodor,gallery,packages -->
            
            

         <img src="<?php echo file_create_url($backgroundimage); ?>" class="imagefixedwd" />
          
           
           
<!--//kaleem private event gallery tab-->
            <?php if($gallery){ ?>
            <div class="row-fluid eventsDownslidermargin privateEventsGallery">
                <ul class="eventsbxslider">
                    <?php print $gallery ?>
                </ul>
            </div>
            <?php } ?>
         
            <!--short description banner gets loads here-->
            <?php if($gallery){ ?>
            <div class="weddingDownsliderbox" style="display: none;">
            <?php }else{ ?>
            <div class="weddingDownsliderbox">
            <?php } ?>    
               
                
                <?php 
                //If Widget Title is Empty hide the Widget Box
                if(!empty($widgetTitle)){ ?>
				<div class="sunsetDownsliderInner">
                    <div class="row-fluid">
                        <div class="span2 sunsetLinePadding"><div class="line"></div></div>
                        
                        <!--  kaleem Tittle of Inner dropbox of submenu  -->
                        
                        <div class="span8"><div class="sliderCaption"><?php print $widgetTitle; ?></div></div>
                        <div class="span2 sunsetLinePadding"><div class="line"></div></div>
                    </div>

                    <div class="sliderInnercontent">
                        
                        <!--  kaleem Description of Inner dropbox of submenu-->
                        
                        <div class="weddingshortdescription"> <?php print $widgetDescription; ?></div>
                        <div class="seeFleet">
                         <?php 
                       //  echo $widget_button_name; exit();
                         if(!empty($widget_button_name)){
						global $base_url;
						?>
						
						<?php 
						if(!empty($widget_button_url)){
							?>
							<div class="request" id=""><a target="_blank" href="<?php echo $widget_button_url?>"><?php echo $widget_button_name;?></a></div>
							<?php
						}else{
						?>
						<div class="request" id="weddingsRequestProposal"><a href="#">REQUEST FOR PROPOSAL</a></div>
						
						<?php 
						}
						
						
						?>
						
                         	
                         <?php }else{?>
                         	<div class="request" id="weddingsRequestProposal"><a href="#">REQUEST FOR PROPOSAL</a></div>
                         <?php }
                         
                         ?>   
                        
                    </div>

                    <div class="row-fluid" id="routeMaplinesholder">
                        <div class="span5"><div class="line"></div></div>
                        <div class="span2" align="center"><img src="<?php print base_path() . drupal_get_path('module', 'cruisesdetailview') . '/images/banner-icon-img.png'; ?>" /></div>
                        <div id="line2routemap" class="span5"><div class="line"></div></div>
                    </div>
                </div>
            </div>
                <!-- short description content widget ends here -->


                	
                <?php }  ?>
                
                

            </div>
        <!-- sunsetDownslider end here -->
    </div>
</div>
<!-- sunset down slider end here -->

<?php if(!empty($categoryslider))
{
	?>
	<!--Package Slider-->
<div class="container">
    <div class="contentbox-inner">

        <div class="row-fluid tiles">
            <div class="span12">
                <ul class="weddingcategory">
                    <?php

                    print $categoryslider; ?>

                </ul>
            </div>
        </div>
        
    </div>

</div>
	<?php
}
	?>

