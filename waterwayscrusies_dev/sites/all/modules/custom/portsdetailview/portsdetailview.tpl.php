<?php
/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
//print base_path();
//print drupal_get_path('module', 'cruisesdetailview');
?>

<script type="text/javascript" src="<?php echo base_path() . path_to_theme() ?>/js/sharebuttons.js"></script>

<!--Map Script file gets load here-->
<link href="<?php print base_path() . drupal_get_path('module', 'portsdetailview') . '/map/css/map.css'; ?>" rel="stylesheet" type="text/css" />
<!--<script type="text/javascript" src="http://maps.google.com/maps/api/js?sensor=false"></script>-->

<!--        <script type="text/javascript" src="<?php print drupal_get_path('module', 'portsdetailview') . '/map/jquery.gmap.js'; ?>"></script>-->
<script type="text/javascript" src="<?php print base_path() . drupal_get_path('module', 'portsdetailview') . '/map/gmap3.js'; ?>"></script>
<!--Maps Script ends here-->

<!--tab script start here--> 
<script type="text/javascript" src="<?php print base_path() . drupal_get_path('module', 'portsdetailview') . '/js/bootstrap-tab.js'; ?>"></script>
<!--tab script end here--> 


<script>

    var addr = "South Lake Union Park,860 Terry Ave N,Seattle, WA 98109";
    var dirbasepath = Drupal.settings.basePath;
    //    var tid = "<?php echo $tidval; ?>";
    var portAddress;
    var departureDetails = '';

    $(window).load(function() {

        // menu active script
        // To do active Tab
        $(".responsiveWeb li").removeClass("active");
        $('.departureports').addClass("active");

    });
    
    stLight.options({
        publisher: 'dr-52e8796a-b9e2-6a17-d82d-251b74277418',
        tracking: 'google',
        embeds: 'true',
        onhover: false
    });

    $(function() {
        
        $("#shareBtn").on("click", function(){
            $("#stwrapper").show();
        });        
        
        $("#map_addresses").gmap3({
            map: {
                action: 'getLatLng',
                address: '<?php print $loadportaddress; ?>',
                options: {
                    zoom: 13,
                    scrollwheel: false,
                    styles: [
                        {
                            featureType: "all",
                            stylers: [
                                { "hue": "#00b2ff" },
                                { "lightness": 5 },
                                { "saturation": -56 }
                            ]
                        }
                    ]
                }
            },
            marker: {
                values: [
                    {address: '<?php print $loadportaddress; ?>', data:<?php print $porttid; ?>}],
                options: {
                    draggable: false,
                    icon : new google.maps.MarkerImage('<?php echo base_path(); ?>sites/all/themes/waterways/assets/img/google-markar.png')
                },
                events: {
                    click: function(marker, event, context) {

                        //                        $('.mapDownsliderbox').show();
                    }
                }
            }
        });
        
        $('.reoutemapDirectionsclosebutton').click(function() {
            
            $('#directionsModal').modal('hide');
        });
      
        $('#directionsPopup').click(function() {
         
           // $('#directionsModal').modal('show');
           $('.getdirections_form .form-item-to #edit-to').attr('readonly','readonly');
             $('#seeparkingDirectionsModal').modal('show');
             
             google.maps.event.trigger($("#getdirections_map_canvas")[0], 'resize');

        });
        
        // parkingDirectionsModal hide function
        $('.reoutemapDirectionsclosebutton').click(function() {
            $('#parkingDirectionsModal').modal('hide');
        });

        $("#directionsContentRender div:first-child").show();

        // fullforcastModal hide function
        $('.fullforcastclosebutton').click(function() {
            $('#fullforcastModal').modal('hide');
        });


        // To do active Tab
        //$(".responsiveWeb li").removeClass("active");
        //$('.firstLink').addClass("active");

        $('#parkingDirectionsPopup').click(function() {

            //            $(".mapDownsliderbox").hide();

            $('#parkingDirectionsModal').modal('show');


        });

        $('#parkingDirectionsmobilePopup').click(function() {

            //            $(".mapDownsliderbox").hide();

            $('#parkingDirectionsModal').modal('show');


        });

        $('#seefullforcastPopup').click(function() {

            $('#fullforcastModal').modal('show');
        });

        $('#seefullforcastmobilePopup').click(function() {

            $('#fullforcastModal').modal('show');
        });




        $('.seereoutemapDirectionsclosebutton').click(function() {
            $('#seeparkingDirectionsModal').modal('hide');
        });

        $('.departurePortsclosebutton').click(function() {
            //            $(".mapDownsliderbox").hide();
        });

        //        /* Arrow scroll bottom script */
        //        $("#downArrow").click(function() {
        //            $("html, body").animate({scrollTop: $(".whetherHolder").offset().top}, 800);
        //        });


    });


</script>








<div id="content">
    <div class="row-fluid visible-phone hidden-desktop hidden-tablet">
        <div class="span12">
            <?php
            drupal_set_title($loadporttitle, $output=CHECK_PLAIN);
            ?>
            <div class="portAddressDetails">
                <h1><?php print $loadporttitle; ?></h1>
                <h4><?php print $loadportaddress; ?></h4>
                <span><?php print $portlocation; ?></span>
                <div class="seeParkingDirection">
                    <img src="<?php print base_path() . path_to_theme(); ?>/images/clickmapicon.png">
                    <a href="javascript:void(0)" class="clickMapLink" id="parkingDirectionsmobilePopup">SEE PARKING GUIDE</a>
                </div>
            </div>
            <div class="row-fluid portAddresslinesholder">
                <div class="span5"><div class="line"></div></div>
                <div class="span2" align="center"><img src="<?php print base_path() . path_to_theme(); ?>/images/banner-icon-img.png" /></div>
                <div class="span5"><div class="line"></div></div>
            </div>
        </div>
    </div>


    <div class="mapDownsliderbox">
        <div class="departurePortsclosebutton">
            <div class="departurePortsclosebuttoninner">Close</div>
        </div>
        <div class="departureMapboxInner">
            <div class="row-fluid">
                <div class="span2 LinePadding hidden-phone"><div class="line"></div></div>
                <div class="span8"><div class="portTitle"><?php print $loadporttitle; ?></div></div>
                <div class="span2 LinePadding hidden-phone"><div class="line"></div></div>
            </div>

            <div class="sliderInnercontent">
                <div class="row-fluid">
                    <div class="span1 responsiveWeb"></div>
                    <div class="portAddress span10">
                        <h3><?php print $loadportaddress; ?></h3>
                    </div>
                    <div class="span1 responsiveWeb"></div>
                </div>

                <div class="row-fluid">
                    <div class="span1 responsiveWeb"></div>
                    <div class="span10">
                        <div class="portlocation">
                            <?php print $portlocation; ?>
                        </div>
                    </div>
                    <div class="span1 responsiveWeb"></div>
                </div>
                <div class="row-fluid directionsContentRenderHeader">
                    <div class="span1"></div>
                    <div class="span11">
                        <a href="javascript:void(0)" class="clickMapLink" id="directionsPopup">DIRECTIONS</a>
                    </div>
                </div>
                <div class="row-fluid" style="margin-top: 5px;">
                    <div class="span1 responsiveWeb"></div>
                    <div class="span10">
                        <img src="<?php print base_path() . path_to_theme(); ?>/images/clickmapicon.png">
                        <a href="javascript:void(0)" class="clickMapLink" id="parkingDirectionsPopup">SEE PARKING GUIDE</a>
                    </div>
                    <div class="span1 responsiveWeb"></div>
                </div>

            </div>

            <div class="row-fluid" id="routeMaplinesholder">
                <div class="span5 "><div class="line"></div></div>
                <div class="span2" align="center"><img src="<?php print base_path() . path_to_theme(); ?>/images/banner-icon-img.png" /></div>
                <div class="span5 "><div class="line"></div></div>
            </div>
        </div><!-- departureMapboxInner end -->
    </div>
    <!-- sunset map view ends here -->

    <div class="departureMapView">
        <!--        <div id="downArrow">
                    <img src="<?php //print base_path() . path_to_theme();  ?>/images/sunset-banner-downarrow.png" />
                </div>-->
        <!--<div class="mapOverlay"></div>-->
        <div class="detailBackLink"><a href="<?php echo base_path() ?>departureports"><img src="<?php echo base_path() . path_to_theme(); ?>/images/back_arrow.png" /> BACK TO ALL PORTS</a></div>
        <div class="detailShareLink" id="shareBtn">
            <div class="waterways-btn">
                <!--<div class="waterways-btn-inner">Share</div>-->
                <span class="waterways-btn-inner st_sharethis_custom" st_url="<?php 
                        $server_url = $GLOBALS['base_url'];
                        echo $server_url.'/portsdetailview/portid/'.$porttid; ?>"  displayText="Share" >Share</span>
                <!--Share this code--> 
                <?php
//        global $base_url;
//        $mPath=  $base_url.'/portsdetailview/portid/'.$porttid;
//        $data_options = sharethis_get_options_array();
//   $sharebutton=sharethis_get_button_HTML($data_options,$mPath,$loadporttitle);
//   print $sharebutton;
                ?>

                <!--End of share this code-->
            </div>
        </div>
        <!--MAP starts here-->



        <div class="tab-pane" id="Map">

            <div id="map_addresses" class="map">
                <p>This will be replaced with the Google Map.</p>
            </div>
        </div>
        <!--Map Ends here-->




    </div>
    <!-- sunset map view start here -->

    <!--
        <div class="contentDetail">
    
             sunset dining start here 
            <div class="row-fluid cruiseDetailinfo">
                <div class="span12">
                    <div class="row-fluid">
                        <div class="span2"></div>
                        <div class="span10">
                             here rendering cruise details here 
                            <div id="cruiseDetailsHolder"></div>
    
                        </div>
                    </div> row-fluid end 
                </div>
            </div>
             sunset dining end here 
    
        </div>-->
    <div class="container">
        <div class="whetherHolder visible-desktop hidden-phone visible-tablet">
            <div class="row-fluid">
                <div class="span6">
                    <img src="<?php print $waetherwidget['currentdaycondition']['imagesurl']; ?>" width="100" height="100"/>
                    <div class="whetherTitle">Weather<br/>
                        <span><?php print isset($waetherwidget['currentdaycondition']['temperaturefarenhit']) ? $waetherwidget['currentdaycondition']['temperaturefarenhit'] : '34'; ?><sup>o</sup>F <?php print $waetherwidget['currentdaycondition']['currentwaether']; ?>
                            <div class="seefullmargin"><a href="javascript:void(0)" class="clickMapLink" id="seefullforcastPopup">SEE FULL FORECAST</a></div></span>
                    </div>
                    <img src="<?php echo base_path() . path_to_theme(); ?>/images/border-right.png" class="borderRight" />
                </div>
                <div class="span3">
                    <div class="whetherTitle">High<br/>
                        <span><?php print isset($waetherwidget['currentdayhigh']->fahrenheit) ? $waetherwidget['currentdayhigh']->fahrenheit:'39'; ?><sup>o</sup>F <?php print $waetherwidget['currentdaycondition']['currentwaether']; ?></span>
                    </div>
                </div>
                <div class="span3">
                    <div class="whetherTitle">Low<br/>
                        <span><?php print isset($waetherwidget['currentdaylow']->fahrenheit) ? $waetherwidget['currentdaylow']->fahrenheit:'29'; ?><sup>o</sup>F <?php print $waetherwidget['currentdaycondition']['currentwaether']; ?></span>
                    </div>
                </div>
            </div>
        </div>
        <div class="whetherHolder visible-phone hidden-desktop hidden-tablet">
            <div class="row-fluid">
                <div class="span6">
                    <div class="row-fluid whetherimgplace">
                        <div class="span4">
                            <img src="<?php print $waetherwidget['currentdaycondition']['imagesurl']; ?>" width="100" height="100"/>
                        </div>
                        <div class="span8">
                            <div class="whetherTitle">Weather<br/>
                                <span><?php print $waetherwidget['currentdaycondition']['temperaturefarenhit']; ?><sup>o</sup>F <?php print $waetherwidget['currentdaycondition']['currentwaether']; ?>
                                    <div class="seefullmargin"><a href="javascript:void(0)" class="clickMapLink" id="seefullforcastmobilePopup">SEE FULL FORECAST</a></div></span>
                            </div>
                        </div>
                    </div>
                    <div class="linedotted"></div>
                </div>
                <div class="span3 whetherhighplace">
                    <div class="whetherTitle">High<br/>
                        <span><?php print $waetherwidget['currentdayhigh']->fahrenheit; ?><sup>o</sup>F <?php print $waetherwidget['currentdaycondition']['currentwaether']; ?></span>
                    </div>
                </div>
                <div class="span3 whetherhighplace">
                    <div class="whetherTitle">Low<br/>
                        <span><?php print $waetherwidget['currentdaylow']->fahrenheit; ?><sup>o</sup>F <?php print $waetherwidget['currentdaycondition']['currentwaether']; ?></span>
                    </div>
                </div>
            </div>
        </div>
    </div>


</div>


<!-- Route Map Modal PopUp starts from here -->
<div class="modal hide" tabindex="-1" role="dialog" aria-hidden="true" id="parkingDirectionsModal">
    <div class="reoutemapDirectionsbox">
        <div class="reoutemapDirectionsboxInner">

            <div class="row-fluid">
                <div class="span3 reoutemapDirectionsboxInnerleft">
                    <div class="row-fluid" id="routeMaplinesholder">
                        <div class="span5 "><div class="line"></div></div>
                        <div class="span2 image-align" align="center"><img src="<?php print base_path() . path_to_theme(); ?>/images/banner-icon-img.png" /></div>
                        <div class="span5 "><div class="line"></div></div>
                    </div>
                    <div class="row-fluid">
                        <div class="span12"><div class="sliderCaption">Parking Directions</div></div>
                    </div>
                    <div class="row-fluid">
                        <div class="span12">
                            <div id="parkingDirectionsPorts">
                                <?php print $loadporttitle; ?> 
                            </div>
                        </div>
                    </div>
                </div>
                <div class="span9 reoutemapDirectionsboxInnerright">
                    <!--close button for modal popup-->
                    <div class="reoutemapDirectionsclosebutton">
                        <div class="reoutemapDirectionsclosebuttoninner">Close</div>
                    </div>
                    <!-- end of close button for modal popup-->
                    <div class="row-fluid">
                        <div class="span12 routemapContent" id="portContentRender">
                            <?php print $loadporttitle; ?> <br/>
                            <?php print $loadportdirection; ?>  
                        </div>
                    </div>
                </div>
            </div>



        </div><!-- reoutemapDirectionsboxInner end -->
    </div>
</div>
<!-- Route Map Modal PopUp ends here -->




<!-- See Route Map Modal PopUp starts from here -->
<div class="modal hide" tabindex="-1" role="dialog" aria-hidden="true"
     id="seeparkingDirectionsModal">
    <div class="modal-body reoutemapDirectionsbox " >
        <!--close button for modal popup-->
        <div class="seereoutemapDirectionsclosebutton">
            <div class="seereoutemapDirectionsclosebuttoninner">Close</div>
        </div>
        <!-- end of close button for modal popup-->
        <?php print getdirections_direction($loadportaddress); ?>
    </div>
</div>
<!-- See Route Map Modal PopUp ends here -->





<!-- Route Map Modal PopUp starts from here -->
<div class="modal hide" tabindex="-1" role="dialog" aria-hidden="true" id="directionsModal">
    <div class="reoutemapDirectionsbox">
        <div class="reoutemapDirectionsboxInner">

            <div class="row-fluid">
               
                <div class="span12 reoutemapDirectionsboxInnerright">
                    <!--close button for modal popup-->
                    <div class="reoutemapDirectionsclosebutton">
                        <div class="reoutemapDirectionsclosebuttoninner">Close</div>
                    </div>
                    <!-- end of close button for modal popup-->

                    <div class="row-fluid">
                        <div class="span12 routemapContent" id="directionsContentRender">
                            <div class="row-fluid" id="directionsContentHeader">
                                <div class="span4 sunsetLinePadding"><div class="line"></div></div>
                                <div class="span4"><div class="sliderCaption">Directions</div></div>
                                <div class="span4 sunsetLinePadding"><div class="line"></div></div>
                            </div>

                            <?php print $directions ?>
                        </div>
                    </div>
                </div>
            </div>



        </div><!-- sunsetDownsliderInner end -->
    </div>
</div>
<!-- Route Map Modal PopUp ends here -->






<!-- SEE FULL FORCAST Modal PopUp starts from here -->
<div class="modal hide" tabindex="-1" role="dialog" aria-hidden="true" id="fullforcastModal">
    <div class="modal-body">
        <!--close button for modal popup-->
        <div class="fullforcastclosebutton">
            <div class="fullforcastclosebuttoninner">Close</div>
        </div>
        <!-- end of close button for modal popup-->

        <h1> FULL FORECAST </h1>

        <div class="row-fluid">
            <div class="span12">
                <div>

                    <?php
                    
                    //var_dump("satya".$porttid); exit();
                    $weatherforecast = getExtendForcastview($porttid);
                    ?>



                    <div class="tabbable">
                        <ul class="nav nav-tabs">
                            <li class="active"><a href="#pane1" data-toggle="tab">Current Data</a></li>
                            <li><a href="#pane2" data-toggle="tab">Extended Forecast</a></li>

                        </ul>
                        <div class="tab-content">
                            <div id="pane1" class="tab-pane active">
                                <!--current day forcast-->
                                <?php print $weatherforecast['dayfullforecast']; ?>

                            </div>
                            <div id="pane2" class="tab-pane">
                                <!--days for cast-->
                                <?php print $weatherforecast['extendedforcast']; ?>

                            </div>
                        </div><!-- /.tab-content -->
                    </div><!-- /.tabbable -->




                </div>
            </div>

        </div>


    </div>
</div>
<!-- SEE FULL FORCAST Modal PopUp ends here -->



