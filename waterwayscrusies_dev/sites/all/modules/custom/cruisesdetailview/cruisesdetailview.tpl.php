<?php
/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
//print base_path();
//print drupal_get_path('module', 'cruisesdetailview');
//var_dump($nodeid);

$selectedportId = isset($_GET['portid']) ? $_GET['portid'] : '';
$selectedtime = isset($_GET['eventdate']) ? $_GET['eventdate'] : '';
$selectedevent = isset($_GET['eventid']) ? $_GET['eventid'] : '';
?>

<!--<script type="text/javascript" src="<?php echo base_path() . path_to_theme() ?>/js/sharebuttons.js"></script>-->
<script type="text/javascript" src="<?php echo base_path() . path_to_theme() ?>/js/buttons.js"></script>
<!--Map Script file gets load here-->
<link href="<?php print base_path() . path_to_theme() ?>/map/css/map.css" rel="stylesheet" type="text/css" />
<script type="text/javascript" src="<?php print base_path() . path_to_theme() ?>/map/gmap3.js"></script>
<!--Maps Script ends here-->

<script src="<?php echo base_path() . path_to_theme() ?>/js/bootstrap-tab.js"></script>
<script src="<?php echo base_path() . path_to_theme() ?>/js/jquery.bxslider.js"></script>
<link rel="stylesheet" href="<?php echo base_path() . path_to_theme() ?>/css/jquery.bxslider.css" type="text/css" />

<link rel="stylesheet" href="<?php echo base_path() . path_to_theme() ?>/magnific-popup/magnific-popup.css" type="text/css" />

<script src="<?php echo base_path() . path_to_theme() ?>/magnific-popup/jquery.magnific-popup.js"></script>

<script>

    function detectmob() {
        if( navigator.userAgent.match(/Android/i)
                || navigator.userAgent.match(/iPhone/i)
                || navigator.userAgent.match(/iPad/i)
                || navigator.userAgent.match(/iPod/i)
                || navigator.userAgent.match(/BlackBerry/i)
                || navigator.userAgent.match(/Windows Phone/i)
    ){
            return true;
        }
        else {
            return false;
        }
    }
    
    function setHeightOfPopup(isClick){
        // Announce the new orientation number
        //  alert(window.orientation);
   
        if (detectmob()) {
            /* if ((window.orientation == 0) || (window.orientation == 180)) // Portrait
             {*/
            var height=window.innerHeight;
            if(navigator.userAgent.match(/Android/i) && isClick){
                height=window.innerWidth;
            }
            if(height-100 < 500){
                $('.reoutemapDirectionsbox').css("max-height",height-100);
            }
            else {
                $('.reoutemapDirectionsbox').removeAttr("style");
            }
            // }

            /*   else if ((window.orientation == 90) || (window.orientation == -90))
             {
             //            alert("noportrait");
             }*/
        }
        else {
            if(window.innerHeight-200 < 500){
                $('.reoutemapDirectionsbox').css("max-height",window.innerHeight-200);
            }
            else {
                $('.reoutemapDirectionsbox').removeAttr("style");
            }
        }
    }
    $(window).resize(function(){
        if (!detectmob()) {
            setHeightOfPopup(false);
        }
    });
    
    
    window.addEventListener("orientationchange", function() {
         if (detectmob()) {
       setHeightOfPopup(true);
         }
    }, false);


    var dirbasepath = Drupal.settings.basePath;


    /*  based on get params we will auto fill the selections    */
    var selectedportId='<?php echo $selectedportId; ?>';
    var selectedtime='<?php echo $selectedtime; ?>';
    var selectedevent='<?php echo $selectedevent; ?>';



    var departureDetails = '';
    var rightSliderimages = '';
    var middleSliderimages = '';
    var menudata = '';
    var eventdates = '';
    var mindates = 0;
    var eventdatestatus = true;
    var setDate = '';
    stLight.options({
        publisher: 'dr-52e8796a-b9e2-6a17-d82d-251b74277418',
        tracking: 'google',
        embeds: 'true',
        onhover: false
    });


    /*  required for sharethis multiple contents  starts  */
    var switchTo5x=true;
    var shareurl=$(location).attr('href');
    var sharetitle= $('#sharetitle').text();
    var sharedesc= $('#sharedesc').children('p').text();
    stWidget.addEntry({
        "service":"sharethis",
        "element":document.getElementById('shareBtnhdn'),
        "url":shareurl,
        "title":sharetitle,
        "type":"large",
//                  "text":"ShareThis" ,
//                  "image":"http://icons.iconarchive.com/icons/iconshock/high-detail-social/256/sharethis-icon.png",
        "summary": sharedesc
    });

    $(function() {

        var unavailableDates = new Array();
        $("#rightsliderImages div:first-child").addClass("active");
        //format::[20131011,20131025,20131015,20131018]
        unavailableDates =<?php print json_encode($dates); ?>;
//        console.log(unavailableDates);
//      unavailableDates=[20131011,20131025,20131015,20131018];

        if(selectedportId !=''){
            jQuery("#detailsViewPorts").val(selectedportId);
            if(selectedtime !=''){
                var selecteddate =new Date(selectedtime*1000);
            }else{
                var selecteddate =new Date();
            }
            var selectedyear = selecteddate.getFullYear();
            var selectedmonth = selecteddate.getMonth() + 1;
            changedate(selectedmonth, selectedyear, selectedportId, 'onfocus');
        }




//         alert(selectedevent);
//         jQuery("#detailsViewSlots").val(selectedevent);
        //Booking through detailed pages

        jQuery("#detailed-view-book-now").click(function() {



//            if (jQuery("#detailsViewPorts").val() != '' && jQuery("#cruiseDate").val() != '')
            if (jQuery("#detailsViewPorts").val() != '' && jQuery("#cruisecalander").val() != '')
            {
                var portit = jQuery("#detailsViewPorts").val();
                var eventid = jQuery("#detailsViewSlots").val();
//                var cruisedate = jQuery("#cruiseDate").val();
                var cruisedate = jQuery("#cruisecalander").val();
                var cruisenid = '<?php echo $itemid; ?>';


                var dates = cruisedate.split('/');
                //dd/mm/yyyy
                var selecteddate = dates[2] + dates[0] + dates[1];//yyyymmdd
//                console.log(selecteddate);
//                console.log(dates);


                //getting the values and rdirecting them to 
//                 booknow/date/%/item/%
                var bookingurl = Drupal.settings.basePath + 'booknow/date/' + selecteddate + '/item/' + eventid;
//                 console.log(bookingurl);
//                 return false;
                window.location.href = bookingurl;

            }
            else
            {
                alert("please select the port and date fields");
            }
            //checking for above two fields empty or not
        });


        $('#directionsRouteMapPopup').click(function() {
            // $('#directionsModal').modal('show');
            $('#clicktoSeeRouteMapModal').modal('show');
            setHeightOfPopup(false);
        });

        // Click to see Route Map Modal Popup hide function
        $('.clicktoseereoutemapDirectionsclosebutton').click(function() {
            //alert("testclose");
            $('#clicktoSeeRouteMapModal').modal('hide');
        });
        
        
        
        //click to close the  google  map popup hide function
      $('.seereoutemapDirectionsclosebutton').click(function(){
         // alert("close direction map");
          $('#seeparkingDirectionsModal').modal('hide');
          
    });

        /* Click to See Route Map popup tabing script */
        $('#parkingDirectionsPorts a').click(function(e) {

            e.preventDefault();
            $(this).tab('show');
        });






        //portfields on change events
        $("#detailsViewPorts").change(function() {
            //on event fires the event dates get changed in the datepicker
            var currentdate = new Date();
            var currentmonth = currentdate.getMonth() + 1;
            var currentyear = currentdate.getFullYear();
            var portid = $('#detailsViewPorts').val();

            changedate(currentmonth, currentyear, portid, onfocus);

// 					console.log(date);
// 					console.log(month);
// 					console.log(year);




// 					changedate(11,2013);
        });

        //end of portfield change events






        function changedate(month, year, portid, onfocus)
        {

//         	  $currentmonth = $_GET['mon'];
//         	    $currentyear = $_GET['year'];
//         	    $templateid = $_GET['templateid'];
//         	    $portid=$_GET['portid'];

            var url = Drupal.settings.basePath + 'eventsbymonth';
            var params = 'mon=' + month + '&year=' + year + '&portid=' + portid + '&templateid=<?php print $itemid; ?>';

            jQuery.ajax({
                type: "GET",
                cache: false,
                async: true,
                url: url,
                data: params,
                dataType: "json",
                beforeSend: function() {
                    $("#ajaxloadmessage").css('display', 'block');
                    $('#transparentLoader').css('display', 'block');
                },
                error: function(request, error) {
                    //error codes replaces here
                },
                success: function(response, status, req) {
//                    console.log(response);
                    unavailableDates = response;
                    eventdates = response;
//               
                    if(response.length >0){
                        var dString = response[0];
                        dString = dString.toString();
                        var year = dString.substring(0,4);
//                         year = parseInt(year);
                        var month = dString.substring(4,6);
//                         month =parseInt(month);
                        var date = dString.substring(6,8);
//                         date = parseInt(date);
                        var d1 = new Date(year+'-'+month+'-'+date);
//                        var d2 = new Date();
//                        var t2 = d2.getTime();
//                        var t1 = d1.getTime();
//                        mindates= parseInt((t2-t1)/(24*3600*1000));
//                        alert(mindates);
//                          eventdatestatus =false;
                    }
//                    eventdatestatus=true;
//                    $("#cruiseDate").datepicker("refresh");
                    $("#cruisecalander").datepicker("refresh");

                    if(selectedtime !='' && selectedportId==portid){
                        var selecteddate =new Date(selectedtime*1000);
//                        alert(selecteddate);
                        $("#cruisecalander").datepicker("setDate", selecteddate);
                    }else{
                        $("#cruisecalander").datepicker("setDate", d1);
                    }

                    var selecteddate = jQuery("#cruisecalander").val();
                    var params = 'selecteddate=' + selecteddate + '&portid=' + portid + '&templateid=<?php print $itemid; ?>';
                    gettimeoptions(params);
                    $("#transparentLoader").modal('hide');
                    $(".modal-backdrop,.modal-backdrop.fade.in").css("opacity", 0.8);
                },
                complete: function() {
                    $("#ajaxloadmessage").css('display', 'none');
                    $('#transparentLoader').css('display', 'none');
                }
            });
        }


        //  $('#cruiseDate').datepicker({beforeShow: chec});
        $('#cruisecalander').datepicker({
            beforeShowDay: function(date) {
                var ymd = date.getFullYear() + "" + ("0" + (date.getMonth() + 1)).slice(-2) + "" + ("0" + date.getDate()).slice(-2);
                if (eventdates != '') {
                    if ($.inArray(parseInt(ymd), eventdates) < 0) {
                        return [false, "disabled", "Booked Out"];
                    } else {
                        return [true, "disabled", "Booked In"];
                    }
                } else {
                    return [false, "", "unAvailable"];
                }
            },
            onSelect: function(date) {
                // Your CSS changes, just in case you still need them
                var portid = $('#detailsViewPorts').val();
                //time slots ajax calls

                var params = 'selecteddate=' + date + '&portid=' + portid + '&templateid=<?php print $itemid; ?>';
                gettimeoptions(params);

                //end of time slot ajax calls
            }
        });


        function gettimeoptions(params){
            var url = Drupal.settings.basePath + 'timeslots';
            jQuery.ajax({
                type: "GET",
                cache: false,
                async: true,
                url: url,
                data: params,
                dataType: "json",
                beforeSend: function() {
                    $('#transparentLoader').css('display', 'block');
                },
                error: function(request, error) {
                    //error codes replaces here
                },
                success: function(response, status, req) {
                    var optionshtml = '<option>Choose Time</option>';
                    $.each(response, function(i, item)
                    {
                        if(selectedevent == i){
                            optionshtml += '<option selected="selected" value=' + i + '>' + item + '</option>';
                        }else{
                            optionshtml += '<option value=' + i + '>' + item + '</option>';
                        }

                    });
                    $('#detailsViewSlots').html(optionshtml);
                },
                complete: function() {
                    $('#transparentLoader').css('display', 'none');
                }
            });
        }
//        $('#cruiseDate').datepicker({
//            onChangeMonthYear: function(year, month, inst) {
//                console.log(year);
//                console.log(month);
//                changedate(month, year);
//                $("#transparentLoader").modal('show');
//                $(".modal-backdrop,.modal-backdrop.fade.in").css("opacity", 0);
//            },
//            beforeShowDay: unavailable,
//            onSelect: function(date) {
//                // Your CSS changes, just in case you still need them
//                console.log(date);
//
//                var portid = $('#detailsViewPorts').val();
//                //time slots ajax calls
//                var url = Drupal.settings.basePath + 'timeslots';
//                var params = 'selecteddate=' + date + '&portid=' + portid + '&templateid=<?php print $itemid; ?>';
//
//                jQuery.ajax({
//                    type: "GET",
//                    cache: false,
//                    async: true,
//                    url: url,
//                    data: params,
//                    dataType: "json",
//                    beforeSend: function() {
//                        $("#ajaxloadmessage").css('display', 'block');
//                    },
//                    error: function(request, error) {
//                        //error codes replaces here
//                    },
//                    success: function(response, status, req) {
//                        console.log(response);
//                        var optionshtml = '';
//                        $.each(response, function(i, item)
//                        {
//
//                            console.log(i);
//                            console.log(item);
//
//                            optionshtml += '<option value=' + i + '>' + item + '</option>';
//                        });
//
//                        $('#detailsViewSlots').append(optionshtml);
//                    },
//                    complete: function() {
//                        $("#ajaxloadmessage").css('display', 'none');
//                    }
//                });
//                //end of time slot ajax calls
//
//
////               detailsViewSlots
//            }
//        });
//        $('#cruiseDate').focus(function(){
//            unavailableDates=<?php print json_encode($dates); ?>;
//            $("#cruiseDate").datepicker( "refresh" );
//        });

        var days = ["Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday"];
//        var unavailableDates = ["2013/09/26","2012/03/27","2012/04/05"]; // yyyy/MM/dd
        var unavailableDays = ["Saturday", "Sunday"];
        function unavailable(date) {


            ymd = date.getFullYear() + "" + ("0" + (date.getMonth() + 1)).slice(-2) + "" + ("0" + date.getDate()).slice(-2);
            day = new Date(ymd).getDay();
//            console.log(unavailableDates)

            if ($.inArray(parseInt(ymd), unavailableDates) < 0) {
                return [false, "disabled", "Booked Out"];

            } else {
//                console.log('enabled')
                return [true, "enabled", "Book Now"];
            }
        }


        $("#ui-datepicker-div").hide();
        $("#shareBtn").on("click", function() {
//            $("#stwrapper").show();
            $( "#shareBtnhdn").trigger( "click" );
        });
        var landingSlider=$('.dinnerCruiseMenu').bxSlider({
            infiniteLoop: false,
            hideControlOnEnd: false,
            minSlides: 1,
            maxSlides: 3,
            slideWidth: 350,
            slideHeight: 300,
            slideMargin: -30,
            touchEnabled: true,
            pager: false
        });
        window.addEventListener("resize", function() {
            var windowsize = $(window).width();
            if (windowsize <= 768) {
            if(landingSlider){
				 landingSlider.reloadSlider();
			}
            } 

            setTimeout(function(){
                $('.dinnerCruiseMenu').css('-webkit-transform', 'translate3d(0px, 0px, 0px)');
                },50);
        }, false);

        
        $('.fleetbxslider').magnificPopup({
            delegate: 'a',
            closeMarkup: '<button title="Close (Esc)" type="button" class="mfp-close"><div class="mfp-close close-text">Close</div></button>',
            // child items selector, by clicking on it popup will open
            gallery: {
                enabled: true
            }

        });


        $('.fleetbxslider').bxSlider({
            infiniteLoop: false,
            hideControlOnEnd: false,
            minSlides: 1,
            maxSlides: 5,
            slideWidth: 300,
            slideHeight: 250,
            slideMargin: 10,
            touchEnabled: true
        });


        /*Carousel Indicators gets starting*/
        $("#routmapSlider div:first-child").addClass("active");
        $("#directionsContentRender div:first-child").show();


        if ($("#routmapSlider").children().length > 1) {
            $("#routmapSlider .item").each(function(index) {
                if (index == 0) {
                    var indicators = '<li><div class="silderIndicator active" id="middleind' + index + '"></div> </li>';
                } else {
                    var indicators = '<li><div class="silderIndicator" id="middleind' + index + '"></div> </li>';
                }
                $(".customIndicatorsMiddle").append(indicators);
            });
        }
        $("#rightsliderImages div:first-child").addClass("active");
        if ($("#rightsliderImages").children().length > 1) {
            $("#rightsliderImages .item").each(function(index) {
                if (index == 0) {
                    var indicators = '<li><div class="silderIndicatorRight active" id="rightind' + index + '"></div> </li>';
                } else {
                    var indicators = '<li><div class="silderIndicatorRight" id="rightind' + index + '"></div> </li>';
                }
                $(".customIndicatorsRight").append(indicators);
            });
        }

        $(document).on('click', '.silderIndicator', function() {
            var indicatorid = parseInt(this.id.slice(9));
            $('#mydownCarousel').carousel('pause');
            $('#mydownCarousel').carousel(indicatorid);
        });
        $("#middlecaroselnext").click(function(e) {
            $('#mydownCarousel').carousel('next');
            $("#mydownCarousel").carousel("cycle");
            e.preventDefault();
        });
        $("#middlecaroselprev").click(function(e) {
            $('#mydownCarousel').carousel('prev');
            $("#mydownCarousel").carousel("cycle");
            e.preventDefault();
        });


        $(document).on('click', '.silderIndicatorRight', function() {
            var indicatorid = parseInt(this.id.slice(8));
            $('#myCarousel').carousel('pause');
            $('#myCarousel').carousel(indicatorid);
        });
        $("#rightcaroselnext").click(function(e) {
            $('#myCarousel').carousel('next');
            $("#myCarousel").carousel("cycle");
            e.preventDefault();
        });
        $("#rightcaroselprev").click(function(e) {
            $('#myCarousel').carousel('prev');
            $("#myCarousel").carousel("cycle");
            e.preventDefault();
        });
        $('#myCarousel').carousel({
            interval: 8000
        }).on('slide', function(e) {
            var mainSlider = parseInt($('#myCarousel .active').index('#myCarousel .item'));
            var slideFrom = $(this).find('.active').index();
            var slideTo = $(e.relatedTarget).index();
            var nextslide = mainSlider + 1;
            var nextindicatorname = '#rightind' + slideTo;
            $(".customIndicatorsRight li div").removeClass("active");
            $(nextindicatorname).addClass("active");


        });

        // parkingDirectionsModal hide function
        $('.reoutemapDirectionsclosebutton').click(function() {
            $('#parkingDirectionsModal').modal('hide');
            $('#directionsModal').modal('hide');
        });



        // To do active Tab
        $(".responsiveWeb li").removeClass("active");
        $('.firstLink').addClass("active");

        //16 Dec 2013  Parking Directions
        $('#parkingDirectionsPopup').click(function() {
            //Selected Port ID
            var portid = $('#portidval').val();
            var url = Drupal.settings.basePath + 'getportdetails';
            var params = 'portid=' + portid;

            jQuery.ajax({
                type: "GET",
                cache: false,
                async: true,
                url: url,
                data: params,
                dataType: "json",
                beforeSend: function() {
                    $('#portListDiv').html('');
                    $('#portContentRender').html('');
                    $('#parkingDirectionLoader').show();
                },
                error: function(request, error) {
                    //error codes replaces here
                },
                success: function(response, status, req) {
                    var ldata;
                    var rdata;

                    try {
                        var ldata = '<ul>';
                        ldata += "<li id='" + portid + "'><a>" + response.portname + "</a></li>";
                        ldata += '</ul>';
                        var rdata = "<img src='" + response.parkingimg + "'>";
                    } catch (exception) {
                        var message = exception.message;
//                        console.log(message);
                    }
                    $('#portListDiv').html(ldata);
                    $('#portContentRender').html(rdata);
                },
                complete: function() {
                    $('#parkingDirectionLoader').hide();
                    $('#parkingDirectionsModal').modal('show');
                }
            });
        });

        function popupcontent(){
            
       var portid = $('#portidval').val();
       var url = Drupal.settings.basePath + 'getportdetails';
       var params = 'portid=' + portid;
       
        
        jQuery.ajax({
              type: "GET",
                cache: false,
                async: true,
                url: url,
                data: params,
                dataType: "json",
                 beforeSend: function() {
                    $('#directionpopup').show();
                    $('#directionpopupcontent').html('');
                },
             success: function(response, status, req) {
             console.log(response);
                   var res = '';
                   try {
                       res += response.portdescription;
                  } catch (exception) {
                        var message = exception.message;
                       console.log(message);
                   }
                  $('#directionpopupcontent').html(res);
            },
            complete: function() {
                    $('#directionpopup').hide();
                   //$('#directionsModal').modal('show');
                }
                     
                     
            });
            
    }

//        //16 Dec 2013 
//        $('#directionsPopup').click(function() {
//            var portid = $('#portidval').val();
//            var url = Drupal.settings.basePath + 'getportdetails';
//            var params = 'portid=' + portid;
//
//            jQuery.ajax({
//                type: "GET",
//                cache: false,
//                async: true,
//                url: url,
//                data: params,
//                dataType: "json",
//                beforeSend: function() {
//                    $('#directionLoader').show();
//                    $('#directionItem').html('');
//                },
//                error: function(request, error) {
//                    //error codes replaces here
//                },
//                success: function(response, status, req) {
//
////                    var res = '';
////                    try {
////                        res += "<h4>" + response.portname + "</h4>";
////                        res += response.field_directions;
////                        res += "<img src='" + response.parkingimg + "'>";
////                    } catch (exception) {
////                        var message = exception.message;
//////                        console.log(message);
////                    }
////                    $('#directionItem').html(res);
//                },
//                complete: function() {
//                    $('#directionLoader').hide();
//                    $('#directionsModal').modal('show');
//                }
//            });
//
//            $('#seeparkingDirectionsModal').modal('show');
//            google.maps.event.trigger($("#getdirections_map_canvas")[0], 'resize');
//
//        });
///comment for direction port manually


  $('#directionsPopup').click(function() {
            var portid = $('#portidval').val();
            var url = Drupal.settings.basePath + 'getportdetails';
            var params = 'portid=' + portid;

            jQuery.ajax({
                type: "GET",
                cache: false,
                async: true,
                url: url,
                data: params,
                dataType: "json",
                beforeSend: function() {
                    $('#directionLoader').show();
                    $('#directionItem').html('');
                },
                error: function(request, error) {
                    //error codes replaces here
                },
                success: function(response, status, req) {
                    try {
        $('.form-item-to #edit-to').val(response.portname);
        $('.form-item-to #edit-to').attr('readonly');
        $('#seeparkingDirectionsModal').modal('show');
        $('.form-item-to #edit-to').attr('readonly', true);
         google.maps.event.trigger($("#getdirections_map_canvas")[0], 'resize');
         $('#directionLoader').hide();
                    } catch (exception) {
                        var message = exception.message;
                        console.log(message);
                    }
//                    $('#directionItem').html(res);
                },
                complete: function() {
            $('#directionLoader').hide();
                }
            });
   
        });




         $('#parkingDirectionsPorts li').click(function(el) {
             var id=$(this).attr('id');
             $(".direction-details").removeClass("active");
             $("#directionItem"+id).addClass("active");
         });


        /* Arrow scroll bottom script */
        $("#downArrow").click(function() {
            $("html, body").animate({scrollTop: $(".sunsetDownslider").offset().top}, 800);
        });

        /* Arrow scroll top script */
        $("#upArrow").click(function() {
            $("html, body").animate({scrollTop: $("#header").offset().top}, 800);
        });

        /* click to see route map script */
        $("#seeRouteMap").click(function() {
            $('#seeparkingDirectionsModal').modal('show');
            google.maps.event.trigger($("#getdirections_map_canvas")[0], 'resize');
        });

        $('.departurePortsclosebutton').click(function() {
            $(".mapDownsliderbox").hide();
        });

        $("#map_addresses").gmap3({
            map: {
                options: {
                    center: [47.606209, -122.332071],
                    zoom: 11,
                    scrollwheel: false,
                    styles: [
                        {
                            featureType: "all",
                            stylers: [
                                {"hue": "#00b2ff"},
                                {"lightness": 5},
                                {"saturation": -56}
                            ]
                        }
                    ]
                }
            },
            marker: {
                values: <?php print json_encode($mapaddress); ?>,
//          values:[
//      {address:"South Lake Union Park,860 Terry Ave N,Seattle, WA 98109", data:"Poitiers : great city !"},
//      {address:"66000 Perpignan, France", data:"Perpignan ! GO USAP !", }
//    ],



                options: {
                    draggable: false,
                    icon: new google.maps.MarkerImage('<?php echo base_path(); ?>sites/all/themes/waterways/assets/img/google-markar.png')
                },
                events: {
                    click: function(marker, event, context) {
                        //alert(context.data);
//                        console.log(context.data)
                     
                        $(".direction-details").show();
                        $("#parkingDirectionsPorts li").hide();
                        $("#" + context.data).show();
                        $("#directionItem" + context.data).show();
                        $('.mapDownsliderbox').show();
                        $('#portidval').val(context.data);
                         popupcontent();
                        $("#directionsContentRender .directionItem").hide();
                        $("#" + context.data + "directions").show();
                    }

                }
            }
        });

    });
    
    
           function sharethisfn(shareval){
                if(shareval == 'facebook'){
                    $("#fbsharebn").children('span').trigger( "click" );
                }
                if(shareval == 'twitter'){
                    $("#twitsharebn").children('span').trigger( "click" );
                }
                if(shareval == 'share'){
                   $("#sharethisbtn" ).trigger( "click" );
                }
            }

</script>
<?php
$cur_uri = $_SERVER['HTTP_HOST'] . '' . request_uri();
?>


<div id="content">
    <div class="contentDetail">
        <!-- sunset dining start here -->

        <div class="row-fluid cruiseDetailinfo cruisedetailview">
            <div class="span6">
                <div class="detailBackLink">
                    <a href="<?php echo base_path() ?>diningcruises"><img
                            src="<?php echo base_path() . path_to_theme(); ?>/images/back_arrow.png" />
                        BACK TO DINING CRUISES</a>
                </div>
                <div class="row-fluid">
                    <div class="span2"></div>
                    <div class="span10">
                        <!-- here rendering cruise details here -->

                        <div id="cruiseDetailsHolder"><?php print $details; ?></div>

                        <script type="text/javascript">var addthis_config = {"data_track_addressbar": false};</script>
                        <script type="text/javascript"
                        src="//s7.addthis.com/js/300/addthis_widget.js#pubid=ra-51fb54c15c854213"></script>
                        <div class="sunsetBannerBooknow">
                            Book Now!
                            <div class="row-fluid">
                                <div class="span6">
                                    <span>Port</span> 
                                    <select id="detailsViewPorts">
                                        <option>Choose Port</option>
                                        <?php print $portOptions ?>
                                    </select>
                                </div>
                                <div class="span6">
                                    <span>Date</span>
                                    <div id="cruiseBookNowDate" class="input-append date">
                                        <!--<input data-format="dd/MM/yyyy" type="text" id="cruiseDate" style='display:none;' placeholder="Choose Date" />--> 
                                        <input data-format="dd/MM/yyyy" type="text" id="cruisecalander" readonly="readonly" placeholder="Choose Date" /> 
                                        <span class="add-on"> 
                                            <i class="icon-calendar"></i>
                                        </span>
                                    </div>
                                </div>
                            </div>
                            <!-- row-fluid end -->

                            <div class="row-fluid">
                                <div class="span6 responsiveMobile"></div>
                                <div class="span6">
                                    <span>Time</span> <select id="detailsViewSlots">
                                        <option>Choose Time</option>
                                    </select>

                                </div>
                            </div>
                            <!-- row-fluid end -->
                        </div>

                        <div class="row-fluid sunsetBooknowMobile">
                            <div class="span4 sunsetLinePadding">
                                <div class="line"></div>
                            </div>
                            <div class="span4">
                                <!-- booknow start here -->
                                <div class="booknow ">
                                    <div class="booknow-inner" id="detailed-view-book-now">Book
                                        Now!</div>
                                </div>
                                <p id="ajaxdate" style="display:none; font-size:10px;">Please wait loading...</p>
                                <!-- booknow end here -->
                            </div>
                            <div class="span4 sunsetLinePadding">
                                <div class="line"></div>
                            </div>
                        </div>

                        <!-- here rendering cruise description here -->
                        <div class="groupResNote" id="descriptionHolder"><?php print $titledescription; ?></div>

                    </div>
                </div>
                <p id="ajaxdate" style="display:none; font-size:10px;">Please wait loading...</p>
                <!-- row-fluid end -->

            </div>
            <div class="span6">


                <div class="sunset-slider-buttons">
                    <div id="downArrow" class="cruiseDownArrow">
                        <img src="<?php echo base_path() . path_to_theme(); ?>/images/sunset-banner-downarrow.png" />
                    </div>



                    <div id="myCarousel" class="carousel slide">
                        <div class="slider">
                            <ul class="customIndicatorsRight">

                            </ul>
                        </div>


                        <!-- Carousel items -->
                        <div class="carousel-inner" id="rightsliderImages">
                            <?php //print $rightSlider;  ?>
                            <?php
                            if ($rightSlider != "") {
                                print $rightSlider;
                            } else {
                                print '<div class="item"><img src="' . base_path() . 'sites/default/files/default_images/right-slider-image.jpg"></div>';
                            }
                            ?>
                        </div>
                        <!-- Carousel nav -->
                        <a class="carousel-control left" href="#myCarousel"
                           id="rightcaroselprev"><img
                                src="<?php echo base_path() . path_to_theme(); ?>/images/carousol-leftarrow.png" /></a>
                        <a class="carousel-control right" href="#myCarousel"
                           id="rightcaroselnext"><img
                                src="<?php echo base_path() . path_to_theme(); ?>/images/carousol-rightarrow.png" /></a>
                    </div>




                    <div class="booknow share-mobile responsiveMobile">
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
                                    
                                </div>
<!--                        <a>
                            <div class="booknow-inner">
                                <div class="addthis_toolbox addthis_default_style ">
                                    <iframe
                                        src="//www.facebook.com/plugins/like.php?href=https%3A%2F%2Fwww.facebook.com%2Fpages%2FMyartmatch%2F206787809421622&amp;width=450&amp;height=35&amp;colorscheme=light&amp;layout=standard&amp;action=like&amp;show_faces=false&amp;send=false&amp;appId=1378753459016747"
                                        scrolling="no" frameborder="0"
                                        style="border: none; overflow: hidden; width: 58px; height: 29px; float: left; margin-left: 7%; margin-top: 1.5%;"
                                        allowTransparency="true"></iframe>
                                    <div class="twitter-share">
                                        <a href="https://twitter.com/share"
                                           class="twitter-share-button"
                                           data-url="<?php echo $cur_uri; ?>"
                                           data-url="<?php // echo base_path();  ?>/cruisesdetailview/category/<?php print $category; ?>"
                                           data-via="lsnwaterways">Tweet</a>
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
                                    <div class="share-text">Share</div>
                                </div>

                            </div>
                        </a>-->
                    </div>
                    <div class="booknow share responsiveWeb" id="shareBtn">
                     <span class="booknow-inner st_sharethis_custom"

                              displayText="Share"
                              >Share</span>
                        <span id="shareBtnhdn" style="display:none;"></span>

                    </div>

                    <div class="booknow Reviews">
                        <a href="<?php echo base_path() ?>connectreview">
                            <div class="booknow-inner">See Reviews!</div>
                        </a>
                    </div>
                </div>


            </div>
        </div>

        <!-- sunset dining end here -->
    </div>
    <!-- sunset down slider start here -->
    <div class="row-fluid">
        <div class="span12">
            <!-- sunsetDownslider start here -->
            <div class="sunsetDownslider">

                <div id="mydownCarousel" class="carousel slide">
                    <div class="slider">
                        <ul class="customIndicatorsMiddle">

                        </ul>
                    </div>
                    <!-- Carousel items -->
                    <div class="carousel-inner" id="routmapSlider">
                        <?php //print $middleSlider;  ?>
                        <?php
                        if ($middleSlider != "") {
                            print $middleSlider;
                        } else {
                            print '<div class="item"><img src="' . base_path() . 'sites/default/files/default_images/middle-slider-image.jpg"></div>';
                        }
                        ?>
                    </div>
                    <!-- Carousel nav -->
                    <a class="carousel-control left" href="#mydownCarousel"
                       id="middlecaroselprev"><img
                            src="<?php echo base_path() . path_to_theme(); ?>/images/carousol-leftarrow.png" /></a>
                    <a class="carousel-control right" href="#mydownCarousel"
                       id="middlecaroselnext"><img
                            src="<?php echo base_path() . path_to_theme(); ?>/images/carousol-rightarrow.png" /></a>
                </div>


                <div class="sunsetDownsliderbox">
                    <div class="sunsetDownsliderInner">
                        <div class="row-fluid">
                            <div class="span2 sunsetLinePadding">
                                <div class="line"></div>
                            </div>
                            <div class="span8">
                                <div class="sliderCaption" id="sharetitle"><?php print $caption; ?></div>
                            </div>
                            <div class="span2 sunsetLinePadding">
                                <div class="line"></div>
                            </div>
                        </div>

                        <div class="sliderInnercontent" id="sharedesc">
                            <?php print $middleContent; ?>
                            <br> <img
                                src="<?php echo base_path() . path_to_theme(); ?>/images/clickmapicon.png" />
                            <!--<div id="seeRouteMap" class="clickMapLink">CLICK TO SEE ROUTE MAP</div><br>-->
                            <div id="directionsRouteMapPopup" class="clickMapLink">CLICK TO
                                SEE ROUTE MAP</div>
                            <br>
                        </div>

                        <div class="row-fluid" id="routeMaplinesholder">
                            <div class="span5 ">
                                <div class="line"></div>
                            </div>
                            <div class="span2 banner-icon" align="center">
                                <img
                                    src="<?php echo base_path() . path_to_theme(); ?>/images/banner-icon-img.png" />
                            </div>
                            <div class="span5 ">
                                <div class="line"></div>
                            </div>
                        </div>
                    </div>
                    <!-- sunsetDownsliderInner end -->
                </div>
            </div>
            <!-- sunsetDownslider end here -->
        </div>
    </div>
    <!-- sunset down slider end here -->


    <div class="container">

        <!-- here rendering cruise menu content -->
        <div class="dinnermenu" id="cruiseMenuRender">

            <div class="title">
                <?php print $coursemenulabels->menu_lable; ?>
                <div class="pull-right">

                    <?php
                    foreach ($menupdfs as $key => $value) {
                        echo '<img src="' . base_path() . 'sites/default/files/cruisemenu/download-icon.png" /> ';
                        echo ' <a href="' . $value . '" target="_balnk">Download Menu and Specialty Cocktails PDF </a> <br/>';
                    }
                    ?>
    <!-- <img src="<?php //echo //base_path(); ?>sites/default/files/cruisemenu/download-icon.png" /> -->
    <!-- <a href="<?php //print $dinnermenupdfurl;  ?>" target="_balnk">Download Menu and Specialty Cocktails PDF</a> -->
                </div>
            </div>
            <div class="subtitle"><?php print $coursemenulabels->menu_caption; ?></div>

            <?php
            /*             * *
             * Get the menu layout cases
             * case 1:Layout 1:then render the elements in to 
             * single column without applying the bxslider
             * 
             * case 2:Layout 2:then render the elements in to
             * two column without applying bxslider.
             * 
             * case 3:Layout 3:with three column render elements
             * in to three column with applying bxslider.
             * 
             */
//                                var_dump($menulayout);
            if ($menulayout == 1)
                $menuclass = 'singlemenucol';
            if ($menulayout == 2)
                $menuclass = 'twomenucol';
            if ($menulayout == 3)
                $menuclass = 'dinnerCruiseMenu';
            ?>


            <ul class="<?php echo $menuclass; ?>">
                <?php print $menu; ?>
            </ul>

        </div>
        <!-- dinnermenu end here -->

    </div>



    <!-- sunset map view start here -->
    <div class="sunsetMapView">
        <div class="mapOverlay"></div>
        <!--MAP starts here-->
        <div class="mapDownsliderbox">
            <div class="departurePortsclosebutton">
                <div class="departurePortsclosebuttoninner">Close</div>
            </div>
            <div class="sunsetDownsliderInner">
                <div class="row-fluid">
                    <input type="hidden" id="portidval" name="portidval">
                    <div class="span2 sunsetLinePadding linepadding">
                        <div class="line"></div>
                    </div>
                    <div class="span8">
                        <div class="sliderCaption">Departure Ports</div>
                    </div>
                    <div class="span2 sunsetLinePadding linepadding">
                        <div class="line"></div>
                    </div>
                </div>

                <div class="sliderInnercontent">
                    
                     <span class="help-inline" id="directionpopup" style="display: none;">Please Wait..</span>
                     
                     <div class='direction-details' id='directionpopupcontent'></div>
<!--                    Echo park iphone godard retro, ugh id neutra fingerstache occaecat.
                    Exercitation flannel. Truffaut cray in, elit polaroid keffiyeh, PBR
                    yr typewriter dolor portland pinterest. Echo park iphone godard
                    retro, ugh id neutra fingerstache occaecat. Exercitation flannel. <br>-->


                    <div class="row-fluid">
                        <div class="span12">
                            <div id="selectedPort"></div>
                        </div>
                    </div>
                    <div class="row-fluid">
                        <div class="span12">
                            <!-- satya code -->
                            <a href="javascript:void(0)" class="clickMapLink"
                               id="directionsPopup">DIRECTIONS </a></br>
                            <span class="help-inline" id="directionLoader" style="display: none;">Please Wait..</span>
                        </div>
                    </div>
                </div>
                <div class="row-fluid">
                    <div class="span12">
                        <a href="javascript:void(0)" class="clickMapLink"
                           id="parkingDirectionsPopup">SEE PARKING DIRECTIONS</a></br>
                        <span class="help-inline" id="parkingDirectionLoader" style="display: none;">Please Wait..</span>
                    </div>
                </div>

            </div>

            <div class="row-fluid" id="routeMaplinesholder">
                <div class="span5 ">
                    <div class="line"></div>
                </div>
                <div class="span2" align="center">
                    <img
                        src="<?php echo base_path() . path_to_theme(); ?>/images/banner-icon-img.png" />
                </div>
                <div class="span5 ">
                    <div class="line"></div>
                </div>
            </div>
        </div>
        <!-- sunsetDownsliderInner end -->
    </div>

    <div class="tab-pane" id="Map">
        <!--		<div id="upArrow" class="cruiseUpArrow">
                                <img src="<?php echo base_path() . path_to_theme(); ?>/images/sunset-banner-uparrow.png" />
                        </div>-->

        <div id="map_addresses" class="map">
            <p>This will be replaced with the Google Map.</p>
        </div>


    </div>
    <!--Map Ends here-->

    <div class="row-fluid cruiseDownslidermargin" id="hideGalleryPager">
        <ul class="fleetbxslider">
            <?php
//                if($gallery != ""){
//                    print $gallery;
//                } else {
//                    print '<div class="item"><img src="'.base_path().'sites/default/files/default_images/gallery-image.jpg"></div>';
//                }

            if ($gallery != "") {
                print $gallery;
            } else {
                ?>

                <script>
                    $(document).ready(function() {
                        $('#hideGalleryPager').addClass("hideGalleryPagercl");
                    });
                </script>

                <?php
            }
            ?>
            <?php //print $gallery ?>
        </ul>

        <div id="upArrow" class="cruiseUpArrow">
            <img src="<?php echo base_path() . path_to_theme(); ?>/images/sunset-banner-uparrow.png" />
        </div>
    </div>


</div>
<!-- sunset map view start here -->




</div>


<!-- Route Map Modal PopUp starts from here -->
<div class="modal hide" tabindex="-1" role="dialog" aria-hidden="true" id="parkingDirectionsModal">
    <div class="reoutemapDirectionsbox">
        <div class="reoutemapDirectionsboxInner">

            <div class="row-fluid">
                <div class="span4 reoutemapDirectionsboxInnerleft">
                    <div class="row-fluid" id="routeMaplinesholder">
                        <div class="span5 linepadding">
                            <div class="line"></div>
                        </div>
                        <div class="span2" align="center">
                            <img
                                src="<?php echo base_path() . path_to_theme(); ?>/images/banner-icon-img.png" />
                        </div>
                        <div class="span5 linepadding">
                            <div class="line"></div>
                        </div>
                    </div>
                    <div class="row-fluid">
                        <div class="span12">
                            <div class="sliderCaption">Parking Directions</div>
                        </div>
                    </div>
                    <div class="row-fluid">
                        <!-- satya code starts here -->
                        <div class="span12" id="portListDiv">
                            <!--                             <ul id="parkingDirectionsPorts"> -->
                            <?php //echo print $portList   ?>


                            <!--                             </ul> -->
                        </div>
                    </div>
                </div>
                <div class="span8 reoutemapDirectionsboxInnerright">
                    <!--close button for modal popup-->
                    <div class="reoutemapDirectionsclosebutton">
                        <div class="reoutemapDirectionsclosebuttoninner">Close</div>
                    </div>
                    <!-- end of close button for modal popup-->
                    <div class="row-fluid">
                        <div class="span12 routemapContent" id="portContentRender">
                            <?php //print $portDirections   ?>
                        </div>
                    </div>
                </div>
            </div>



        </div>
        <!-- sunsetDownsliderInner end -->
    </div>
</div>
<!-- Route Map Modal PopUp ends here -->








<!-- Route Map Modal PopUp starts from here -->
<div class="modal hide" tabindex="-1" role="dialog" aria-hidden="true" id="clicktoSeeRouteMapModal">
    <div class="reoutemapDirectionsbox">
        <div class="reoutemapDirectionsboxInner">

            <div class="row-fluid">
                <div class="span4 reoutemapDirectionsboxInnerleft">
                    <div class="row-fluid" id="routeMaplinesholder">
                        <div class="span5 linepadding">
                            <div class="line"></div>
                        </div>
                        <div class="span2" align="center">
                            <img
                                src="<?php echo base_path() . path_to_theme(); ?>/images/banner-icon-img.png" />
                        </div>
                        <div class="span5 linepadding">
                            <div class="line"></div>
                        </div>
                    </div>
                    <div class="row-fluid">
                        <div class="span12">
                            <div class="sliderCaption">Ports</div>
                        </div>
                    </div>
                    <div class="row-fluid">
                        <!-- satya code starts here -->
                        <div class="span12" id="portListDiv">
                            <ul id="parkingDirectionsPorts" class="nav nav-tabs">
                                <?php print $PORTSLIST; ?>
                            </ul>
                        </div>
                    </div>
                </div>
                <div class="span8 reoutemapDirectionsboxInnerright">
                    <!--close button for modal popup-->
                    <div class="clicktoseereoutemapDirectionsclosebutton">
                        <div class="clicktoseereoutemapDirectionsclosebuttoninner">Close</div>
                    </div>
                    <!-- end of close button for modal popup-->
                    <div class="row-fluid">
                        <div class="span12 routemapContent" id="portContentRender">		
                            <ul id="parkingDirectionsPorts" class="nav nav-tabs">
                                <?php //print $PORTSLIST;  ?>
                            </ul>					
                            <div class="tab-content">
                                <?php print $DIRECTIONS ?>
                            </div>
                        </div>
                    </div>
                </div>
            </div>



        </div>
        <!-- sunsetDownsliderInner end -->
    </div>
</div>
<!-- Route Map Modal PopUp ends here -->

<!-- Route Map Modal PopUp starts from here -->
<div class="modal hide" tabindex="-1" role="dialog" aria-hidden="true" id="directionsModal">
    <div class="reoutemapDirectionsbox">
        <div class="reoutemapDirectionsboxInner">

            <div class="row-fluid">

                <div class="span12 reoutemapDirectionsboxInnerright">
                    <!--close button for modal popup-->
                    <div class="reoutemapDirectionsclosebutton">
                        <div class="reoutemapDirectionsclosebuttoninner">Close</div>
                    </div>
                    <!-- end of close button for modal popup-->

                    <div class="row-fluid">
                        <div class="span12 routemapContent" id="directionsContentRender">
                            <div class="row-fluid directionsContentHeader">
                                <div class="span4 sunsetLinePadding">
                                    <div class="line"></div>
                                </div>
                                <div class="span4">
                                    <div class="sliderCaption">Directions</div>
                                </div>
                                <div class="span4 sunsetLinePadding">
                                    <div class="line"></div>
                                </div>
                            </div>


                            <div class='direction-details' id='directionItem'></div>
                            <?php //print $directions;   ?>
                            <?php print $portDirections; ?>
                        </div>
                    </div>
                </div>
            </div>



        </div>
        <!-- sunsetDownsliderInner end -->
    </div>
</div>
<!-- Route Map Modal PopUp ends here -->


<!-- See Route Map Modal PopUp starts from here -->
<div class="modal hide" tabindex="-1" role="dialog" aria-hidden="true"
     id="seeparkingDirectionsModal">
    <div class="modal-body reoutemapDirectionsbox " >
        <!--close button for modal popup-->
        <div class="seereoutemapDirectionsclosebutton">
            <div class="seereoutemapDirectionsclosebuttoninner">Close</div>
        </div>
        <!-- end of close button for modal popup-->
        <?php print getdirections_direction(); ?>
    </div>
</div>
<!-- See Route Map Modal PopUp ends here -->
