<?php
/**
 * @file
 * Custom theme implementation to display the basic html structure of a single
 * Drupal page.
 */
?>
<!DOCTYPE html>
<html lang="<?php print $language->language ?>" class="no-js">
    <head profile="<?php print $grddl_profile ?>">
        <meta http-equiv="X-UA-Compatible" content="IE=edge,chrome=1">
        <!--<meta name="viewport" content="width=device-width, initial-scale=1.0,maximum-scale=1, user-scalable=no"/>-->
        <meta name="viewport" content="width=100%; initial-scale=1; maximum-scale=1; minimum-scale=1; user-scalable=no;" />
        <?php print $head ?>
        <title><?php print $head_title ?></title>
        <?php print $styles ?>
        <?php print $scripts ?>
        <!-- HTML5 support for IE6-8 --> 
        <!--[if lt IE 9]>
          <script src="//html5shiv.googlecode.com/svn/trunk/html5.js"></script>
        <![endif]-->
        <!--<script src="//ajax.googleapis.com/ajax/libs/jquery/1.8.1/jquery.min.js"></script>-->
        <!--<script src="http://ajax.googleapis.com/ajax/libs/jquery/1/jquery.min.js"></script>-->
        <script src="<?php echo base_path() . path_to_theme(); ?>/assets/js/bootstrap-collapse.js"></script>
        <script src="<?php echo base_path() . path_to_theme() ?>/assets/js/jquery-1.9.1.js"></script>
        <script type="text/javascript">
            $(window).load(function() {
                $("#progress-bar").stop().animate({width: "100%"}, 600, function() {
                    $("#loading").fadeOut("fast", function() {
                        $(this).remove();
                    });
                });
            });
        </script>
        <script src="<?php echo base_path() . path_to_theme(); ?>/assets/js/validation.js"></script>
        <script src="<?php echo base_path() . path_to_theme(); ?>/assets/js/script.js"></script>
<!--        <link href="<?php echo base_path() ?>/sites/all/themes/waterways/assets/css/largedesktopmediaqueries.css" rel="stylesheet" type="text/css" media="only screen and (min-width: 1001px) and (max-width: 3000px)" />
        <link href="<?php echo base_path() ?>/sites/all/themes/waterways/assets/css/jquery-ui.css" rel="stylesheet" type="text/css" />-->
        <link href="<?php echo base_path() . path_to_theme(); ?>/assets/css/largedesktopmediaqueries.css" rel="stylesheet" type="text/css" media="only screen and (min-width: 1001px) and (max-width: 3000px)" />
        <link href="<?php echo base_path() . path_to_theme(); ?>/assets/css/jquery-ui.css" rel="stylesheet" type="text/css" />

        <?php
        global $user;
        if (in_array('admin', $user->roles)) {
            ?>
            <style>
                #menu{
                    position: static !important;
                }
                #content{
                    display: block  !important;
                    padding-bottom: 1%;
                }
            </style>
            <?php
        }
        ?>



    </head>
    <body class="<?php print $classes ?>"<?php print $attributes ?>>

        <?php print $page_top ?>
        <?php print $page ?>
        <?php print $page_bottom ?>

        <!-- Placed at the end of the document so the pages load faster -->
        <script src="<?php echo base_path() . path_to_theme(); ?>/assets/js/bootstrap-alert.js"></script>
        <script src="<?php echo base_path() . path_to_theme(); ?>/assets/js/bootstrap-tooltip.js"></script>
        <script src="<?php echo base_path() . path_to_theme(); ?>/assets/js/bootstrap-popover.js"></script>
        <script src="<?php echo base_path() . path_to_theme(); ?>/assets/js/bootstrap-modal.js"></script>
<!--        <script src="<?php echo base_path() . path_to_theme(); ?>/assets/js/bootstrap-datepicker.js"></script>-->

        <script defer src="<?php echo base_path() . path_to_theme() ?>/assets/js/jquery.ui.datepicker.js"></script>
        <script defer src="<?php echo base_path() . path_to_theme() ?>/assets/js/jquery-ui.multidatespicker.js"></script>
        <script src="<?php echo base_path() . path_to_theme(); ?>/assets/js/bootstrap-transition.js"></script>
        <script src="<?php echo base_path() . path_to_theme(); ?>/assets/js/bootstrap-carousel.js"></script>

        <!--[if lte IE 9]><!-->
        <script src="<?php echo base_path() . path_to_theme() ?>/assets/js/jquery.placeholder.js"></script>
        <!--<link type="text/css" rel="stylesheet" href="/<?php print $directory ?>/assets/css/bootstrap-responsive-940.css" media="screen" />
        <script type="text/javascript" src="/<?php print $directory ?>/assets/js/bootstrap-responsive-940.js"></script>
        <script type="text/javascript" src="/<?php print $directory ?>/assets/js/affix.js"></script>-->
        <!--<![endif]-->

        <script type="text/javascript">
            function fm_optimizeInput() {
                $("input[placeholder],textarea[placeholder]").each(function() {
                    var tmpText = $(this).attr("placeholder");
                    if (tmpText != "") {
                        $(this).attr("placeholder", "").attr("placeholder", tmpText);
                    }
                });
            }
            function getCaptcha() {
                var chars = "0Aa1Bb2Cc3Dd4Ee5Ff6Gg7Hh8Ii9Jj0Kk1Ll2Mm3Nn4Oo5Pp6Qq7Rr8Ss9Tt0Uu1Vv2Ww3Xx4Yy5Zz";
                var string_length = 5;
                var captchastring = '';
                for (var i = 0; i < string_length; i++) {
                    var rnum = Math.floor(Math.random() * chars.length);
                    captchastring += chars.substring(rnum, rnum + 1);
                }
                document.getElementById("randomfield").innerHTML = captchastring;
                document.getElementById("captchavalue").value = captchastring;

            }
            $(function() {
                
                
                var ua = navigator.userAgent.toLowerCase();
                var isAndroid = ua.indexOf("android") > -1; //&& ua.indexOf("mobile");
                if(isAndroid) {

                         $('#sliderContent .slider-caption').css('top','-210px');
                      //Listen for orientation changes
                        window.addEventListener("orientationchange", function() {
                         // Announce the new orientation number
                         if(window.orientation==90 || window.orientation==(-90))
                         {
                              $('#sliderContent .slider-caption').css('top','-230px');
                         }
                         if(window.orientation==0){
                              $('#sliderContent .slider-caption').css('top','-210px');
                         }
                        }, false);
                    }



                
                
                
                
                
                
                

                $("#rquestquoteClose").click(function() {
                    $("#myModalrequest").modal('hide');
                    $('div').removeClass('modal-backdrop fade in');
                    $('#requestQuoteForm input').removeClass('error');

                });
                //carousel code implementation
                $('#myCarousel').carousel({
                    interval: 8000
                }).on('slide', function(e) {
                    var mainSlider = parseInt($('#myCarousel .active').index('#myCarousel .item'));
                    var slideFrom = $(this).find('.active').index();
                    var slideTo = $(e.relatedTarget).index();
                    var nextslide = mainSlider + 1;
                    var nextindicatorname = '#ind' + slideTo;
                    $(".customIndicators li div").removeClass("active");
                    $(nextindicatorname).addClass("active");


                });
                $('#mydownCarousel').carousel({
                    interval: 8000
                }).on('slide', function(e) {
                    var downSlider = parseInt($('#mydownCarousel .active').index('#mydownCarousel .item'));
                    var middleslideFrom = $(this).find('.active').index();
                    var middleslideTo = $(e.relatedTarget).index();
                    var middlenextslide = downSlider + 1;
                    var middlenextindicatorname = '#middleind' + middleslideTo;
                    $(".customIndicatorsMiddle li div").removeClass("active");
                    $(middlenextindicatorname).addClass("active");

                });




                $("#slidersitems div:first-child").addClass("active");

                $(document).on('click', '.indicator', function() {
                    var indicatorid = parseInt(this.id.slice(3));
                    $('#myCarousel').carousel('pause');
                    $('#myCarousel').carousel(indicatorid);
                });
                $("#caroselnext").click(function(e) {
                    $('#myCarousel').carousel('next');
                    $("#myCarousel").carousel("cycle");
                    e.preventDefault();
                });
                $("#caroselprev").click(function(e) {
                    $('#myCarousel').carousel('prev');
                    $("#myCarousel").carousel("cycle");
                    e.preventDefault();
                });
                //end of bootstrap carosle
                //Issue fix script for Iphone potrait view and landscapr view
                $(window).bind("orientationchange.fm_optimizeInput", fm_optimizeInput);
                $("#cruiseKeyPop").popover({
                    //title: 'Popover on Top',
                    html: true,
                    trigger: 'click',
                    content: function() {
                        return $('#popover_content_wrapper').html();
                    },
                    placement: 'top'
                });

                $('input, textarea').placeholder();

                //for faq page mobile code
                $('.responsiveMobile .mobileFaq').change(function() {
                    window.location.href = '<?php echo base_path() ?>' + $(this).val();
                });
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

  

                // for search box hide and show when click on search icon
                $('.searchBtnmobile').on('click', function() {
                    $('.search').show();
//                    $('.links').hide();
                    $('.searchBtnmobile').hide();
                });
                $("#directionsModal").on('show', function(event) {
                    $('body').removeClass('modal-open');
                    $('.modal-backdrop').remove();
                });


                $("#myModalrequest").on('show', function(event) {
                    $(window).scrollTop(0);

                    $('body').removeClass('modal-open');
                    $('.modal-backdrop').remove();
                });

                $("#myModalfeed").on('show', function(event) {
                    $(window).scrollTop(0);
                    $('body').removeClass('modal-open');
                    $('.modal-backdrop').remove();
                    $('body').removeClass('modal-open');
                    $('.modal-backdrop').remove();
                });

                $("#corporateGiving").on('show', function(event) {
                    $(window).scrollTop(0);
                    $('body').removeClass('modal-open');
                    $('.modal-backdrop').remove();
                    
                    getCaptcha();
                });

                var date = new Date();
                $('#quoteEventDatediv').datepicker({autoclose: false, minDate:date,
                 altField: "#quoteEventDatetext"});
//                $('#quoteEventDatetext').datepicker({autoclose: true});
//            $('#quoteEventDatetext').datepicker('update', date.toString('mm/dd/yyyy'));
//                $("#quoteEventDatetext").datepicker();



                $(".add-on").click(function() {
                    $(this).prev().focus();
                });
                //                 ui-datepicker-div


//                $('#corporateEventDateText').datepicker({autoclose: true});
                   $('#corporateEventDateText').datepicker({autoclose: true, minDate:new Date()});
                //                $('#corporateEventDateText').datepicker('update',date.toString('mm/dd/yyyy'));

                $("#ui-datepicker-div").hide();

                $('#quoteEventtypeotherText').hide();
                $('#OtherText').change(function() {

//                    if ($(this).attr('checked'))
                    if (this.checked)
                    {
                        $('#quoteEventtypeotherText').show();
                    }
                    else
                    {
                        $('#quoteEventtypeotherText').hide();
                    }
                });

                $('#myModalrequest').on('hidden', function() {
                    $('div').removeClass('modal-backdrop fade in');
                });

                $('.modal').on('hidden', function() {
                    $(".carousel").carousel("cycle");
                });
                $('.modal').on('show', function() {
                    $(".carousel").carousel("pause");
                });

                $('#corporateGiving').on('hidden', function() {
                    // do somethingÃ¢â‚¬Â¦
                    $('#corporateGiving input,textarea').removeClass('error');
                    $('div').removeClass('modal-backdrop fade in');
                });
                //To do Later on we need to check for the proper javascript methods
                $("#feedbackmodalClose").click(function() {
                    $("#myModalfeed").modal('hide');

                    $('div').removeClass('modal-backdrop fade in');
                    $('#feedbackform input,textarea').removeClass('error');
                });

                $("#corporatemodalClose").click(function() {
                    $("#corporateGiving").modal('hide');
                    $('div').removeClass('modal-backdrop fade in');
                    $('#corporateGivingform input').removeClass('error');

                });


                $("#rquestquotethanksClose").click(function() {
                    $("#myModalthankyou").modal('hide');
                    $('div').removeClass('modal-backdrop fade in');

                });


                $("#feedbackthanksClose").click(function() {
                    $("#myModalfeedbackthankyou").modal('hide');
                    $('div').removeClass('modal-backdrop fade in');
                });

                //End of Feedback Modal Popup close button task 

                $("#corporategivingthanksClose").click(function() {
                    $("#myModalcorporategivingthankyou").modal('hide');
                    $('div').removeClass('modal-backdrop fade in');
                });

                //End of Corporate Giving Modal Popup close button task 

                //RequestQuote Form
                $("#requestQuotemodalbtn").click(function() {
                    //          console.log($("#quoteEventtypeotherText").val());

                    var form = $("#requestQuoteForm");

                    if (form.valid())
                    {
                        $(".loadergif").show();
                        $("#requestQuotemodalbtn").attr('disabled', 'disabled');
                        //map is used to convert the original array to new one
                        var EventTypes = $('input[name="quoteEventtype[]"]:checked').map(function() {
                            return $(this).val();
                        }).get();

                        var quoteEventTypes = EventTypes.join();
                        var quoteEventtypeotherText = $("#quoteEventtypeotherText").val();
                        var quoteUserComments = $("#quoteUserComments").val();
                        var quoteEstimatedbudget = $("#estimatedBudget").val();

                        var EventEnabled = $('input[name="quoteSpecialEmailenabled[]"]:checked').map(function() {
                            return $(this).val();
                        }).get();
                        /*  specials by email  */
                        var quoteEventEnabled = EventEnabled.join();
                        if (quoteEventEnabled == 'Yes')
                        {
                            quoteEventEnabled = 'Yes';
                        }
                        else
                        {
                            quoteEventEnabled = 'No';
                        }
                        var quoteUserComments = $("#quoteUserComments").val();
                        var quotefindby = $("#quotefindby").val();
                        var quotepreferredvenue = $("#quotepreferredvenue").val();
                        var EventTimes = $('input[name="quoteEventTime[]"]:checked').map(function() {
                            return $(this).val();
                        }).get();
                        var quoteEventTimes = EventTimes.join();


                        var preferredtocontact = $('input[name="preferdcontact[]"]:checked').map(function() {
                            return $(this).val();
                        }).get();

                        var quotepreferredtocontact = preferredtocontact.join();

                        var myEventSelection = $('input[name="myEventSelection[]"]:checked').map(function() {
                            return $(this).val();
                        }).get();
                        

                        var myEventchoice = myEventSelection.join();
                        
                        var quoteEventDate = $("#quoteEventDatetext").val();
                        var quoteEstimatedbudget = $("#estimatedBudget").val();

                        var quoteCompany = $("#quoteCompany").val();
                        var quoteconfirmEmail = $("#quoteconfirmEmail").val();
                        var quoteFirstName = $("#quoteFirstName").val();
                        var quoteGuestsNo = $("#quoteGuestsNo").val();
                        var quoteLastName = $("#quoteLastName").val();
                        var quoteBestTimecall = $("#quoteBestTimecall").val();
                        var quotePhoneNumber = $("#quotePhoneNumber").val();
                        var quoteEmail = $("#quoteEmail").val();
                        var quoteEventtypeotherText = $("#quoteEventtypeotherText").val();


                        //ajax call for Quote Form
                        //        var url = Drupal.settings.basePath + 'minisiteservice/userprofile';
                        var url = Drupal.settings.basePath + 'requestquoteservice';
                        var params = 'eventTypes=' + quoteEventTypes + '&otherEvenTypetext=' + quoteEventtypeotherText;
                        params = params + '&firstName=' + quoteFirstName + '&lastName=' + quoteLastName + '&email=' + quoteEmail;
                        params = params + '&phoneNumber=' + quotePhoneNumber + '&timetocall=' + quoteBestTimecall;
                        params = params + '&eventDate=' + quoteEventDate + '&guestnumbers=' + quoteGuestsNo;
                        params = params + '&eventTime=' + quoteEventTimes + '&preferedVenue=' + quotepreferredvenue;
                        params = params + '&quoteCompany=' + quoteCompany + '&quoteconfirmEmail=' + quoteconfirmEmail;
                        params = params + '&quoteUserComments=' + quoteUserComments + '&quoteEventEnabled=' + quoteEventEnabled;
                        params = params + '&quoteEventtypeotherText=' + quoteEventtypeotherText + '&quotepreferredtocontact=' + quotepreferredtocontact;
                        params = params + '&findBy=' + quotefindby + '&quoteEstimatedbudget=' + quoteEstimatedbudget;
                        params = params + '&myEventchoice=' + myEventchoice;
                        $.ajax({
                            type: "POST",
                            cache: false,
                            async: true,
                            url: url,
                            data: params,
                            dataType: "json",
                            beforeSend: function() {

                            },
                            error: function(request, error) {
                                //error codes replaces here
                            },
                            success: function(response, status, req) {
                                if (response == 'success')
                                {
                                    //               alert('request quote created successfully');
                                    resetForm('requestQuoteForm');


                                    $("#myModalrequest").modal('hide');
                                    $('#myModalthankyou').modal('show');
                                    //               $("html, body").animate({ scrollTop: 0 }, "slow");
                                    $(window).scrollTop(0);
                                    //To Do Modal Popup Need to show
                                }
                                else {
                                    alert('failed to add Request quote');
                                }
                            },
                            complete: function() {
                                $(".loadergif").hide();
                                $("#requestQuotemodalbtn").removeAttr("disabled");
                            }
                        });
                        return true;
                    }
                    else
                    {
                        $("#requestQuoteForm input").each(function(i, inputitem) {
                            var attr = $(this).attr('required');

                            if (attr && $(this).val() == "") {
                                $(this).addClass("error");
                            }
                        });
                        $("input.error").each(function(i, inputitem) {
                            if (i == 0)
                            {
                                $('input[name="' + this.name + '"]').focus();
                            }
                        });
                        return false;
                    }

                });

                //End Of Request Form


                // corporate form submission
                $("#corporateformSubmit").click(function() {
                    //          console.log($("#quoteEventtypeotherText").val());

                    var form = $("#corporateGivingform");

                    if (form.valid())
                    {
                        $(".loadergif").show();
                        //$("#requestQuotemodalbtn").attr('disabled', 'disabled');
                        var donation = $("#donation").val();
                        var corporateEventDate = $("#corporateEventDateText").val();
                        var description = $(".corporate-desc").val();
                        var eventgoal = $("#eventgoal").val();
                        var corporateFirstName = $("#corporateFirstName").val();
                        var corporateLastName = $("#corporateLastName").val();
                        var corporateAddress = $("#corporateAddress").val();
                        var address2 = $("#address1").val();
                        var corporateCity = $("#corporateCity").val();
                        var corporatestate = $("#corporatestate").val();
                        var corporateZipcode = $("#corporateZipcode").val();
                        var corporatePhone = $("#corporatePhone").val();
                        var corporateEmail = $("#corporateEmail").val();
                        var corporateConfirmEmail = $("#corporateConfirmemail").val();



                        //ajax call for Corporate Form
                        //        var url = Drupal.settings.basePath + 'minisiteservice/userprofile';
                        var url = Drupal.settings.basePath + 'savecorporate';
                        var params = 'organising_requsting_donation=' + donation + '&date_event=' + corporateEventDate;
                        params = params + '&description_event=' + description + '&goal_event=' + eventgoal + '&first_name=' + corporateFirstName;
                        params = params + '&last_name=' + corporateLastName + '&address1=' + corporateAddress;
                        params = params + '&address2=' + address2 + '&city=' + corporateCity;
                        params = params + '&state=' + corporatestate + '&zipcode=' + corporateZipcode;
                        params = params + '&phone_number=' + corporatePhone + '&mail=' + corporateEmail;
                        params = params + '&confirmmail=' + corporateConfirmEmail;


                        $.ajax({
                            type: "POST",
                            cache: false,
                            async: true,
                            url: url,
                            data: params,
                            dataType: "json",
                            beforeSend: function() {

                            },
                            error: function(request, error) {
                                //error codes replaces here
                            },
                            success: function(response, status, req) {
                                if (response == 'success')
                                {
                                    //alert('request quote created successfully');
                                    resetForm('corporateGivingform');
                                     $("#corporateGiving").modal('hide');
                                     $('#myModalcorporategivingthankyou').modal('show');
                                    $(window).scrollTop(0);


                                }
                                else {
                                    alert('failed to save corporate');
                                }
                            },
                            complete: function() {
                                $(".loadergif").hide();
                                $("#corporateformSubmit").removeAttr("disabled");
                            }
                        });
                        return true;
                    }
                    else
                    {
                        $("#corporateGivingform input").each(function(i, inputitem) {
                            var attr = $(this).attr('required');

                            if (attr && $(this).val() == "") {
                                console.log($(this).val())
                                $(this).addClass("error");
                            }
                        });
                        $("input.error").each(function(i, inputitem) {
                            if (i == 0)
                            {
                                $('input[name="' + this.name + '"]').focus();
                            }
                        });
                        if ($("#corporatestate").val() == "") {
                            $("#corporatestate").addClass("error");
                        }

                        return false;
                    }

                });




                //feedbacl form
                $("#feedbackformSubmit").click(function() {

                    var formfeedback = $("#feedbackform");

                    if (formfeedback.valid())
                    {

                        $(".loadergif").show();
                        $("#feedbackformSubmit").attr('disabled', 'disabled');

                        var CommentType = $('input[name="feedbacktype"]:checked').val();
                        var feedbackdescription = $('#feedbackDescription').val();
                        var feedbackfirstname = $('#feedbackFirstName').val();
                        var feedbacklastname = $('#feedbackLastName').val();
                        var feedbackemail = $('#feedbackEmail').val();

                        var url = Drupal.settings.basePath + 'feedbackservice';
                        var params = 'commentstype=' + CommentType + '&feedbackdescription=' + feedbackdescription + '&firstName=' + feedbackfirstname + '&lastName=' + feedbacklastname + '&email=' + feedbackemail;
                        jQuery.ajax({
                            type: "POST",
                            cache: false,
                            async: true,
                            url: url,
                            data: params,
                            dataType: "json",
                            beforeSend: function() {

                            },
                            error: function(request, error) {
                                //error codes replaces here
                            },
                            success: function(response, status, req) {
                                if (response == 'success')
                                {
                                    //   alert('Feedback comments submitted successfully');
                                    resetForm('feedbackform');
                                    $("#myModalfeed").modal('hide');
                                    $('#myModalfeedbackthankyou').modal('show');
                                    $(window).scrollTop(0);
                                    //To Do Modal Popup Need to show
                                }
                                else {
                                    alert('failed to add Request quote');
                                }
                            },
                            complete: function() {
                                $(".loadergif").hide();
                                $("#feedbackformSubmit").removeAttr("disabled");
                            }
                        });
                        return true;
                    }
                    else
                    {
                        $("textarea.error,input.error").each(function(i, inputitem) {

                            if (i == 0)
                            {
                                $('textarea[name="' + this.name + '"]').focus();
                            }
                        });

                        return false;
                    }


                });

                //feedback form end 


                // Web Menu and Mobile Menu highlight with using following code.
                $('#mobilerequestProposal ul.menu li:nth-child(2) a').attr({
                    role: 'button',
                    "data-toggle": 'modal',
                    href: '#myModalrequest'
                });


            });//end of main Jquery


            function resetForm(id) {
                $('#' + id).each(function() {
                    this.reset();
                });
            }


        </script>
        <!--<script src="http://172.16.0.95:9090/target/target-script-min.js#uncle1234"></script>-->
    </body>
</html>
