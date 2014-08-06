(function($) {      
    Drupal.ajax.prototype.commands.redirectlistview = function(ajax, response, status) {
        // this will be executed after the ajax call
        var dealid=$("#edit-dealid").val();
        var baseUrl = Drupal.settings.basePath;
        window.location = baseUrl+"groupon/"+dealid+"/view";
    }
})(jQuery);





function changestatus(grouponid){
    var baseUrl = Drupal.settings.basePath;
    var url=baseUrl+'grouponcodechangestatus';
    var params = 'grouponcodeid=' + grouponid
    jQuery.ajax({
        type: "POST",
        cache: false,
        async: true,
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

            $('#transparentLoader').hide();
            //sucess code replaces here
            // console.log(response);
            if (response.code == 200)
            {
                
            }
        },
        complete: function() {
            //complete  codes gets replace here
            window.location.reload(true);
        }
    });
}