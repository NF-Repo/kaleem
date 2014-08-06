(function($) {


    Drupal.ajax.prototype.commands.resetbuffetcontent = function(ajax, response, status) {
        // this will be executed after the ajax call
        var contentid = 'edit-catering-selection-fieldset-buffet-content0-value';
        $('#buffet_image').replaceWith($('#buffet_image').clone());
        $('#buffet_image').val('');
        for (name in CKEDITOR.instances)
        {
            var iscatering = name.indexOf('catering-selection');
            if (iscatering >= 0) {
                if (name == contentid) {
                    CKEDITOR.instances[contentid].setData('');
                    $('#buffet_caption').val('');
                } else {
                    CKEDITOR.instances[name].destroy();
                    $("#" + name).parent().parent().parent().parent().remove();
                }
            }
        }
    }
    Drupal.ajax.prototype.commands.resetupdatebuffetcontent = function(ajax, response, status) {
        // this will be executed after the ajax call
        $('#buffet_image').replaceWith($('#buffet_image').clone());
        $('#buffet_image').val('');
        var contentid = 'edit-catering-selection-fieldset-buffet-content0-value';
        CKEDITOR.instances[contentid].setData('');
        $('#addcoursediv').css('display', 'block');
        $('#updatecoursediv').css('display', 'none');
    }


    Drupal.ajax.prototype.commands.resetbuffeteditcontent = function(ajax, response, status) {
        // this will be executed after the ajax call
//        var contentid = 'edit-catering-selection-fieldset-buffet-content0-value';
//        var contentid = 'edit-buffet-edit-fieldset-buffet-content-edit0-value';
//        var contentid = 'edit-buffet-edit-fieldset-buffet-content-edit0-value';
        for (name in CKEDITOR.instances)
        {
            var isbuffetedit = name.indexOf('buffet-edit');
            if (isbuffetedit >= 0) {

                CKEDITOR.instances[name].destroy();
                $("#" + name).parent().parent().parent().parent().remove();
            }
        }

    }

})(jQuery);

function test() {
    var buffetcontentcount = $('#buffetcontentcount').val();
//    alert('presen count----' + buffetcontentcount);
    for (name in CKEDITOR.instances)
    {
        var isbuffetedit = name.indexOf("buffet-edit");
        if (isbuffetedit >= 0) {
            var isexist = 0;
//            alert(name);
            for (var i = 0; i < buffetcontentcount; i++) {
                var editorname = "edit-buffet-edit-fieldset-buffet-content-edit" + i + "-value";
                if (editorname == name) {
//                    CKEDITOR.replace("#" + editorname);
                    isexist = 1;
                }
            }
            if (isexist == 0) {
//                CKEDITOR.instances[name].destroy();
                // $("#" + name).parent().parent().parent().parent().remove();
            }
        }
    }
}

function addcontent() {
    var count = $("#buffetcontentcount").val();
    count++;
    $("#buffetcontentcount").val(count);
//    alert("inc count----" + count);
}
function removecontent() {
    var count = $("#buffetcontentcount").val();
    if (count > 1) {
        count--;
    }
//    alert("deduct count----" + count);
    $("#buffetcontentcount").val(count);
}

function getbuffetdata(buffetid) {
    var baseUrl = Drupal.settings.basePath;
    var params = 'buffetid=' + buffetid;
    $('#buffet_image').val('');
    $('#buffet_image').replaceWith($('#buffet_image').clone());
    jQuery.ajax({
        type: "POST",
        cache: false,
        async: false,
        url: baseUrl + 'getbuffetdatabybuffetid',
        data: params,
        dataType: "json",
        beforeSend: function() {
            $('#transparentLoader').css('display', 'block');
        },
        error: function(request, error) {
        },
        success: function(response, status, req) {
            if (response != '') {

//                console.log(response.buffetimagesrc)
                $("#buffet_title").val(response.buffet_title);
                $("#buffet_id").val(response.id);
                $("#buffet_caption").val(response.buffet_caption);
                var contentid = 'edit-catering-selection-fieldset-buffet-content0-value';
                CKEDITOR.instances[contentid].setData(response.buffetcontent);
                var buffetimage = response.buffet_image;
                $('#buffimgremove').val('');
                if (response.buffet_image != '') {
                    $("#buffimgpreview").html('<image src="' + response.buffetimagesrc + '" style="padding:14px 4px 0 0;width:200px"/><a style="vertical-align: top;padding:0px 4px" onclick="removeimage(' + buffetimage + ');"  href="#" class="button remove">X</a>');
                }
            }
            $('#transparentLoader').css('display', 'none');
            $('#addcoursediv').css('display', 'none');
            $('#updatecoursediv').css('display', 'block');
            $("#buffet_title").focus();
        },
        complete: function() {
            $('#transparentLoader').css('display', 'none');
        }
    });
}

/*   removes the image by hiding temperarly   */
function removeimage(buffetimage) {
    $('#buffimgpreview').css('display', 'none');
    $('#buffimgremove').val(buffetimage);
}


function buffetmenucancel() {
    $("#buffet_title").val('');
    $('#buffet_image').val('');
    $("#buffet_id").val('');
    $("#buffet_caption").val('');
    var contentid = 'edit-catering-selection-fieldset-buffet-content0-value';
    CKEDITOR.instances[contentid].setData('');
    $('#buffimgremove').val('');
    $("#buffimgpreview").html('');
    $('#addcoursediv').css('display', 'block');
    $('#updatecoursediv').css('display', 'none');
}