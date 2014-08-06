
<script defer src="<?php echo base_path() . path_to_theme() ?>/js/jquery.bxslider.js"></script>
<link rel="stylesheet" href="<?php echo base_path() . path_to_theme() ?>/css/jquery.bxslider.css" type="text/css" />

<script type="text/javascript">


function packageDetailsView(eventId,packageCategoryId){
	 window.location.href = '<?php echo base_path() ?>private_events/packages/' + eventId+'/'+packageCategoryId;
   // alert('hello');
    //window.location.href = '<?php //echo base_path() ?>//eventscruise/' + nid;

}


       function privateEventsDetails(nid,aliaspath){

    	   if(aliaspath=='false'){
    		   window.location.href = '<?php echo base_path() ?>eventscruise/' + nid;
    		}
    		else{
    			window.location.href = '<?php echo base_path(); ?>'+aliaspath;

    		}

      		
  
    
}
    $(function() {

        // menu active script
        // To do active Tab
        
        $(".responsiveWeb li").removeClass("active");
        $('.privateevents').addClass("active");

        $('li .sliderBox').mouseover(function(e) {
            $(this).children(".sliderBoxout").hide();
            $(this).children(".sliderBoxover").show();
        });

        $('li .sliderBox').mouseout(function(e) {
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

    });
</script>



<div class="container">
    <div class="contentbox-inner">
        <h1 class="weddingTitle visible-phone hidden-desktop hidden-tablet">Private Events</h1>
        <?php print $loadprivatecontent;?>
    </div>
</div>