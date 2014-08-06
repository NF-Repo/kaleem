<?php
/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
?>
<!--<script src="http://localhost/waterwayscruise/web/misc/jquery.js"></script>-->
<script type="text/javascript">
    $(function() {

//        $("#voucherboxFirst").bind('hover', function() {
//            $(".contentbox-first").css("display", "none");
//            $(".contentboxFirsthover").css("display", "block");
//        });

        $('#voucherboxFirst').mouseover(function(e) {
            $('#contentboxFirstout').hide();
            $('#contentboxFirstover').show();
        });
        $('#voucherboxFirst').mouseout(function(e) {
            $('#contentboxFirstout').show();
            $('#contentboxFirstover').hide();

        });

        $('#voucherboxSecond').mouseover(function(e) {
            $('#contentboxSecondout').hide();
            $('#contentboxSecondhover').show();
        });
        $('#voucherboxSecond').mouseout(function(e) {
            $('#contentboxSecondout').show();
            $('#contentboxSecondhover').hide();

        });

        $('#voucherboxThird').mouseover(function(e) {
            $('#contentboxThirdout').hide();
            $('#contentboxThirdhover').show();
        });
        $('#voucherboxThird').mouseout(function(e) {
            $('#contentboxThirdout').show();
            $('#contentboxThirdhover').hide();

        });

//        var dirbasepath = Drupal.settings.basePath;
//
//        $.getJSON(dirbasepath + 'rest/content?type=homepageslider', function(data) {
//            $.each(data, function(i, item) {
//                //       console.log(item);
//                //
//                if (i != 0)
//                {
//
//                    if (i == 1)
//                    {
//                        loadContentData(data[i].nid, 1);
//                    }
//                    else
//                    {
//                        loadContentData(data[i].nid, 0);
//                    }
//
//                }
//            });
//        });


        $('.carousel').carousel({
            interval: 6000
        });
        

//        $("#requestQuotemodal").click(function() {
//            console.log($("#quoteEventtypeotherText").val());
//            //map is used to convert the original array to new one
//            var EventTypes = $('input[name="quoteEventtype[]"]:checked').map(function() {
//                return $(this).val();
//            }).get(); // <----
//            console.log(EventTypes);
//            console.log($("#quoteUserComments").val());
//            var EventEnabled = $('input[name="quoteSpecialEmailenabled[]"]:checked').map(function() {
//                return $(this).val();
//            }).get(); // <----
//            console.log(EventEnabled);
//            console.log($("#quotefindby").val());
//            console.log($("#quotepreferredvenue").val());
//            var EventTimes = $('input[name="quoteEventTime[]"]:checked').map(function() {
//                return $(this).val();
//            }).get(); // <----
//            console.log(EventTimes);
//            console.log($("#quoteEventDate").val());
//            console.log($("#quoteCompany").val());
//            console.log($("#quoteconfirmEmail").val());
//            console.log($("#quoteLastName").val());
//            console.log($("#quoteBestTimecall").val());
//            console.log($("#quotePhoneNumber").val());
//            console.log($("#quoteEmail").val());
//            console.log($("#quoteFirstName").val());
//            console.log($("#quoteEventtypeotherText").val());
//
//        });

    });
//    function loadContentData(nid, firstitem)
//    {
//        console.log('loaddata');
//        var dirbasepath = Drupal.settings.basePath;
//        //    Drupal.settings.basePath;
//        $.getJSON(dirbasepath + 'rest/content/' + nid, function(nodedata) {
//            console.log(nodedata.fields);
//            //console.log(nodedata.title);
//            //console.log(nodedata.body[0].value);
//            //console.log(nodedata.fields.field_imagefile[0].uri);
//            //console.log(nodedata.fields.field_button_text[0].value);
//            //console.log(nodedata.fields.field_headding[0].value);
//
//            //var title = nodedata.title;
//            var bodycontent = nodedata.body[0].value;
//            var imageuri = nodedata.fields.field_imagefile[0].uri.replace("public://", dirbasepath + 'sites/default/files/');
//            //var buttontext = nodedata.fields.field_button_text[0].value;
//            //var headingtext = nodedata.fields.field_headding[0].value;
//
//            //image adding script
//
//            if (firstitem == 1)
//            {
//                var html = '';
//                var sliderhtml = '<div class="item"><img src="' + imageuri + '"><div id="sliderContent">' + bodycontent + '</div></div>';
//
//                //image adding scripts ends
//            }
//            else
//            {
//                var sliderhtml = '<div class="active item"><img src="' + imageuri + '"><div id="sliderContent">' + bodycontent + '</div></div>';
//            }
//            $("#slidersitems").append(sliderhtml);
//            //$("#sliderContent").append(bodycontent);
//        });
//    }


</script>
<div> 
    <!-- removed container class -->
    <!--<a name="top"></a>-->
    <!-- slider start here -->
    <!--<div id="myCarousel" class="carousel slide" style="margin-bottom: 0px;">
        <div id="slider">
            <div class="slider">

                <ol class="carousel-indicators">
                    <li data-target="#myCarousel" data-slide-to="0" class="active"></li>
                    <li data-target="#myCarousel" data-slide-to="1"></li>
                </ol>

                <div class="carousel-inner" id="slidersitems"></div>

                <!-- Carousel nav -->
                <!--<a class="carousel-control left" href="#myCarousel" data-slide="prev"><img src="<?php //echo base_path() . path_to_theme(); ?>/images/carousol-leftarrow.png" /></a>
                <a class="carousel-control right" href="#myCarousel" data-slide="next"><img src="<?php //echo base_path() . path_to_theme(); ?>/images/carousol-rightarrow.png" /></a>

            </div>
        </div>
    </div>
    <!-- slider end here -->

</div> 
<!-- /container -->

<div class="contentbox-inner">
    <div class="row-fluid">
        <div class="span4" align="center">
            <!-- contentbox_first start here -->
            <div id="voucherboxFirst">
                <div class="contentbox-first" id="contentboxFirstout">
                    <div class="contentbox-first-inner-normal">
                        <img src="<?php echo base_path(); ?>sites/all/themes/waterways/assets/img/innerbox-top-img.png" />
                        The Gift of 
                        <span>Unforgettable</span>
                        <div class="line"></div>
                    </div>
                </div>

                <div class="contentbox-first" id="contentboxFirstover" style="display: none;">
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
            </div>
            <!-- contentbox_first end here -->
        </div>
        <div class="span4" align="center">
            <!-- contentbox_second start here -->
            <div id="voucherboxSecond">
                <div class="contentbox-second" id="contentboxSecondout">
                    <div class="contentbox-second-inner">
                        <img src="<?php echo base_path(); ?>sites/all/themes/waterways/assets/img/innerbox-top-img.png" />
                        REDEEM YOUR <br>
                        <span>Voucher</span> <br>
                        <img src="<?php echo base_path(); ?>sites/all/themes/waterways/assets/img/innerbox-logo-img1.png" />
                        <div class="line"></div>
                    </div>
                </div>
                <div class="contentbox-second" style="display: none;"  id="contentboxSecondhover">
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
            </div>
            <!-- contentbox_second end here -->
        </div>
        <div class="span4" align="center">
            <!-- contentbox_third start here -->
            <div id="voucherboxThird">
                <div class="contentbox-third" id="contentboxThirdout">
                    <div class="contentbox-third-inner">
                        <img src="<?php echo base_path(); ?>sites/all/themes/waterways/assets/img/innerbox-top-img.png" />
                        REDEEM YOUR <br>
                        <span>Voucher</span> <br>
                        <br>
                        <img src="<?php echo base_path(); ?>sites/all/themes/waterways/assets/img/innerbox-logo-img2.png" />
                        <div class="line"></div>
                    </div>
                </div>
                <div class="contentbox-second" style="display: none;"  id="contentboxThirdhover">
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


<!-- REQUEST A PROPOSAL modal popup start here -->
<div id="myModalrequest" class="modal hide fade" tabindex="-1" role="dialog" aria-labelledby="myModalLabel" aria-hidden="true">
    <div class="modal-header">
        <button type="button" class="close" data-dismiss="modal" aria-hidden="true">×</button>
        <h3 id="myModalLabel">REQUEST A PROPOSAL</h3>
    </div>
    <div class="modal-body">
        <div class="row-fluid">
            <div class="span12">
                Start planning your event by requesting a quote information!  Call us at 206-223-2060 or fill out this form and one of our event planners will contact you within 48 hours.
                <br>
                <div class="alert-info"> *This information will be used only to contact you regarding your request,  and it will not be sold or transferred to anyone else.</div>
                <div class="row-fluid">
                    <div class="span12">
                        <form name="requestQuoteForm" id="requestQuoteForm">
                            <fieldset>
                                <legend>Type of Event</legend>
                                <div class="row-fluid">
                                    <div class="span6">
                                        <label class="checkbox">
                                            <input type="checkbox" name="quoteEventtype[]" value="Dining Cruise"> Dining Cruise
                                        </label>
                                        <label class="checkbox" name="quoteEventtype[]">
                                            <input type="checkbox" name="quoteEventtype[]" value="Wedding"> Wedding
                                        </label>
                                        <label class="checkbox">
                                            <input type="checkbox" name="quoteEventtype[]" value="Corporate Event"> Corporate Event
                                        </label>
                                        <label class="checkbox">
                                            <input type="checkbox" name="quoteEventtype[]" value="Birthday"> Birthday
                                        </label>
                                    </div>
                                    <div class="span6">
                                        <label class="checkbox">
                                            <input type="checkbox" name="quoteEventtype[]" value="Anniversary"> Anniversary
                                        </label>
                                        <label class="checkbox">
                                            <input type="checkbox" name="quoteEventtype[]" value="School Event"> School Event
                                        </label>
                                        <label class="checkbox">
                                            <input type="checkbox" name="quoteEventtype[]" value="EventTypeOther"> Other
                                        </label>
                                        <label class="input">
                                            <input type="text" placeholder="Enter Text.." 
                                                   name="quoteEventtypeotherText" id="quoteEventtypeotherText"> 
                                        </label>
                                    </div>
                                </div><!-- row-fluid end -->
                                <hr/>
                                <div class="row-fluid">
                                    <div class="span6">
                                        <label class="input-medium">
                                            <input type="text" placeholder="*First Name" name="quoteFirstName" id="quoteFirstName"/>
                                        </label>
                                        <label class="input-medium">
                                            <input type="text" placeholder="*Email" name="quoteEmail" id="quoteEmail"/>
                                        </label>
                                        <label class="input-medium">
                                            <input type="text" placeholder="*Phone Number" name="quotePhoneNumber" id="quotePhoneNumber"/>
                                            <small>(555) 555-5555</small>
                                        </label>
                                        <label>Best time to call?</label>
                                        <select name="quoteBestTimecall" id="quoteBestTimecall">
                                            <option value="Early Morning 8-10am">Early Morning 8-10am</option>
                                            <option value="Mid Morning 10am-1pm">Mid Morning 10am-1pm</option>
                                            <option value="Afternoon 1-5pm">Afternoon 1-5pm</option>
                                            <option value="Evening 5-8pm">Evening 5-8pm</option>
                                            <option value="Anytime 8am-8pm">Anytime 8am-8pm</option>
                                        </select>
                                    </div>
                                    <div class="span6">
                                        <label class="input-medium">
                                            <input type="text" placeholder="*Last Name" name="quoteLastName" id="quoteLastName"/>
                                        </label>
                                        <label class="input-medium">
                                            <input type="text" placeholder="*Confirm Email" name="quoteconfirmEmail" id="quoteconfirmEmail"/>
                                        </label>
                                        <label class="input-medium">
                                            <input type="text" placeholder="Compnay" name="quoteCompany" id="quoteCompany"/>
                                        </label>
                                    </div>
                                </div><!-- row-fluid end -->
                                <hr/>
                                <div class="row-fluid">
                                    <div class="span6">
                                        <label>Your preferred date of the event?</label>
                                        <!--To Do Need to Replace Date Picker-->
                                        <input type="text" placeholder="2013-02-02" 
                                               name="quoteEventDate" id="quoteEventDate"/>

                                        <label>Your preferred time of the event</label>
                                        <label class="checkbox">
                                            <input type="checkbox" name="quoteEventTime[]" value="Morning"> Morning
                                        </label>
                                        <label class="checkbox">
                                            <input type="checkbox" name="quoteEventTime[]" value="Afternoon"> Afternoon
                                        </label>
                                        <label class="checkbox">
                                            <input type="checkbox" name="quoteEventTime[]" value="Evening"> Evening
                                        </label>
                                    </div>
                                    <div class="span6">
                                        <label>Estimated number of guests</label>
                                        <select name="quoteGuestsNo" id="quoteGuestsNo">
                                            <option value="4">4</option>
                                        </select>
                                        <label>Your preferred venue?</label>
                                        <select name="quotepreferredvenue" id="quotepreferredvenue">
                                            <option value="Emerald Star (220 ppl)">Emerald Star (220 ppl)</option>
                                            <option value="Olympic Star (125 ppl)">Olympic Star (125 ppl)</option>
                                            <option value="The Destiny (40 ppl)">The Destiny (40 ppl)</option>
                                        </select>
                                    </div>
                                </div><!-- row-fluid end -->
                                <hr/>
                                <div class="row-fluid">
                                    <div class="span6">
                                        <label>How did you find out about us?</label>
                                        <select name="quotefindby" id="quotefindby">
                                            <option value="Website">Website</option>
                                        </select>

                                    </div>
                                    <div class="span6">
                                        <label>Yes, I would like to receive Waterways specials by email.</label>
                                        <label class="checkbox">
                                            <input type="checkbox" name="quoteSpecialEmailenabled[]" value="Morning"> Morning
                                        </label>
                                    </div>
                                </div><!-- row-fluid end -->
                                <hr/>
                                <div class="row-fluid">
                                    <div class="span12">
                                        <textarea placeholder="Comments" rows="4" class="span10" id="quoteUserComments"></textarea>

                                    </div>
                                </div><!-- row-fluid end -->
                            </fieldset>
                        </form>

                    </div>

                </div><!-- row-fluid end -->

            </div>
        </div><!-- row-fluid end -->
    </div>
    <div class="modal-footer">
        <button class="btn" data-dismiss="modal" aria-hidden="true">Close</button>
        <button class="btn btn-primary" id="requestQuotemodal">Submit Request</button>
    </div>
</div>

<!-- REQUEST A PROPOSAL modal popup end here -->

<!-- FEEDBACK FORM modal popup start here -->
<div id="myModalfeed" class="modal hide fade" tabindex="-1" role="dialog" aria-labelledby="myModalLabel" aria-hidden="true">
    <div class="modal-header">
        <button type="button" class="close" data-dismiss="modal" aria-hidden="true">×</button>
        <h3 id="myModalLabel">FEEDBACK FORM</h3>
    </div>
    <div class="modal-body">
        <div class="row-fluid">
            <div class="span12">
                We would love to hear your thoughts, concerns or problems with anything so we can improve!<br>
                <small> Required fields*</small>


                <div class="row-fluid">
                    <div class="span12">
                        <form>
                            <fieldset>

                                <div class="row-fluid">
                                    <div class="span12">
                                        <br>
                                        <label>Feedback Type</label>
                                        <div class="row-fluid">
                                            <div class="span3"> <label class="radio">
                                                    <input type="radio"> Comments
                                                </label></div>
                                            <div class="span3">
                                                <label class="radio">
                                                    <input type="radio"> Bug Reports
                                                </label>

                                            </div>
                                            <div class="span3">
                                                <label class="radio">
                                                    <input type="radio"> Questions
                                                </label>

                                            </div>
                                        </div><!-- row-fluid end -->


                                    </div>
                                </div><!-- row-fluid end -->
                                <hr/>
                                <div class="row-fluid">
                                    <div class="span12">
                                        <label>Describe Feedback*</label>
                                        <textarea placeholder="Enter text..." rows="4" class="span10" ></textarea>

                                    </div>
                                </div><!-- row-fluid end -->
                                <hr/>
                                <div class="row-fluid">
                                    <div class="span6">
                                        <label>First Name*</label>
                                        <input type="text" placeholder="Enter text..." />

                                    </div>
                                    <div class="span6">
                                        <label>Last Name*</label>
                                        <input type="text" placeholder="Enter text..." />

                                    </div>
                                </div><!-- row-fluid end -->
                                <hr/>
                                <div class="row-fluid">
                                    <div class="span6">
                                        <label>Email*</label>
                                        <input type="text" placeholder="Enter text..." />

                                    </div>
                                    <div class="span6">
                                        <label>Confirm Email*</label>
                                        <input type="text" placeholder="Enter text..." />

                                    </div>
                                </div><!-- row-fluid end -->

                            </fieldset>
                        </form>

                    </div>

                </div><!-- row-fluid end -->

            </div>
        </div><!-- row-fluid end -->
    </div>
    <div class="modal-footer">
        <button class="btn" data-dismiss="modal" aria-hidden="true">Close</button>
        <button class="btn btn-primary">Submit</button>
    </div>
</div>

<!-- REQUEST A PROPOSAL modal popup end here -->