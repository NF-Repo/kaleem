<?php
/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
//print base_path();
//print drupal_get_path('module', 'cruisesdetailview');
//var_dump($nodeid);
?>

<script defer src="<?php echo base_path() . path_to_theme() ?>/js/jquery-panzoom.js"></script>

<!-- Bxslider javascript and style -->
<script defer src="<?php echo base_path() . path_to_theme() ?>/js/jquery.bxslider.js"></script>
<!--<script defer src="<?php echo base_path() . path_to_theme() ?>/js/jquery.fitvids.js"></script>-->
<link rel="stylesheet" href="<?php echo base_path() . path_to_theme() ?>/css/jquery.bxslider.css" type="text/css" />

<!-- Lightbox javascript and stylesheet -->
<!--<script defer src="<?php echo base_path() . path_to_theme() ?>/js/jquery.chocolat.js"></script>-->
<!--<link rel="stylesheet" href="<?php echo base_path() . path_to_theme() ?>/css/chocolat.css" type="text/css" />-->

<link rel="stylesheet" href="<?php echo base_path() . path_to_theme() ?>/magnific-popup/magnific-popup.css" type="text/css" />

<script defer src="<?php echo base_path() . path_to_theme() ?>/magnific-popup/jquery.magnific-popup.js"></script>


<script>

    var dirbasepath = Drupal.settings.basePath;
    $(function() {
        
         
        $("#carousel-indicators li:first-child").addClass("active");
        $('.responsiveMobile select').change(function() {
            window.location.href='<?php echo base_path() ?>' + $(this).val();
        });
        
        //aboutus slider first carousel gets here
        
        //slider pi code gets starts here
        $('#aboutusfirstCarousel').carousel({
            interval:5000
        }).on('slide', function (e) {
            var slideFrom = $('#aboutusfirstCarousel').find('.active').index();
            var slideTo = $(e.relatedTarget).index();
            var nextindicatorname='#aboutusfirstind'+slideTo;
            $(".aboutsoverviewfirstindicators li div").removeClass("active");
            $(nextindicatorname).addClass("active");
        });
  
        $(document).on('click','.aboutusfirstindicator',function(){
            var indicatorid=parseInt(this.id.slice(15));
    
            $('#aboutusfirstCarousel').carousel('pause');
            $('#aboutusfirstCarousel').carousel(indicatorid);
        });
    
    
        $("#aboustfirstnext").click(function(e){
            $('#aboutusfirstCarousel').carousel('next'); 
            $("#aboutusfirstCarousel").carousel("cycle");
            e.preventDefault(); 
        });
        $("#aboustfirstprev").click(function(e){
            $('#aboutusfirstCarousel').carousel('prev'); 
            $("#aboutusfirstCarousel").carousel("cycle");
            e.preventDefault(); 
        });
       
       
        //slider api code gets ends here
        $("#aboutusfirstsliderinneritems div:first-child").addClass("active");
        if($("#aboutusfirstsliderinneritems").children().length>1){
            $("#aboutusfirstsliderinneritems .item").each(function(index){
                if(index==0){
                    
                    var indicators='<li><div class="aboutusfirstindicator active" id="aboutusfirstind'+index+'"></div> </li>';

    
                }else{
                    var indicators='<li><div class="aboutusfirstindicator" id="aboutusfirstind'+index+'"></div> </li>';

                }
                $(".aboutsoverviewfirstindicators").append(indicators);
            });
        }
        
        //aboutus first acrousel gets ends 
        
        //Second slider gets loads here
        $('#aboutussecondCarousel').carousel({
            interval:5000
        }).on('slide', function (e) {
            var slideFrom = $('#aboutussecondCarousel').find('.active').index();
            var slideTo = $(e.relatedTarget).index();
            var nextindicatorname='#aboutussecondind'+slideTo;
            $(".aboutussecondindicators li div").removeClass("active");
            $(nextindicatorname).addClass("active");
        });
  
        $(document).on('click','.aboutussecondindicator',function(){
            var indicatorid=parseInt(this.id.slice(16));
    
            $('#aboutussecondCarousel').carousel('pause');
            $('#aboutussecondCarousel').carousel(indicatorid);
        });
    
    
        $("#aboutussecondnext").click(function(e){
            $('#aboutussecondCarousel').carousel('next'); 
            $("#aboutussecondCarousel").carousel("cycle");
            e.preventDefault(); 
        });
        $("#aboutussecondprev").click(function(e){
            $('#aboutussecondCarousel').carousel('prev'); 
            $("#aboutussecondCarousel").carousel("cycle");
            e.preventDefault(); 
        });
        
        
        
        $("#aboutussecondCarousel div:first-child").addClass("active");
        if($("#aboutussecondslideritems").children().length>1){
            $("#aboutussecondslideritems .item").each(function(index){
                if(index==0){
                    var indicators='<li><div class="aboutussecondindicator active" id="aboutussecondind'+index+'"></div> </li>';



                }else{
                    var indicators='<li><div class="aboutussecondindicator" id="aboutussecondind'+index+'"></div> </li>';
                }
                $(".aboutussecondindicators").append(indicators);
            });
        }
        //end of second slisder
    });
</script>




<div id="content" class="aboutusPagesContainer">
    <div class="contentDetail">


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
                <div class="aboutusDownslider">  
                    <div class="aboutus-slider-buttons">
                        <!-- removed downarrow from here -->
                        <!--slider one gets here-->
                        <div id="aboutusfirstCarousel" class="carousel slide">
                            <ul class="aboutsoverviewfirstindicators"></ul>
                            <div class="carousel-inner" id="aboutusfirstsliderinneritems">
                                <?php print $overviewheroimage; ?>
                            </div>
                            <a class="carousel-control left" href="javascript:void(0)" id="aboustfirstprev"><img src="<?php echo base_path() ?>sites/all/modules/custom/fleet/images/carousol-leftarrow.png"></a>
                            <a class="carousel-control right" href="javascript:void(0)" id="aboustfirstnext"><img src="<?php echo base_path() ?>sites/all/modules/custom/fleet/images/carousol-rightarrow.png"></a>

                         
                        </div>
                           <!--box view-->
                            <div class="aboutusupsliderbox">
                                <div class="aboutusDownsliderInner">
                                    <div align="center" class="row-fluid">

                                        <div class="span2 left-line-margin sunsetLinePadding"><div class="line"></div></div>
                                        <div class="span8"><div class="sliderCaption"><?php echo $firstTitle;?></div></div>
                                        <div class="span2 sunsetLinePadding"><div class="line"></div></div>
                                    </div>

                                    <div class="sliderInnercontent">
                                        <?php print $overviewherocontent; ?>
                                    </div> 
                                    <div id="routeMaplinesholder" class="row-fluid">
                                        <div class="span5 "><div class="line"></div></div>
                                        <div align="center" class="span2"><img src="<?php echo base_path() ?>sites/all/modules/custom/fleet/images/banner-icon-img.png"></div>
                                        <div class="span5 "><div class="line"></div></div>
                                    </div>

                                </div>


                            </div><!-- sunsetDownsliderInner end -->
                        <!--slider one gets ends here-->
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

                <!--slider second gets loads here-->

                <div id="aboutussecondCarousel" class="carousel slide">
                    <ul class="aboutussecondindicators">                                    
                    </ul>
                    <div class="carousel-inner" id="aboutussecondslideritems">
                        <?php print $overviewsliders; ?>
                    </div>
                    <!-- Carousel nav -->
                    <a class="carousel-control left" href="javascript:void(0)" id="aboutussecondprev"><img src="<?php echo base_path() ?>sites/all/modules/custom/fleet/images/carousol-leftarrow.png"></a>
                    <a class="carousel-control right" href="javascript:void(0)" id="aboutussecondnext"><img src="<?php echo base_path() ?>sites/all/modules/custom/fleet/images/carousol-rightarrow.png"></a>


                </div>

                <!--end  slider two gets here-->

                <div class="aboutusDownsliderbox">
                    <div class="aboutusDownsliderInner">
                        <div class="row-fluid" align="center">

                            <div class="span2 left-line-margin sunsetLinePadding"><div class="line"></div></div>
                            <div class="span8"><div class="sliderCaption"><?php echo $secondTitle;?></div></div>
                            <div class="span2 sunsetLinePadding"><div class="line"></div></div>
                        </div>

                        <div class="sliderInnercontent">

                            <?php print $overviewslidercontent; ?>

                        </div>

                        <div class="row-fluid" id="routeMaplinesholder">
                            <div class="span5 "><div class="line"></div></div>
                            <div class="span2" align="center"><img src="<?php echo base_path() ?>sites/all/modules/custom/fleet/images/banner-icon-img.png"></div>
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



