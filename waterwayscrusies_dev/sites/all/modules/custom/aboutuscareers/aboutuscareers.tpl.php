<?php
/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
?>
<script>

    var dirbasepath = Drupal.settings.basePath;
    $(function() {
        $('.responsiveMobile select').change(function() {
            window.location.href='<?php echo base_path() ?>' + $(this).val();
        });
    });
</script>

<div id="content">
    <div class="contentDetail careers-container">
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


        <div class="row-fluid careerscontent">
            <div class="span12 careersheroimage">
                <?php print $careersheroimage; ?>
            
            <div class="aboutusDownsliderbox">
                <div class="aboutusDownsliderInner">
                    <div align="center" class="row-fluid">

                        <div class="span2 left-line-margin sunsetLinePadding"><div class="line"></div></div>
                        <div class="span8"><div class="sliderCaption"><?php echo $boxtitle;?></div></div>
                        <div class="span2 sunsetLinePadding"><div class="line"></div></div>
                    </div>

                    <div class="sliderInnercontent">
                        <p><?php print $careersherocontent; ?></p>                                </div>
                    <div id="routeMaplinesholder" class="row-fluid">
                        <div class="span5 "><div class="line"></div></div>
                        <div align="center" class="span2"><img src="<?php echo base_path(); ?>sites/all/modules/custom/aboutusourcrew/images/banner-icon-img.png"></div>
                        <div class="span5 "><div class="line"></div></div>
                    </div>

                </div>
</div>

            </div>



        </div>

        <div class="row-fluid featuredJobsList">

            <div class="span12">
                <div class="featuredJobs">
                    <?php print $featuredjobs; ?>
                </div>
            </div>
        </div>

        <!-- sunset down slider end here -->





    </div>



