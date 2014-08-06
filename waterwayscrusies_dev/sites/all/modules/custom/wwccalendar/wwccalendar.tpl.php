<?php
/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
?>

<script>
$(document).ready(function () {
window.onresize=function(){
if($(".cruiseKey .popover").hasClass("in")){
$(".cruiseKey .popover").css("left",$("#cruiseKeyPop").position().left-43);
}
};
});

window.addEventListener("orientationchange", function() {
	if($(window).width() >= 768){
		if($(".cruiseKey .popover").hasClass("in")){
			$(".cruiseKey .popover").css("left",$("#cruiseKeyPop").position().left-43);
			}
	}
});
    var currentTime = new Date();
    var month = currentTime.getMonth() + 1;
    var day = currentTime.getDate();
    var year = currentTime.getFullYear();
    var selectedmonth;
    var selectedyear, cruisefilter;
    var loaderleadersObject;
    var portfilter = 'all';
    var htmlcontent='';
    var uniportfilter='';
    var monthNames = ["January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"]; //array of month names.
    function getEventDetails(validdata)
    {
 
        $(".in").each(function() {
            $('#' + this.id).collapse('hide');
        });
        $('#' + validdata).collapse('show');

    }

    function loadcruiseTypes()
    {
        //Loading Available CruiseEvents Types
        var url = Drupal.settings.basePath + 'cruiseservicelisttypes';
        jQuery.ajax({
            type: "GET",
            cache: false,
            async: true,
            url: url,
            data: '',
            dataType: "json",
            beforeSend: function() {

            },
            error: function(request, error) {
                //error codes replaces here
            },
            success: function(response, status, req) {
//                console.log(response);
                $.each(response, function(i, items) {
//                    console.log(i);
//                    console.log(items);
                    $("#cruiseslist").append('<option value=' + i + '>' + items + '</option>');
                });

                //cruise list
//             
            },
            complete: function() {
            }
        });

    }


    function loadparentports()
    {
        var url = Drupal.settings.basePath + 'parentports';
        jQuery.ajax({
            type: "GET",
            cache: false,
            async: true,
            url: url,
            data: '',
            dataType: "json",
            beforeSend: function() {

            },
            error: function(request, error) {
                //error codes replaces here
            },
            success: function(response, status, req) {
                //array of Objects

                //port list
//jQuery.each( response, function( i, items ) {
//   var portsHtml = '<div class="calendarRadio"><div class="portRadioCustom" id="' + i + '"></div> <label onclick="getcalendereventsbyports(' + i + ');">' + items + '</label></div>'
//                    //                        <input type="radio" name="portRadio" class="portRadio" value="'+response.ports[i].cid+'">
//                    $("#ports").append(portsHtml);
//});
                var portsHtml='';
                $.each(response, function(i, items) {

                    portsHtml = '<div class="calendarRadio"><div class="portRadioCustom" id="' + i + '"></div> <label onclick="getcalendereventsbyports(' + i + ');">' + items + '</label></div>'+portsHtml;
                    //                        <input type="radio" name="portRadio" class="portRadio" value="'+response.ports[i].cid+'">
                    
                });
                $("#ports").append(portsHtml);

            },
            complete: function() {
            }
        });
    }


    function getCruiseKeys()
    {
        var url = Drupal.settings.basePath + 'portservice';
        jQuery.ajax({
            type: "GET",
            cache: false,
//            async: true,
            async: false,
            url: url,
            data: '',
            dataType: "json",
            beforeSend: function() {

            },
            error: function(request, error) {
                //error codes replaces here
            },
            success: function(response, status, req) {
                //                console.log(response);
                //array of Objects
                var cruiseKeyHtmlcontent='';
                $.each(response.ports, function(i, items) {                   
                    for (var k = 0; k < items.length; k++) {
                        var cruiseKeyHtml = '<li><div class="port-colors" style="background-color:#' + items[k].color + '"></div> <span> ' + items[k].name + ' </span> </li>';
                        $("#cruiseKeyPopover").append(cruiseKeyHtml);
                        $(".cruiseMobileKey").append(cruiseKeyHtml);
                        cruiseKeyHtmlcontent=cruiseKeyHtmlcontent+cruiseKeyHtml;
                    }
                });
                  htmlcontent='<div id="popover_content_wrapper"><ul id="cruiseKeyPopover">'+cruiseKeyHtmlcontent+'</ul></div>';
                //End of ports list

            },
            complete: function() {
            }
        });
    }
    $(function() {




        $('body').click(function(e) {
            if ($(e.target).attr("id") != "cruiseKeyPop") {
                $("#cruiseKeyPop").popover("hide");
            }

            if ($(".responsiveMobile").css('display') == 'none') {

                if (($(e.target).attr("class") != "events" && $(e.target).attr("class") != "event-port" && $(e.target).attr("class") != "event-cruise") && $('.events').find($(e.target).attr("class")) != -1) {
                    $(".events ul").css({"opacity": 0, "bottom": 0, 'display': 'none'});
                }
            }
        });


        $("#transparentLoader").modal('show');

        $(".responsiveWeb li").removeClass("active");
        $('.wwccalender').addClass("active");
        //calendar reload on data changed



        //Loading Available Cruise Events Types Ends
        loadcruiseTypes();
        loadparentports();
//        getCruiseKeys();
        //Loadind available Ports

        //Cruise Events Filteration functionality on change functionality
        $('#cruiseslist').change(function(el) {
            $("#transparentLoader").modal('show');
            $("#responsiveMobileviewEventslist").html("");
            $(".modal-backdrop,.modal-backdrop.fade.in").css("opacity", 0);
            portfilter = "all";
            cruisefilter = $('#cruiseslist').val();
            $('.portRadioCustom').removeClass("active");
            $('#all').addClass("active");
            uniportfilter='undefined';
            getCalendar(selectedmonth, year, portfilter, cruisefilter);
            
        });
        //End CruiseType change functionality
 
        //port click event based function
        $(document).on('click', '.portRadioCustom', function(e) {
            var portfilter = this.id;
            $('.portRadioCustom').removeClass("active");            
            $(e.currentTarget).addClass("active");
            $("#transparentLoader").modal('show');
            uniportfilter=portfilter;
            $('#cruiseslist').val('all');
            getCalendar(selectedmonth, year, portfilter);
        });

        getCalendar(month, year, portfilter, cruisefilter);
       
        
        $('.CruisecalendarPrevious').click(function() {
            $("#responsiveMobileviewEventslist").html("");
            $("#transparentLoader").modal('show');
            $(".modal-backdrop,.modal-backdrop.fade.in").css("opacity", 0);
            var previousmonth = selectedmonth - 1;
            if (previousmonth > 0)
            {
                var previousyear = selectedyear;
            }
            else
            {
                var previousmonth = 12;
                var previousyear = selectedyear - 1;
            }
            var cruiseslist=$('#cruiseslist').val();            
//            getCalendar(previousmonth, previousyear, portfilter, cruisefilter);
            getCalendar(previousmonth, previousyear, portfilter, cruiseslist);
        });
        $('.CruisecalendarNext').click(function() {

            $("#responsiveMobileviewEventslist").html("");
            $("#transparentLoader").modal('show');
            $(".modal-backdrop,.modal-backdrop.fade.in").css("opacity", 0);
            var nextmonth = selectedmonth + 1;
            if (nextmonth > 12)
            {
                var nextyear = selectedyear + 1;
                var nextmonth = 1;
            }
            else
            {
                var nextmonth = selectedmonth + 1;
                var nextyear = selectedyear;
            }
            var cruiseslist=$('#cruiseslist').val();
//            getCalendar(nextmonth, nextyear, portfilter, cruisefilter);
            getCalendar(nextmonth, nextyear, portfilter, cruiseslist);
        });
    });

function getcalendereventsbyports(portfilter){
            $('.portRadioCustom').removeClass("active");            
            $('#'+portfilter).addClass("active");
            $("#transparentLoader").modal('show');
            uniportfilter=portfilter;
            $('#cruiseslist').val('all');
            getCalendar(selectedmonth, year, portfilter);
        }
    function getCalendar(month, year, portfilter, cruisefilter)
    {
        
        
//        
//        portfilter = $('.portRadioCustom.active').;
//            cruisefilter = $('#cruiseslist').val();
//portRadioCustom active
//if(portfilter ==''){
if(uniportfilter !=''){
    portfilter=uniportfilter;
}
    
//}
//if(cruisefilter==''){
   var  ccruisefilter=$('#cruiseslist').val();
//}
if(ccruisefilter !='all'){
    cruisefilter=ccruisefilter;
}

        
        //loading form cruiseevent module
        var url = Drupal.settings.basePath + 'calenderevents';
        var params = 'mon=' + month + '&year=' + year + '&cruisefilter=' + cruisefilter + '&portfilter=' + portfilter;
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
                //                console.log(response);
                selectedmonth = month;
                selectedyear = year;
                var calendartitle = monthNames[selectedmonth - 1];
                $(".displayMonthName").html(calendartitle);
                $("#cruiseCalendarleftContent").html(response);
                $("#cruiseCalendar").html(response);
                $("#cruiseCalendarrightContent").html(response);
                $("#transparentLoader").modal('hide');
                $(".modal-backdrop,.modal-backdrop.fade.in").css("opacity", 0.8);
            },
            complete: function() {
            }
        });
    }
   $(function ()  
{ 
    
//     $(".custom-select").each(function(){
//            $(this).wrap("<span class='select-wrapper'></span>");
//            $(this).after("<span class='holder'></span>");
//        });
//        $(".custom-select").change(function(){
//            var selectedOption = $(this).find(":selected").text();
//            $(this).next(".holder").text(selectedOption);
//        }).trigger('change');
    
     

    
    getCruiseKeys();
//    var htmlcontent='<div id="popover_content_wrapper"><ul id="cruiseKeyPopover"></ul></div>';
    $("#cruiseKeyPop").popover({html:true,title: '', content: htmlcontent});  
});  
   
</script>  

<div class="calendarHeader">
    <div class="row-fluid">
        <div class="span12">
            <div class="pull-left calendarTitlediv">
                <div class="calendarTitle">
                    CRUISE CALENDAR
                </div>
            </div>
            <div class="pull-left headerFilters portsmiddle">
                <div class="choose">  Choose Port</div>
                <div class="choosePortRadio">
                    <div class="radioLine"></div>
                    <div class="calendarPortRadio">

                        <div id="ports">
                            <div class="calendarRadioall"> 
    <!--                            <input type="radio" name="portRadio" class="portRadio" value="all" checked/> -->
                                <div class="portRadioCustom active" id="all"></div>
                                <label onclick="getcalendereventsbyports('all');">All</label> 
                            </div>
                            <!--append dynamic port list-->
                        </div>
                    </div>

                </div>
            </div>
            <div class="pull-left headerFilters portsright">
                <div class="choose choose-cruise"> Choose Cruise </div>
                <div class="selectbodholder">
                    <!--<select class="span12" id="cruiseslist" on>-->
                    <select id="cruiseslist" on class="custom-select">
                        <!--<option selected="" value="Filter by cruise type">Filter by Cruise Type</option>-->
                        <option value="all">All</option>

                    </select>
                </div>
            </div>
        </div>
<!--        <div class="span6">
            <div class="calendarTitle">
                CRUISE CALENDAR
            </div>
        </div>
        <div class="span3 headerFilters">
            <div class="choose">  Choose Port</div>
            <div class="choosePortRadio">
                <div class="radioLine"></div>
                <div class="calendarPortRadio">

                    <div id="ports">
                        <div class="calendarRadioall"> 
                            <input type="radio" name="portRadio" class="portRadio" value="all" checked/> 
                            <div class="portRadioCustom active" id="all"></div>
                            <label>All</label> 
                        </div>
                        append dynamic port list
                    </div>
                </div>

            </div>
        </div>
        <div class="span3 headerFilters">
            <div class="choose choose-cruise"> Choose Cruise </div>
            <div class="selectbodholder">
                <select class="span12" id="cruiseslist" on>
                <select id="cruiseslist" on>
                    <option selected="" value="Filter by cruise type">Filter by Cruise Type</option>
                    <option value="all">All</option>

                </select>
            </div>
        </div>-->
    </div>
    <div class="row-fluid calendarTitlemargin">
        <div class="span5">
            <div class="calendarLine"></div>
        </div>
        <div class="span2" align="center">  
            <img src="<?php echo base_path(); ?>sites/all/themes/waterways/assets/img/banner-icon-img.png"/>
        </div>
        <div class="span5">
            <div class="calendarLine"></div>
        </div>
    </div>
    <div class="row-fluid responsiveWeb">
        <div class="span12">
            <div id="displayMonthName" class="monthName displayMonthName"></div>
        </div>
    </div>
    <div class="responsiveMobile arrowButtonsMobile">
        <div class="row-fluid">
            <div class="span3" align="right">
                <button id="CruisecalendarPrevious" class="CruisecalendarPrevious"></button>
            </div>
            <div class="span6">
                <div id="displayMonthName" class="monthName displayMonthName"></div>

            </div>
            <div class="span3"><button id="CruisecalendarNext" class="CruisecalendarNext"></button></div>
        </div>
    </div>

</div>

<!--<div class="mainCalendar">
    <div id="cruiseCalendarleft">
        <button id="CruisecalendarPrevious" class="CruisecalendarPrevious responsiveWeb"></button>
        <div id="cruiseCalendarleftContent" style="visibility: hidden;"></div>
    </div>
    <div id="cruiseCalendar"></div>

    <div id="cruiseCalendarright">
        <button id="CruisecalendarNext" class="CruisecalendarNext responsiveWeb"></button>
        <div id="cruiseCalendarrightContent" style="visibility: hidden;"></div>
    </div>-->



<div class="row-fluid">
    <div class="span12">
        <div class="row-fluid">
            <div class="span12">
                <div class="row-fluid ">
<!--                    <div class="span12">
                        <div class="pull-left mainCalendar" id="cruiseCalendarleftdiv" style="width:10%;">
                            <div id="cruiseCalendarleft">                    
                                <div id="cruiseCalendarleftContent" style="visibility: hidden;"></div>
                                <button id="CruisecalendarPrevious" class="CruisecalendarPrevious responsiveWeb"></button>
                            </div>
                        </div>
                        <div class="pull-left" id="calendermaindiv">
                            <div id="cruiseCalendar"></div>
                        </div>
                        <div class="pull-left mainCalendar" style="height:756px;width:10%;"  id="cruiseCalendarrightdiv">
                            <div id="cruiseCalendarright">                    
                                <div id="cruiseCalendarrightContent" style="visibility: hidden;"></div>
                                <button id="CruisecalendarNext" class="CruisecalendarNext responsiveWeb"></button>
                            </div>        
                        </div>
                    </div>-->
                    <div class="span2 mainCalendar" id="cruiseCalendarleftdiv">
                        <div id="cruiseCalendarleft">                    
                            <div id="cruiseCalendarleftContent" style="visibility: hidden;display:none;"></div>
                            <!--<button id="CruisecalendarPrevious" class="CruisecalendarPrevious responsiveWeb"></button>-->
                            <input type="button" id="CruisecalendarPrevious" class="CruisecalendarPrevious responsiveWeb"/>
                        </div>
                    </div>
                    <div class="span8">
                        <div id="cruiseCalendar"></div>
                    </div>
                    <div class="span2 mainCalendar" style="height:756px;"  id="cruiseCalendarrightdiv">
                        <div id="cruiseCalendarright">                    
                            <div id="cruiseCalendarrightContent" style="visibility: hidden;display:none;"></div>
                            <!--<button id="CruisecalendarNext" class="CruisecalendarNext responsiveWeb"></button>-->
                            <input type="button" id="CruisecalendarNext" class="CruisecalendarNext responsiveWeb"/>
                        </div>        
                    </div>
                </div>
                <div class="row-fluid"></div>
            </div>
        </div>
        <div class="mainCalendar">
            <!--    <div id="cruiseCalendarleft">
                    <button id="CruisecalendarPrevious" class="CruisecalendarPrevious responsiveWeb"></button>
                    <div id="cruiseCalendarleftContent" style="visibility: hidden;"></div>
                </div>-->
            <!--    <div id="cruiseCalendar"></div>
            
                <div id="cruiseCalendarright">
                    <button id="CruisecalendarNext" class="CruisecalendarNext responsiveWeb"></button>
                    <div id="cruiseCalendarrightContent" style="visibility: hidden;"></div>
                </div>-->
            <!--Mobile calendar view-->
            <div class="bx-loading"></div>
            <div class="responsiveMobile dateholder">
                <div id="responsiveMobileviewEventslist">       </div>
            </div>
            <!--end of mobile view-->
            <div class="cruiseKey responsiveWeb">
                <div class="cruiseKeyInner" id="cruiseKeyPop" class="btn large primary" rel="popover" data-placement="top">Cruise Key</div>
<!--                
                <div id="popover_content_wrapper" style="display: none">
                    <ul id="cruiseKeyPopover">

                    </ul>

                </div>-->
            </div>
            <div class="responsiveMobile">
                <br> <br>
                <div class="cruiseKeyTitle"> Cruise Key</div>

                <div class="cruiseKey" id="cruiseMobileKey">
                    <ul class="cruiseKeyInner cruiseMobileKey">

                    </ul>
                </div>
            </div>
        </div>
    </div>
</div>




<!--<button type="button" id="example" class="btn btn-default" data-container="body" data-toggle="popover" data-placement="top" data-content="Vivamus sagittis lacus vel augue laoreet rutrum faucibus.">
  Popover on top
</button>-->

 