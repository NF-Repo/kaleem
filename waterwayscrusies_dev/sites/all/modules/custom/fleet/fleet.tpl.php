<?php
/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
//print base_path();
//print drupal_get_path('module', 'cruisesdetailview');
//var_dump($nodeid);
?>
<html>
    <head>
        <!--<title>Panzoom for jQuery</title>-->
        <meta http-equiv="Content-Type" content="text/html; charset=UTF-8" />
    <!--    <style type="text/css">
          body { background: #F5FCFF; color: #333666; }
          section { text-align: center; margin: 50px 0; }
          .panzoom-parent { border: 2px solid #333; }
          .panzoom-parent .panzoom { border: 2px dashed #666; }
          .buttons { margin: 40px 0 0; }
        </style>-->

    <!--    <script src="../test/libs/jquery.mousewheel.js"></script>-->
    </head>
    <body>
        <!--<script type="text/javascript" src="<?php // echo base_path() . path_to_theme() ?>/js/sharebuttons.js"></script>-->
        <script type="text/javascript" src="<?php echo base_path() . path_to_theme() ?>/js/buttons.js"></script>

<!--<script defer src="<?php echo base_path() . path_to_theme() ?>/js/jquery-panzoom.js"></script>-->
        <script defer src="<?php echo base_path() . path_to_theme() ?>/js/panzoom.js"></script>

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

//            stLight.options({
//                publisher: 'dr-52e8796a-b9e2-6a17-d82d-251b74277418',
//                tracking: 'google',
//                embeds: 'true',
//                onhover: false
//            });
  /*  required for sharethis multiple contents  starts  */
            var switchTo5x=true;
            stLight.options({
                publisher: 'dr-52e8796a-b9e2-6a17-d82d-251b74277418',
                 doNotHash: false, 
                 doNotCopy: false,
                 hashAddressBar: false
            });
//            stLight.options({publisher: "dr-52e8796a-b9e2-6a17-d82d-251b74277418", doNotHash: false, doNotCopy: false, hashAddressBar: false});
           
           


//                var shareurl=document.URL;
//                var sharetitle=$('.fleetSubTitle').text().toUpperCase();
//                  var sharedesc=$('.fleetDescription').children('p').text();
//                  stWidget.addEntry({
//                    "service":"sharethis",
//                    "element":document.getElementById('sharethisbtn'),
//                    "url":shareurl,
//                    "title":sharetitle,
//                    "type":"large",
////                  "text":"ShareThis" ,
////                  "image":"http://icons.iconarchive.com/icons/iconshock/high-detail-social/256/sharethis-icon.png",
//                    "summary": sharedesc
//                });
                
                
                
                



            var dirbasepath = Drupal.settings.basePath;
            $(function() {










//                $("#shareBtn").on("click", function() {
//                    $("#stwrapper").show();
//                });

                // To do active Tab
                $(".responsiveWeb li").removeClass("active");
                $('.fleetmenu').addClass("active");


                $('.fleetbxslider').magnificPopup({
                    delegate: 'a',
                    closeMarkup: '<button title="Close (Esc)" type="button" class="mfp-close"><div class="mfp-close close-text">Close</div></button>',
                    // child items selector, by clicking on it popup will open
                    gallery: {
                        enabled: true
                    }
                    //  type:'image'

                    // other options
                });


                $('.fleetbxslider').bxSlider({
                    infiniteLoop: false,
                    hideControlOnEnd: true,
                    minSlides: 1,
                    maxSlides: 5,
                    slideWidth: 300,
                    slideHeight: 250,
                    slideMargin: 10,
                    touchEnabled: true
                });

                $("#portContentRender div:first-child").addClass("active");

                $("#routmapSlider div:first-child").addClass("active");
                $("#virualtourlist li:first-child").addClass("active");

                $('.responsiveMobile select').change(function() {

                    window.location.href = '<?php echo base_path() ?>fleet/' + $(this).val();
                });

                $("#virualtourlist li").click(function() {
                    
                    $("#virualtourlist li").removeClass("active");
                    $(".panImage").removeClass("active");
//                    $(".panImage").removeClass("panzoom");
                    $(this).addClass("active");
//                    alert(this);
//                    $(this).addClass("panzoom");
                    var id = $(this).attr("id");
                    $("." + id + "Image").addClass("active");
                    $(".panImage.active").removeClass("panzoom");
                    $(".panImage." + id + "Image.active").addClass("panzoom");
                    
                    var $section = $('div');
        //          var $section = $('.active').first();
                    $section.find('.panzoom.active').panzoom({
//                        $zoomIn: $section.find(".zoom-in"),
//                        $zoomOut: $section.find(".zoom-out"),
//                        $zoomRange: $section.find(".zoom-range"),
//                        $reset: $section.find(".reset")
                        $zoomIn: $section.find(".zoom-in"),
                        $zoomOut: $section.find(".zoom-out"),
                        $zoomRange: $section.find(".zoom-range"),
                        $reset: $section.find(".reset"),
                        startTransform: 'scale(1.1)',
                        increment: 0.1,
                        minScale: 1,
                        contain: 'invert',
                        min_width: $(".panImage.active img").width(),
                        min_height: $(".panImage.active img").height()
                    });
                    $( ".reset" ).trigger( "click" );
        //            $('.panImage img').panZoom({'zoomIn': $('.plus'),
        //                'zoomOut': $('.minus'),
        //                'mousewheel': false,
        //                'draggable': false,
        //                min_width: $(".panImage").width(),
        //                min_height: $(".panImage").height()});
                });


                $(".fleetDownslider .carousel").on('slid', function(e) {
                    var index = $(this).find(".item.active").index() + 1;
//                    $(".accommodations").removeClass("active");
//                    $(".sliderInnercontent .accommodations:nth-child(" + index + ")").addClass("active");
                });


                $(".virtualtourInner").click(function() {

                    $('#virtualTourModal').modal('show');
                    var $section = $('div');
        //          var $section = $('.active').first();
                    $section.find('.panzoom.active').panzoom({
//                        $zoomIn: $section.find(".zoom-in"),
//                        $zoomOut: $section.find(".zoom-out"),
//                        $zoomRange: $section.find(".zoom-range"),
//                        $reset: $section.find(".reset")
                        $zoomIn: $section.find(".zoom-in"),
                        $zoomOut: $section.find(".zoom-out"),
                        $zoomRange: $section.find(".zoom-range"),
                        $reset: $section.find(".reset"),
                        startTransform: 'scale(1.1)',
                        increment: 0.1,
                        minScale: 1,
                        contain: 'invert',
                        min_width: $(".panImage.active img").width(),
                        min_height: $(".panImage.active img").height()
                    });
                    $( ".reset" ).trigger( "click" );
        //            $('.panImage img').panZoom({'zoomIn': $('.plus'),
        //                'zoomOut': $('.minus'),
        //                'mousewheel': false,
        //                'draggable': false,
        //                min_width: $(".panImage").width(),
        //                min_height: $(".panImage").height()});
                });
                $(".reoutemapDirectionsclosebuttoninner").click(function() {
                    $('#virtualTourModal').modal('hide');
                });


                /* Arrow scroll bottom script */
                $("#downArrow").click(function() {
                    $("html, body").animate({scrollTop: $("#routmapSlider").offset().top}, 800);
                });

                /* Arrow scroll top script */
                $("#upArrow").click(function() {
                    $("html, body").animate({scrollTop: $("#header").offset().top}, 800);
                });


                //carousel animation api code gets start here

        //         
                $('#mydownCarousel').carousel({
                    interval: 5000
                }).on('slide', function(e) {
        //        var xx = parseInt($('#mydownCarousel .active').index('#mydownCarousel .item'));
        //        console.log(xx);
                    var slideFrom = $('#mydownCarousel').find('.active').index();

                    var slideTo = $(e.relatedTarget).index();

                    var nextindicatorname = '#ind' + slideTo;
                    $(".fleetIndicators li div").removeClass("active");
                    $(nextindicatorname).addClass("active");
                });



                $(document).on('click', '.fleetindicator', function() {
                    var indicatorid = parseInt(this.id.slice(3));
                    $('#mydownCarousel').carousel('pause');
                    $('#mydownCarousel').carousel(indicatorid);
                });
                $("#fleetcaroselnext").click(function(e) {
                    $('#mydownCarousel').carousel('next');
                    $("#mydownCarousel").carousel("cycle");
                    e.preventDefault();
                });
                $("#fleetcaroselprev").click(function(e) {
                    $('#mydownCarousel').carousel('prev');
                    $("#mydownCarousel").carousel("cycle");
                    e.preventDefault();
                });
                //carosule animation api code gets ends here

                $(".booknow-inner").click(function() {
                    $('#mydownCarousel').carousel('pause');
                });





                /*Carousel Indicators gets starting*/
                $("#routmapSlider div:first-child").addClass("active");
                if ($("#routmapSlider").children().length > 1) {
                    $("#routmapSlider .item").each(function(index) {
                        if (index == 0) {

                            var indicators = '<li><div class="fleetindicator active" id="ind' + index + '"></div> </li>';
        //      var indicators='<li data-target="#mydownCarousel" data-slide-to="'+index+'" class="active"></li>';


                        } else {


                            var indicators = '<li><div class="fleetindicator" id="ind' + index + '"></div> </li>';
        //                  var indicators='<li data-target="#mydownCarousel" data-slide-to="'+index+'"></li>';
                        }
                        $(".fleetIndicators").append(indicators);
                    });
                }




                /*Carousel Indicators gets ending*/

            });
            
            function sharethisfn(shareval){
                var selfleet=$('#fleetsel').val();
//                var shtml='<span class="st_facebook_large pull-left facebook st_url="'+shareurl+'" st_title="'+sharetitle+'" style="width:30%" displayText="Facebook" id="fbsharebn"></span>\n\
//                    <span class="st_twitter_large pull-left twitter"  id="twitsharebn" style="width:30%" displayText="Tweet"></span>\n\
//                    <div class="share-text pull-left" onclick="sharethisfn();"> Share</div>';
//                $('#sharebtnsdiv').html(shtml);
                if(shareval == 'facebook'){
//                    $( "#fbsharebn" ).children('span').children('.stLarge').trigger( "click" );
                    $( "#fbsharebn"+selfleet ).children('span').trigger( "click" );
                }
                if(shareval == 'twitter'){
                    $( "#twitsharebn"+selfleet ).children('span').trigger( "click" );
                }
                if(shareval == 'share'){
                   $( "#sharethisbtn" ).trigger( "click" );
                }
                
//                 $("#stLframe").width('310px');
//                 $("#stLframe html body #outerContainer").css("width":"310px");
                 
//        $("#stLframe").height(326);
            }
        </script>






        <div id="content">
            <div class="contentDetail">
                <!-- sunset dining start here -->

                <div class="row-fluid cruiseDetailinfo fleetdetailview">
                    <div class="span6">
                        <div class="row-fluid">
                            <div class="span2">

                            </div>
                            <div class="span10">
                                <div class="fleetTitle">fleet</div>
                                <div class="subMenu responsiveWeb">
                                    <ul id="fleetItems">
                                        <?php print $webmenu; ?>
                                    </ul>
                                </div>
                                <div class="responsiveMobile subMenu">
                                    <?php print $mobilemenu ?>

                                </div>


                                <?php print $loadfleetcontent; ?>
                                <div class="row-fluid requset-proposal" align="center">

                                    <div class="span4 sunsetLinePadding left-line-margin">
                                        <div class="line"></div>
                                    </div>
                                    <div class="span4 requestProposalauto">
                                        <!-- booknow start here -->
                                        <div class="booknow ">
                                            <a href="#myModalrequest" role="button" data-toggle="modal">
                                                <div class="booknow-inner">Request a Proposal</div>
                                            </a>
                                        </div>
                                        <!-- booknow end here -->
                                    </div>
                                    <div class="span4 sunsetLinePadding right-line-margin">
                                        <div class="line"></div>
                                    </div>
                                </div>


                                <div class="virtualtour">
                                    <div class="virtualtourInner">
                                        <a href="javascript:void(0)">
                                            <img src="<?php echo base_path() . path_to_theme(); ?>/images/banner-icon-img.png">
                                            <br/>Click for the virtual tour
                                        </a>
                                    </div>
                                </div>

                                <div id="downArrow">
                                    <img src="<?php echo base_path() . path_to_theme(); ?>/images/sunset-banner-downarrow.png" />
                                </div>

                            </div>
                        </div><!-- row-fluid end -->

                    </div>
                    <div class="span6">

                        <div class="fleet-slider-buttons">
                            <!-- removed downarrow from here -->
                            <?php print $rightimage; ?> 
                            <!--<div class="booknow share-mobile responsiveMobile">-->
                            <div class="booknow share-mobile responsiveMobile">
<!--                                <div class="buttonsdiv">
                                    <span class='pull-left facebook fbsharebtnimg' onclick="sharethisfn('facebook');" style="width:30%" displayText='Facebook'></span>
                                    <span class='pull-left twitter twitsharebtnimg' onclick="sharethisfn('twitter');" style="width:30%" displayText='Tweet'></span>
                                    <div class="share-text pull-left" onclick="sharethisfn('share');"> Share</div>
                                    <span class='st_facebook_large pull-left facebook'  id="fbsharebn" st_url="http://www.google.com" st_title="Title to share" style="width:30%" displayText='Facebook'></span>
                                    <span class='st_twitter_large pull-left twitter' id="twitsharebn" style="width:30%" displayText='Tweet'></span>
                                    <div class="share-text pull-left" onclick="sharethisfn();"> Share</div>
                                    <span class='st_sharethis_large' displayText='ShareThis' id="sharethisbtn" style="display:none;"></span>
                                    
                                    
                                    <div id="sharebtnsdiv" style="display: none;">
                                    <span class='st_facebook_large pull-left facebook'  id="fbsharebn" st_url="http://www.google.com" st_title="Title to share" style="width:30%" displayText='Facebook'></span>
                                    <span class='st_twitter_large pull-left twitter' id="twitsharebn" style="width:30%" displayText='Tweet'></span>
                                    <div class="share-text pull-left" onclick="sharethisfn();"> Share</div>
                                    <span class='st_sharethis_large' displayText='ShareThis' id="sharethisbtn" style="display:none;"></span>
                                    </div>
                                    
                                    <span class='st_facebook_hcount pull-left facebook' style="width:30%" displayText='Facebook'></span>
                                    <span class='st_twitter_hcount pull-left twitter' style="width:30%" displayText='Tweet'></span>
                                    <div class="share-text pull-left" onclick="sharethisfn();"> Share</div>
                                    <span class='st_sharethis_hcount' displayText='ShareThis' id="sharethisbtn" style="display:none;"></span>
                                </div>-->



                                 <div class="buttonsdiv">
                                    <!--<span class='pull-left facebook fbsharebtnimg' onclick="sharethisfn('facebook');" style="width:30%" displayText='Facebook'></span>-->
                                    
                                    <div class="pull-left twitter">
                                        <a href="https://twitter.com/share" class="twitter-share-button" data-count="none">Tweet</a>
                                    </div>
                                    
                                    <script>!function(d,s,id){var js,fjs=d.getElementsByTagName(s)[0],p=/^http:/.test(d.location)?'http':'https';if(!d.getElementById(id)){js=d.createElement(s);js.id=id;js.src=p+'://platform.twitter.com/widgets.js';fjs.parentNode.insertBefore(js,fjs);}}(document, 'script', 'twitter-wjs');</script>

                                    <div class='pull-left pinterest'>
                                        <a href="http://www.pinterest.com/pin/create/button/
                                           ?url=http%3A%2F%2Fwww.flickr.com%2Fphotos%2Fkentbrew%2F6851755809%2F
                                           &media=http%3A%2F%2Ffarm8.staticflickr.com%2F7027%2F6851755809_df5b2051c9_z.jpg
                                           &description=Next%20stop%3A%20Pinterest"
                                           data-pin-do="buttonPin"
                                           data-pin-config="above">
                                            <img src="//assets.pinterest.com/images/pidgets/pin_it_button.png" />
                                        </a>
                                    </div>
                                    <script type="text/javascript" src="//assets.pinterest.com/js/pinit.js"></script>
                                    <script type="text/javascript">
                                    (function(d){
                                        var f = d.getElementsByTagName('SCRIPT')[0], p = d.createElement('SCRIPT');
                                        p.type = 'text/javascript';
                                        p.async = true;
                                        p.src = '//assets.pinterest.com/js/pinit.js';
                                        f.parentNode.insertBefore(p, f);
                                    }(document));
                                    </script>
                                    <div id="fb-root"></div>
                                    <script>(function(d, s, id) {
                                      var js, fjs = d.getElementsByTagName(s)[0];
                                      if (d.getElementById(id)) return;
                                      js = d.createElement(s); js.id = id;
                                      js.src = "//connect.facebook.net/en_US/all.js#xfbml=1&appId=384985111641504";
                                      fjs.parentNode.insertBefore(js, fjs);
                                    }(document, 'script', 'facebook-jssdk'));
                                    </script>
                                    <div class="fb-like pull-left facebook" data-href="https://developers.facebook.com/docs/plugins/" data-layout="button" data-action="like" data-show-faces="true" data-share="false"></div>
                                    <!--<span class='pull-left twitter twitsharebtnimg' onclick="sharethisfn('twitter');" style="width:30%" displayText='Tweet'></span>-->
                                    <div class="sharetext pull-left" onclick="sharethisfn('share');"> Share</div>
                                     <span class='st_sharethis_large' displayText='ShareThis' id="sharethisbtn" style="display:none;"></span>
                                </div>















<!--                                <span class='st_facebook_large' displayText='Facebook'></span>
                                <span class='st_twitter_large' displayText='Tweet'></span>
                                <div class="share-text"> Share</div>
                                <span class='st_sharethis_large' displayText='ShareThis' id="sharethisbtn" style="display:none;"></span>-->
<!--                                <a>
                                    <div class="booknow-inner"> 
                                        <div class="addthis_toolbox addthis_default_style ">
                                            <iframe src="//www.facebook.com/plugins/like.php?href=https%3A%2F%2Fwww.facebook.com%2Fpages%2FMyartmatch%2F206787809421622&amp;width=450&amp;height=35&amp;colorscheme=light&amp;layout=standard&amp;action=like&amp;show_faces=false&amp;send=false&amp;appId=1378753459016747" scrolling="no" frameborder="0" style="border:none; overflow:hidden; width:50px; height: 29px;float: left;margin-left: 7%;margin-top: 1%;" allowTransparency="true"></iframe>
                                            <div class="twitter-share">
                                                <a href="https://twitter.com/share" class="twitter-share-button" data-url="<?php echo base_path(); ?>/eventscruise/<?php print $cruiseid; ?>" data-via="lsnwaterways">Tweet</a>
                                                <script>!function(d, s, id) {
                                            var js, fjs = d.getElementsByTagName(s)[0], p = /^http:/.test(d.location) ? 'http' : 'https';
                                            if (!d.getElementById(id)) {
                                                js = d.createElement(s);
                                                js.id = id;
                                                js.src = p + '://platform.twitter.com/widgets.js';
                                                fjs.parentNode.insertBefore(js, fjs);
                                            }
                                        }(document, 'script', 'twitter-wjs');</script>
                                            </div>
                                            <div class="share-text"> Share</div>
                                        </div>

                                    </div>
                                </a>-->
                            </div>
                        </div>


                    </div>
                </div>

            </div>
            <!-- sunset down slider start here 
            <div class="row-fluid">
                <div class="span12">
            <!-- sunsetDownslider start here -->
            <div class="fleetDownslider">                
                <div id="mydownCarousel" class="carousel slide">
                    <!--                                <ol class="carousel-indicators">
                                                                </ol>-->

                    <ul class="fleetIndicators"></ul>   
                    <!-- Carousel items -->
                    <div class="carousel-inner" id="routmapSlider">
                        <?php print $loadslider; ?>
                    </div>
                    <!-- Carousel nav -->

                    <a class="carousel-control left" href="javascript:void(0);" id="fleetcaroselprev"><img src="<?php echo base_path() . path_to_theme(); ?>/images/carousol-leftarrow.png" /></a>
                    <a class="carousel-control right" href="javascript:void(0);" id="fleetcaroselnext"><img src="<?php echo base_path() . path_to_theme(); ?>/images/carousol-rightarrow.png" /></a>



<!--                    <a class="carousel-control left" href="#mydownCarousel" data-slide="prev"><img src="<?php echo base_path() . path_to_theme(); ?>/images/carousol-leftarrow.png" /></a>
<a class="carousel-control right" href="#mydownCarousel" data-slide="next"><img src="<?php echo base_path() . path_to_theme(); ?>/images/carousol-rightarrow.png" /></a>
                    -->



                </div>


                <div class="fleetDownsliderbox">
                    <div class="fleetDownsliderInner">
                        <div class="row-fluid" align="center">

                            <div class="span2 left-line-margin sunsetLinePadding"><div class="line"></div></div>
                            <div class="span8"><div class="sliderCaption">Seating Accommodations</div></div>
                            <div class="span2 sunsetLinePadding"><div class="line"></div></div>
                        </div>

                        <div class="sliderInnercontent">
                            <?php print $loadaccommodations ?>
                        </div>

                        <div class="row-fluid" id="routeMaplinesholder">
                            <div class="span5 "><div class="line"></div></div>
                            <div class="span2" align="center"><img src="<?php echo base_path() . path_to_theme(); ?>/images/banner-icon-img.png"/></div>
                            <div class="span5 "><div class="line"></div></div>
                        </div>
                    </div><!-- sunsetDownsliderInner end -->
                </div>
            </div>
            <!-- sunsetDownslider end here -->
        </div>
    </div>
    <!-- sunset down slider end here -->

    <div class="row-fluid fleetDownslidermargin">
        <ul class="fleetbxslider">
            <?php print $loadgallery ?>
        </ul>
    </div><!-- row-fluid end -->

    <div class="row-fluid">
        <div class="span12">
            <div id="upArrow">
                <img src="<?php echo base_path() . path_to_theme(); ?>/images/sunset-banner-uparrow.png" />
            </div>
        </div>
    </div>

</div>

<!-- Route Map Modal PopUp starts from here -->
<div class="modal hide" tabindex="-1" role="dialog" aria-hidden="true" id="virtualTourModal">
    <div class="reoutemapDirectionsbox">
        <div class="reoutemapDirectionsboxInner">

            <div class="row-fluid">
                <div class="span3 reoutemapDirectionsboxInnerleft">
                    <div class="row-fluid" id="routeMaplinesholder">
                        <div class="span5 "><div class="line"></div></div>                       
                        <div class="span2" align="center"><img src="<?php echo base_path() . path_to_theme(); ?>/images/banner-icon-img.png" /></div>
                        <div class="span5 "><div class="line"></div></div>
                    </div>
                    <div class="row-fluid">
                        <div class="span12"><div class="sliderCaption"><?php print $loadtitle ?></div></div>
                    </div>
                    <div class="row-fluid">
                        <div class="span12 panText">Click and drag on the picture to pan the view.</div>
                    </div>
                    <div class="row-fluid">
                        <div class="span12 panText">Click on doorways to move to other rooms.</div>
                    </div>
                    <div class="row-fluid">
                        <div class="span12">
                            <br>
                            <ul id="virualtourlist">
                                <?php print $loadvirualtour; ?>
                            </ul>
                        </div>
                    </div>
                </div>
                <div class="span9 reoutemapDirectionsboxInnerright">
                    <!--close button for modal popup-->
                    <div class="reoutemapDirectionsclosebutton">
                        <div class="reoutemapDirectionsclosebuttoninner">Close Tour</div>
                    </div>
                    <div class="buttons" style="padding:10px;">
                        <button class="zoom-in panzoombtn"><div class="panzoombtnminus">+</div></button>
                        <button class="zoom-out panzoombtn"><div class="panzoombtnminus">-</div></button>
<!--                        <button class="zoom-in panzoombtn"><img style="width:18px;" src="<?php echo base_path() . path_to_theme(); ?>/images/zoomin.png"></button>
                        <button class="zoom-out panzoombtn"><img style="width:20px;" src="<?php echo base_path() . path_to_theme(); ?>/images/zoomout.png"></button>-->
                        <input type="range" class="zoom-range">
                        <button class="reset panzoomresetbtn">Reset</button>
                    </div>  
                    <!--                    <div class="plusholder">
                                            <div class="plus">&#43;</div>
                                        </div>
                                        <div class="minusholder">
                                            <div class="minus">&#45;</div>
                                        </div>-->
                    <!-- end of close button for modal popup-->
                    <div class="row-fluid">
                        <div class="span12 routemapContent" id="portContentRender">
                            <?php print $loadtourimages; ?>
                        </div>
                    </div>
                </div>
            </div>



        </div><!-- sunsetDownsliderInner end -->
    </div>
</div>
<!-- Route Map Modal PopUp ends here -->



</body>
</html>