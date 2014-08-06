<?php
/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
?>

<!-- Bxslider javascript and style -->
<!--<script defer src="<?php echo base_path() . path_to_theme() ?>/js/jquery.bxslider.js"></script>
<link rel="stylesheet" href="<?php echo base_path() . path_to_theme() ?>/css/jquery.bxslider.css" type="text/css" />-->




<script>

    var dirbasepath = Drupal.settings.basePath;
    $(function() {
        $('.responsiveMobile select').change(function() {
            window.location.href = '<?php echo base_path() ?>' + $(this).val();
        });
        
        

    });
</script>




<div id="content" class="aboutusCorporateContainer">
    <div class="contentDetail">
        <!-- sunset dining start here -->
        <div class="container">
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
            <div class="row-fluid aboutcorporate">
                <div class="span6">
                    <div><?php print $pagecontent; ?></div>
                   
                </div>
                <div class="span6 address-container">
                    <div class="address-content">
                     <b>
                        <div><?php print $corporatepageaddress; ?></div>
                    </b>
                    <br>
                    <div class="corporate-details">
                        <b>
                            <div><img src="<?php echo base_path(); ?>sites/all/themes/waterways/assets/img/phone-footer-icon.png">&nbsp;<?php print $corporatepagephone; ?></div>
                            <div><img src="<?php echo base_path(); ?>sites/all/themes/waterways/assets/img/fax-footer-icon.png">&nbsp;Fax:<?php print $corporatepagefax; ?></div>
                            <div><img src="<?php echo base_path(); ?>sites/all/themes/waterways/assets/img/email-footer-icon.png">&nbsp;<a href="mailto:<?php print $corporatepagecontactemail; ?>" class="emailLinkColor"><?php print $corporatepagecontactemail; ?></a></div>
                        </b>
                    </div>
                     <br>
                         <div class="row-fluid requset-proposal" align="center">

                            <div class="span2 sunsetLinePadding left-line-margin">
                                <div class="line"></div>
                            </div>
                            <div class="span8">
                                <!-- booknow start here -->
                                <div class="booknow ">
                                    <a href="#corporateGiving" role="button" data-toggle="modal">
                                        <div class="booknow-inner">Corporate Giving Form</div>
                                    </a>
                                </div>
                                <!-- booknow end here -->
                            </div>
                            <div class="span2 sunsetLinePadding right-line-margin">
                                <div class="line"></div>
                            </div>
                        </div>
                    
                </div>
                </div> 
            </div>
           

        </div>


    </div>



