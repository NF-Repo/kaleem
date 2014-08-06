<?php
/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
?>

<script type="text/javascript" src="<?php echo base_path().path_to_theme() ?>/js/sharebuttons.js"></script>

<script>
    
    stLight.options({
        publisher: 'dr-52e8796a-b9e2-6a17-d82d-251b74277418',
        tracking: 'google',
        embeds: 'true',
        onhover: false
    });
    
    $(document).ready(function() {
        
        $("#shareBtn").on("click", function(){
            $("#stwrapper").show();
        });

    });

</script>

<style type="text/css" media="print">
          @media print{
             @page {
  size: auto;   /* auto is the initial value */
  margin: 10% 10% 10% 2mm;
}
      @page :first {
  margin-left: 2mm;
}
   body * { visibility: hidden; }
  .press-room-details * { visibility: visible; }
  a{display: none ; }
  .press-room-details { position: absolute; top:-80px;}
  #shareBtn{display:none !important;}
  .span5{display:none !important;}
  .printdownloadpdf{display:none !important;}
  .pressReleaseContainer{width:100% !important;}
    </style>

<div id="content">
    <div class="contentDetail">
        <!-- sunset dining start here -->
        <div class="press-room-details">
            <div class="row-fluid backtoContainer">
                <div class="span6">
                    <div class="detailBackLink">
                        <a href="<?php echo base_path(); ?>pressroom"><img src="<?php echo base_path(); ?>sites/all/modules/custom/cruisesdetailview/images/back_arrow.png"> BACK TO PRESS RELEASES</a>
                    </div>
                </div>
                <div class="span6 sharebtnContainer">
                    <div  class="booknow span3" id="shareBtn">
                            <span class="booknow-inner st_sharethis_custom" st_url="<?php 
                        $server_url = $GLOBALS['base_url'];
                        
                        echo $server_url.'/pressroom/pressid/'.$pressid; ?>" displayText="Share" ><a href="javascript:void(0)" style="color:#fff;">Share</a></span>
                    </div>
                    <div class="booknow span3">
                        <a href="#" onclick="window.print();">
                            <div class="booknow-inner">Print</div>
                        </a>
                    </div>
                </div>
            </div>

            <div class="row-fluid pressroom-info">

                <div class="span12 pressroom-title">
                    PRESS RELEASE
                </div>
            </div>

             
            
            
            
            
            
            <div class="row-fluid pressroom-info">
                <div class='span8 pressReleaseContainer'>
                    <?php print $loadpressrelease; ?>
                </div>
                <div class='span4'>
                    <div class = "pressroom-sub-title">Media Contacts</div>
                    <ul class = "pressroom-list">
                        <?php print $loadcontacts ?>
                    </ul>
                    <br> 
                    <div class = "pressroom-sub-title">Media Photos</div>
                    <ul class = "pressroom-list">
                        <?php print $loadphotos ?>
                    </ul>

                </div>
            </div>
        </div> 
    </div>
</div>