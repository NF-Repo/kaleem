/* 
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */


Drupal.behaviors.ordereditcalendardayevents = {attach: function(context, settings)
    {
//	var baseUrl = Drupal.settings.basePath;
//	$('#changecruisebtn').click(function() {
//		
//		var eventid=$('#selectedEventId').val();
//		var orderid=$('#hiddenorderid').val();
//		var eventdate=$('#hiddenselectedcruisedate').val();
//		
//		
//		//$dt=eventdate.replace("/","-"); 
//		window.location.href=baseUrl+'bookingorder/'+eventid+'/'+orderid+'/'+eventdate+'changecruise';
//		
//		//alert(val);
//		
//		//alert('asdf');
//	});



        Drupal.ajax.prototype.commands.reloadafterpayment = function(ajax, response, status) {
            // this will be executed after the ajax call
            var orderid = $("#hiddenorderid").val();
            var baseUrl = Drupal.settings.basePath;
            window.location = baseUrl + "bookingorder/" + orderid + "/edit";
        }


    }};





function checkEvents(day, month, year) {
    //edit-datebasedeventlist

    var formatdate = year + '' + month + '' + day;
    $('#hiddenselectedcruisedate').val(formatdate);

    var baseUrl = Drupal.settings.basePath;
    var params = 'day=' + day + '&month=' + month + '&year=' + year;
    jQuery.ajax({
        type: "POST",
        cache: false,
        async: true,
        url: baseUrl + 'ajax-getevents',
        data: params,
        dataType: "json",
        beforeSend: function() {
            $('#transparentLoader').css('display', 'block');
            $('#datebasedeventlist').empty();

        },
        error: function(request, error) {
        },
        success: function(response, status, req) {
            console.log(response);

            //var obj= jQuery.parseJSON(response);
//	            var mySelect = $('#edit-datebasedeventlist');
//	            $.each(response, function(id, name) {
//	                mySelect.append(
//	                    $('<option></option>').val('asdf').html('asdfa')
//	                );
//	            });


            $.each(response, function(id, name) {
                $('#datebasedeventlist')
                        .append($("<option></option>")
                                .attr("value", id)
                                .text(name));
            });
            //$('#courselistdiv').html(response);
            $('#transparentLoader').css('display', 'none');
        },
        complete: function() {
            //complete  codes gets replace here
            $('#transparentLoader').css('display', 'none');
        }
    });

}



function getcruiseevents(seldate) {

    $('#hiddenselectedcruisedate').val(seldate);
    //alert(seldate);
    var baseUrl = Drupal.settings.basePath;
    var params = 'seldate=' + seldate;
    jQuery.ajax({
        type: "POST",
        cache: false,
        async: true,
        url: baseUrl + 'getcruiseeventoptions',
        data: params,
        dataType: "json",
        beforeSend: function() {
            $('#transparentLoader').css('display', 'block');
        },
        error: function(request, error) {
            //error codes replaces here
        },
        success: function(response, status, req) {

            var html = '';

            if (response.length > 0) {
                for (var i = 0; i < response.length; i++) {
                    html = html + '<option value=' + response[i].id + '>' + response[i].name + '</option>';
                }
            }
            $("#edit-cruisetype").html(html);


        },
        complete: function() {
            //complete  codes gets replace here
            $('#transparentLoader').css('display', 'none');
        }
    });

}
function geteventhtmldata() {

    //alert(seldate);
    var baseUrl = Drupal.settings.basePath;
    var cruisetype = $("#edit-cruisetype").val();
    var params = 'cruisetype=' + cruisetype;
    jQuery.ajax({
        type: "POST",
        cache: false,
        async: true,
        url: baseUrl + 'geteventhtmldata',
        data: params,
        dataType: "json",
        beforeSend: function() {
            $('#transparentLoader').css('display', 'block');
        },
        error: function(request, error) {
            //error codes replaces here
        },
        success: function(response, status, req) {
            console.log(response);
            $("#eventdetailsdiv").html(response);

        },
        complete: function() {
            //complete  codes gets replace here
            $('#transparentLoader').css('display', 'none');
        }
    });

}

function changecruise() {
    var cruisetype = $("#edit-cruisetype").val();
    var hiddenorderid = $("#hiddenorderid").val();
    var hiddenselectedcruisedate = $("#hiddenselectedcruisedate").val();
    var year = hiddenselectedcruisedate.substring(6, 10);
    var month = hiddenselectedcruisedate.substring(0, 2);
    var date = hiddenselectedcruisedate.substring(3, 5);
    var d1 = year + month + date;
    var baseUrl = Drupal.settings.basePath;
    var url = baseUrl + "bookingorder/" + cruisetype + "/" + hiddenorderid + "/" + d1 + "/changecruise";

    window.location.href = url;
//    alert(cruisetype);
//    console.log(cruisetype);
//    console.log(hiddenorderid);
//    console.log(hiddenselectedcruisedate);

}