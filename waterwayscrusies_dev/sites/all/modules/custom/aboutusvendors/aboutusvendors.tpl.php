<?php
/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
?>


<!--l-->
        <style>           
            .wide, .thin { clear:both;font-size: 16px; }
        </style>
        <script defer src="<?php echo base_path() . path_to_theme() ?>/js/jquery.columnizer.js"></script>
        <script>
            $(function() {
                
                var curbrowser=get_browser();
                var curbrowserver=get_browser_version();
                if(curbrowser=='MSIE'){
                 $('#forievendors').css("display", "block");
                    $('#forothervendors').css("display", "none");
                } else {
                    $('#forievendors').css("display", "none");
                    $('#forothervendors').css("display", "block");
                }
                $('.wide').columnize({width: 300});
            });

            function get_browser() {
                var N = navigator.appName, ua = navigator.userAgent, tem;
                var M = ua.match(/(opera|chrome|safari|firefox|msie)\/?\s*(\.?\d+(\.\d+)*)/i);
                if (M && (tem = ua.match(/version\/([\.\d]+)/i)) != null)
                    M[2] = tem[1];
                M = M ? [M[1], M[2]] : [N, navigator.appVersion, '-?'];
                return M[0];
            }
            function get_browser_version() {
                var N = navigator.appName, ua = navigator.userAgent, tem;
                var M = ua.match(/(opera|chrome|safari|firefox|msie)\/?\s*(\.?\d+(\.\d+)*)/i);
                if (M && (tem = ua.match(/version\/([\.\d]+)/i)) != null)
                    M[2] = tem[1];
                M = M ? [M[1], M[2]] : [N, navigator.appVersion, '-?'];
                return M[1];
            }
        </script>
        
        
        
        
        
<!-- Bxslider javascript and style -->

<!--<script defer src="<?php echo base_path() . path_to_theme() ?>/js/jquery.bxslider.js"></script>
<link rel="stylesheet" href="<?php echo base_path() . path_to_theme() ?>/css/jquery.bxslider.css" type="text/css" />




<script>

    var dirbasepath = Drupal.settings.basePath;
    $(function() {
        $('.responsiveMobile select').change(function() {
            window.location.href = '<?php echo base_path() ?>' + $(this).val();
        });

    });
</script>




<div id="content" class="aboutusVendorsContainer">
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
            <div class="row-fluid aboutvendors">
                <div class="span12 vendorColumns">
                    <div class="vendorgroup" id="forothervendors">
                        <?php print $vendorlist; ?>
                    </div>
                    <div class="vendorgroup wide" id="forievendors">
                        <?php print $vendorlist; ?>
                    </div>
                </div>
            </div>
        </div>


    </div>



