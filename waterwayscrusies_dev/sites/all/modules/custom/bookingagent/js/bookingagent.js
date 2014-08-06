


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
//Gift  Cards Check Balance : Ajax callback
function checkbalance()
{
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
            $('#loaderText').show();
        },
        error: function(request, error) {
            //error codes replaces here
        },
        success: function(response, status, req) {
            $('#loaderText').hide();
            //$('#balanceCheckerPopup').hide();
            //$(".modal-backdrop").hide();
            //console.log(response);
            if (response.code == 200)
            {
              jQuery("#giftcardbalance_amount").html('$' + response.results.amount);
            }
            else
            {
             jQuery("#giftcardbalance_amount").html('This Gift Card code is invalid or has expired');
            }
        },
        complete: function() {


        }
    });
    //End of GiftCard ajax call
}
jQuery(document).ready(function() {
    var url = Drupal.settings.basePath + 'loadbooklingentrees';

   


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

    $('#dietarysubmitbtn').click(function() {
        saveEntree();
    });
});


function saveEntree()
{
	var restrictiontext = $('#dietarytext').val();
	var val=$('#hiddenSelectedTicket').val();
	var val1='dietarycomments-'+val;
	$('#'+val1).val(restrictiontext);
	$("#reservationCheckboxPopup").hide();
    $('.reservationCheckBackdrop').hide();
  
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
            $("#reservationCheckboxPopup").hide();
            $('.reservationCheckBackdrop').hide();
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


    }
};


//   jQuery(function($){
//	$("#edit-guest-areacode").mask("(999)");
//	$("#edit-billing-areacode").mask("(999)");
//	});


jQuery(function($){
 //Billing Area Code
    $('#billingAreaCode').keyup(function(e){
    	if($(this).val().length==3){
    		$('#billingPrefix').focus();
	    	//var newstring="("+$(this).val()+")";
	    	//$('#billingAreaCode').val(newstring);
    	}
         
    });
    
    //Billing prefix
    
    $( "#billingAreaCode").click(function() {
    	$( "#billingAreaCode" ).val('');
    });
    
  
    
    
    $('#billingPrefix').keyup(function(e){
    	if($(this).val().length==3){
    		$('#billingLineNumber').focus();
    	}
    });
    
    
   
   
    
   //Billing Area Code
    $('#guestAreaCode').keyup(function(e){
    	if($(this).val().length==3){
    		$('#guestPrefix').focus();
	    	//var newstring="("+$(this).val()+")";
	    	//$('#guestAreaCode').val(newstring);
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








