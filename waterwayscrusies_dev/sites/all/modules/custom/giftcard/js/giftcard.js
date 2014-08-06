
   jQuery(function($){
	   window.location.hash="no-back-button";
	   window.location.hash="Again-no-back-button";//for google chrome
	   window.onhashchange=function(){window.location.hash="no-back-button";}
	   $("#customerphonenumber").mask("999-999-9999");
	   
	   
	   
	   
	   $("#finishorder").click(function() {
		   
		   $('#finishorderdiv').hide();
		   
		   $('#finishorderprocessingdiv').show();
		   //alert('asdf');
		  // this.disabled = true;
		   //this.value  = "Processing";
		   //alert("Test");
		  // var submitButton = document.getElementById('edit-next');
		  // submitButton.disabled=true; 
		  // return true;
	  });
	   
	
	   
	
	});
   
  
   
   
  
   
  
  