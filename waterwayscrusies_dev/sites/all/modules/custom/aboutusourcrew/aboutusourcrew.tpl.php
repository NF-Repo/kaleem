<?php

/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
?>

<!-- Bxslider javascript and style -->
<script defer src="<?php echo base_path() . path_to_theme() ?>/js/jquery.bxslider.js"></script>
<link rel="stylesheet" href="<?php echo base_path() . path_to_theme() ?>/css/jquery.bxslider.css" type="text/css" />




<script>
    function crewMemberDetails(nid){
        window.location.href = '<?php echo base_path() ?>crewmember/id/' + nid;
    }
    var dirbasepath = Drupal.settings.basePath;
    $(function() {
        $('.membersList li .sliderBox').mouseover(function(e) {
            $(this).children(".sliderBoxout").hide();
            $(this).children(".sliderBoxover").show();
        });

        $('.membersList li .sliderBox').mouseout(function(e) {
            $(this).children(".sliderBoxout").show();
            $(this).children(".sliderBoxover").hide();
        });
        $('.membersList').bxSlider({
            infiniteLoop: false,
            hideControlOnEnd: true,
            minSlides: 1,
            maxSlides: 3,
            slideWidth: 300,
            slideHeight: 300,
            slideMargin: 10,
            touchEnabled: false
//            pager: true
        });
        $('.responsiveMobile select').change(function() {
            window.location.href='<?php echo base_path() ?>' + $(this).val();
        });
        
        
        //first template slider api get starts here
        
        $('#aboutuscrewCarousel').carousel({
       interval:5000
                 }).on('slide', function (e) {
        var slideFrom = $('#aboutuscrewCarousel').find('.active').index();
   var slideTo = $(e.relatedTarget).index();
       var nextindicatorname='#crewind'+slideTo;
     $(".aboutuscrewCarsouselindicators li div").removeClass("active");
   $(nextindicatorname).addClass("active");
     });
  
  $(document).on('click','.aboutuscrewindicator',function(){
    var indicatorid=parseInt(this.id.slice(7));
    
   $('#aboutuscrewCarousel').carousel('pause');
    $('#aboutuscrewCarousel').carousel(indicatorid);
    });
    
    
   $("#aboutuscrewnext").click(function(e){
   $('#aboutuscrewCarousel').carousel('next'); 
   $("#aboutuscrewCarousel").carousel("cycle");
     e.preventDefault(); 
                  });
      $("#aboutuscrewprev").click(function(e){
      $('#aboutuscrewCarousel').carousel('prev'); 
      $("#aboutuscrewCarousel").carousel("cycle");
         e.preventDefault(); 
       });
       
        
        
        
        
        
        
        $("#aboutuscrewslideritems div:first-child").addClass("active");
        if($("#aboutuscrewslideritems").children().length>1){
            $("#aboutuscrewslideritems .item").each(function(index){
                if(index==0){
                   var indicators='<li><div class="aboutuscrewindicator active" id="crewind'+index+'"></div></li>';

    }else{
                       var indicators='<li><div class="aboutuscrewindicator" id="crewind'+index+'"></div> </li>';

                }
                $(".aboutuscrewCarsouselindicators").append(indicators);
            });
        }
        
        //slider api gets ends here
    });
</script>




<div id="content" class="aboutusPagesContainer">
    <div class="contentDetail">
        <!-- sunset dining start here -->
            <div class="row-fluid cruiseDetailinfo">
                <div class="span12">
                    <div class="row-fluid">

                        <div class="span12">
                            <div class="aboutusTitle">About Us</div>
                            <div class="subMenu responsiveWeb">
                                <ul>
                                    <?php print $loadmenu; ?> </ul>
                            </div>
                            <div class="subMenu responsiveMobile">

                                <select>
                                    <?php print $mobilemenu; ?>    
                                </select>


                            </div>

                        </div>
                    </div><!-- row-fluid end -->

                </div>
            </div>

        <div class="row-fluid">
            <div class="span12">

                <div class="aboutus-slider-buttons">
                    <!-- removed downarrow from here -->
                   <!--first template slider--> 
                    <div id="aboutuscrewCarousel" class="carousel slide">
                        <ul class="aboutuscrewCarsouselindicators">

                        </ul>
                        <div class="carousel-inner" id="aboutuscrewslideritems">
                            <?php print $crewheroimage; ?>
                        </div>
                        <a class="carousel-control left" href="javascript:void(0)" id="aboutuscrewprev"><img src="<?php echo base_path() ?>sites/all/modules/custom/fleet/images/carousol-leftarrow.png"></a>
                        <a class="carousel-control right" href="javascript:void(0)" id="aboutuscrewnext"><img src="<?php echo base_path() ?>sites/all/modules/custom/fleet/images/carousol-rightarrow.png"></a>

                        <!--box view-->
                        <div class="aboutusDownsliderbox">
                            <div class="aboutusDownsliderInner">
                                <div align="center" class="row-fluid">

                                    <div class="span2 left-line-margin sunsetLinePadding"><div class="line"></div></div>
                                    <div class="span8"><div class="sliderCaption"><?php echo $boxtitle;?></div></div>
                                    <div class="span2 sunsetLinePadding"><div class="line"></div></div>
                                </div>

                                <div class="sliderInnercontent">
                                    <?php print $crewherocontent; ?>
                                </div> 
                                <div id="routeMaplinesholder" class="row-fluid">
                                    <div class="span5 "><div class="line"></div></div>
                                    <div align="center" class="span2"><img src="<?php echo base_path() . path_to_theme(); ?>/images/banner-icon-img.png"></div>
                                    <div class="span5 "><div class="line"></div></div>
                                </div>

                            </div>


                        </div><!-- sunsetDownsliderInner end -->
                    </div>
                   <!--end of first template slider-->
                   
                   
                   
                   
                    <!--end of box view-->


                </div>
            </div>

        </div>
          <div class="row-fluid members-row">
    <div class="span12 tiles">
        <ul class="membersList">
            <?php print $crewmembers ?>
        </ul>
    </div>
</div>
    </div>


</div>





</div>



