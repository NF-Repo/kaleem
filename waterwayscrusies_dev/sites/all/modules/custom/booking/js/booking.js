 
 





//Dietary checkbox checked operations
/**
 * 
 * @param {type} ticketname
 * @param {type} aid
 * @param {type} icrmentval
 * @returns {undefined}
 * 
 * 
 */
var selectedentreenode;



function loadattributewidget(itemqty, dpfitemid, aid, selectedentrnode)
{

    console.log(itemqty);
    console.log(dpfitemid);
    console.log(aid);

    var productnode = $('input[name=selecteditemid]').val();
    var pricename = 'priceattr-' + aid;
    console.log(pricename);
    if ($("." + pricename)) {
        $("." + pricename).remove();
    }
    var itemprice = $("input[name=" + pricename + "]").val();
    console.log(itemprice);
//        var itemqty = this.value;
    var attribname = $("label[for=" + dpfitemid + "]").text();
    var ticketnames = attribname.split("$");
    var ticketname = $.trim(ticketnames[0]);

    var rowid = "row-" + dpfitemid;
    var itemprice = $("input[name=" + pricename + "]").val();
//        var itemqty = this.value;


    //  var url = Drupal.settings.basePath + 'attributeentrees';//commonajaxservice
    var params = 'aid=' + aid + '&productnode=' + productnode;
    var entryoptionhtml = '';
    jQuery.ajax({
        type: "POST",
        cache: false,
        async: false,
        url: url,
        data: params,
        dataType: "json",
        beforeSend: function() {
            $('#transparentLoader').show();
        },
        error: function(request, error) {
            //error codes replaces here
        },
        success: function(response, status, req) {
            //sucess code replaces here
//              console.log(response);
            $('#transparentLoader').hide();
            if (response.length > 0)
            {
                $.each(response, function(i, item) {
                    console.log(i);
                    console.log(item.nid);
                    console.log(item.title);
                    if (selectedentrnode == item.nid)
                    {

                        entryoptionhtml += '<option value="' + item.nid + '" selected>' + item.title + '</option>';
                    }
                    else
                    {
                        entryoptionhtml += '<option value="' + item.nid + '">' + item.title + '</option>';
                    }
                });
            }
            else
            {
                entryoptionhtml += '<option value="0">no options</option>';
            }


        },
        complete: function() {
            //complete  codes gets replace here

        }
    });
    //end of loading entree events



    var entrhtml = '<div class="row-fluid">';
    entrhtml += '<div class="span12 purchaseTicketDetails">';
    entrhtml += '<table cellpadding="0" cellspacing="0" border="0">';
    if ($('.entrywidget #entryContent .fieldset-wrapper').is(':empty'))
    {
        entrhtml += '<tr>';
        entrhtml += '<td class="bookingagentheader2">Quantity</td>';
        entrhtml += '<td class="bookingagentheader2">Ticket</td>';
        entrhtml += '<td class="bookingagentheader2">Entree</td>';
        entrhtml += '<td class="bookingagentheader2"></td>';
        entrhtml += '<td class="bookingagentheader2">Price</td>';
        entrhtml += '</tr>';
    }

    for (var i = 0; i < itemqty; i++)
    {

        var formattedAmt = parseFloat(itemprice).toFixed(2);
        var amtArr = formattedAmt.split('.');
        var amtWholeNumber = amtArr[0];
        var amtDecimalNumber = amtArr[1];
        var itemPrice = amtWholeNumber + ".<sup>" + amtDecimalNumber + "</sup>";
        var commonname = 'cruise';
        var dietarycheckboxvalue = commonname + '-' + aid + '-' + (i + 1);//ticket name+attrid+1;
        entrhtml += '<tr class="' + pricename + '">';
        entrhtml += '<td class="ticketquantity">1</td>';
        entrhtml += '<td>' + ticketname + '</td>';
        entrhtml += '<td>';
        var ticketnumber = i + 1;
        entrhtml += '<select class="entreeoption ticketpriceqty" id="option-' + dietarycheckboxvalue + '-' + ticketnumber + '">';
        entrhtml += '<option value="empty">--select--</option>';
        entrhtml += entryoptionhtml;
        entrhtml += '</select>';
        entrhtml += '</td>';


        entrhtml += '<td><div class="dietarywrap"><input type="checkbox" id="' + dietarycheckboxvalue + '-' + ticketnumber + '" name="dietrestrictions[]" value="' + dietarycheckboxvalue + '-' + ticketnumber + '" style="float: left;" onclick="dietarycheckfieldEvent(' + JSON.stringify(dietarycheckboxvalue + '-' + (i + 1)).replace(/"/g, "&quot;") + ');" disabled/>';
        entrhtml += '<span class="ticketdesc">Dietary restrictions</span></div></td>';
        entrhtml += '<input type="hidden" name="' + dietarycheckboxvalue + '" id="userinput' + dietarycheckboxvalue + '"/>';
//        entrhtml += '<td>$' + itemprice + '.<sup>00</sup></td>';
        entrhtml += '<td>$' + itemPrice + '</td>';
        entrhtml += '</tr>';
    }

    entrhtml += '</div>';

    entrhtml += '</div>';

    return entrhtml;
}

function collapsefieldset(id,type){
 $("#"+id).removeClass("collapsed");
 
}
function errormsg(id,type){   
    if(type == 'set'){
        $("#"+id).addClass("error");
    }else{
        $("#"+id).removeClass("error");
    }
    
}
function checkbalance()
{
	
	


    //Gift Card Ajax call

    var url = Drupal.settings.basePath + 'balancechecker';
    var cardId = jQuery("#giftcardpop_number").val();
    var entryType = "K";
    
  


    var params = 'giftcardid=' + cardId + '&entryType=' + entryType + '&callmethod=Inquiry';
    jQuery.ajax({
        type: "POST",
        cache: false,
        async: true,
        url: url,
        data: params,
        dataType: "json",
        beforeSend: function() {
        	//jQuery("#giftcardbalance_amount").html('');
            $('#loaderText').show();
        },
        error: function(request, error) {
            //error codes replaces here
        },
        success: function(response, status, req) {

            $('#loaderText').hide();
            //$('#balanceCheckerPopup').hide();
           // $(".modal-backdrop").hide();
            console.log(response);
            if (response.code == 200)
            {
            	//console.log(resonse);
                //console.log(response.results);
                //console.log(response.results.valueCode);
               // console.log(response.results.amount);
               	jQuery("#giftcardbalance_amount").html('$' + response.results.amount);
              }
            else
            {
            	jQuery("#giftcardbalance_amount").html('This Gift Card code is invalid or has expired');
                //var cardbalancehtml = '<h4>' + response.message + '</h4>';

            }


        },
        complete: function() {


        }
    });
    //End of GiftCard ajax call






}
jQuery(document).ready(function() {
    var url = Drupal.settings.basePath + 'loadbooklingentrees';


//laxmi bookingagent page comment entree check box

$("#commentcheck").click(function(){
  alert('Comments');
});

//end of bookingagent comment entree check box





//Widget Action : Edit Ticket Order
    $("#editticketorderlinkPayment").click(function() {
        $("#backtoticketorderbutton").trigger("click");
    });
    
    $("#editticketorderlinkEnhancements").click(function() {
        $("#enhancementsbackbutton").trigger("click");
    });
    $("#backtoenhancementslink").click(function() {
        $("#paymentbackbutton").trigger("click");
    });


    //satya sep 30
    $('#dietarysubmitbtn').click(function() {
        saveEntree();
    });
    
      
    $('#edit-guest-areacode,#edit-guest-prefix,#edit-guest-linenumber').keyup(function(e){
    if($(this).val().length==$(this).attr('maxlength'))
    $(this).next(':input').focus()
    });
    

});


function saveEntree()
{
	
	var restrictiontext = $('#dietarytext').val();
	var val=$('#hiddenSelectedTicket').val();
	
	//alert(val);
	var val1='dietarycomments-'+val;
	if(restrictiontext==""){
		$("#reservationCheckboxPopup").hide();
	    $('.reservationCheckBackdrop').hide();
	    $("#dietary-"+val).removeAttr('checked');
	}else{
		$('#'+val1).val(restrictiontext);
		$("#reservationCheckboxPopup").hide();
	    $('.reservationCheckBackdrop').hide();
	    $("#dietary-"+val).attr('checked',true);
	    $("#dietary-"+val).prop('checked');
	  
	}
	
	
	
  
}



function dietarycheckfieldEvent(selectedTicket)
{
	
	var value=$('#'+'dietarycomments-'+selectedTicket).val();
	$('#dietarytext').val(value);
	//Setting Values
    $('#hiddenSelectedTicket').val(selectedTicket);
    $('.reservationCheckBackdrop').show();
    $('#reservationCheckboxPopup').show();
}




Drupal.behaviors.bookingagent = {
    attach: function(context, settings) {
    	
    	

        $("#reservationCheckboxClose").click(function() {
        	$("#dietarysubmitbtn").trigger("click");
           // $("#reservationCheckboxPopup").hide();
           // $('.reservationCheckBackdrop').hide();
        });

        /* Gift Card Balace Checker PopUp Close */
        $("#balanceCheckerPopupClose").click(function() {
            $('#balanceCheckerPopup').hide();
            $(".modal-backdrop").remove();
        });

        $("#balanceCheckerPopup").on('show', function(event) {
            $('.modal-backdrop').remove();
        });


        //checkbox checked script same as guest wit biling address
        $('.ticketsameas', context).change(function() {
            if (this.checked) {
                //Do stuff


                $('#edit-billing-firstname').val($('#edit-guest-firstname').val());
                $('#edit-billing-lastname').val($('#edit-guest-lastname').val());
                $('#edit-billing-address').val($('#edit-guest-address').val());
                $('#edit-billing-otheraddress').val($('#edit-guest-otheraddress').val());
                $('#edit-billing-city').val($('#edit-guest-city').val());
                $('#edit-billing-state').val($('#edit-guest-state').val());
                $('#edit-billing-zipcode').val($('#edit-guest-zipcode').val());
                //$('#edit-billing-phone').val($('#edit-guest-phone').val());
                $('#billingAreaCode').val($('#guestAreaCode').val());
                $('#billingPrefix').val($('#guestPrefix').val());
                $('#billingLineNumber').val($('#guestLineNumber').val());
                $('#edit-billing-email').val($('#edit-guest-email').val());
                $('#edit-billing-confirmemail').val($('#edit-guest-confirmemail').val());
            }
            else
            {

                $('#edit-billing-firstname').val('');
                $('#edit-billing-lastname').val('');
                $('#edit-billing-address').val('');
                $('#edit-billing-otheraddress').val('');
                $('#edit-billing-city').val('');
                $('#edit-billing-state').val('');
                $('#edit-billing-zipcode').val('');
                //$('#edit-billing-phone').val('');
                $('#billingAreaCode').val('');
                $('#billingPrefix').val('');
                $('#billingLineNumber').val('');
                $('#edit-billing-email').val('');
                $('#edit-billing-confirmemail').val('');
            }
        });

        $('#edit-guest-confirmemail').attr('autocomplete', 'off');
        $('#edit-guest-confirmemail').bind('copy paste', function(e) {
            e.preventDefault();
        });

        $('#edit-billing-confirmemail').attr('autocomplete', 'off');
        $('#edit-billing-confirmemail').bind('copy paste', function(e) {
            e.preventDefault();
        });

        $("#addvoucherlink").click(function() {
            $("#edit-back").click();
        });


        $("#edit-guest-zipcode").keydown(function(event) {
            disableKeys(event);
        });
        $("#edit-billing-zipcode").keydown(function(event) {
            disableKeys(event);
        });

//        $("#edit-guest-areacode").keydown(function(event) {
//            disableKeys(event);
//        });
//
//        $("#edit-guest-linenumber").keydown(function(event) {
//            disableKeys(event);
//        });
//        $("#edit-billing-areacode").keydown(function(event) {
//            disableKeys(event);
//        });
//        $("#edit-billing-prefix").keydown(function(event) {
//            disableKeys(event);
//        });
//        $("#edit-billing-linenumber").keydown(function(event) {
//            disableKeys(event);
//        });
        $("#edit-creditcardnumber").keydown(function(event) {
            disableKeys(event);
        });
        
        
//        $('#edit-guest-areacode').live("keyup", function (e) { 
//            if ($(this).val().length > 3) {
//            	$( "#edit-guest-prefix" ).focus();
//                
//            }
//        });
//    	
//    	$('#edit-guest-prefix').live("keyup", function (e) { 
//            if ($(this).val().length > 3) {
//            	$( "#edit-guest-linenumber" ).focus();
//                
//            }
//        });
        
        
        



    }
};


//   jQuery(function($){
//	$("#edit-guest-areacode").mask("(999)");
//	$("#edit-billing-areacode").mask("(999)");
//	});
   
   jQuery(function($){
//	    $('#billingAreaCode,#billingPrefix,#billingLineNumber').keyup(function(e){
//	    	
//	    	
//	    	if(){
//	    		
//	    	}
//	    //alert($(this).val().length);
////	    	alert('hai');
//	    	if($(this).val().length==$(this).attr('maxlength'))
//	    $(this).next(':input').focus()
//	    });
	   
	   
	 //Billing Area Code
	    $('#billingAreaCode').keyup(function(e){
	    	if($(this).val().length==3){
	    		$('#billingPrefix').focus();
		    	//var newstring="("+$(this).val()+")";
		    	//$('#billingAreaCode').val(newstring);
	    	}
	         
	    });
	    
	    $( "#billingAreaCode").click(function() {
	    	$( "#billingAreaCode" ).val('');
	    });
	    
	    
	    
	   
	    
	    
	    
	    //Billing prefix
	    $('#billingPrefix').keyup(function(e){
	    	if($(this).val().length==3){
	    		$('#billingLineNumber').focus();
	    	}
	    });
	    
	    
	   
	   
	    
	   //Billing Area Code
	    $('#guestAreaCode').keyup(function(e){
	    	if($(this).val().length==3){
	    		$('#guestPrefix').focus();
//		    	var newstring="("+$(this).val()+")";
//		    	$('#guestAreaCode').val(newstring);
	    	}
	         
	    });
	    
	    
	    $( "#guestAreaCode").click(function() {
	    	$( "#guestAreaCode" ).val('');
	    });
	    
	    //Billing prefix
	    $('#guestPrefix').keyup(function(e){
	    	if($(this).val().length==3){
	    		$('#guestLineNumber').focus();
	    	}
	    });
	    
	    
   });
	    
	    

//edit-guest-areacode
//edit-guest-prefix
//edit-guest-linenumber






/***
 * Allowing only Numbers in text fields
 * @param {type} event
 * @returns {nothing}
 */


function disableKeys(event) {
    if (event.keyCode == 9) {
        return true;
    }
    if (event.shiftKey)
        event.preventDefault();
    if (event.keyCode == 46 || event.keyCode == 8) {
    }
    else {
        if (event.keyCode < 95) {

            if (event.keyCode < 48 || event.keyCode > 57) {
                if (event.keyCode >= 37 && event.keyCode <= 40) {
                }
                else
                {
                    event.preventDefault();
                }
            }
        }
        else {
            if (event.keyCode < 96 || event.keyCode > 105) {
                event.preventDefault();
            }
        }
    }
}


//Onchange functionality for ticket entrees
function saveTicketEntree(ticketentreeid, ticketid, selectedTicket,eventId) {
	
	//alert(selectedTicket);
	
	//dietarycomments-3-0	
	//var t='dietarycomments'+selectedTicket;
	//$('#dietarycomments-3-0').val('test');
    //var entreenodevalue = $('#' + productEntreeId).val();
//    var ticketentreeid = $('#' + ticketentreeid).val();
//    var ticketid = ticketid;
//    var restrictiontext = '';
//    var cartsession = jQuery("#cruisecardsession").val();
//    var selectedticket = selectedTicket;
//    var eventid=eventId;
//    var url = Drupal.settings.basePath + 'ticketentrees';
//    var params = 'ticketentreeid=' + ticketentreeid + '&ticketid=' + ticketid + '&dietarytext=' + restrictiontext + '&selectedticket=' + selectedticket + '&cruisecartsession=' + cartsession+ '&eventid=' + eventid;
//    var entryoptionhtml = '';
//    jQuery.ajax({
//        type: "POST",
//        cache: false,
//        async: true,
//        url: url,
//        data: params,
//        dataType: "json",
//        beforeSend: function() {
//            $('#transparentLoader').show();
//        },
//        error: function(request, error) {
//            //error codes replaces here
//        },
//        success: function(response, status, req) {
//
//            $('#transparentLoader').hide();
//            //sucess code replaces here
//            // console.log(response);
//            if (response.code == 200)
//            {
//
//            }
//
//        },
//        complete: function() {
//            //complete  codes gets replace here
//
//        }
//    });

}






