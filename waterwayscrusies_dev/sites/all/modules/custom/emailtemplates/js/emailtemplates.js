jQuery(document).ready(function() {

    $(".checkboxgroup").click(function() {
//        var arr = new Array();
        var userslist = "";
        $.each($("input[name*='groupcheck']:checked"), function() {
//             arr.push($(this).val());            
            if (userslist != '') {
                userslist = userslist + ',' + $(this).val();
            } else {
                userslist = $(this).val();
            }
        });
//        alert(userslist);
        $("#edit-template-users").val(userslist);
        $("#userids").val(userslist);
        return true;
    });


});
