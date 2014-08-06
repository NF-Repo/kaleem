<?php
/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
//print base_path();
//var_dump($nodeid);
?>

<script type="text/javascript" src="<?php echo base_path() . path_to_theme() ?>/js/sharebuttons.js"></script>
<script>
    $(function() {

        $("#shareBtn").on("click", function(){
            $("#stwrapper").show();
        });
        
         $('.reoutemapDirectionsclosebutton').click(function() {
            
            $('#directionsModal').modal('hide');
        });
        
    });
    
    
    
    
    
   

</script>
<style type="text/css" media="print">
          @media print{
             @page {
  size: auto;   /* auto is the initial value */
  margin: 10%;
}
      @page :first {
  margin-left: 2mm;
}
   body * { visibility: hidden; }
  .printconfirmationdiv * { visibility: visible; }
  .printconfirmationdiv { position: absolute; top:40px;}
  .confirmationContainerright * { display:none; }
  .confirmationContainerleft{width:100% !important; }
  .bookingTicketDetails{width:80% !important;}
  .printcss{width:80% !important;}
  .printcssfee{width:80% !important;}
  
   a{display: none ; }
    </style>

<div class="container confirmationWrapper">
    <div class="connectMain mobilebookingContainer">
        <!--connect title and sub menu start here--> 

        <div class="row-fluid menuRelative">
            <div class="span12">

                <div class="subMenu responsiveWeb">
                    <ul>
                        <li>TICKETS</li>
                        <li>ENHANCEMENTS</li>
                        <li>PAYMENT</li>
                        <li>REVIEW ORDER</li>
                        <li class="active">CONFIRMATION</li>
                    </ul>
                </div>

            </div>
        </div>
        
<!--        <div class="row-fluid shreBtnmargin responsiveWeb">
            <div class="span12">
                <div class="booknow share shareBtnWidth" id="shareBtn">
                    <span class="booknow-inner st_sharethis_custom" 
                          st_url="<?php // echo "http://" . $_SERVER["SERVER_NAME"] . $_SERVER['REQUEST_URI']; ?>" displayText="Share" >Share</span>
                </div>
            </div>
        </div>-->

        <!--main template confirmation data-->
        
<!--        <div class="row-fluid responsiveWeb">
            <div class="span8"><div class="ticketTitle">Confirmation</div></div>
            <div class="span4" align="right"><h5><img src="<?php // echo base_path().path_to_theme() ?>/images/fax-footer-icon.png" alt="Print" />&nbsp;&nbsp;<a href="#" onclick="window.print();">PRINT CONFIRMATION</a></h5></div>
        </div>-->
        
        <?php
           ///changes by Mohammed kalimulla April 7 mon 2014
        if (in_array('staff admin', $user->roles)) {
//          echo "staff admin";
          $printdisplay="none";
        }else if(in_array('agent',$user->roles)){
            $printdisplay="none";
        }else{
            $printdisplay="block";
        }
        ?>
        
        
         <div class="row-fluid responsiveWeb ">
            <div class="span8"><div class="ticketTitle">Confirmation</div></div>
            <div class="span4" align="right" style='display:<?php echo$printdisplay ?>;'><h5><img src="<?php echo base_path().path_to_theme() ?>/images/fax-footer-icon.png" alt="Print" />&nbsp;&nbsp;<a href="#" onclick="window.print();">PRINT CONFIRMATION</a></h5></div>
        </div>
        
        
        <!--main div adding which is used for printable-->
        <div class="printconfirmationdiv">
        
        
        
        
        <div class="row-fluid confirmationContainer">

            <!--side one-->
            <div class="span7 confirmationContainerleft">
                
                <div class="row-fluid">
                    <div class="span12">
                        <p class="bookingSubTitle">Thank you for using Waterways Cruises for your cruise experience!</p>
                        
                        
                        <h1 class="bookingheader1">Your Waterways Cruise <?php print $orderid;?></h1>
                       
                        
<!--                        <p>Please keep this number as your e-ticket for records only. <strong>You do not need to print your confirmation for boarding.</strong></p>-->
                        <p>Please keep this number as your e-ticket for records only.</p>
                        <div class="cruiseDetailsOuterbox">
                          
                          <!--cruise inner box design-->
                          <?php print $cruiseboxwidget;?>
                         <!--end of cruise inner box design-->
                          
                          
                          
                          
                        </div>
                    </div>
                </div>

            </div>
            <!--side one ends here-->


            <!--side two-->
            <div class="span5 confirmationContainerright">
                
                <div class="row-fluid">
                    <div class="span3"><img src="<?php echo base_path().path_to_theme() ?>/images/seattle-slu-logo.png" alt="Seattle" /></div>
                    <div class="span9 groupdirection">
                        <h5><?php echo $cruiseportname; ?></h5>
                        <p><?php echo $cruiseportaddrs; ?></p>
                        <!--<p> <a href="#directionsModal"  role="button" data-toggle="modal">GET DIRECTIONS</a></p>-->
                        <p><a href="<?php echo base_path().'portsdetailview/portid/'.$cruiseportid; ?>" target="_blank">GET DIRECTIONS</a></p>
                    </div>
                </div>
                
                <div class="row-fluid groupmarginTop">
                    <div class="span3"><img src="<?php echo base_path().path_to_theme() ?>/images/booking-parking-logo.png" alt="Seattle" /></div>
                    <div class="span9 groupdirection">
                        <h5>Parking</h5>
                        <p>Echo park iphone godard retro, ugh id neutrar.</p>
                        <?php 
                        if($parkingpdf!="javascript:void(0)"){
						echo '<p><a href="'.$parkingpdf.'" target="_blank">DOWNLOAD PDF</a></p>';
						}
                        ?>
                        
                    </div>
                </div>
                
                <div class="row-fluid groupmarginTop">
                    <div class="span3"><img src="<?php echo base_path().path_to_theme() ?>/images/booking-faq-logo.png" alt="Seattle" /></div>
                    <div class="span9 groupdirection">
                        <h5>FAQ's</h5>
                        <p>Echo park iphone godard retro, ugh id neutrar.</p>
                        <p><a href="<?php echo base_path() ?>faq-page" target="_blank">GO TO OUR FAQ SECTION </a></p>
                    </div>
                </div>
                
                <div class="row-fluid groupmarginTop">
                    <div class="span12 groupdirectiondesc">
                        <h5>Do you have any questions?</h5>
                        <p>Please contact a representative at</p>
                        <p><a href="mailto:reservations@waterwayscruises.com">reservations@waterwayscruises.com</a></p>
                        <p>or by calling (206) 223-2060</p>
                    </div>
                </div>
                
            </div>
            <!--side two ends here -->           
            
        </div>
        <!--end of main confirmation-->
        
        
        <!-- Quantity Ticket details widget -->
        
        
        
        
        <!-- Ticket details starts from here -->
        <div class="row-fluid">
            <div class="span12">
                <h1 class="bookingheader1 textmarginTop">Ticket Details</h1>
            </div>
        </div>
        
        <div class="row-fluid">
            <div class="span7 bookingTicketDetails">
                <!--Ticket table gets here-->
                <?php print $ticketswidget;?>
                <!--Ticket tebale ends here-->
            </div>
            <div class="span5 responsiveWeb"></div>
            
        </div>
        <!-- Ticket details ends here ->
        
        
        
        <!-- Enhancement details starts from here -->
        
<!--        <div class="row-fluid">
            <div class="span12">
                <h1 class="bookingheader1 textmarginTop">Enhancements</h1>
            </div>
        </div>
        
        <div class="row-fluid">
            <div class="span7 bookingTicketDetails">
                Ticket table gets here
                <?php print $enhancementwidget;?>
                Ticket tebale ends here
            </div>
            <div class="span5 responsiveWeb"></div>
            
        </div>-->
        <!-- Enhancements details ends here ->
          <?php
        
        $display ='display:block;';
        
        
        if(is_null($enhancementwidget)){
        $display ='display:none;';
        }
        
        ?>
        
        <!-- Enhancement details starts from here -->
        <div class="row-fluid" style="<?php echo $display; ?>">
            <div class="span12">
                <h1 class="bookingheader1 textmarginTop">Enhancements</h1>
            </div>
        </div>
       
     
        
  
        <div class="row-fluid" style="<?php echo $display; ?>">
            <div class="span7 bookingTicketDetails">
                <!--Ticket table gets here-->
                <?php print $enhancementwidget;?>
               
                <!--Ticket tebale ends here-->
            </div>
            <div class="span5 responsiveWeb"></div>
            
        </div>
        <!-- Enhancements details ends here ->

        <!-- price details starts from here -->
        <div class="row-fluid textmarginTop confirmPricingDetails">
          <?php print $subcharges;?>
<!--            <div class="span7">
                <div class="chargesWrapper bookingheader3">
                    <h3 class="chargesWrapperLeft bookingheader3">Voucher Codes:</h3><h4>&nbsp;&nbsp;94JQ-3F6N-GRF2R,RPGV-9LFR-YWYY</h4><span></span><h3 class="chargesWrapperRight bookingheader3">-$68.<sup>00</sup></h3>
                </div>
                <div class="chargesWrapper textmarginTop">
                    <h3 class="chargesWrapperLeft bookingheader3">Gratuity</h3><span></span><h3 class="chargesWrapperRight bookingheader3">$14.<sup>00</sup></h3>
                </div>
                <div class="chargesWrapper">
                    <h3 class="chargesWrapperLeft bookingheader3">Port Charge</h3><span></span><h3 class="chargesWrapperRight bookingheader3">$8.<sup>00</sup></h3>
                </div>
                <div class="chargesWrapper">
                    <h3 class="chargesWrapperLeft bookingheader3">Tax</h3><span></span><h3 class="chargesWrapperRight bookingheader3">$13.<sup>00</sup></h3>
                </div>
                <div class="chargesWrapper">
                    <h3 class="chargesWrapperLeft bookingheader3">Subtotal</h3><span></span><h3 class="chargesWrapperRight bookingheader3">$151.<sup>00</sup></h3>
                </div>
                <div class="row-fluid textmarginTop">
                    <div class="span12" align="right">
                        <span class="bookingheader1">Total</span>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="bookingheader1">$164.<sup>00</sup></span>
                    </div>
                </div>
            </div>-->
            <div class="span5 responsiveWeb"></div>
        </div>
        <!-- price details ends here ->
        
        
        <!-- Payment Details starts from here -->
        <div class="row-fluid confirmPricingDetails">
            <div class="span7 printcss">
                <h1 class="bookingheader1">Payment Details</h1>
                
                
                
                
                
     
                <div class="row-fluid textmarginTop">
                             <!--Guest Details--> 
                    <?php print $guestwidget;?>
                  <!--end guest details-->
                  
                  
                  <!--billing details-->
                  
                    <?php print $billingwidget;?>
                  <!--end of billing details-->
                </div>
                
                
                
                
                
                
                
                
                
                <div class="row-fluid textmarginTop">
                  
                  <!--credit card details-->
                    <?php print $creditwidget;?>
                  <!--end of credit card details-->
                  
                  
                  
                  
                    <div class="span6 responsiveWeb"></div>
                </div>
                <p class="paymentdescription groupmarginTop"><i>*Please note that your tickets are non-refundable and non-transferable. If you give us an advance notice (72 hours or more prior to your cruise departure), we may be able to reschedule your tickets to another cruise date, based on availability. If you miss your cruise, we will not be able to issue a refund or reschedule your reservation. Your entree choice(dinner cruises only) may be changed with an advance notice only (72 hours or more prior to your cruise departure). Tickets for Special Events and Holiday Cruises can not be refunded or rescheduled.</i></p>
            </div>
            <div class="span5 responsiveWeb"></div>
        </div>
        <!-- Payment Details ends here -->
        
        
    </div><!--div section for printable div end case-->
        
        
        
        
        

    </div>



</div>
    
    
<!-- container end here -->

<div class="modal hide" tabindex="-1" role="dialog" aria-hidden="true" id="directionsModal">
    <div class="reoutemapDirectionsbox">
        <div class="reoutemapDirectionsboxInner">

            <div class="row-fluid">

                <div class="span12 reoutemapDirectionsboxInnerright">
                    <!--close button for modal popup-->
                    <div class="reoutemapDirectionsclosebutton">
                        <div class="reoutemapDirectionsclosebuttoninner">Close</div>
                    </div>
                    <!-- end of close button for modal popup-->

                    <div class="row-fluid">
                        <div class="span12 routemapContent" id="directionsContentRender">
                            <div class="row-fluid" id="directionsContentHeader">
                                <div class="span4 sunsetLinePadding"><div class="line"></div></div>
                                <div class="span4"><div class="sliderCaption">Directions</div></div>
                                <div class="span4 sunsetLinePadding"><div class="line"></div></div>
                            </div>
                            <?php print $directions ?>
                            <div class="direction-image">  <?php print $parkingImg ?></div>
                        </div>
                    </div>
                </div>
            </div>



        </div><!-- sunsetDownsliderInner end -->
    </div>
</div>
<!-- Route Map Modal PopUp ends here -->