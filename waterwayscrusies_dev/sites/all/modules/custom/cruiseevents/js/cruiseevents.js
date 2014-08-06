(function($) {
    Drupal.behaviors.cruiseevents = {
        attach: function(context, settings) {

            //Showing the Days if the recurrence value is 1
            $('#recurrenceType', context).change(function() {
                var flag = $('#recurrenceType').val();
                if (flag == "1") {
                    $('#recurranceWeeklyDatediv').show();
                } else {
                    $('#recurranceWeeklyDatediv').hide();
                }
            });
            //recurrenceType ends here
        }
    };

}(jQuery));