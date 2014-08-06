<?php
/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
?>

<script defer src="<?php echo base_path() . path_to_theme() ?>/js/jquery.bxslider.js"></script>
<link rel="stylesheet" href="<?php echo base_path() . path_to_theme() ?>/css/jquery.bxslider.css" type="text/css" />

<script>

  $(function() {

    $('.connectOverviewGallery').bxSlider({
      infiniteLoop: false,
      hideControlOnEnd: true,
      minSlides: 1,
      maxSlides: 5,
      slideWidth: 300,
      slideHeight: 300,
      slideMargin: 10,
      touchEnabled: true
//      pager: false
    });

    $('#facebookWidget').carousel({
      interval: 8000
    });

    $('#twitterYourWidget').carousel({
      interval: 8000
    });

    $('#twitterOurWidget').carousel({
      interval: 8000
    });

    $('.responsiveMobile select').change(function() {
      window.location.href = '<?php echo base_path() ?>' + $(this).val();
    });
    // To do active Tab
    $(".responsiveWeb li").removeClass("active");
    $('.connect').addClass("active");

    $('.connectOverviewGallery li .sliderBox').mouseover(function(e) {
      $(this).children(".sliderBoxout").hide();
      $(this).children(".sliderBoxover").show();
    });

    $('.connectOverviewGallery li .sliderBox').mouseout(function(e) {
      $(this).children(".sliderBoxout").show();
      $(this).children(".sliderBoxover").hide();
    });


  });

</script>



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
      <div class="span6 overviewRightSide">
        <!-- contenttext_widget start here -->
        <div class="contenttext-widget">
          <span>Let's Get Social</span>
          <br>
         <?php print $socialcontent; ?>
          <!--Fecho park iphone godard retro, ugh id neutra fingerstache occaecat. Exercitation flannel dolor, echo park portland messenger bag deserunt eu selfies beard plaid skateboard.-->

        </div>
        <!-- contenttext_widget end here -->
      </div>
      <div class="span6 overviewLeftSide">
        <!-- socialicons_widget start here -->
        <div class="row-fluid">
          <div class="span6">
            <!-- content_facebook_widget start here -->
            <div class="content-facebook-widget">

              <a href="https://facebook.com/waterwayscruises" target="_blank"> <img src="<?php echo base_path(); ?>sites/all/themes/waterways/assets/img/facebook-content-icon.png" class="social-img"/></a>
              <span>
                <div class="widget-title"> <a href="https://facebook.com/waterwayscruises" target="_blank">Facebook</a></div> 
                <?php print $facebookcontent; ?>
<!--                Echo park iphone <br>
                godard retro, ugh-->
              </span>

            </div>
            <!-- content_facebook_widget end here -->
          </div>
          <div class="span6 blogMargin">
            <!-- content_twitter_widget start here -->
            <div class="content-twitter-widget">

              <a href="https://twitter.com/waterwayscruise" target="_blank"> <img src="<?php echo base_path(); ?>sites/all/themes/waterways/assets/img/twitter-content-icon.png" class="social-img"/></a>
              <span>
                <div class="widget-title"> <a href="https://twitter.com/waterwayscruise" target="_blank">Twitter<br>
                    @waterwayscruise </a></div>
                   <?php print $twittercontent; ?>
<!--                Echo park iphone <br>
                godard retro, ugh-->
              </span>

            </div>
            <!-- content_twitter_widget end here -->
          </div>
        </div><!-- row-fluid end -->
        <div class="row-fluid">
          <div class="span6">
            <!-- content_pintrest_widget start here -->
            <div class="content-pintrest-widget">

              <a href="http://pinterest.com/seattlecruises/" target="_blank"> <img src="<?php echo base_path(); ?>sites/all/themes/waterways/assets/img/pintrest-content-icon.png" class="social-img"/></a>
              <span>
                <div class="widget-title"> <a href="http://pinterest.com/seattlecruises/" target="_blank">Pinterest</a></div>
                <?php print $pinterestcontent; ?>
<!--                Echo park iphone <br>
                godard retro, ugh-->
              </span>

            </div>
            <!-- content_pintrest_widget end here -->
          </div>
          <div class="span6 blogMargin">
            <!-- content_youtube_widget start here -->
            <div class="content-youtube-widget">

              <a href="http://youtube.com/user/waterwayscruises" target="_blank">  <img src="<?php echo base_path(); ?>sites/all/themes/waterways/assets/img/youtube-content-icon.png" class="social-img"/></a>
              <span>
                <div class="widget-title"> <a href="http://youtube.com/user/waterwayscruises" target="_blank">Youtube</a></div>
                   <?php print $youtubecontent; ?>
<!--                Echo park iphone <br>
                godard retro, ugh-->
              </span>

            </div>
            <!-- content_youtube_widget end here -->
          </div>
        </div><!-- row-fluid end -->
        <!-- socialicons_widget end here -->
      </div>
    </div>
    <!-- row-fluid end -->

  </div>
</div> <!-- container end here -->

<div class="row-fluid galleryHolder">
  <div class="span12">
    <ul class="connectOverviewGallery">
      <?php print $instagramloadmedia;?>

    </ul>
  </div>
</div>



<div class="row-fluid socialiconblockview">
  <div class="span3 mobileFeeds">   
    <div class="socialiconblock">
      <img src="<?php echo base_path() . path_to_theme() ?>/images/facebook-content-icon.png" width="22" height="22" />
      <span class="socialWidgetTitle">facebook</span>


      <!--facebook feeds-->

      <div id="facebookWidget" class="carousel slide">
        <div class="carousel-inner">
          <!--    <div class="item active"><a href="#">facebookLink1</a></div>
                                          <div class="item"><a href="#">facebookLink2</a></div>
                                          <div class="item"><a href="#">facebookLink3</a></div>-->
          <?php print $facebookfeeds; ?>
        </div>
        <ol class="carousel-indicators">
          <li data-target="#facebookWidget" data-slide-to="0" class="active"></li>
          <li data-target="#facebookWidget" data-slide-to="1"></li>
          <li data-target="#facebookWidget" data-slide-to="2"></li>
        </ol>
      </div>

      <!--end of facebooks feeds-->
    </div>
  </div>
  <div class="span3 mobileFeeds">   
    <div class="socialiconblock">
      <img src="<?php echo base_path() . path_to_theme() ?>/images/twitter-content-icon.png" width="22" height="22" />
      <span class="socialWidgetTitle">your tweets</span>
      <!--Twitter feeds-->

      <div id="twitterYourWidget" class="carousel slide">
        <div class="carousel-inner">
          <?php print $yourtwitterfeeds; ?>
        </div>
        <ol class="carousel-indicators">
          <li data-target="#twitterYourWidget" data-slide-to="0" class="active"></li>
          <li data-target="#twitterYourWidget" data-slide-to="1"></li>
          <li data-target="#twitterYourWidget" data-slide-to="2"></li>
        </ol>
      </div>
      <!--twitter ends--> 
    </div>
  </div>
  <div class="span3 mobileFeeds">
    <div class="socialiconblock">
      <img src="<?php echo base_path() . path_to_theme() ?>/images/twitter-content-icon.png"/>
      <span class="socialWidgetTitle">our tweets</span>
      <div id="twitterOurWidget" class="carousel slide">
        <div class="carousel-inner">
          <?php print $ourtwitterfeeds; ?>
        </div>
        <ol class="carousel-indicators">
          <li data-target="#twitterOurWidget" data-slide-to="0" class="active"></li>
          <li data-target="#twitterOurWidget" data-slide-to="1"></li>
          <li data-target="#twitterOurWidget" data-slide-to="2"></li>
        </ol>
      </div>
    </div>
  </div>
  <div class="span3 mobileFeeds">
    <div class="socialiconblock">
        
      <div class="lastvideo"> <?php print $youtubevideo; ?></div>
    </div>

  </div>
</div>

