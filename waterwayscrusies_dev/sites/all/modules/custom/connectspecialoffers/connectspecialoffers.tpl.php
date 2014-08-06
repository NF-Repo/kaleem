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
            touchEnabled: false,
            pager: false
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
        $('.specialoffers-list li .sliderBox').mouseover(function(e) {
            $(this).children(".sliderBoxout").hide();
            $(this).children(".sliderBoxover").show();
        });
        $('.specialoffers-list li .sliderBox').mouseout(function(e) {
            $(this).children(".sliderBoxover").hide();
            $(this).children(".sliderBoxout").show();
        });
        var dinnerUnavailableDates = new Array();
        var brunchUnavailableDates = new Array();
        dinnerUnavailableDates=<?php print json_encode($dinner); ?>;
        brunchUnavailableDates=<?php print json_encode($brunch); ?>;
        
        
        function changedate(month,year,dinner,itemid)
        {
            var url = Drupal.settings.basePath + 'eventsbymonth';
            var params = 'mon='+month+'&year='+year+'&cruisefilter='+itemid;

            jQuery.ajax({
                type: "GET",
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
                    console.log(response);
                    if(dinner=="dinner"){
                        dinnerUnavailableDates=response;
                        $("#sunsetdinerDateText").datepicker( "refresh" );
                    }else{
                        brunchUnavailableDates=response;
                        $("#sundayBrunchDateText").datepicker( "refresh" );
                    }
                    $("#transparentLoader").modal('hide');
                    $(".modal-backdrop,.modal-backdrop.fade.in").css("opacity",0.8);  
                },
                complete: function() {
                }
            });
        }
        function brunchUnavailable(date) {
            ymd = date.getFullYear() + "/" + ("0"+(date.getMonth()+1)).slice(-2) + "/" + ("0"+date.getDate()).slice(-2);
            day = new Date(ymd).getDay();
    
            // if ($.inArray(ymd, unavailableDates) < 0 && $.inArray(days[day], unavailableDays) < 0) {
            console.log('checking with dates');
            if ($.inArray(ymd, brunchUnavailableDates) < 0 ) {
                return [true, "enabled", "Book Now"];
            } else {
                return [false,"disabled","Booked Out"];
            }
        }
        function dinerUnavailable(date) {
            ymd = date.getFullYear() + "/" + ("0"+(date.getMonth()+1)).slice(-2) + "/" + ("0"+date.getDate()).slice(-2);
            day = new Date(ymd).getDay();
    
            // if ($.inArray(ymd, unavailableDates) < 0 && $.inArray(days[day], unavailableDays) < 0) {
            console.log('checking with dates');
            if ($.inArray(ymd, dinnerUnavailableDates) < 0 ) {
                return [true, "enabled", "Book Now"];
            } else {
                return [false,"disabled","Booked Out"];
            }
        }
        var date=new Date();
        $('#sunsetdinerDateText').datepicker({
            onChangeMonthYear: function(year, month, inst){
                $("#transparentLoader").modal('show');
                $(".modal-backdrop,.modal-backdrop.fade.in").css("opacity",0); 
                changedate(month,year,"dinner",44);
            },
            onSelect:function(){
                var dateObject = $(this).datepicker('getDate');
              
                $(".book-now").hide();
                $(this).parent().parent().find(".book-now").show();
            },
            beforeShowDay: dinerUnavailable 
        }).on('changeDate', function(ev) {
            $(".book-now").show();
        });
      
        var date=new Date();
        $('#sundayBrunchDateText').datepicker({ 
            onChangeMonthYear: function(year, month, inst){
                $("#transparentLoader").modal('show');
                $(".modal-backdrop,.modal-backdrop.fade.in").css("opacity",0); 
                changedate(month,year,"brunch",46);
            },
            onSelect:function(){
                var dateObject = $(this).datepicker('getDate');
             
                $(".book-now").hide();
                $(this).parent().parent().find(".book-now").show();
            },
            beforeShowDay: brunchUnavailable
        }).on('changeDate', function(ev) {
            $(".book-now").show();
        });
      
        $('#sundayBrunchDateText').on('blur', function(ev) {
            if($('#sundayBrunchDateText').val()!=""){
                $(".book-now").hide();
            }
        });

        $('#sunsetdinerDateText').datepicker().on('blur', function(ev) {
            if($('#sundayBrunchDateText').val()==="" && $('#sunsetdinerDateText').val()===""){
                $(".book-now").hide();
            }
        });
       
        $('.responsiveMobile select').change(function() {
            window.location.href='<?php echo base_path() ?>' + $(this).val();
        });
        // To do active Tab
        $(".responsiveWeb li").removeClass("active");
        $('.connect').addClass("active");

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

            <ul class="specialoffers-list">

                <li>
                    <div class="connectSpecialoffer" id="contentboxFirstout">
                        <div class="connectSpecialofferInner">

                            <div class="row-fluid">
                                <div class="span5">
                                    <div class="line"></div>
                                </div>
                                <div class="span2">
                                    <img src="<?php echo base_path() . path_to_theme(); ?>/images/banner-icon-img.png">
                                </div>
                                <div class="span5">
                                    <div class="line"></div>
                                </div>
                            </div>
                            <div class="row-fluid">
                                <div class="span12">
                                    <div class="offerTitle"> Private Desk charter special</div>
                                    <div class="offerDescription">Beard in lo-fi, raw denim ea fugiat organic consectetur. Gentrify duis proident.</div>
                                    <a href="#myModalrequest" role="button" data-toggle="modal"> <div class="offerRequest">Request a Proposal</div></a>
                                    <div class="line"></div>
                                </div>
                            </div>


                        </div>
                    </div>
                    <!--<div class="sliderBox"><div class="sliderBoxout" style="background:url('<?php echo base_path() . path_to_theme(); ?>/images/special_offer.jpg');"><div class="event-bgimage"></div><div class="sliderBoxInner"><div class="title">Private Desk charter special</div></div></div><div class="sliderBoxover" style="display: none;"><div class="sliderBox-first-inner"><img src="<?php echo base_path(); ?>sites/all/themes/waterways/assets/img/innerbox-top-img.png"><br><br>Beard in lo-fi, raw denim ea fugiat organic consectetur. Gentrify duis proident.<br><a href="">Requset a Proposal</a><div class="line"></div></div></div></div>-->
                </li>
                <li>
                    <div class="connectSpecialoffer" id="contentboxFirstout">
                        <div class="connectSpecialofferInner">
                            <div class="row-fluid">
                                <div class="span5">
                                    <div class="line"></div>
                                </div>
                                <div class="span2">
                                    <img src="<?php echo base_path() . path_to_theme(); ?>/images/banner-icon-img.png">
                                </div>
                                <div class="span5">
                                    <div class="line"></div>
                                </div>
                            </div>
                            <div class="row-fluid">
                                <div class="span12">
                                    <div class="offerTitle"> Rehearsal Dinner special </div>
                                    <div class="offerDescription">Beard in lo-fi, raw denim ea fugiat organic consectetur. Gentrify duis proident.</div>
                                    <a href="#myModalrequest" role="button" data-toggle="modal"> <div class="offerRequest">Request a Proposal</div></a>
                                    <div class="line"></div>
                                </div>
                            </div>
                        </div>
                    </div>
                    <!--<div class="sliderBox"><div class="sliderBoxout" style="background:url('<?php echo base_path() . path_to_theme(); ?>/images/special_offer.jpg');"><div class="event-bgimage"></div><div class="sliderBoxInner"><div class="title">Rehearsal Dinner special</div></div></div><div class="sliderBoxover" style="display: none;"><div class="sliderBox-first-inner"><img src="<?php echo base_path(); ?>sites/all/themes/waterways/assets/img/innerbox-top-img.png"><br><br>Beard in lo-fi, raw denim ea fugiat organic consectetur. Gentrify duis proident.<br><a href="">Requset a Proposal</a><div class="line"></div></div></div></div>-->
                </li>

            </ul>
        </div>
        <div class="row-fluid">
            <div class="redeemtext">
                <h3>Redeem your Voucher</h3>
                <p>
                    To redeem your SUNDAY BRUNCH CRUISE or SUNSET DINNER CRUISE vouchers, click on a button below, select your preferred date for the cruise and get started!
                </p>
            </div>
        </div>

        <div class="row-fluid voucherList">

            <div class="span4">
                <h3>Sunday Brunch Voucher</h3>
                <span>Choose Date</span>
                <div id="sundayBrunchDate" class="input-append date">

                    <input data-format="dd/MM/yyyy" class="span12" type="text" id="sundayBrunchDateText" placeholder="choose date">
                    <span class="add-on">
                        <i class="icon-calendar"></i>
                    </span>
                </div>
                <div class="booknow-content">
                <div class="row-fluid book-now vocher-booknow" align="center">


                    <div class="span3 sunsetLinePadding">
                        <div class="line"></div>
                    </div>
                    <div class="span6">
                        <!-- booknow start here -->
                        <div class="booknow">
                            <a href="#">
                                <div class="booknow-inner">Book Now!</div>
                            </a>
                        </div>
                        <!-- booknow end here -->
                    </div>
                    <div class="span3 sunsetLinePadding">
                        <div class="line"></div>
                    </div>

                </div>

            </div>  
            </div>  

            <div class="span4 rightVoucherDate">
                <h3>Sunset Dinner Voucher</h3>
                <span>Choose Date</span>
                <div id="sunsetdinerDate" class="input-append date">

                    <input data-format="dd/MM/yyyy" class="span12" type="text" id="sunsetdinerDateText" placeholder="choose date">
                    <span class="add-on">
                        <i class="icon-calendar"></i>
                    </span>
                </div>
                <div class="booknow-content">
                <div class="row-fluid book-now vocher-booknow" align="center">


                    <div class="span3 sunsetLinePadding">
                        <div class="line"></div>
                    </div>
                    <div class="span6">
                        <!-- booknow start here -->
                        <div class="booknow">
                            <a href="#">
                                <div class="booknow-inner">Book Now!</div>
                            </a>
                        </div>
                        <!-- booknow end here -->
                    </div>
                    <div class="span3 sunsetLinePadding">
                        <div class="line"></div>
                    </div>

                </div>
                </div>

            </div>


        </div>



        <div class="row-fluid giftcardHolder">
            <div class="span6">
                <h3>Gift cards</h3>
                <p>
                    Echo park iphone godard retro, ugh id neutra fingerstache occoest Exercitation flannel dolor,echo park portland messenger bag deserunt eu selfies beard plaid sketeboard. Truffaunt cray in, elit polaroid keffiyeh PBR yr typewriter dolor portland pintrest. Lomo godard nesciunt laboris synth portland. 
                </p>
                <div class="row-fluid purchase">

                    <a href="#">PURCHASE GIFT CARD! or CALL (206) 223-2060 </a>
                    <br><br>
                    <div class="span5 linepadding"><div class="line"></div></div>
                    <div class="span2" align="center"><img src="<?php echo base_path() . path_to_theme(); ?>/images/banner-icon-img.png"></div>
                    <div class="span5 linepadding"><div class="line"></div></div>

                </div>
            </div>
            <div class="span6 specialofferImg responsiveWeb">
                <img src="<?php echo base_path() . path_to_theme(); ?>/images/special_offer.jpg "/>
            </div>
        </div>
    </div>

</div>
<!-- row-fluid end -->
