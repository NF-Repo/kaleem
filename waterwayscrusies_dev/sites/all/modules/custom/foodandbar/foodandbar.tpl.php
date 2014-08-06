
<script defer src="<?php echo base_path() . path_to_theme() ?>/js/jquery.bxslider.js"></script>
<link rel="stylesheet" href="<?php echo base_path() . path_to_theme() ?>/css/jquery.bxslider.css" type="text/css" />

<script type="text/javascript">
    function foodandbarDetails(nid,aliaspath){


    	if(aliaspath=='false'){
    		window.location.href = '<?php echo base_path() ?>foodandbar/category/' + nid;
 		}
 		else{
 			window.location.href = '<?php echo base_path(); ?>'+aliaspath;

 		}

 		
        
    
    }
    $(function() {

        // menu active script
        // To do active Tab
        
        $(".responsiveWeb li").removeClass("active");
        $('.foodandbar').addClass("active");
        $('.foodandbarlist').bxSlider({
            infiniteLoop: false,
            hideControlOnEnd: true,
            minSlides: 2,
            maxSlides: 3,
            slideWidth: 300,
            slideHeight: 300,
            slideMargin: 10,
            touchEnabled: true
        });
        $('.foodandbarlist li .sliderBox').mouseover(function(e) {
            $(this).children(".sliderBoxout").hide();
            $(this).children(".sliderBoxover").show();
        });
                
        $('.foodandbarlist li .sliderBox').mouseout(function(e) {
            $(this).children(".sliderBoxout").show();
            $(this).children(".sliderBoxover").hide();
        });
    });
</script>

<div class="container menu-container wwcfixedContainer">
    <div class="contentbox-inner menulist-container tiles">
        <div class="eventTitle">FOOD & BAR</div>
        <ul class="foodandbarlist">
            <?php print $loadfoodandbar; ?>
        </ul>
    </div>
</div>