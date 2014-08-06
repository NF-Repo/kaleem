<?php
/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
?>

<script src="//ajax.googleapis.com/ajax/libs/jquery/1.8.1/jquery.min.js"></script>

<script>
  $(document).ready(function() {
  $.getJSON(Drupal.settings.basePath+'rest/content?type=homepageslider', function(data) {
      $.each(data, function(i, item) {
// console.log(item);
         if (i != 0)
        {
          if (i == 1)
          {
            loadContentData(data[i].nid, 1);
          }
          else
          {
            loadContentData(data[i].nid, 0);
          }
        }
      });
    });

  });

  function loadContentData(nid, firstitem)
  {
    console.log('loaddata');
    $.getJSON(Drupal.settings.basePath+'rest/content/' + nid, function(nodedata) {
      console.log(nodedata.fields);
      console.log(nodedata.title);
      console.log(nodedata.fields.field_imagefile[0].uri);
      console.log(nodedata.fields.field_button_text[0].value);
      console.log(nodedata.fields.field_headding[0].value);

      var title = nodedata.title;
      var imageuri = nodedata.fields.field_imagefile[0].uri.replace("public://", Drupal.settings.basePath + 'sites/default/files/');
      var buttontext = nodedata.fields.field_button_text[0].value;
      var headingtext = nodedata.fields.field_headding[0].value;

      //image adding script

      if (firstitem == 1)
      {
        var html = '';
        var sliderhtml = '<div class="active item"><img src="' + imageuri + '"></div>';
        
        //image adding scripts ends
      }
      else
      {
        var sliderhtml = '<div class="item"><img src="' + imageuri + '"></div>';
      }
      $("#slidersitems").append(sliderhtml);
    });
  }

</script>
<div class=""> 
  <!-- removed container class -->
  <a name="top"></a>

  <!--  <div id="myCarousel" class="carousel slide">
      <ol class="carousel-indicators">
        <li data-target="#myCarousel" data-slide-to="0" class="active"></li>
        <li data-target="#myCarousel" data-slide-to="1"></li>
      </ol>
       Carousel items 
  
      <div class="carousel-inner" id="slidersitems"></div>
  
       Carousel nav 
      <a class="carousel-control left" href="#myCarousel" data-slide="prev">‹</a>
      <a class="carousel-control right" href="#myCarousel" data-slide="next">›</a>
    </div>-->

  <!-- slider start here -->
  <div id="myCarousel" class="carousel slide">
    <div id="slider">
      <div class="slider">

        <ol class="carousel-indicators">
          <li data-target="#myCarousel" data-slide-to="0" class="active"></li>
          <li data-target="#myCarousel" data-slide-to="1"></li>
        </ol>

        <div class="carousel-inner" id="slidersitems"></div>

        <!-- Carousel nav -->
        <a class="carousel-control left" href="#myCarousel" data-slide="prev">‹</a>
        <a class="carousel-control right" href="#myCarousel" data-slide="next">›</a>
            
        <!--slider content starts-->
        <div class="row-fluid sliderContent">
          <div class="span12">
            <!-- slider top caption start here -->
            <div class="row-fluid">
              <div class="span8">
                <!-- slider caption start here -->
                <div class="slider-caption">

                  <div class="row-fluid">
                    <div class="span2">
                      <div class="line"></div>
                    </div>
                    <div class="span8">
                      <div class="welcometext">Welcome to</div> 
                    </div>
                    <div class="span2">
                      <div class="line"></div>
                    </div>
                  </div>
                  <div class="row-fluid">
                    <div class="span12">
                      <div class="logotext">
                        WATERWAYS
                        CRUISES
                      </div>
                    </div>
                  </div>
                  <div class="row-fluid">
                    <div class="span4 line-padding">
                      <div class="line"></div>
                    </div>
                    <div class="span4">
                      <!-- view-featured-button start here -->
                      <div class="view-featured-button">
                        <a href="#">
                          <div class="view-featured-button-inner">
                            View Featured Cruises
                          </div>
                        </a>
                      </div>
                      <!-- view-featured-button end here -->
                    </div>
                    <div class="span4 line-padding">
                      <div class="line"></div>
                    </div>
                  </div>

                </div>
              </div>
              <div class="span4">
              </div>
              <!-- slider caption end here -->
            </div>

          </div>
          <!-- slider caption end here -->
          <!-- slider bottom links start here -->
          <div class="row-fluid">
            <div class="span12">
              <div class="slider-bottom">
                <div class="slider-bottomlinks">

                  <div class="book-dining">
                    <a href="#">
                      <img src="<?php echo base_path(); ?>sites/all/themes/waterways/assets/img/banner-icon-img.png" /><br>
                      BOOK A DINING CRUISE
                    </a>
                  </div>
                  <div class="request-proposal">
                    <a href="#myModal" role="button" data-toggle="modal">
                      <img src="<?php echo base_path(); ?>sites/all/themes/waterways/assets/img/banner-icon-img.png" /><br>
                      REQUEST A PROPOSAL
                    </a>
                  </div>
                </div>
              </div>
            </div>
          </div>
          <!-- slider bottom links end here -->
        </div>
        <!--end of slider content starts-->
      </div>
    </div>
  </div>
  <!-- slider end here -->




</div> <!-- /container -->
<div class="contentbox-inner">
  <div class="row-fluid">
    <div class="span4" align="center">
      <!-- contentbox_first start here -->
      <div class="contentbox-first">
        <div class="contentbox-first-inner">
          <img src="<?php echo base_path(); ?>sites/all/themes/waterways/assets/img/innerbox-top-img.png" />
          <br>
          <br>
          Beard in lo-fi, raw denim ea <br>
          fugiat organic consectetur. <br>
          Gentrify duis proident.<br>
          <a href="#">Click to Learn More</a>
          <div class="line"></div>
        </div>
      </div>
      <!-- contentbox_first end here -->
    </div>
    <div class="span4" align="center">
      <!-- contentbox_second start here -->
      <div class="contentbox-second">

        <div class="contentbox-second-inner">
          <img src="<?php echo base_path(); ?>sites/all/themes/waterways/assets/img/innerbox-top-img.png" />
          REDEEM YOUR <br>
          <span>Voucher</span> <br>
          <img src="<?php echo base_path(); ?>sites/all/themes/waterways/assets/img/innerbox-logo-img1.png" />
          <div class="line"></div>
        </div>

      </div>
      <!-- contentbox_second end here -->
    </div>
    <div class="span4" align="center">
      <!-- contentbox_third start here -->
      <div class="contentbox-third">

        <div class="contentbox-third-inner">
          <img src="<?php echo base_path(); ?>sites/all/themes/waterways/assets/img/innerbox-top-img.png" />
          REDEEM YOUR <br>
          <span>Voucher</span> <br>
          <br>
          <img src="<?php echo base_path(); ?>sites/all/themes/waterways/assets/img/innerbox-logo-img2.png" />
          <div class="line"></div>
        </div>

      </div>
      <!-- contentbox_third end here -->
    </div>

  </div><!-- row-fluid end -->
  <div class="line2"></div>

  <div class="row-fluid">
    <div class="span6">
      <!-- contenttext_widget start here -->
      <div class="contenttext-widget">
        <h2>CONNECT</h2>
        <span>Let's Get Social</span>
        <br>
        Fecho park iphone godard retro, ugh id neutra fingerstache occaecat. Exercitation flannel dolor, echo park portland messenger bag deserunt eu selfies beard plaid skateboard.

      </div>
      <!-- contenttext_widget end here -->
    </div>
    <div class="span6">
      <!-- socialicons_widget start here -->
      <div class="row-fluid">
        <div class="span6">
          <!-- content_facebook_widget start here -->
          <div class="content-facebook-widget">

            <img src="<?php echo base_path(); ?>sites/all/themes/waterways/assets/img/facebook-content-icon.png" />
            <span>
              <div class="widget-title">Facebook</div> 
              Echo park iphone <br>
              godard retro, ugh id <br>
              neutrar.
            </span>

          </div>
          <!-- content_facebook_widget end here -->
        </div>
        <div class="span6">
          <!-- content_twitter_widget start here -->
          <div class="content-twitter-widget">

            <img src="<?php echo base_path(); ?>sites/all/themes/waterways/assets/img/twitter-content-icon.png" />
            <span>
              <div class="widget-title">Twitter<br>
                @waterwayscruise </div>
              Echo park iphone <br>
              godard retro, ugh id <br>
              neutrar.
            </span>

          </div>
          <!-- content_twitter_widget end here -->
        </div>
      </div><!-- row-fluid end -->
      <div class="row-fluid">
        <div class="span6">
          <!-- content_pintrest_widget start here -->
          <div class="content-pintrest-widget">

            <img src="<?php echo base_path(); ?>sites/all/themes/waterways/assets/img/pintrest-content-icon.png" />
            <span>
              <div class="widget-title">Pinterest</div>
              Echo park iphone <br>
              godard retro, ugh id <br>
              neutrar.
            </span>

          </div>
          <!-- content_pintrest_widget end here -->
        </div>
        <div class="span6">
          <!-- content_youtube_widget start here -->
          <div class="content-youtube-widget">

            <img src="<?php echo base_path(); ?>sites/all/themes/waterways/assets/img/youtube-content-icon.png" />
            <span>
              <div class="widget-title">Youtube</div>
              Echo park iphone <br>
              godard retro, ugh id <br>
              neutrar.
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