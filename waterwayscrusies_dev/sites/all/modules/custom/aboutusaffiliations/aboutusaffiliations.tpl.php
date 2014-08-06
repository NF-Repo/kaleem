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
        $('.membersList li .sliderBox').mouseover(function(e) {
            $(this).children(".sliderBoxout").hide();
            $(this).children(".sliderBoxover").show();
        });

        $('.membersList li .sliderBox').mouseout(function(e) {
            $(this).children(".sliderBoxout").show();
            $(this).children(".sliderBoxover").hide();
        });
         $('.responsiveMobile select').change(function() {
            window.location.href='<?php echo base_path() ?>' + $(this).val();
        });
    });
</script>




<div id="content" class="aboutusAffiliationContainer">
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


<div>
<div class="row-fluid affiliationscontent">
    
    <div class="span12 affiliationImg">
            <?php print $affiliationsheroimage; ?>
 
    <div class="aboutusDownsliderbox">
                            <div class="aboutusDownsliderInner">
                                <div align="center" class="row-fluid">

                                    <div class="span2 left-line-margin sunsetLinePadding"><div class="line"></div></div>
                                    <div class="span8"><div class="sliderCaption"><?php echo $boxtitle;?></div></div>
                                    <div class="span2 sunsetLinePadding"><div class="line"></div></div>
                                </div>

                                <div class="sliderInnercontent">
                                    <p><?php print $affiliationsherocontent; ?></p>                                </div>
                                <div id="routeMaplinesholder" class="row-fluid">
                                    <div class="span5 "><div class="line"></div></div>
                                    <div align="center" class="span2"><img src="<?php echo base_path();?>sites/all/modules/custom/aboutusourcrew/images/banner-icon-img.png"></div>
                                    <div class="span5 "><div class="line"></div></div>
                                </div>

                            </div>


                        </div>
    
       </div>
<!--    <div class="span2 responsiveWeb"></div>
    <div class="span4 logodescription">
         
    </div>
    <div class="span4 affiliationImage" align="center">
          <?php //print $affiliationsheroimage; ?>
    </div>
    <div class="span2"></div>-->
</div>




<div class="row-fluid affiliationslogos">
    <div class="span12">
        <ul class="logosList">
            <?php print $affiliationslogos ?>
        </ul>
    </div>
</div>
    </div>
<!-- sunset down slider end here -->





</div>


