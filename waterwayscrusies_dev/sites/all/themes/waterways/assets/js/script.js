(function($) {

    Drupal.behaviors.dining_cruises = {
        attach: function(context, settings) {
            var baseUrl = Drupal.settings.basePath;
            $.validator.addMethod('custom', function(value, element) {
                return this.optional(element) ||/^[0-9]*$/.test(value);
            }, "Please enter a valid phone number");

             $.validator.addMethod('customphone', function(value, element) {
               return this.optional(element) || /^\(?\d{3}\)?[- ]?\d{3}[- ]?\d{4}$/.test(value);
            }, "Please enter a valid phone number");


            $("#requestQuoteForm").validate({
                rules: {
                    Other: {
                        required: true
                    },
                    quoteFirstName: {
                        required: true

                    },
                    quoteEmail: {
                        required: true,
                        email: true

                    },
                    quotePhoneNumber: {
                       // custom: true,
//                      required: true

                    },
                    quoteLastName: {
                        required: true

                    },
                    quoteconfirmEmail: {
                        required: true,
                        email: true,
                        equalTo: "#quoteEmail"

                    }
                },
                messages: {
                    name: "Required Field"
                }

            });
            $(".blogCommentform").validate({
                rules:{
                    bloguserEmail: {
                        required: true,
                        email: true
                    },
                    bloguserName: {
                        required: true   
                    },
                    blogtextcomments:{
                      required: true  
                    }
                    
       
                } 
            });
               $("#blogCommentform").validate({
                rules:{
                    bloguserEmail: {
                        required: true,
                        email: true
                    },
                    bloguserName: {
                        required: true   
                    },
                 
       
                } 
            });

            $("#corporateGivingform").validate({
                rules: {
                    donation: {
                        required: true
                    },
                    date: {
                        required: true
                    },
                    description: {
                        required: true
                    },
                    eventgoal: {
                        required: true
                    },
                    firstname: {
                        required: true
                    },
                    lastname: {
                        required: true
                    },
                    address: {
                        required: true
                    },
                    city:{
                        required: true
                    },
                    state:{
                        required: true
                    },
                    zipcode:{
                        required: true,
                        number:true
                    },
                    corporatestate:{
                        required: true
                    },
                    phoneNumber:{
                        required: true,
                        customphone: true
                    },
                    corporateEmail:{
                        required: true,
                        email: true
                    },
                    corporateConfirmemail:{
                        required: true,
                        equalTo: "#corporateEmail"
                    },
                    captcha:{
                        required: true,
                        equalTo: "#captchavalue"
                    }
                }

            });

            $('#feedbackform').validate({
                rules: {
                    feedbackFirstName: {
                        required: true
                    },
                    feedbackLastName: {
                        required: true
                    },
                    feedbackEmail: {
                        required: true,
                        email: true
                    },
                    feedbackConfirmemail: {
                        required: true,
                        email: true,
                        equalTo: "#feedbackEmail"
                    },
                    feedbackDescription: {
                        required: $(this).addClass('error'),
//                        minlength: 5,
//                        maxlength:400
                    }
                },
                messages: {
            }

            });



        }
    };

})(jQuery);
//testing 
