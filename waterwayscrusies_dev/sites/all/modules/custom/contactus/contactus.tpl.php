<?php
/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
?>

<script>

    $(document).ready(function() {

        $('.contactBox-list li .sliderBox').mouseover(function(e) {
//            $(this).children(".sliderBoxout").hide();
//            $(this).children(".sliderBoxover").show();
        });
        $('.contactBox-list li .sliderBox').mouseout(function(e) {
//            $(this).children(".sliderBoxover").hide();
//            $(this).children(".sliderBoxout").show();
        });

         //Billing prefix
	    $('#phonenumberfield1').keyup(function(e){
	    	if($(this).val().length==3){
	    		$('#phonenumberfield2').focus();
	    	}
    });
	    $('#phonenumberfield2').keyup(function(e){
	    	if($(this).val().length==3){
	    		$('#phonenumberfield3').focus();
	    	}
	    });


    });

</script>

<div class="container">
<div class="contentbox-inner contactus-content">

    <div class="row-fluid">
        <div class="span7">
            <div class="row-fluid">
                <div class="span12">
                    <div class="contactTitle">CONTACT US</div>
                </div>
            </div>
            <div class="row-fluid">
                <div class="span6">
                    <div class="contenttext-widget">
                        <div><b>Waterways Cruises and Events</b><br/>
                            <b>2441 N. Northlake Way,</b><br/>
                            <b>Seattle, WA 98103 </b><br/></div>
                    </div>
                    <div class="footer-address">
                        <b>
                             <div class="row-fluid"><div clss="span12"><img src="<?php echo base_path() . path_to_theme(); ?>/images/phone-footer-icon.png" /> 206-223-2060<br></div></div>
                             <div class="row-fluid"><div clss="span12"><img src="<?php echo base_path() . path_to_theme(); ?>/images/fax-footer-icon.png" /><div class="fax"> Fax: </div><div class="fax-number"> 206-223-2066</div></div></div>
                            <div class="row-fluid"><div clss="span12"><img src="<?php echo base_path() . path_to_theme(); ?>/images/email-footer-icon.png" class="pull-left"/> <a href="mailto:contact@waterwayscruises.com" class="emailLinkColor pull-left">contact@waterwayscruises.com</a> </div></div>
                        </b>  
                    </div>
                </div>
                <div class="span6 contactGroup">
                    <h3>Have a group of 15 or more?</h3>
                    <span>Helvetica aute dolor, ugh marfa et pug hashtag cillum four loko church-key brunch terry richardson.</span><br/><br/>
<!--                    <span class="fillForm">FILL OUT THE FORM</span>-->
<!--                    Adding a Request a proposal link-->
                    <span class="fillForm">
                         <a  data-toggle="modal" href="#myModalrequest">
                          FILL OUT THE FORM
                          </a>
                     </span>  
                </div>
            </div>

            <div class="formContainer">

                <div class="row-fluid">
                    <div class="span12">
                        <h3>For general inquires fill out the form below:</h3>
                    </div>
                </div>

                <div class="row-fluid">
                    <div class="span12">
                    <div class="span6">
                        <?php print drupal_render($form['first_name']); ?>
                    </div>
                    <div class="span6 lastnamecontactus">
                        <?php print drupal_render($form['last_name']); ?>
                    </div>
                </div>
                </div>

                <div class="row-fluid">
                     <div class="span12">
                    <div class="span6 mailcontactus">
                        <?php print drupal_render($form['mail']); ?>
                    </div>
                    <div class="span6 confirmmail">
                        <?php print drupal_render($form['confirmmail']); ?>
                    </div>
                </div>
                </div>

                <div class="row-fluid">
                    <div class="span12">
                        <div class="span6 phoneContainer">
                            <div class="phoneTitle">
                                <?php print $form['phone_number']['field1']['#title']; ?>
                            </div>
                            <?php print drupal_render($form['phone_number']); ?>
                        </div>
                        <div class="span6"></div>
                    </div>
                </div>
                <div>(555-555-5555)</div>
                <div class="row-fluid">
                    <div class="span12 comments-contactus">
                        <?php print drupal_render($form['comments']); ?>
                    </div>
                        
                    </div>

                <div class="row-fluid presssubmitContainer">
                    <div class="span4">
                        <div class="line"></div>
                    </div>
                    <div class="span4 booknow">
                        <div class="booknow-inner">
                            <?php print drupal_render($form['submit']); ?>
                        </div>
                    </div>
                    <div class="span4">
                        <div class="line"></div>
                    </div>
                </div>
            </div>
  <?php print drupal_render($form['form_id']); ?>
        <?php print drupal_render($form['form_token']); ?>
        </div>

      
        <div class="span5 tiles-content">
            <ul class="contactBox-list">
                <li>
                    <div class="sliderBox">
                        <div class="sliderBoxout">
                            <div class="sliderBoxInner">
                                <img src="<?php echo base_path(); ?>sites/all/themes/waterways/assets/img/innerbox-top-img.png"><br>
                                <div class="title">BOOK A CRUISE<br>
                                    <span style="font-size: 36px; margin-top: 10px; display: block;">Today</span>
                                </div>
                                <div class="booknowMain">
                                    <div class="booknow">
                                        <a href="<?php global $base_path; echo $base_path;?>wwccalendar">
                                            <div class="booknow-inner">Book Now!</div>
                                        </a>
                                            
                                    </div>
                                        
                                </div><br/>
                                <div class="line"></div>
                                    
                            </div>
                                
                        </div>
<!--                        <div class="sliderBoxover" style="display: none;">
                            <div class="sliderBox-first-inner">
                                <img src="<?php echo base_path(); ?>sites/all/themes/waterways/assets/img/innerbox-top-img.png"><br>
                                <br>Beard in lo-fi, raw denim ea fugiat organic consectetur. Gentrify duis proident.<br>
                                <a href="<?php echo base_path().'wwccalendar' ?>">
                                    Click to Learn More
                                </a>
                                <div class="line">
                                    
                                </div>
                                
                            </div>
                                
                        </div>-->
                            
                    </div>
                </li>
                <li>
                    <div class="sliderBox">
                        <div class="sliderBoxout">
                            <div class="sliderBoxInner">
                                <img src="<?php echo base_path(); ?>sites/all/themes/waterways/assets/img/innerbox-top-img.png"><br>
                                <div class="title">PLANNING A<br>
                                    <span style="font-size: 36px; display: block; margin-top: 10px;">Private Event?</span>
                                </div>
                                <div class="booknowMaincontactus">
                                    <div class="bookhere">
                                        
                                        <a  data-toggle="modal" href="#myModalrequest">
                                            <div class="booknow-request-proposal">Request a Proposal</div>
                                        </a>
                                            
                                    </div>
                                        
                                </div>
                                <br/>
                                <div class="line">
                                    
                                </div>
                                    
                            </div>
                                
                        </div>
<!--                        <div class="sliderBoxover" style="display: none;">
                            <div class="sliderBox-first-inner">
                                <img src="<?php echo base_path(); ?>sites/all/themes/waterways/assets/img/innerbox-top-img.png"><br>
                                <br>Beard in lo-fi, raw denim ea fugiat organic consectetur. Gentrify duis proident.<br>
                                <a href="#">Click to Learn More</a>
                                <div class="line">
                                    
                                </div>
                                    
                            </div>
                                
                        </div>-->
                            
                    </div>
                </li>
            </ul>
        </div>
    </div>
    <!-- row-fluid ends here -->

</div>
</div>