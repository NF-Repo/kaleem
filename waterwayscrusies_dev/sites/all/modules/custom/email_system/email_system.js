if (Drupal.jsEnabled) {

    $('#registerButton').bind('click',function(){

        alert('test');


    });
   
    function myCommunity()
    {    
       
        /*  Community member Details     */
        var communityid=document.getElementById('communityid').value;   
        
//        if(communityid.value=='')
//            {
//                alert('Please enter community id');
//                return false;
//            }
//            else
//                {
//                    return true;
//                }
        
        var params='communityid='+communityid;
        
        console.log(params);
           jQuery.ajax({
                type: "POST",
                cache: false,
                async: false,
                url:'./mycommunity',
                data:params,
                dataType: "json",
                beforeSend: function() {

                },
                error: function (request,error) {
                //                 alert(error);
                },
                success: function (response, status, req) {
                   
                   
                    var email = response[0].email;
                    var phone = response[0].phone;
                    var firstname = response[0].firstname;
                    
                
                    document.getElementById('emailid').innerHTML = email;
                    document.getElementById('phoneno').innerHTML = phone;
                    document.getElementById('fname').innerHTML = firstname;

                },
                complete: function() {
                }
            });

        
        
    }

}
