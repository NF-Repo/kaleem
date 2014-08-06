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

    var dirbasepath = Drupal.settings.basePath;
    $(function() {
      
      
      //slider one api code gets here
      
      $('#crewtempfirstCarousel').carousel({
       interval:5000
                 }).on('slide', function (e) {
        var slideFrom = $('#crewtempfirstCarousel').find('.active').index();
   var slideTo = $(e.relatedTarget).index();
       var nextindicatorname='#crewfirsttempind'+slideTo;
     $(".crewtempfirstindicators li div").removeClass("active");
   $(nextindicatorname).addClass("active");
     });
  
  $(document).on('click','.crewfirsttempindicator',function(){
    var indicatorid=parseInt(this.id.slice(16));
    
   $('#crewtempfirstCarousel').carousel('pause');
    $("#crewtempfirstCarousel").carousel(indicatorid);
    });
    
    
   $("#crewfirsttempnext").click(function(e){
   $('#crewtempfirstCarousel').carousel('next'); 
   $("#crewtempfirstCarousel").carousel("cycle");
     e.preventDefault(); 
                  });
      $("#crewfirsttempprev").click(function(e){
      $('#crewtempfirstCarousel').carousel('prev'); 
      $("#crewtempfirstCarousel").carousel("cycle");
         e.preventDefault(); 
       });
      
      //slider one api gets here
      
         $("#crewtempfirstslideritems div:first-child").addClass("active");
        if($("#crewtempfirstslideritems").children().length>1){
            $("#crewtempfirstslideritems .item").each(function(index){
                if(index==0){
               var indicators='<li><div class="crewfirsttempindicator active" id="crewfirsttempind'+index+'"></div></li>';

    }else{
        var indicators='<li><div class="crewfirsttempindicator" id="crewfirsttempind'+index+'"></div></li>';
                }
                $(".crewtempfirstindicators").append(indicators);
            });
        }
        
        //slider one api ends here
        
        
        $('#crewtempsecondCarousel').carousel({
       interval:5000
                 }).on('slide', function (e) {
        var slideFrom = $('#crewtempsecondCarousel').find('.active').index();
   var slideTo = $(e.relatedTarget).index();
       var nextindicatorname='#crewsecondtempind'+slideTo;
     $(".crewtempsecondindicators li div").removeClass("active");
   $(nextindicatorname).addClass("active");
     });
  
  $(document).on('click','.crewsecondtempindicator',function(){
    var indicatorid=parseInt(this.id.slice(17));
    
   $('#crewtempsecondCarousel').carousel('pause');
    $("#crewtempsecondCarousel").carousel(indicatorid);
    });
    
    
   $("#crewsecondtempnext").click(function(e){
   $('#crewtempsecondCarousel').carousel('next'); 
   $("#crewtempsecondCarousel").carousel("cycle");
     e.preventDefault(); 
                  });
      $("#crewsecondtempprev").click(function(e){
      $('#crewtempsecondCarousel').carousel('prev'); 
      $("#crewtempsecondCarousel").carousel("cycle");
         e.preventDefault(); 
       });
        //slider two api gets here
        $("#crewtempsecondslideritems div:first-child").addClass("active");
        if($("#crewtempsecondslideritems").children().length>1){
            $("#crewtempsecondslideritems .item").each(function(index){
                if(index==0){
               var indicators='<li><div class="crewsecondtempindicator active" id="crewsecondtempind'+index+'"></div></li>';

    }else{
        var indicators='<li><div class="crewsecondtempindicator" id="crewsecondtempind'+index+'"></div></li>';
                }
                $(".crewtempsecondindicators").append(indicators);
            });
        }
        
        //slider two api gets ends here
    });
</script>




<div id="content">
    <div class="contentDetail">
        <!-- sunset dining start here -->
        <div class="container crew-member">
            <div class="row-fluid cruiseDetailinfo">
                <div class="span12">
                    <div class="cruisedetailBackLink responsiveWeb"><a href="<?php echo base_path() ?>crewview"><img src="<?php echo base_path() ?>sites/all/modules/custom/privateevents_cruise/images/back_arrow.png"> BACK TO OUR CREW</a></div>

                    <div class="row-fluid">
                        <div class="span12">
                            <div class="crewTitle"> <?php print $crewname; ?> </div>
                        </div>
                    </div><!-- row-fluid end -->

                </div>
            </div>
        </div>
        <div class="row-fluid">
            <div class="span12">

                <div class="aboutus-slider-buttons">
                    <!-- removed downarrow from here -->
                    <!--member template slider one-->
                  
                    <div id="crewtempfirstCarousel" class="carousel slide">
                        <ul class="crewtempfirstindicators">

                        </ul>
                        <div class="carousel-inner" id="crewtempfirstslideritems">
                            <?php print $crewheroimage; ?>
                        </div>
                      
                      
            <a class="carousel-control left" href="javascript:void(0)" id="crewfirsttempprev"><img src="<?php echo base_path() . path_to_theme(); ?>/images/carousol-leftarrow.png"></a>
       <a class="carousel-control right" href="javascript:void(0)" id="crewfirsttempnext"><img src="<?php echo base_path() . path_to_theme(); ?>/images/carousol-rightarrow.png"></a>
            
                      
                        <!--box view-->
                        <div class="aboutusDownsliderbox">
                            <div class="aboutusDownsliderInner">
                                <div align="center" class="row-fluid">

                                    <div class="span2 left-line-margin sunsetLinePadding"><div class="line"></div></div>
                                    <div class="span8"><div class="sliderCaption"><?php echo $titleone;?></div></div>
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
                    <!--member slisder first ends here-->
                    <!--end of box view-->


                </div>
            </div>

        </div>
    </div>


</div>
<!-- sunset down slider start here -->
<div class="row-fluid">
    <div class="span12">
        <!-- sunsetDownslider start here -->
        <div class="aboutusDownslider">                
            <div id="crewtempsecondCarousel" class="carousel slide">
                <ul class="crewtempsecondindicators">

                </ul>
                <div class="carousel-inner" id="crewtempsecondslideritems">
                    <?php print $biographysliders; ?>
                </div>
                <!-- Carousel nav -->
                <a class="carousel-control left" href="javascript:void(0)" id="crewsecondtempprev"><img src="<?php echo base_path() . path_to_theme(); ?>/images/carousol-leftarrow.png"></a>
                <a class="carousel-control right" href="javascript:void(0)" id="crewsecondtempnext"><img src="<?php echo base_path() . path_to_theme(); ?>/images/carousol-rightarrow.png"></a>
            
            </div>

            <div class="aboutusDownsliderbox">
                <div class="aboutusDownsliderInner">
                    <div class="row-fluid" align="center">

                        <div class="span2 left-line-margin sunsetLinePadding"><div class="line"></div></div>
                        <div class="span8"><div class="sliderCaption"><?php echo $titletwo;?></div></div>
                        <div class="span2 sunsetLinePadding"><div class="line"></div></div>
                    </div>

                    <div class="sliderInnercontent">

                        <?php print $biographyslidercontent; ?>

                    </div>

                    <div class="row-fluid" id="routeMaplinesholder">
                        <div class="span5 "><div class="line"></div></div>
                        <div class="span2" align="center"><img src="<?php echo base_path() . path_to_theme(); ?>/images/banner-icon-img.png"></div>
                        <div class="span5 "><div class="line"></div></div>
                    </div>
                </div><!-- sunsetDownsliderInner end -->
            </div>
        </div>
        <!-- sunsetDownslider end here -->
    </div>
</div>
<!-- sunset down slider end here -->





</div>



