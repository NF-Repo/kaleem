<?php
/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
?>


<script>
    $(document).ready(function() {
        if($("#remainblogsInner").length==0){
            $(".moreblogs").hide();
        }

    $(".moreblogsInner").click(function(){
        $("#remainblogsInner").show();
        $('.moreblogs').hide();
    });
    
     $("#archives").change(function() {

            var year = $('option:selected', this).closest('optgroup').attr('label');
            var month = $(this).val();


            window.location.href = Drupal.settings.basePath + "pressroom/year/" + year + "/month/" + month;
        });

    });

</script>




<div class="container pressroomContainer">
    <div class="contentDetail">

        <div class="press-room">
            
            <div class="row-fluid pressroom-info">
                <div class="span12 pressroom-title">
                    PRESS ROOM
                </div>
            </div>

            <div class="row-fluid pressroom-info">
                <div class = "span4">
                    <div class = "pressroom-sub-title">Media Contacts</div>
                    <ul class = "pressroom-list contact-list">
                        <?php print $loadcontacts ?>
                    </ul>
                </div>
                <div class = "span4">
                    <div class = "pressroom-sub-title">Media Photos</div>
                    <ul class = "pressroom-list">
                        <?php print $loadphotos ?>
                    </ul>
                </div>
                <div class = "span4">
                    <div class = "pressroom-sub-title">Media Kit</div>
                    <ul class = "pressroom-list">
                        <?php print $loadkit ?>
                    </ul>
                </div>
            </div>
            
            <?php
            
            

       
       $frst_image = preg_match_all( '|<img.*?src=[\'"](.*?)[\'"].*?>|i', $pressimage, $matches ); 
        $item['image'] = $matches[ 1 ][ 0 ];
        

       global $base_url;
       
   if($item['image']==$base_url.'/'){
    $image_display= "none";
    $press_image="span12";
        }else{
           $image_display= "block"; 
           $press_image="span8";
        }
            
         if( $latestdownloadpdfurl==$base_url.'/'){
            $latestdownload_display="none";
        }else{
            $latestdownload_display="block";
        }
        
        
            ?>
            
            
            <div class="row-fluid">
                <div class="span12 pressReleaseContainer">
                    <div class="row-fluid">
                        <div class="span4 pressImageone" style="display:<?php echo $image_display;?>">
                            <?php print $pressimage; ?>
                        </div>
                        <div class="<?php echo $press_image;?>">
                            <div class="pressroomTitle">
                                <a href="<?php echo base_path() . 'pressroom/pressid/' . $pressid; ?>"><?php print $presstitle; ?></a>
                                <br/>
                                <span><?php print $presscreated; ?></span>
                                <div class="pressroomDescription">
                                    <?php print $presspage; ?>
                                </div>
                                <?php
                                $downloadicon = base_path() . 'sites/all/modules/custom/pressroom/images/download-icon.png';
                                ?>
                                <div style="display:<?php echo $latestdownload_display ?>"><img src="<?php echo $downloadicon ?>" style="float: left; " /><a href="<?php echo $latestdownloadpdfurl; ?>" class="downloadpdfLink">Download PDF</a></div>
                            </div>
                        </div>
                    </div>
                </div>

                <div class="row-fluid pastPressReleaseTitle">
                    <div class="span12">
                        <h3>Past Press Releases</h3>
                        <h4>Archives</h4>
                        <?php print $archivepressreleases; ?>
                    </div>
                </div>


                <div class="allPressroomReleases">

                    <?php
                    $i = 0;
                    foreach ($pressthumpage as $k => $v) {

                        if ($i == 2) {
                            ?>
                        </div>                 

                        <div class="remainblogsInner" id="remainblogsInner" style="display:none">
                            <?php
                    }
                        print_r($v);
                        $i++;
                    }
                    ?>




                </div>


                <div class="row-fluid">
                    <div class="span12">
                        <div class="moreblogs">
                            <div class="moreblogsInner">
                                <a>
                                    <img src="<?php echo base_path() . path_to_theme() ?>/images/banner-icon-img.png" style="margin-bottom: 10px;">
                                    <br>Click For More Releases
                                </a>
                            </div>
                        </div>
                    </div>
                </div>
            </div>

        </div>
    </div> 
</div>
