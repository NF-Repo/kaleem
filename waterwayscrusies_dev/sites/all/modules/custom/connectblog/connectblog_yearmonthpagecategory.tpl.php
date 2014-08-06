<?php
/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
?>

<script type="text/javascript" src="<?php echo base_path() . path_to_theme() ?>/js/sharebuttons.js"></script>
<script src="http://connect.facebook.net/en_US/all.js"></script>

<script>
    
    stLight.options({
        publisher: 'dr-52e8796a-b9e2-6a17-d82d-251b74277418',
        tracking: 'google',
        embeds: 'true',
        onhover: false
    });
    
    $(document).ready(function() {
        
        $("#blogCommentsCount").on("click", function(){
            $("#blogcommentslist").fadeToggle("slow", "linear");
        });
        $(".moreblogsInner").click(function(){
            $("#remainblogsInner").show();
            $('.moreblogs').hide();
        });
          if($("#remainblogsInner").length==0){
            $(".moreblogs").hide();
        }
        
        $('.mustReadbloglist select').change(function() {
          
            window.location.href = '<?php echo base_path() ?>' + $(this).val();
        });
        
         $('.blogLikelist select').change(function() {
            window.location.href = '<?php echo base_path() ?>' + $(this).val();
        });


        $("#shareBtn").on("click", function() {
            $("#stwrapper").show();
        });

        // Load Facebook User script
        $('.load-button').click(function() {
            getUser();
        });

        $("#archives").change(function() {
            var year = $('option:selected', this).closest('optgroup').attr('label');
            var month = $(this).val();

            window.location.href = Drupal.settings.basePath + "connectblog/year/" + year + "/month/" + month;
        });
        $("#subscribeClose").click(function() {
            $("#myModalSubscribe").modal('hide');
            $('div').removeClass('modal-backdrop fade in');
//            $('#requestQuoteForm input').removeClass('error');
        });

        $('.responsiveMobile select').change(function() {
            window.location.href = '<?php echo base_path() ?>' + $(this).val();
        });

        // To do active Tab
        $(".responsiveWeb li").removeClass("active");
        $('.connect').addClass("active");


        //Post Comment Button Submit function gete here
        $("#postcommentbtn").click(function() {
            if($(".blogCommentform").valid()){
            var blogid = $("#currentblogid").val();
            var blogcomments = $("#blogComments").val();
            var blogemail = $("#bloguserEmail").val();
            var blogusername = $("#bloguserName").val();
            var blogwebsite = $("#bloguserWebsite").val();

            if ($('#blognotificationscomment').is(':checked')) {
                var blognotificationcommentemail = 'Enabled';
            } else {
                var blognotificationcommentemail = 'disabled';
            }

            if ($('#blognotificationspost').is(':checked')) {
                var blognotificationpostemail = 'enabled';
            } else {
                var blognotificationpostemail = 'disabled';
            }

            //ajax call
            var params = 'blogid=' + blogid + '&blogcomments=' + blogcomments + '&bloguseremail=' + blogemail + '&blogusername=' + blogusername + '&bloguserwebsite=' + blogwebsite + '&commentnotify=' + blognotificationcommentemail + '&postnotify=' + blognotificationpostemail;
//            console.log(params);
            var url = Drupal.settings.basePath + "commentspost";

            // end of ajax call


            jQuery.ajax({
                type: "POST",
                cache: false,
                async: true,
                url: url,
                data: params,
                dataType: "json",
                beforeSend: function() {
                    $(".loadergif").show();
                    $("#postcommentbtn").attr("disabled", "disabled");
                },
                error: function(request, error) {
                    //error codes replaces here
                },
                success: function(response, status, req) {
                    //sucess code replaces here
                    if (response == 'success')
                    {
                        window.location.reload();
                    }
                },
                complete: function() {
                    //complete  codes gets replace here
                    $(".loadergif").hide();
                    $("#postcommentbtn").removeAttr("disabled");
                    $("#bloguserEmail").val("");
                    $("#bloguserName").val("");
                    $("#blogComments").val("");
                    $("#bloguserWebsite").val("");
                    $("#blognotificationscomment").prop('checked', false);
                    $("#blognotificationspost").prop('checked', false);
                }
            });
            }else{
               $("input.error").each(function(i, inputitem) {
                    if (i == 0)
                    {
                        $('input[name="' + this.name + '"]').focus();
                    }
                });
            }

        });


        $("#subscribeBlog").click(function() {
 if($("#blogCommentform").valid()){
            var blogid = $("#currentblogidPopup").val();
            var blogemail = $("#bloguserEmailPopup").val();
            var blogusername = $("#bloguserNamePopup").val();
            var blogwebsite = $("#bloguserWebsitePopup").val();

//            alert(blogid);
//            alert(blogemail);
//            alert(blogusername);
//            alert(blogwebsite);


            if ($('#blognotificationscommentPopup').is(':checked')) {
                var blognotificationcommentemail = 'enabled';
            } else {
                var blognotificationcommentemail = 'disabled';
            }

//            alert(blognotificationcommentemail);


            if ($('#blognotificationspostPopup').is(':checked')) {
                var blognotificationpostemail = 'enabled';
            } else {
                var blognotificationpostemail = 'disabled';
            }

//            alert(blognotificationpostemail);

            var params = 'blogid=' + blogid + '&bloguseremail=' + blogemail + '&blogusername=' + blogusername + '&bloguserwebsite=' + blogwebsite + '&commentnotify=' + blognotificationcommentemail + '&postnotify=' + blognotificationpostemail;

            var url = Drupal.settings.basePath + "subscribeBlog";


            jQuery.ajax({
                type: "POST",
                cache: false,
                async: true,
                url: url,
                data: params,
                dataType: "json",
                beforeSend: function() {
                    $(".loadergif").show();
                    $("#subscribeBlog").attr("disabled", "disabled");
                },
                error: function(request, error) {
                    //error codes replaces here
                },
                success: function(response, status, req) {
                    //sucess code replaces here
                    if (response == 'success')
                    {
                        window.location.reload();
                    }
                },
                complete: function() {
                    //complete  codes gets replace here
                    $(".loadergif").hide();
                    $("#subscribeBlog").removeAttr("disabled");
                    $("#bloguserEmailPopup").val("");
                    $("#bloguserNamePopup").val("");
                    $("#blogCommentsPopup").val("");
                    $("#bloguserWebsitePopup").val("");
                    $("#blognotificationscommentPopup").prop('checked', false);
                    $("#blognotificationspostPopup").prop('checked', false);
                }
            });
  }else{
               $("input.error").each(function(i, inputitem) {
                    if (i == 0)
                    {
                        $('input[name="' + this.name + '"]').focus();
                    }
                });
            }

        });


        //end of post comment button ends here



    });
    
    function twitterOauth()
    {
        window.location.href = Drupal.settings.basePath + "wwctwitteraouth";
    }

    function getUser()
    {
          var facebookappid=<?php echo $facebookappid;?>;
        FB.init({
            appId: facebookappid, //App ID
//            channelUrl: channelurl, //Channel File
            status: true, // check login status
            cookie: true, // enable cookies to allow the server to access the session
            xfbml: true  // parse XFBML
        });
        //check current user login status
        FB.getLoginStatus(function(response) {
            if (response.status === 'connected') {
                loadFriends();
            } else {
                //user is not connected.
                FB.login(function(response) {
                    console.log(response);
                    if (response.authResponse) {
                        loadFriends();
                    } else {
                        alert('User cancelled login or did not fully authorize.');
                    }
                }, {scope: 'email'});
            }
        }, {scope: 'email'});

    }

    function loadFriends()
    {
        //get array of friends
        ///me/friends
        /* FB.api('/me/friends', function(response) {
         console.log(response);
         var divContainer=$('.facebook-friends');
         for(i=0;i<response.data.length;i++)
         {
         $(document.createElement("img")).attr({ src: 'https://graph.facebook.com/'+response.data[i].id+'/picture', title: response.data[i].name ,onClick:'alert("You poked "+this.title);alert(this.data[i].email);'})
         .appendTo(divContainer);       
         }
         });*/
//        alert("you there");
        FB.api('/me', function(response) {
//            alert("Got response");
            console.log(response);
            console.log(response.email);
            console.log(response.name);
            $("#bloguserEmail").val(response.email);
            $("#bloguserName").val(response.name);
        });

    }
    
</script>
<?php $server_url = $GLOBALS['base_url'];
$_SESSION['blogid'] = $blogid;
$_SESSION['year']=$yearselected;
$_SESSION['month']=$monthselected;?>
<div class="container">

    <div class="detailBackLink">
        <a href="<?php echo base_path() . 'connectblog'; ?>"><img src="<?php echo base_path() . path_to_theme() ?>/images/back_arrow.png" />&nbsp;BACK TO BLOG</a>
    </div>

    <div class="connectMain">
        <!--connect title and sub menu start here--> 
        <div class="row-fluid" style="display: none;">
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

        <div class="row-fluid blogMain">
            <div class="span5">
                <!-- Archive start here -->
                <div class="blogArchive">
                    <div class="archiveTitle">Archive</div>
                    <?php print $archivecalendar; ?>
                </div>
                <!-- Archive end here -->
                <!--Subscribe start here--> 
                <div class="blogSubscribe">
                    <a data-toggle="modal" role="button" href="#myModalSubscribe">subscribe to our blog </a>
                </div>
                <!--Subscribe end here--> 
                
                <!--Must Read Start here--> 
                <div class="blogMustRead">
                    <div class="mustReadTitle">
                        Must Read Posts!
                    </div>
                    <div class="mustReadbloglist visible-desktop visible-tablet hidden-phone">
                        <?php print $mustreadposts['htmlweb']; ?>
                    </div>
                    <div class="mustReadbloglist visible-phone hidden-desktop hidden-tablet">
                        <select><?php print $mustreadposts['htmlmobile']; ?></select>
                    </div>
                </div>
                <!--Must Read end here-->
                
                <!--Blogs Like start here-->
                <div class="blogLike">
                    <div class="blogLikeTitle"> Blogs We Like </div>
                    <div class="blogLikelist visible-desktop visible-tablet hidden-phone">
                        <?php print $likereadposts['htmlweb']; ?>
                    </div>
                    <div class="blogLikelist visible-phone hidden-desktop hidden-tablet">
                        <select><?php print $likereadposts['htmlmobile']; ?></select>
                    </div>
                </div>
                <!--Blogs Like end here-->



            </div>

            <!--blog content starts here-->


            <div class="span7">
                <?php
                if ($blogpage != 'norecords') {
                    ?>
                    <div class="blogInner">

                        <div class="blogtemplateTitle">
                            <?php print $blogtitle; ?><br>
                        
                        </div>
                            <div class="blogCreated"> <?php print $blogcreated; ?></div>
                        <div class="blogShare" id="shareBtn">
                            <span class="blogShareInner st_sharethis_custom" st_url="<?php echo $server_url . '/connectblog/year/'.$yearselected.'/month/'.$monthselected;  ?>" >Share</span>
                        </div>

                        <div class="blogContent">
                            <?php print $blogpage; ?>
                        </div>



                        <?php //print $blogimage;?>

                        <div class="blogRecommend">
                            <div class="blogRecommendTitle">Recommend this!</div>
                            <div class="blogyes">
                                <a href="#"> 
                                    <div class="blogyesInner">Yes</div>
                                </a>
                            </div>






                        </div>
                        <div class="joinconversation">
                            Join the Conversation <span id="blogCommentsCount"><?php print $comments; ?></span>
                            <!--Blog Comment get showed here-->
                            <div id="blogcommentslist" style="display: none;">
                                <?php print $expandcomments; ?>
                            </div>
                            <!--end of blog comments showing -->
                        </div>
                        <form class="blogCommentform">

                            <textarea placeholder="Enter your comment here..." id="blogComments"></textarea>
                            <label>
                                Fill in your details below or click an icon to log in:
                            </label>
                            <div class="socialmediaIcons">
                                <div id="fb-root"></div>
                                <a href="#" onclick="twitterOauth();"><img src="<?php echo base_path() . path_to_theme(); ?>/images/twitter-icon.png"/></a> &nbsp;
                                <span class="load-button"><img src="<?php echo base_path() . path_to_theme(); ?>/images/facebook-icon.png"/></span>
                            </div>
                            <input type="hidden" id="currentblogid" name="currentblogid" value="<?php print $blogid; ?>"/>
                            <input type="text" placeholder="Email (required, address never made public)" 
                                   name="bloguserEmail" id="bloguserEmail" />
                                   <?php
                                   $twittername = variable_get('twttername');
                                   if (isset($twittername)) {
                                       ?>
                                    <input type="text" placeholder="Name (required)" 
                                           name="bloguserName" id="bloguserName" value="<?php print $twittername; ?>"/>

                                    <?php
                                    variable_del('twttername');
                                    } else {
                                    ?>
                                    <input type="text" placeholder="Name (required)" 
                                           name="bloguserName" id="bloguserName"/>
                                    <?php } ?>
                            <input type="text" placeholder="Website" 
                                   name="bloguserWebsite" id="bloguserWebsite"/>

                            <div class="checkbox" style="margin-top: 3%;">
                                <input type="checkbox"
                                       name="blognitifications"
                                       id="blognotificationscomment" value="commentemail"/> Notify me of follow-up comments via email</div>
                            <div class="checkbox">
                                <input type="checkbox"
                                       name="blognitifications" 
                                       id="blognotificationspost" value="commentpost"/> Notify me of new posts via email</div>

                            <span class="loadergif" style="display:none;"><img src="<?php echo base_path() . path_to_theme(); ?>/images/status-active.gif" /></span>
                            <div class="row-fluid commentButton" align="center">

                                <div class="span4">
                                    <div class="blogline"></div>
                                </div>
                                <div class="span4 postCommentBtn">
                                    <!-- booknow start here -->
                                    <div class="booknow ">
                                        <div class="booknow-inner">
                                            <input type="button" class="inptBooknow" value="Post Comment" id="postcommentbtn" />
                                        </div>
                                    </div>
                                    <!-- booknow end here -->
                                </div>
                                <div class="span4">
                                    <div class="blogline"></div>
                                </div>
                            </div>

                        </form>


                    </div>

                    <div id="totalblogs">
                        <?php
                        $i = 0;
                        foreach ($blogthumpage as $k => $v) {

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


                    <div class="moreblogs">
                        <div class="moreblogsInner">
                            
                                <img src="<?php echo base_path();  ?>sites/all/modules/custom/fleet/images/banner-icon-img.png">
                                <br>Click for more blogs
                            
                        </div>
                    </div>
                <?php } else { ?>
                    <div>No Records Found</div>
                <?php } ?>

            </div>

            <!--blog content-->



        </div>



    </div>
</div> <!-- container end here -->


<div id="myModalSubscribe" class="modal hide" tabindex="-1" role="dialog" aria-labelledby="myModalLabel" aria-hidden="true">
    <form class="blogCommentform" id="blogCommentform">
        <div class="modal-header">
            <button type="button" id="subscribeClose"  class="close">x</button>
            <h3 id="myModalLabel">Subscribe To Our Blog</h3>
        </div>
        <div class="modal-body">
            <input type="hidden" id="currentblogidPopup" name="currentblogid" value="<?php print $blogid; ?>"/>
            <input type="text" placeholder="Email (required, address never made public)" name="bloguserEmail" id="bloguserEmailPopup" />
            <input type="text" placeholder="Name (required)" name="bloguserName" id="bloguserNamePopup"/>
            <input type="text" placeholder="Website" name="bloguserWebsite" id="bloguserWebsitePopup"/>

            <div class="checkbox">
                <input type="checkbox" name="blognitifications" id="blognotificationscommentPopup" value="commentemail"/> Notify me of follow-up comments via email
            </div>
            <div class="checkbox">
                <input type="checkbox" name="blognitifications" id="blognotificationspostPopup" value="commentpost"/> Notify me of new posts via email
            </div>
        </div>
        <div class="modal-footer">
            <div class="span4 booknow">
                <div class="booknow-inner">
                    <input type="button" id="subscribeBlog" class="pressbtn" value="Submit" />
                </div>
            </div>
            
            <span class="loadergif" style="display:none;"><img src="<?php echo base_path() . path_to_theme(); ?>/images/status-active.gif" /></span>
        </div>
    </form>
</div>