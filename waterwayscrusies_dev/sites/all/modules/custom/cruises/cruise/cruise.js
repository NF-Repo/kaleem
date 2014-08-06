(function($) {
    Drupal.behaviors.cruiseAjax = {
        attach: function(context, settings) {
            var baseUrl = Drupal.settings.basePath;


            $('#resetcourse').click(function() {
                $('#coursename').val('');
                $('#columnname').val('');
            });

            $('#resetmenus').click(function() {
                $('#menu_title').val('');
                $('#selected_course').val('');
                $('#menudescription').val('');
                $('#menu_image').val('');
                CKEDITOR.instances['edit-description-value'].setData('');


            });



        }

    };





})(jQuery);



/*
 * Delete Course
 */
function deletecourse(id,$cruise_id) {
	var pathname = window.location.pathname;
	var cnt=0;
	 var baseUrl = Drupal.settings.basePath;
	    var params = 'courseid=' + id;
	    jQuery.ajax({
	        type: "POST",
	        cache: false,
	        async: false,
	        url: baseUrl + 'getmenucountbycourseid',
	        data: params,
	        dataType: "json",
	        beforeSend: function() {
	    
	        },
	        error: function(request, error) {
	        },
	        success: function(response, status, req) {
	        	cnt=parseInt(response);
	             $('#transparentLoader').css('display', 'none');
	        },
	        complete: function() {
	           
	        }
	    });
	    
	    if(cnt>0){
	    	 var confirmresult= confirm(' Menu Records exist with this course \n Are you sure you want to delete?');
	    	 if(confirmresult){
	    		 var baseUrl = Drupal.settings.basePath;
	    		    var params = 'courseid=' + id;
	    		    jQuery.ajax({
	    		        type: "POST",
	    		        cache: false,
	    		        async: true,
	    		        url: baseUrl + 'deletecourse',
	    		        data: params,
	    		        dataType: "json",
	    		        beforeSend: function() {
	    		            $('#transparentLoader').css('display', 'block');

	    		        },
	    		        error: function(request, error) {
	    		        },
	    		        success: function(response, status, req) {
	    		           //response = response.replace(/\&amp;/g, '&');
	    		            courseresponse = response['coursedata'].replace(/\&amp;/g, '&');
	     		        	$('#courselistdiv').html(courseresponse);
	     		        	
	     		        	menuresponse = response['menu_data'].replace(/\&amp;/g, '&');
	     		        	$('#menudiv').html(menuresponse);
	     		        	
	     		        	
	     		        	console.log(courseresponse);
	     		        	console.log(menuresponse);
	     		        	
	    		            //window.location.href=pathname; 
	    		            // $('#transparentLoader').css('display', 'none');
	    		        },
	    		        complete: function() {
	    		            //complete  codes gets replace here
	    		            $('#transparentLoader').css('display', 'none');
	    		            
	    		        }
	    		    });
	    	 }
	    }else{
	    	var baseUrl = Drupal.settings.basePath;
 		    var params = 'courseid=' + id;
 		    jQuery.ajax({
 		        type: "POST",
 		        cache: false,
 		        async: true,
 		        url: baseUrl + 'deletecourse',
 		        data: params,
 		        dataType: "json",
 		        beforeSend: function() {
 		            $('#transparentLoader').css('display', 'block');

 		        },
 		        error: function(request, error) {
 		        },
 		        success: function(response, status, req) {
 		           // response = response.replace(/\&amp;/g, '&');
 		           
 		        	
 		        	courseresponse = response['coursedata'].replace(/\&amp;/g, '&');
 		        	$('#courselistdiv').html(courseresponse);
 		        	
 		        	menuresponse = response['menu_data'].replace(/\&amp;/g, '&');
 		        	$('#menudiv').html(menuresponse);
 		        	console.log(courseresponse);
 		        	console.log(menuresponse);
 		            $('#transparentLoader').css('display', 'none');
 		        },
 		        complete: function() {
 		            //complete  codes gets replace here
 		            $('#transparentLoader').css('display', 'none');
 		          
 		        }
 		    });
	    }
	    

	   
	//return false;


   
}


function checkCourseMenuExistence(courseid){
	 var baseUrl = Drupal.settings.basePath;
	    var params = 'courseid=' + courseid;
	    jQuery.ajax({
	        type: "POST",
	        cache: false,
	        async: true,
	        url: baseUrl + 'getmenucountbycourseid',
	        data: params,
	        dataType: "json",
	        beforeSend: function() {
	    
	        },
	        error: function(request, error) {
	        },
	        success: function(response, status, req) {
	           console.log(response);
	            // $('#transparentLoader').css('display', 'none');
	        },
	        complete: function() {
	           
	        }
	    });
	    
	
}



/*
 * Update Course
 */

function getCourseDetails(id) {
    var baseUrl = Drupal.settings.basePath;
    var params = 'courseid=' + id;
    jQuery.ajax({
        type: "GET",
        cache: false,
        async: true,
        url: baseUrl + 'getcouredetails',
        data: params,
        dataType: "json",
        beforeSend: function() {
            $('#transparentLoader').css('display', 'block');

        },
        error: function(request, error) {
            //error codes replaces here
        },
        success: function(response, status, req) {
            $('#coursehiddenvalue').val(id);
            $('#coursename').val(response.course_name);
            $('#columnname').val(response.column_name);
            if (response.courselabel_show == 'show') {
                $('#edit-courselabelshow-show').attr('checked', true);
            } else {
                $('#edit-courselabelshow-hide').attr('checked', true);
            }
            // $('#transparentLoader').css('display', 'none');
        },
        complete: function() {
            //complete  codes gets replace here
            $('#transparentLoader').css('display', 'none');
        }
    });



}


function deleteCourseMenu(id) {

    var baseUrl = Drupal.settings.basePath;
    var params = 'coursemenuid=' + id;
    jQuery.ajax({
        type: "POST",
        cache: false,
        async: true,
        url: baseUrl + 'deletecoursemenu',
        data: params,
        dataType: "json",
        beforeSend: function() {
            $('#transparentLoader').css('display', 'block');

        },
        error: function(request, error) {
        },
        success: function(response, status, req) {
            response = response.replace(/\&amp;/g, '&');
//            console.log(response);

            $('#menudiv').html(response);
            // $('#transparentLoader').css('display', 'none');
        },
        complete: function() {
            //complete  codes gets replace here
            $('#transparentLoader').css('display', 'none');
        }
    });



}


function deleteImage(id) {

    //alert(id);

    var baseUrl = Drupal.settings.basePath;
    var params = 'coursemenuid=' + id;
    jQuery.ajax({
        type: "POST",
        cache: false,
        async: true,
        url: baseUrl + 'deletecoursemenuimage',
        data: params,
        dataType: "json",
        beforeSend: function() {
            $('#transparentLoader').css('display', 'block');

        },
        error: function(request, error) {
        },
        success: function(response, status, req) {
            response = response.replace(/\&amp;/g, '&');
            $('#menudiv').html(response);
        },
        complete: function() {
           
            $('#transparentLoader').css('display', 'none');
        }
    });
}





function getCourseMenuDetails(id) {

    $('#coursemenuhiddenvalue').val(id);
    var baseUrl = Drupal.settings.basePath;
    var params = 'menuid=' + id;
    jQuery.ajax({
        type: "GET",
        cache: false,
        async: true,
        url: baseUrl + 'getcoursemenudetails',
        data: params,
        dataType: "json",
        beforeSend: function() {
            $('#transparentLoader').css('display', 'block');

        },
        error: function(request, error) {
            //error codes replaces here
        },
        success: function(response, status, req) {
            $('#coursemenuhiddenvalue').val(id);
            $('#menu_title').val(response.menu_title);
            $('#selected_course').val(response.course_name);
//            $('#menudescription').val(response.description);
            $('#edit-description-value').val(response.description);
//            $('#edit-description-value').elrte('val') = response.description;
            CKEDITOR.instances['edit-description-value'].setData(response.description);
//             $("#scholarship-short_desc").ckeditor();
            if (response.menulabel_show == 'show') {
               // $('#edit-menulabelshow-show').attr('checked', 'checked');
                $('input:radio[id=edit-menulabelshow-show]').prop('checked', true);
                //$('#edit-menulabelshow-hide').removeAttr('checked');
            }
            if (response.menulabel_show == 'hide') {
            	
            	//alert('hide');
               // $('#edit-menulabelshow-hide').attr('checked', 'checked');
                $('input:radio[id=edit-menulabelshow-hide]').prop('checked', true);
               // $('#edit-menulabelshow-show').removeAttr('checked');
            }
            // $('#transparentLoader').css('display', 'none');
        },
        complete: function() {
            //complete  codes gets replace here
            $('#transparentLoader').css('display', 'none');
        }
    });



}

