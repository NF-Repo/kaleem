<?php
/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
?>

<!--<script type="text/javascript" src="<?php echo base_path() . path_to_theme() ?>/js/sharebuttons.js"></script>-->
<script defer src="<?php echo base_path() . path_to_theme() ?>/js/jquery.bxslider.js"></script>
<link rel="stylesheet" href="<?php echo base_path() . path_to_theme() ?>/css/jquery.bxslider.css" type="text/css" />



<script type="text/javascript" src="<?php echo base_path() . path_to_theme() ?>/js/buttons.js"></script>	
<script>
    stLight.options({
        publisher: 'dr-52e8796a-b9e2-6a17-d82d-251b74277418',
        tracking: 'google',
        embeds: 'true',
        onhover: false
    });
    
    
    
    /*  required for sharethis multiple contents  starts  */
    var switchTo5x=true;
    
    function addentryforshare(totalshares){
              var sharedesc='';
              var sharetitle='';
              for(var i=0;i<totalshares;i++){
                  var urlid='#sharethis'+i+'url';
                  var titleid='#sharethis'+i+'title';
                  var descid='#sharethis'+i+'desc';
                  var buttonid='sharethis'+i+'btn';
                  
                  var shareurl=$(urlid).val();
                  var sharetitle=$(titleid).val().toUpperCase();
                  var sharedesc=$(descid).children('p').text();
                  stWidget.addEntry({
                    "service":"sharethis",
                    "element":document.getElementById(buttonid),
                    "url":shareurl,
                    "title":sharetitle,
                    "type":"large",
//                  "text":"ShareThis" ,
//                  "image":"http://icons.iconarchive.com/icons/iconshock/high-detail-social/256/sharethis-icon.png",
                    "summary": sharedesc
                });
              }
		
        }
     /*  required for sharethis multiple contents ends    */   
     
     
        
    $(function() {
        
        $('.connectOverviewGallery').bxSlider({
            infiniteLoop: false,
            hideControlOnEnd: true,
            minSlides: 1,
            maxSlides: 5,
            slideWidth: 300,
            slideHeight: 300,
            slideMargin: 10,
            touchEnabled: false,
            pager: false
        });
$(".more-reviews-inner").click(function(){
    $(".more-reviews-list").show();
    $(".more-reviews-content").hide();
});
        
        
        /*  required for sharethis multiple contents starts   */
        $('.sharethisbt').click(function(){
            $( "#sharethis0btn").trigger( "click" );
            $( "#"+this.id+'n' ).trigger( "click" );
        });
        /*  required for sharethis multiple contents ends   */
        
   if($(".more-reviews-list").length==0){
            $(".more-reviews-content").hide();
        }

        $('#facebookWidget').carousel({
            interval: 8000
        });

        $('#twitterYourWidget').carousel({
            interval: 8000
        });

        $('#twitterOurWidget').carousel({
            interval: 8000
        });
        $('.reviewbox-list li .sliderBox').mouseover(function(e) {
            $(this).children(".sliderBoxout").hide();
            $(this).children(".sliderBoxover").show();
        });
        $('.reviewbox-list li .sliderBox').mouseout(function(e) {
            $(this).children(".sliderBoxover").hide();
            $(this).children(".sliderBoxout").show();
        });
        $('.responsiveMobile select').change(function() {
            window.location.href='<?php echo base_path() ?>' + $(this).val();
        });
//        $(".shareBtn").on("click", function(e){
//               var sharebtn='#'+this.id+'desc';
//               console.log(sharebtn);
//               var textcontent=$(sharebtn).children('p').text();
//               $('#sharecontent').text(textcontent);
//               console.log(textcontent);
//            $("#stwrapper").show();
//        });        
        
        // To do active Tab
        $(".responsiveWeb li").removeClass("active");
        $('.connect').addClass("active");
        
        $('.reviewbox-list li').click(function(e) {
            if($(e.currentTarget).attr("id")=="tip"){
                window.open("http://www.tripadvisor.com/Attraction_Review-g60878-d1130187-Reviews-Waterways_Cruises-Seattle_Washington.html");
            }else{
                window.open("http://www.yelp.com/biz/waterways-cruises-seattle");
            }
            
        });
    });

</script>


<p style="display:none;" id="sharecontent"></p>
<div class="container">
    <div class="connectMain">
        <!--connect title and sub menu start here--> 
        <div class="row-fluid">
            <div class="span12">
                <div class="connectTitle"> CONNECT </div>

                <div class="subMenu responsiveWeb">
                    <ul>
                        <?php print $menu; ?>
                    </ul>
                </div>
                <div class="subMenu responsiveMobile">

                    <select>
                        <?php print $mobilemenu; ?>
                    </select>


                </div>

            </div>
        </div>
        <!--connect title and sub menu end here--> 

        <div class="row-fluid overviewMain">

            <div class="span4">
                <ul class="reviewbox-list">
                    <li id="tip"><div class="sliderBox"><div class="sliderBoxout"><div class="sliderBoxInner"><img src="<?php echo base_path(); ?>sites/all/themes/waterways/assets/img/innerbox-top-img.png"><br><div class="title">WRITE YOUR<br><span>Review</span><br><img src="<?php echo base_path() . path_to_theme(); ?>/images/tripadvisor-logo.png"></div><div class="line"></div></div></div><div class="sliderBoxover" style="display: none;"><div class="sliderBox-first-inner"><img src="<?php echo base_path(); ?>sites/all/themes/waterways/assets/img/innerbox-top-img.png"><br><br><?php print $tripadvisordescription; ?><br><a>Click to Learn More</a><div class="line"></div></div></div></div></li>
                    <li id="yelp"><div class="sliderBox"><div class="sliderBoxout"><div class="sliderBoxInner"><img src="<?php echo base_path(); ?>sites/all/themes/waterways/assets/img/innerbox-top-img.png"><br><div class="title">WRITE YOUR<br><span>Review</span><br><img src="<?php echo base_path() . path_to_theme(); ?>/images/yelp-logo.png"></div><div class="line"></div></div></div><div class="sliderBoxover" style="display: none;"><div class="sliderBox-first-inner"><img src="<?php echo base_path(); ?>sites/all/themes/waterways/assets/img/innerbox-top-img.png"><br><br><?php print $yelpdescription; ?><br><a>Click to Learn More</a><div class="line"></div></div></div></div></li>
                </ul>
            </div>
            <div class="span8">
                <ul class="reviews-list">
                      <?php print $reviewContent; ?>
                </ul>

                <div class="more-reviews-content">
                    <div class="more-reviews">
                        <div class="more-reviews-inner">
                            <img src="<?php echo base_path(); ?>sites/all/modules/custom/fleet/images/banner-icon-img.png">
                            <br><a>CLICK FOR MORE REVIEWS</a>
                        </div>
                    </div>
                </div>

            </div>

        </div>
        <!-- row-fluid end -->

    </div>
</div> <!-- container end here -->


