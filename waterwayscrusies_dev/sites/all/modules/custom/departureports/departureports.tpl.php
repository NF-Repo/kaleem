<?php
/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
?>

<script defer src="<?php echo base_path() . path_to_theme() ?>/js/jquery.bxslider.js"></script>
<link rel="stylesheet" href="<?php echo base_path() . path_to_theme() ?>/css/jquery.bxslider.css" type="text/css" />

<script type="text/javascript">
        function portsDetails(nid,aliaspath){
   


   if(aliaspath=='false'){
	   window.location.href = '<?php echo base_path() ?>portsdetailview/portid/' + nid;
	}
	else{
		window.location.href = '<?php echo base_path(); ?>'+aliaspath;

	}

	
    
}
    
    $(function() {
        
        // menu active script

        // To do active Tab
        $(".responsiveWeb li").removeClass("active");
        $('.departureports').addClass("active");
        
        $('.kirklandDeparture').bxSlider({
            infiniteLoop: false,
            hideControlOnEnd: true,
            minSlides: 1,
            maxSlides: 3,
            slideWidth: 300,
            slideHeight: 300,
            slideMargin: 10,
            touchEnabled: true,
//            pager: false
        });
       
                $('.portsList li .sliderBox').mouseover(function(e) {
                    $(this).children(".sliderBoxout").hide();
                    $(this).children(".sliderBoxover").show();
                });
                
                $('.portsList li .sliderBox').mouseout(function(e) {
                    $(this).children(".sliderBoxout").show();
                    $(this).children(".sliderBoxover").hide();
                });
     
    });
</script>

<div class="container">
    <div class="contentbox-inner">
        <?php print $loadportscontent; ?>

    </div>
</div>